#ifndef MSG_RECEIVER_INTERFACE_H
#define MSG_RECEIVER_INTERFACE_H

#include <QObject>
#include <QHash>

#include "conference_description.h"
#include "terminal_description.h"
#include "qos_description.h"
#include "media_description.h"
#include "config_description.h"
#include "floor_control_info_description.h"

struct MediaRelayMsg {
  QString remote_uri;
  QString remote_ip;
  QString local_vuri;
  MediaStreamType type;
};

struct MediaReplyMsg {
  QString remote_uri;
  QString remote_vuri;
  MediaStreamType type;
  bool permission;
};

struct MediaStatusMsg {
  QString remote_uri;
  MediaControlType type;
  bool send;
};

struct MediaControlMsg {
  QString terminal_uri; // 远程终端或本地终端
  MediaControlType type;
  bool send;
};

struct RecordControlMsg {
  QString requester_uri;
  QString requester_ip;
  QString tx_uri;
  QString target_vuri;
  int transaction_id;
  int type;
  int port;
};

struct RecordControlAckMsg {
  QString requester_uri;
  QString tx_uri;
  QString target_vuri;
  int transaction_id;
  int error_code;
};

class IMsgReceiver : public QObject {
  Q_OBJECT
public:
  IMsgReceiver(QObject *parent) : QObject(parent) {

  }
  virtual ~IMsgReceiver() {};

  static void RegisterMetaType() {
    qRegisterMetaType<LoginPermissionInfo>("LoginPermissionInfo");
    qRegisterMetaType<LoginRejectionInfo>("LoginRejectionInfo");
    qRegisterMetaType<QoSServerInfo>("QoSServerInfo");

    qRegisterMetaType<MediaRelayMsg>("MediaRelayMsg");
    qRegisterMetaType<MediaReplyMsg>("MediaReplyMsg");
    qRegisterMetaType<MediaStatusMsg>("MediaStatusMsg");
    qRegisterMetaType<MediaControlMsg>("MediaControlMsg");

    qRegisterMetaType<RecordControlMsg>("RecordControlMsg");
    qRegisterMetaType<RecordControlAckMsg>("RecordControlAckMsg");

    MediaHelper::RegisterMetaType();
    TerminalHelper::RegisterMetaType();
    ConferenceHelper::RegisterMetaType();
    ConfigRegister::RegisterMetaType();
    FloorControlInfoRegister::RegisterMetaType();
  }

Q_SIGNALS:
  // 接收到请求登陆Sip服务器的状态回应
  // state的值参考enum SipAccountState
  void NotifySipAccountStateChangedSignal(const QString &name, int state);

  // 接收到本地终端是否获得真实源地址认证
  void NotifyRecvSAVAResultSignal(bool valid);

  // 接收到终端配置信息
  void NotifyRecvTerminalConfigSignal(const ConfigDict &config);

  // 接收到会议信息
  void NotifyRecvConferenceListSignal(const ConferenceList &conferences);
  void NotifyRecvFloorControlInfoSignal(const FloorControlInfoPointer &floor_control_info);
  void NotifyRecvTerminalListSignal(const TerminalList &terminals);
  void NotifyRecvTerminalLoginSignal(const TerminalList &terminals);
  void NotifyRecvTerminalLogoutSignal(const QString &uri);
  void NotifyRecvTerminalHandUpSignal(const QString &uri);
  void NotifyRecvTerminalHandDownSignal(const QString &uri);

  // 接收到远程终端向本地终端请求向其发送媒体流
  void NotifyRecvStartMediaRelaySignal(const MediaRelayMsg &info);

  // 接收到远程终端对本地终端向其请求发送媒体流的回复
  void NotifyRecvStartMediaReplySignal(const MediaReplyMsg &info);

  // 接收到远程终端向本地终端请求停止向其发送媒体流
  void NotifyRecvStopMediaRelaySignal(const MediaRelayMsg &info);

  // 接收到远程终端对本地终端向其请求停止发送媒体流的回复
  void NotifyRecvStopMediaReplySignal(const MediaReplyMsg &info);

  // 接收到远程终端的媒体流发送状态
  void NotifyRecvRemoteMediaStatusSignal(const MediaStatusMsg &info);

  // 接收到媒体流控制信息
  void NotifyRecvMediaControlInfoSignal(const MediaControlMsg &info);

  // 收到QoS允许登陆的信息
  void NotifyRecvQoSLoginPermissionSignal(const LoginPermissionInfo &info);
  // 收到QoS拒绝登陆的信息
  void NotifyRecvQoSLoginRejectionSignal(const LoginRejectionInfo &info);
  // 收到QoS服务器信息
  void NotifyRecvQoSServerInfoSignal(const QoSServerInfo &info);

  // 收到选择主讲人的信息
  void NotifyRecvPermitSpeakSignal(const QString &uri);
  // 收到取消主讲人的信息
  void NotifyRecvForbidSpeakSignal(const QString &uri);
  // 收到作为主讲人的终端
  void NotifyRecvSpeakerTerminalSignal(const QString &uri);

  // 收到选择主席的信息
  void NotifyRecvChairmanUpdateSignal(const QString &uri);
  // 收到作为主席的终端
  void NotifyRecvChairmanSignal(const QString &uri);

  // 收到作为共享屏幕的终端
  void NotifyRecvSharedScreenTerminalSignal(const QString &uri);
  // 收到共享屏幕终端的发送状态
  void NotifyRecvSharedScreenControlSignal(const QString &ip, bool enable);

  // 收到sip服务器的心跳信息
  // type为test或heartbeat
  void NotifyRecvOnlineMessageSignal(const QString &type, const QString &from);

  void NotifyTextMessageSignal(const QString &text, const QString &sender);

  // 收到录制控制的请求
  void NotifyRecvRecordControlSignal(const RecordControlMsg &msg, const QString &from);
  // 收到录制控制的回复
  void NotifyRecvRecordControlAckSignal(const RecordControlAckMsg &msg, const QString &from);
};

#endif // MSG_RECEIVER_INTERFACE_H
