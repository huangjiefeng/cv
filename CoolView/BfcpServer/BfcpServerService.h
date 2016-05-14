#ifndef BFCPSERVERSERVICE_H
#define BFCPSERVERSERVICE_H

#include <QObject>
// NOTE: QDBusConnection should included first
#include <QtDBus/QDBusConnection>

#include <boost/shared_ptr.hpp>
#include <muduo/net/EventLoopThread.h>
#include <bfcp/server/base_server.h>

#include <dbus/BfcpServer/common/BfcpServerServiceCommon.h>

class BfcpServerService : public QObject
{
  Q_OBJECT

public:
  BfcpServerService(QObject *parent);
  ~BfcpServerService();

  void Initialize();

public Q_SLOTS: // METHODS
  void Start(const QByteArray &server_arg);
  void Stop();
  void Quit();

  void GetConferenceIDs();
  void GetConferenceInfo(uint conference_id);

  void SetChair(uint conference_id, ushort floor_id, ushort user_id);
  void AddConference(uint conference_id, const QByteArray &conference_arg);
  void AddFloor(uint conference_id, ushort floor_id, const QByteArray &floor_arg);
  void AddUser(uint conference_id, ushort user_id, const QByteArray &user_arg);
  
  void ModifyConference(uint conference_id, const QByteArray &conference_arg);
  void ModifyFloor(uint conference_id, ushort floor_id, const QByteArray &floor_arg);
  
  void RemoveChair(uint conference_id, ushort floor_id);
  void RemoveConference(uint conference_id);
  void RemoveFloor(uint conference_id, ushort floor_id);
  void RemoveUser(uint conference_id, ushort user_id);

  void EmitConferenceControlSignal(int control_error, uint conference_id);
  void EmitFloorControlSignal(int control_error, uint conference_id, ushort floor_id);
  void EmitUserControlSignal(int control_error, uint conference_id, ushort user_id);
  void EmitChairControlSignal(int control_error, uint conference_id, ushort floor_id, ushort user_id);
  void EmitConferenceIDsSignal(int control_error, const QByteArray &conference_ids);
  void EmitConferenceInfoSignal(int control_error, uint conference_id, const QString &conference_info);

Q_SIGNALS: // SIGNALS
  void ChairControlSignal(int control_error, uint conference_id, ushort floor_id, ushort user_id);
  void ConferenceControlSignal(int control_error, uint conference_id);
  void ConferenceIDsSignal(int control_error, const QByteArray &conference_ids);
  void ConferenceInfoSignal(int control_error, uint conference_id, const QString &conference_info);
  void FloorControlSignal(int control_error, uint conference_id, ushort floor_id);
  void ServerControlSignal(int control_error);
  void UserControlSignal(int control_error, uint conference_id, ushort user_id);

private:
  typedef boost::shared_ptr<bfcp::BaseServer> BaseServerPtr;
  static void ResetServer(BaseServerPtr server);

  void HandleConferenceControlResult(
    uint conference_id, 
    bfcp::ControlError error);

  void HandleFloorControlResult(
    uint conference_id, 
    ushort floor_id, 
    bfcp::ControlError error);
  
  void HandleUserControlResult(
    uint conference_id, 
    ushort user_id, 
    bfcp::ControlError error);
  
  void HandleChairControlResult(
    uint conference_id, 
    ushort floor_id, 
    ushort user_id, 
    bfcp::ControlError error);

  void HandleGetCoferenceIDsResult(
    bfcp::ControlError error, void *data);

  void HandleGetConferenceInfoResult(
    uint conference_id, bfcp::ControlError error, void *data);

  BaseServerPtr server_;
  muduo::net::EventLoop *loop_;
  muduo::net::EventLoopThread thread_;
};

#endif // BFCPSERVERSERVICE_H
