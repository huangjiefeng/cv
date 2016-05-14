/*
 * 遥控器与终端通信服务器端管理类的类声明文件
 * V1.3.1.0 2012-05-02  1、根据CNetManager类的调整而做了相应变更
 *                      2、增加对IPv6连接的支持
 * V1.3.0.0 2012-02-28  使用NetManager模块进行网络管理，使得本模块代码量得以缩减，仅专注于业务逻辑
 * V1.2.0.0 2011-12-09  使用CommonThread和SimpleMemPool模块做资源的辅助管理，调用消息处理的代码完成 By Lzy
 * V1.1.0.0 2011-11-24  调整内部实现 By Lzy
 * V0.0.0.0 2011-02-23  创建文件 By Lzy
 */
#ifndef NETSERV_H
#define NETSERV_H

#include "NetManager.h"

#define NOTIFY_BUFFER_SIZE 2048 //通告消息缓冲区大小
#define NOTIFY_ALL 0x4000000 //AddNotify函数中指明向所有通告连接发送消息的标识号

class CNetServ
{
public:
    //------枚举------
    enum EConnectTypeEnum{Type_Tcp,//使用TCP连接方式
                         Type_Udp,//使用UDP连接方式
                         Type_Both};//同时使用TCP和UDP连接方式
    //------函数------
    
    /*
     * 【公开】【内联】【静态】创建CNetServ类的单实例
     * @param 各参数说明看CNetServ的构造函数
     * @return CNetServ *,返回构造好的单实例指针
     */
    static CNetServ *CreateInstance(unsigned int uCommandPort,EConnectTypeEnum eCommandConnectType,unsigned int uNotifyPort,unsigned int uNotifyListLen = 50)
    {
        if (CNetServ::m_pNetServInstance != NULL)
            delete CNetServ::m_pNetServInstance;
        CNetServ::m_pNetServInstance = new CNetServ(uCommandPort,eCommandConnectType,uNotifyPort,uNotifyListLen);
        return CNetServ::m_pNetServInstance;
    }

    /*
     * 【公开】【内联】【静态】获取CNetServ单实例指针，如果没使用CNetServ::CreateInstance进行实例初始化，这里获取到的将会是NULL值
     * @return CNetServ *,返回单实例的指针值，如果实例未构造则返回NULL
     */
    static CNetServ *GetInstance()
    {
        return CNetServ::m_pNetServInstance;
    }

    /*
     * 【公开】【内联】【静态】释放CNetServ单实例
     * @return void
     */
    static void ReleaseInstance()
    {
        if (CNetServ::m_pNetServInstance != NULL)
            delete CNetServ::m_pNetServInstance;
        CNetServ::m_pNetServInstance = NULL;
    }

    int CommandSendBack(const char *pData,int nLen,int nSockResIndex);//对主动请求消息发送返回的数据
    int AddNotify(const char *pData,int nLen,int nTarget = NOTIFY_ALL);//添加要发送的通告消息（发送动作延后进行）
private:
    //------结构体------
    struct SNotifyNode//通告暂存队列节点
    {
        char m_pBuffer[NOTIFY_BUFFER_SIZE];
        int m_nUsedSize;//Buffer中有效数据长度
        int m_nTargetIndex;//通告消息返回的目标节点索引，与m_pTargetAddr直接相关，其中-1表示全体，如果目标所对应的连接不是返回通告消息的连接，则该目标也直接略过
    };
    //------变量-------
    static CNetServ *m_pNetServInstance;//CNetServ类的单实例指针
    bool m_bThreadEnd;//线程结束标志，通知所有线程结束运行*/
    //  ------网络连接相关------
    CNetManager *m_pNetManagerInstance;//网络管理模块实例指针
    //    ------遥控器主动请求消息处理相关------
    SNodeID m_oCommandListenerV4TCPID;//监听遥控器IPv4下的TCP方式主动请求连接的索引结构
    SNodeID m_oCommandListenerV6TCPID;//监听遥控器IPv6下的TCP方式主动请求连接的索引结构
    SNodeID m_oCommandListenerV4UDPID;//监听遥控器IPv4下的UDP方式主动请求连接的索引结构
    SNodeID m_oCommandListenerV6UDPID;//监听遥控器IPv6下的UDP方式主动请求连接的索引结构
    //  ------遥控器通告消息处理相关------
    SNodeID m_oNotifyListenerV4ID;//监听遥控器被动消息处理请求的IPv4连接的索引结构
    SNodeID m_oNotifyListenerV6ID;//监听遥控器被动消息处理请求的IPv6连接的索引结构
    bool m_bNotifySendThreadIsRunning;//标记_NotifySendTCPThread函数是否仍然在运行
    //        ------通告消息管理相关------
    SNotifyNode *m_pNotifyList;//通告暂存队列（定长循环队列）
    int m_nNotifyListLen;//通告暂存队列可访问的长度
    int m_nNotifyListReadIndex;//通告暂存队列[读]索引
    int m_nNotifyListWriteIndex;//通告暂存队列[写]索引
    CRITICAL_SECTION *m_pNotifyListLock;//通告消息队列管理数组互斥访问锁（准确来说此锁只对写操作有效，读操作不需要加锁）
    //------函数------
    CNetServ(unsigned int uCommandPort,EConnectTypeEnum eCommandConnectType,unsigned int uNotifyPort,unsigned int uNotifyListLen);
    ~CNetServ();
    CNetServ(const CNetServ&){};
    bool _IDParse(SNodeID &rClientID,unsigned char nExternAdd,int &rResult);//ID转换函数
    void _IDUnParse(int nUnParseID,SNodeID &rClientID,unsigned char &rExternAdd);//ID逆转换函数
    //    ------回调函数------
    static bool _CommandListenerTCPAcceptFunc(SNodeID oClientID,SNodeID oServID,void *pNetManager);//TCP主动通信监听到新的连接发起时的回调函数
    static void _CommandClientRecvFunc(SNodeID oClientID,SNodeID oServID,ENUM_Protocol eProtocol,const char *pData,int nDataLen,void *pNetManager);//主动通信中接收到消息时的回调函数
    static bool _NotifyListenerTCPAcceptFunc(SNodeID oClientID,SNodeID oServID,void *pNetManager);//TCP通告通信监听到新的连接发起时的回调函数
    static void _NotifyClientDisconnectFunc(SNodeID oClientID,SNodeID oServID,ENUM_Protocol eProtocol,bool bError,void *);//TCP通告通信连接断开时的回调函数
    //    ------线程函数------
    static void *_NotifySendTCPThread(void *pParam);//向订阅了通告消息的遥控器发送相关信息用线程函数
};
#endif