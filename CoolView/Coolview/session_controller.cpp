#include "session_controller.h"

#include <algorithm>
#include <cassert>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <QApplication>
#include <QDesktopWidget>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>

#include <DeviceManager/DeviceManager.h>
#include <log/Log.h>

#include "util/NetworkUtil.h"
#include "util/GetFileVersion.h"
#include "profile/RunningProfile.h"
#include "config/RunningConfig.h"

#include "CoolviewCommon.h"

namespace {

QString GetVersionByName(const TCHAR *name) {
  char buffer[20];
  memset(buffer, 0, sizeof(buffer));
  // 使用CGetFileVersion类读取rtpware.dll的版本信息
  CGetFileVersion version_reader;
  version_reader.Open(name);	// 打开文件，读取版本信息
  if (!version_reader.IsValid()) {
    // 读取错误
    printf("error to read the version of %s \n", name);
  } else {
    // 输出版本号信息
    sprintf(buffer, "%d.%d.%d.%d", 
      version_reader.GetFileVersionMajor(),
      version_reader.GetFileVersionMinor(), 
      version_reader.GetFileVersionBuild(), 
      version_reader.GetFileVersionQFE());
  }
  return QString::fromStdString(std::string(buffer));
}

}  // namespace

SessionController::SessionController(QObject *parent, 
                                     RunningProfile *profile)
    : ISessionController(parent), 
      profile_(profile) {

  has_initialized_ = false;
  state_ = kIsOffline;
  is_in_conference_ = false;

  WSADATA wsa_data; 
  int rc = WSAStartup(MAKEWORD(2,2), &wsa_data);

  check_network_timer_ = nullptr;
  check_network_interval_ = kCheckNetworkInterval;
  network_interface_timeout_ = kNetworkInterfaceErrorTimeout;

  send_register_timer_ = nullptr;
  send_register_msg_interval_ = kSendRegisterMsgInterval;

  send_online_timer_ = nullptr;
  send_online_msg_interval_ = kSendOnlineMsgInterval;

  retry_login_timer_ = nullptr;
  retry_login_interval_ = kRetryLoginInterval;
  is_retry_login_ = false;
  is_logout_ = false;
}

SessionController::~SessionController() {
  WSACleanup();
}

void SessionController::Initialize(
    IMsgManager *msg_manager,
    IConferenceController *conference_controller ) {
  ConnectWithMsgManager(msg_manager);
  ConnectWithConferenceController(conference_controller);

  check_network_timer_ = new QTimer(this);
  connect(check_network_timer_, SIGNAL(timeout()),
    this, SLOT(HandleCheckNetworkRequestSlot()));

  send_register_timer_ = new QTimer(this);
  connect(send_register_timer_, SIGNAL(timeout()),
    this, SLOT(HandleSendRegisterRequestSlot()));

  send_online_timer_ = new QTimer(this);
  connect(send_online_timer_, SIGNAL(timeout()),
          this, SLOT(HandleSendOnlineRequestSlot()));

  retry_login_timer_ = new QTimer(this);
  connect(retry_login_timer_, SIGNAL(timeout()),
          this, SLOT(HandleLoginRequestSlot()));

  has_initialized_ = true;
}

void SessionController::ConnectWithMsgManager( IMsgManager *msg_manager ) {
  assert(msg_manager);
  
  IMsgSender *sender = msg_manager->GetMsgSender();
  assert(sender);
  connect(
    this, SIGNAL(SendTerminalLoginMsgSignal(const TerminalLoginParam&)),
    sender, SLOT(SendTerminalLoginInfoSlot(const TerminalLoginParam&)));

  connect(
    this, SIGNAL(SendTerminalSAVAMsgSignal(const TerminalSAVAParam&)),
    sender, SLOT(SendTerminalInfoForSAVASlot(const TerminalSAVAParam&)));

  connect(this, SIGNAL(SendRegisterMsgSignal()),
          sender, SLOT(SendRegisterHeartBeatToSipServerSlot()));

  connect(this, SIGNAL(SendOnlineMsgSignal()),
          sender, SLOT(SendOnlineHeartBeatToKeepLoginSlot()));

  connect(this, SIGNAL(SendTerminalLogoutMsgSignal()),
          sender, SLOT(SendTerminalLogoutInfoSlot()));

  IMsgReceiver *receiver = msg_manager->GetMsgReceiver();
  assert(receiver);
  connect(receiver, SIGNAL(NotifyRecvSAVAResultSignal(bool)),
          this, SIGNAL(NotifyRecvSaviSignal(bool)));

  connect(
    receiver, SIGNAL(NotifySipAccountStateChangedSignal(const QString&, int)),
    this, SLOT(HandleSipAccountStateChangedSlot(const QString&, int)));
}

void SessionController::ConnectWithConferenceController(
    IConferenceController *conference_controller) {
  assert(conference_controller);

  connect(this, SIGNAL(LeaveConferenceRequestSignal()),
          conference_controller, SLOT(HandleLeaveConferenceRequestSlot()));

  connect(
    conference_controller, SIGNAL(NotifyConferenceStateChangedSignal(
      const QString&, IConferenceController::ConferenceState)),
    this, SLOT(HandleConferenceStateChangedNotifySlot(
      const QString&, IConferenceController::ConferenceState)));
}

void SessionController::HandleLoginRequestSlot() {
  assert(has_initialized_);

  if (state_ == kIsOnline) {
    emit NotifyLoginErrorSignal(kErrorOfAlreadyLogin);
    return;
  }

  QString username = QString::fromStdString(profile_->username());
  QString password = QString::fromStdString(profile_->password());
  QString realm = QString::fromStdString(profile_->realm());
  QString ip_addr = QString::fromStdString(profile_->ip_address());
  QString register_server = realm;
  QString proxy_server = register_server;
  QString display_name = QString("%1@%2").arg(username).arg(register_server);
  QString mac = QString::fromStdString(profile_->mac_address());

  TerminalLoginParam info;
  info.display_name = display_name;
  info.username = username;
  info.identity = username;
  info.password = password;
  info.realm = realm;
  info.proxy_server = proxy_server;
  info.register_server = register_server;
  info.ip_address = ip_addr;
  info.mac = mac;
  info.global_service = QString("sip:globalService@") + realm;
  RunningConfig::Instance()->GlobalService(info.global_service);

  emit SendTerminalLoginMsgSignal(info);

  check_network_timer_->start( check_network_interval_ );
  is_first_alarm_ = true;
  interupt_elapse_time_ = 0;
}

void SessionController::HandleSipAccountStateChangedSlot(
  const QString &username, int state) {
  QString local_username = QString::fromStdString(profile_->username());
  if (username == local_username) {
    switch (state) {
      // 登陆错误
      case SipAccountStateUnknown:
        set_state(kIsOffline);
        emit NotifyLoginErrorSignal(kErrorOfUnknown); 
        break;
      case SipAccountStateTimeout:
        if (state_ == kIsLogining && !is_retry_login_) {
          // 非重新尝试连接时，登陆超时，发出信号
          set_state(kIsOffline);
          emit NotifyLoginErrorSignal(kErrorOfTimeOut);

        } else if (state_ == kIsOnline) { 
          // TODO:登陆后，与sip服务器连接超时了
          // 重新尝试连接
          StopOnlineBeat();
          state_ = kIsOffline;
          is_retry_login_ = true;
          retry_login_timer_->start(retry_login_interval_);
        }
        break;
      case SipAccountStateAuthenticationError:
        set_state(kIsOffline);
        emit NotifyLoginErrorSignal(kErrorOfAuthenticationFailure);
        break;
      case SipAccountStateServerError:
        set_state(kIsOffline);
        emit NotifyLoginErrorSignal(kErrorOfConnectToServerFailure); 
        break;
      case SipAccountStateNotPermitted:
        set_state(kIsOffline);
        emit NotifyLoginErrorSignal(kErrorOfPermissionDenied);
        break;
      // 登陆状态
      case SipAccountStateProgress:
        // 由于发送register消息后，会返回正在处理状态
        // 因此若已经登陆了，则不修改当前session状态
        if (state_ != ISessionController::kIsOnline) {
          set_state(kIsLogining);
        }
        break;
      case SipAccountStateClosed:
        set_state(kIsOffline);
        break;
      case SipAccountStateOk: {
        is_retry_login_ = false;
        retry_login_timer_->stop();

        TerminalSAVAParam info;
        GetHostIPs(info.ipv4_addr, info.ipv6_addr);
        info.terminal_version = GetVersionByName(L"coolview.exe");
        info.screen_count = QApplication::desktop()->numScreens();
        emit SendTerminalSAVAMsgSignal(info);

        send_register_timer_->start(send_register_msg_interval_);
        send_online_timer_->start(send_online_msg_interval_);
       
        set_state(kIsOnline);
        emit NotifyLoginErrorSignal(kOk);
        break;
      }
      default:
        assert(false);
    }
  }
}

void SessionController::set_state( SessionState state ) {
  if (state_ != state) {
    state_ = state;
    emit NotifySessionStateChangedSignal(state);
  }
}

void SessionController::HandleCheckNetworkRequestSlot() {
  typedef vector<string> IPList;
  static bool first_alarm = true;

  string ip = profile_->ip_address();
  if (IsIpv4Address(ip)) {
    IPList ips;
    GetIPList(AF_INET, &ips);
    IPList::iterator it = std::find(ips.begin(), ips.end(), ip);
    if (it == ips.end()) {
      // TODO: use another error code
      emit NotifyLoginErrorSignal(kErrorOfNetworkInterfaceNonoperational);
      // TODO: config whether to logout
      // HandleLogoutRequestSlot();
    }
  }

  string mac = profile_->mac_address();
  if (state_ == kIsOnline && !mac.empty()) {
    InterfaceStatus status;
    if (DeviceManager::GetInstatnce()->GetNetworkAdaptorState(mac, status) != NO_ERROR) {
      // 无法访问网络适配器状态
      HandleLogoutRequestSlot();
      emit NotifyLoginErrorSignal(kErrorOfAccessNetworkAdaptorFailure);
    } else {
      static QTime last_interupt_time;
      static int elapse_time = 0;
      if (status == IF_STATUS_OPERATIONAL) {
        is_first_alarm_ = true;
        interupt_elapse_time_ = 0;
      } else {
        if (is_first_alarm_) {
          is_first_alarm_ = false;
          interupt_time_.restart();
          // 网络连接出现问题
          HandleLogoutRequestSlot();
          emit NotifyLoginErrorSignal(kErrorOfNetworkInterfaceNonoperational);

        } else {
          interupt_elapse_time_ = interupt_time_.elapsed();
          if (interupt_elapse_time_ > network_interface_timeout_) {
            interupt_elapse_time_ = 0;
            // TODO: config whether to logout
            // HandleLogoutRequestSlot();
          }
        }
      }
    }
  }
}

void SessionController::HandleLogoutRequestSlot() {
  if (state_ == kIsOffline || is_logout_) {
    return;
  }

  if (is_in_conference_) {
    is_logout_ = true;
    emit LeaveConferenceRequestSignal();
  } else {
    LogoutDirectly();
  }
}

void SessionController::HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri,
    IConferenceController::ConferenceState state ) {
  if (state == IConferenceController::kIsInConference ||
      state == IConferenceController::kIsLeavingConference) {
    is_in_conference_ = true;
  }
  if (state == IConferenceController::kIsNotInConference) {
    is_in_conference_ = false;
  }
  if (is_logout_ && !is_in_conference_) {
    LogoutDirectly();
  }
}

void SessionController::LogoutDirectly() {
  StopOnlineBeat();

  emit SendTerminalLogoutMsgSignal();
  set_state(kIsOffline);
  is_logout_ = false;
}

void SessionController::StopOnlineBeat() {
  check_network_timer_->stop();
  send_register_timer_->stop();
  send_online_timer_->stop();
}

void SessionController::HandleSendRegisterRequestSlot() {
  emit SendRegisterMsgSignal();
}

void SessionController::HandleSendOnlineRequestSlot() {
  emit SendOnlineMsgSignal();
}



