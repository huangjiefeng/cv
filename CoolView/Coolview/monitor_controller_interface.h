#ifndef MONITOR_CONTROLLER_INTERFACE_H
#define MONITOR_CONTROLLER_INTERFACE_H

#include <QObject>

#include "hardware_info_description.h"

class IMonitorController : public QObject {
  Q_OBJECT

public:
  IMonitorController(QObject *parent) {}
  virtual ~IMonitorController() {}

Q_SIGNALS:
  void NotifyHardwareInfoSignal(const HardwareInfo &info);
  void NotifyNetworkInfoSignal(double in_bandwidth, double out_bandwidth);

public Q_SLOTS:
  virtual void HandleRequestHardwareInfoSlot() = 0;
};

#endif // MONITOR_CONTROLLER_INTERFACE_H
