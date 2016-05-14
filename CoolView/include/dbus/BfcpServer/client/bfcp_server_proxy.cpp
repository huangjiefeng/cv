#include "bfcp_server_proxy.h"

namespace bfcps
{

class Register
{
public:
  Register()
  {
    qRegisterMetaType<bfcps::ErrorCode>("bfcps::ErrorCode");
    qRegisterMetaType<bfcps::AcceptPolicy>("bfcps::AcceptPolicy");
    qRegisterMetaType<bfcps::ConferenceIDList>("bfcps::ConferenceIDList");
    qRegisterMetaType<bfcps::ServerArg>("bfcps::ServerArg");
    qRegisterMetaType<bfcps::ConferenceArg>("bfcps::ConferenceArg");
    qRegisterMetaType<bfcps::FloorArg>("bfcps::FloorArg");
    qRegisterMetaType<bfcps::UserArg>("bfcps::UserArg");
  }
};

} // bfcps

BfcpServerProxy::BfcpServerProxy(QObject *parent)
    : QObject(parent)
{
  static bfcps::Register meta_type_register;

  server_proxy_ = new BfcpServerIf(
    BFCP_SERVER_SERVICE_NAME, 
    BFCP_SERVER_SERVICE_OBJECT_PATH, 
    QDBusConnection::sessionBus());

  Connect();
}

BfcpServerProxy::~BfcpServerProxy()
{
  delete server_proxy_;
}

void BfcpServerProxy::Connect()
{
  connect(
    server_proxy_, &BfcpServerIf::ServerControlSignal,
    this, &BfcpServerProxy::HandleServerControlResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::ConferenceControlSignal,
    this, &BfcpServerProxy::HandleConferenceControlResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::FloorControlSignal,
    this, &BfcpServerProxy::HandleFloorControlResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::UserControlSignal,
    this, &BfcpServerProxy::HandleUserControlResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::ChairControlSignal,
    this, &BfcpServerProxy::HandleChairControlResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::ConferenceIDsSignal,
    this, &BfcpServerProxy::HandleConferenceIDsResultSlot);

  connect(
    server_proxy_, &BfcpServerIf::ConferenceInfoSignal,
    this, &BfcpServerProxy::HandleConferenceInfoResultSlot);
}

void BfcpServerProxy::Start( const bfcps::ServerArg &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->Start(bytes);
}

void BfcpServerProxy::Stop()
{
  server_proxy_->Stop();
}

void BfcpServerProxy::Quit()
{
  server_proxy_->Quit();
}

void BfcpServerProxy::HandleServerControlResultSlot( int control_error )
{
  qDebug() << "received server control result: " << control_error;
  emit NotifyServerControlResultSignal(
    static_cast<bfcps::ErrorCode>(control_error));
}

void BfcpServerProxy::AddConference(uint32_t conference_id, 
                                         const bfcps::ConferenceArg &arg)
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->AddConference(conference_id, bytes);
}

void BfcpServerProxy::RemoveConference( uint32_t conference_id )
{
  server_proxy_->RemoveConference(conference_id);
}

void BfcpServerProxy::ModifyConference(uint32_t conference_id, 
                                            const bfcps::ConferenceArg &arg)
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->ModifyConference(conference_id, bytes);
}

void BfcpServerProxy::HandleConferenceControlResultSlot(
    int control_error, uint conference_id)
{
  qDebug() << "received conference control result: " << control_error;
  emit NotifyConferenceControlResultSignal(
    static_cast<bfcps::ErrorCode>(control_error), conference_id);
}

void BfcpServerProxy::GetConferenceIDs()
{
  server_proxy_->GetConferenceIDs();
}

void BfcpServerProxy::HandleConferenceIDsResultSlot(
  int control_error, const QByteArray &conference_ids)
{
  qDebug() << "received conference ids result: " << control_error;
  QByteArray bytes;
  QDataStream data_stream(conference_ids);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcps::ConferenceIDList ids;
  data_stream >> ids;
  qDebug() << "conference ids count: " << ids.size();
  emit NotifyConferenceIDsSignal(
    static_cast<bfcps::ErrorCode>(control_error),
    ids);
}

void BfcpServerProxy::GetConferenceInfo(uint32_t conference_id)
{
  server_proxy_->GetConferenceInfo(conference_id);
}

void BfcpServerProxy::HandleConferenceInfoResultSlot(
  int control_error, uint conference_id, const QString &conference_info)
{
  qDebug() << "received conference info result: " << control_error;
  qDebug() << "conference id: " << conference_id;
  qDebug() << "conference info: " << conference_info.toLocal8Bit().constData();
  emit NotifyConferenceInfoSignal(
    static_cast<bfcps::ErrorCode>(control_error),
    conference_id,
    conference_info);
}

void BfcpServerProxy::AddFloor(uint32_t conference_id, 
                               uint16_t floor_id, 
                               const bfcps::FloorArg &arg)
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->AddFloor(conference_id, floor_id, bytes);
}

void BfcpServerProxy::RemoveFloor(uint32_t conference_id, uint16_t floor_id)
{
  server_proxy_->RemoveFloor(conference_id, floor_id);
}

void BfcpServerProxy::ModifyFloor(uint32_t conference_id, 
                                  uint16_t floor_id, 
                                  const bfcps::FloorArg &arg)
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->RemoveFloor(conference_id, floor_id);
}

void BfcpServerProxy::HandleFloorControlResultSlot(
  int control_error, uint32_t conference_id, uint16_t floor_id)
{
  qDebug() << "received floor control result: " << control_error;
  emit NotifyFloorControlResultSignal(
    static_cast<bfcps::ErrorCode>(control_error),
    conference_id, floor_id);
}

void BfcpServerProxy::AddUser(uint32_t conference_id, 
                              uint16_t user_id, 
                              const bfcps::UserArg &arg)
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  server_proxy_->AddUser(conference_id, user_id, bytes);
}

void BfcpServerProxy::RemoveUser( uint32_t conference_id, uint16_t user_id )
{
  server_proxy_->RemoveUser(conference_id, user_id);
}

void BfcpServerProxy::HandleUserControlResultSlot(
  int control_error, uint32_t conference_id, uint16_t user_id)
{
  qDebug() << "received user control result: " << control_error;
  emit NotifyUserControlResultSignal(
    static_cast<bfcps::ErrorCode>(control_error),
    conference_id, user_id);
}

void BfcpServerProxy::SetChair(
    uint32_t conference_id, uint16_t floor_id, uint16_t user_id)
{
  server_proxy_->SetChair(conference_id, floor_id, user_id);
}

void BfcpServerProxy::RemoveChair(uint32_t conference_id, uint16_t floor_id)
{
  server_proxy_->RemoveChair(conference_id, floor_id);
}

void BfcpServerProxy::HandleChairControlResultSlot(int control_error, 
                                                   uint32_t conference_id, 
                                                   uint16_t floor_id, 
                                                   uint16_t user_id)
{
  qDebug() << "received chair control result: " << control_error;
  emit NotifyChairControlResultSignal(
    static_cast<bfcps::ErrorCode>(control_error),
    conference_id, floor_id, user_id);
}
