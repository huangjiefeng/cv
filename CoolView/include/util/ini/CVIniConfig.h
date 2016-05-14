#ifndef _NM_NCONFIG_H
#define _NM_NCONFIG_H

#include <string>
using namespace std;

#define CVCFG_VALUE_MODEL_CATEGORY_HD      0 
#define CVCFG_VALUE_MODEL_CATEGORY_TX      1 

class CIni;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ini配置管理类，单实例
////////////////////////////////////////////////////////////////////////////////////////////////////
class CVIniConfig
{
public:
    static CVIniConfig & getInstance();

    //加载配置文件，返回是否加载成功
    bool loadConfig();

    //设置配置文件的名称，默认名称是NetworkMeasurer.ini
    void setConfigFileName(const char * filename);

	bool isConfigLoaded() const;

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

    // 是否强制使用自定义DSCP（非QoS服务器分配的）
    bool isUseCustomedDscp() const { return _useCustomedDscp; }
    void setUseCustomedDscp(bool val);

    int getVideoDscp() const { return _videoDscp; }
    void setVideoDscp(int dscp);

    int getAudioDscp() const { return _audioDscp; }
    void setAudioDscp(int dscp);


	//保存配置，如果配置文件不存在，则保存配置文件
	bool saveConfig();


	//获取Coolview接收udp数据的端口
	int getCoolviewQosServerUdpPort() const;
  bool setCoolviewQosServerUdpPort( const int port );

  int getConfRoomQosServerUdpPort() const;
  bool setConfRoomQosServerUdpPort( const int port );

	int getModelCategory() const;
	bool setModelCategory(int cate);

    bool IsModelTX() const;
    bool IsModelHD() const;

    bool useMultiThread() const { return _useMultiThread; }
    void setMultiThread(bool use_multi_thread);

    bool isVirtualConferenceEnable() const { return _enableVirtualConference; }
    void setVirtualConferenceEnable(bool val);

    bool useTransparentSubtitle() const { return _useTransparentSubtitle; }
    void setTransparentSubtitle(bool val);

    bool isAutoSetPrimaryScreen() const { return _isAutoSetPrimaryScreen; }
    void setAutoSetPrimaryScreen(bool val);

protected:
    CVIniConfig(void);
    ~CVIniConfig(void);

	//以下两个函数只定义不实现，防止出现类拷贝
	CVIniConfig(const CVIniConfig&);
	CVIniConfig& operator=(const CVIniConfig&);

protected:
  CIni	* _ini;
	string _configFileName;
	bool _isConfigLoaded;		

	/************************************************************************/
	/* 配置字段                                                                     */
	/************************************************************************/
  string _qosServerHostIP;		//qos服务器主机IP
  string _qosServerSipUri;		//qos服务器SIP Uri
  int _qosServerTcpPort;		//qos服务器tcp端口，用于与QoS客户端通信
  int _qosServerUdpPort;		//qos服务器udp端口，用于接收测试统计报告
    
	string	_currentConferenceUri;	//当前正在进行的会议URI
	int		_currentConferenceCid;	//当前正在进行的会议ID
	bool _useCustomedDscp;     //是否强制使用自定义DSCP（非QoS服务器分配的）
  int  _videoDscp;
  int _audioDscp;  

  int _coolviewQosServerUdpPort;	//Coolview接收Qos报告的端口
  int		_confRoomQosServerUdpPort;	//会议室UI接收Qos报告的端口

	int _modelCategory;  //型号类别
  bool _useMultiThread;
  bool _enableVirtualConference; //是否允许本地虚拟会议

  //TODO:ConfRoom配置项，移到单独配置文件中？
  bool _useTransparentSubtitle;

  bool _isAutoSetPrimaryScreen;
};

inline bool  CVIniConfig::isConfigLoaded() const 
{ 
	return _isConfigLoaded; 
}


inline const char * CVIniConfig::getQosServerHostIP() const
{
	return _qosServerHostIP.c_str();
}

inline const char * CVIniConfig::getQosServerSipUri() const
{
	return _qosServerSipUri.c_str();
}


inline int CVIniConfig::getQosServerUdpPort() const
{
	return _qosServerUdpPort;
}

inline int CVIniConfig::getQosServerTcpPort() const
{
	return _qosServerTcpPort;
}

inline const char* CVIniConfig::getCurrentConferenceUri() const
{
	return _qosServerSipUri.c_str();
}

inline int CVIniConfig::getCurrentConferenceCid() const
{
	return _currentConferenceCid;
}

inline int CVIniConfig::getCoolviewQosServerUdpPort() const 
{
	return _coolviewQosServerUdpPort;
}

inline int CVIniConfig::getConfRoomQosServerUdpPort() const 
{
  return _confRoomQosServerUdpPort;
}

inline int CVIniConfig::getModelCategory() const
{
	return _modelCategory;
}

inline bool CVIniConfig::IsModelTX() const {
  return _modelCategory == CVCFG_VALUE_MODEL_CATEGORY_TX;
}

inline bool CVIniConfig::IsModelHD() const {
  return _modelCategory == CVCFG_VALUE_MODEL_CATEGORY_HD;
}

#endif
