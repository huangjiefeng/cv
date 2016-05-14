#include "system_controller.h"

#include <cassert>

#include "session_controller_interface.h"
#include "msg_receiver_interface.h"
#include "msg_sender_interface.h"
#include "config_manager.h"

namespace {

class ShutdownAction {
public:
  void operator()() const {
#ifdef _DEBUG
    system("C:\\Windows\\System32\\shutdown.exe -s -f -t 30");
#else
    system("C:\\Windows\\System32\\shutdown.exe -s -f -t 0");
#endif
  }
};

class RestartAction {
public:
  void operator()() const {
    system("C:\\Windows\\System32\\shutdown.exe -r -f -t 0");
  }
};

class NoAction {
public:
  void operator()() const {
    // empty
  }
};

} // namespace

SystemController::SystemController(QObject *parent)
    : ISystemController(parent) {
  action_ = NoAction();
  session_controller_ = nullptr;
}

SystemController::~SystemController() {

}

void SystemController::Initialize(IMsgManager *msg_manager, 
                                  ISessionController *session_controller) 
{
  assert(msg_manager != nullptr);
  assert(session_controller != nullptr);
  session_controller_ = session_controller;
  connect(this, SIGNAL(LogoutRequestSignal()),
          session_controller_, SLOT(HandleLogoutRequestSlot()));
  connect(
    session_controller_, 
    SIGNAL(NotifySessionStateChangedSignal(ISessionController::SessionState)),
    this, 
    SLOT(HandleSessionStateChangedNotify(ISessionController::SessionState)));

  IMsgSender *sender = msg_manager->GetMsgSender();
  connect(this, &SystemController::RequestGetTerminalConfigSignal,
    sender, &IMsgSender::SendGetTerminalConfigSlot);
  connect(this, &SystemController::ReportTerminalConfigSignal,
    sender, &IMsgSender::SendReportTerminalConfigSlot);

  IMsgReceiver *receiver = msg_manager->GetMsgReceiver();
  connect(receiver, &IMsgReceiver::NotifyRecvTerminalConfigSignal,
    this, &SystemController::HandleRecvTerminalConfigSlot);
}

void SystemController::HandleRestartSystemRequestSlot() {
  assert(session_controller_ != nullptr);
  action_ = RestartAction();
  if (session_controller_->GetCurrentState() != ISessionController::kIsOffline) {
    emit LogoutRequestSignal();
  } else {
    emit NotifyQuitSystemSignal();
    action_();
  }
}

void SystemController::HandleShutDownSystemRequestSlot() {
  assert(session_controller_ != nullptr);
  action_ = ShutdownAction();
  if (session_controller_->GetCurrentState() != ISessionController::kIsOffline) {
    emit LogoutRequestSignal();
  } else {
    emit NotifyQuitSystemSignal();
    action_();
  }
}

void SystemController::HandleQuitSystemRequestSlot() {
  assert(session_controller_ != nullptr);
  action_ = NoAction();
  if (session_controller_->GetCurrentState() != ISessionController::kIsOffline) {
    emit LogoutRequestSignal();
  } else {
    emit NotifyQuitSystemSignal();
    action_();
  }
}

void SystemController::HandleSessionStateChangedNotify(
    ISessionController::SessionState state ) {
  if (state == ISessionController::kIsOffline) {
    action_();
    emit NotifyQuitSystemSignal();
  } else if (state == ISessionController::kIsOnline) {
    // 在登陆后，向服务器报告终端参数并请求终端配置的参数
    ConfigDict config = ConfigManager::instance().Dump();
    emit ReportTerminalConfigSignal(config);
    emit RequestGetTerminalConfigSignal();
  }
}

void SystemController::ExecutionActionSlot() {
  action_();
}

void SystemController::HandleRecvTerminalConfigSlot( const ConfigDict &config )
{
  if (ConfigManager::instance().Update(config)) {
    ConfigManager::instance().SaveToConfigFiles();
    emit NotifyNewConfigSignal();
  }
}


