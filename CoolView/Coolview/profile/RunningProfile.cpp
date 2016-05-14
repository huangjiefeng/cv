#include "RunningProfile.h"
#include "util\ini\Ini.h"

#include  <QtCore\QtCore>


//取消 unicode
#ifdef _UNICODE
#undef    _UNICODE
#endif
#ifdef UNICODE
#undef  UNICODE
#endif

#define SECTION_NAME_LOGIN   "Login"
#define SECTION_NAME_CONF    "Conf"

#define KEY_LOGIN_USERNAME   "Username"
#define KEY_LOGIN_PASSWORD   "Password"
#define KEY_LOGIN_REALM      "Realm"
#define KEY_LOGIN_AUTOLOGIN  "Autologin"
#define KEY_LOGIN_REMEMBER   "Remember"
#define KEY_LOGIN_HTTP_PORT  "HttpPort"

#define KEY_CONF_CURRENTCONF "CurrentConf"


RunningProfile::RunningProfile() {
  QString app_dir = QCoreApplication::applicationDirPath();
  app_dir.replace("/" , "\\" );
  QString profile_file_path = app_dir + "\\cfg\\profile.ini";

  ukey_certify_ = false;
  mac_address_ = "";

  profile_file_path_ = profile_file_path.toLocal8Bit();
  set_remember_profile(false);
  LoadFile();
}

void RunningProfile::LoadFile() {
  // using ini
  CIni ini;
  ini.SetPathName(profile_file_path_.c_str());

  const int BUF_SIZE = 256;
  char buf[BUF_SIZE] = {0};

  ini.GetString(SECTION_NAME_LOGIN, KEY_LOGIN_USERNAME, buf, BUF_SIZE, "");
  username_ = buf;
  ini.GetString(SECTION_NAME_LOGIN, KEY_LOGIN_PASSWORD, buf, BUF_SIZE, "");
  password_ = buf;
  ini.GetString(SECTION_NAME_LOGIN, KEY_LOGIN_REALM,    buf, BUF_SIZE, "");
  realm_ = buf;
  http_port_ = ini.GetInt(SECTION_NAME_LOGIN, KEY_LOGIN_HTTP_PORT, 8282);
  auto_login_ = ini.GetBool(SECTION_NAME_LOGIN, KEY_LOGIN_AUTOLOGIN, false);
  remember_profile_ = ini.GetBool(SECTION_NAME_LOGIN, KEY_LOGIN_REMEMBER, false);

  ini.GetString(SECTION_NAME_CONF, KEY_CONF_CURRENTCONF, buf, BUF_SIZE, "");
  current_conference_uri_ = buf;

  // old method
  /*QFile profileFile( QString::fromLocal8Bit( _profileFilePath.c_str()) );
  QDataStream in( &profileFile );
  if( profileFile.open( QIODevice::ReadOnly ) )
  {
	  QString currentConfURI , ipAddress , password , realm, username, qosServerUri ;

	  in >> currentConfURI;
	  in >> ipAddress;
	  in >> password;
	  in >> realm;
	  in >> username;
	  in >> this->_rememberProfile;
	  in >> this->_autoLogin;

	  this->_currentConfURI = currentConfURI.toStdString();
	  this->_ipAddress = ipAddress.toStdString();
	  this->_password = password.toStdString();
	  this->_realm = realm.toStdString();
	  this->_username = username.toStdString();
  }*/
}

void RunningProfile::SaveFile() {
  //如果使用UKEY认证登录的话这些信息不应该保存
  if( !ukey_certify_ ) {
	  // using ini
	  CIni ini;
	  ini.SetPathName(profile_file_path_.c_str());

	  ini.WriteString(SECTION_NAME_LOGIN, KEY_LOGIN_USERNAME,  username_.c_str());
	  ini.WriteString(SECTION_NAME_LOGIN, KEY_LOGIN_PASSWORD,  password_.c_str());
	  ini.WriteString(SECTION_NAME_LOGIN, KEY_LOGIN_REALM   ,  realm_.c_str());
	  ini.WriteInt   (SECTION_NAME_LOGIN, KEY_LOGIN_HTTP_PORT, http_port_);
	  ini.WriteBool  (SECTION_NAME_LOGIN, KEY_LOGIN_AUTOLOGIN, auto_login_);
	  ini.WriteBool  (SECTION_NAME_LOGIN, KEY_LOGIN_REMEMBER,  remember_profile_);

	  ini.WriteString(SECTION_NAME_CONF, KEY_CONF_CURRENTCONF, current_conference_uri_.c_str());

	  // old method
	  /*QFile profileFile(QString::fromLocal8Bit( _profileFilePath.c_str()) );
	  QDataStream out(&profileFile);
	  if( profileFile.open( QIODevice::WriteOnly ) )
	  {
		  out << QString::fromStdString( this->_currentConfURI);
		  out << QString::fromStdString(this->_ipAddress);
		  out << QString::fromStdString( this->_password );
		  out << QString::fromStdString( this->_realm );
		  out << QString::fromStdString( this->_username );
		  out << this->_rememberProfile;
		  out << this->_autoLogin;
	  }*/
  }
}