/*
 * 网络管理类，在CommonSocket模块基础上对网络操作进行封装 类实现文件 By Lzy
 * 2012-05-09   V1.0.1.1    允许在连接托管的情况下使用同步数据接收操作
 * 2012-05-02   V1.0.1.0    1、修正IOCP管理下断开的连接未被清理的问题
 *                          2、加入连接识别代码，防止对已回收但再重用的连接操作
 *                          3、重新考虑了线程冲突问题
 * 2012-02-27   V1.0.0.0    初版完成，未包含linux的epoll部分
 * 2012-01-27   V0.0.0.0    创建文件
 */

#include "NetManager.h"

using namespace simple_mem_pool;
using namespace thread_operation;

CNetManager *CNetManager::m_pNetManager = NULL;

/*
 * 【公开】【静态】获取类实例的函数，如果尚未创建实例则自动创建
 * @return CNetManager *,返回类实例
 */
CNetManager *CNetManager::GetInstance()
{
    if (CNetManager::m_pNetManager == NULL)
        CNetManager::m_pNetManager = new CNetManager;
    return CNetManager::m_pNetManager;
}

/*
 * 【公开】【静态】释放类实例
 * @return void
 */
void CNetManager::ReleaseInstance()
{
    if (CNetManager::m_pNetManager != NULL)
    {
        delete CNetManager::m_pNetManager;
        CNetManager::m_pNetManager = NULL;
    }
}

/*
 * 【私有】【静态/回调函数】MemPool释放SServNode结构时调用的函数
 * @param in void *pObject,传入的释放对象的指针
 * @param in void *pCommonSocket,CCommonSocket类实例指针
 * @return void
 */
void CNetManager::_ForMemPoolReleaseServNode(void *pObject,void *pCommonSocket)
{
    SServNode *pServNode = (SServNode *)pObject;
    pServNode->m_bExit = true;
    ((CCommonSocket *)pCommonSocket)->CloseSocket(pServNode->m_oSocket);
    while (pServNode->m_nVisitingCount > 0)
        Sleep_Thread(5);
    ReleaseMemPool(&pServNode->m_oClientPool,pCommonSocket);
    ReleaseCS(pServNode->m_pClientPoolLock);
    ReleaseCS(pServNode->m_pClientLinkLock);
    ++pServNode->m_oIDO.m_chUID;
}

/*
 * 【私有】【静态/回调函数】MemPool释放SClientNode结构时调用的函数
 * @param in void *pObject,传入的释放对象的指针
 * @param in void *pCommonSocket,CCommonSocket类实例指针
 * @return void
 */
void CNetManager::_ForMemPoolReleaseClientNode(void *pObject,void *pCommonSocket)
{
    SClientNode *pClientNode = (SClientNode *)pObject;
    pClientNode->m_bExit = true;
    ((CCommonSocket *)pCommonSocket)->CloseSocket(pClientNode->m_oSocket);
    while (1)
    {
        while (pClientNode->m_nVisitingCount > 0)
            Sleep_Thread(5);
        EnterCS(pClientNode->m_pVisitingCountLock);
        if (pClientNode->m_nVisitingCount > 0)
            ExitCS(pClientNode->m_pVisitingCountLock);
        else
            break;
    }
    if (pClientNode->m_pfDisconnect != NULL && CNetManager::GetInstance()->m_bObjectRelease == false)
        pClientNode->m_pfDisconnect(pClientNode->m_oIDO,pClientNode->m_oServIDO,pClientNode->m_eProtocol,pClientNode->m_bErrorClose,pClientNode->m_pUser);
    ReleaseCS(pClientNode->m_pVisitingCountLock);
    ++pClientNode->m_oIDO.m_chUID;
}

/*
 * 【私有】【静态/线程函数】单个服务器端运行的线程函数
 * @param in void *pObject,线程管理的SServNode结构指针
 * @return void *,返回值恒为NULL
 */
void *CNetManager::_ForServNodeThread(void *pObject)
{
    SServNodeThreadParam *pServNodeThreadParam = (SServNodeThreadParam *)pObject;
    SServNode *pServNode = pServNodeThreadParam->m_pServNode;
    CNetManager *pNetManager = pServNodeThreadParam->m_pNetManager;
    ++pServNode->m_nVisitingCount;
    pServNodeThreadParam->m_bHasRead = true;
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();
    while (pServNode->m_bExit == false)
    {
        void *pNewSocket = pCommonSocket->AcceptSocket(pServNode->m_oSocket);
        if (pNewSocket != NULL)
        {
            SNodeID oIDO;
            //1、锁资源
            if (TryEnterCS(pServNode->m_pClientPoolLock,pServNode->m_bExit) == false)//>加内存锁
                break;
            //2、分配资源
            SClientNode *pNewClientNode = (SClientNode *)AllocMemory(pServNode->m_oClientPool,NULL,&(oIDO.m_nPID));
            if (pNewClientNode == NULL)
            {
                ExitCS(pServNode->m_pClientPoolLock);
                break;
            }
            //3、填充连接信息
            pNewClientNode->m_oSocket = pNewSocket;
            pNewClientNode->m_eManagerType = NET_NONE;
            pNewClientNode->m_nVisitingCount = 0;
            pNewClientNode->m_bExit = false;
            pNewClientNode->m_eProtocol = TCP_Protocol;
            pNewClientNode->m_oServIDO = pServNode->m_oIDO;
            pNewClientNode->m_oIDO.m_nPID = oIDO.m_nPID;
            ++pNewClientNode->m_oIDO.m_chUID;
            oIDO.m_chUID = pNewClientNode->m_oIDO.m_chUID;
            pNewClientNode->m_bErrorClose = false;
            pNewClientNode->m_pVisitingCountLock = CreateCS();
            pNewClientNode->m_pfDisconnect = NULL;
            ExitCS(pServNode->m_pClientPoolLock);//<解内存锁
            //>>链表锁定
            if (TryEnterCS(pServNode->m_pClientLinkLock,pNetManager->m_bObjectRelease) == false)
                break;
            pNewClientNode->m_pNext = (SClientNode *)pServNode->m_pHeadClientNode;
            if (pServNode->m_pHeadClientNode != NULL)
                ((SClientNode *)pServNode->m_pHeadClientNode)->m_pPre = pNewClientNode;
            pNewClientNode->m_pPre = NULL;
            pServNode->m_pHeadClientNode = pNewClientNode;
            pNewClientNode->m_bAddedLink = true;
            //<<链表解锁
            ExitCS(pServNode->m_pClientLinkLock);
            //4、调用回调
            if (pServNode->m_pfAccept != NULL)
            {
                if (pServNode->m_pfAccept(oIDO,pServNode->m_oIDO,pServNode->m_pUser) == false)
                    pNetManager->_CloseNodes(pServNode,pNewClientNode,false,true);
            }
        }
    }
    pNetManager->_CloseNodes(pServNode,NULL,true,true);
    --pServNode->m_nVisitingCount;
    return NULL;
}

/*
 * 【私有】【静态/线程函数】使用独立线程管理单个连接的托管方式时的线程函数
 * @param in void *pObject,要管理的SClientNode节点指针
 * @return void *,恒返回NULL
 */
void *CNetManager::_ForClientNodeThread(void *pObject)
{
    SClientNodeThreadParam *pClientNodeThreadParam = (SClientNodeThreadParam *)pObject;
    SServNode *pServNode = pClientNodeThreadParam->m_pServNode;
    SClientNode *pClientNode = pClientNodeThreadParam->m_pClientNode;
    CNetManager *pNetManager = pClientNodeThreadParam->m_pNetManager;
    pClientNodeThreadParam->m_bHasRead = true;
    if (TryEnterCS(pClientNode->m_pVisitingCountLock,pClientNode->m_bExit) == false)
        return NULL;
    ++pClientNode->m_nVisitingCount;
    ExitCS(pClientNode->m_pVisitingCountLock);
    char bBuf[NET_MANAGER_RECEIVE_BUFFER_SIZE];//接收数据缓冲区
    int nReceiveLen = 0;
    while (pClientNode->m_bExit == false)
    {
        if (pClientNode->m_eProtocol == TCP_Protocol)
            nReceiveLen = pNetManager->m_pCommonSocket->ReceiveSocket(pClientNode->m_oSocket,bBuf,NET_MANAGER_RECEIVE_BUFFER_SIZE);
        else if (pClientNode->m_eProtocol == UDP_Protocol)
            nReceiveLen = pNetManager->m_pCommonSocket->ReceiveSocket(pClientNode->m_oSocket,bBuf,NET_MANAGER_RECEIVE_BUFFER_SIZE,(sockaddr *)(&pClientNode->m_uAddr),&pClientNode->m_nAddrLen);
        else
            nReceiveLen = SOCKET_ERROR;
        if (nReceiveLen <= 0)
        {
            if (pClientNode->m_bIsUDPServ == false)//对于非UDP SERV类型，出错后即退出线程
            {
#ifdef _DEBUG
                printf ("[E]Thread Managed Client Receive Error Code=%d\n",WSAGetLastError());
#endif
                break;
            }
        }
        else if (pClientNode->m_pfRecv != NULL)
            pClientNode->m_pfRecv(pClientNode->m_oIDO,pClientNode->m_oServIDO,pClientNode->m_eProtocol,bBuf,nReceiveLen,pClientNode->m_pUser);
    }
    //回收连接节点
    if (nReceiveLen == SOCKET_ERROR)
        pClientNode->m_bErrorClose = true;
    pNetManager->_CloseNodes(pServNode,pClientNode,true,true);
    EnterCS(pClientNode->m_pVisitingCountLock);
    --pClientNode->m_nVisitingCount;
    ExitCS(pClientNode->m_pVisitingCountLock);
    return NULL;
}

/*
 * 【私有】【静态/线程函数】监测IOCP线程使用状况并创建IOCP线程的线程函数
 * @param in void *pObject,CNetManager实例对象指针
 * @return void *,返回值恒为NULL
 */
void *CNetManager::_ForIOCPThreadManageThread(void *pObject)
{
    CNetManager *pNetManager = (CNetManager *)pObject;
    while (pNetManager->m_bObjectRelease == false)
    {
        if (pNetManager->m_nCurCreatedIOCPThreadCount == pNetManager->m_nCurRunningIOCPThreadCount)
        {
            Sleep_Thread(100);
            if (pNetManager->m_nCurCreatedIOCPThreadCount == pNetManager->m_nCurRunningIOCPThreadCount)
            {
                if (pNetManager->m_nCurCreatedIOCPThreadCount + 1 <= MAX_IOCP_THREAD_COUNT || MAX_IOCP_THREAD_COUNT <= 0)
                {
                    if (TryEnterCS(pNetManager->m_pCurIOCPThreadCountVarLock,pNetManager->m_bObjectRelease) == true)
                    {
                        if (CreateThreadManager(_ForIOCPPoolThread,true,pNetManager) != NULL)//创建管理线程
                            ++pNetManager->m_nCurCreatedIOCPThreadCount;
                        ExitCS(pNetManager->m_pCurIOCPThreadCountVarLock);
                    }
                }
            }
        }
        Sleep_Thread(1000);
    }
    return NULL;
}

/*
 * 【私有】【静态/线程函数】IOCP线程池使用的线程函数
 * @param in void *pObject,CNetManager实例对象指针
 * @return void *,返回值恒为NULL
 */
void *CNetManager::_ForIOCPPoolThread(void *pObject)
{
    CNetManager *pNetManager = (CNetManager *)pObject;
    DWORD dwUnUsedVar1,dwUnUsedVar2;
    OVERLAPPED *pReceiveOverLapped;//存放接收到的OVERLAPPED结构
    char bBuf[NET_MANAGER_RECEIVE_BUFFER_SIZE];//数据接收缓冲区
    int nReceiveLen;//接收到的数据的长度
#ifdef _DEBUG
    printf("[I]IOCP Thread Created,Total Count = %d\n",pNetManager->m_nCurCreatedIOCPThreadCount);
#endif
    while (1)
    {
        if (GetQueuedCompletionStatus(pNetManager->m_hIOCP_Handle,&dwUnUsedVar1,&dwUnUsedVar2,&pReceiveOverLapped,INFINITE) == FALSE)
            continue;
        if (pReceiveOverLapped == NULL)
            break;
        SClientNode *pClientNode = (SClientNode *)pReceiveOverLapped;
        //操作线程数+1
        if (TryEnterCS(pNetManager->m_pCurIOCPThreadCountVarLock,pNetManager->m_bObjectRelease) == false)
            break;
        ++pNetManager->m_nCurRunningIOCPThreadCount;
        ExitCS(pNetManager->m_pCurIOCPThreadCountVarLock);
        //接收数据
        if (TryEnterCS(pClientNode->m_pVisitingCountLock,pClientNode->m_bExit) == true)
        {
            ++pClientNode->m_nVisitingCount;
            ExitCS(pClientNode->m_pVisitingCountLock);
            if (pClientNode->m_eProtocol == TCP_Protocol)
                nReceiveLen = pNetManager->m_pCommonSocket->ReceiveSocket(pClientNode->m_oSocket,bBuf,NET_MANAGER_RECEIVE_BUFFER_SIZE);
            else
                nReceiveLen = SOCKET_ERROR;//对于UDP连接将直接被拒绝
            //接收后的处理
            if (nReceiveLen > 0)
            {
                if (pClientNode->m_pfRecv != NULL)
                    pClientNode->m_pfRecv(pClientNode->m_oIDO,pClientNode->m_oServIDO,pClientNode->m_eProtocol,bBuf,nReceiveLen,pClientNode->m_pUser);
                //重新发起异步请求
                if (WSARecv(pNetManager->m_pCommonSocket->GetSocket(pClientNode->m_oSocket),&(pNetManager->m_oWSARecvZeroBuf),1,&(pNetManager->m_dwWSARecvReceiveLen),&(pNetManager->m_dwWSARecvFlag),&(pClientNode->m_OverLapped),NULL) == SOCKET_ERROR)//发起异步接收
                {
                    if (WSAGetLastError() != WSA_IO_PENDING)
                        nReceiveLen = SOCKET_ERROR;
                }
            }
            //错误处理
            if (nReceiveLen <= 0 && pClientNode->m_bAddedLink == true)
            {
                if (nReceiveLen == SOCKET_ERROR)
                {
                    pClientNode->m_bErrorClose = true;
#ifdef _DEBUG
            printf ("[E]IOCP Managed Client Receive Error Code=%d\n",WSAGetLastError());
#endif
                }
                if (pClientNode->m_oServIDO.m_nPID >= 0)
                {
                    SServNode *pServNode = (SServNode *)FindMemory(pNetManager->m_oServPool,pClientNode->m_oServIDO.m_nPID);
                    if (pServNode != NULL)
                        pNetManager->_CloseNodes(pServNode,pClientNode,true,true);
                }
                else
                    pNetManager->_CloseNodes(NULL,pClientNode,true,true);
            }
            EnterCS(pClientNode->m_pVisitingCountLock);
            --pClientNode->m_nVisitingCount;
            ExitCS(pClientNode->m_pVisitingCountLock);
        }
        //操作线程数-1
        EnterCS(pNetManager->m_pCurIOCPThreadCountVarLock);
        --pNetManager->m_nCurRunningIOCPThreadCount;
        ExitCS(pNetManager->m_pCurIOCPThreadCountVarLock);    
    }
    EnterCS(pNetManager->m_pCurIOCPThreadCountVarLock);
    --pNetManager->m_nCurCreatedIOCPThreadCount;
    ExitCS(pNetManager->m_pCurIOCPThreadCountVarLock);
#ifdef _DEBUG
    printf("[I]IOCP Thread Exit,Total Left = %d\n",pNetManager->m_nCurCreatedIOCPThreadCount);
#endif
    return NULL;
}

/*
 * 【私有】【静态/线程函数】处理要关闭或回收的节点的线程函数
 * @param in void *pObject,CNetManager对象指针
 * @return void *,恒返回NULL
 */
void *CNetManager::_ForNodeManage(void *pObject)
{
    CNetManager *pNetManager = (CNetManager *)pObject;
    int nNextReadIndex;
    while (pNetManager->m_bObjectRelease == false)
    {
        while (pNetManager->m_nWriteIndex != pNetManager->m_nReadIndex && pNetManager->m_bObjectRelease == false)
        {
            nNextReadIndex = pNetManager->m_nReadIndex;
            if (pNetManager->m_aRecycle[nNextReadIndex].m_pClientNode != NULL) //回收单个客户端节点
            {
                if (pNetManager->m_aRecycle[nNextReadIndex].m_pServNode != NULL)
                {
                    if (TryEnterCS(pNetManager->m_aRecycle[nNextReadIndex].m_pServNode->m_pClientPoolLock,pNetManager->m_bObjectRelease) == false)
                        return NULL;
                    RecycleMemory(pNetManager->m_aRecycle[nNextReadIndex].m_pServNode->m_oClientPool,pNetManager->m_aRecycle[nNextReadIndex].m_pClientNode,pNetManager->m_pCommonSocket);
                    ExitCS(pNetManager->m_aRecycle[nNextReadIndex].m_pServNode->m_pClientPoolLock);
                }
                else
                {
                    if (TryEnterCS(pNetManager->m_pClientPoolLock,pNetManager->m_bObjectRelease) == false)
                        return NULL;
                    RecycleMemory(pNetManager->m_oClientPool,pNetManager->m_aRecycle[nNextReadIndex].m_pClientNode,pNetManager->m_pCommonSocket);
                    ExitCS(pNetManager->m_pClientPoolLock);
                }
            }
            else //回收多个节点
            {
                if (pNetManager->m_aRecycle[nNextReadIndex].m_pServNode == NULL || (pNetManager->m_aRecycle[nNextReadIndex].m_pServNode != NULL && pNetManager->m_aRecycle[nNextReadIndex].m_bRecycleServNode == false))
                {
                    if (TryEnterCS(pNetManager->m_pClientPoolLock,pNetManager->m_bObjectRelease) == false)
                        return NULL;
                    RecycleMemory(pNetManager->m_oClientPool,NULL,pNetManager->m_pCommonSocket);
                    pNetManager->m_aRecycle[nNextReadIndex].m_pServNode->m_pHeadClientNode = NULL;
                    ExitCS(pNetManager->m_pClientPoolLock);
                }
                else
                {
                    if (TryEnterCS(pNetManager->m_pServPoolLock,pNetManager->m_bObjectRelease) == false)
                        return NULL;
                    RecycleMemory(pNetManager->m_oServPool,pNetManager->m_aRecycle[nNextReadIndex].m_pServNode,pNetManager->m_pCommonSocket);
                    ExitCS(pNetManager->m_pServPoolLock);
                }
            }
            ++nNextReadIndex;
            if (nNextReadIndex >= NET_RECYCLE_ARRAY_SIZE)
                nNextReadIndex = 0;
            pNetManager->m_nReadIndex = nNextReadIndex;
        }
        Sleep_Thread(100);
    }
    return NULL;
}

/*
 * 【私有】构造函数，初始化成员变量
 */
CNetManager::CNetManager()
{
    this->m_pCommonSocket = CCommonSocket::GetInstance();
    this->m_oServPool = CreateMemPool(sizeof(SServNode),NULL,_ForMemPoolReleaseServNode);
    this->m_pServPoolLock = CreateCS();
    this->m_pServLinkLock = CreateCS();
    this->m_pHeadServNode = NULL;
    this->m_oClientPool = CreateMemPool(sizeof(SClientNode),NULL,_ForMemPoolReleaseClientNode);
    this->m_pClientPoolLock = CreateCS();
    this->m_pClientLinkLock = CreateCS();
    this->m_pHeadClientNode = NULL;
    this->m_nReadIndex = 0;
    this->m_nWriteIndex = 0;
    this->m_pWriteLock = CreateCS();
    this->m_bObjectRelease = false;
    if (CreateThreadManager(_ForNodeManage,true,this) == NULL)
    {
        printf("[E]CNetManager Create Thread Failed\n");
        this->m_bObjectRelease = true;
    }
#ifdef WIN32
    this->m_hIOCP_Handle = NULL;
#endif
}

/*
 * 【私有】析构函数，释放成员对象
 */
CNetManager::~CNetManager()
{
    EnterCS(this->m_pWriteLock);
    this->m_bObjectRelease = true;
    EnterCS(this->m_pServLinkLock);
    EnterCS(this->m_pServPoolLock);
    ReleaseMemPool(&this->m_oServPool,this->m_pCommonSocket);
    ReleaseCS(this->m_pServPoolLock);
    ReleaseCS(this->m_pServLinkLock);
    EnterCS(this->m_pClientLinkLock);
    EnterCS(this->m_pClientPoolLock);
    ReleaseMemPool(&this->m_oClientPool,this->m_pCommonSocket);
    ReleaseCS(this->m_pClientPoolLock);
    ReleaseCS(this->m_pClientLinkLock);
#ifdef WIN32
    //IOCP清理
    if (this->m_hIOCP_Handle != NULL)
    {
        while (this->m_nCurCreatedIOCPThreadCount != 0)
        {
            PostQueuedCompletionStatus(this->m_hIOCP_Handle,0,NULL,NULL);
            Sleep_Thread(50);
        }
        ReleaseCS(this->m_pCurIOCPThreadCountVarLock);
        CloseHandle(this->m_hIOCP_Handle);
    }
#endif
    ReleaseCS(this->m_pWriteLock);
    CCommonSocket::ReleaseInstance();
}

/*
 * 【公开】创建TCP的服务器端
 * @info 服务器端所有接收的连接默认只做最基本的释放管理，如果没在本函数中设置有效的pfAccept函数处理这些新连接，等同于所有接收的连接的收发都无法处理
 * @param in const char *pszIP,服务器端监听IP地址，可设置为NULL，此时将根据IP版本使用默认的IP地址
 * @param in ENUM_IPVersion eIPVer,IP版本号
 * @param in unsigned int nPort,监听的端口
 * @param in Accept_FuncP pfAccept = NULL,当有新连接请求建立时的通知回调函数，如果设置了这个回调函数，则只有回调函数返回true时该连接请求才会被建立
 * @param in void *pUser = NULL,用户定义数据，该数据将传给通知回调函数，如果没设置该回调函数，则此参数的设置无效
 * @return SNodeID,返回的结构里m_nPID非负数表示服务器端的检索号，-1表示错误
 */
SNodeID CNetManager::CreateTCPServ(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,Accept_FuncP pfAccept,void *pUser)
{
    SNodeID oIDO;//存放创建的服务器端的标识数据
    oIDO.m_nPID = -1;
    //>内存池锁定
    if (TryEnterCS(this->m_pServPoolLock,this->m_bObjectRelease) == false)
        return oIDO;
    SServNode *pNewNode = (SServNode *)AllocMemory(this->m_oServPool,NULL,&(oIDO.m_nPID));
    if (pNewNode == NULL)
    {
        ExitCS(this->m_pServPoolLock);//<分支一内存池解锁
        oIDO.m_nPID = -1;
        return oIDO;
    }
    pNewNode->m_pVisitingCountLock = CreateCS();
    pNewNode->m_pClientPoolLock = CreateCS();
    pNewNode->m_pClientLinkLock = CreateCS();
    pNewNode->m_oClientPool = NULL;
    pNewNode->m_nVisitingCount = 0;
    pNewNode->m_oSocket = this->m_pCommonSocket->CreateSocket(eIPVer,TCP_Protocol);
    this->m_pCommonSocket->BindSocket(pNewNode->m_oSocket,pszIP,nPort);
    if (this->m_pCommonSocket->ListenSocket(pNewNode->m_oSocket) == SOCKET_ERROR)
        goto tag_OnTCPServErrRelease;
    pNewNode->m_oClientPool = CreateMemPool(sizeof(SClientNode),NULL,_ForMemPoolReleaseClientNode);
    pNewNode->m_pfAccept = pfAccept;
    pNewNode->m_pUser = pUser;
    pNewNode->m_bExit = false;
    pNewNode->m_bAddedLink = false;
    pNewNode->m_oIDO.m_nPID = oIDO.m_nPID;
    ++pNewNode->m_oIDO.m_chUID;
    oIDO.m_chUID = pNewNode->m_oIDO.m_chUID;
    pNewNode->m_pHeadClientNode = NULL;
    ExitCS(this->m_pServPoolLock);//<分支二内存池解锁
    //>>链表锁定
    if (TryEnterCS(this->m_pServLinkLock,this->m_bObjectRelease) == false)
        goto tag_OnTCPServErrReturn;
    pNewNode->m_pNext = this->m_pHeadServNode;
    this->m_pHeadServNode = pNewNode;
    pNewNode->m_bAddedLink = true;
    //<<链表解锁
    ExitCS(this->m_pServLinkLock);
    SServNodeThreadParam sServNodeThreadParam;
    sServNodeThreadParam.m_pServNode = pNewNode;
    sServNodeThreadParam.m_pNetManager = this;
    sServNodeThreadParam.m_bHasRead = false;
    if (CreateThreadManager(_ForServNodeThread,true,&sServNodeThreadParam) != NULL) //创建管理线程，创建失败时进入回收流程
    {
        while (sServNodeThreadParam.m_bHasRead == false)
            Sleep_Thread(5);
        return oIDO;
    }
    else
    {
        this->_CloseNodes(pNewNode,NULL,false,true);
        goto tag_OnTCPServErrReturn;
    }
tag_OnTCPServErrRelease:
    if (TryEnterCS(this->m_pServPoolLock,this->m_bObjectRelease) == true)//>>>内存池锁定
    {
        RecycleMemory(this->m_oServPool,pNewNode,this->m_pCommonSocket);
        ExitCS(this->m_pServPoolLock);//<<<内存池解锁
    }
tag_OnTCPServErrReturn:
    oIDO.m_nPID = -1;
    return oIDO;
}

/*
 * 【公开】创建客户端
 * @info 对于UDP客户端将无法进行托管操作，因此UDP客户端下的eManageType参数会被忽略
 * @param in const char *pszIP,对方的IP地址
 * @param in ENUM_IPVersion eIPVer,IP版本号
 * @param in unsigned int nPort,对方的端口号
 * @param in ENUM_Protocol eProtocol,连接方式（指使用的是TCP还是UDP）
 * @param in EManageType eManageType,连接使用的托管方式，如设置为NET_NONE，则为用户自行管理，用户可使用ManageClient函数把这些自行管理的连接转为托管管理
 * @param in Recv_FuncP pfRecv = NULL,接收到消息时的回调函数，如果该连接没被托管，则此回调函数不会被用到
 * @param in Disconnect_FuncP pfDisconnect = NULL,连接断开时的回调函数
 * @param in void *pUser = NULL,用户自定义数据，此数据会在接收回调函数或断开回调函数被调用时作为函数参数之一传送给回调函数
 * @return SNodeID,返回的结构里m_nPID非负数表示服务器端的检索号，-1表示错误
 */
SNodeID CNetManager::CreateClient(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,ENUM_Protocol eProtocol,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser)
{
    return this->_CreateClient(pszIP,eIPVer,nPort,eProtocol,false,eManageType,pfRecv,pfDisconnect,pUser);
}

/*
 * 【公开】创建UDP服务器端
 * @param in const char *pszIP,绑定的IP地址，设置为NULL则使用本地地址
 * @param in ENUM_IPVersion eIPVer,IP版本号
 * @param in unsigned int nPort,绑定的IP端口
 * @param in EManageType eManageType,托管类型
 * @param in Recv_FuncP pfRecv = NULL,接收到消息时的回调函数
 * @param in Disconnect_FuncP pfDisconnect = NULL,UDP服务器关闭时的回调函数
 * @param in void *pUser = NULL,用户自定义参数，在调用回调函数时会把此参数传过去
 * @return SNodeID,返回的结构里m_nPID非负数表示服务器端的检索号，-1表示错误
 */
SNodeID CNetManager::CreateUDPServ(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser)
{
    return this->_CreateClient(pszIP,eIPVer,nPort,UDP_Protocol,true,eManageType,pfRecv,pfDisconnect,pUser);
}

/*
 * 【私有】创建TCP客户端,UDP客户端或服务器端
 * @param in const char *pszIP,对方的IP地址
 * @param in ENUM_IPVersion eIPVer,IP版本号
 * @param in unsigned int nPort,对方的端口号
 * @param in ENUM_Protocol eProtocol,连接方式（指使用的是TCP还是UDP）
 * @param in bool bIsUDPServ,只有eProtocol是UDP_Protocol时此参数才有效，指定是否创建UDP的服务器端
 * @param in EManageType eManageType,连接使用的托管方式，如设置为NET_NONE，则为用户自行管理，用户可使用ManageClient函数把这些自行管理的连接转为托管管理
 * @param in Recv_FuncP pfRecv,接收到消息时的回调函数，如果该连接没被托管，则此回调函数不会被用到
 * @param in Disconnect_FuncP pfDisconnect,连接断开时的回调函数
 * @param in void *pUser,用户自定义数据，此数据会在接收回调函数或断开回调函数被调用时作为函数参数之一传送给回调函数
 * @return SNodeID,返回的结构里m_nPID非负数表示服务器端的检索号，-1表示错误
 */ 
SNodeID CNetManager::_CreateClient(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,ENUM_Protocol eProtocol,bool bIsUDPServ,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser)
{
    SNodeID oIDO;//存放创建的客户端的标识数据
    oIDO.m_nPID = -1;
    //>内存池锁定
    if (TryEnterCS(this->m_pClientPoolLock,this->m_bObjectRelease) == false)
        return oIDO;
    SClientNode *pNewNode = (SClientNode *)AllocMemory(this->m_oClientPool,NULL,&(oIDO.m_nPID));
    if (pNewNode == NULL)
    {
        ExitCS(this->m_pClientPoolLock);//<分支一内存池解锁
        return oIDO;
    }
    pNewNode->m_oSocket = this->m_pCommonSocket->CreateSocket(eIPVer,eProtocol);
    pNewNode->m_pVisitingCountLock = CreateCS();
    pNewNode->m_eManagerType = NET_NONE;
    pNewNode->m_bExit = false;
    pNewNode->m_bAddedLink = false;
    pNewNode->m_nVisitingCount = 0;
    pNewNode->m_oServIDO.m_nPID = SERV_NONE;
    pNewNode->m_oIDO.m_nPID = oIDO.m_nPID;
    ++pNewNode->m_oIDO.m_chUID;
    oIDO.m_chUID = pNewNode->m_oIDO.m_chUID;
    pNewNode->m_bErrorClose = false;
    pNewNode->m_eProtocol = eProtocol;
    ExitCS(this->m_pClientPoolLock);//<分支二内存池解锁
    if (eProtocol == TCP_Protocol)
    {
        if (this->m_pCommonSocket->ConnectSocket(pNewNode->m_oSocket,pszIP,nPort) == SOCKET_ERROR)
            goto tag_OnClientErrRelease;
    }
    else if (eProtocol == UDP_Protocol)
    {
        pNewNode->m_bIsUDPServ = bIsUDPServ;
        if (bIsUDPServ == false)
        {
            pNewNode->m_nAddrLen = this->m_pCommonSocket->GetSockAddr(eIPVer,pszIP,nPort,(sockaddr *)(&pNewNode->m_uAddr));
            if (pNewNode->m_nAddrLen == 0)
                goto tag_OnClientErrRelease;
            if (this->m_pCommonSocket->SendSocket(pNewNode->m_oSocket,NULL,0,(sockaddr *)(&pNewNode->m_uAddr),pNewNode->m_nAddrLen) == SOCKET_ERROR)
                goto tag_OnClientErrRelease;
        }
        else
        {
            pNewNode->m_nAddrLen = sizeof(pNewNode->m_uAddr);
            if (this->m_pCommonSocket->BindSocket(pNewNode->m_oSocket,pszIP,nPort) == SOCKET_ERROR)
                goto tag_OnClientErrRelease;
        }
    }
    //>>链表锁定
    if (TryEnterCS(this->m_pClientLinkLock,this->m_bObjectRelease) == false)
        goto tag_OnClientErrReturn;
    pNewNode->m_pNext = this->m_pHeadClientNode;
    if (this->m_pHeadClientNode != NULL)
        this->m_pHeadClientNode->m_pPre = pNewNode;
    pNewNode->m_pPre = NULL;
    this->m_pHeadClientNode = pNewNode;
    pNewNode->m_bAddedLink = true;
    //<<链表解锁
    ExitCS(this->m_pClientLinkLock);
    if (this->_ManageClient(NULL,pNewNode,eManageType,pfRecv,pfDisconnect,pUser) == 1)//写入托管信息，托管处理失败时进入回收流程
        return oIDO;
    else
    {
        this->_CloseNodes(NULL,pNewNode,false,true);
        goto tag_OnClientErrReturn;
    }
tag_OnClientErrRelease:
    //>>>加内存池锁
    if (TryEnterCS(this->m_pClientPoolLock,this->m_bObjectRelease) == true)
    {
        RecycleMemory(this->m_oClientPool,pNewNode,this->m_pCommonSocket);
        ExitCS(this->m_pClientPoolLock);//<<<解除内存池锁
    }
tag_OnClientErrReturn:
    oIDO.m_nPID = -1;
    return oIDO;
}

/*
 * 【私有】对特定范围的连接进行指定操作
 * @param in SNodeID oClientIDO,连接标识数据，里面的m_nPID可使用CLIENT_ALL表示在满足nServID下的所有连接或CLIENT_NONE表示与任何连接无关
 * @param in SNodeID oServIDO,连接所属的服务器端的标识数据，如果是通过CreateClient建立的连接，则里面的m_nPID字段填入SERV_NONE，或是使用SERV_ALL表示所有有效的服务器端
 * @param in int nOperationNum,操作编号，在头文件中由以“OP_”开头的预定义值定义
 * @param in void *pParam,操作参数，因不同操作而异
 * @return int,返回值因应操作不同而不同，但如果涉及到对多个连接的操作，返回值只表示最后一个操作的返回值
 */
int CNetManager::_RangeOperation(SNodeID oClientIDO,SNodeID oServIDO,int nOperationNum,void *pParam)
{
    //1、操作范围分析，生成位数据
    unsigned char bRange = 0;
    if (oClientIDO.m_nPID >= 0)
        bRange |= 3;//B 0000 0011
    else if (oClientIDO.m_nPID == CLIENT_ALL)
        bRange |= 1;//B 0000 0001
    else if (oClientIDO.m_nPID == CLIENT_NONE)
        bRange |= 2;//B 0000 0010
    if (oServIDO.m_nPID >= 0)
        bRange |= 12;//B 0000 1100
    else if (oServIDO.m_nPID == SERV_ALL)
        bRange |= 4;//B 0000 0100
    else if (oServIDO.m_nPID == SERV_NONE)
        bRange |= 8;//B 0000 1000
    //2、检查范围是否有效，这里针对OP_RECEIVE操作有特殊的检查项
    if ((bRange & 3) == 0 || (bRange & 12) == 0 || (nOperationNum == OP_RECEIVE && bRange != 15))
        return 0;
    //3.>服务器端链表锁定
    SServNode *pServView = NULL;
    if ((bRange & 12) != 8)
    {
        if (TryEnterCS(this->m_pServLinkLock,this->m_bObjectRelease) == false)
            return 0;
        if (oServIDO.m_nPID >= 0) //只针对单个节点操作
        {
            pServView = (SServNode *)FindMemory(this->m_oServPool,oServIDO.m_nPID);
            if (pServView == NULL || pServView->m_bAddedLink == false || oServIDO.m_chUID != pServView->m_oIDO.m_chUID) //检查节点是否已失效
            {
                ExitCS(this->m_pServLinkLock);
                return 0;
            }
        }
        else
            pServView = this->m_pHeadServNode;
    }
    //4、遍历指定范围的服务器端对象
    int nResult = 0;
    while (pServView != NULL || oServIDO.m_nPID == SERV_NONE)
    {
        SClientNode *pClientView = NULL;
        //4.1、>>A.服务器有效则锁定服务器关联链表 >>B.服务器端无效则锁定无关联客户端链表
        SServNode *pNextServView;
        if (pServView != NULL)
        {
            if (TryEnterCS(pServView->m_pClientLinkLock,pServView->m_bExit) == false)
                break;
            pNextServView = pServView->m_pNext;
            if (oClientIDO.m_nPID >= 0) //只针对单个节点操作
                pClientView = (SClientNode *)FindMemory(pServView->m_oClientPool,oClientIDO.m_nPID);
            else if (oClientIDO.m_nPID == CLIENT_ALL)
                pClientView = (SClientNode *)pServView->m_pHeadClientNode;
        }
        else
        {
            if (TryEnterCS(this->m_pClientLinkLock,this->m_bObjectRelease) == false)
                break;
            if (oClientIDO.m_nPID >= 0)
                pClientView = (SClientNode *)FindMemory(this->m_oClientPool,oClientIDO.m_nPID);
            else if (oClientIDO.m_nPID == CLIENT_ALL)
                pClientView = this->m_pHeadClientNode;
        }
        if (pClientView == NULL || pClientView->m_bAddedLink == false || (bRange == 15 && oClientIDO.m_chUID != pClientView->m_oIDO.m_chUID) ) //检查节点是否已失效
            pClientView = NULL;
        //4.2、OP_CLOSE操作专用，当要关闭的是一个线程池内的所有连接时，则在这里直接略过下面的客户端遍历
        bool bActiveCallBack = true;
        if (oClientIDO.m_nPID < 0 && nOperationNum == OP_CLOSE)
        {
            bActiveCallBack = *((bool *)pParam);
            if (oClientIDO.m_nPID == CLIENT_ALL)
            {
                this->_CloseNodes(pServView,NULL,bActiveCallBack,false);
                pClientView = NULL;
            }
            else if (oClientIDO.m_nPID == CLIENT_NONE && pServView != NULL)
            {
                this->_CloseNodes(pServView,NULL,bActiveCallBack,true);
                goto tag_NextServ;
            }
        }
        //4.3、遍历客户端
        while (pClientView != NULL)
        {
            SClientNode *pNextClientView = pClientView->m_pNext;
            //4.3.1、执行操作
            nResult = 0;
            switch (nOperationNum)
            {
            case OP_MANAGE:
            {
                SManageParam *pManageParam = (SManageParam *)pParam;
                nResult = this->_ManageClient(pServView,pClientView,pManageParam->m_eManageType,pManageParam->m_pfRecv,pManageParam->m_pfDisconnect,pManageParam->m_pUser);
                break;
            }
            case OP_SEND:
            {
                SSendReceiveParam *pSendParam = (SSendReceiveParam *)pParam;
                if (pClientView->m_eProtocol == TCP_Protocol)
                    nResult = this->m_pCommonSocket->SendSocket(pClientView->m_oSocket,pSendParam->m_pUnion.u_pData,pSendParam->m_nLen);
                else if (pClientView->m_eProtocol == UDP_Protocol)
                    nResult = this->m_pCommonSocket->SendSocket(pClientView->m_oSocket,pSendParam->m_pUnion.u_pData,pSendParam->m_nLen,(sockaddr *)(&pClientView->m_uAddr),pClientView->m_nAddrLen);
                else
                    break;
                if (nResult == SOCKET_ERROR && (pClientView->m_eProtocol != UDP_Protocol || pClientView->m_bIsUDPServ == false)) //发送数据异常，如果该连接不是UDP Serv，则让case直接转入OP_CLOSE操作
                {
                    pClientView->m_bErrorClose = true;
                    goto tag_OP_CLOSE;
                }
                break;
            }
            case OP_RECEIVE:
            {
                SSendReceiveParam *pReceiveParam = (SSendReceiveParam *)pParam;
                if (pClientView->m_eProtocol == TCP_Protocol)
                    nResult = this->m_pCommonSocket->ReceiveSocket(pClientView->m_oSocket,pReceiveParam->m_pUnion.u_pReceiveBuf,pReceiveParam->m_nLen);
                else if (pClientView->m_eProtocol == UDP_Protocol)
                    nResult = this->m_pCommonSocket->ReceiveSocket(pClientView->m_oSocket,pReceiveParam->m_pUnion.u_pReceiveBuf,pReceiveParam->m_nLen,(sockaddr *)(&pClientView->m_uAddr),&pClientView->m_nAddrLen);
                else
                    break;
                if (nResult == SOCKET_ERROR && (pClientView->m_eProtocol != UDP_Protocol || pClientView->m_bIsUDPServ == false)) //接收数据异常，让case直接转入OP_CLOSE操作去回收连接，并触发断开连接的回调函数
                {
                    pClientView->m_bErrorClose = true;
                    goto tag_OP_CLOSE;
                }
                break;
            }
            case OP_CLOSE:
tag_OP_CLOSE:
                this->_CloseNodes(pServView,pClientView,bActiveCallBack,true);
                break;
            }
            //4.3.2、移动指针
            if (oClientIDO.m_nPID >= 0)
                break;
            else
                pClientView = pNextClientView;
        }
        //4.4、>>A.服务器有效则解锁服务器关联链表 >>B.服务器端无效则解锁无关联客户端链表 | 移动指针
        if (pServView != NULL)
            ExitCS(pServView->m_pClientLinkLock);
        else
            ExitCS(this->m_pClientLinkLock);
tag_NextServ:
        if (oServIDO.m_nPID != SERV_ALL)
            break;
        else
            pServView = pNextServView;
    }
    //5、<服务器端链表及内存池锁定解锁
    if ((bRange & 12) != 8)
        ExitCS(this->m_pServLinkLock);
    return nResult;
}

/*
 * 【公开】托管指定的连接/连接组（也可用于对已托管的连接/连接组重新设置回调函数和用户自定义数据）
 * @info 1、本函数只能把处于NET_NONE状态的连接转为eManageType参数指定的其它非NET_NONE托管状态，如果是对一个非NET_NONE托管状态的连接调用本函数，则函数会忽略eManageType参数，仅更新连接的回调函数和用户自定义数据信息。
 *       2、如果托管的为多条连接，则只要有一条连接托管成功，均会返回成功。实际有多少连接要被处理，是由nClientID和nServID两个值共同决定的，如nClientID=CLIENT_UNDEFINE,nServID=0，则表示索引为0的服务器端下的所有连接均被托管
 *       3、NET_SYSTEM_RELATIVE托管方式在win下的表现分两种，对于tcp连接使用NET_WIN32_IOCP管理，对于udp则使用NET_THREAD管理
 *       4、对于未成功发送数据的UDP客户端，托管会导致因接收数据出错而关闭
 *       5、消息处理回调函数只有在该连接处于托管状态时才会触发，而连接断开回调函数在该连接未被托管的情况下如果连接是异常断开，则也会被触发
 * @param in SNodeID oClientIDO,连接识别结构，里面的m_nPID可使用CLIENT_ALL表示在满足nServID下的所有连接，而CLIENT_NONE值在此函数中无效果
 * @param in SNodeID oServIDO,连接所属的服务器端的识别结构，如果是通过CreateClient建立的连接，则结构里的m_nPID填入SERV_NONE，也可使用SERV_ALL表示所有有效的服务器端
 * @param in EManageType eManageType,托管方式，对于已处于托管状态的连接，此参数会被忽略
 * @param in Recv_FuncP pfRecv = NULL,接收到消息时的回调函数
 * @param in Disconnect_FuncP pfDisconnect = NULL,连接断开时的回调函数（在托管状态下，如果指定了回调函数，除在CloseNet函数中指定不触发回调而断开连接的情况外，其它情况下的断开都会触发回调函数）
 * @param in void *pUser = NULL,用户自定义的传给回调函数的数据指针
 * @return int,托管成功返回1，否则返回0，如果托管连接不止一个，则此返回值仅反映最后一个操作连接的托管结果
 */
int CNetManager::ManageClient(SNodeID oClientIDO,SNodeID oServIDO,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser)
{
    SManageParam sManageParam;
    sManageParam.m_eManageType = eManageType;
    sManageParam.m_pfRecv = pfRecv;
    sManageParam.m_pfDisconnect = pfDisconnect;
    sManageParam.m_pUser = pUser;
    return this->_RangeOperation(oClientIDO,oServIDO,OP_MANAGE,&sManageParam);
}

/*
 * 【私有】ManageClient的内部版，托管指定的连接（不做参数检查，互斥锁也必须外部调用时处理）
 * @info 1、NET_SYSTEM_RELATIVE托管方式在win下的表现分两种，对于tcp连接使用NET_WIN32_IOCP管理，对于udp则使用NET_THREAD管理
 *       2、udp不支持NET_WIN32_IOCP管理，使用此托管方式将返回0，不支持的原因是使用WSARecvFrom函数时必须提供足够大的缓冲区放接收的数据，否则iocp会
 *          在收到数据后给出错误号234的ERROR_MORE_DATA错误。而tcp则可以传入0大小缓冲区，后面iocp返回时用recv拿数据。这与udp数据在系统内部不缓存有
 *          关，不拿就会直接丢失，如果要让udp使用iocp管理，则必须每个udp服务器端附带一个专用的缓存区，造成额外的内存开销，最后还是放弃这种做法。
 * @param in SServNode *pServNode,要调整连接管理的服务器节点指针，没有关联的情况下填入NULL
 * @param in SClientNode *pClientNode,指定要调整的连接对应的结构指针，函数默认此参数是有效的，不做NULL检查
 * @param in EManageType eManageType,托管方式，对于已处于托管状态的连接，此参数会被忽略
 * @param in Recv_FuncP pfRecv = NULL,接收到消息时的回调函数
 * @param in Disconnect_FuncP pfDisconnect = NULL,连接断开时的回调函数（在托管状态下，如果指定了回调函数，除在CloseNet函数中指定不触发回调而断开连接的情况外，其它情况下的断开都会触发回调函数）
 * @param in void *pUser = NULL,用户自定义的传给回调函数的数据指针
 * @return int,托管成功返回1，否则返回0，如果托管连接不止一个，则此返回值仅反映最后一个操作连接的托管结果
 */
int CNetManager::_ManageClient(SServNode *pServNode,SClientNode *pClientNode,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser)
{
    pClientNode->m_pUser = pUser;
    pClientNode->m_pfRecv = pfRecv;
    pClientNode->m_pfDisconnect = pfDisconnect;
    if (pClientNode->m_eManagerType == NET_NONE)
    {
        if (eManageType == NET_THREAD || (eManageType == NET_SYSTEM_RELATIVE && pClientNode->m_eProtocol == UDP_Protocol))
        {
            SClientNodeThreadParam sClientNodeThreadParam;
            sClientNodeThreadParam.m_pServNode = pServNode;
            sClientNodeThreadParam.m_pClientNode = pClientNode;
            sClientNodeThreadParam.m_pNetManager = this;
            sClientNodeThreadParam.m_bHasRead = false;
            if (CreateThreadManager(_ForClientNodeThread,true,&sClientNodeThreadParam) == NULL)//创建管理线程
                return 0;
            while (sClientNodeThreadParam.m_bHasRead == false)
                Sleep_Thread(5);
        }
        else
        {
#ifdef WIN32
            if (eManageType == NET_SYSTEM_RELATIVE || (eManageType == NET_WIN32_IOCP && pClientNode->m_eProtocol == TCP_Protocol)) //仅对TCP采用此托管方式
            {
                if (this->m_hIOCP_Handle == NULL) //初始化IOCP
                {
                    this->m_nCurCreatedIOCPThreadCount = 0;
                    this->m_nCurRunningIOCPThreadCount = 0;
                    this->m_dwWSARecvFlag = 0;
                    this->m_oWSARecvZeroBuf.len = 0;
                    this->m_hIOCP_Handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
                    if (this->m_hIOCP_Handle == NULL)
                        return 0;
                    this->m_pCurIOCPThreadCountVarLock = CreateCS();
                    if (CreateThreadManager(_ForIOCPThreadManageThread,true,this) == NULL)//创建管理线程
                        return 0;
                }
                memset(&(pClientNode->m_OverLapped),0,sizeof(OVERLAPPED));
                if (CreateIoCompletionPort((HANDLE)this->m_pCommonSocket->GetSocket(pClientNode->m_oSocket),this->m_hIOCP_Handle,0,0) == NULL)
                    return 0;
                int nResult;
                if (pClientNode->m_eProtocol == TCP_Protocol) //发起异步接收
                    nResult = WSARecv(this->m_pCommonSocket->GetSocket(pClientNode->m_oSocket),&(this->m_oWSARecvZeroBuf),1,&(this->m_dwWSARecvReceiveLen),&(this->m_dwWSARecvFlag),&(pClientNode->m_OverLapped),NULL);
                //else 禁止对UDP使用IOCP方式管理
                //    nResult = WSARecvFrom(this->m_pCommonSocket->GetSocket(pClientNode->m_oSocket),&(this->m_oWSARecvZeroBuf),1,&(this->m_dwWSARecvReceiveLen),&(this->m_dwWSARecvFlag),(sockaddr *)(&(pClientNode->m_uAddr)),&(pClientNode->m_nAddrLen),&(pClientNode->m_OverLapped),NULL);
                if (nResult == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSA_IO_PENDING)
                        return 0;
                }
            }
#else
#ifdef LINUX
            if (eManageType == NET_SYSTEM_RELATIVE || eManageType == NET_LINUX_EPOLL)
            {
                ;
            }
#else
            return 0;
#endif
#endif
        }
        pClientNode->m_eManagerType = eManageType;
    }
    return 1;
}

/*
 * 【公开】断开指定的连接/连接组
 * @info 当需要关闭某服务器端下的所有连接，但不关闭服务器端，则nClientID应该设置为CLIENT_ALL，当需要连服务器端一起关闭时，nClientID应设置为CLIENT_NONE
 * @param in SNodeID oClientIDO,连接标识数据，里面的m_nPID字段可使用CLIENT_ALL表示在满足nServID下的所有连接或CLIENT_NONE表示与任何连接无关
 * @param in SNodeID oServIDO,连接所属的服务器端的标识数据，如果是通过CreateClient建立的连接，则数据里的m_nPID字段填入SERV_NONE，或是填入SERV_ALL表示所有有效的服务器端
 * @param in bool bActiveDisconnectCallBack,关闭连接/连接组时是否触发相应回调函数
 * @return void
 */
void CNetManager::CloseNet(SNodeID oClientIDO,SNodeID oServIDO,bool bActiveDisconnectCallBack)
{
    this->_RangeOperation(oClientIDO,oServIDO,OP_CLOSE,&bActiveDisconnectCallBack);
}

/*
 * 【公开】向指定的连接发送消息
 * @info 1、此函数对于UDP服务器端同样有效，如果该服务器端已经收到过某个地址发来的消息，则此发送操作的目标则为最近一次收到消息的发送方，否则将发送失败
 *       2、当发送时出现SOCKET_ERROR时，如果该连接不是作为UDP服务器端，则会被函数关闭并回收
 * @param in SNodeID oClientIDO,连接标识数据，里面的m_nPID字段可使用CLIENT_ALL表示在满足nServID下的所有连接，而对该字段设置CLIENT_NONE值在此函数中无效果
 * @param in SNodeID oServIDO,连接所属的服务器端的标识数据，如果是通过CreateClient建立的连接，则数据里的m_nPID填入SERV_NONE，也可使用SERV_ALL表示所有有效的服务器端
 * @param in const char *pData,要发送的数据所在的缓冲区
 * @param in nDataLen,要发送的字节数
 * @return int,返回实际发送长度，连接出现错误返回SOCKET_ERROR，其它错误返回0,如果要发送数据的连接不止一个，则此返回值仅反映最后一个操作连接的发送结果
 */
int CNetManager::SendNet(SNodeID oClientIDO,SNodeID oServIDO,const char *pData,int nDataLen)
{
    SSendReceiveParam sSendParam;
    sSendParam.m_pUnion.u_pData = pData;
    sSendParam.m_nLen = nDataLen;
    return this->_RangeOperation(oClientIDO,oServIDO,OP_SEND,&sSendParam);
}

/*
 * 【公开】从指定连接接收消息，如果该连接已设置为托管，此函数将执行失败
 * @info 1、对于已托管的所有连接，此函数不会接收任何数据，只是返回0
 *       2、不建议对UDP客户端使用此函数，对UDP客户端使用此函数会导致两种可能：当未成功使用该客户端发送数据前，函数会返回SOCKET_ERROR，同时连接会被关闭；
 *                                                                            当成功发送过数据后，因建立的本地连接端口是随机的，所以必须有指向这个随机端口的UDP数据到达才能使此函数返回
 *       3、当发生SOCKET_ERROR错误时，产生该错误的连接会被关闭并被系统回收
 *       4、此函数不支持对多个连接同时操作
 * @param in SNodeID oClientIDO,连接标识数据，里面的m_nPID字段不能使用CLIENT_ALL或CLIENT_NONE值，必须指定具体连接
 * @param in SNodeID oServIDO,连接所属的服务器端的标识数据，如果是通过CreateClient建立的连接，则结构里的m_nPID应填入SERV_NONE，此函数里该字段不支持使用SERV_ALL值
 * @param in char *pDataBuf,存放接收到的数据的缓冲区
 * @param in int nBufLen,pDataBuf指向缓冲区的大小（可存放的最大字节数）
 * @return int,返回实际接收到数据的长度，连接出现错误返回SOCKET_ERROR，其它错误返回0，如果连接处于托管状态，也返回0
 */
int CNetManager::ReceiveNet(SNodeID oClientIDO,SNodeID oServIDO,char *pDataBuf,int nBufLen)
{
    SSendReceiveParam sReceiveParam;
    sReceiveParam.m_pUnion.u_pReceiveBuf = pDataBuf;
    sReceiveParam.m_nLen = nBufLen;
    return this->_RangeOperation(oClientIDO,oServIDO,OP_RECEIVE,&sReceiveParam);
}

/*
 * 【私有】关闭服务器/客户端节点，把节点放入回收处理队列，并视乎参数组合决定是否触发关闭回调
 * @addition info,1、此函数根据指定的节点是否还挂在链表中来决定是否执行操作
 *                2、函数的操作分成节点从链表中断开和把节点放入处理队列，等处理线程去操作
 *                3、pServNode和pClientNode的组合效果为：
 *                   |  NULL   |   非NULL  |  关闭指定的无关联客户端
 *                   |  NULL   |    NULL   |  关闭所有无关联客户端
 *                   | 非NULL  |   非NULL  |  关闭指定的关联客户端
 *                   | 非NULL  |    NULL   |  关闭指定服务器端下所有关联客户端
 * @param in SServNode *pServNode,连接所关联的服务器节点指针，与pClientNode组合决定执行效果
 * @param in SClientNode *pClientNode,要关联的连接节点指针，与pServNode组合决定执行效果
 * @param bool bActiveCallBack,是否触发关闭回调函数
 * @param bool bRecycleServNode,针对pClientNode = NULL,pServNode非NULL的组合，指定是否回收Serv节点
 * @return void
 */
void CNetManager::_CloseNodes(SServNode *pServNode,SClientNode *pClientNode,bool bActiveCallBack,bool bRecycleServNode)
{
    if (pServNode != NULL)
    {
        if (TryEnterCS(pServNode->m_pClientLinkLock,pServNode->m_bExit) == false)
            return;
    }
    else
    {
        if (TryEnterCS(this->m_pClientLinkLock,this->m_bObjectRelease) == false)
            return;
    }
    if (pClientNode != NULL)
    {
        if (pClientNode->m_bAddedLink == false)
        {
            if (pServNode != NULL)
                ExitCS(pServNode->m_pClientLinkLock);
            else
                ExitCS(this->m_pClientLinkLock);;
            return;
        }
        if (pClientNode->m_pPre != NULL)
            pClientNode->m_pPre->m_pNext = pClientNode->m_pNext;
        else
        {
            if (pServNode != NULL)
                pServNode->m_pHeadClientNode = pClientNode->m_pNext;
            else
                this->m_pHeadClientNode = pClientNode->m_pNext;
        }
        if (pClientNode->m_pNext != NULL)
            pClientNode->m_pNext->m_pPre = pClientNode->m_pPre;
        pClientNode->m_bAddedLink = false;
        pClientNode->m_bExit = true;
        if (bActiveCallBack == false)
            pClientNode->m_pfDisconnect = NULL;
    }
    else
    {
        if (pServNode != NULL)
        {
            if (bRecycleServNode == true)
            {
                if (TryEnterCS(this->m_pServLinkLock,this->m_bObjectRelease) == false)
                {
                    ExitCS(pServNode->m_pClientLinkLock);
                    return;
                }
                if (pServNode->m_bAddedLink == false)
                {
                    ExitCS(pServNode->m_pClientLinkLock);
                    ExitCS(this->m_pServLinkLock);
                    return;
                }
                SServNode *pViewServ = this->m_pHeadServNode;
                if (pViewServ == pServNode)
                    this->m_pHeadServNode = pServNode->m_pNext;
                else
                {
                    while (pViewServ != NULL && pViewServ->m_pNext != pServNode)
                        pViewServ = pViewServ->m_pNext;
                    if (pViewServ != NULL)
                        pViewServ->m_pNext = pServNode->m_pNext;
                }
                pServNode->m_bAddedLink = false;
                pServNode->m_bExit = true;
                ExitCS(this->m_pServLinkLock);
            }
            pClientNode = (SClientNode *)pServNode->m_pHeadClientNode;
            pServNode->m_pHeadClientNode = NULL;
        }
        else
        {
            pClientNode = this->m_pHeadClientNode;
            this->m_pHeadClientNode = NULL;
        }
        while (pClientNode != NULL)
        {
            pClientNode->m_bAddedLink = false;
            pClientNode->m_bExit = true;
            if (bActiveCallBack == false)
                pClientNode->m_pfDisconnect = NULL;
            pClientNode = pClientNode->m_pNext;
        }
    }
    if (pServNode != NULL)
        ExitCS(pServNode->m_pClientLinkLock);
    else
        ExitCS(this->m_pClientLinkLock);
    //加入处理队列
    if (TryEnterCS(this->m_pWriteLock,this->m_bObjectRelease) == true)
    {
        int nNextWriteIndex = this->m_nWriteIndex + 1;
        if (nNextWriteIndex >= NET_RECYCLE_ARRAY_SIZE)
            nNextWriteIndex = 0;
        while (nNextWriteIndex == this->m_nReadIndex) //循环数组满格
            Sleep_Thread(5);
        this->m_aRecycle[this->m_nWriteIndex].m_pServNode = pServNode;
        this->m_aRecycle[this->m_nWriteIndex].m_pClientNode = pClientNode;
        this->m_aRecycle[this->m_nWriteIndex].m_bRecycleServNode = bRecycleServNode;
        this->m_nWriteIndex = nNextWriteIndex;
        ExitCS(this->m_pWriteLock);
    }
}
