#include "bfcp_client_proxy.h"


namespace bfcpc
{

class Register
{
public:
  Register()
  {
    qRegisterMetaType<bfcpc::ClientState>("bfcpc::ClientState");
    qRegisterMetaType<bfcpc::ControlError>("bfcpc::ControlError");
    qRegisterMetaType<bfcpc::ResponseError>("bfcpc::ResponseError");
    qRegisterMetaType<bfcpc::FloorRequestStatus>("bfcpc::FloorRequestStatus");
    qRegisterMetaType<bfcpc::FloorStatus>("bfcpc::FloorStatus");
    qRegisterMetaType<bfcpc::UserStatus>("bfcpc::UserStatus");
    qRegisterMetaType<bfcpc::RequestStatus>("bfcpc::RequestStatus");
  }
};

} // namespace bfcpc

BfcpClientProxy::BfcpClientProxy(QObject *parent)
    : QObject(parent)
{
  static bfcpc::Register meta_type_register;

  client_proxy_ = new BfcpClientIf(
    BFCP_CLIENT_SERVICE_NAME,
    BFCP_CLIENT_SERVICE_OBJECT_PATH,
    QDBusConnection::sessionBus());

  Connect();
}

BfcpClientProxy::~BfcpClientProxy()
{
  delete client_proxy_;
}

void BfcpClientProxy::Connect()
{
  connect(
    client_proxy_, &BfcpClientIf::ClientControlSignal,
    this, &BfcpClientProxy::HandleClientControlResultSlot);

  connect(
    client_proxy_, &BfcpClientIf::ErrorReceivedSignal,
    this, &BfcpClientProxy::HandleErrorReceivedResultSlot);

  connect(
    client_proxy_, &BfcpClientIf::FloorRequestInfoReceivedSignal,
    this, &BfcpClientProxy::HandleFloorRequestInfoSlot);

  connect(
    client_proxy_, &BfcpClientIf::UserStatusReceivedSignal,
    this, &BfcpClientProxy::HandleUserStatusReceivedSlot);

  connect(
    client_proxy_, &BfcpClientIf::FloorStatusReceivedSignal,
    this, &BfcpClientProxy::HandleFloorStatusReceivedSlot);

  connect(
    client_proxy_, &BfcpClientIf::StateChangedSignal,
    this, &BfcpClientProxy::HandleStateChangedSlot);
}

void BfcpClientProxy::CreateClient( const bfcpc::ClientArg &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  client_proxy_->CreateClient(bytes);
}

void BfcpClientProxy::DestroyClient( int client_id )
{
  client_proxy_->DestroyClient(client_id);
}

void BfcpClientProxy::SendFloorRequest( int client_id, const bfcpc::FloorRequestArg &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  client_proxy_->SendFloorRequest(client_id, bytes);
}

void BfcpClientProxy::SendFloorRelease( int client_id, ushort floor_request_id )
{
  client_proxy_->SendFloorRelease(client_id, floor_request_id);
}

void BfcpClientProxy::SendFloorRequestQuery( int client_id, ushort floor_request_id )
{
  client_proxy_->SendFloorRequestQuery(client_id, floor_request_id);
}

void BfcpClientProxy::SendUserQuery( int client_id, const bfcpc::UserQueryArg &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  client_proxy_->SendUserQuery(client_id, bytes);
}

void BfcpClientProxy::SendFloorQuery( int client_id, const bfcpc::FloorIDList &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  client_proxy_->SendFloorQuery(client_id, bytes);
}

void BfcpClientProxy::SendChairAction( int client_id, const bfcpc::ChairActionArg &arg )
{
  QByteArray bytes;
  QDataStream data_stream(&bytes, QIODevice::WriteOnly);
  data_stream.setVersion(QDataStream::Qt_4_4);
  data_stream << arg;
  client_proxy_->SendChairAction(client_id, bytes);
}

void BfcpClientProxy::SendHello( int client_id )
{
  client_proxy_->SendHello(client_id);
}

void BfcpClientProxy::SendGoodbye( int client_id )
{
  client_proxy_->SendGoodBye(client_id);
}

void BfcpClientProxy::HandleClientControlResultSlot(int control_error, 
                                                    int client_id, 
                                                    const QByteArray &data)
{
  qDebug() << "received client control: " << control_error;
  bfcpc::ControlError err = static_cast<bfcpc::ControlError>(control_error);
  if (err == bfcpc::ControlError::kClientNotExist)
  {
    emit NotifyClientNotExistSignal(client_id);
  }
  else
  {
    QByteArray bytes;
    QDataStream data_stream(data);
    data_stream.setVersion(QDataStream::Qt_4_4);
    bfcpc::ClientArg arg;
    data_stream >> arg;
    if (err == bfcpc::ControlError::kClientAlreadyExist)
    {
      emit NotifyClientAlreadyExistSignal(client_id, arg);
    }
    else if (err == bfcpc::ControlError::kOk)
    {
      emit NotifyClientControlSuccessedSignal(client_id, arg);
    }
  }
}

void BfcpClientProxy::HandleStateChangedSlot( int client_id, int state )
{
  qDebug() << "received state: " << state;
  emit NotifyStateChangeSignal(
    client_id, 
    static_cast<bfcpc::ClientState>(state));
}

void BfcpClientProxy::HandleErrorReceivedResultSlot( int client_id, const QByteArray &data )
{
  qDebug() << "received error";
  QByteArray bytes;
  QDataStream data_stream(data);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::ResponseError err;
  data_stream >> err;
  emit NotifyErrorReceivedSignal(client_id, err);
}

void BfcpClientProxy::HandleFloorRequestInfoSlot( int client_id, const QByteArray &data )
{
  qDebug() << "received floor request status";
  QByteArray bytes;
  QDataStream data_stream(data);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::FloorRequestInfo info;
  data_stream >> info;
  emit NotifyFloorRequestInfoSignal(client_id, info);
}

void BfcpClientProxy::HandleUserStatusReceivedSlot( int client_id, const QByteArray &data )
{
  qDebug() << "received user status";
  QByteArray bytes;
  QDataStream data_stream(data);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::UserStatus status;
  data_stream >> status;
  emit NotifyUserStatusSignal(client_id, status);
}

void BfcpClientProxy::HandleFloorStatusReceivedSlot( int client_id, const QByteArray &data )
{
  qDebug() << "received floor status";
  QByteArray bytes;
  QDataStream data_stream(data);
  data_stream.setVersion(QDataStream::Qt_4_4);
  bfcpc::FloorStatus status;
  data_stream >> status;
  emit NotifyFloorStatusSignal(client_id, status);
}






