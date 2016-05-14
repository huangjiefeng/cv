#ifndef MONITOR_CONTROLLER_H
#define MONITOR_CONTROLLER_H

#include <QThread>

#include "monitor_controller_interface.h"

class QTimer;
class MonitorClient;
class ISystemController;

class MonitorController : public IMonitorController {
  Q_OBJECT

public:
  MonitorController(QObject *parent);
  ~MonitorController();

  void Initialize(ISystemController *system_controller);

Q_SIGNALS:
  void RequestBeepForAWhileSignal(int ms);
  void RequestRestartSignal();
  void RequestShutDownSignal();

public Q_SLOTS:
  void HandleRequestHardwareInfoSlot() override;

private Q_SLOTS:
  void HandleHardwareInfoNotifySlot(const HardwareInfo &info);
  void ResetCPUBeepSlot();
  void ResetMainboardBeepSlot();

private:
  enum TriggerType {
    kCPUTrigger = 0,
    kMainboardTriegger = 1,
    kTriggers = 2,
  };

  void TriggerAction(TriggerType type, int action);

private:
  HardwareInfo hardware_info[2];
  int current_info_;

  MonitorClient *monitor_client_;
  QThread client_thread_;
  QTimer *refresh_info_timer_;
  
  bool has_initialized_;
  bool can_beep_[kTriggers]; // one for cpu, one for mainboard
};

#endif // MONITOR_CONTROLLER_H
