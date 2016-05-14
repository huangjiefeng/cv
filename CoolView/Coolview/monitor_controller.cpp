#include "monitor_controller.h"

#include <cassert>
#include <QTimer>

#include <dbus/core/service/CvCoreService.h>
#include "monitor_client.h"

#include "config/RunningConfig.h"
#include "system_controller_interface.h"

MonitorController::MonitorController(QObject *parent)
    : IMonitorController(parent),
      monitor_client_(nullptr),
      refresh_info_timer_(nullptr),
      current_info_(0),
      has_initialized_(false){
  hardware_info[0].mainboard_temperature = std::numeric_limits<float>::quiet_NaN();
  hardware_info[0].cpu_load = std::numeric_limits<float>::quiet_NaN();
  hardware_info[0].cpu_temperature = std::numeric_limits<float>::quiet_NaN();
  hardware_info[0].ram_available_mem = std::numeric_limits<float>::quiet_NaN();

  for (int i = 0; i < kTriggers; ++i) {
    can_beep_[i] = true;
  }
}

MonitorController::~MonitorController() {
  if (client_thread_.isRunning()) {
    client_thread_.terminate();
  }
  client_thread_.wait();

  // TODO: 可能需要move to main thread后再删除
  if (monitor_client_) {
    monitor_client_->deleteLater();
  }
}

void MonitorController::Initialize(ISystemController *system_controller) {
  assert(system_controller);
  assert(!has_initialized_);

  connect(
    this, &MonitorController::RequestShutDownSignal,
    system_controller, &ISystemController::HandleShutDownSystemRequestSlot);

  connect(
    this, &MonitorController::RequestRestartSignal,
    system_controller, &ISystemController::HandleRestartSystemRequestSlot);

  monitor_client_ = new(std::nothrow) MonitorClient(nullptr);
  if (!monitor_client_) {
    return;
  }
  monitor_client_->moveToThread(&client_thread_);
  client_thread_.start();

  refresh_info_timer_ = new(std::nothrow) QTimer(this);
  if (refresh_info_timer_) {
    connect(
      refresh_info_timer_, &QTimer::timeout,
      monitor_client_, &MonitorClient::HandleRequestHardwareInfoSlot);

    connect(monitor_client_, &MonitorClient::NotifyHardwareInfoSignal,
      this, &MonitorController::HandleHardwareInfoNotifySlot);

    connect(this, &MonitorController::RequestBeepForAWhileSignal,
      monitor_client_, &MonitorClient::HandleRequestBeepForAWhileSlot);

    refresh_info_timer_->start(1000);
  }

  connect(CoreService2::getInstance(), &CoreService2::NetworkBandwidthSignal,
    this, &MonitorController::NotifyNetworkInfoSignal);

  has_initialized_ = true;
}

void MonitorController::HandleRequestHardwareInfoSlot() {
  emit NotifyHardwareInfoSignal(hardware_info[current_info_]);
}

void MonitorController::HandleHardwareInfoNotifySlot(const HardwareInfo &info) {
  hardware_info[1-current_info_] = info;
  current_info_ = 1 - current_info_;

  RunningConfig *config = RunningConfig::Instance();
  if (info.mainboard_temperature > config->GetMainboardTempThreshold()) {
    TriggerAction(kMainboardTriegger, config->GetMainboardTempAction());
  }

  if (info.cpu_temperature > config->GetCpuTempThreshold()) {
    TriggerAction(kCPUTrigger, config->GetCpuTempAction());
  }
}

void MonitorController::TriggerAction(TriggerType type, int action ) {
  switch (action) {
    case 1: // beep
      if (can_beep_[type]) {
        can_beep_[type] = false;
        // TODO: 配置文件获取蜂鸣的时间
        emit RequestBeepForAWhileSignal(500);
      }
      break;
    case 2: // shutdown
      can_beep_[type] = true;
      emit RequestShutDownSignal();
      break;
    case 3: // restart
      can_beep_[type] = true;
      emit RequestRestartSignal();
      break;
    default:
      can_beep_[type] = true;
      break;
  }
}

void MonitorController::ResetCPUBeepSlot() {
  can_beep_[kCPUTrigger] = true;
}

void MonitorController::ResetMainboardBeepSlot() {
  can_beep_[kMainboardTriegger] = true;
}


