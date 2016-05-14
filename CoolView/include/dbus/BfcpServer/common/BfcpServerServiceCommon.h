#pragma once

#include <cstdint>
#include <Qtcore/QString>
#include <QtCore/QDataStream>

#define BFCP_SERVER_SERVICE_NAME "com.dcampus.coolview.BfcpServer"
#define BFCP_SERVER_SERVICE_OBJECT_PATH "/com/dcampus/coolview/BfcpServer"

namespace bfcps
{

enum ErrorCode 
{ 
  kNoError = 0,
  kUserNotExist = 1,
  kUserAlreadyExist = 2,
  kFloorNotExist = 3,
  kFloorAlreadyExist = 4, 
  kChairNotExost = 5,
  kChairAlreadyExist = 6,
  kConferenceNotExist = 7, 
  kConferenceAlreadyExist = 8, 
  kServerNotStart = 9, 
  kServerAlreadyStart = 10 
};

enum AcceptPolicy
{
  kAutoAccept = 0,
  kAutoDeny = 1
};

struct ServerArg 
{
  int af;
  uint16_t port;
  bool enable_connection_thread;
  int work_thread_num;
  double user_obsoleted_time; // when < 0.0, unlimited
};

struct ConferenceArg
{
  uint16_t max_floor_request;
  AcceptPolicy policy;
  double time_for_chair_action; // when < 0.0, unlimited
};

struct FloorArg
{
  uint16_t max_granted_num; // when == 0, unlimited
  double max_holding_time; // when < 0.0, unlimited
};

struct UserArg
{
  QString user_name;
  QString user_uri;
};

typedef QVector<uint32_t> ConferenceIDList;


} // namespace bfcps

inline QDataStream& operator>>(QDataStream &in, bfcps::ServerArg &arg)
{
  in >> arg.af >> arg.port >> arg.enable_connection_thread >> arg.work_thread_num >> arg.user_obsoleted_time;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcps::ServerArg &arg)
{
  out << arg.af << arg.port << arg.enable_connection_thread << arg.work_thread_num << arg.user_obsoleted_time;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcps::ConferenceArg &arg)
{
  int policy = 0;
  in >> arg.max_floor_request >> policy >> arg.time_for_chair_action;
  arg.policy = static_cast<bfcps::AcceptPolicy>(policy);
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcps::ConferenceArg &arg)
{
  out << arg.max_floor_request << static_cast<int>(arg.policy) 
      << arg.time_for_chair_action;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcps::FloorArg &arg)
{
  in >> arg.max_granted_num >> arg.max_holding_time;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcps::FloorArg &arg)
{
  out << arg.max_granted_num << arg.max_holding_time;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcps::UserArg &arg)
{
  in >> arg.user_name >> arg.user_uri;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcps::UserArg &arg)
{
  out << arg.user_name << arg.user_uri;
  return out;
}

