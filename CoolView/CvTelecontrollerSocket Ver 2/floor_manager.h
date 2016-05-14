#ifndef FLOOR_MANAGER_H
#define FLOOR_MANAGER_H

#include <atomic>
#include <mutex>
#include <map>
#include <string>
#include <memory>

#include <QObject>


#include <dbus/BfcpServer/common/BfcpServerServiceCommon.h>
#include <dbus/BfcpClient/common/BfcpClientServiceCommon.h>

class BfcpServerProxy;
class BfcpClientProxy;

class FloorManager : public QObject
{
  Q_OBJECT
public:
  enum ClientState 
  {
    kNotCreated = 0,
    kCreating = 1,
    kCreated = 2,
  };

  static FloorManager* GetInstance();
  static void ReleaseInstance();

  void Initialize();

  void AddFloor(uint16_t floor_id);
  bool IsUserHasFloor(const std::string &uri, uint16_t floor) const;
  bool IsUserHasAllFloors(const std::string &uri) const;
  bool IsFloorGranted(uint16_t floor_id) const;
  bool IsAnyFloorGranted() const;
  
  std::string GetFirstChairURI() const;

  void ApplyForAllFloors(const std::string &uri, int notify_index);
  void ApplyForFloor(const std::string &uri, int notify_index, uint16_t floor_id);
  void AcceptAccess(const std::string &chair_uri, const std::string &requester_uri);
  void DenyAccess(const std::string &chair_uri, const std::string &requester_uri);
  void UpdateUserNotifyIndex(const std::string &uri, int notify_index);
  void OnUserOffline(const std::string &uri);

private Q_SLOTS:
  /* handle server proxy notify */
  void HandleServerControlResultNotifySlot(bfcps::ErrorCode error_code);
  void HandleConferenceControlResultNotifySlot(
    bfcps::ErrorCode error_code, 
    uint32_t conference_id);
  void HandleFloorControlResultNotifySlot(
    bfcps::ErrorCode error_code, 
    uint32_t conference_id, 
    uint16_t floor_id);
  void HandleUserControlResultNotifySlot(
    bfcps::ErrorCode error_code, 
    uint32_t conference_id, 
    uint16_t user_id);
  void HandleChairControlResultNotifySlot(
    bfcps::ErrorCode error_code, 
    uint32_t conference_id, 
    uint16_t floor_id, 
    uint16_t user_id);

  /* handle client proxy notify */
  void HandleClientControlSuccessNotifySlot(
    int client_id, 
    bfcpc::ClientArg &arg);
  void HandleClientAlreadyExistNotifySlot(
    int client_id, 
    bfcpc::ClientArg &arg);
  void HandleClientNotExistNotifySlot(int client_id);
  void HandleClientStateChangeNotifySlot(
    int client_id, 
    bfcpc::ClientState state);
  void HandleErrorReceivdNotifySlot(
    int client_id, 
    const bfcpc::ResponseError &error);
  void HandleFloorRequestInfoSlot(
    int client_id,
    const bfcpc::FloorRequestInfo &info);
  void HandleUserStatusNotifySlot(
    int client_id,
    const bfcpc::UserStatus &status);
  void HandleFloorStatusSlot(
    int client_id,
    const bfcpc::FloorStatus &status);

private:
  enum UserType 
  {
    kFirstParty,
    kThirdParty,
  };

  struct FloorRequest 
  {
    FloorRequest(const bfcpc::FloorRequestInfo &info, uint16_t proxy_user_id)
        : floor_request_id(info.floor_request_id),
          request_by_proxy(proxy_user_id == info.request_by.id),
          status(info.overall_req_status.status.request_status) 
    {
      for (auto &floor_req_status : info.floor_req_statuses) {
        floor_ids.push_back(floor_req_status.floor_id);
      }
    }

    bool ContainFloor(uint16_t floor_id) {
      auto it = std::find(floor_ids.begin(), floor_ids.end(), floor_id);
      return it != floor_ids.end();
    }

    uint16_t floor_request_id;
    std::vector<uint16_t> floor_ids;
    bfcpc::RequestStatus status;
    bool request_by_proxy;
  };
  typedef std::shared_ptr<FloorRequest> FloorRequestPtr;

  // key: floor request id, value: floor request
  typedef std::map<uint16_t, FloorRequestPtr> FloorRequestMap;

  struct UserInfo 
  {
    UserInfo(uint16_t id, int index, UserType t) 
        : user_id(id), 
          notify_index(index), 
          type(t) 
    {}

    uint16_t user_id;

    // Use only for third party
    // > 0: avaliable remote controller; 
    // == -1: disavailable remote controller;
    int notify_index;

    FloorRequestMap floor_requests;

    UserType type;
  };

  struct Floor 
  {
    uint16_t floor_id;
    int chair_id;
  };

  // key: uri, value: user info
  typedef std::map<std::string, UserInfo> UserInfoMap;

  FloorManager(QObject *parent);
  ~FloorManager();

  void ConnectWithServerProxy(BfcpServerProxy *proxy);
  void ConnectWithClientProxy(BfcpClientProxy *proxy);
  void StartServer();
  void CreateConference();
  void CreateClient();
  void ClearClient();

  void UpdateFloorRequestInfo(const bfcpc::FloorRequestInfo &info);

  bool TryAddThirdPartyUser(const std::string &user_uri, int notify_index, uint16_t &user_id);
  bool TryAddUser(const std::string &user_uri, int notify_index, UserType type, uint16_t &user_id);

  void HandlePendingFloorRequest(
    const std::string &user_uri, 
    UserInfo &user_info,
    FloorRequestPtr &floor_request);
  void HandleAcceptedFloorRequest(
    const std::string &user_uri,
    UserInfo &user_info,
    FloorRequestPtr &floor_request);
  void HandleDeniedFloorRequest(
    const std::string &user_uri,
    UserInfo &user_info,
    FloorRequestPtr &floor_request);
  void HandleCancelledFloorRequest(
    const std::string &user_uri,
    UserInfo &user_info,
    FloorRequestPtr &floor_request);
  void HandleGrantedFloorRequest(
    const std::string &user_uri,
    UserInfo &user_info,
    FloorRequestPtr &floor_request);
  void HandleReleaseFloorRequest(
    const std::string &user_uri,
    UserInfo &user_info,
    FloorRequestPtr &floor_request);

  void NotifyApplyResult(const UserInfo &user_info, bool is_accepted);
  void HandleFloorRequestByChairAction(
    const std::string &chair_uri,
    const std::string &requester_uri, 
    bfcpc::RequestStatus status);

private:
  static std::atomic<FloorManager*> instance_;
  static std::mutex mutex_;

  uint32_t conference_id_;
  uint16_t proxy_user_id_; // telecontroller user id
  
  std::vector<Floor> floors_;

  UserInfoMap user_info_table_;

  bfcps::ServerArg server_arg_;
  BfcpServerProxy *server_proxy_;

  int client_id_;
  ClientState client_state_;
  bfcpc::ClientArg client_arg_;
  BfcpClientProxy *client_proxy_;
  
  uint16_t next_avaliable_user_id_;
  bool initialized_;
};

#endif // FLOOR_MANAGER_H
