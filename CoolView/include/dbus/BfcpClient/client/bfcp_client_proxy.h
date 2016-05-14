#ifndef BFCP_CLIENT_PROXY_H
#define BFCP_CLIENT_PROXY_H

#include <QObject>

#include <dbus/BfcpClient/common/BfcpClientServiceCommon.h>
#include <dbus/BfcpClient/client/BfcpClientIf.h>

class BfcpClientProxy : public QObject
{
  Q_OBJECT

public:
  BfcpClientProxy(QObject *parent);
  ~BfcpClientProxy();

public Q_SLOTS:
  void CreateClient(const bfcpc::ClientArg &arg);
  void DestroyClient(int client_id);
  void SendFloorRequest(int client_id, const bfcpc::FloorRequestArg &arg);
  void SendFloorRelease(int client_id, ushort floor_request_id);
  void SendFloorRequestQuery(int client_id, ushort floor_request_id);
  void SendUserQuery(int client_id, const bfcpc::UserQueryArg &arg);
  void SendFloorQuery(int client_id, const bfcpc::FloorIDList &arg);
  void SendChairAction(int client_id, const bfcpc::ChairActionArg &arg);
  void SendHello(int client_id);
  void SendGoodbye(int client_id);

Q_SIGNALS:
  void NotifyClientControlSuccessedSignal(int client_id, bfcpc::ClientArg &arg);
  void NotifyClientAlreadyExistSignal(int client_id, bfcpc::ClientArg &arg);
  void NotifyClientNotExistSignal(int client_id);

  void NotifyStateChangeSignal(
    int client_id, 
    bfcpc::ClientState state);

  void NotifyErrorReceivedSignal(
    int client_id,
    const bfcpc::ResponseError &error);

  void NotifyFloorRequestInfoSignal(
    int client_id,
    const bfcpc::FloorRequestInfo &info);

  void NotifyUserStatusSignal(
    int client_id,
    const bfcpc::UserStatus &status);

  void NotifyFloorStatusSignal(
    int client_id,
    const bfcpc::FloorStatus &status);

private Q_SLOTS:
  void HandleClientControlResultSlot(int control_error, int client_id, const QByteArray &data);
  void HandleStateChangedSlot(int client_id, int state);
  void HandleErrorReceivedResultSlot(int client_id, const QByteArray &data);
  void HandleFloorRequestInfoSlot(int client_id, const QByteArray &data);
  void HandleUserStatusReceivedSlot(int client_id, const QByteArray &data);
  void HandleFloorStatusReceivedSlot(int client_id, const QByteArray &data);

private:
  void Connect();

  BfcpClientIf *client_proxy_;
};

#endif // BFCP_CLIENT_PROXY_H
