#ifndef MONITOR_TAB_H
#define MONITOR_TAB_H

#include <QWidget>

#include "ui_monitor_tab.h"

#include "hardware_info_description.h"

class QTimer;
class QLabel;
class IMonitorController;

class MonitorTab : public QWidget {
  Q_OBJECT

public:
  MonitorTab(QWidget *parent = 0);
  ~MonitorTab();

  void Initiliaze(IMonitorController *controller);

Q_SIGNALS:
  void RequestHardwareInfoSignal();

public Q_SLOTS:
  void StartRequestHardwareInfo();
  void StopRequestHarewareInfo();

private Q_SLOTS:
  void saveMonitorConfigSlot();
  void HandleHardwareInfoNotifySlot(const HardwareInfo &info);
  void HandleNetworkInfoNotifySlot(double in_bandwidth, double out_bandwidth);

private:
  struct HDDLebals {
    HDDLebals(): used_space(nullptr), temperature(nullptr) {}

    QLabel *used_space;
    QLabel *temperature;
  };

private:
  Ui::MonitorTab ui_;
  QTimer *refresh_info_timer_;
  QList<HDDLebals> hdd_lebals;
};

#endif // MONITOR_TAB_H
