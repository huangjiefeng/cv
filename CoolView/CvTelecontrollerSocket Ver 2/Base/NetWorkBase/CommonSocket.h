/*
 * 网络操作类声明文件 By Lzy
 * @info 本类部分函数非线程安全，这不仅仅指同一时间同样函数被调用可能出问题，也包括这堆函数中不同函数的调用，要获得线程安全的函数，需要定义MUL_THREAD宏或外部自行保证
 * 2012-02-27   V1.0.2.5    新增GetSocket和GetSockAddr两个公开函数
 * 2012-02-26   V1.0.2.4    调整UDP用的SendSocket和ReceiveSocket函数的输入参数
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

#ifndef COMMONSOCKET_H
#define COMMONSOCKET_H

//#define MUL_THREAD //当定义了这个后能使CommonSocket类是线程安全的

#include <string>
#include <string.h>
#include "SimpleMemPool.h"

#ifdef MUL_THREAD
#include "CommonThread.h"
using namespace thread_operation;
#endif
using namespace std;

#ifndef NULL
#define NULL 0
#endif

#ifdef LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
typedef int SOCKET;
#else
#ifdef WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
typedef int ssize_t;
typedef int socklen_t;
#endif
#endif

enum ENUM_IPVersion//IP版本枚举类型
{
    Version4,
    Version6
};

enum ENUM_Protocol//协议类型枚举
{
    TCP_Protocol,
    UDP_Protocol
};

class CCommonSocket
{
public:
    static CCommonSocket *GetInstance();//创建新的实例对象，如果实例已经存在，则直接返回该实例
    static void ReleaseInstance();//释放创建的实例对象
    void *CreateSocket(ENUM_IPVersion IP_Version,ENUM_Protocol Protocol);//创建新的Socket
    int BindSocket(void *SocketIdentify,const char *IP,int Port);//为Socket绑定IP地址和端口号
    int ListenSocket(void *SocketIdentify);//打开socket的监听
    void *AcceptSocket(void *SocketIdentify);//转接socket
    int ConnectSocket(void *SocketIdentify,const char *IP,int Port);//发起连接请求
    int CloseSocket(void *SocketIdentify);//关闭指定的SOCKET
    ssize_t SendSocket(void *SocketIdentify,const char *Data,int Len);//向已建立好连接的Socket发送数据
    ssize_t SendSocket(void *SocketIdentify, const char *Data, int Len,sockaddr *To,socklen_t ToLen);//向指定接收方发送数据
    ssize_t ReceiveSocket(void *SocketIdentify,char *RecvData,int MaxLen);//从已建立好连接的Socket处接收数据
    ssize_t ReceiveSocket(void *SocketIdentify, char *RecvData, int MaxLen,sockaddr *From,socklen_t *FormLen);//接收数据并获得发送方消息
    socklen_t GetSockAddr(ENUM_IPVersion IP_Version,const char *IP,int Port,sockaddr *TargetSockAddr);//填充sockaddr对象
    SOCKET GetSocket(void *SocketIdentify);//从管理结构中获取SOCKET对象
private:
    struct SocketNode//存放Socket结构的节点
    {
        SOCKET Socket;
        ENUM_IPVersion IP_Version;
        void SetUsing(bool State) {this->Using = State;}
        bool GetUsing() {return this->Using;}
    private:
        bool Using;
    };
    //------------------
    CCommonSocket();
    ~CCommonSocket();
    CCommonSocket(const CCommonSocket&){};
    static void ForMemPoolInitialSock(void *pObject,void *pNULL);//为线程池初始化对象用
    static void ForMemPoolReleaseSock(void *pObject,void *pNULL);//为线程池释放对象用
    inline SocketNode *_GetSocketNode();//获取空闲SocketNode节点
#ifdef WIN32
    inline void _NumCStr(int Num,char *Buf);//10进制数值转换字符串的函数（用于端口号转换）（Win32版本）
#else
    void _NumCStr(int Num,char *Buf);//10进制数值转换字符串的函数（用于端口号转换）（其他系统版本）
#endif
    
    /*
     * 【私有】【静态】【内联】纯关闭连接用函数
     * @param SOCKET Sock,要关闭的SOCKET标识
     * @return int,操作结果
     */
    static int _CloseSock(SOCKET Sock)
    {
#ifdef LINUX
        return shutdown(Node->Socket,SHUT_RDWR);
#endif
#ifdef WIN32
        return closesocket(Sock);
#endif
    }
    //------------------
    static CCommonSocket *SocketClassP;//存放创建的实例
    bool InitSuccess;//实例初始化结果
    void *MemPoolNode;//内存池管理指针
#ifdef MUL_THREAD
    CRITICAL_SECTION *OpLock;//多线程下的操作锁
    bool DestructObject;//析构函数用于通知所有执行中的函数退出的变量
#endif
};

#endif
