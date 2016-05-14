#pragma once

#include <cstdint>
#include <QtCore/QVector>
#include <Qtcore/QString>
#include <QtCore/QDataStream>

#define BFCP_CLIENT_SERVICE_NAME "com.dcampus.coolview.BfcpClient"
#define BFCP_CLIENT_SERVICE_OBJECT_PATH "/com/dcampus/coolview/BfcpClient"

namespace bfcpc
{

enum ClientState 
{ 
  kDisconnecting = 0,
  kDisconnected = 1,
  kConnecting = 2,
  kConnected = 3
};

enum ControlError
{
  kOk = 0,
  kClientAlreadyExist,
  kClientNotExist,
};

struct ClientArg
{
  int af;
  QString server_ip;
  uint16_t server_port;
  uint32_t conference_id;
  uint16_t user_id;
  double heart_beat_interval;

  bool operator==(const ClientArg &other)
  {
    return conference_id == other.conference_id &&
           user_id == other.user_id &&
           af == other.af &&
           server_ip == other.server_ip &&
           server_port == other.server_port;
  }
};

enum BfcpPriority
{
  kLowest = 0,
  kLow = 1,
  kNormal = 2,
  kHigh = 3,
  kHighest = 4
};

struct FloorRequestArg
{
  QVector<uint16_t> floor_ids;
  uint16_t beneficiary_id;
  QString participant_info;
  int priority;
  bool use_benficirary_id;
};

typedef QVector<uint16_t> FloorIDList;

enum RequestStatus
{
  kPending = 1,
  kAccepted = 2,
  kGranted = 3,
  kDenied = 4,
  kCancelled = 5,
  kRelease = 6,
  kRevoked = 7
};

struct ReqStatus
{
  RequestStatus request_status;
  uint8_t queue_pos;
};

struct ChairActionArg
{
  uint16_t floor_request_id;
  // only kAccepted, kDenied and kRevoked allowed in chair action
  ReqStatus status;
  QString status_info;
  FloorIDList floor_ids;
  // FIXME: currently ignore the floor status
};

struct UserQueryArg
{
  uint16_t beneficiary_id;
  bool use_beneficiary_id;
};


enum BfcpErrorCode
{
  kConfNoExist = 1,
  kUserNotExist = 2,
  kUnknownPrim = 3,
  kUnKnownMandAttr = 4,
  kUnauthOperation = 5,
  kInvalidFloorID = 6,
  kFloorReqIDNotExist = 7,
  kMaxFloorReqReached = 8,
  kUseTLS = 9,
  kParseError = 10,
  kUseDTLS = 11,
  kUnsupportedVersion = 12,
  kBadLength = 13,
  kGenericError = 14,
};

// response data
struct ResponseError
{
  BfcpErrorCode error_code;
  // FIXME: currently ignore error detail
  QString info;
};

struct OverallRequestStatus
{
  uint16_t floor_request_id;
  ReqStatus status;
  QString status_info;
  bool has_request_status;
};

struct FloorRequestStatus
{
  uint16_t floor_id;
  ReqStatus status;
  QString status_info;
  bool has_request_status;
};

typedef QVector<FloorRequestStatus> FloorRequestStatusList;

struct UserInfo
{
  uint16_t id;
  QString user_name;
  QString user_uri;
};

enum ContentType
{
  kHasNone = 0,
  kHasOverallRequestStatus = 0x1,
  kHasBeneficiaryInfo = 0x2,
  kHasRequestedByInfo = 0x4
};

struct FloorRequestInfo
{
  uint16_t floor_request_id;
  uint32_t content; // bit mask of Content Type
  OverallRequestStatus overall_req_status;
  FloorRequestStatusList floor_req_statuses;
  UserInfo beneficiary;
  UserInfo request_by;
  int priority;
  QString participant_priovided_info;
};

typedef QVector<FloorRequestInfo> FloorRequestInfoList;

struct UserStatus
{
  UserInfo beneficiary;
  FloorRequestInfoList frq_info_list;
  bool has_beneficiary;
};

struct FloorStatus
{
  uint16_t floor_id;
  bool has_floor_id;
  FloorRequestInfoList frq_info_list;
};

} // namespace bfcpc

inline QDataStream& operator>>(QDataStream &in, bfcpc::ClientArg &arg)
{
  in >> arg.af >> arg.server_ip >> arg.server_port >> arg.conference_id >> arg.user_id >> arg.heart_beat_interval;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::ClientArg &arg)
{
  out << arg.af << arg.server_ip << arg.server_port << arg.conference_id << arg.user_id << arg.heart_beat_interval;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::FloorRequestArg &arg)
{
  in >> arg.floor_ids >> arg.beneficiary_id >> arg.participant_info 
     >> arg.priority >> arg.use_benficirary_id;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::FloorRequestArg &arg)
{
  out << arg.floor_ids << arg.beneficiary_id << arg.participant_info 
      << arg.priority << arg.use_benficirary_id;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::ReqStatus &arg)
{
  int status;
  in >> status;
  arg.request_status = static_cast<bfcpc::RequestStatus>(status);
  in >> arg.queue_pos;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::ReqStatus &arg)
{
  out << arg.request_status << arg.queue_pos;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::ChairActionArg &arg)
{
  in >> arg.floor_request_id >> arg.status >> arg.status_info >> arg.floor_ids;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::ChairActionArg &arg)
{
  out << arg.floor_request_id << arg.status << arg.status_info << arg.floor_ids;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::UserQueryArg &arg)
{
  in >> arg.beneficiary_id >> arg.use_beneficiary_id;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::UserQueryArg &arg)
{
  out << arg.beneficiary_id << arg.use_beneficiary_id;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::ResponseError &arg)
{
  int error_code;
  in >> error_code;
  arg.error_code = static_cast<bfcpc::BfcpErrorCode>(error_code);
  in >> arg.info;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::ResponseError &arg)
{
  out << arg.error_code << arg.info;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::OverallRequestStatus &arg)
{
  in >> arg.floor_request_id >> arg.status >> arg.status_info >> arg.has_request_status;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::OverallRequestStatus &arg)
{
  out << arg.floor_request_id << arg.status << arg.status_info << arg.has_request_status;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::FloorRequestStatus &arg)
{
  in >> arg.floor_id >> arg.status >> arg.status_info >> arg.has_request_status;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::FloorRequestStatus &arg)
{
  out << arg.floor_id << arg.status << arg.status_info << arg.has_request_status;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::UserInfo &arg)
{
  in >> arg.id >> arg.user_name >> arg.user_uri;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::UserInfo &arg)
{
  out << arg.id << arg.user_name << arg.user_uri;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::FloorRequestInfo &arg)
{
  in >> arg.floor_request_id >> arg.content >> arg.overall_req_status
     >> arg.floor_req_statuses >> arg.beneficiary >> arg.request_by 
     >> arg.priority >> arg.participant_priovided_info;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::FloorRequestInfo &arg)
{
  out << arg.floor_request_id << arg.content << arg.overall_req_status
      << arg.floor_req_statuses << arg.beneficiary << arg.request_by 
      << arg.priority << arg.participant_priovided_info;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::UserStatus &arg)
{
  in >> arg.beneficiary >> arg.frq_info_list;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::UserStatus &arg)
{
  out << arg.beneficiary << arg.frq_info_list;
  return out;
}

inline QDataStream& operator>>(QDataStream &in, bfcpc::FloorStatus &arg)
{
  in >> arg.floor_id >> arg.has_floor_id >> arg.frq_info_list;
  return in;
}

inline QDataStream& operator<<(QDataStream &out, const bfcpc::FloorStatus &arg)
{
  out << arg.floor_id << arg.has_floor_id << arg.frq_info_list;
  return out;
}



