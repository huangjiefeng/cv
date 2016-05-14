#ifndef _CONTROLLER_MANAGER_INTERFACE_H_
#define _CONTROLLER_MANAGER_INTERFACE_H_

#include <QObject>

class ISessionController;
class IConferenceController;
class ILayoutController;
class IDisplayController;
class ISystemController;
class ITXController;
class ITXClientController;
class IFilePlayController;
class IMonitorController;
class IDaemonController;
class IFloorController;

class IControllerManager : public QObject {
  Q_OBJECT
public:
  IControllerManager(QObject *parent) : QObject(parent) {}
  virtual ~IControllerManager() {}

  virtual ISessionController* GetSessionController() = 0;
  virtual IConferenceController* GetConferenceController() = 0;
  virtual IDisplayController* GetDisplayController() = 0;
  virtual ITXController* GetTXController() = 0;
  virtual ITXClientController* GetTXClientController() = 0;
  virtual IFilePlayController* GetFilePlayController() = 0;
  virtual ISystemController *GetSystemController() = 0;
  virtual IMonitorController *GetMonitorController() = 0;
  virtual IDaemonController *GetDaemonController() = 0;
  virtual IFloorController* GetFloorController() = 0;

  virtual const ISessionController* GetSessionController() const = 0;
  virtual const IConferenceController* GetConferenceController() const = 0;
  virtual const IDisplayController* GetDisplayController() const = 0;
  virtual const ITXController* GetTXController() const = 0;
  virtual const ITXClientController* GetTXClientController() const = 0;
  virtual const IFilePlayController* GetFilePlayController() const = 0;
  virtual const ISystemController *GetSystemController() const = 0;
  virtual const IMonitorController *GetMonitorController() const = 0;
  virtual const IDaemonController *GetDaemonController() const = 0;
  virtual const IFloorController* GetFloorController() const = 0;
};

#endif // _CONTROLLER_MANAGER_INTERFACE_H_