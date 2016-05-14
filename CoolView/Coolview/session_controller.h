#ifndef SESSION_CONTROLLER_H
#define SESSION_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QTime>

#include "session_controller_interface.h"

#include "conference_controller_interface.h"
#include "msg_manager_interface.h"

class RunningProfile;

class SessionController : public ISessionController {
  Q_OBJECT
 public:
  static const int kCheckNetworkInterval = 3000; // 3s
  static const int kSendRegisterMsgInterval = 600000; // 10m
  static const int kSendOnlineMsgInterval = 60000; // 60s
  static const int kNetworkInterfaceErrorTimeout = 30000; // 30s
  static const int kRetryLoginInterval = 3000; // 3s

 public:
  SessionController(QObject *parent, RunningProfile *profile);

  ~SessionController();

  void Initialize(
    IMsgManager *msg_manager,
    IConferenceController *conference_controller);

  SessionState GetCurrentState() const override { return state_; }

 Q_SIGNALS:
  void SendTerminalLoginMsgSignal(const TerminalLoginParam &info);
  void SendTerminalSAVAMsgSignal(const TerminalSAVAParam &info);
  void SendRegisterMsgSignal();
  void SendOnlineMsgSignal();
  void SendTerminalLogoutMsgSignal();

  void LeaveConferenceRequestSignal();

 public Q_SLOTS: 
  void HandleLoginRequestSlot() override;
  void HandleLogoutRequestSlot() override;

 private Q_SLOTS:
  void HandleSipAccountStateChangedSlot(const QString &username, int state);
  void HandleCheckNetworkRequestSlot();
  void HandleSendRegisterRequestSlot();
  void HandleSendOnlineRequestSlot();
  void HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri,
    IConferenceController::ConferenceState state);

 private:
  void ConnectWithMsgManager(IMsgManager *msg_manager);
  void ConnectWithConferenceController(
    IConferenceController *conference_controller);

  void set_state(SessionState state);
  void LogoutDirectly();

  void StopOnlineBeat();

  

 private:
  RunningProfile *profile_;

  SessionState state_;
  bool has_initialized_;
  bool is_logout_;
  bool is_in_conference_;
  bool is_retry_login_;

  // 检查网络的定时器
  QTimer *check_network_timer_;
  int check_network_interval_;
  int network_interface_timeout_;

  // 往SIP服务器发送Register信息的定时器，用于SIP服务器的保活
  QTimer *send_register_timer_;
  int send_register_msg_interval_;

  // 往会议服务器中的GlobalService中发送Online信息，用于登录后向会议服务器的保活
  QTimer *send_online_timer_;
  int send_online_msg_interval_;

  QTimer *retry_login_timer_;
  int retry_login_interval_;

  QTime interupt_time_;
  int interupt_elapse_time_;
  bool is_first_alarm_;
};

#endif // SESSION_CONTROLLER_H
