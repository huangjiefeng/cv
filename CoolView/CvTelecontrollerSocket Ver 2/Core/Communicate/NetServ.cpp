/*
 * 遥控器与终端通信服务器端管理类的类定义文件
 * V1.3.1.0 2012-05-02  1、根据CNetManager类的调整而做了相应变更
 *                      2、增加对IPv6连接的支持
 * V1.3.0.0 2012-02-28  使用NetManager模块进行网络管理，使得本模块代码量得以缩减，仅专注于业务逻辑
 * V1.2.0.0 2011-12-09  使用CommonThread和SimpleMemPool模块做资源的辅助管理，调用消息处理的代码完成 By Lzy
 * V1.1.0.0 2011-11-24  调整内部实现 By Lzy
 * V0.0.0.0 2011-02-23  创建文件 By Lzy
 */

#include "NetServ.h"
#include <iostream>
#include "PMessageProcess.h"
#include "OMessageProcess.h"

using namespace thread_operation;
using namespace simple_mem_pool;

CNetServ *CNetServ::m_pNetServInstance = NULL;

/*
 * 【私有】构造函数，决定开放给遥控器的监听端口及连接方式
 * @param in unsigned int uCommandPort,一般消息监听端口
 * @param in EConnectTypeEnum eCommandConnectType,一般消息处理支持的连接方式
 * @param in unsigned int uNootifyPort,被动信息订阅端口，用于监听遥控器订阅被动消息（如终端发送的状态消息或其他遥控器发来的某些请求消息）的请求，端口号不能与ListenerPort相同
 * @param in unsigned int uNotifyListLen,通告暂存队列最大长度（有效范围[10-5000]，越界自动修正）
 */
CNetServ::CNetServ(unsigned int uCommandPort,EConnectTypeEnum eCommandConnectType,unsigned int uNotifyPort,unsigned int uNotifyListLen)
{
    int nWarnCount = 0;
    //1、内部变量赋初值
    printf("[I]Initializing(1/6):Base Param Setting\n");
    this->m_oNotifyListenerV4ID.m_nPID = -1;
    this->m_oNotifyListenerV6ID.m_nPID = -1;
    this->m_bThreadEnd = false;
    this->m_bNotifySendThreadIsRunning = false;
    //2、被动消息发送队列初始化
    printf("[I]Initializing(2/6):Notify List Initializing\n");
    this->m_nNotifyListLen = uNotifyListLen > 5000 ? 5000 : (uNotifyListLen < 10 ? 10 : uNotifyListLen);
    this->m_pNotifyList = (SNotifyNode*)malloc(sizeof(SNotifyNode)*this->m_nNotifyListLen);
    if (this->m_pNotifyList == NULL)
    {
        printf("[E]Notify List Initialize Failed\n");
        return;
    }
    this->m_nNotifyListReadIndex = 0;
    this->m_nNotifyListWriteIndex = 0;
    this->m_pNotifyListLock = CreateCS();//初始化通告信息管理相关操作临界锁
    //3、网络管理总体初始化
    printf("[I]Initializing(3/6):Network Initialing\n");
    this->m_pNetManagerInstance = CNetManager::GetInstance();
    if (this->m_pNetManagerInstance == NULL)
    {
        printf("[E]Network Initialize Failed\n");
        return;
    }
    //4、创建遥控器主动消息处理的TCP连接
    printf("[I]Initializing(4/6):Creating Command TCP Listener If Necessary\n");
    if (eCommandConnectType == CNetServ::Type_Tcp || eCommandConnectType == CNetServ::Type_Both)
    {
        this->m_oCommandListenerV4TCPID = this->m_pNetManagerInstance->CreateTCPServ(NULL,Version4,uCommandPort,CNetServ::_CommandListenerTCPAcceptFunc,this->m_pNetManagerInstance);
        if (this->m_oCommandListenerV4TCPID.m_nPID < 0)
        {
            printf("[W]Create Command IPv4 TCP Listener Failed\n");
            ++nWarnCount;
        }
        this->m_oCommandListenerV6TCPID = this->m_pNetManagerInstance->CreateTCPServ(NULL,Version6,uCommandPort,CNetServ::_CommandListenerTCPAcceptFunc,this->m_pNetManagerInstance);
        if (this->m_oCommandListenerV6TCPID.m_nPID < 0)
        {
            printf("[W]Create Command IPv6 TCP Listener Failed\n");
            ++nWarnCount;
        }
        if (nWarnCount == 2)
        {
            printf("[E]All Command TCP Listener Had Failed To Create\n");
            return;
        }
    }
    //5、创建遥控器主动消息处理的UDP连接
    printf("[I]Initializing(5/6):Creating Command UDP Listener If Necessary\n");
    if (eCommandConnectType == CNetServ::Type_Udp || eCommandConnectType == CNetServ::Type_Both)
    {
        this->m_oCommandListenerV4UDPID = this->m_pNetManagerInstance->CreateUDPServ(NULL,Version4,uCommandPort,NET_THREAD,_CommandClientRecvFunc,NULL,this->m_pNetManagerInstance);
        if (this->m_oCommandListenerV4UDPID.m_nPID < 0)
        {
            printf("[W]Create Command IPv4 UDP Listener Failed\n");
            nWarnCount+=4;
        }
        this->m_oCommandListenerV6UDPID = this->m_pNetManagerInstance->CreateUDPServ(NULL,Version6,uCommandPort,NET_THREAD,_CommandClientRecvFunc,NULL,this->m_pNetManagerInstance);
        if (this->m_oCommandListenerV6UDPID.m_nPID < 0)
        {
            printf("[W]Create Command IPv6 UDP Listener Failed\n");
            nWarnCount+=4;
        }
        if (nWarnCount >= 8)
        {
            printf("[E]All Command UDP Listener Had Failed To Create\n");
            return;
        }
    }
    //6、创建遥控器被动消息（通告消息）处理的TCP连接
    printf("[I]Initializing(6/6):Creating Notify TCP Listener\n");
    this->m_oNotifyListenerV4ID = this->m_pNetManagerInstance->CreateTCPServ(NULL,Version4,uNotifyPort,_NotifyListenerTCPAcceptFunc,this->m_pNetManagerInstance);
    if (this->m_oNotifyListenerV4ID.m_nPID < 0)
    {
        printf("[W]Create Notify IPv4 TCP Listener Failed\n");
        nWarnCount+=16;
    }
    this->m_oNotifyListenerV6ID = this->m_pNetManagerInstance->CreateTCPServ(NULL,Version6,uNotifyPort,_NotifyListenerTCPAcceptFunc,this->m_pNetManagerInstance);
    if (this->m_oNotifyListenerV6ID.m_nPID < 0)
    {
        printf("[W]Create Notify IPv6 TCP Listener Failed\n");
        nWarnCount+=16;
    }
    if (nWarnCount >= 32)
    {
        printf("[E]All Notify TCP Listener Had Failed To Create\n");
        return;
    }
    CreateThreadManager(_NotifySendTCPThread,true,this);//启动被动消息（通告消息）发送线程
    //8、结束初始化
    if (nWarnCount == 0)
        printf("[I]All Initialize Success\n");
    else
        printf("[I]Initialize Success With Some Warning\n");
}

/*
 * 【私有】析构函数
 */
CNetServ::~CNetServ(void)
{
    //1、通知所有线程结束
    printf("[I]Destructing(1/3):Waiting Notify Send Thread Close\n");
    this->m_bThreadEnd = true;
    while (this->m_bNotifySendThreadIsRunning == true)
        Sleep(5);
    //2、释放网络资源
    printf("[I]Destructing(2/3):Releasing Network\n");
    CNetManager::ReleaseInstance();
    //3、释放通告消息管理队列
    printf("[I]Destructing(3/3):Releasing Notify List\n");
    if (this->m_pNotifyList != NULL)
    {
        EnterCS(this->m_pNotifyListLock);
        free(this->m_pNotifyList);
        ReleaseCS(this->m_pNotifyListLock);
    }
    printf("[I]All Destruct Success\n");
}

/*
 * 【私有】【静态】【回调函数】TCP主动通信监听到新的连接发起时的回调函数
 * @param in SNodeID oClientID,新连接的连接索引号
 * @param in SNodeID oServID,收到这个新连接的服务器端索引号
 * @param in void *pNetManager,CNetManager的实例指针
 * @return bool,接受新连接的连接请求返回true，否则返回false
 */
bool CNetServ::_CommandListenerTCPAcceptFunc(SNodeID oClientID,SNodeID oServID,void *pNetManager)
{
    if (((CNetManager *)pNetManager)->ManageClient(oClientID,oServID,NET_SYSTEM_RELATIVE,_CommandClientRecvFunc,NULL,pNetManager) != 0)
        return true;
    else
        return false;
}

/*
 * 【私有】连接ID转换函数，转换后的值可直接用于网络传输
 * @addition info,转换基于int类型长度不少于32位这个假设，且当nClientPID的值大于16位时函数将转换失败
 * @param in SNodeID &rClientID,连接的标识
 * @param in unsigned char nExternAdd,要附加到转换结果上的值
 * @param out int &rResult,存放转换后的结果值
 * @return bool,转换成功返回true，否则返回false
 */
bool CNetServ::_IDParse(SNodeID &rClientID,unsigned char nExternAdd,int &rResult)
{
    if (rClientID.m_nPID > 65535)
        return false;
    rResult = rClientID.m_nPID;
    rResult += ((int)(rClientID.m_chUID)) << 16;
    rResult += ((int)(nExternAdd)) << 24;
    return true;
}

/*
 * 【私有】ID逆转换函数
 * @param in int nUnParseID,要逆转换的值
 * @param out SNodeID &rClientID,存放逆转换后的连接标识信息
 * @param out unsigned char &rExternAdd,存放逆转换后的附加结果值
 * @return void
 */
void CNetServ::_IDUnParse(int nUnParseID,SNodeID &rClientID,unsigned char &rExternAdd)
{
    rClientID.m_nPID = nUnParseID & 65535;
    rClientID.m_chUID = (nUnParseID >> 16) & 255;
    rExternAdd = (nUnParseID >> 24) & 255;
}

/*
 * 【私有】【静态】【回调函数】主动通信中接收到消息时的回调函数
 * @param in SNodeID oClientID,发来消息的连接的连接索引号
 * @param in SNodeID oServID,发来消息的连接所属的服务器端索引号
 * @param in ENUM_Protocol eProtocol,指明连接类型（TCP_Protocol,UDP_Protocol）
 * @param in const char *pData,收到的数据
 * @param in int nDataLen,收到的数据的长度，其值>0
 * @param in void *pNetManager,CNetManager的实例指针
 * @return void
 */
void CNetServ::_CommandClientRecvFunc(SNodeID oClientID,SNodeID oServID,ENUM_Protocol eProtocol,const char *pData,int nDataLen,void *pNetManager)
{
    int nIdentify;//存放连接标识，当需要向此连接发送返回数据时会用到
    bool bParseResult = false;//记录转换ID的结果
    CNetServ *pNetServ = CNetServ::GetInstance();
    if (eProtocol == TCP_Protocol)
    {
        printf("[I]New Command TCP Connection Create,ID = %d\n",oClientID.m_nPID);
        if (oServID == pNetServ->m_oCommandListenerV4TCPID)
            bParseResult = pNetServ->_IDParse(oClientID,1,nIdentify);//附加值含义：Bxxxx xxx1 = TCP,Bxxxx xx0x = IPv4
        else if (oServID == pNetServ->m_oCommandListenerV6TCPID)
            bParseResult = pNetServ->_IDParse(oClientID,3,nIdentify);//附加值含义：Bxxxx xxx1 = TCP,Bxxxx xx1x = IPv6
    }
    else
    {
        printf("[I]New Command UDP Connection Create,ID = %d\n",oClientID.m_nPID);
        if (oClientID == pNetServ->m_oCommandListenerV4UDPID)
            bParseResult = pNetServ->_IDParse(oClientID,0,nIdentify);//附加值含义：Bxxxx xxx0 = UDP,Bxxxx xx0x = IPv4
        else if (oClientID == pNetServ->m_oCommandListenerV6UDPID)
            bParseResult = pNetServ->_IDParse(oClientID,2,nIdentify);//附加值含义：Bxxxx xxx0 = UDP,Bxxxx xx1x = IPv6
    }
    if (bParseResult == false)
    {
        printf("[E]ID Parse Error,Command TCP Connection ID = %d\n",oClientID.m_nPID);
        return;
    }
#ifdef _DEBUG
    ((char *)pData)[nDataLen] = '\0';
#endif
    CPMessageProcess oMessageProcess(CNetServ::m_pNetServInstance,nIdentify);
    oMessageProcess.MainProcess(pData,nDataLen);//处理主动请求消息
    if (eProtocol == TCP_Protocol)
        ((CNetManager *)pNetManager)->CloseNet(oClientID,oServID,false);//关闭连接
}

/*
 * 【公开】对主动请求消息发送返回的数据
 * @param in const char *pData,发送的消息内容
 * @param in int nLen,发送的消息的长度
 * @param in int nSockResIndex,连接标识号
 * @return int,返回发送的字节数，出现错误返回SOCKET_ERROR 或 0
 */
int CNetServ::CommandSendBack(const char *pData,int nLen,int nSockResIndex)
{
    //把标识号转换成连接索引号
    SNodeID oServID,oClientID;
    this->_IDUnParse(nSockResIndex,oClientID,oServID.m_chUID);
    if ((oServID.m_chUID & 1) == 1)//TCP连接 附加值含义：Bxxxx xxx1 = TCP
    {
        if ((oServID.m_chUID & 2) == 0)//IPv4 附加值含义：Bxxxx xx0x = IPv4
            oServID = this->m_oCommandListenerV4TCPID;
        else//IPv6 附加值含义：Bxxxx xx1x = IPv6
            oServID = this->m_oCommandListenerV6TCPID;
    }
    else//UDP连接 附加值含义：Bxxxx xxx0 = UDP
        oServID.m_nPID = SERV_NONE;
    //发送返回数据
    return this->m_pNetManagerInstance->SendNet(oClientID,oServID,pData,nLen);
}

/*
 * 【私有】【静态】【回调函数】TCP通告通信监听到新的连接发起时的回调函数
 * @param in SNodeID oClientID,新连接的连接索引号
 * @param in SNodeID oServID,收到这个新连接的服务器端索引号
 * @param in void *pNetManager,CNetManager的实例指针
 * @return bool,恒返回true
 */
bool CNetServ::_NotifyListenerTCPAcceptFunc(SNodeID oClientID,SNodeID oServID,void *pNetManager)
{
    printf("[I]New Notify TCP Connection Create,ID = %d\n",oClientID.m_nPID);
    ((CNetManager *)pNetManager)->ManageClient(oClientID,oServID,NET_WIN32_IOCP,NULL,_NotifyClientDisconnectFunc);//不托管连接，仅设置断开回调函数
    CNetServ *pNetServ = CNetServ::GetInstance();
    int nParseID;
    unsigned char chExternAdd = 0;
    if (oServID == pNetServ->m_oNotifyListenerV6ID)
        chExternAdd = 1;
    if (pNetServ->_IDParse(oClientID,chExternAdd,nParseID) == false)
    {
        printf("[E]Notify Listener Accept:ID Parse Error,ClientID = %d\n",oClientID.m_nPID);
        return false;
    }
    Notify_Process_Space::NewNotifyConnectionCreated(nParseID);//交给外部去对新通告连接做额外处理
    return true;
}

/*
 * 【私有】【静态】【回调函数】TCP通告通信连接断开时的回调函数
 * @param in SNodeID oClientID,断开的连接的连接索引号
 * @param in SNodeID oServID,断开的连接所属的服务器端索引号
 * @param in ENUM_Protocol eProtocol,指明连接类型（TCP_Protocol,UDP_Protocol）
 * @param in bool bError,指示当前的断开是异常断开(true)还是正常断开(false)
 * @param in void *,没用到的数据
 * @return void
 */
void CNetServ::_NotifyClientDisconnectFunc(SNodeID oClientID,SNodeID oServID,ENUM_Protocol eProtocol,bool bError,void *)
{
    int nParseID;
    unsigned char chExternAdd = 0;
    CNetServ *pNetServ = CNetServ::GetInstance();
    if (oServID == pNetServ->m_oNotifyListenerV6ID)
        chExternAdd = 1;
    if (pNetServ->_IDParse(oClientID,chExternAdd,nParseID) == false)
    {
        printf("[E]Notify Client Disconnect:ID Parse Error,ClientID = %d\n",oClientID.m_nPID);
        return;
    }
    Notify_Process_Space::NotifyConnectionClosed(nParseID);//交给外部去对已无效的通告连接做额外处理
}

/*
 * 【私有】【静态】【线程函数】向订阅了通告消息的遥控器发送相关信息用线程函数
 * @param in void *pParam,CNetServ的实例指针
 * @return void *,这里恒返回NULL
 */
void *CNetServ::_NotifySendTCPThread(void *pParam)
{
    CNetServ* pNetServ = (CNetServ *)(pParam);
    pNetServ->m_bNotifySendThreadIsRunning = true;
    SNodeID oClientID;
    unsigned char chExternAdd;
    while (pNetServ->m_bThreadEnd == false)
    {
        if (pNetServ->m_nNotifyListReadIndex != pNetServ->m_nNotifyListWriteIndex)//检查是否有通告可读
        {
            int nTmpRead = pNetServ->m_nNotifyListReadIndex;//用于暂存通告暂存队列的[读]索引
            const char *pToSendBuf = &(pNetServ->m_pNotifyList[nTmpRead].m_pBuffer[0]);//指向将要发送的缓冲区的指针
            int nToSendLen = pNetServ->m_pNotifyList[nTmpRead].m_nUsedSize;//将发送数据大小
            if (nToSendLen > 0)
            {
                pNetServ->_IDUnParse(pNetServ->m_pNotifyList[nTmpRead].m_nTargetIndex,oClientID,chExternAdd);
                if ((chExternAdd & 4) == 0)
                {
                    if ((chExternAdd & 1) == 0)
                        pNetServ->m_pNetManagerInstance->SendNet(oClientID,pNetServ->m_oNotifyListenerV4ID,pToSendBuf,nToSendLen);
                    else
                        pNetServ->m_pNetManagerInstance->SendNet(oClientID,pNetServ->m_oNotifyListenerV6ID,pToSendBuf,nToSendLen);
                }
                else
                {
                    oClientID.m_nPID = CLIENT_ALL;
                    pNetServ->m_pNetManagerInstance->SendNet(oClientID,pNetServ->m_oNotifyListenerV4ID,pToSendBuf,nToSendLen);
                    pNetServ->m_pNetManagerInstance->SendNet(oClientID,pNetServ->m_oNotifyListenerV6ID,pToSendBuf,nToSendLen);
                }
            }
            if (++nTmpRead >= pNetServ->m_nNotifyListLen)
                nTmpRead = 0;
            pNetServ->m_nNotifyListReadIndex = nTmpRead;
        }
        Sleep_Thread(50);
    }
    pNetServ->m_bNotifySendThreadIsRunning = false;
    return NULL;
}

/*
 * 【公开】添加要发送的通告消息（发送动作延后进行）
 * @param in char *pData,要添加的通告消息
 * @param in int nLen,通告消息长度
 * @param in int nTarget = NOTIFY_ALL,通告发送目标，NOTIFY_ALL表示发送给全部订阅者，其它值则为只发送给特定的一个订阅者
 * @return int,返回操作结果 0-正常结束 1——暂存队列已满 2——本类即将析构，本次执行取消 3——要发送数据长度值有问题 负数——要发送的数据超出当前缓冲区大小的值（差值）
 */
int CNetServ::AddNotify(const char *pData,int nLen,int nTarget)
{
    //nLen = nLen + 1;                                //加1是补上'\0'的值...zhenyi.lie 20110324
    int nRst = 0;
    if (TryEnterCS(this->m_pNotifyListLock,this->m_bThreadEnd) == false)
        return 2;
    int nTmpIndex = this->m_nNotifyListWriteIndex;
    if (++nTmpIndex >= this->m_nNotifyListLen)
        nTmpIndex = 0;
    if (nTmpIndex != this->m_nNotifyListReadIndex)
    {
        if (nLen < NOTIFY_BUFFER_SIZE && nLen > 0)
        {
            this->m_pNotifyList[this->m_nNotifyListWriteIndex].m_nUsedSize = nLen;
            this->m_pNotifyList[this->m_nNotifyListWriteIndex].m_nTargetIndex = nTarget;
            memcpy(this->m_pNotifyList[this->m_nNotifyListWriteIndex].m_pBuffer,pData,nLen);
            this->m_nNotifyListWriteIndex = nTmpIndex;
#ifdef _DEBUG
            printf("[D]Debug output in AddNotify->\nWriteIndex:%d\nData:%s\n",nTmpIndex,pData);
#endif
        }
        else if (nLen > NOTIFY_BUFFER_SIZE)
        {
            printf("[W]Data length is bigger than BUFFER_SIZE[%d]\n",NOTIFY_BUFFER_SIZE);
            nRst = NOTIFY_BUFFER_SIZE - nLen;
        }
        else
            nRst = 3;
    }
    else
        nRst = -1;
    ExitCS(this->m_pNotifyListLock);
    return nRst;
}
