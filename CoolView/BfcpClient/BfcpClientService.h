#ifndef BFCPCLIENTSERVICE_H
#define BFCPCLIENTSERVICE_H

#include <map>

#include <QObject>
// NOTE: QDBusConnection should included first
#include <QtDBus/QDBusConnection>

#include <boost/shared_ptr.hpp>
#include <muduo/net/EventLoopThread.h>
#include <bfcp/client/base_client.h>

#include <dbus/BfcpClient/common/BfcpClientServiceCommon.h>

class BfcpClientService : public QObject
{
  Q_OBJECT

public:
  BfcpClientService(QObject *parent);
  ~BfcpClientService();

  void Initialize();

public Q_SLOTS: // METHODS
  void CreateClient(const QByteArray &client_arg);
  void DestroyClient(int client_id);
  void SendFloorRequest(int client_id, const QByteArray &floor_request_arg);
  void SendFloorRelease(int client_id, ushort floor_request_id);
  void SendFloorRequestQuery(int client_id, ushort floor_request_id);
  void SendUserQuery(int client_id, const QByteArray &user_query_arg);
  void SendFloorQuery(int client_id, const QByteArray &floor_ids);
  void SendChairAction(int client_id, const QByteArray &chair_action_arg);
  void SendHello(int client_id);
  void SendGoodbye(int client_id);

  void EmitStateChangedSignal(int client_id, int state);
  void EmitErrorReceivedSignal(int client_id, const QByteArray &data);
  void EmitFloorRequestInfoSignal(int client_id, const QByteArray &data);
  void EmitUserStatusSignal(int client_id, const QByteArray &data);
  void EmitFloorStatusSignal(int client_id, const QByteArray &data);
  

Q_SIGNALS: // SIGNALS
  void ClientControlSignal(int control_error, int client_id, const QByteArray &client_arg);
  void StateChangedSignal(int client_id, int state);
  void ErrorReceivedSignal(int client_id, const QByteArray &data);
  void FloorRequestInfoReceivedSignal(int client_id, const QByteArray &data);
  void UserStatusReceivedSignal(int client_id, const QByteArray &data);
  void FloorStatusReceivedSignal(int client_id, const QByteArray &data);
  

private:
  typedef boost::shared_ptr<bfcp::BaseClient> BaseClientPtr;
  struct Client
  {
    bfcpc::ClientArg arg;
    BaseClientPtr base_client;
  };
  typedef std::map<int, Client> ClientMap;
  
  static void ResetClient(BaseClientPtr client);

  void OnClientStateChange(int client_id, bfcp::BaseClient::State state);
  void OnReceivedResponse(
    int client_id, 
    bfcp::BaseClient::Error error, 
    bfcp_prim prim, 
    void *data);
  void OnReceivedError(int client_id, void *data);
  void OnReceivedFloorRequestStatus(int client_id, void *data);
  void OnReceivedUserStatus(int client_id, void *data);
  void OnReceivedFloorStatus(int client_id, void *data);

private:
  int next_client_id_;
  ClientMap clients_;
  muduo::net::EventLoop *loop_;
  muduo::net::EventLoopThread thread_;
};

#endif // BFCPCLIENTSERVICE_H
