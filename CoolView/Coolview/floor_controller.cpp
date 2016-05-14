#include "floor_controller.h"

#include <cassert>

#include <dbus/BfcpClient/client/bfcp_client_proxy.h>
#include <winsock2.h>
#include <log/Log.h>

const double FloorController::kDefaultHearBeatInterval = 8.0;

FloorController::FloorController( QObject *parent )
    : IFloorController(parent), proxy_(nullptr) {
  Clear();
}


FloorController::~FloorController(void)
{
  if (proxy_) {
    delete proxy_;
  }
}

void FloorController::Clear()
{
  created_ = false;
  client_id_ = -1;
  floor_ids_.clear();
  charged_floor_ids_.clear();
  user_floor_statuses_.clear();
}

void FloorController::HandleCreateClientRequestSlot(
    uint16_t user_id, const FloorControlInfoPointer &info) {
  if (created_) return;
  
  if (!proxy_) {
    proxy_ = new BfcpClientProxy(this);
    Connect();
  }
  bfcpc::ClientArg arg;
  arg.af = info->ip.contains(":") ? AF_INET6 : AF_INET;
  arg.conference_id = static_cast<uint32_t>(info->cid.toInt());
  arg.server_ip = info->ip;
  arg.server_port = info->port;
  arg.user_id = user_id;
  arg.heart_beat_interval = -1.0;
  local_client_arg_ = arg;
  
  proxy_->CreateClient(arg);

  local_user_id_ = user_id;

  // 记录所有的floor
  floor_ids_.reserve(info->floors.size());
  for (auto &floor : info->floors) {
    floor_ids_.push_back(floor.floor_id);
  }

  // 记录用户所管理的floor
  for (auto &chair : info->chairs) {
    if (chair.user_id == user_id) {
      for (auto floor_id : chair.floor_ids) {
        charged_floor_ids_.insert(floor_id);
      }
      break;
    }
  }
}

void FloorController::Connect()
{
  assert(proxy_);
  connect(proxy_, &BfcpClientProxy::NotifyClientControlSuccessedSignal,
    this, &FloorController::HandleClientControlSuccessedSignalSlot);

  connect(proxy_, &BfcpClientProxy::NotifyClientAlreadyExistSignal,
    this, &FloorController::HandleClientAlreadyExistSignalSlot);

  connect(proxy_, &BfcpClientProxy::NotifyClientNotExistSignal,
    this, &FloorController::HandleClientNotExistSignalSlot);

  connect(proxy_, &BfcpClientProxy::NotifyStateChangeSignal,
    this, &FloorController::HandleClientStateChangedSignalSlot);

  connect(proxy_, &BfcpClientProxy::NotifyErrorReceivedSignal,
    this, &FloorController::HandleErrorReceivedSignalSlot);
  
  connect(proxy_, &BfcpClientProxy::NotifyFloorRequestInfoSignal,
    this, &FloorController::HandleFloorRequestInfoSignalSlot);

  connect(proxy_, &BfcpClientProxy::NotifyUserStatusSignal,
    this, &FloorController::HandleUserStatusSignalSlot);
  
  connect(proxy_, &BfcpClientProxy::NotifyFloorStatusSignal,
    this, &FloorController::HandleFloorStatusSignalSlot);
}

void FloorController::HandleDestroyClientRequestSlot()
{
  if (!proxy_ || !created_) return;
  HandleReleaseFloorRequestSlot(local_user_id_);
  proxy_->DestroyClient(client_id_);
}

void FloorController::HandleClientControlSuccessedSignalSlot(
    int client_id, bfcpc::ClientArg &arg ) {
  if (local_client_arg_ == arg) {
    if (!created_) {
      CV_LOG_INFO("BFCP客户端创建成功");
      client_id_ = client_id;
      created_ = true;
    } else {
      CV_LOG_INFO("BFCP客户端删除成功");
      assert(client_id_ == client_id);
      Clear();
    }
  }
}

void FloorController::HandleClientAlreadyExistSignalSlot(
    int client_id, bfcpc::ClientArg &arg) {
  if (!created_ && local_client_arg_ == arg) {
    CV_LOG_ERROR("BFCP客户端已存在");
    client_id_ = client_id;
    created_ = true;
  }
}

void FloorController::HandleClientNotExistSignalSlot( int client_id ) {
  if (created_ && client_id_ == client_id) {
    CV_LOG_ERROR("BFCP客户端尚未创建");
    // 重新创建BFCP客户端
    proxy_->CreateClient(local_client_arg_);
    created_ = false;
  }
}

void FloorController::HandleClientStateChangedSignalSlot(
    int client_id, bfcpc::ClientState state) {
  if (created_ && client_id_ == client_id) {
    switch (state) {
      case bfcpc::kDisconnected:
        // 断线重连
        CV_LOG_ERROR("与FCS连接中断，重新连接");
        proxy_->SendHello(client_id);
        break;
      case bfcpc::kConnected: {
        // 查询自身的发言权状况
        bfcpc::UserQueryArg arg;
        arg.use_beneficiary_id = false;
        proxy_->SendUserQuery(client_id, arg);
        // 查询所有floor的状况
        proxy_->SendFloorQuery(client_id_, floor_ids_);
        // 查询所有用户的状态
        for (auto &user_floor_status : user_floor_statuses_) {
          bfcpc::UserQueryArg arg;
          arg.beneficiary_id = user_floor_status.user_id;
          arg.use_beneficiary_id = true;
          proxy_->SendUserQuery(client_id_, arg);
        }
      } break;
    }
  }
}

void FloorController::HandleErrorReceivedSignalSlot(
    int client_id, const bfcpc::ResponseError &error) {
  if (created_ && client_id_ == client_id) {
    QString msg = 
      QString::fromLocal8Bit("BFCP错误%1, %2").arg(error.error_code).arg(error.info);
    CV_LOG_ERROR(msg.toLocal8Bit().constData());
  }
}

void FloorController::HandleFloorRequestInfoSignalSlot(
    int client_id, const bfcpc::FloorRequestInfo &info) {
  if (created_ && client_id_ == client_id) {
    UpdateFloorRequestInfo(info);
  }
}

void FloorController::UpdateFloorRequestInfo( const bfcpc::FloorRequestInfo &info )
{
  if (info.content & bfcpc::ContentType::kHasRequestedByInfo &&
      info.content & bfcpc::ContentType::kHasOverallRequestStatus) {
    if (!info.overall_req_status.has_request_status) {
      return;
    }

    uint16_t request_user_id = info.request_by.id;
    auto it = user_floor_statuses_.find(request_user_id);
    if (it == user_floor_statuses_.end()) {
      it = user_floor_statuses_.insert(
        it, request_user_id, UserFloorStatus(request_user_id));
    }

    auto &user_floor_status = it.value();
    int floor_request_id = static_cast<int>(info.floor_request_id);
    if (user_floor_status.floor_request_id == -1 && 
        user_floor_status.floor_request_id != floor_request_id) {
      user_floor_status.floor_request_id = floor_request_id;
      proxy_->SendFloorRequestQuery(client_id_, floor_request_id);
    }
    
    auto request_status = info.overall_req_status.status.request_status;
    if (user_floor_status.request_status != request_status) {
      user_floor_status.request_status = request_status;
      switch (request_status)
      {
        case bfcpc::kPending:
        case bfcpc::kAccepted:
        case bfcpc::kGranted:
          break;
        case bfcpc::kDenied:
        case bfcpc::kCancelled:
        case bfcpc::kRelease:
        case bfcpc::kRevoked:
          user_floor_status.floor_request_id = -1;
          user_floor_status.request_status = -1;
          break;
      }
      if (local_user_id_ == request_user_id && request_status == bfcpc::kPending) {
        // 尝试接受自己的请求
        AcceptFloorRequest(floor_request_id);
      }
      emit NotifyFloorRequestStatusSignal(request_user_id, request_status);
    }
  }
}

void FloorController::HandleUserStatusSignalSlot(
    int client_id, const bfcpc::UserStatus &status) {
  if (created_ && client_id_ == client_id) {
    for (auto &frq : status.frq_info_list) {
      UpdateFloorRequestInfo(frq);
    }
  }
}

void FloorController::HandleFloorStatusSignalSlot(
    int client_id, const bfcpc::FloorStatus &status) {
  if (created_ && client_id_ == client_id) {
    if (!status.has_floor_id) return;
    for (auto &frq : status.frq_info_list) {
      UpdateFloorRequestInfo(frq);
    }
  }
}

void FloorController::HandleApplyFloorRequstSlot() {
  if (!created_) {
    CV_LOG_ERROR("尚未创建BFCP客户端");
    return;
  }
  bfcpc::FloorRequestArg arg;
  arg.priority = bfcpc::BfcpPriority::kNormal;
  arg.floor_ids = floor_ids_;
  arg.use_benficirary_id = false;
  proxy_->SendFloorRequest(client_id_, arg);
}

void FloorController::HandleAcceptFloorRequestSlot( uint16_t user_id )
{
  if (!created_) {
    CV_LOG_ERROR("尚未创建BFCP客户端");
    return;
  }

  auto it = user_floor_statuses_.find(user_id);
  if (it == user_floor_statuses_.end()) {
    CV_LOG_ERROR("找不到BFCP用户");
    return;
  }
  auto &user_floor_status = it.value();
  auto floor_request_id = user_floor_status.floor_request_id;
  if (floor_request_id == -1) {
    CV_LOG_ERROR("用户没有发言权请求");
    return;
  }
  if (user_floor_status.request_status != bfcpc::RequestStatus::kPending) {
    CV_LOG_ERROR("无法Accept发言权请求");
    return;
  }
  AcceptFloorRequest(floor_request_id);
}

void FloorController::AcceptFloorRequest( uint16_t floor_request_id ) {
  DoChairAction(floor_request_id, bfcpc::RequestStatus::kAccepted);
}

void FloorController::HandleReleaseFloorRequestSlot( uint16_t user_id )
{
  auto it = user_floor_statuses_.find(user_id);
  if (it == user_floor_statuses_.end()) {
    CV_LOG_ERROR("找不到BFCP用户");
    return;
  }
  auto &user_floor_status = it.value();
  auto floor_request_id = user_floor_status.floor_request_id;
  if (floor_request_id == -1) {
    CV_LOG_ERROR("用户没有发言权请求");
    return;
  }
  if (user_id == local_user_id_) {
    proxy_->SendFloorRelease(client_id_, floor_request_id);
  } else {
    auto request_status = user_floor_status.request_status;
    if (request_status == bfcpc::RequestStatus::kPending) {
      DenyFloorRequest(floor_request_id);
    } else if (request_status == bfcpc::RequestStatus::kGranted) {
      RevokeFloorRequest(floor_request_id);
    }
  }
}

void FloorController::DenyFloorRequest( uint16_t floor_request_id ) {
  DoChairAction(floor_request_id, bfcpc::RequestStatus::kDenied);
}

void FloorController::RevokeFloorRequest( uint16_t floor_request_id ) {
  DoChairAction(floor_request_id, bfcpc::RequestStatus::kRevoked);
}

void FloorController::DoChairAction( uint16_t floor_request_id, bfcpc::RequestStatus status )
{
  if (charged_floor_ids_.empty()) {
    CV_LOG_ERROR("当前用户不是主席");
    return;
  }

  bfcpc::ChairActionArg arg;
  arg.floor_request_id = floor_request_id;
  arg.floor_ids.reserve(charged_floor_ids_.size());
  for (auto floor_id : charged_floor_ids_) {
    arg.floor_ids.push_back(floor_id);
  }
  arg.status.request_status = status;
  arg.status.queue_pos = 0;
  
  proxy_->SendChairAction(client_id_, arg);
}

