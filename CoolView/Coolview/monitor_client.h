#ifndef MONITOR_CLIENT_H
#define MONITOR_CLIENT_H

#include <QObject>

#include "hardware_info_description.h"

class QDomElement;

namespace monitor_soap {
class BasicHttpBinding_USCOREIMonitorServiceProxy;
}

class MonitorClient : public QObject {
  Q_OBJECT

public:
  MonitorClient(QObject *parent);
  ~MonitorClient();

Q_SIGNALS:
  void NotifyHardwareInfoSignal(const HardwareInfo &info);

public Q_SLOTS:
  void HandleRequestHardwareInfoSlot();
  void HandleRequestBeepForAWhileSlot(int ms);

private:
  class RegisterMetaTypeHelper {
  public:
    RegisterMetaTypeHelper() {
      qRegisterMetaType<HardwareInfo>("HardwareInfo");
    }
  };

  void ParseHardwareInfoFromXML(const QDomElement &doc, HardwareInfo &info);
  float FindMinValue(const QDomElement &element, const QString &tag_name);
  float FindSensorValue(const QDomElement &element, const QString &name);

private:
  monitor_soap::BasicHttpBinding_USCOREIMonitorServiceProxy *proxy_;
};

#endif // MONITOR_CLIENT_H
