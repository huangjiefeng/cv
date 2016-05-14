//取消 unicode
#ifdef _UNICODE
#undef    _UNICODE
#endif
#ifdef UNICODE
#undef  UNICODE
#endif
#include "NConfig.h"
#include "Ini.h"
#include "QoSDef.h"
#include <io.h>

//默认的ini配置文件名
#define  DEFAULT_CV_CONFIG_FILE_NAME "Coolview.ini"
#define  SECTION_NAME "Settings"
#define  KEY_LOCALSIMULATEPORT_NAME  "LocalSimulatePort"
#define  KEY_QOSSERVERSIPURI_NAME  "QosServerSipUri"
#define  KEY_QOSSERVERHOSTIP_NAME  "QosServerHostIP"
#define  KEY_QOSSERVERTCPPORT_NAME  "QosServerTcpPort"
#define  KEY_QOSSERVERUDPPORT_NAME  "QosServerUdpPort"
#define	 KEY_CURRENTCONFERENCEURI_NAME	"CurrentConferenceSipUri"
#define  KEY_CURRENTCONFERENCECID_NAME	"CurrentConferenceCid"
#define  KEY_ConfRoomQosServerUdpPort  "ConfRoomQosServerUdpPort"
#define  KEY_CoolviewQosServerUdpPort  "CoolviewQosServerUdpPort"
bool isFileExists(const char * filename)
{
	return	_access(filename, 0) != -1;
}
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

string getDefaultConfigFileName()
{
	string fileName;
	char buffer[200];
	memset(buffer,0,sizeof(buffer));
	if (getApplicationDirPath(buffer)==0)
	{
		fileName=buffer;
		fileName +="\\cfg\\";
		fileName += DEFAULT_CV_CONFIG_FILE_NAME;
	}
	else
	{
		fileName = DEFAULT_CV_CONFIG_FILE_NAME;
	}
	return fileName;
}



NConfig & NConfig::getInstance()
{
    static NConfig instance;
    return instance;
}

NConfig::NConfig(void)
{
	_configFileName = getDefaultConfigFileName();
	_ini = new CIni();	
	_ini->SetPathName(_configFileName.c_str());

    _isConfigLoaded = false;
    _localSimulatePort = DEFAULT_LOCAL_SIMULATE_TCP_PORT;
    _qosServerHostIP = DEFAULT_QOS_SERVER_HOST_IP;
	_qosServerSipUri =	DEFAULT_QOS_SERVER_SIP_URI;
    _qosServerUdpPort = DEFAULT_QOS_SERVER_UDP_PORT;
    _qosServerTcpPort = DEFAULT_QOS_SERVER_TCP_PORT;
	_currentConferenceUri = "";
	_currentConferenceCid = -1;
	_confRoomQosServerUdpPort = 0;
	_coolviewQosServerUdpPort = 0;

	loadConfig();
}


NConfig::~NConfig(void)
{
    if (_ini)
        delete _ini;
    _ini = NULL;
}


void NConfig::setConfigFileName( const char * filename )
{
    if (filename)
    {
        _configFileName = filename;
        _ini->SetPathName(filename);
    }
}

bool NConfig::loadConfig()
{
	//检测配置文件是否存在，如果不存在，则先保存配置文件
	if(!isFileExists(_configFileName.c_str()))
	{
		saveConfig();
	}
    else
    {
        _localSimulatePort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_LOCALSIMULATEPORT_NAME),
                                          DEFAULT_LOCAL_SIMULATE_TCP_PORT);
        char strBuffer[100];
        memset(strBuffer, 0, sizeof(strBuffer));
        DWORD len = _ini->GetString(_T(SECTION_NAME), _T(KEY_QOSSERVERHOSTIP_NAME),
                                    strBuffer, sizeof(strBuffer), _T(DEFAULT_QOS_SERVER_HOST_IP));
        if (len > 0)
            _qosServerHostIP = strBuffer;
		len = _ini->GetString(_T(SECTION_NAME), _T(KEY_QOSSERVERSIPURI_NAME),
			strBuffer, sizeof(strBuffer), _T(DEFAULT_QOS_SERVER_SIP_URI));
		if (len > 0)
			_qosServerSipUri = strBuffer;

        _qosServerTcpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_QOSSERVERTCPPORT_NAME),
                                         DEFAULT_QOS_SERVER_TCP_PORT);
        _qosServerUdpPort = _ini->GetInt(_T(SECTION_NAME), _T(KEY_QOSSERVERUDPPORT_NAME),
                                         DEFAULT_QOS_SERVER_UDP_PORT);

		len = _ini->GetString(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCEURI_NAME), strBuffer, sizeof(strBuffer));
		if(len > 0)
			_currentConferenceUri = strBuffer;
		_currentConferenceCid = _ini->GetInt(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCECID_NAME), -1);

		_confRoomQosServerUdpPort = _ini->GetInt(_T(SECTION_NAME) , _T(KEY_ConfRoomQosServerUdpPort) ,34543 );
		_coolviewQosServerUdpPort = _ini->GetInt(_T(SECTION_NAME) , _T(KEY_CoolviewQosServerUdpPort) ,34544 );
    }

    _isConfigLoaded = true;
    return true;
}

bool NConfig::saveConfig()
{
    if (setLocalSimulatePort(_localSimulatePort)  &&
            setQosServerHostIP(_qosServerHostIP.c_str()) &&
			setQosServerSipUri(_qosServerSipUri.c_str()) &&
            setQosServerUdpPort(_qosServerUdpPort) &&
            setQosServerTcpPort(_qosServerTcpPort) &&
			setCurrentConferenceUri(_currentConferenceUri.c_str()) &&
			setCurrentConferenceCid(_currentConferenceCid)
			)
    {
        _isConfigLoaded = true;
        return true;
    }
    return false;
}


bool NConfig::setLocalSimulatePort( int port )
{
    _localSimulatePort = port;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_LOCALSIMULATEPORT_NAME), port);
}

bool NConfig::setQosServerSipUri( const char * uri )
{
	_qosServerSipUri = uri != NULL ? uri : "";
	return _ini->WriteString(_T(SECTION_NAME), _T(KEY_QOSSERVERSIPURI_NAME), uri);
}

bool NConfig::setQosServerHostIP( const char * ip )
{
    _qosServerHostIP = ip != NULL ? ip : "";
    return _ini->WriteString(_T(SECTION_NAME), _T(KEY_QOSSERVERHOSTIP_NAME), ip);
}


bool NConfig::setQosServerUdpPort( int port )
{
    _qosServerUdpPort = port;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_QOSSERVERUDPPORT_NAME), port);
}

bool NConfig::setQosServerTcpPort( int port )
{
    _qosServerTcpPort = port;
    return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_QOSSERVERTCPPORT_NAME), port);
}

bool NConfig::setCurrentConferenceUri(const char *uri)
{
	_currentConferenceUri = uri;
	return _ini->WriteString(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCEURI_NAME), uri);
}
bool NConfig::setCurrentConferenceCid(const int cid)
{
	_currentConferenceCid = cid;
	return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_CURRENTCONFERENCECID_NAME), cid);
}

bool NConfig::setConfRoomQosServerUdpPort( const int port )
{
	_confRoomQosServerUdpPort = port;
	return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_ConfRoomQosServerUdpPort) , port );
}

bool NConfig::setCoolviewQosServerUdpPort( const int port )
{
	_coolviewQosServerUdpPort = port;
	return _ini->WriteInt(_T(SECTION_NAME), _T(KEY_CoolviewQosServerUdpPort) , port );
}