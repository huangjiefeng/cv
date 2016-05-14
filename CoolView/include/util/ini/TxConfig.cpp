
#pragma warning(disable:4995)

//取消 unicode
#ifdef _UNICODE
#undef    _UNICODE
#endif
#ifdef UNICODE
#undef  UNICODE
#endif

#include <fstream>
#include <windows.h>
#include <shlwapi.h>
#include "TxConfig.h"
#include "util\ini\Ini.h"
#ifdef TXMONITOR
#include "Util.h"
#endif


#define TX_CONFIG_FILE "tx.ini"


#define SEC_RECORD "Record"
#define SEC_CLEAN "Clean"
#define SEC_OPTIMIZER "Optimizer"
#define SEC_SCHEDULE "Schedule"
#define SEC_UPLOAD "Upload"
#define SEC_DOWNLOAD "Download"
#define SEC_SOAP "Soap"
#define SEC_SOAPC "SoapClient"


#define KEY_REC_PATH		"Path"
#define KEY_REC_FILE_NAME_FORMAT "FileNameFormat"
#define KEY_REC_LOCAL_FILE_NAME_FORMAT "LocalFileNameFormat"
#define KEY_REC_LOCAL_DURATION	"LocalRecDuration"
#define KEY_REC_MAX_FILE_SIZE	"MaxFileSize"
#define KEY_REC_MAX_FILE_DUR	"MaxFileDuration"
#define KEY_REC_AUTO_RECORD	"AutoRecord"
#define KEY_REC_AUTO_RECORD_DELAY	"AutoRecordDelay"
#define KEY_REC_UI_SCREEN_COUNT	"UIScreenCount"
#define KEY_REC_UI_SCREEN_TYPE	"UIScreenType"
#define KEY_OPT_CMD			"CMD"
#define KEY_OPT_PARAM		"Param"
#define KEY_CLN_LOW_SPACE	"LowSpace"
#define KEY_CLN_HIGH_SPACE	"HighSpace"
#define KEY_UPL_PATH		"Path"
#define KEY_UPL_MARK		"UploadedMark"
#define KEY_UPL_DIR_MARK	"UploadedDirMark"
#define KEY_DOWNL_PATH		"Path"
#define KEY_SCH_CHECK_INTERVAL "CheckInterval"
#define KEY_SCH_TRANS_UTIL "TransmitUtilization"
#define KEY_SCH_UPLOAD_UTIL "UploadUtilization"
#define KEY_SCH_UPLOAD_INC "UploadSpeedIncrement"
#define KEY_SOAP_LISTENING_PORT "ListeningPort"
#define KEY_SOAPC_SERVER "server"
#define KEY_SOAPC_REF_STAT_INTERVAL "RefreshStatusIntervalInSec"
#define KEY_SOAPC_ALLOW_GLOBAL_CTRL "AllowGlobalCtrl"
#define KEY_SOAPC_ALLOW_SELF_CTRL "AllowSelfCtrl"


#define DEFAULT_REC_PATH "txcache\\rec"
#define DEFAULT_REC_FILE_NAME_FORMAT "\\{confdate}_{conf}\\{date}\\{terminal}\\{title}_{datetime}_{camid}_{vinfo}.mp4"
#define DEFAULT_REC_LOCAL_FILE_NAME_FORMAT "\\LocalVideo\\{date}\\Local_{datetime}_{camid}_{vinfo}.mp4"
    //{}中的占位符要与tx_util.h中的一致
#define DEFAULT_REC_LOCAL_DURATION 600 // 600 sec
#define DEFAULT_REC_MAX_FILE_SIZE	(((LONGLONG)1 << 30) * 1.9) // 1.9G
#define DEFAULT_REC_MAX_FILE_DUR	(2 * 3600) // in sec, equals 2h
#define DEFAULT_REC_AUTO_RECORD	TRUE
#define DEFAULT_REC_AUTO_RECORD_DELAY	3
#define DEFAULT_REC_UI_SCREEN_COUNT	4 // 4屏幕
#define DEFAULT_REC_UI_SCREEN_TYPE	2 // IDisplayController::ScreenLayout中定义的kLayoutOf3x3
#define DEFAULT_OPT_CMD "tools\\mp4creator.exe"
#define DEFAULT_OPT_PARAM "-optimize"
#define DEFAULT_CLN_LOW_SPACE ((LONGLONG)1 << 30) // 1G
#define DEFAULT_CLN_HIGH_SPACE ((LONGLONG)1 << 31) // 2G
#define DEFAULT_UPL_PATH "txcache\\upload"
#define DEFAULT_UPL_MARK "_weblib_uploaded"
#define DEFAULT_UPL_DIR_MARK "_weblib_uploaded"
#define DEFAULT_DOWNL_PATH "txcache\\download"
#define DEFAULT_SCH_CHECK_INTERVAL 2000 // 2s
#define DEFAULT_SCH_TRANS_UTIL 0.8 // 80%带宽
#define DEFAULT_SCH_UPLOAD_UTIL 0.75 // 75%带宽
#define DEFAULT_SCH_UPLOAD_INC 500000 // 单位增量500k
#define DEFAULT_SOAP_LISTENING_PORT 8091
#define DEFAULT_SOAPC_SERVER ""
#define DEFAULT_SOAPC_REF_STAT_INTERVAL 2 // 2s
#define DEFAULT_SOAPC_ALLOW_GLOBAL_CTRL FALSE
#define DEFAULT_SOAPC_ALLOW_SELF_CTRL TRUE


CTxConfig * CTxConfig::instance = NULL;

// 为了迁移到CoolView项目
#ifdef TXMONITOR
#define getApplicationDirPath GetApplicationDirPath
#else
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
#endif


static std::string getDefualtConfigFileName()
{
	std::string fileName;
	char buffer[200];
	memset(buffer, 0, sizeof(buffer));
	if(getApplicationDirPath(buffer) == 0)
	{
		fileName = buffer;
		fileName += "\\cfg\\";
		fileName += TX_CONFIG_FILE;
	}
	else
	{
		fileName = TX_CONFIG_FILE;
	}
	return fileName;
}


CTxConfig & CTxConfig::getInstance()
{
	if (instance == NULL)
	{
		instance = new CTxConfig;
		instance->loadConfig();
	}
	return *instance;
}


CTxConfig::CTxConfig(void)
:_ini(NULL)
{
}


CTxConfig::~CTxConfig(void)
{
	if(_ini) delete _ini;
	_ini = NULL;
}


bool CTxConfig::loadConfig()
{
	if (s_strConfigFile.empty())
	{
		s_strConfigFile = getDefualtConfigFileName();
	}

	_ini = new CIni();
	_ini->SetPathName(s_strConfigFile.c_str());

	char strBuffer[MAX_PATH] = {0};
	DWORD len = 0;

	len = _ini->GetString(_T(SEC_RECORD), _T(KEY_REC_PATH), strBuffer, sizeof(strBuffer), _T(DEFAULT_REC_PATH));
	if(len > 0)
	{
		_rec_path = strBuffer;
  }

  len = _ini->GetString(_T(SEC_RECORD), _T(KEY_REC_FILE_NAME_FORMAT), 
    strBuffer, sizeof(strBuffer), _T(DEFAULT_REC_FILE_NAME_FORMAT));
  if(len > 0)
  {
    _rec_file_name_format = strBuffer;
  }

  len = _ini->GetString(_T(SEC_RECORD), _T(KEY_REC_LOCAL_FILE_NAME_FORMAT), 
    strBuffer, sizeof(strBuffer), _T(DEFAULT_REC_LOCAL_FILE_NAME_FORMAT));
  if(len > 0)
  {
    _rec_local_file_name_format = strBuffer;
  }

  _rec_local_duration = _ini->GetInt(_T(SEC_RECORD), _T(KEY_REC_LOCAL_DURATION), DEFAULT_REC_LOCAL_DURATION);
  _rec_max_file_size = _ini->GetInt64(_T(SEC_RECORD), _T(KEY_REC_MAX_FILE_SIZE), DEFAULT_REC_MAX_FILE_SIZE);
  _rec_max_file_duration = _ini->GetInt64(_T(SEC_RECORD), _T(KEY_REC_MAX_FILE_DUR), DEFAULT_REC_MAX_FILE_DUR);
  _rec_auto_record = _ini->GetBool(_T(SEC_RECORD), _T(KEY_REC_AUTO_RECORD), DEFAULT_REC_AUTO_RECORD);
  _rec_auto_record_delay = _ini->GetInt(_T(SEC_RECORD), _T(KEY_REC_AUTO_RECORD_DELAY), DEFAULT_REC_AUTO_RECORD_DELAY);
  _rec_ui_screen_count = _ini->GetInt(_T(SEC_RECORD), _T(KEY_REC_UI_SCREEN_COUNT), DEFAULT_REC_UI_SCREEN_COUNT);
  _rec_ui_screen_type = _ini->GetInt(_T(SEC_RECORD), _T(KEY_REC_UI_SCREEN_TYPE), DEFAULT_REC_UI_SCREEN_TYPE);

	len = _ini->GetString(_T(SEC_OPTIMIZER), _T(KEY_OPT_CMD), strBuffer, sizeof(strBuffer), _T(DEFAULT_OPT_CMD));
	if(len > 0)
	{
		_opt_cmd = strBuffer;
	}

	len = _ini->GetString(_T(SEC_OPTIMIZER), _T(KEY_OPT_PARAM), strBuffer, sizeof(strBuffer), _T(DEFAULT_OPT_PARAM));
	if(len > 0)
	{
		_opt_param = strBuffer;
	}

	_cln_low_space = _ini->GetInt64(_T(SEC_CLEAN), _T(KEY_CLN_LOW_SPACE), DEFAULT_CLN_LOW_SPACE);
	_cln_high_space = _ini->GetInt64(_T(SEC_CLEAN), _T(KEY_CLN_HIGH_SPACE), DEFAULT_CLN_HIGH_SPACE);

	len = _ini->GetString(_T(SEC_UPLOAD), _T(KEY_UPL_PATH), strBuffer, sizeof(strBuffer), _T(DEFAULT_UPL_PATH));
	if(len > 0)
	{
		_upload_path = strBuffer;
	}

	len = _ini->GetString(_T(SEC_UPLOAD), _T(KEY_UPL_MARK), strBuffer, sizeof(strBuffer), _T(DEFAULT_UPL_MARK));
	if(len > 0)
	{
		_upload_mark = strBuffer;
	}

	len = _ini->GetString(_T(SEC_UPLOAD), _T(KEY_UPL_DIR_MARK), strBuffer, sizeof(strBuffer), _T(DEFAULT_UPL_DIR_MARK));
	if(len > 0)
	{
		_upload_dir_mark = strBuffer;
    }

    len = _ini->GetString(_T(SEC_DOWNLOAD), _T(KEY_DOWNL_PATH), strBuffer, sizeof(strBuffer), _T(DEFAULT_DOWNL_PATH));
    if(len > 0)
    {
        _download_path = strBuffer;
    }

	_sch_check_interval = _ini->GetInt(_T(SEC_SCHEDULE), _T(KEY_SCH_CHECK_INTERVAL), DEFAULT_SCH_CHECK_INTERVAL);
	_sch_transmit_utilization = _ini->GetDouble(_T(SEC_SCHEDULE), _T(KEY_SCH_TRANS_UTIL), DEFAULT_SCH_TRANS_UTIL);
	_sch_upload_utilization = _ini->GetDouble(_T(SEC_SCHEDULE), _T(KEY_SCH_UPLOAD_UTIL), DEFAULT_SCH_UPLOAD_UTIL);
  _sch_upload_speed_increment = _ini->GetInt(_T(SEC_SCHEDULE), _T(KEY_SCH_UPLOAD_INC), DEFAULT_SCH_UPLOAD_INC);

  _soap_listening_port = _ini->GetInt(_T(SEC_SOAP), _T(KEY_SOAP_LISTENING_PORT), DEFAULT_SOAP_LISTENING_PORT);

  len = _ini->GetString(_T(SEC_SOAPC), _T(KEY_SOAPC_SERVER), strBuffer, sizeof(strBuffer), _T(DEFAULT_SOAPC_SERVER));
  if(len > 0)
  {
    _soapc_server = strBuffer;
  }
  _soapc_refresh_status_interval_in_sec = _ini->GetInt(_T(SEC_SOAPC), _T(KEY_SOAPC_REF_STAT_INTERVAL), DEFAULT_SOAPC_REF_STAT_INTERVAL);
  _soapc_allow_global_ctrl = _ini->GetBool(_T(SEC_SOAPC), _T(KEY_SOAPC_ALLOW_GLOBAL_CTRL), DEFAULT_SOAPC_ALLOW_GLOBAL_CTRL);
  _soapc_all_self_ctrl = _ini->GetBool(_T(SEC_SOAPC), _T(KEY_SOAPC_ALLOW_SELF_CTRL), DEFAULT_SOAPC_ALLOW_SELF_CTRL);

	saveConfig(); //对不存在的配置保存默认参数

	return true;
}

bool CTxConfig::saveConfig()
{
	bool ret = true;
	ret = ret && SetRecPath(_rec_path.c_str());
  ret = ret && SetRecFileNameFormat(_rec_file_name_format);
  ret = ret && SetRecLocalFileNameFormat(_rec_local_file_name_format);
  ret = ret && SetRecLocalDuration(_rec_local_duration);
  ret = ret && SetRecMaxFileSize(_rec_max_file_size);
  ret = ret && SetRecMaxFileDuration(_rec_max_file_duration);
  ret = ret && SetRecAutoStart(_rec_auto_record);
  ret = ret && SetRecAutoStartDelay(_rec_auto_record_delay);
  ret = ret && SetRecUiScreenCount(_rec_ui_screen_count);
  ret = ret && SetRecUiScreenType(_rec_ui_screen_type);
	ret = ret && SetOptCmd(_opt_cmd.c_str());
	ret = ret && SetOptParam(_opt_param.c_str());
	ret = ret && SetClnLowSpace(_cln_low_space);
	ret = ret && SetClnHighSpace(_cln_high_space);
	ret = ret && SetUploadPath(_upload_path.c_str());
	ret = ret && SetUploadMark(_upload_mark.c_str());
    ret = ret && SetUploadDirMark(_upload_dir_mark.c_str());
    ret = ret && SetDownloadPath(_download_path.c_str());
	ret = ret && SetSchCheckInterval(_sch_check_interval);
	ret = ret && SetSchTransmitUtilization(_sch_transmit_utilization);
	ret = ret && SetSchUploadUtilization(_sch_upload_utilization);
	ret = ret && SetSchUploadSpeedIncrement(_sch_upload_speed_increment);
  ret = ret && SetSoapListeningPort(_soap_listening_port);
  ret = ret && SetSoapCServer(_soapc_server.c_str());
  ret = ret && SetSoapCRefreshStatusInterval(_soapc_refresh_status_interval_in_sec);
  ret = ret && SetSoapCAllowGlobalCtrl(_soapc_allow_global_ctrl);
  ret = ret && SetSoapCAllowSelfCtrl(_soapc_all_self_ctrl);

	return ret;
}

const char * CTxConfig::GetRecPath()
{
	return _rec_path.c_str();
}

bool CTxConfig::SetRecPath(const char * path)
{
	_rec_path = (path == NULL ? DEFAULT_REC_PATH : path);
	return _ini->WriteString(_T(SEC_RECORD), _T(KEY_REC_PATH), path);
}

bool CTxConfig::SetRecFileNameFormat( std::string val )
{
  _rec_file_name_format = val;
  return _ini->WriteString(_T(SEC_RECORD), _T(KEY_REC_FILE_NAME_FORMAT), 
    _rec_file_name_format.c_str());
}

bool CTxConfig::SetRecLocalFileNameFormat( std::string val )
{
  _rec_local_file_name_format = val;
  return _ini->WriteString(_T(SEC_RECORD), _T(KEY_REC_LOCAL_FILE_NAME_FORMAT), 
    _rec_local_file_name_format.c_str());
}

const int CTxConfig::GetRecLocalDuration()
{
	return _rec_local_duration;
}

bool CTxConfig::SetRecLocalDuration(int sec)
{
	if (0 > sec)
	{
		return false;
	}
	if (0 < sec && 20 > sec)
	{
		sec = 20; // 自动分割间隔不得小于20s，防止文件太细碎，也防止不必要的问题
	}
	_rec_local_duration = sec;
	return _ini->WriteInt(_T(SEC_RECORD), _T(KEY_REC_LOCAL_DURATION), sec);
}

const __int64 CTxConfig::GetRecMaxFileSize()
{
  return _rec_max_file_size;
}

bool CTxConfig::SetRecMaxFileSize( __int64 size )
{
  if (size < 0)
  {
    return false;
  }
  _rec_max_file_size = size;
  return _ini->WriteInt64(_T(SEC_RECORD), _T(KEY_REC_MAX_FILE_SIZE), size);
}

const __int64 CTxConfig::GetRecMaxFileDuration()
{
  return _rec_max_file_duration;
}

bool CTxConfig::SetRecMaxFileDuration( __int64 dur )
{
  if (dur < 0)
  {
    return false;
  }
  _rec_max_file_duration = dur;
  return _ini->WriteInt64(_T(SEC_RECORD), _T(KEY_REC_MAX_FILE_DUR), dur);
}

const char * CTxConfig::GetOptCmd()
{
	return _opt_cmd.c_str();
}

bool CTxConfig::SetOptCmd(const char * cmd)
{
	_opt_cmd = (cmd == NULL ? "" : cmd);
	return _ini->WriteString(_T(SEC_OPTIMIZER), _T(KEY_OPT_CMD), cmd);
}

const char * CTxConfig::GetOptParam()
{
	return _opt_param.c_str();
}

bool CTxConfig::SetOptParam(const char * param)
{
	_opt_param = (param == NULL ? "" : param);
	return _ini->WriteString(_T(SEC_OPTIMIZER), _T(KEY_OPT_PARAM), param);
}

const char * CTxConfig::GetUploadPath()
{
	return _upload_path.c_str();
}

bool CTxConfig::SetUploadPath(const char * path)
{
	_upload_path = (path == NULL ? DEFAULT_UPL_PATH : path);
	return _ini->WriteString(_T(SEC_UPLOAD), _T(KEY_UPL_PATH), path);
}

const char * CTxConfig::GetUploadMark()
{
	return _upload_mark.c_str();
}

bool CTxConfig::SetUploadMark(const char * mark)
{
	_upload_mark = (mark == NULL ? DEFAULT_UPL_MARK : mark);
	return _ini->WriteString(_T(SEC_UPLOAD), _T(KEY_UPL_MARK), mark);
}

const char * CTxConfig::GetUploadDirMark()
{
	return _upload_dir_mark.c_str();
}

bool CTxConfig::SetUploadDirMark(const char * mark)
{
	_upload_dir_mark = (mark == NULL ? DEFAULT_UPL_DIR_MARK : mark);
	return _ini->WriteString(_T(SEC_UPLOAD), _T(KEY_UPL_DIR_MARK), mark);
}

const char * CTxConfig::GetDownloadPath()
{
    return _download_path.c_str();
}

bool CTxConfig::SetDownloadPath(const char * path)
{
    _download_path = (path == NULL ? DEFAULT_DOWNL_PATH : path);
    return _ini->WriteString(_T(SEC_DOWNLOAD), _T(KEY_DOWNL_PATH), path);
}

const __int64 CTxConfig::GetClnLowSpace()
{
	return _cln_low_space;
}

bool CTxConfig::SetClnLowSpace(__int64 space)
{
	if (space < 0)
	{
		return false;
	}
	_cln_low_space = space;
	return _ini->WriteInt64(_T(SEC_CLEAN), _T(KEY_CLN_LOW_SPACE), space);
}

const __int64 CTxConfig::GetClnHighSpace()
{
	return _cln_high_space;
}

bool CTxConfig::SetClnHighSpace(__int64 space)
{
	if (space < _cln_low_space)
	{
		return false;
	}
	_cln_high_space = space;
	return _ini->WriteInt64(_T(SEC_CLEAN), _T(KEY_CLN_HIGH_SPACE), space);
}

const int CTxConfig::GetSchCheckInterval()
{
  return _sch_check_interval;
}

bool CTxConfig::SetSchCheckInterval( const int interval )
{
  if (500 >= interval)
  {
    // 不能小于500ms，防止检查过频繁
    return false;
  }
  _sch_check_interval = interval;
  return _ini->WriteInt(_T(SEC_SCHEDULE), _T(KEY_SCH_CHECK_INTERVAL), interval);
}

const double CTxConfig::GetSchTransmitUtilization()
{
  return _sch_transmit_utilization;
}

bool CTxConfig::SetSchTransmitUtilization( const double util )
{
  if (0.1 > util || 1.0 < util)
  {
    // 带宽利用率不能大于100%或小于10%
    return false;
  }
  _sch_transmit_utilization = util;
  return _ini->WriteDouble(_T(SEC_SCHEDULE), _T(KEY_SCH_TRANS_UTIL), util);
}

const double CTxConfig::GetSchUploadUtilization()
{
  return _sch_upload_utilization;
}

bool CTxConfig::SetSchUploadUtilization( const double util )
{
  if (0.1 > util || 1.0 < util)
  {
    // 带宽利用率不能大于100%或小于10%
    // TODO:上传利用率还应小于传输利用率_sch_transmit_utilization，为防止频繁
    // 的改变上传限速，_sch_upload_utilization应小于_sch_transmit_utilization
    return false;
  }
  _sch_upload_utilization = util;
  return _ini->WriteDouble(_T(SEC_SCHEDULE), _T(KEY_SCH_UPLOAD_UTIL), util);
}

const int CTxConfig::GetSchUploadSpeedIncrement()
{
  return _sch_upload_speed_increment;
}

bool CTxConfig::SetSchUploadSpeedIncrement( const int inc )
{
  if (0 >= inc)
  {
    // 不能小于或等于0byte
    return false;
  }
  _sch_upload_speed_increment = inc;
  return _ini->WriteInt(_T(SEC_SCHEDULE), _T(KEY_SCH_UPLOAD_INC), inc);
}

const bool CTxConfig::GetRecAutoStart()
{
  return _rec_auto_record;
}

bool CTxConfig::SetRecAutoStart( bool b )
{
  _rec_auto_record = b;
  return _ini->WriteBool(_T(SEC_RECORD), _T(KEY_REC_AUTO_RECORD), b);
}

bool CTxConfig::SetRecAutoStartDelay(int ms)
{
  _rec_auto_record_delay = ms;
  return _ini->WriteInt(_T(SEC_RECORD), _T(KEY_REC_AUTO_RECORD_DELAY), ms);
}

bool CTxConfig::SetRecUiScreenCount( int val )
{
  _rec_ui_screen_count = val;
  return _ini->WriteInt(_T(SEC_RECORD), _T(KEY_REC_UI_SCREEN_COUNT), val);
}

bool CTxConfig::SetRecUiScreenType( int val )
{
  _rec_ui_screen_type = val;
  return _ini->WriteInt(_T(SEC_RECORD), _T(KEY_REC_UI_SCREEN_TYPE), val);
}

bool CTxConfig::SetSoapListeningPort( int port )
{
  if (port <= 0) {
    return false;
  }
  return _ini->WriteInt(_T(SEC_SOAP), _T(KEY_SOAP_LISTENING_PORT), port);
}

bool CTxConfig::SetSoapCServer( const char *val )
{
  _soapc_server = (val == NULL ? DEFAULT_SOAPC_SERVER : val);
  return _ini->WriteString(_T(SEC_SOAPC), _T(KEY_SOAPC_SERVER), val);
}

bool CTxConfig::SetSoapCRefreshStatusInterval( int val )
{
  _soapc_refresh_status_interval_in_sec = val;
  return _ini->WriteInt(_T(SEC_SOAPC), _T(KEY_SOAPC_REF_STAT_INTERVAL), val);
}

bool CTxConfig::SetSoapCAllowGlobalCtrl( bool val )
{
  _soapc_allow_global_ctrl = val;
  return _ini->WriteBool(_T(SEC_SOAPC), _T(KEY_SOAPC_ALLOW_GLOBAL_CTRL), val);
}

bool CTxConfig::SetSoapCAllowSelfCtrl( bool val )
{
  _soapc_all_self_ctrl = val;
  return _ini->WriteBool(_T(SEC_SOAPC), _T(KEY_SOAPC_ALLOW_SELF_CTRL), val);
}


