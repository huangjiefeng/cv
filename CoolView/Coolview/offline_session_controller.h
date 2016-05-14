/************************************************************************/
/* 定义一个用于测试的离线假session                                      */
/************************************************************************/

#ifndef OFFLINE_SESSION_CONTROLLER_H
#define OFFLINE_SESSION_CONTROLLER_H

#include <QObject>

#include "session_controller_interface.h"

#include "conference_controller_interface.h"
#include "msg_manager_interface.h"

class OfflineSessionController : public ISessionController {
  Q_OBJECT

 public:
  OfflineSessionController(QObject *parent);

  void Initialize(
    IMsgManager *msg_manager,
    IConferenceController *conference_controller) {}

  SessionState GetCurrentState() const override { return state_; }

 public Q_SLOTS: 
  void HandleLoginRequestSlot() override;
  void HandleLogoutRequestSlot() override;

private:
  SessionState state_;
};

#endif // OFFLINE_SESSION_CONTROLLER_H
