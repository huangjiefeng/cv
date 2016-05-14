#ifndef _NM_NCONFIG_H
#define _NM_NCONFIG_H


#include <string>
#include "NonCopyable.h"
using namespace std;

class CIni;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 配置管理类，单实例
////////////////////////////////////////////////////////////////////////////////////////////////////
class NConfig: public NonCopyable
{
public:
    static NConfig & getInstance();

    //加载配置文件，返回是否加载成功
    bool loadConfig();

    //设置配置文件的名称，默认名称是NetworkMeasurer.ini
    void setConfigFileName(const char * filename);

	bool isConfigLoaded() const;

    int getLocalSimulatePort() const;
    bool setLocalSimulatePort(int port);

    const char * getQosServerHostIP() const;
    bool setQosServerHostIP(const char * ip);

	const char * getQosServerSipUri() const;
	bool setQosServerSipUri(const char * uri);

    int getQosServerTcpPort() const;
    bool setQosServerTcpPort(int port);

    int getQosServerUdpPort() const;
    bool setQosServerUdpPort(int port);

	const char* getCurrentConferenceUri() const;
	bool setCurrentConferenceUri( const char* uri );

	int getCurrentConferenceCid() const;
	bool setCurrentConferenceCid( const int Cid );

	int getConfRoomQosServerUdpPort() const;
	bool setConfRoomQosServerUdpPort( const int port );

	int getCoolviewQosServerUdpPort() const;
	bool setCoolviewQosServerUdpPort( const int port );

	//保存配置，如果配置文件不存在，则保存配置文件
	bool saveConfig();

protected:
    NConfig(void);
    ~NConfig(void);

protected:
    CIni	* _ini;
	string _configFileName;
	bool _isConfigLoaded;		


	/************************************************************************/
	/* 配置字段                                                                     */
	/************************************************************************/

    int _localSimulatePort;		//本地主机网络测量监听的tcp端口
    string _qosServerHostIP;		//qos服务器主机IP
     string _qosServerSipUri;		//qos服务器SIP Uri
    int _qosServerTcpPort;		//qos服务器tcp端口，用于与QoS客户端通信
    int _qosServerUdpPort;		//qos服务器udp端口，用于接收测试统计报告
	
	string	_currentConferenceUri;	//当前正在进行的会议URI
	int		_currentConferenceCid;	//当前正在进行的会议ID
	int		_confRoomQosServerUdpPort;	//会议室UI接收Qos报告的端口
	int		_coolviewQosServerUdpPort;	//会议室UI接收Qos报告的端口
};

inline bool  NConfig::isConfigLoaded() const 
{ 
	return _isConfigLoaded; 
}

inline int NConfig::getLocalSimulatePort() const
{
	return _localSimulatePort;
}

inline const char * NConfig::getQosServerHostIP() const
{
	return _qosServerHostIP.c_str();
}

inline const char * NConfig::getQosServerSipUri() const
{
	return _qosServerSipUri.c_str();
}


inline int NConfig::getQosServerUdpPort() const
{
	return _qosServerUdpPort;
}

inline int NConfig::getQosServerTcpPort() const
{
	return _qosServerTcpPort;
}

inline const char* NConfig::getCurrentConferenceUri() const
{
	return _qosServerSipUri.c_str();
}

inline int NConfig::getCurrentConferenceCid() const
{
	return _currentConferenceCid;
}

inline int NConfig::getConfRoomQosServerUdpPort() const 
{
	return _confRoomQosServerUdpPort;
}

inline int NConfig::getCoolviewQosServerUdpPort() const 
{
	return _coolviewQosServerUdpPort;
}

#endif
