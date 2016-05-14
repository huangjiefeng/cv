#pragma  once
#include <string>

class RunningProfile {
public:
  static RunningProfile* getInstance() {
    static RunningProfile runningProfile;
    return &runningProfile;
  }

  void set_username(const std::string &UserName) {
    username_ = UserName;
  }

  const std::string& username() {
    return username_;
  }

  void set_password(const std::string &Password) {
    password_ = Password;
  }

  const std::string& password() const {
    return password_;
  }

  void set_ip_address(const std::string &IpAddress) {
    ip_address_ = IpAddress;
  }

  const std::string& ip_address() const {
    return ip_address_;
  }

  void set_realm(const std::string &Realm) {
    realm_ = Realm;
  }

  const std::string& realm() const {
    return realm_;
  }

  void set_http_port(const int port) {
	  http_port_ = port;
  }

  int http_port() const {
	  return http_port_;
  }

  void set_current_conference_uri(const std::string &CurrentConfURI) {
    current_conference_uri_ = CurrentConfURI;
  }

  const std::string& current_conference_uri() const {
    return current_conference_uri_;
  }

  void set_user_uri(const std::string &UserUri) {
    user_uri_ = UserUri;
  }

  std::string user_uri() const {
    return user_uri_;
  }

  bool remember_profile() const { 
    return remember_profile_; 
  }

  void set_remember_profile(bool remember) { 
    remember_profile_ = remember; 
  }

  bool auto_login() const { return auto_login_; }

  void set_auto_login(bool val) { auto_login_ = val; }

  bool ukey_certify( ) { return ukey_certify_; }

  void set_ukey_certify(bool certify) { ukey_certify_ = certify; }

  const std::string& mac_address() const { return mac_address_; }
  void set_mac_address(const std::string& val) { mac_address_ = val; }

  void SaveFile();
  void LoadFile();

private:
  RunningProfile();
  ~RunningProfile() {}

private:
  std::string profile_file_path_;
  std::string username_;
  std::string password_;
  std::string realm_;
  std::string current_conference_uri_;
  std::string ip_address_;
  std::string mac_address_;
  std::string user_uri_;

  int http_port_;

  bool auto_login_;
  bool ukey_certify_;
  bool remember_profile_;
};


