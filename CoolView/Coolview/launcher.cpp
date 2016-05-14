#include "launcher.h"

#include <QApplication>
#include <DeviceManager/DeviceManager.h>

#include <dbus/core/service/CvCoreService.h>

#include "util/ini/CVIniConfig.h"
#include "config/RunningConfig.h"
#include "profile/RunningProfile.h"

#include "util/MediaManager.h"
#include "controller_manager.h"
#include "ui_manager.h"
#include "sip_msg_manager.h"
#include "http_msg_manager.h"
#include "vconf_sip_msg_manager.h"

#include "conference_controller_interface.h"
#include "display_controller_interface.h"
#include "daemon_controller.h"

#include "ptz_controller.h"

Launcher::Launcher(QObject *parent)
    : QObject(parent) {
  controller_manager_ = nullptr;
  ui_manager_ = nullptr;
  msg_manager_ = nullptr;
  use_multi_thread_ = CVIniConfig::getInstance().useMultiThread();
}

Launcher::~Launcher() {
  if (controller_manager_) {
    controller_manager_->deleteLater();
  }
  if (msg_manager_) {
    msg_manager_->deleteLater();
  }
  if (ui_manager_) {
    ui_manager_->deleteLater();
  }
}

void Launcher::Initialize() {
  // 需要先于SipMsgManager创建
  DaemonController *daemon_controller = new DaemonController(nullptr);
  daemon_controller->Initialize();
  daemon_controller->StartDaemon();

  if (CVIniConfig::getInstance().isVirtualConferenceEnable()) {
    VConfSipMsgManager *msg_manager = new VConfSipMsgManager(nullptr);
    msg_manager->Initialize();
    msg_manager_ = msg_manager;
  } else {
    SipMsgManager *msg_manager = new SipMsgManager(nullptr);
    msg_manager->Initialize();
    msg_manager_ = msg_manager;
  }

  http_msg_manager_ = new HttpMsgManager(nullptr);

  // 目前，media manager必须和controller manager在同一个线程中。
  MediaManager *media_manager = MediaManager::getInstance();

  controller_manager_ = new ControllerManager(nullptr);
  controller_manager_->Initialize(
    RunningProfile::getInstance(),
    RunningConfig::Instance(),
    DeviceManager::GetInstatnce(),
    msg_manager_,
    media_manager,
    daemon_controller,
    http_msg_manager_);

  media_manager->setConferenceController(
    controller_manager_->GetConferenceController());

  IConferenceController *conference_controller = 
    controller_manager_->GetConferenceController();
  connect(
    this, 
    &Launcher::StartSendStreamMediaSignal,
    conference_controller, 
    &IConferenceController::HandleCreateSendStreamMediaRequestSlot);

  IDisplayController *display_controller = 
    controller_manager_->GetDisplayController();
  connect(
    this, 
    &Launcher::ReceiveAllLocalMediaSignal,
    display_controller, 
    &IDisplayController::HandleReceiveAllLocalVideoNotifySlot);

  core_service_ = CoreService2::getInstance();
  core_service_->Initialize(controller_manager_);
  // TODO: connect core_service with ptz_controller
  

  ui_manager_ = new UIManager(nullptr);
  ui_manager_->Initialize(controller_manager_);

  if (use_multi_thread_) {
    msg_manager_->moveToThread(&msg_thread_);
    http_msg_manager_->moveToThread(&msg_thread_);
    media_manager->moveToThread(&controller_thread_);
    controller_manager_->moveToThread(&controller_thread_);
    core_service_->moveToThread(&core_service_thread_);
  }
}

void Launcher::Start() {
  if (use_multi_thread_) {
    controller_thread_.start();
    msg_thread_.start();
    core_service_thread_.start();
  }

  emit StartSendStreamMediaSignal();
  emit ReceiveAllLocalMediaSignal();

  ui_manager_->Show();
}

void Launcher::Stop() {
  if (use_multi_thread_) {
    core_service_thread_.terminate();
    msg_thread_.terminate();
    controller_thread_.terminate();

    core_service_thread_.wait();
    msg_thread_.wait();
    controller_thread_.wait();
  }
	if (controller_manager_) {
	  controller_manager_->GetDaemonController()->StopDaemon();
	}
}


