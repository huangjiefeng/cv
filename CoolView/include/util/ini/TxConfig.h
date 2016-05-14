#pragma once

#include <string>

class CIni;

class CTxConfig
{
public:

	// static func
	static CTxConfig & getInstance();

	// member func
	const char * GetRecPath();
  bool         SetRecPath(const char * path);

  std::string GetRecFileNameFormat() const { return _rec_file_name_format; }
  bool SetRecFileNameFormat(std::string val);

  std::string GetRecLocalFileNameFormat() const { return _rec_local_file_name_format; }
  bool SetRecLocalFileNameFormat(std::string val);

	const int    GetRecLocalDuration();
	bool         SetRecLocalDuration(int sec);

  const __int64 GetRecMaxFileSize();
  bool          SetRecMaxFileSize(__int64 size);

  const __int64 GetRecMaxFileDuration();
  bool          SetRecMaxFileDuration(__int64 size);

  const bool GetRecAutoStart();
  bool       SetRecAutoStart(bool b);

  const int GetRecAutoStartDelay() { return _rec_auto_record_delay; }
  bool      SetRecAutoStartDelay(int ms);

  int GetRecUiScreenCount() const { return _rec_ui_screen_count; }
  bool SetRecUiScreenCount(int val);

  int GetRecUiScreenType() const { return _rec_ui_screen_type; }
  bool SetRecUiScreenType(int val);

	const char * GetOptCmd();
	bool         SetOptCmd(const char * cmd);

	const char * GetOptParam();
	bool         SetOptParam(const char * param);

	const __int64   GetClnLowSpace();
	bool            SetClnLowSpace(__int64 space);

	const __int64   GetClnHighSpace();
	bool            SetClnHighSpace(__int64 space);

	const char * GetUploadPath();
    bool         SetUploadPath(const char * path);

    const char * GetDownloadPath();
    bool         SetDownloadPath(const char * path);

	const char * GetUploadMark();
	bool         SetUploadMark(const char * mark);

	const char * GetUploadDirMark();
	bool         SetUploadDirMark(const char * mark);

	const int GetSchCheckInterval();
	bool      SetSchCheckInterval(const int interval);

	const double GetSchTransmitUtilization();
	bool         SetSchTransmitUtilization(const double util);

	const double GetSchUploadUtilization();
	bool         SetSchUploadUtilization(const double util);

	const int GetSchUploadSpeedIncrement();
	bool      SetSchUploadSpeedIncrement(const int inc);

  const int GetSoapListeningPort() const { return _soap_listening_port; }
  bool      SetSoapListeningPort(int port);

  std::string GetSoapCServer() const { return _soapc_server; }
  bool SetSoapCServer(const char *val);

  int GetSoapCRefreshStatusInterval() const { return _soapc_refresh_status_interval_in_sec; }
  bool SetSoapCRefreshStatusInterval(int val);

  bool GetSoapCAllowGlobalCtrl() const { return _soapc_allow_global_ctrl; }
  bool SetSoapCAllowGlobalCtrl(bool val);

  bool GetSoapCAllowSelfCtrl() const { return _soapc_all_self_ctrl; }
  bool SetSoapCAllowSelfCtrl(bool val);

private:

	// struct
	CTxConfig(void);
	~CTxConfig(void);

	// private member func
	bool loadConfig();
	bool saveConfig();

	// static var
	static CTxConfig * instance;

	// members var
	std::string s_strConfigFile;
	CIni	* _ini;

	std::string _rec_path;
  std::string _rec_file_name_format;
  std::string _rec_local_file_name_format;
  int         _rec_local_duration;
  __int64     _rec_max_file_size;
  __int64     _rec_max_file_duration;
  bool        _rec_auto_record;
  int         _rec_auto_record_delay; //sec
  int         _rec_ui_screen_count;
  int         _rec_ui_screen_type;
	std::string _opt_cmd;
	std::string _opt_param;

	__int64     _cln_low_space;
	__int64     _cln_high_space;

	std::string _upload_path;
	std::string _upload_mark;
    std::string _upload_dir_mark;

    std::string _download_path;

	int   _sch_check_interval;
	double _sch_transmit_utilization;
	double _sch_upload_utilization;
	int   _sch_upload_speed_increment;

  int _soap_listening_port;

  std::string _soapc_server;
  int _soapc_refresh_status_interval_in_sec;
  bool _soapc_allow_global_ctrl;
  bool _soapc_all_self_ctrl;
};
