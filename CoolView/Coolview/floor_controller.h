#ifndef FLOOR_CONTROLLER_H
#define FLOOR_CONTROLLER_H

#include <memory>
#include <QSet>
#include <QMap>
#include <dbus/BfcpClient/common/BfcpClientServiceCommon.h>
#include "floor_controller_interface.h"

class BfcpClientProxy;
class IConferenceController;

class FloorController : public IFloorController
{
public:
  static const double kDefaultHearBeatInterval;

  FloorController(QObject *parent);
  virtual ~FloorController(void);

public Q_SLOTS:
  void HandleCreateClientRequestSlot(
    uint16_t user_id, 
    const FloorControlInfoPointer &info) override;

  void HandleDestroyClientRequestSlot() override;

  void HandleApplyFloorRequstSlot() override;
  void HandleAcceptFloorRequestSlot(uint16_t user_id) override;
  void HandleReleaseFloorRequestSlot(uint16_t user_id) override;

private Q_SLOTS:
  void HandleClientControlSuccessedSignalSlot(
    int client_id, 
    bfcpc::ClientArg &arg);

  void HandleClientAlreadyExistSignalSlot(
    int client_id, 
    bfcpc::ClientArg &arg);
  
  void HandleClientNotExistSignalSlot(int client_id);

  void HandleClientStateChangedSignalSlot(
    int client_id, 
    bfcpc::ClientState state);

  void HandleErrorReceivedSignalSlot(
    int client_id, 
    const bfcpc::ResponseError &error);

  void HandleFloorRequestInfoSignalSlot(
    int client_id, 
    const bfcpc::FloorRequestInfo &info);

  void HandleUserStatusSignalSlot(
    int client_id,
    const bfcpc::UserStatus &status);

  void HandleFloorStatusSignalSlot(
    int client_id,
    const bfcpc::FloorStatus &status);

private:
  struct UserFloorStatus {
    UserFloorStatus(uint16_t user_id) {
      this->user_id = user_id;
      floor_request_id = -1;
      request_status = -1;
    }
    uint16_t user_id;
    int floor_request_id;
    int request_status;
  };

  void Connect();
  void Clear();

  void AcceptFloorRequest(uint16_t floor_request_id);
  void DenyFloorRequest(uint16_t floor_request_id);
  void RevokeFloorRequest(uint16_t floor_request_id);
  void DoChairAction(uint16_t floor_request_id, bfcpc::RequestStatus status);

  void UpdateFloorRequestInfo(const bfcpc::FloorRequestInfo &info);
  void OnFloorRequestPending(const bfcpc::FloorRequestInfo &info);

  BfcpClientProxy *proxy_;

  int client_id_; // 当前客户端id
  uint16_t local_user_id_;  // 当前用户id
  bfcpc::ClientArg local_client_arg_; // 客户端参数
  QSet<uint16_t> charged_floor_ids_;  // 记录当前用户所管理的floor id
  bfcpc::FloorIDList floor_ids_; // 记录当前会议所有的floor id
  QMap<uint16_t, UserFloorStatus> user_floor_statuses_;
  bool created_;  // 是否已创建
};

#endif //FLOOR_CONTROLLER_H


