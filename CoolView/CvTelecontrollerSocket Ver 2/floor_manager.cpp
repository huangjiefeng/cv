#include "floor_manager.h"

#include <dbus/BfcpServer/client/bfcp_server_proxy.h>
#include <dbus/BfcpClient/client/bfcp_client_proxy.h>

#include <windows.h>

#include "CommonMessageProcess.h"
using Common_Message_Space::SetNewMonitor;

std::atomic<FloorManager*> FloorManager::instance_ = nullptr;
std::mutex FloorManager::mutex_;

FloorManager* FloorManager::GetInstance() 
{
  FloorManager *tmp = instance_.load(std::memory_order_acquire);
  if (tmp == nullptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    tmp = instance_.load(std::memory_order_relaxed);
    if (tmp == nullptr) {
      tmp = new FloorManager(nullptr);
      instance_.store(tmp, std::memory_order_release);
    }
  }
  return tmp;
}

void FloorManager::ReleaseInstance() 
{
  FloorManager *tmp = instance_.load(std::memory_order_acquire);
  if (tmp) {
    std::lock_guard<std::mutex> lock(mutex_);
    tmp = instance_.load(std::memory_order_relaxed);
    if (tmp) {
      delete tmp;
      instance_.store(nullptr, std::memory_order_release);
    }
  }
}

FloorManager::FloorManager(QObject *parent)
    : QObject(parent), 
      conference_id_(0), 
      proxy_user_id_(0),
      server_proxy_(new BfcpServerProxy(this)),
      client_proxy_(new BfcpClientProxy(this)),
      next_avaliable_user_id_(proxy_user_id_ + 1),
      initialized_(false) 
{
  ClearClient();
  ConnectWithServerProxy(server_proxy_);
  ConnectWithClientProxy(client_proxy_);
}

FloorManager::~FloorManager() 
{
  if (server_proxy_) {
    server_proxy_->Quit();
    delete server_proxy_;
  }
  if (client_proxy_)
    delete client_proxy_;
}

void FloorManager::ConnectWithServerProxy(BfcpServerProxy *proxy) 
{
  assert(proxy);
  connect(
    proxy, &BfcpServerProxy::NotifyServerControlResultSignal,
    this, &FloorManager::HandleServerControlResultNotifySlot);

  connect(
    proxy, &BfcpServerProxy::NotifyConferenceControlResultSignal,
    this, &FloorManager::HandleConferenceControlResultNotifySlot);

  connect(
    proxy, &BfcpServerProxy::NotifyFloorControlResultSignal,
    this, &FloorManager::HandleFloorControlResultNotifySlot);

  connect(proxy, &BfcpServerProxy::NotifyUserControlResultSignal,
    this, &FloorManager::HandleUserControlResultNotifySlot);

  connect(proxy, &BfcpServerProxy::NotifyChairControlResultSignal,
    this, &FloorManager::HandleChairControlResultNotifySlot);
}

void FloorManager::ConnectWithClientProxy( BfcpClientProxy *proxy ) 
{
  assert(proxy);
  connect(
    proxy, &BfcpClientProxy::NotifyClientControlSuccessedSignal,
    this, &FloorManager::HandleClientControlSuccessNotifySlot);

  connect(
    proxy, &BfcpClientProxy::NotifyClientAlreadyExistSignal,
    this, &FloorManager::HandleClientAlreadyExistNotifySlot);

  connect(
    proxy, &BfcpClientProxy::NotifyClientNotExistSignal,
    this, &FloorManager::HandleClientNotExistNotifySlot);

  connect(
    proxy, &BfcpClientProxy::NotifyErrorReceivedSignal,
    this, &FloorManager::HandleErrorReceivdNotifySlot);

  connect(
    proxy, &BfcpClientProxy::NotifyFloorRequestInfoSignal,
    this, &FloorManager::HandleFloorRequestInfoSlot);

  connect(
    proxy, &BfcpClientProxy::NotifyUserStatusSignal,
    this, &FloorManager::HandleUserStatusNotifySlot);

  connect(
    proxy, &BfcpClientProxy::NotifyFloorStatusSignal,
    this, &FloorManager::HandleFloorStatusSlot);
}

void FloorManager::ClearClient() 
{
  client_id_ = -1;
  client_state_ = kNotCreated;
}

void FloorManager::Initialize() 
{
  if (initialized_) return;
  StartServer();
  CreateConference();
  initialized_ = true;
}

void FloorManager::StartServer() 
{
  server_arg_.af = AF_INET;
  server_arg_.port = 50000;
  server_arg_.enable_connection_thread = false;
  server_arg_.work_thread_num = 0;
  server_arg_.user_obsoleted_time = -1.0;
  server_proxy_->Start(server_arg_);
}

void FloorManager::CreateConference() 
{
  bfcps::ConferenceArg conference_arg;
  conference_arg.max_floor_request = 1;
  conference_arg.policy = bfcps::AcceptPolicy::kAutoAccept;
  conference_arg.time_for_chair_action = -1.0;
  server_proxy_->AddConference(conference_id_, conference_arg);

  bfcps::FloorArg floor_arg;
  floor_arg.max_granted_num = 1;
  floor_arg.max_holding_time = -1.0;
  for (auto &floor : floors_) {
    server_proxy_->AddFloor(conference_id_, floor.floor_id, floor_arg);
  }

  bfcps::UserArg user_arg;
  server_proxy_->AddUser(conference_id_, proxy_user_id_, user_arg);
}

void FloorManager::HandleServerControlResultNotifySlot(bfcps::ErrorCode error_code) 
{
  if (initialized_ && error_code != bfcps::ErrorCode::kNoError) {
    printf("[E]Server Control Error: %d\n", error_code);
  }
}

void FloorManager::HandleConferenceControlResultNotifySlot(
  bfcps::ErrorCode error_code, uint32_t conference_id) 
{
  if (initialized_ && conference_id_ == conference_id) {
    if (error_code != bfcps::ErrorCode::kNoError) {
      printf("[E]Conference Control Error: %d\n", error_code);
    }
  }
}

void FloorManager::HandleFloorControlResultNotifySlot(bfcps::ErrorCode error_code, 
                                                      uint32_t conference_id, 
                                                      uint16_t floor_id) 
{
  if (initialized_ && conference_id_ == conference_id) {
    if (error_code != bfcps::ErrorCode::kNoError) {
      printf("[E]Floor Control Error: %d\n", error_code);
    }
  }
}

void FloorManager::HandleUserControlResultNotifySlot( 
  bfcps::ErrorCode error_code, uint32_t conference_id, uint16_t user_id ) 
{
  if (initialized_ && conference_id_ == conference_id) {
    if (user_id == proxy_user_id_) {
      if (error_code == bfcps::ErrorCode::kNoError || 
          error_code == bfcps::ErrorCode::kUserAlreadyExist) {
        CreateClient();
      } else {
        printf("[E]User Control Error: %d\n", error_code);
      }
    }
  }
}

void FloorManager::HandleChairControlResultNotifySlot(bfcps::ErrorCode error_code, 
                                                      uint32_t conference_id, 
                                                      uint16_t floor_id,
                                                      uint16_t user_id) 
{
  if (initialized_ && conference_id_ == conference_id) {
    if (error_code != bfcps::ErrorCode::kNoError) {
      printf("[E]Chair Control Error: %d\n", error_code);
    }
  }
}

void FloorManager::CreateClient() 
{
  // FIXME: add creating state
  if (client_state_ == kNotCreated) {
    printf("[I]Create Client\n");
    client_state_ = kCreating;

    client_arg_.af = server_arg_.af;
    client_arg_.server_ip = "127.0.0.1";
    client_arg_.server_port = server_arg_.port;
    client_arg_.conference_id = conference_id_;
    client_arg_.user_id = proxy_user_id_;
    client_arg_.heart_beat_interval = -1.0;
    client_proxy_->CreateClient(client_arg_);
  }
}

void FloorManager::HandleClientControlSuccessNotifySlot(
    int client_id, bfcpc::ClientArg &arg) 
{
  if (client_arg_ == arg) {
    if (client_state_ == kCreating) {
      printf("[I]BFCP client is created: %d\n", client_id);
      client_id_ = client_id;
      client_state_ = kCreated;
    } else if (client_state_ == kCreated) {
      printf("[I]BFCP client is destroyed: %d\n", client_id);
      assert(client_id_ == client_id);
      ClearClient();
    }
  }
}

void FloorManager::HandleClientAlreadyExistNotifySlot(
    int client_id, bfcpc::ClientArg &arg) 
{
  if (client_state_ == kCreating && client_arg_ == arg) {
    printf("[W]BFCP client already exist: %d\n", client_id);
    client_id_ = client_id;
    client_state_ = kCreated;
  }
}

void FloorManager::HandleClientNotExistNotifySlot( int client_id )
{
  if (client_state_ == kCreated && client_id_ == client_id) {
    printf("[E]BFCP client not exist\n");
    client_state_ = kCreating;
    client_proxy_->CreateClient(client_arg_);
  }
}

void FloorManager::HandleClientStateChangeNotifySlot(
    int client_id, bfcpc::ClientState state) 
{
  if (client_state_ == kCreated && client_id_ == client_id)  {
    switch (state) {
      case bfcpc::kDisconnected:
        client_proxy_->SendHello(client_id); 
        break;

      case bfcpc::kConnected: {
        bfcpc::FloorIDList floor_ids;
        for (auto &floor : floors_) {
          floor_ids.push_back(floor.floor_id);
        }
        client_proxy_->SendFloorQuery(client_id, floor_ids);

        for (auto &elem : user_info_table_) {
          bfcpc::UserQueryArg arg;
          arg.beneficiary_id = elem.second.user_id;
          arg.use_beneficiary_id = true;
          client_proxy_->SendUserQuery(client_id_, arg);
        }
      } break;
    }
  }
}

void FloorManager::HandleErrorReceivdNotifySlot(int client_id, 
                                                const bfcpc::ResponseError &error) 
{
  if (client_state_ == kCreated && client_id_ == client_id) {
    printf("[E]Received BFCP error: %d, %s\n", error.error_code, qPrintable(error.info));
  }
}

void FloorManager::HandleFloorRequestInfoSlot(int client_id,
                                              const bfcpc::FloorRequestInfo &info) 
{
  if (client_state_ == kCreated && client_id_ == client_id) {
    printf("[I]Received BFCP FloorRequestInfo\n");
    UpdateFloorRequestInfo(info);
  }
}

void FloorManager::HandleUserStatusNotifySlot(int client_id, 
                                              const bfcpc::UserStatus &status) 
{
  if (client_state_ == kCreated && client_id_ == client_id) {
    printf("[I]Received BFCP UserStatus\n");
    for (auto &frq : status.frq_info_list) {
      UpdateFloorRequestInfo(frq);
    }
  }
}

void FloorManager::HandleFloorStatusSlot(int client_id, 
                                         const bfcpc::FloorStatus &status) 
{
  if (client_state_ == kCreated && client_id_ == client_id) {
    printf("[I]Received BFCP FloorStatus\n");
    if (!status.has_floor_id) return;
    for (auto &frq : status.frq_info_list) {
      UpdateFloorRequestInfo(frq);
    }
  }
}

void FloorManager::UpdateFloorRequestInfo( const bfcpc::FloorRequestInfo &info )
{
  if (info.content & bfcpc::ContentType::kHasRequestedByInfo &&
      info.content & bfcpc::ContentType::kHasOverallRequestStatus) {

    if (!info.overall_req_status.has_request_status) {
      return;
    }

    uint16_t beneficiary_id = info.request_by.id;
    if (info.content & bfcpc::ContentType::kHasBeneficiaryInfo) {
      beneficiary_id = info.beneficiary.id;
    }
    
    // find beneficiary user info
    auto it = std::find_if(user_info_table_.begin(), user_info_table_.end(),
      [=](const std::pair<const std::string, UserInfo> &elem) {
        return elem.second.user_id == beneficiary_id;
    });
    assert(it != user_info_table_.end());

    auto request_status = info.overall_req_status.status.request_status;

    // find floor request
    FloorRequestPtr floor_request = nullptr;
    auto &user_info = (*it).second;
    auto &floor_requests = user_info.floor_requests;
    auto lb = floor_requests.lower_bound(info.floor_request_id);
    if (lb != floor_requests.end() && info.floor_request_id == lb->first) {
      floor_request = (*lb).second;
      if (request_status == floor_request->status) {
        return;
      }
      floor_request->status = request_status;

    } else { // new floor request
      if (request_status != bfcpc::RequestStatus::kPending) {
        printf("[E]unexpected new floor request with status %d\n", request_status);
        return;
      }
      FloorRequestPtr new_floor_request(new FloorRequest(info, proxy_user_id_));
      auto tmp_it = floor_requests.insert(lb, 
        std::make_pair(info.floor_request_id, new_floor_request));
      floor_request = (*tmp_it).second;
    }
    
    assert(floor_request);
    switch (request_status) {
      case bfcpc::kPending:
        HandlePendingFloorRequest((*it).first, user_info, floor_request); 
        break;
      case bfcpc::kAccepted:
        HandleAcceptedFloorRequest((*it).first, user_info, floor_request);
        break;
      case bfcpc::kGranted:
        HandleGrantedFloorRequest((*it).first, user_info, floor_request);
        break;
      case bfcpc::kDenied:
        HandleDeniedFloorRequest((*it).first, user_info, floor_request);
        break;
      case bfcpc::kCancelled:
        HandleCancelledFloorRequest((*it).first, user_info, floor_request);
        break;
      case bfcpc::kRelease:
      case bfcpc::kRevoked:
        HandleReleaseFloorRequest((*it).first, user_info, floor_request);
        break;
    }
  }
}

void FloorManager::HandlePendingFloorRequest(const std::string &user_uri,
                                             UserInfo &user_info,
                                             FloorRequestPtr &floor_request)
{ 
  auto *data_manager = CDataManager::GetInstance();
  const char *tmp = data_manager->GetDataString("Chairman");
  bool beneficary_is_chairman = tmp != nullptr && user_uri == tmp;

  for (auto floor_id : floor_request->floor_ids)
  {
    // find floor
    auto it = std::find_if(floors_.begin(), floors_.end(), 
      [=](const Floor &floor) {
        return floor.floor_id == floor_id;
    });
    assert(it != floors_.end());

    if (it->chair_id == -1) { // no chair
      continue;
    }
    
    // find chair
    auto chair_it = std::find_if(user_info_table_.begin(), user_info_table_.end(),
      [&](const std::pair<const std::string, UserInfo> &elem) {
        return elem.second.user_id == static_cast<uint16_t>(it->chair_id);
    });
    assert(chair_it != user_info_table_.end());

    auto &chair = (*chair_it).second;
    if (chair.type == kThirdParty) { // third party chair
      if (chair.user_id == user_info.user_id) { // chair request floor from self
        // deny the floor request of chair
        bfcpc::ChairActionArg arg;
        arg.floor_ids.push_back(floor_id);
        arg.floor_request_id = floor_request->floor_request_id;
        arg.status.request_status = bfcpc::RequestStatus::kDenied;
        arg.status_info = QString::fromLocal8Bit("Chair request floor from self");
        client_proxy_->SendChairAction(client_id_, arg);

      } else if (beneficary_is_chairman) {
        // accept the floor request of chairman
        bfcpc::ChairActionArg arg;
        arg.floor_ids.push_back(floor_id);
        arg.floor_request_id = floor_request->floor_request_id;
        arg.status.request_status = bfcpc::RequestStatus::kAccepted;
        arg.status.queue_pos = 1; // the first position
        client_proxy_->SendChairAction(client_id_, arg);

        // release all floor requests with floor_id of chair
        for (auto &fq : chair.floor_requests) {
          if (fq.second->ContainFloor(floor_id)) {
            client_proxy_->SendFloorRelease(client_id_, fq.first);
          }
        }
      } else {
        // notify the chair with ApplyAccess
        if (chair.notify_index > 0) {
          // notify the monitor about the pending floor request
          Json::Value root;
          root["info_type"] = Json::Value(TELE_ApplyAccess);
          root["usersip"] = Json::Value(user_uri);
          root["index"] = Json::Value(user_info.notify_index);
          Json::FastWriter writer;
          std::string json_str = writer.write(root);
          CNetServ::GetInstance()->AddNotify(
            json_str.c_str(), json_str.size(), chair.notify_index);
        }
      }
    }
  }
}

void FloorManager::HandleAcceptedFloorRequest(const std::string &user_uri, 
                                              UserInfo &user_info,
                                              FloorRequestPtr &floor_request)
{
  // empty
}

void FloorManager::HandleDeniedFloorRequest(const std::string &user_uri, 
                                            UserInfo &user_info, 
                                            FloorRequestPtr &floor_request)
{
  if (user_info.type == kThirdParty) {
    NotifyApplyResult(user_info, false);
  }
  user_info.floor_requests.erase(floor_request->floor_request_id);
}

void FloorManager::HandleCancelledFloorRequest(const std::string &user_uri, 
                                               UserInfo &user_info, 
                                               FloorRequestPtr &floor_request)
{
  user_info.floor_requests.erase(floor_request->floor_request_id);
}

void FloorManager::NotifyApplyResult(const UserInfo &user_info, bool is_accepted)
{
  assert(user_info.type == kThirdParty);
  if (user_info.notify_index > 0) {
    Json::Value root;
    root["info_type"] = Json::Value(TELE_ApplyResult);
    root["result"] = Json::Value(is_accepted);
    Json::FastWriter writer;
    std::string json_str = writer.write(root);
    CNetServ::GetInstance()->AddNotify(
      json_str.c_str(), json_str.size(), user_info.notify_index);
  }
}

void FloorManager::HandleGrantedFloorRequest(const std::string &user_uri, 
                                             UserInfo &user_info, 
                                             FloorRequestPtr &floor_request)
{
  uint16_t actual_chair_id = 
    user_info.type == kFirstParty ? user_info.user_id : proxy_user_id_;

  for (auto floor_id : floor_request->floor_ids)
  {
    // find floor
    auto it = std::find_if(floors_.begin(), floors_.end(), 
      [=](const Floor &floor) {
        return floor.floor_id == floor_id;
    });
    assert(it != floors_.end());
    
    // set actual chair
    (*it).chair_id = user_info.user_id;
    server_proxy_->SetChair(conference_id_, floor_id, actual_chair_id);
  }

  if (user_info.type == kThirdParty) {
    NotifyApplyResult(user_info, true);
  }
}

void FloorManager::HandleReleaseFloorRequest(const std::string &user_uri, 
                                             UserInfo &user_info, 
                                             FloorRequestPtr &floor_request)
{
  for (auto floor_id : floor_request->floor_ids)
  {
    // find floor
    auto it = std::find_if(floors_.begin(), floors_.end(), 
      [=](const Floor &floor) {
        return floor.floor_id == floor_id;
    });
    assert(it != floors_.end());

    // remove chair
    (*it).chair_id = -1;
    server_proxy_->RemoveChair(conference_id_, floor_id);
  }
  user_info.floor_requests.erase(floor_request->floor_request_id);
}

void FloorManager::ApplyForAllFloors( const std::string &uri, int notify_index )
{
  assert(client_state_ == kCreated);

  // send third-party floor request for all floors
  bfcpc::FloorRequestArg arg;
  if (TryAddThirdPartyUser(uri, notify_index, arg.beneficiary_id)) {
    for (auto &floor : floors_) {
      arg.floor_ids.push_back(floor.floor_id);
    }
    arg.priority = bfcpc::kNormal;
    arg.use_benficirary_id = true;
    client_proxy_->SendFloorRequest(client_id_, arg);
  }
}

void FloorManager::ApplyForFloor( const std::string &uri, int notify_index, uint16_t floor_id )
{
  assert(client_state_ == kCreated);

  // send third-party floor request for all floors
  bfcpc::FloorRequestArg arg;
  if (TryAddThirdPartyUser(uri, notify_index, arg.beneficiary_id)) {
    arg.floor_ids.push_back(floor_id);
    arg.priority = bfcpc::kNormal;
    arg.use_benficirary_id = true;
    client_proxy_->SendFloorRequest(client_id_, arg);
  }
}

bool FloorManager::TryAddThirdPartyUser(const std::string &user_uri,
                                        int notify_index,
                                        uint16_t &user_id) 
{
  return TryAddUser(user_uri, notify_index, kThirdParty, user_id);
}

bool FloorManager::TryAddUser(const std::string &user_uri, 
                              int notify_index, 
                              UserType type,
                              uint16_t &user_id)
{
  auto it = user_info_table_.lower_bound(user_uri);
  if (it != user_info_table_.end() && user_uri == (*it).first) {
    (*it).second.notify_index = notify_index;
    user_id = (*it).second.user_id;

  } else {
    user_id = next_avaliable_user_id_++;
    if (user_id == 0) {
      printf("[E]Unable to assign User ID\n");
      return false;
    }
    user_info_table_.insert(
      it, std::make_pair(user_uri, UserInfo(user_id, notify_index, type)));

    bfcps::UserArg arg;
    arg.user_uri = QString::fromLocal8Bit(user_uri.c_str());
    server_proxy_->AddUser(conference_id_, user_id, arg);
  }
  return true;
}

void FloorManager::AcceptAccess(const std::string &chair_uri, 
                                const std::string &requester_uri)
{
  HandleFloorRequestByChairAction(chair_uri, requester_uri, bfcpc::RequestStatus::kAccepted);
}

void FloorManager::DenyAccess(const std::string &chair_uri,
                              const std::string &requester_uri)
{
  HandleFloorRequestByChairAction(chair_uri, requester_uri, bfcpc::RequestStatus::kDenied);
}

void FloorManager::HandleFloorRequestByChairAction(const std::string &chair_uri,
                                                   const std::string &requester_uri,
                                                   bfcpc::RequestStatus status)
{
  // find chair
  auto it = user_info_table_.find(chair_uri);
  if (it == user_info_table_.end()) {
    printf("[E]Chair %s is not found\n", chair_uri.c_str());
    return;
  }
  // only for third-party chair
  auto &chair_info = (*it).second;
  if (chair_info.type != kThirdParty) {
    printf("[E]Chair %s is not a third-party user\n", chair_uri.c_str());
    return;
  }
  // find chair floors
  std::vector<uint16_t> chair_floors;
  for (auto &floor : floors_)
  {
    if (floor.chair_id == chair_info.user_id) {
      chair_floors.push_back(floor.floor_id);
    }
  }
  assert(!chair_floors.empty());

  // find requester
  auto qit = user_info_table_.find(requester_uri);
  if (qit == user_info_table_.end()) {
    printf("[E]Requester %s is not found\n", requester_uri.c_str());
    return;
  }

  std::vector<uint16_t> floors_effected;

  // do the chair action to the floor requests of the requester
  auto &requester_info = (*qit).second;
  for (auto &fq : requester_info.floor_requests) {
    // only the pending floor requests
    if (fq.second->status != bfcpc::RequestStatus::kPending) {
      continue;
    }

    std::vector<uint16_t> floors_to_chair_action;
    for (auto floor_id : chair_floors) {
      if (fq.second->ContainFloor(floor_id)) {
        floors_to_chair_action.push_back(floor_id);
        floors_effected.push_back(floor_id);
      }
    }
    if (!floors_to_chair_action.empty()) {
      bfcpc::ChairActionArg arg;
      arg.floor_request_id = fq.first;
      arg.floor_ids = QVector<uint16_t>::fromStdVector(floors_to_chair_action);
      arg.status.request_status = status;
      arg.status.queue_pos = 0; 
      client_proxy_->SendChairAction(client_id_, arg);
    }
  }
  
  if (!floors_effected.empty() && status == bfcpc::RequestStatus::kAccepted) {
    // release the floor requests of chair
    for (auto &fq : chair_info.floor_requests) {
      for (auto floor_id : floors_effected) {
        if (fq.second->ContainFloor(floor_id)) {
          client_proxy_->SendFloorRelease(client_id_, fq.first);
          break;
        }
      }
    }
  }
}

void FloorManager::UpdateUserNotifyIndex( const std::string &uri, int notify_index )
{
  uint16_t user_id;
  TryAddThirdPartyUser(uri, notify_index, user_id);
}

void FloorManager::OnUserOffline( const std::string &uri )
{
  auto it = user_info_table_.find(uri);
  if (it == user_info_table_.end()) {
    printf("[E]User %s is not found\n", uri.c_str());
    return;
  }
  auto &user_info = (*it).second;
  // Only third-party floor request send by proxy can be released
  for (auto &fq : user_info.floor_requests) {
    if (fq.second->request_by_proxy) {
      client_proxy_->SendFloorRelease(client_id_, fq.first);
    }
  }
  user_info.notify_index = -1;
}

void FloorManager::AddFloor( uint16_t floor_id )
{
  Floor floor;
  floor.floor_id = floor_id;
  floor.chair_id = -1;
  floors_.push_back(floor);

  if (initialized_) {
    bfcps::FloorArg floor_arg;
    floor_arg.max_granted_num = 1;
    floor_arg.max_holding_time = -1.0;
    server_proxy_->AddFloor(conference_id_, floor_id, floor_arg);
  }
}

bool FloorManager::IsUserHasFloor( const std::string &uri, uint16_t floor ) const
{
  auto it = user_info_table_.find(uri);
  if (it == user_info_table_.end()) {
    printf("[E]User %s not found\n", uri.c_str());
    return false;
  }
  for (auto & floor : floors_) {
    if (floor.chair_id == (*it).second.user_id) {
      return true;
    }
  }
  return false;
}

bool FloorManager::IsUserHasAllFloors( const std::string &uri ) const
{
  auto it = user_info_table_.find(uri);
  if (it == user_info_table_.end()) {
    printf("[E]User %s not found\n", uri.c_str());
    return false;
  }
  for (auto & floor : floors_) {
    if (floor.chair_id != (*it).second.user_id) {
      return false;
    }
  }
  return true;
}

bool FloorManager::IsFloorGranted( uint16_t floor_id ) const
{
  for (auto &floor : floors_) {
    if (floor.floor_id == floor_id) {
      return floor.chair_id != -1;
    }
  }
  assert(false);
  return false;
}

bool FloorManager::IsAnyFloorGranted() const
{
  for (auto &floor : floors_) {
    if (floor.chair_id != -1) {
      return true;
    }
  }
  return false;
}

std::string FloorManager::GetFirstChairURI() const
{
  for (auto &floor : floors_) {
    if (floor.chair_id != -1) {
      auto it = std::find_if(user_info_table_.begin(), user_info_table_.end(),
        [&](const std::pair<const std::string, UserInfo> &elem) {
          return elem.second.user_id == floor.chair_id;
      });
      assert(it != user_info_table_.end());
      return (*it).first;
    }
  }
  return std::string();
}

