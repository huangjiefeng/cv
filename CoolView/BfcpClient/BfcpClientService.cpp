#include "BfcpClientService.h"

#include <cassert>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <muduo/base/Logging.h>
#include <dbus/BfcpClient/service/BfcpClientAdaptor.h>

namespace details
{

void Convert(bfcpc::OverallRequestStatus &overal_req_status, 
             const bfcp::OverallRequestStatusParam &param)
{
  overal_req_status.floor_request_id = param.floorRequestID;
  overal_req_status.status.request_status = 
    static_cast<bfcpc::RequestStatus>(param.requestStatus.status);
  overal_req_status.status.queue_pos = param.requestStatus.qpos;
  overal_req_status.status_info = QString::fromUtf8(param.statusInfo.c_str());
  overal_req_status.has_request_status = param.hasRequestStatus;
}

void Convert(bfcpc::FloorRequestStatusList &floor_req_statuses, 
             const bfcp::FloorRequestStatusParamList &param_list)
{
  floor_req_statuses.reserve(param_list.size());
  floor_req_statuses.clear();
  bfcpc::FloorRequestStatus req_status;
  for (auto &param : param_list)
  {
    req_status.floor_id = param.floorID;
    req_status.status.request_status = 
      static_cast<bfcpc::RequestStatus>(param.requestStatus.status);
    req_status.status.queue_pos = param.requestStatus.qpos;
    req_status.status_info = QString::fromUtf8(param.statusInfo.c_str());
    req_status.has_request_status = param.hasRequestStatus;
    floor_req_statuses.push_back(req_status);
  }
}

void Convert(bfcpc::UserInfo &info, const bfcp::UserInfoParam &param)
{
  info.id = param.id;
  info.user_name = QString::fromUtf8(param.username.c_str());
  info.user_uri = QString::fromUtf8(param.useruri.c_str());
}

void Convert(bfcpc::FloorRequestInfo &info, const bfcp::FloorRequestInfoParam &param)
{
  info.floor_request_id = param.floorRequestID;
  info.content = param.valueType;
  
  Convert(info.overall_req_status, param.oRS);
  Convert(info.floor_req_statuses, param.fRS);
  Convert(info.beneficiary, param.beneficiary);
  Convert(info.request_by, param.requestedBy);

  info.priority = static_cast<int>(param.priority);
  info.participant_priovided_info = 
    QString::fromUtf8(param.partPriovidedInfo.c_str());
}

void Convert(bfcpc::FloorRequestInfoList &info_list, 
             const bfcp::FloorRequestInfoParamList &param_list)
{
  info_list.reserve(param_list.size());
  info_list.clear();
  bfcpc::FloorRequestInfo info;
  for (auto &param : param_list)
  {
    Convert(info, param);
    info_list.push_back(info);
  }
}

} // namespace details


BfcpClientService::BfcpClientService(QObject *parent)
    : QObject(parent), next_client_id_(0), loop_(nullptr)
{
  LOG_TRACE << "BfcpClientService::BfcpClientService constructing";
  new BfcpClientAdaptor(this);
}

BfcpClientService::~BfcpClientService()
{
  LOG_TRACE << "BfcpClientService::~BfcpClientService destructing";
  for (auto &client : clients_)
  {
    loop_->runInLoop(boost::bind(
      &BfcpClientService::ResetClient, std::move(client.second.base_client)));
    client.second.base_client = nullptr;
  }
  clients_.clear();
}

void BfcpClientService::Initialize()
{
  // dbus
  QDBusConnection connection = QDBusConnection::sessionBus();
  bool ret = connection.registerService(BFCP_CLIENT_SERVICE_NAME);
  if (!ret)
  {
    QDBusError err = connection.lastError();
    LOG_ERROR << "Cannot register service: " << qPrintable(err.name());
    return;
  }

  ret = connection.registerObject(BFCP_CLIENT_SERVICE_OBJECT_PATH, this);
  if (!ret)
  {
    LOG_ERROR << "Cannot register object";
    return;
  }
  LOG_INFO << "Register service success!";
}

void BfcpClientService::CreateClient(const QByteArray &client_arg)
{
  QDataStream data_stream(client_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::ClientArg arg;
  data_stream >> arg;

  QByteArray ip_bytes = arg.server_ip.toLocal8Bit();
  LOG_INFO << "Create Participiant with {af: " << arg.af
           << ", serverIP: " << ip_bytes.constData()
           << ", serverPort: " << arg.server_port
           << ", conferenceID: " << arg.conference_id
           << ", userID: " << arg.user_id 
           << ", heartBeatInterval: " << arg.heart_beat_interval << "}";
  
  for (auto &client : clients_)
  {
    if (arg == client.second.arg)
    {
      LOG_WARN << "Client already exist";
      emit ClientControlSignal(
        bfcpc::ControlError::kClientAlreadyExist, client.first, client_arg);
      return;
    }
  }

  if (!loop_)
  {
    loop_ = thread_.startLoop();
  }
  
  muduo::net::InetAddress server_addr(
    arg.af, ip_bytes.constData(), arg.server_port);

  int client_id = next_client_id_++;
  BaseClientPtr base_client = boost::make_shared<bfcp::BaseClient>(
    loop_, server_addr, arg.conference_id, arg.user_id, arg.heart_beat_interval);
  base_client->setStateChangedCallback(
    boost::bind(&BfcpClientService::OnClientStateChange, this, client_id, _1));
  base_client->setResponseReceivedCallback(
    boost::bind(&BfcpClientService::OnReceivedResponse, this, client_id, _1, _2, _3));

  Client client;
  client.arg = arg;
  client.base_client = base_client;
  auto res = clients_.insert(std::make_pair(client_id, client));
  assert(res.second == true);

  emit ClientControlSignal(bfcpc::ControlError::kOk, client_id, client_arg);

  base_client->connect();
}

void BfcpClientService::OnClientStateChange(int client_id, 
                                            bfcp::BaseClient::State state)
{
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitStateChangedSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, client_id), 
    Q_ARG(int, state));
}

void BfcpClientService::EmitStateChangedSignal( int client_id, int state )
{
  emit StateChangedSignal(client_id, state);
}

void BfcpClientService::OnReceivedResponse(int client_id, 
                                           bfcp::BaseClient::Error error, 
                                           bfcp_prim prim, 
                                           void *data)
{
  // NOTE: called from the work thread
  if (error == bfcp::BaseClient::kNoError)
  {
    switch (prim)
    {
    case BFCP_FLOOR_REQUEST_STATUS:
      OnReceivedFloorRequestStatus(client_id, data); break;
    case BFCP_USER_STATUS:
      OnReceivedUserStatus(client_id, data); break;
      break;
    case BFCP_FLOOR_STATUS:
      OnReceivedFloorStatus(client_id, data); break;
    case BFCP_ERROR:
      OnReceivedError(client_id, data); break;
    case BFCP_HELLO_ACK:
      break;
    default:
      LOG_ERROR << "Unexpected response: " << prim;
      break;
    }
  }
}

void BfcpClientService::OnReceivedFloorRequestStatus( int client_id, void *data )
{
  LOG_INFO << "Client[" << client_id << "] received floor request status";
  if (data)
  {
    bfcp::FloorRequestInfoParam *param = 
      static_cast<bfcp::FloorRequestInfoParam*>(data);

    bfcpc::FloorRequestInfo info;
    details::Convert(info, *param);

    QByteArray bytes;
    QDataStream data_stream(&bytes, QIODevice::WriteOnly);
    data_stream.setVersion(QDataStream::Qt_4_4);
    data_stream << info;

    QMetaObject::invokeMethod(
      this, "EmitFloorRequestInfoSignal",
      Qt::QueuedConnection,
      Q_ARG(int, client_id), 
      Q_ARG(const QByteArray &, bytes));
  }
}

void BfcpClientService::EmitFloorRequestInfoSignal(int client_id, 
                                                     const QByteArray &data)
{
  emit FloorRequestInfoReceivedSignal(client_id, data);
}

void BfcpClientService::OnReceivedUserStatus( int client_id, void *data )
{
  LOG_INFO << "Client[" << client_id << "] received user status";
  if (data)
  {
    bfcp::UserStatusParam *param = static_cast<bfcp::UserStatusParam*>(data);

    bfcpc::UserStatus user_status;
    user_status.has_beneficiary = param->hasBeneficiary;
    if (param->hasBeneficiary)
    {
      details::Convert(user_status.beneficiary, param->beneficiary);
    }
    details::Convert(user_status.frq_info_list, param->frqInfoList);

    QByteArray bytes;
    QDataStream data_stream(&bytes, QIODevice::WriteOnly);
    data_stream.setVersion(QDataStream::Qt_4_4);
    data_stream << user_status;

    QMetaObject::invokeMethod(
      this, "EmitUserStatusSignal", 
      Qt::QueuedConnection,
      Q_ARG(int, client_id), 
      Q_ARG(const QByteArray &, bytes));
  }
}

void BfcpClientService::EmitUserStatusSignal( int client_id, const QByteArray &data )
{
  emit UserStatusReceivedSignal(client_id, data);
}

void BfcpClientService::OnReceivedFloorStatus( int client_id, void *data )
{
  LOG_INFO << "Client[" << client_id << "] received floor status";
  if (data)
  {
    bfcp::FloorStatusParam *param = 
      static_cast<bfcp::FloorStatusParam*>(data);

    bfcpc::FloorStatus floor_status;
    floor_status.floor_id = param->floorID;
    floor_status.has_floor_id = param->hasFloorID;
    details::Convert(floor_status.frq_info_list, param->frqInfoList);

    QByteArray bytes;
    QDataStream data_stream(&bytes, QIODevice::WriteOnly);
    data_stream.setVersion(QDataStream::Qt_4_4);
    data_stream << floor_status;

    QMetaObject::invokeMethod(
      this, "EmitFloorStatusSignal", 
      Qt::QueuedConnection,
      Q_ARG(int, client_id), 
      Q_ARG(const QByteArray &, bytes));
  }
}

void BfcpClientService::EmitFloorStatusSignal( int client_id, const QByteArray &data )
{
  emit FloorStatusReceivedSignal(client_id, data);
}

void BfcpClientService::OnReceivedError( int client_id, void *data )
{
  LOG_INFO << "Client[" << client_id << "] received error";
  if (data)
  {
    bfcp::ErrorParam *param = static_cast<bfcp::ErrorParam*>(data);

    bfcpc::ResponseError err;
    err.error_code = static_cast<bfcpc::BfcpErrorCode>(param->errorCode.code);
    err.info = QString::fromUtf8(param->errorInfo.c_str());

    QByteArray bytes;
    QDataStream data_stream(&bytes, QIODevice::WriteOnly);
    data_stream.setVersion(QDataStream::Qt_4_4);
    data_stream << err;

    QMetaObject::invokeMethod(
      this, "EmitErrorReceivedSignal", 
      Qt::QueuedConnection,
      Q_ARG(int, client_id), 
      Q_ARG(const QByteArray &, bytes));
  }
}

void BfcpClientService::EmitErrorReceivedSignal(int client_id,
                                                const QByteArray &data)
{
  emit ErrorReceivedSignal(client_id, data);
}

void BfcpClientService::DestroyClient( int client_id )
{
  LOG_INFO << "Destroy client [" << client_id << "]";
  QByteArray bytes;
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  assert(loop_);
  BaseClientPtr base_client = (*it).second.base_client;
  
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << (*it).second.arg;

  clients_.erase(it);
  loop_->runInLoop(
    boost::bind(&BfcpClientService::ResetClient, std::move(base_client)));

  emit ClientControlSignal(
    bfcpc::ControlError::kOk, client_id, bytes);
}

void BfcpClientService::ResetClient( BaseClientPtr client )
{
  client = nullptr;
}

void BfcpClientService::SendFloorRequest(int client_id,
                                         const QByteArray &floor_request_arg)
{
  LOG_INFO << "Client [" << client_id << "] send floor request";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  QDataStream data_stream(floor_request_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::FloorRequestArg arg;
  data_stream >> arg;

  bfcp::FloorRequestParam param;
  param.floorIDs.reserve(arg.floor_ids.size());
  param.floorIDs.assign(arg.floor_ids.begin(), arg.floor_ids.end());

  param.beneficiaryID = arg.beneficiary_id;
  param.pInfo = arg.participant_info.toUtf8().constData();
  param.priority = static_cast<bfcp_priority>(arg.priority);
  param.hasBeneficiaryID = arg.use_benficirary_id;

  (*it).second.base_client->sendFloorRequest(param);
}

void BfcpClientService::SendFloorRelease( int client_id, ushort floor_request_id )
{
  LOG_INFO << "Client [" << client_id << "] send floor release";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  (*it).second.base_client->sendFloorRelease(floor_request_id);
}

void BfcpClientService::SendFloorRequestQuery( int client_id, ushort floor_request_id )
{
  LOG_INFO << "Client [" << client_id << "] send floor request query";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }
  (*it).second.base_client->sendFloorRequestQuery(floor_request_id);
}

void BfcpClientService::SendUserQuery( int client_id, const QByteArray &user_query_arg )
{
  LOG_INFO << "Client [" << client_id << "] send user query";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  QDataStream data_stream(user_query_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::UserQueryArg arg;
  data_stream >> arg;

  bfcp::UserQueryParam param;
  param.beneficiaryID = arg.beneficiary_id;
  param.hasBeneficiaryID = arg.use_beneficiary_id;

  (*it).second.base_client->sendUserQuery(param);
}

void BfcpClientService::SendFloorQuery(int client_id, const QByteArray &floor_ids)
{
  LOG_INFO << "Client [" << client_id << "] send floor query";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  QDataStream data_stream(floor_ids);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::FloorIDList arg;
  data_stream >> arg;

  bfcp_floor_id_list ids;
  ids.reserve(arg.size());
  ids.assign(arg.begin(), arg.end());

  (*it).second.base_client->sendFloorQuery(ids);
}

void BfcpClientService::SendChairAction( int client_id, const QByteArray &chair_action_arg )
{
  LOG_INFO << "Client [" << client_id << "] send chair action";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }

  QDataStream data_stream(chair_action_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::ChairActionArg arg;
  data_stream >> arg;

  bfcp::FloorRequestInfoParam param;
  param.floorRequestID = arg.floor_request_id;

  param.valueType |= AttrValueType::kHasOverallRequestStatus;
  param.oRS.floorRequestID = arg.floor_request_id;
  param.oRS.requestStatus.status = 
    static_cast<bfcp_reqstat>(arg.status.request_status);
  param.oRS.requestStatus.qpos = arg.status.queue_pos;
  param.oRS.statusInfo = arg.status_info.toUtf8().constData();
  param.oRS.hasRequestStatus = true;

  bfcp::FloorRequestStatusParam floor_param;
  for (auto floor_id : arg.floor_ids)
  {
    floor_param.floorID = floor_id;
    param.fRS.push_back(floor_param);
  }

  (*it).second.base_client->sendChairAction(param);
}

void BfcpClientService::SendHello( int client_id )
{
  LOG_INFO << "Client [" << client_id << "] send hello";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }
  if ((*it).second.base_client->getState() == bfcp::BaseClient::kConnected)
  {
    (*it).second.base_client->sendHello();
  }
  else
  {
    // NOTE: when connected, base client will send hello message
    (*it).second.base_client->connect();  
  }
}

void BfcpClientService::SendGoodbye( int client_id )
{
  LOG_INFO << "Client [" << client_id << "] send goodbye";
  auto it = clients_.find(client_id);
  if (it == clients_.end())
  {
    QByteArray bytes;
    LOG_WARN << "Client [" << client_id <<"] not exist";
    emit ClientControlSignal(
      bfcpc::ControlError::kClientNotExist, client_id, bytes);
    return;
  }
  (*it).second.base_client->sendGoodbye();
}
