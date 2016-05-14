#ifndef _SESSION_CONTROLLER_INTERFACE_H_
#define _SESSION_CONTROLLER_INTERFACE_H_

#include <QObject>

class ISessionController : public QObject {
  Q_OBJECT

public:
  enum SessionState {
    kIsOnline = 0,
    kIsLogining,
    kIsOffline,
  };

  enum LoginError {
    kOk = 0,
    kErrorOfUnknown,
    kErrorOfTimeOut,
    kErrorOfAuthenticationFailure,
    kErrorOfConnectToServerFailure,
    kErrorOfPermissionDenied,
    kErrorOfAccessNetworkAdaptorFailure,
    kErrorOfNetworkInterfaceNonoperational,
    kErrorOfAlreadyLogin,
  };

public:
  ISessionController(QObject *parent) : QObject(parent) {}
  virtual ~ISessionController() {};

  static void RegisterMetaType() {
    qRegisterMetaType<ISessionController::SessionState>(
      "ISessionController::SessionState");
    qRegisterMetaType<ISessionController::LoginError>(
      "ISessionController::LoginError");
  }

  virtual SessionState GetCurrentState() const = 0;

Q_SIGNALS:
  void NotifyLoginErrorSignal(ISessionController::LoginError error);
  void NotifySessionStateChangedSignal(ISessionController::SessionState state);
  void NotifyRecvSaviSignal(bool res);

public Q_SLOTS: 
  virtual void HandleLoginRequestSlot() = 0;
  virtual void HandleLogoutRequestSlot() = 0;

};

#endif // _SESSION_CONTROLLER_INTERFACE_H_