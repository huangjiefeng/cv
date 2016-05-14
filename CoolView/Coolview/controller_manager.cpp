#include "controller_manager.h"

#include <cassert>

#include <DeviceManager/DeviceManager.h>

#include "util/MediaManager.h"

#include "profile/RunningProfile.h"
#include "config/RunningConfig.h"
#include "util/ini/CVIniConfig.h"

#include "session_controller.h"
#include "conference_controller.h"
#include "display_controller.h"
#include "tx_display_controller.h"
#include "tx_controller.h"
#include "tx_client_controller.h"
#include "fileplay_controller.h"
#include "system_controller.h"
#include "monitor_controller.h"
#include "daemon_controller.h"
#include "floor_controller.h"

#include "msg_manager_interface.h"

#include "offline_session_controller.h"

ControllerManager::ControllerManager(QObject *parent)
    : IControllerManager(parent) {
  profile_ = nullptr;
  config_ = nullptr;
  device_manager_ = nullptr;
  media_manager_ = nullptr;

  session_controller_ = nullptr;
  conference_controller_ = nullptr;
  display_controller_ = nullptr;
  tx_controller_ = nullptr;
  tx_client_controller_ = nullptr;
  fileplay_controller_ = nullptr;
  system_controller_ = nullptr;
  monitor_controller_ = nullptr;
  daemon_controller_ = nullptr;
}

ControllerManager::~ControllerManager() {
}

void ControllerManager::Initialize(RunningProfile *profile, 
                                   RunningConfig *config, 
                                   DeviceManager *device_manager,
                                   IMsgManager *msg_manager,
                                   MediaManager *media_manager,
                                   IDaemonController *daemon_controller,
                                   HttpMsgManager *http_msg_manager) {
  assert(profile != nullptr);
  assert(config != nullptr);
  assert(device_manager != nullptr);
  assert(media_manager != nullptr);
  assert(msg_manager != nullptr);
  assert(http_msg_manager != nullptr);
  assert(daemon_controller != nullptr);

  profile_ = profile;
  config_ = config;
  device_manager_ = device_manager;
  msg_manager_ = msg_manager;
  media_manager_ = media_manager;
  daemon_controller_ = daemon_controller;
  http_msg_manager_ = http_msg_manager;

  InitSenssionAndConferenceController();
  InitDisplayController();
  InitTXController();
  InitFilePlayController();
  InitSystemController();
  InitMonitorController();

  media_manager->setConferenceController(conference_controller_);
}

void ControllerManager::InitSenssionAndConferenceController() {
  FloorController *controller = new FloorController(this);
  floor_controller_ = controller;

  ISessionController::RegisterMetaType();
  SessionController *session_controller = new SessionController(this, profile_);
  //OfflineSessionController *session_controller = new OfflineSessionController(this); //TODO:debug only!
  
  IConferenceController::RegisterMetaType();
  ConferenceController *conference_controller = 
    new ConferenceController(this, profile_, config_, media_manager_, this);

  session_controller->Initialize(msg_manager_, conference_controller);
  conference_controller->Initialize(
    msg_manager_, 
    http_msg_manager_, 
    session_controller, 
    floor_controller_);

  conference_controller_ = conference_controller;
  session_controller_ = session_controller;
}

void ControllerManager::InitDisplayController() {
  assert(display_controller_ == nullptr);
  
  IDisplayController::RegisterMetaType();

  if (CVIniConfig::getInstance().IsModelTX()) {
    TXDisplayController *controller = 
      new TXDisplayController(this, media_manager_);
    controller->Initialize(conference_controller_);

    display_controller_ = controller;
  } else {
    DisplayController *controller = 
      new DisplayController(this, media_manager_);
    controller->Initialize(conference_controller_);

    display_controller_ = controller;
  }
}

void ControllerManager::InitTXController() {
  if (CVIniConfig::getInstance().IsModelTX()) {
    assert(tx_controller_ == nullptr);
    ITXController::RegisterMetaType();
    TXController *tx_controller = new TXController(this, profile_, config_);
    tx_controller->Initialize(session_controller_, msg_manager_,
      conference_controller_, display_controller_, media_manager_);
    tx_controller_ = tx_controller;
  } else {
    assert(tx_client_controller_ == nullptr);
    //不要制定parent为this，TXClientController远程调用SOAP会阻塞，需要在单独线程运行
    TXClientController *tx_client_controller = new TXClientController(nullptr, profile_, config_);
    tx_client_controller->Initialize(msg_manager_, conference_controller_, media_manager_);
    tx_client_controller_ = tx_client_controller;
  }
}

void ControllerManager::InitFilePlayController()
{
  assert(fileplay_controller_ == nullptr);
  IFilePlayController::RegisterMetaType();
  FilePlayController *controller = new FilePlayController(this);
  controller->Initialize(conference_controller_, media_manager_, msg_manager_);
  fileplay_controller_ = controller;
}

void ControllerManager::InitSystemController() {
  assert(system_controller_ == nullptr);
  assert(session_controller_ != nullptr);

  SystemController *controller = new SystemController(this);
  controller->Initialize(msg_manager_, session_controller_);

  system_controller_ = controller;
}

void ControllerManager::InitMonitorController() {
  assert(monitor_controller_ == nullptr);
  assert(system_controller_);

  MonitorController *controller = new MonitorController(this);
  controller->Initialize(system_controller_);

  monitor_controller_ = controller;
}