#include "offline_session_controller.h"


OfflineSessionController::OfflineSessionController(QObject *parent)
  : ISessionController(parent), 
    state_(kIsOffline)
{

}

void OfflineSessionController::HandleLoginRequestSlot()
{
  state_ = kIsOnline;
  emit NotifySessionStateChangedSignal(state_);
}

void OfflineSessionController::HandleLogoutRequestSlot()
{
  state_ = kIsOffline;
  emit NotifySessionStateChangedSignal(state_);
}
