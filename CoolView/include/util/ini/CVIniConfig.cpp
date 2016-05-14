
//取消 unicode
#ifdef _UNICODE
#undef    _UNICODE
#endif
#ifdef UNICODE
#undef  UNICODE
#endif

#include "CVIniConfig.h"
#include "util\ini\Ini.h"
#include "util\ini\QoSDef.h"
#include <io.h>

//默认的ini配置文件名
#define  DEFAULT_CV_CONFIG_FILE_NAME "CoolView.ini"

#define  SECTION_NAME "Settings"
#define  CONFROOM_SECTION_NAME "ConfRoom"

// global settings
#define  KEY_QOSSERVERSIPURI_NAME  "QosServerSipUri"
#define  KEY_QOSSERVERHOSTIP_NAME  "QosServerHostIP"
#define  KEY_QOSSERVERTCPPORT_NAME  "QosServerTcpPort"
#define  KEY_QOSSERVERUDPPORT_NAME  "QosServerUdpPort"
#define  KEY_CURRENTCONFERENCEURI_NAME  "CurrentConferenceSipUri"
#define  KEY_CURRENTCONFERENCECID_NAME  "CurrentConferenceCid"
#define  KEY_UseCustomedDscp  "UseCustomedDscp"  //值为true或false
#define  KEY_VideoDscp  "VideoDscp"
#define  KEY_AudioDscp  "AudioDscp"
#define  KEY_CoolviewQosServerUdpPort	"CoolViewQosServerUdpPort"
#define  KEY_ConfRoomQosServerUdpPort  "ConfRoomQosServerUdpPort"
#define  KEY_MODEL_CATEGORY   "ModelCategory"
#define  KEY_USE_MULTI_THREAD "UseMultiThread"
#define  KEY_ENABLE_VIRTUAL_CONF "EnableVirtualConference"

// confroom settings
#define  KEY_USE_TRANSPARENT_SUBTITLE "UseTransparentSubtitle"
#define  KEY_AUTO_SET_PRIMARY_SCREEN "AutoSetPrimaryScreen"


static int getApplicationDirPath(char* applicationDirPath)
{
    char szAppPath[MAX_PATH] = "";
    if(!applicationDirPath)
        return -1;
    if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
    {
        //printf("failed to get module file name!\n");
        return -1;
    }
    // Extract directory
    strncpy(applicationDirPath, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
    applicationDirPath[strlen(applicationDirPath)] = '\0';
    return 0;
}


string getDefualtConfigFileName()
{
    string fileName;
    char buffer[200];
    memset(buffer, 0, sizeof(buffer));
    if(getApplicationDirPath(buffer) == 0)
    {
        fileName = buffer;
        fileName += "\\cfg\\";
        fileName += DEFAULT_CV_CONFIG_FILE_NAME;
    }
    else
    {
        fileName = DEFAULT_CV_CONFIG_FILE_NAME;
    }
    return fileName;
}


bool isFileExists(const char* filename)
{
    return  _access(filename, 0) != -1;
}

CVIniConfig& CVIniConfig::getInstance()
{
    static CVIniConfig instance;
    return instance;
}

CVIniConfig::CVIniConfig(void)
{
    _configFileName = getDefualtConfigFileName();
    _ini = new CIni();
    _ini->SetPathName(_configFileName.c_str());

    _isConfigLoaded = false;
    _qosServerHostIP = DEFAULT_QOS_SERVER_HOST_IP;
    _qosServerSipUri = DEFAULT_QOS_SERVER_SIP_URI;
    _qosServerUdpPort = DEFAULT_QOS_SERVER_UDP_PORT;
    _qosServerTcpPort = DEFAULT_QOS_SERVER_TCP_PORT;
    _currentConferenceUri = "";
    _currentConferenceCid = -1;
    _useCustomedDscp = false;
    _videoDscp = 0;
    _audioDscp = 0;
	_coolviewQosServerUdpPort = 34544;
	_modelCategory = CVCFG_VALUE_MODEL_CATEGORY_HD;
    _useMultiThread = false;
    _enableVirtualConference = false;
    _isAutoSetPrimaryScreen = true;
    //加载配置
    loadConfig();
}


CVIniConfig::~CVIniConfig(void)
{
    if(_ini)
        delete _ini;
    _ini = NULL;
}


void CVIniConfig::setConfigFileName(const char* filename)
{
    if(filename)
    {
        _configFileName = filename;
        _ini->SetPathName(TEXT(filename));
    }
}

bool CVIniConfig::loadConfig()
{
	char strBuffer[200];
	memset(strBuffer, 0, sizeof(strBuffer));
	DWORD len = _ini->GetString(_T(SECTION_NAME), _T(KEY_QOSSERVERHOSTIP_NAME), strBuffer, sizeof(strBuffer), _T(DEFAULT_QOS_SERVER_HOST_IP));
	if(len > 0)
		_qosServerHostIP = strBuffer;
	len = _ini->GetString(_T(SECTION_NAME), _T(KEY_QOSSERVERSIPURI_NAME), strBuffer, sizeof(strBuffer), _T(DEFAULT_QOS_SERVER_SIP_URI));
	if(len > 0)
		_qosServerSipUri = strBuffer;

	_qosServerTcpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_QOSSERVERTCPPORT_NAME), DEFAULT_QOS_SERVER_TCP_PORT);
	_qosServerUdpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_QOSSERVERUDPPORT_NAME), DEFAULT_QOS_SERVER_UDP_PORT);

	len = _ini->GetString(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCEURI_NAME), strBuffer, sizeof(strBuffer), _T(""));
	if(len > 0)
		_currentConferenceUri = strBuffer;

	_currentConferenceCid = _ini->GetInt(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCECID_NAME), -1);
	_useCustomedDscp = _ini->GetBool(_T(SECTION_NAME), _T(KEY_UseCustomedDscp), false);
	_videoDscp = _ini->GetInt(_T(SECTION_NAME), _T(KEY_VideoDscp), 0);
	_audioDscp = _ini->GetInt(_T(SECTION_NAME), _T(KEY_AudioDscp), 0);
  _coolviewQosServerUdpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_CoolviewQosServerUdpPort), 34544);
  _confRoomQosServerUdpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_ConfRoomQosServerUdpPort), 34543);
	_modelCategory = _ini->GetInt(_T(SECTION_NAME), _T(KEY_MODEL_CATEGORY), CVCFG_VALUE_MODEL_CATEGORY_HD);
    _useMultiThread = _ini->GetInt(_T(SECTION_NAME), _T(KEY_USE_MULTI_THREAD), false);
    _enableVirtualConference = _ini->GetInt(_T(SECTION_NAME), _T(KEY_ENABLE_VIRTUAL_CONF), false);
  _useTransparentSubtitle = _ini->GetInt(_T(CONFROOM_SECTION_NAME), _T(KEY_USE_TRANSPARENT_SUBTITLE), false);
  _isAutoSetPrimaryScreen = _ini->GetBool(_T(CONFROOM_SECTION_NAME), _T(KEY_AUTO_SET_PRIMARY_SCREEN), TRUE);
  _isConfigLoaded = true;

	//检测配置文件是否存在，如果不存在，则先保存配置文件
	if(!isFileExists(_configFileName.c_str()))
	{
		saveConfig();
	}

    return true;
}

bool CVIniConfig::saveConfig()
{
    setQosServerHostIP(_qosServerHostIP.c_str());
    setQosServerSipUri(_qosServerSipUri.c_str());
    setQosServerUdpPort(_qosServerUdpPort);
    setQosServerTcpPort(_qosServerTcpPort);
    setCurrentConferenceUri(_currentConferenceUri.c_str());
    setCurrentConferenceCid(_currentConferenceCid);
    setUseCustomedDscp(_useCustomedDscp);
    setVideoDscp(_videoDscp);
    setAudioDscp(_audioDscp);
	setCoolviewQosServerUdpPort(_coolviewQosServerUdpPort);
  setConfRoomQosServerUdpPort(_confRoomQosServerUdpPort);
	setModelCategory(_modelCategory);
    setMultiThread(_useMultiThread);
    setVirtualConferenceEnable(_enableVirtualConference);
    setTransparentSubtitle(_useTransparentSubtitle);
    setAutoSetPrimaryScreen(_isAutoSetPrimaryScreen);
    _isConfigLoaded = true;
    return true;
}

bool CVIniConfig::setQosServerSipUri(const char* uri)
{
    _qosServerSipUri = uri != NULL ? uri : "";
    return _ini->WriteString(_T(SECTION_NAME), _T(KEY_QOSSERVERSIPURI_NAME), uri);
}

bool CVIniConfig::setQosServerHostIP(const char* ip)
{
    _qosServerHostIP = ip != NULL ? ip : "";
    return _ini->WriteString(_T(SECTION_NAME), _T(KEY_QOSSERVERHOSTIP_NAME), ip);
}

bool CVIniConfig::setQosServerUdpPort(int port)
{
    _qosServerUdpPort = port;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_QOSSERVERUDPPORT_NAME), port);
}

bool CVIniConfig::setQosServerTcpPort(int port)
{
    _qosServerTcpPort = port;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_QOSSERVERTCPPORT_NAME), port);
}


bool CVIniConfig::setCurrentConferenceUri(const char* uri)
{
    _currentConferenceUri = uri;
    return _ini->WriteString(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCEURI_NAME), uri);
}


bool CVIniConfig::setCurrentConferenceCid(const int Cid)
{
    _currentConferenceCid = Cid;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCECID_NAME), Cid);
}

void CVIniConfig::setUseCustomedDscp(bool val)
{
    _useCustomedDscp = val;
    _ini->WriteBool(_T(SECTION_NAME), _T(KEY_UseCustomedDscp), val);
}

void CVIniConfig::setVideoDscp(int dscp)
{
    if(dscp < 0 || dscp > 63)
        return;
    _videoDscp = dscp;
    _ini->WriteInt(_T(SECTION_NAME), _T(KEY_VideoDscp), dscp);
}

void CVIniConfig::setAudioDscp(int dscp)
{
    if(dscp < 0 || dscp > 63)
        return;
    _audioDscp = dscp;
    _ini->WriteInt(_T(SECTION_NAME), _T(KEY_AudioDscp), dscp);
}

bool CVIniConfig::setCoolviewQosServerUdpPort( const int port )
{
	_coolviewQosServerUdpPort = port;
	return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_CoolviewQosServerUdpPort) , port );
}

bool CVIniConfig::setConfRoomQosServerUdpPort( const int port )
{
  _confRoomQosServerUdpPort = port;
  return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_ConfRoomQosServerUdpPort) , port );
}

bool CVIniConfig::setModelCategory( int cate )
{
	_modelCategory = cate;
	return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_MODEL_CATEGORY) , cate );
}

void CVIniConfig::setMultiThread( bool use_multi_thread ) {
  _useMultiThread = use_multi_thread;
  _ini->WriteBool(_T(SECTION_NAME), _T(KEY_USE_MULTI_THREAD), use_multi_thread);
}

void CVIniConfig::setTransparentSubtitle( bool val ) { 
  _useTransparentSubtitle = val; 
  _ini->WriteBool(_T(CONFROOM_SECTION_NAME), _T(KEY_USE_TRANSPARENT_SUBTITLE), 
    _useTransparentSubtitle);
}

void CVIniConfig::setVirtualConferenceEnable( bool val )
{
    _enableVirtualConference = val;
    _ini->WriteBool(_T(SECTION_NAME), _T(KEY_ENABLE_VIRTUAL_CONF), _enableVirtualConference);
}

void CVIniConfig::setAutoSetPrimaryScreen( bool val )
{
  _isAutoSetPrimaryScreen = val;
  _ini->WriteBool(_T(CONFROOM_SECTION_NAME), _T(KEY_AUTO_SET_PRIMARY_SCREEN), 
    _isAutoSetPrimaryScreen);
}
