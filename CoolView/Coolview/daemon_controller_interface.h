#ifndef DAEMON_CONTROLLER_INTERFACE_H
#define DAEMON_CONTROLLER_INTERFACE_H

#include <QObject>

class IDaemonController : public QObject {
  Q_OBJECT
public:
  IDaemonController(QObject *parent) : QObject(parent) {} 
  virtual ~IDaemonController() {}

  virtual void StartDaemon() = 0;
  virtual void StopDaemon() = 0;

public slots:
  virtual void HandleShowConsoleSlot() = 0;

};

#endif // DAEMON_CONTROLLER_INTERFACE_H