#include "BfcpServerService.h"

#include <boost/make_shared.hpp>
#include <muduo/base/Logging.h>
#include <dbus/BfcpServer/service/BfcpServerAdaptor.h>

using namespace bfcps;

namespace details
{

bfcp::AcceptPolicy convertTo(AcceptPolicy policy)
{
  switch (policy)
  {
  case AcceptPolicy::kAutoAccept:
    return bfcp::AcceptPolicy::kAutoAccept; break;
  case AcceptPolicy::kAutoDeny:
    return bfcp::AcceptPolicy::kAutoDeny; break;
  default:
    LOG_ERROR << "Unknown policy: " << static_cast<int>(policy)
              << " set as auto deny policy";
    return bfcp::AcceptPolicy::kAutoDeny; break;
  }
}

ErrorCode convertTo(bfcp::ControlError error)
{
  switch (error)
  {
  case bfcp::ControlError::kNoError:
    return ErrorCode::kNoError;
    break;
  case bfcp::ControlError::kUserNotExist:
    return ErrorCode::kUserNotExist;
    break;
  case bfcp::ControlError::kUserAlreadyExist:
    return ErrorCode::kUserAlreadyExist;
    break;
  case bfcp::ControlError::kFloorNotExist:
    return ErrorCode::kFloorNotExist;
    break;
  case bfcp::ControlError::kFloorAlreadyExist:
    return ErrorCode::kFloorAlreadyExist;
    break;
  case bfcp::ControlError::kChairNotExist:
    return ErrorCode::kChairNotExost;
    break;
  case bfcp::ControlError::kChairAlreadyExist:
    return ErrorCode::kChairAlreadyExist;
    break;
  case bfcp::ControlError::kConferenceNotExist:
    return ErrorCode::kConferenceNotExist;
    break;
  case bfcp::ControlError::kConferenceAlreadyExist:
    return ErrorCode::kConferenceAlreadyExist;
    break;
  default:
    // FIXME: unknow error
    assert(false);
    return ErrorCode::kNoError;
    break;
  }
}


} // details

BfcpServerService::BfcpServerService(QObject *parent)
    : QObject(parent), loop_(nullptr)
{
  LOG_TRACE << "BfcpServerService::BfcpServerService constructing";
  new BfcpServerAdaptor(this);
}

BfcpServerService::~BfcpServerService()
{
  LOG_TRACE << "BfcpServerService::~BfcpServerService destructing";
  if (server_)
  {
    server_->stop();
    // NOTE: server_ should destructor in the loop thread
    loop_->runInLoop(boost::bind(&BfcpServerService::ResetServer, std::move(server_)));
    server_ = nullptr;
  }
}

void BfcpServerService::Initialize()
{
  // dbus
  QDBusConnection connection = QDBusConnection::sessionBus();
  bool ret = connection.registerService(BFCP_SERVER_SERVICE_NAME);
  if (!ret)
  {
    LOG_ERROR << "Cannot register service";
    return;
  }

  ret = connection.registerObject(BFCP_SERVER_SERVICE_OBJECT_PATH, this);
  if (!ret)
  {
    LOG_ERROR << "Cannot register object";
    return;
  }
  LOG_INFO << "Register service success!";
}

void BfcpServerService::Start( const QByteArray &server_arg )
{
  QDataStream data_stream(server_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::ServerArg arg;
  data_stream >> arg;

  LOG_INFO << "Start with {af: " << arg.af
           << ", port: " << arg.port 
           << ", enableConnectionThread: " << (arg.enable_connection_thread ? "true" : "false")
           << ", workThreadNum: " << arg.work_thread_num
           << ", userObsoletedTime: "<< arg.user_obsoleted_time << "}";
  
  if (server_)
  {
    LOG_WARN << "Server already start";
    emit ServerControlSignal(ErrorCode::kServerAlreadyStart);
    return;
  }
  if (!loop_)
  {
    loop_ = thread_.startLoop();
  }
  muduo::net::InetAddress listenAddr(arg.af, arg.port);
  server_ = boost::make_shared<bfcp::BaseServer>(loop_, listenAddr);
  if (arg.enable_connection_thread)
  {
    server_->enableConnectionThread();
  }
  server_->setWorkerThreadNum(arg.work_thread_num);
  server_->setUserObsoleteTime(arg.user_obsoleted_time);
  server_->start();
  emit ServerControlSignal(ErrorCode::kNoError);
}

void BfcpServerService::Stop()
{
  LOG_INFO << "Stop";
  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit ServerControlSignal(ErrorCode::kServerNotStart);
    return;
  }
  assert(loop_);
  server_->stop();
  // NOTE: server_ should destruct in the loop thread
  loop_->runInLoop(boost::bind(&BfcpServerService::ResetServer, std::move(server_)));
  server_ = nullptr;

  emit ServerControlSignal(ErrorCode::kNoError);
}

void BfcpServerService::ResetServer( BaseServerPtr server )
{
  server = nullptr;
}

void BfcpServerService::Quit()
{
  QCoreApplication::instance()->quit();
}

void BfcpServerService::AddConference( uint conference_id, const QByteArray &conference_arg )
{
  QDataStream data_stream(conference_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::ConferenceArg arg;
  data_stream >> arg;
  bfcp::AcceptPolicy policy = details::convertTo(arg.policy);
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", maxFloorRequest: " << arg.max_floor_request
            << ", policy: " << bfcp::toString(policy)
            << ", timeforChairAction: " << arg.time_for_chair_action << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit ConferenceControlSignal(
      ErrorCode::kServerNotStart,
      conference_id);
    return;
  }
  bfcp::ConferenceConfig config;
  config.maxFloorRequest = arg.max_floor_request;
  config.acceptPolicy = policy;
  config.timeForChairAction = arg.time_for_chair_action;

  server_->addConference(
    conference_id, 
    config,
    boost::bind(&BfcpServerService::HandleConferenceControlResult, 
      this, conference_id, _1)
  );
}

void BfcpServerService::RemoveConference( uint conference_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id << "}";
  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit ConferenceControlSignal(
      ErrorCode::kServerNotStart,
      conference_id);
    return;
  }

  server_->removeConference(
    conference_id, 
    boost::bind(&BfcpServerService::HandleConferenceControlResult, 
      this, conference_id, _1));
}

void BfcpServerService::ModifyConference( uint conference_id, const QByteArray &conference_arg )
{
  QDataStream data_stream(conference_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::ConferenceArg arg;
  data_stream >> arg;
  bfcp::AcceptPolicy policy = details::convertTo(arg.policy);
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", maxFloorRequest: " << arg.max_floor_request
            << ", policy: " << bfcp::toString(policy)
            << ", timeforChairAction: " << arg.time_for_chair_action << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit ConferenceControlSignal(
      ErrorCode::kServerNotStart,
      conference_id);
    return;
  }

  bfcp::ConferenceConfig config;
  config.maxFloorRequest = arg.max_floor_request;
  config.acceptPolicy = policy;
  config.timeForChairAction = arg.time_for_chair_action;
  server_->modifyConference(
    conference_id, 
    config,
    boost::bind(&BfcpServerService::HandleConferenceControlResult, 
      this, conference_id, _1));
}

void BfcpServerService::HandleConferenceControlResult(uint conference_id, 
                                                bfcp::ControlError error)
{
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitConferenceControlSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(uint, conference_id));
}

void BfcpServerService::EmitConferenceControlSignal( int control_error, uint conference_id )
{
  emit ConferenceControlSignal(control_error, conference_id);
}

void BfcpServerService::AddFloor( uint conference_id, ushort floor_id, const QByteArray &floor_arg )
{
  QDataStream data_stream(floor_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::FloorArg arg;
  data_stream >> arg;

  LOG_TRACE << "{conferenceID: " << conference_id
            << ", floorID: " << floor_id
            << ", maxGrantedNum: " << arg.max_granted_num 
            << ", maxHoldingTime: " << arg.max_holding_time << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit FloorControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      floor_id);
    return;
  }

  bfcp::FloorConfig config;
  config.maxGrantedNum = arg.max_granted_num;
  config.maxHoldingTime = arg.max_holding_time;

  server_->addFloor(
    conference_id,
    floor_id, 
    config,
    boost::bind(&BfcpServerService::HandleFloorControlResult, 
      this, conference_id, floor_id, _1)
  );
}

void BfcpServerService::ModifyFloor( uint conference_id, ushort floor_id, const QByteArray &floor_arg )
{
  QDataStream data_stream(floor_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::FloorArg arg;
  data_stream >> arg;

  LOG_TRACE << "{conferenceID: " << conference_id
            << ", floorID: " << floor_id
            << ", maxGrantedNum: " << arg.max_granted_num 
            << ", maxHoldingTime: " << arg.max_holding_time << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit FloorControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      floor_id);
    return;
  }

  bfcp::FloorConfig config;
  config.maxGrantedNum = arg.max_granted_num;

  server_->modifyFloor(
    conference_id,
    floor_id, 
    config,
    boost::bind(&BfcpServerService::HandleFloorControlResult, 
      this, conference_id, floor_id, _1)
  );
}

void BfcpServerService::RemoveFloor( uint conference_id, ushort floor_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", floorID: " << floor_id << "}";
  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit FloorControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      floor_id);
    return;
  }

  server_->removeFloor(
    conference_id,
    floor_id, 
    boost::bind(&BfcpServerService::HandleFloorControlResult, 
      this, conference_id, floor_id, _1)
  );
}

void BfcpServerService::HandleFloorControlResult(
  uint conference_id, ushort floor_id, bfcp::ControlError error)
{
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitFloorControlSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(uint, conference_id), 
    Q_ARG(ushort, floor_id));
}

void BfcpServerService::EmitFloorControlSignal( int control_error, uint conference_id, ushort floor_id )
{
  emit FloorControlSignal(control_error, conference_id, floor_id);
}

void BfcpServerService::AddUser( uint conference_id, ushort user_id, const QByteArray &user_arg )
{
  QDataStream data_stream(user_arg);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::UserArg arg;
  data_stream >> arg;

  LOG_TRACE << "{conferenceID: " << conference_id
            << ", userID: " << user_id
            << ", userName: " << arg.user_name.toLocal8Bit().constData()
            << ", userURI: " << arg.user_uri.toLocal8Bit().constData() << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit UserControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      user_id);
    return;
  }

  bfcp::UserInfoParam user;
  user.id = user_id;
  user.username = arg.user_name.toUtf8().constData();
  user.useruri = arg.user_uri.toUtf8().constData();

  server_->addUser(
    conference_id, 
    user,
    boost::bind(&BfcpServerService::HandleUserControlResult, 
      this, conference_id, user_id, _1)
  );
}

void BfcpServerService::RemoveUser( uint conference_id, ushort user_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", userID: " << user_id << "}";
  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit UserControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      user_id);
    return;
  }

  server_->removeUser(
    conference_id, 
    user_id,
    boost::bind(&BfcpServerService::HandleUserControlResult, 
    this, conference_id, user_id, _1)
  );
}

void BfcpServerService::HandleUserControlResult( uint conference_id, ushort user_id, bfcp::ControlError error )
{
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitUserControlSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(uint, conference_id), 
    Q_ARG(ushort, user_id));
}

void BfcpServerService::EmitUserControlSignal( int control_error, uint conference_id, ushort user_id )
{
  emit UserControlSignal(control_error, conference_id, user_id);
}

void BfcpServerService::SetChair( uint conference_id, ushort floor_id, ushort user_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", floorID: " << floor_id
            << ", userID: " << user_id << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";

    emit ChairControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      floor_id,
      user_id);

    return;
  }

  server_->setChair(
    conference_id, 
    floor_id,
    user_id,
    boost::bind(&BfcpServerService::HandleChairControlResult, 
      this, conference_id, floor_id, user_id, _1)
  );
}

void BfcpServerService::RemoveChair( uint conference_id, ushort floor_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id
            << ", floorID: " << floor_id << "}";

  if (!server_)
  {
    LOG_WARN << "Server not start";
    emit ChairControlSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      floor_id,
      0);
    return;
  }

  server_->removeChair(
    conference_id, 
    floor_id,
    boost::bind(&BfcpServerService::HandleChairControlResult, 
      this, conference_id, floor_id, 0, _1)); // UserId: 0
}

void BfcpServerService::HandleChairControlResult(uint conference_id, 
                                           ushort floor_id, 
                                           ushort user_id, 
                                           bfcp::ControlError error)
{
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitChairControlSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(uint, conference_id), 
    Q_ARG(ushort, floor_id),
    Q_ARG(ushort, user_id));
}

void BfcpServerService::EmitChairControlSignal(int control_error, 
                                         uint conference_id, 
                                         ushort floor_id, 
                                         ushort user_id)
{
  emit ChairControlSignal(control_error, conference_id, floor_id, user_id);
}

void BfcpServerService::GetConferenceIDs()
{
  LOG_TRACE << "";
  if (!server_)
  {
    QByteArray bytes;
    QDataStream data_stream(&bytes, QIODevice::WriteOnly);
    data_stream.setVersion(QDataStream::Qt_4_4);
    bfcps::ConferenceIDList conferenceIDs;

    LOG_WARN << "Server not start";  
    data_stream << conferenceIDs;

    emit ConferenceIDsSignal(
      ErrorCode::kServerNotStart,
      bytes);

    return;
  }
  
  server_->getConferenceIDs(boost::bind(
    &BfcpServerService::HandleGetCoferenceIDsResult, 
    this, _1, _2));
}

void BfcpServerService::HandleGetCoferenceIDsResult(bfcp::ControlError error, void *data)
{
  // FIXME: move serialization to emit function?
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::ConferenceIDList conferenceIDs;
  if (data)
  {
    bfcp::BaseServer::ConferenceIDList *res = 
      static_cast<bfcp::BaseServer::ConferenceIDList*>(data);
    conferenceIDs.reserve(res->size());
    for (auto id : *res)
    {
      conferenceIDs.push_back(id);
    }
  }
  data_stream << conferenceIDs;
  
  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitConferenceIDsSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(const QByteArray &, bytes));
}

void BfcpServerService::EmitConferenceIDsSignal(
  int control_error, const QByteArray &conference_ids)
{
  emit ConferenceIDsSignal(control_error, conference_ids);
}

void BfcpServerService::GetConferenceInfo( uint conference_id )
{
  LOG_TRACE << "{conferenceID: " << conference_id  << "}";
  if (!server_)
  {
    LOG_WARN << "Server not start";
    QString conference_info;

    emit ConferenceInfoSignal(
      ErrorCode::kServerNotStart,
      conference_id,
      conference_info);

    return;
  }

  server_->getConferenceInfo(
    conference_id, 
    boost::bind(&BfcpServerService::HandleGetConferenceInfoResult, 
    this, conference_id, _1, _2));
}

void BfcpServerService::HandleGetConferenceInfoResult(
  uint conference_id, bfcp::ControlError error, void *data)
{
  // FIXME: move serialization to emit function?
  QString info;
  
  if (data)
  {
    bfcp::string *res = static_cast<bfcp::string*>(data);
    info = QString::fromUtf8(res->c_str());
  }

  // NOTE: called from the work thread
  QMetaObject::invokeMethod(
    this, "EmitConferenceInfoSignal", 
    Qt::QueuedConnection,
    Q_ARG(int, details::convertTo(error)), 
    Q_ARG(uint, conference_id),
    Q_ARG(const QString &, info));
}

void BfcpServerService::EmitConferenceInfoSignal(
  int control_error, uint conference_id, const QString &conference_info)
{
  emit ConferenceInfoSignal(control_error, conference_id, conference_info);
}








