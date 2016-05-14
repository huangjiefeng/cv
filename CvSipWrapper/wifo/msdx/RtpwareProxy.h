////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   RtpwareProxy.h
/// @brief 定义动态加载rtpware dll时获取其导出函数的方法
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef  RW_RtpwareProxy_H
#define  RW_RtpwareProxy_H

#include "RtpStat.h"
#include <windows.h>

//日志记录宏
#ifdef _DEBUG
#include  <stdio.h>
#define  rwp_log	 printf
#else
#define  rwp_log(...)
#endif // _DEBUG


#define RTPWARE_DLL_FILE_NAME "rtpware.dll"
#define RTPWARE_DLL_RELATIVE_FILE_NAME ".\\filter\\rtpware.dll"     //rtpware dll的相对路径，一般位于filter目录下

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief rtpware dll导出函数的代理接口，属于单实例。
////////////////////////////////////////////////////////////////////////////////////////////////////
class RtpwareProxy
{
public:
    static RtpwareProxy& getInstance();

    /*
        * @brief  计算RtpWare所有传输会话的总统计信息，把结果保存到输出变量中。
        *             注意该函数是dll导出函数，msdx会通过动态加载dll方式调用该函数（根据签名），所以它的签名不能随意改变，不得以改变后需通知msdx修改。
        * @param [in,out] transtat   传入一个TranStat结构指针变量，统计结果值将保存到该变量中
        * @return 返回0表示获取成功，-1则表示获取失败.
        */
    int computeGlobalTranStat(TranStat * transtat);


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	 设置发送测试报告的服务器地址和端口（UDP方式）
    /// @param	serverIP	The server ip.
    /// @param	port		The port.
    /// @return	返回0则设置成功，返回负数则表示函数调用失败或者执行失败
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int setSimluteReportServer(const char * serverIP, const int port);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	 启动一个新的发送器
    /// @param	sessionId	会话ID
    /// @param	remoteIP	接收端(目标)主机IP，不能为空
    /// @param	remotePort	 目标端口
    /// @param	sendRate		发送速率，单位kbps
    /// @param	dscp		     dscp值大小，0-63
    /// @param	duration	持续发送时间，单位秒second
    /// @return 启动成功则返回0，返回负数则表示函数调用失败或者执行失败
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int startNewSimluteSender(int sessionId, const  char* remoteIP, int remotePort, int sendRate, int dscp, int duration);


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief	 启动一个新的接收器
    /// @param	sessionId	会话ID
    /// @param	sourceIP	数据来源的主机IP，或组播地址
    /// @param	localPort	本地端口
    /// @param	duration	 持续接收时间，单位秒second
    /// @return 启动成功则返回0，返回负数则表示函数调用失败或者执行失败
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int startNewSimluteReceiver(int sessionId, const char* sourceIP, int localPort, int duration);

private:
    RtpwareProxy();
    ~RtpwareProxy();

    //加载rtpware dll
    void loadRtpwareDLL();

    //卸载rtpware dll，减少引用数
    void unloadRtpwareDLL();

    //以下两个函数只定义不实现，防止出现类拷贝，杜绝 CClass object = CClass::getInstance()的出现
    RtpwareProxy(const RtpwareProxy&);
    RtpwareProxy& operator=(const RtpwareProxy&);

private:

    /************************************************************************/
    /* 定义一些rtpware dll导出函数的指针类型                                           */
    /************************************************************************/
    typedef int (* Func_computeGlobalTranStat)(TranStat* transtat);

    typedef int  (* Func_setSimluteReportServer)(const char* serverIP, const int port);

    typedef int  (* Func_startNewSimluteSender)(int sessionId, const  char* remoteIP, int remotePort, int sendRate, int dscp, int duration);

    typedef int  (* Func_startNewSimluteReceiver)(int sessionId, const char* sourceIP, int localPort, int duration);

private:
    HINSTANCE _hRtpwareDLL; //句柄
    Func_computeGlobalTranStat _computeGlobalTranStat;
    Func_setSimluteReportServer _setSimluteReportServer;
    Func_startNewSimluteSender _startNewSimluteSender;
    Func_startNewSimluteReceiver _startNewSimluteReceiver;
};


/*
导出函数名称，与rtpware dll的导出符号名称一致
*/
static  char* g_computeGlobalTranStat_name = "computeGlobalTranStat";
static  char* g_setSimluteReportServer_name = "setSimluteReportServer";
static  char* g_startNewSimluteSender_name = "startNewSimluteSender";
static  char* g_startNewSimluteReceiver_name = "startNewSimluteReceiver";

inline RtpwareProxy& RtpwareProxy::getInstance()
{
    static RtpwareProxy rwdllproxy;
    return rwdllproxy;
}


inline RtpwareProxy::RtpwareProxy()
{
    _hRtpwareDLL = NULL;
    _computeGlobalTranStat = NULL;
    _setSimluteReportServer = NULL;
    _startNewSimluteSender = NULL;
    _startNewSimluteReceiver = NULL;

    //加载rtpware dll
    loadRtpwareDLL();
}

inline RtpwareProxy::~RtpwareProxy()
{
    unloadRtpwareDLL();
}

inline void RtpwareProxy::loadRtpwareDLL()
{
    HINSTANCE hRtpwareDll = LoadLibraryA("rtpware.dll");            //从当前目录下查找加载
    if(hRtpwareDll == NULL)
    {
        hRtpwareDll = LoadLibraryA(RTPWARE_DLL_RELATIVE_FILE_NAME); //加载相对路径下的
        if(hRtpwareDll == NULL)
        {
            rwp_log("Could not load rtpware.dll, error code: 0x%x \n", GetLastError());
            return;
        }
    }
    //获取函数地址
    _computeGlobalTranStat = (Func_computeGlobalTranStat)GetProcAddress(hRtpwareDll, g_computeGlobalTranStat_name);
    _setSimluteReportServer = (Func_setSimluteReportServer)GetProcAddress(hRtpwareDll, g_setSimluteReportServer_name);
    _startNewSimluteSender = (Func_startNewSimluteSender)GetProcAddress(hRtpwareDll, g_startNewSimluteSender_name);
    _startNewSimluteReceiver = (Func_startNewSimluteReceiver)GetProcAddress(hRtpwareDll, g_startNewSimluteReceiver_name);

    if(!_computeGlobalTranStat && !_setSimluteReportServer && !_startNewSimluteSender && !_startNewSimluteReceiver)
    {
        rwp_log("faild to load rtpware func: %s\n", g_computeGlobalTranStat_name);
        unloadRtpwareDLL();         //如果所有函数都找不到地址，则卸载dll
    }
    else
        rwp_log("sucess to load rtpware dll and export func.\n");
}

inline void RtpwareProxy::unloadRtpwareDLL()
{
    if(_hRtpwareDLL)
    {
        if(FreeLibrary(_hRtpwareDLL) == 0)
            rwp_log("Could not unload rtpware dll.\n");
    }
    _computeGlobalTranStat = NULL;
    _setSimluteReportServer = NULL;
    _startNewSimluteSender = NULL;
    _startNewSimluteReceiver = NULL;
}


inline int RtpwareProxy::computeGlobalTranStat( TranStat* transtat )
{
    if (_computeGlobalTranStat)
        return _computeGlobalTranStat(transtat);
    else	return -1;
}

inline  int RtpwareProxy::setSimluteReportServer( const char* serverIP, const int port )
{
    if (_setSimluteReportServer)
        return _setSimluteReportServer(serverIP, port);
    else
        return -1;
}

inline int RtpwareProxy::startNewSimluteSender( int sessionId, const char* remoteIP, int remotePort, int sendRate, int dscp, int duration )
{
    if (_startNewSimluteSender)
        return _startNewSimluteSender(sessionId, remoteIP, remotePort,  sendRate,  dscp,  duration );
    else
        return -1;
}

inline  int RtpwareProxy::startNewSimluteReceiver( int sessionId, const char* sourceIP, int localPort, int duration )
{
    if (_startNewSimluteReceiver)
        return _startNewSimluteReceiver(sessionId, sourceIP, localPort, duration );
    else
        return -1;
}

#endif  RW_RtpwareProxy_H