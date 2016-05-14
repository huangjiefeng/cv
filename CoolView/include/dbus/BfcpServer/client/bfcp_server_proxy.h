#ifndef BFCP_SERVER_CONTROLLER_H
#define BFCP_SERVER_CONTROLLER_H

#include <QObject>

#include <dbus/BfcpServer/common/BfcpServerServiceCommon.h>
#include <dbus/BfcpServer/client/BfcpServerIf.h>

class BfcpServerIf;

class BfcpServerProxy : public QObject
{
  Q_OBJECT

public:
  BfcpServerProxy(QObject *parent);
  ~BfcpServerProxy();

public Q_SLOTS:
  void Start(const bfcps::ServerArg &arg);
  void Stop();
  void Quit();

  void AddConference(uint32_t conference_id, const bfcps::ConferenceArg &arg);
  void RemoveConference(uint32_t conference_id);
  void ModifyConference(uint32_t conference_id, const bfcps::ConferenceArg &arg);

  void GetConferenceIDs();
  void GetConferenceInfo(uint32_t conference_id);

  void AddFloor(uint32_t conference_id, uint16_t floor_id, const bfcps::FloorArg &arg);
  void RemoveFloor(uint32_t conference_id, uint16_t floor_id);
  void ModifyFloor(uint32_t conference_id, uint16_t floor_id, const bfcps::FloorArg &arg);

  void AddUser(uint32_t conference_id, uint16_t user_id, const bfcps::UserArg &arg);
  void RemoveUser(uint32_t conference_id, uint16_t user_id);

  void SetChair(uint32_t conference_id, uint16_t floor_id, uint16_t user_id);
  void RemoveChair(uint32_t conference_id, uint16_t floor_id);

Q_SIGNALS:
  void NotifyServerControlResultSignal(bfcps::ErrorCode error_code);

  void NotifyConferenceControlResultSignal(bfcps::ErrorCode error_code, uint32_t conference_id);
  
  void NotifyFloorControlResultSignal(
    bfcps::ErrorCode error_code, uint32_t conference_id, uint16_t floor_id);
  
  void NotifyUserControlResultSignal(
    bfcps::ErrorCode error_code, uint32_t conference_id, uint16_t user_id);

  void NotifyChairControlResultSignal(
    bfcps::ErrorCode error_code, 
    uint32_t conference_id, 
    uint16_t floor_id, 
    uint16_t user_id);
  
  void NotifyConferenceIDsSignal(
    bfcps::ErrorCode error_code, 
    const bfcps::ConferenceIDList &conference_ids);

  void NotifyConferenceInfoSignal(
    bfcps::ErrorCode error_code,
    uint32_t conference_id,
    const QString &conference_info);

private Q_SLOTS:
  void HandleServerControlResultSlot(int control_error);
  void HandleConferenceControlResultSlot(int control_error, uint conference_id);
  void HandleFloorControlResultSlot(int control_error, uint conference_id, ushort floor_id);
  void HandleUserControlResultSlot(int control_error, uint conference_id, ushort user_id);
  void HandleChairControlResultSlot(
    int control_error, uint conference_id, ushort floor_id, ushort user_id);
  void HandleConferenceIDsResultSlot(int control_error, const QByteArray &conference_ids);
  void HandleConferenceInfoResultSlot(
    int control_error, uint conference_id, const QString &conference_info);

private:
  void Connect();

  BfcpServerIf *server_proxy_;
};

#endif // BFCP_SERVER_CONTROLLER_H
