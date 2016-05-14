#ifndef FLOOR_CONTROLLER_INTERFACE_H
#define FLOOR_CONTROLLER_INTERFACE_H

#include <QObject>
#include <dbus/BfcpClient/common/BfcpClientServiceCommon.h>
#include "floor_control_info_description.h"

class IFloorController : public QObject {
  Q_OBJECT
public:
  IFloorController(QObject *parent): QObject(parent) {}
  virtual ~IFloorController() {}

Q_SIGNALS:
  void NotifyFloorRequestStatusSignal(uint16_t user_id, bfcpc::RequestStatus status);

public Q_SLOTS:
  virtual void HandleCreateClientRequestSlot(
    uint16_t user_id, const FloorControlInfoPointer &info) = 0;
  virtual void HandleDestroyClientRequestSlot() = 0;
  virtual void HandleApplyFloorRequstSlot() = 0;
  virtual void HandleAcceptFloorRequestSlot(uint16_t user_id) = 0;
  virtual void HandleReleaseFloorRequestSlot(uint16_t user_id) = 0;
};

#endif // FLOOR_CONTROLLER_INTERFACE_H