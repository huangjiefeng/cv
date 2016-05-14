/*
 * 网络管理类，在CommonSocket模块基础上对网络操作进行封装 头文件 By Lzy
 * 2012-05-02   V1.0.1.0    1、修正IOCP管理下断开的连接未被清理的问题
 *                          2、加入连接识别代码，防止对已回收但再重用的连接操作
 *                          3、重新考虑了线程冲突问题
 * 2012-02-27   V1.0.0.0    初版完成，未包含linux的epoll部分
 * 2012-01-27   V0.0.0.0    创建文件
 */

#ifndef LZY_NET_MANAGER_H
#define LZY_NET_MANAGER_H

#include "CommonSocket.h"
#include "CommonThread.h"

//===================模块可配置属性=================
#define NET_MANAGER_RECEIVE_BUFFER_SIZE 512 //定义托管连接的情况下单个接收数据缓冲区的大小
#define NET_RECYCLE_ARRAY_SIZE 256 //服务器端/客户端节点清理循环队列的大小
#ifdef WIN32
#define MAX_IOCP_THREAD_COUNT 0 //设置IOCP线程池中最大线程数量，当此值小于等于0时，表示无上限
#endif

#define SERV_NONE   -1 //指未与特定服务器端关联
#define SERV_ALL    -2 //不指定服务器端ID，表示操作对象为全体服务器
#define CLIENT_NONE -1 //指未与特定客户端关联
#define CLIENT_ALL  -2 //不指定客户端ID，表示操作对象为满足服务器ID条件下的所有客户端

//===================操作编号定义===================
#define OP_MANAGE   0 //托管操作
#define OP_SEND     1 //同步发送消息操作
#define OP_RECEIVE  2 //同步接收消息操作
#define OP_CLOSE    3 //关闭连接操作

//===================返回值用的结构体定义============
struct SNodeID //连接节点（SServNode和SClientNode节点）的有效识别信息（防止空间复用后当原本的节点来操作）
{
    int m_nPID;//用于在内存池索引节点的ID号
    unsigned char m_chUID;//检查是否被复用而设的ID号，该号每次分配后+1

    bool operator == (const SNodeID &oCompare)
    {
        if (oCompare.m_nPID == this->m_nPID && oCompare.m_chUID == this->m_chUID)
            return true;
        else
            return false;
    }
};

//===================回调函数定义===================
/*
 * 网络服务器端accept到新连接时的回调函数定义
 * @param in SNodeID oClientIDO,新连接的连接标识数据
 * @param in SNodeID oServIDO,收到这个新连接的服务器端标识数据
 * @param in void *pUser,用户数据
 * @return bool,返回true表示接收这个新连接，返回false表示不接收，当选择不接收时，这个新连接会被关闭
 */
typedef bool (*Accept_FuncP)(SNodeID oClientIDO,SNodeID oServIDO,void *pUser);

/*
 * 网络连接收到新消息时的回调函数定义
 * @addition info,此回调函数只有在该连接处于托管状态下才能被触发
 * @param in SNodeID oClientIDO,发来消息的连接的连接标识数据
 * @param in SNodeID oServIDO,发来消息的连接所属的服务器端标识数据
 * @param in ENUM_Protocol eProtocol,指明连接类型（TCP_Protocol,UDP_Protocol）
 * @param in const char *pData,收到的数据
 * @param in int nDataLen,收到的数据的长度
 * @param in void *pUser,用户数据
 * @return void
 */
typedef void (*Recv_FuncP)(SNodeID oClientIDO,SNodeID oServIDO,ENUM_Protocol eProtocol,const char *pData,int nDataLen,void *pUser);

/*
 * 网络连接断开时的回调函数定义
 * @param in SNodeID oClientIDO,断开的连接的连接标识数据
 * @param in SNodeID oServIDO,断开的连接所属的服务器端标识数据
 * @param in ENUM_Protocol eProtocol,指明连接类型（TCP_Protocol,UDP_Protocol）
 * @param in bool bError,指示当前的断开是异常断开(true)还是正常断开(false)
 * @param in void *pUser,用户数据
 * @return void
 */
typedef void (*Disconnect_FuncP)(SNodeID oClientIDO,SNodeID oServIDO,ENUM_Protocol eProtocol,bool bError,void *pUser);
//==================================================

enum EManageType //指定托管类型
{
    NET_NONE,//未拖过，外部自行管理，此值仅能类内部用
    NET_THREAD,//独立线程管理
    NET_WIN32_IOCP,//win下专用，IOCP方式管理
    NET_LINUX_EPOLL,//linux下专用，ePoll方式管理
    NET_SYSTEM_RELATIVE,//因应系统而识别，win下tcp的客户端使用IOCP,udp的服务器端使用thread（不允许udp服务器端使用IOCP是出于内存管理的考虑）,linux下使用ePool
};

class CNetManager
{
public:
    static CNetManager *GetInstance();//获取类实例，如果类实例不存在则自动创建
    static void ReleaseInstance();//释放类实例
    SNodeID CreateTCPServ(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,Accept_FuncP pfAccept = NULL,void *pUser = NULL);//创建TCP服务器端
    SNodeID CreateUDPServ(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,EManageType eManageType,Recv_FuncP pfRecv = NULL,Disconnect_FuncP pfDisconnect = NULL,void *pUser = NULL);//创建UDP服务器端
    SNodeID CreateClient(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,ENUM_Protocol eProtocol,EManageType eManageType,Recv_FuncP pfRecv = NULL,Disconnect_FuncP pfDisconnect = NULL,void *pUser = NULL);//创建客户端
    int ManageClient(SNodeID oClientIDO,SNodeID oServIDO,EManageType eManageType,Recv_FuncP pfRecv = NULL,Disconnect_FuncP pfDisconnect = NULL,void *pUser = NULL);//托管指定的连接/连接组（也可用此函数更改指定连接/连接组的回调函数，用户数据）
    int SendNet(SNodeID oClientIDO,SNodeID oServIDO,const char *pData,int nDataLen);//向指定的连接发送消息
    int ReceiveNet(SNodeID oClientIDO,SNodeID oServIDO,char *pDataBuf,int nBufLen);//从指定连接接收消息，如果该连接已设置为托管，此函数将执行失败
    void CloseNet(SNodeID oClientIDO,SNodeID oServIDO,bool bActiveDisconnectCallBack);//断开指定的连接/连接组
private:
    //------结构体------
    struct SServNode //单个服务器端的节点
    {
        void *m_oSocket;//CommonSocket管理的对象
        Accept_FuncP m_pfAccept;//accept回调函数
        void *m_oClientPool;//管理所有accept到的连接的内存池管理对象
        void *m_pHeadClientNode;//其下管理的所有accept连接所形成的链表的表头
        CRITICAL_SECTION *m_pVisitingCountLock;//修改m_nVisitingCount变量的互斥锁
        CRITICAL_SECTION *m_pClientPoolLock;//访问本结构体中的m_oClientPool的互斥锁
        CRITICAL_SECTION *m_pClientLinkLock;//所管理的客户端链表的访问、修改互斥锁
        bool m_bExit;//通知管理此结构的线程退出的标记
        bool m_bAddedLink;//指示节点是否已加入到链表中
        SNodeID m_oIDO;//当前节点的ID数据结构
        void *m_pUser;//用户定义的数据
        int m_nVisitingCount;//指明当前服务器端节点信息正在被访问的计数，当要关闭此服务器端节点时需要等待此值为0时才能操作，因为此值的改变只在单线程中进行，所以无需锁操作
        SServNode *m_pNext;//单向链中的下一节点
    };

    struct SClientNode //单个连接的节点
    {
#ifdef WIN32
        OVERLAPPED m_OverLapped;//重叠结构，这里仅为迎合WSARecv需要，没其它用途（使用IOCP托管时用到）
#endif
        void *m_oSocket;//CommonSocket管理的对象
        union {sockaddr_in6 u_oAddr6Client;sockaddr_in u_oAddr4Client;}m_uAddr;//UDP专用，保存发送方信息/设置接收方信息
        socklen_t m_nAddrLen;//UDP专用，指定m_uAddr的实际使用大小
        bool m_bIsUDPServ;//UDP专用，指定当前节点是否作为UDP服务器端使用
        ENUM_Protocol m_eProtocol;//连接类型
        Recv_FuncP m_pfRecv;//接收到消息时的回调函数
        Disconnect_FuncP m_pfDisconnect;//断开时的回调函数
        void *m_pUser;//用户定义的数据
        EManageType m_eManagerType;//指定此连接的托管状态
        SNodeID m_oServIDO;//关联的服务器端ID结构，没有与服务器端管理则里面的m_nPID设为SERV_NONE
        SNodeID m_oIDO;//当前节点的ID数据结构
        bool m_bErrorClose;//指定连接是否因错误而关闭（异常关闭），初始值为false
        int m_nVisitingCount;//指明当前客户端节点信息正在被访问的计数，当要关闭此客户端节点时需要等待此值为0时才能操作
        CRITICAL_SECTION *m_pVisitingCountLock;//修改m_nVisitingCount变量值时需要的锁
        bool m_bExit;//此结构点即将被回收的标记
        bool m_bAddedLink;//指示节点是否已加入到链表中
        SClientNode *m_pPre;//双向链中的上一节点
        SClientNode *m_pNext;//双向链中的下一节点
    };

    struct SManageParam //存放托管操作(OP_MANAGE))参数的结构体
    {
        EManageType m_eManageType;//托管类型
        Recv_FuncP m_pfRecv;//接收到消息时的回调函数
        Disconnect_FuncP m_pfDisconnect;//断开连接时的回调函数（在托管状态下，如果指定了回调函数，除在CloseNet函数中指定不触发回调而断开连接的情况外，其它情况下的断开都会触发回调函数）
        void *m_pUser;//用户自定义的传给回调函数的数据指针
    };

    struct SSendReceiveParam //存放发送/接收消息操作(OP_SEND)参数的结构体
    {
        union{const char *u_pData;char *u_pReceiveBuf;}m_pUnion;//要发送的数据所在的缓冲区/接收到的数据存放的缓冲区
        int m_nLen;//要发送数据的字节数/接收数据缓冲区的大小（字节数）
    };

    struct SClientNodeThreadParam //存放传送给_ForClientNodeThread函数参数的结构体
    {
        SServNode *m_pServNode;//与pClientNode所指连接关联的SServNode节点指针，如果该连接并不是由服务器监听而创建的，则这里填入SERV_NONE
        SClientNode *m_pClientNode;//管理的连接节点指针
        CNetManager *m_pNetManager;//当前的CNetManager对象
        bool m_bHasRead;//结构体内的数据是否已被读取的标记，防止结构体内容失效
    };

    struct SServNodeThreadParam //存放传送给_ForServNodeThread函数参数的结构体
    {
        SServNode *m_pServNode;//管理的服务器节点指针
        CNetManager *m_pNetManager;//当前的CNetManager对象
        bool m_bHasRead;//结构体内的数据是否已被读取的标记，防止结构体内容失效
    };

    struct SLoopNodes //回收处理循环链表的表节点结构
    {
        SServNode *m_pServNode;//服务器端指针
        SClientNode *m_pClientNode;//客户端指针
        bool m_bRecycleServNode;//如果处理对象是服务器端，则此字段决定是否回收服务器节点
    };

    //------变量------
    //  类实例指针
    static CNetManager *m_pNetManager;//本类单实例的实例实际指针
    CCommonSocket *m_pCommonSocket;//CCommonSocket类实例指针
    //  服务器端相关
    void *m_oServPool;//存放所有服务器端的信息的内存池管理对象
    CRITICAL_SECTION *m_pServPoolLock;//访问本类成员中的m_oServPool的互斥锁
    CRITICAL_SECTION *m_pServLinkLock;//访问、修改服务器端链表的互斥锁
    SServNode *m_pHeadServNode;//服务器端信息链表的首节点
    //  Client端相关
    void *m_oClientPool;//存放所有由CreateClient函数创建的连接的信息的内存池管理对象
    CRITICAL_SECTION *m_pClientPoolLock;//访问本类成员中的m_oClientPool的互斥锁
    CRITICAL_SECTION *m_pClientLinkLock;//访问、修改无服务器绑定的客户端链表的互斥锁
    SClientNode *m_pHeadClientNode;//Client端信息链表的首节点
    //  节点处理循环队列相关
    CRITICAL_SECTION *m_pWriteLock;//m_nWriteIndex访问、修改互斥锁
    int m_nWriteIndex;//循环队列写指针
    int m_nReadIndex;//循环队列读指针
    SLoopNodes m_aRecycle[NET_RECYCLE_ARRAY_SIZE];//循环队列
    //  其它
    bool m_bObjectRelease;//指示是否要释放对象
#ifdef WIN32
    //  IOCP处理相关
    HANDLE m_hIOCP_Handle;
    unsigned int m_nCurCreatedIOCPThreadCount;//当前已创建的IOCP处理用的线程数
    unsigned int m_nCurRunningIOCPThreadCount;//当前进行接收处理中的线程数目，修改此值需要加互斥锁
    CRITICAL_SECTION *m_pCurIOCPThreadCountVarLock;//修改CurRunningIOCPThreadCount/m_nCurCreatedIOCPThreadCount值时的互斥锁
    //  迎合WSARecv函数需要而设置的变量
	DWORD m_dwWSARecvReceiveLen;//传入给WSARecv函数的lpNumberOfBytesRecvd参数
	DWORD m_dwWSARecvFlag;//传入给WSARecv函数的lpFlags参数
	WSABUF m_oWSARecvZeroBuf;//传入给WSARecv函数的lpBuffers参数，这里为零长度的缓冲区
#endif
    //------函数------
    CNetManager();
    ~CNetManager();
    CNetManager(const CNetManager &){};
    static void _ForMemPoolReleaseServNode(void *pObject,void *pCommonSocket);//MemPool释放SServNode结构时调用的函数
    static void _ForMemPoolReleaseClientNode(void *pObject,void *pCommonSocket);//MemPool释放SClientNode结构时调用的函数
    static void *_ForServNodeThread(void *pObject);//每个服务器端单独跑的线程
    static void *_ForClientNodeThread(void *pObject);//使用独立线程管理单个连接的托管方式时的线程函数
    static void *_ForNodeManage(void *pObject);//处理要关闭或回收的节点的线程函数
#ifdef WIN32
    static void *_ForIOCPThreadManageThread(void *pObject);//监测IOCP线程使用状况并创建IOCP线程的线程函数
    static void *_ForIOCPPoolThread(void *pObject);//IOCP线程池使用的线程函数
#endif
    int _RangeOperation(SNodeID oClientIDO,SNodeID oServIDO,int nOperationNum,void *pParam);//对特定范围的连接进行指定操作
    int _ManageClient(SServNode *pServNode,SClientNode *pClientNode,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser);//ManageClient的内部版，托管指定的连接（不做参数检查，互斥锁也必须外部调用时处理）
    SNodeID _CreateClient(const char *pszIP,ENUM_IPVersion eIPVer,unsigned int nPort,ENUM_Protocol eProtocol,bool bIsUDPServ,EManageType eManageType,Recv_FuncP pfRecv,Disconnect_FuncP pfDisconnect,void *pUser);//创建TCP客户端,UDP客户端或服务器端
    void _CloseNodes(SServNode *pServNode,SClientNode *pClientNode,bool bActiveCallBack,bool bRecycleServNode);//关闭服务器/客户端节点，把节点放入回收处理队列，并视乎参数组合决定是否触发关闭回调
};

#endif