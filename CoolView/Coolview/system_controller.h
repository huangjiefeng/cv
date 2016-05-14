#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <functional>
#include <QTimer>

#include "system_controller_interface.h"
#include "session_controller_interface.h"
#include "msg_manager_interface.h"
#include "config_description.h"

class SystemController : public ISystemController {
  Q_OBJECT
public:
  SystemController(QObject *parent);
  ~SystemController();

  void Initialize(IMsgManager *msg_manager, ISessionController *session_controller);

public Q_SLOTS:
  void HandleRestartSystemRequestSlot() override;
  void HandleShutDownSystemRequestSlot() override;
  void HandleQuitSystemRequestSlot() override;

private Q_SLOTS:
  void HandleSessionStateChangedNotify(ISessionController::SessionState state);
  void ExecutionActionSlot();
  void HandleRecvTerminalConfigSlot(const ConfigDict &config);

Q_SIGNALS:
  void LogoutRequestSignal();
  void RequestGetTerminalConfigSignal();
  void ReportTerminalConfigSignal(const ConfigDict &config);

private:
  std::function<void()> action_;
  ISessionController *session_controller_;
};

#endif // SYSTEM_CONTROLLER_H
