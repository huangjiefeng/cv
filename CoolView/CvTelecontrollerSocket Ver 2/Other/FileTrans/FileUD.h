/*
 * 支持断点续传、分段传输的文件上传、下载的服务器端模块，可支持对4G以上文件的操作（最大同时传输文件数为5000）
 * 2012-05-17   V1.0.0.0    初版
 * 2012-05-09   V0.0.0.0    创建文件
 */

#include <stdio.h>
#include <string>
#include <map>
using namespace std;

#include "CommonSocket.h"
#include "CommonThread.h"
using namespace thread_operation;

#define CFUDS_CHECK_GAP_TIME 500 //更新传输信息节点等待连接用时的线程的更新间隔（毫秒为单位），实际更新间隔还需要在此值基础上加上数组遍历时间和其它一些系统开销时间
#define CFUDS_ENABLED_IPV4 //定义这个标识后可用IPv4连接传输数据
#define CFUDS_ENABLED_IPV6 //定义这个标识后可用IPv6连接传输数据

#define CFUDS_READ_WRITE_BUF 3072 //定义每条连接读、写文件块的大小（字节为单位，上限为int类型最大值，下限不应该小于CFUDS_RECV_BUF值） 上传状态：当收到的数据量达到该值或连接异常断开时就会把数据回写到文件中 下载状态：每次从文件读取此值大小的数据量
#define CFUDS_RECV_BUF 128 //定义每条连接接收网络数据的缓冲区大小，该值的长度至少能容纳指令类型数据的传输，默认值128，不建议修改

class CFileUpDownServ //负责遥控器上传文件到终端的类
{
public:
    static inline CFileUpDownServ *GetInstance(){return CFileUpDownServ::m_pCFUDInstance;};//获取单例对象，使用前必须先用CreateInstance创建单例
    static void ReleaseInstance();//释放单例对象
    static CFileUpDownServ *CreateInstance(int nListenPort,short snMaxTransNum,unsigned int uWaitOverTime);//创建类实例，如果类实例已创建则直接返回
    int RegUpdateTrans(const char *pszFilePath,const char *pszInfoFilePath,long long llFileSize,unsigned int uMaxConnectionCount,unsigned short snBlockCount,bool bIsOverWrite,long long &rllFinished);//设置上传信息，并返回上传权限码，该权限码在传输前需要验证
    int RegDownloadTrans(const char *pszFilePath,unsigned int uMaxConnectionCount,long long &rllFileSize);//设置下载信息，并返回下载权限码，该权限码在传输前需要验证

private:
    //------结构体------
    struct SUpdatePartNode //上传分块的数据信息
    {
        long long m_llStart;//起点数据区（包含）
        long long m_llEnd;//终点数据区（包含）
        void *m_pSocket;//负责处理此块上传数据的连接
        long long m_llFinished;//此块已传输的数据量
        SUpdatePartNode *m_pNext;//下一节点地址
    };

    struct STransNode //传输控制循环列表中节点结构
    {
        unsigned int m_uTimeLeft;//剩余的等待连接时间（只有设置函数和扫描线程会改变这个值，扫描线程每次扫描时如果发现m_uCurConnectionCount=0则对这个值减1，到0时则宣布节点无效）
        short m_snIdentify;//标识值，与节点索引共同组成一个int32类型，作为传输连接识别号
        bool m_bIsUp;//标记客户端是否上传操作，如果是下载操作则置为false
        unsigned int m_uMaxConnectionCount;//传输该文件最大允许连接数
        unsigned int m_uCurConnectionCount;//为传输该文件当前建立的连接数
        CRITICAL_SECTION *m_pCurConnectionLock;//修改m_uCurConnectionCount值或连接线程写入m_pNewSocketIdentify时的互斥锁
        FILE *m_pTransFile;//传输文件的操作句柄
        CRITICAL_SECTION *m_pReadWriteFileLock;//读、写文件时的锁
        void *m_pNewSocketIdentify;//新建立的连接的socket标识，上传情况：节点管理线程在处理后会把此字段归NULL，其它新的连接可再次写入 下载情况：连接线程自行把连接加入到下载连接链中，以便析构时释放
        int m_nNextIndex;//如果节点处于有效状态（m_uTimeLeft > 0），则表示下一个使用中节点的索引值，否则表示下一个空闲节点的索引
        //------↓上传专用↓-------
        long long m_llBlockSize;//上传文件的分块大小，字节为单位
        long long m_llFinishedBlockData;//完成传输的分块中的数据总量（正在传输中的分块不计算在内）
        SUpdatePartNode *m_pUpdatePartList;//剩余传输数据列表，只有节点管理线程会调整此链表
        string m_szInfoFilePath;//续传消息文件的路径
    };

    //------变量------
    static CFileUpDownServ *m_pCFUDInstance;//类单例对象
    unsigned int m_uWaitOverTime;//首个连接发起的最长等待时间
    unsigned int m_uRandSeed;//随机种子
    void *m_pUpdatePartListMemPool;//分配SUpdatePartNode结构的内存池
    CRITICAL_SECTION *m_pUpdatePartListMemPoolLock;//使用m_pUpdatePartListMemPool内存池时的互斥锁
    STransNode *m_pTransNodeArrayList;//传输信息数组链表
    short m_snTransNodeArrayListCap;//传输信息数组的容量
    int m_nNextUsingIndex;//下一个使用中传输信息节点索引
    int m_nNextFreeIndex;//下一个可分配传输信息节点索引
    CRITICAL_SECTION *m_pAdjustNodeLock;//调整传输信息数组链表m_nNextUsingIndex、m_nNextFreeIndex值时的锁
    
    void *m_pConnectionMemPool;//分配存放连接标识的内存池，以便析构时能控制连接关闭操作
    CRITICAL_SECTION *m_pConnectionMemPoolLock;//使用m_pConnectionMemPool内存池时的互斥锁

    bool m_bIsRelease;//对象释放标记
#ifdef CFUDS_ENABLED_IPV4
    void *m_pTCPServV4;//监听传输连接的IPv4服务器端
#endif
#ifdef CFUDS_ENABLED_IPV6
    void *m_pTCPServV6;//监听传输连接的IPv6服务器端
#endif

    //------函数------
    CFileUpDownServ(int nListenPort,short snMaxTransNum,unsigned int uWaitOverTime);
    ~CFileUpDownServ();
    CFileUpDownServ(const CFileUpDownServ&){};
    static void *_RefreshWaitTimeoutThread(void *pNull);//更新所有传输节点等待超时值的线程函数
    static void *_SocketManageThread(void *pSocket);//管理单个传输服务器端连接的线程函数
    static void *_UpdateNodeManageThread(void *pIndex);//管理单个上传节点的线程函数
    static void *_DownloadNodeManageThread(void *pIndex);//管理单个下载节点的线程函数
    static void *_TransThread(void *pSocket);//管理传输连接的线程
    static void _ForReleaseDownloadConnection(void *pObject,void *pCommonSocket);//内存池释放下载连接时调用的函数
};
