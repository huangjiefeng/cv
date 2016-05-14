/*
 * 网络操作类定义文件 By Lzy
 * @info 本类部分函数非线程安全，这不仅仅指同一时间同样函数被调用可能出问题，也包括这堆函数中不同函数的调用，要获得线程安全的函数，需要定义MUL_THREAD宏或外部自行保证
 * 2012-02-27   V1.0.2.5    新增GetSocket和GetSockAddr两个公开函数
 * 2012-02-26   V1.0.2.4    1、调整UDP用的SendSocket和ReceiveSocket函数的输入参数
 *                          2、ConnectSocket函数增加对addrinfo结构的释放
 * 2012-02-03   V1.0.2.3    调整ConnectSocket和BindSocket两个函数的IP参数为const类型
 * 2011-12-14   V1.0.2.2    修正BindSocket函数中没有释放addrinfo结构而导致内存泄漏的问题
 * 2011-12-05   V1.0.2.1    使用CommonThread模块做本模块的多线程支持
 * 2011-11-25   V1.0.2.0    1、直接使用SimpleMemPool模块管理本模块的连接信息存储
 *                          2、把原来cpp中全局的NumCStr函数改成类成员函数_NumCStr
 *                          3、SockNode结构加上对IP版本的记录，同时删去连接和绑定两个函数中要传入IP版本的参数
 *                          4、完善部分注释
 * 2011-11-23   V1.0.1.0    修改原本的链表的存储方式为数组块，以减少内存碎片和缩短分配，释放的时间
 * 2011-11-22   V1.0.0.0    By Lzy
 * 2011-04-01   V0.0.0.0    创建文件 By Lzy
 */

#include "CommonSocket.h"

using namespace simple_mem_pool;

CCommonSocket *CCommonSocket::SocketClassP = NULL;

/*
 * 【私有】【静态】【线程池专用】为线程池初始化对象用
 * @param void *pObject,有效的对象指针
 * @param void *pNULL,在这里为一个空指针，仅配合函数定义而设，无需用到
 * @return void
 */
void CCommonSocket::ForMemPoolInitialSock(void *pObject,void *pNULL)
{
    SocketNode *pNode = (SocketNode *)pObject;
    pNode->SetUsing(false);
}

/*
 * 【私有】【静态】【线程池专用】为线程池释放对象用
 * @param void *pObject,有效的对象指针
 * @param void *pNULL,在这里为一个空指针，仅配合函数定义而设，无需用到
 * @return void
 */
void CCommonSocket::ForMemPoolReleaseSock(void *pObject,void *pNULL)
{
    SocketNode *pNode = (SocketNode *)pObject;
    if (pNode->GetUsing() != false)
    {
        CCommonSocket::_CloseSock(pNode->Socket);
        pNode->SetUsing(false);
    }
}

/*
 * 【私有】构造函数，针对Win操作系统有额外的处理需求
 */
CCommonSocket::CCommonSocket()
{
    this->InitSuccess = true;
#ifdef MUL_THREAD
    this->DestructObject = false;
    this->OpLock = CreateCS();
    if (this->OpLock == NULL)
    {
        this->InitSuccess = false;
        return;
    }
#endif
    this->MemPoolNode = CreateMemPool(sizeof(SocketNode),CCommonSocket::ForMemPoolInitialSock,CCommonSocket::ForMemPoolReleaseSock);
    if (this->MemPoolNode == NULL)
    {
        this->InitSuccess = false;
        return;
    }
#ifdef WIN32
    WSADATA wsaData;
    int rc = WSAStartup(MAKEWORD(2,2),&wsaData);
    if (rc!=0)
        this->InitSuccess = false;
#endif
}

/*
 * 【私有】析构函数，针对Win操作系统有额外的处理需求，需定义了MUL_THREAD后才是线程安全的
 */
CCommonSocket::~CCommonSocket()
{
#ifdef MUL_THREAD
    this->DestructObject = true;
    EnterCS(this->OpLock);
#endif
    ReleaseMemPool(&this->MemPoolNode);
#ifdef MUL_THREAD
    ReleaseCS(this->OpLock);
#endif
#ifdef WIN32
    if (this->InitSuccess)
        WSACleanup();
#endif
}

/*
 * 【公开】【静态】返回实例对象，如果实例未创建则创建一个，该实例对象只会创建一个
 * @return CCommonSocket*,返回新创建的实例对象，该实例对象只能用ReleaseInstance释放
 */
CCommonSocket *CCommonSocket::GetInstance()
{
    if (CCommonSocket::SocketClassP == NULL)
        CCommonSocket::SocketClassP = new CCommonSocket();
    return CCommonSocket::SocketClassP;
}

/*
 * 【公开】【静态】释放创建的实例对象
 * @return void
 */
void CCommonSocket::ReleaseInstance()
{
    if (CCommonSocket::SocketClassP != NULL)
    {
        delete CCommonSocket::SocketClassP;
        CCommonSocket::SocketClassP = NULL;
    }
}

/*
 * 【公开】创建新的Socket，需定义了MUL_THREAD后才是线程安全的
 * @param ENUM_IPVersion IP_Version,使用的IP版本
 * @param ENUM_Protocol Protocol,使用的协议
 * @return void*,返回该Socket的识别地址
 */
void *CCommonSocket::CreateSocket(ENUM_IPVersion IP_Version,ENUM_Protocol Protocol)
{
    if (this->InitSuccess == false)
        return NULL;
    int Domain,Type;
    if (IP_Version == Version4)
        Domain = AF_INET;
    else
        Domain = AF_INET6;
    if (Protocol == TCP_Protocol)
        Type = SOCK_STREAM;
    else
        Type = SOCK_DGRAM;
    SOCKET NewSocket = socket(Domain,Type,0);
    if (NewSocket != INVALID_SOCKET)
    {
        SocketNode *NewNode = this->_GetSocketNode();
        if (NewNode == NULL)
        {
            this->_CloseSock(NewSocket);
            return NULL;
        }
        NewNode->IP_Version = IP_Version;
        NewNode->Socket = NewSocket;
        NewNode->SetUsing(true);
        return NewNode;
    }
    else
        return NULL;
}

/*
 * 【公开】为Socket绑定IP地址和端口号
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理，函数不检查该指针是否有对应的节点
 * @param const char *IP,要监听的IP，设置为NULL则使用默认
 * @param int Port,要监听的端口
 * @return int,如发生错误将返回SOCKET_ERROR
 */
int CCommonSocket::BindSocket(void *SocketIdentify,const char *IP,int Port)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    addrinfo Hints,*addr;
    memset(&Hints,0,sizeof(Hints));
    if (Node->IP_Version != Version6)
        Hints.ai_family = AF_INET;
    else
        Hints.ai_family = AF_INET6;
    Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
    char PortStr[10];
    this->_NumCStr(Port,&(PortStr[0]));
    if (getaddrinfo(IP,PortStr,&Hints,&addr) != 0)
        return SOCKET_ERROR;
    int nResult = bind(Node->Socket,addr->ai_addr,addr->ai_addrlen);
    freeaddrinfo(addr);
    return nResult;
}

/*
 * 【公开】打开socket的监听
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理，函数不检查该指针是否有对应的节点
 * @return int,如发生错误将返回SOCKET_ERROR
 */
int CCommonSocket::ListenSocket(void *SocketIdentify)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    return listen(Node->Socket,SOMAXCONN);
}

/*
 * 【公开】转接socket，需定义了MUL_THREAD后才是线程安全的
 * @param void *SocketIdentify,监听Socket标识，将转换为SocketNode结构指针去处理，函数不检查该指针是否有对应的节点
 * @return int,返回新的socket信息标识，如发生错误将返回NULL
 */
void * CCommonSocket::AcceptSocket(void *SocketIdentify)
{
    if (SocketIdentify == NULL)
        return NULL;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    SOCKET NewSocket = accept(Node->Socket,NULL,NULL);
    if (NewSocket != INVALID_SOCKET)
    {
        SocketNode *NewNode = this->_GetSocketNode();
        if (NewNode == NULL)
        {
            this->_CloseSock(NewSocket);
            return NULL;
        }
        NewNode->IP_Version = Node->IP_Version;
        NewNode->Socket = NewSocket;
        NewNode->SetUsing(true);
        return NewNode;
    }
    else
        return NULL;
}

/*
 * 【公开】发起连接请求
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理，函数不检查该指针是否有对应的节点
 * @param const char *IP,要连接的IP
 * @param int Port,要连接的端口
 * @return int,如发生错误将返回SOCKET_ERROR
 */
int CCommonSocket::ConnectSocket(void *SocketIdentify,const char *IP,int Port)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    addrinfo Hints,*addr;
    memset(&Hints,0,sizeof(Hints));
    if (Node->IP_Version != Version6)
        Hints.ai_family = AF_INET;
    else
        Hints.ai_family = AF_INET6;
    Hints.ai_flags = AI_NUMERICHOST;
    char PortStr[10];
    this->_NumCStr(Port,&(PortStr[0]));
    if (getaddrinfo(IP,PortStr,&Hints,&addr) != 0)
        return SOCKET_ERROR;
    int nResult = connect(Node->Socket,addr->ai_addr,addr->ai_addrlen);
    freeaddrinfo(addr);
    return nResult;
}

/*
 * 【公开】关闭指定的SOCKET，需定义了MUL_THREAD后才是线程安全的
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @return int,如发生错误将返回SOCKET_ERROR
 */
int CCommonSocket::CloseSocket(void *SocketIdentify)
{
    if (this->InitSuccess == false)
        return SOCKET_ERROR;
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
#ifdef MUL_THREAD
    if (TryEnterCS(this->OpLock,this->DestructObject) == false)
        return 0;
#endif
    RecycleMemory(this->MemPoolNode,Node);
#ifdef MUL_THREAD
    ExitCS(this->OpLock);
#endif
    return 0;
}

/*
 * 【公开】向已建立好连接的Socket发送数据
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @param const char *Data,要发送数据的指针
 * @param int Len,发送数据的长度
 * @return ssize_t,如发生错误将返回SOCKET_ERROR，否则将返回发送出去的数据长度
 */
ssize_t CCommonSocket::SendSocket(void *SocketIdentify,const char *Data,int Len)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    return send(Node->Socket,Data,Len,0);
}

/*
 * 【公开】从已建立好连接的Socket处接收数据
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @param const char *Data,存放接收到的数据的指针
 * @param int Len,数据接收区的大小
 * @return ssize_t,如发生错误将返回SOCKET_ERROR，否则将返回接收到的数据长度
 */
ssize_t CCommonSocket::ReceiveSocket(void *SocketIdentify,char *RecvData,int MaxLen)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    return recv(Node->Socket,RecvData,MaxLen,0);
}

/*
 * 【公开】接收数据并获得发送方消息
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @param const char *Data,存放接收到的数据的指针
 * @param int Len,数据接收区的大小
 * @param sockaddr *From,用于存储发送方信息的指针
 * @param socklen_t *FormLen,如果From不为NULL，则函数返回时填入写入Form的结构的大小
 * @return ssize_t,如发生错误将返回SOCKET_ERROR，否则将返回接收到的数据长度
 */
ssize_t CCommonSocket::ReceiveSocket(void *SocketIdentify, char *RecvData, int MaxLen,sockaddr *From,socklen_t *FromLen)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    return recvfrom(Node->Socket,RecvData,MaxLen,0,From,FromLen);
}

/*
 * 【公开】向指定接收方发送数据
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @param const char *Data,存放要发送数据的指针
 * @param int Len,要发送数据的大小
 * @param sockaddr *To,用于指定发送方信息的指针
 * @param socklen_t ToLen,To指针指向的结构的大小
 * @return ssize_t,如发生错误将返回SOCKET_ERROR，否则将返回接收到的数据长度
 */
ssize_t CCommonSocket::SendSocket(void *SocketIdentify, const char *Data, int Len,sockaddr *To,socklen_t ToLen)
{
    if (SocketIdentify == NULL)
        return SOCKET_ERROR;
    SocketNode *Node = (SocketNode*)SocketIdentify;
    return sendto(Node->Socket,Data,Len,0,To,ToLen);
}

/*
 * 【公开】填充sockaddr对象
 * @param ENUM_IPVersion IP_Version,网络版本号
 * @param const char *IP,填充的IP地址
 * @param int Port,填充的端口号
 * @param sockaddr *TargetSockAddr,要填充的sockaddr对象指针
 * @return socklen_t,返回填充的对象的大小，填充失败返回0
 */
socklen_t CCommonSocket::GetSockAddr(ENUM_IPVersion IP_Version,const char *IP,int Port,sockaddr *TargetSockAddr)
{
    addrinfo Hints,*addr;
    if (TargetSockAddr == NULL)
        return 0;
    memset(&Hints,0,sizeof(Hints));
    if (IP_Version != Version6)
        Hints.ai_family = AF_INET;
    else
        Hints.ai_family = AF_INET6;
    Hints.ai_flags = AI_NUMERICHOST;
    char PortStr[10];
    this->_NumCStr(Port,&(PortStr[0]));
    if (getaddrinfo(IP,PortStr,&Hints,&addr) != 0)
        return 0;
    memcpy(TargetSockAddr,addr->ai_addr,addr->ai_addrlen);
    socklen_t AddrLen = addr->ai_addrlen;
    freeaddrinfo(addr);
    return AddrLen;
}

/*
 * 【公开】从管理结构中获取SOCKET对象
 * @param void *SocketIdentify,Socket标识，将转换为SocketNode结构指针去处理
 * @return SOCKET,返回从SocketIdentify中取得的SOCKET对象，获取失败返回0
 */
SOCKET CCommonSocket::GetSocket(void *SocketIdentify)
{
    if (SocketIdentify == NULL)
        return 0;
    else
        return ((SocketNode*)SocketIdentify)->Socket;
}

/*
 * 【私有】【内联】获取空闲SocketNode节点，需定义了MUL_THREAD后才是线程安全的
 * @return SocketNode *,返回可用节点指针，如果出错，将直接抛出异常
 */
CCommonSocket::SocketNode *CCommonSocket::_GetSocketNode()
{
#ifdef MUL_THREAD
    if (TryEnterCS(this->OpLock,this->DestructObject) == false)
        return NULL;
    SocketNode *pTmpNode = (SocketNode *)AllocMemory(this->MemPoolNode);
    ExitCS(this->OpLock);
    return pTmpNode;
#else
    return (SocketNode *)AllocMemory(this->MemPoolNode);
#endif
}

/*
 * 【私有】【内联（仅Win32版本）】10进制数值转换字符串的函数（用于端口号转换）
 * @param int Num,要转换的数字
 * @param char *Buf,字符串缓冲区
 * @return void
 */
 void CCommonSocket::_NumCStr(int Num,char *Buf)
{
#ifdef WIN32
    _itoa(Num,Buf,10);
#else
    int BufP = 0;
    int CountBit = 10;
    while (Num / CountBit > 0)
    {
        ++BufP;
        CountBit *= 10;
    }
    CountBit /= 10;
    Buf[BufP + 1] = '\0';
    int LoopVar = 0;
    while (LoopVar <= BufP)
    {
        Buf[LoopVar] = '0' + (Num / CountBit);
        Num = Num - (Num / CountBit) * CountBit;
        ++LoopVar;
        CountBit /= 10;
    }
#endif
}
