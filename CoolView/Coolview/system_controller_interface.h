#ifndef SYSTEM_CONTROLLER_INTERFACE_H
#define SYSTEM_CONTROLLER_INTERFACE_H

#include <QObject>

class ISystemController : public QObject {
  Q_OBJECT
public:
  ISystemController(QObject *parent) : QObject(parent) {} 
  virtual ~ISystemController() {}

Q_SIGNALS:
  void NotifyQuitSystemSignal();
  void NotifyNewConfigSignal();

public Q_SLOTS:
  virtual void HandleRestartSystemRequestSlot() = 0;
  virtual void HandleShutDownSystemRequestSlot() = 0;
  virtual void HandleQuitSystemRequestSlot() = 0;
};


#endif // SYSTEM_CONTROLLER_INTERFACE_H