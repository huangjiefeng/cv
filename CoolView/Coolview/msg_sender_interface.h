#ifndef MSG_SENDER_INTERFACE_H
#define MSG_SENDER_INTERFACE_H

#include <QObject>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include "media_description.h"
#include "config_description.h"

struct TerminalLoginParam {
  QString display_name; // 终端显示名
  QString username; // 终端用户名
  QString identity; // 终端标识
  QString password; // 密码
  QString realm;    // 会议服务器域名
  QString proxy_server;    // 代理服务器地址
  QString register_server; // 注册服务器地址
  QString ip_address; // 终端的ip地址
  QString mac;        // 终端的mac地址
  QString global_service; // 全局会议服务器
};

struct TerminalSAVAParam {
  QString ipv4_addr; // 终端的ipv4地址
  QString ipv6_addr; // 终端的ipv6地址
  QString terminal_version; // 终端的版本号
  int screen_count; // 终端的显示器数目
};

struct ConferenceJoinParam {
  QString cid; // 会议id
  QString focus_uri; // 会议的uri
  QString uri;  // 终端的uri
  int min_rate; // 最低带宽值
  int max_rate; // 最大带宽值
  QString ip_addr;   // 终端的ip地址
  QString gateway; // 终端的网关
  int virtual_terminal_count; // 终端的虚拟终端数
};

struct MediaRelayParam {
  QString conference_uri;
  QString remote_uri;
  QString remote_vuri;
  QString local_ip_addr;
  MediaStreamType type;
};

struct MediaReplyParam {
  QString conference_uri;
  QString remote_uri;
  QString local_vuri;
  MediaStreamType type;
  bool permission;
};

struct ScreenControlParam {
  QString conference_uri;
  QString terminal_uri;
  QString ip_addr;
  QString ppt_port;
  bool enable;
};

struct MediaControlParam {
  QString conference_uri;
  QString local_uri; // 本地终端uri
  MediaControlType type;
  bool enable;
};

struct RecordControlParam {
  QString conference_uri;
  QString requester_uri;
  QString requester_ip;
  QString tx_uri;
  QString target_vuri;
  int transaction_id;
  int type;
  int port;
};

struct RecordControlAckParam {
  QString conference_uri;
  QString requester_uri;
  QString tx_uri;
  QString target_vuri;
  int transaction_id;
  int error_code;
};

class IMsgSender : public QObject {
  Q_OBJECT
public:
  IMsgSender(QObject *parent) : QObject(parent) {}
  virtual ~IMsgSender() {}

  static void RegisterMetaType() {
    qRegisterMetaType<TerminalLoginParam>("TerminalLoginParam");
    qRegisterMetaType<TerminalSAVAParam>("TerminalSAVAParam");
    qRegisterMetaType<ConferenceJoinParam>("ConferenceJoinParam");
    qRegisterMetaType<MediaRelayParam>("MediaRelayParam");
    qRegisterMetaType<MediaReplyParam>("MediaReplyParam");
    qRegisterMetaType<ScreenControlParam>("ScreenControlParam");
    qRegisterMetaType<MediaControlParam>("MediaControlParam");
    qRegisterMetaType<SDPInfo>("SDPInfo");
    qRegisterMetaType<RecordControlParam>("RecordControlParam");
    qRegisterMetaType<RecordControlAckParam>("RecordControlParamAck");
  }

public Q_SLOTS:
  // 终端向sip服务器登陆
  virtual void SendTerminalLoginInfoSlot(
    const TerminalLoginParam &info) = 0;

  // 终端向服务器请求终端真实源地址认证消息
  virtual void SendTerminalInfoForSAVASlot(
    const TerminalSAVAParam &info) = 0;

  // 用于登陆后，与Sip服务器的保活
  virtual void SendRegisterHeartBeatToSipServerSlot() = 0;

  // 用于登陆后，与会议服务器的保活
  virtual void SendOnlineHeartBeatToKeepLoginSlot() = 0;

  // 用于登陆后，向会议服务器发送在线测试信息
  virtual void SendOnlineTestSlot() = 0;

  // 终端向sip服务器登出
  virtual void SendTerminalLogoutInfoSlot() = 0;

  // 发送请求配置信息
  virtual void SendGetTerminalConfigSlot() = 0;

  // 发送终端配置信息
  virtual void SendReportTerminalConfigSlot(const ConfigDict &param) = 0;

  // 请求会议列表
  virtual void RequestConferenceListSlot() = 0;

  // 请求发言权控制信息
  virtual void RequestFloorControlInfoSlot(const QString &cid) = 0;

  // 请求加入会议
  virtual void SendConferenceJoinInfoSlot(
    const ConferenceJoinParam &conference_info,
    const SDPInfo &sdp_info) = 0;

  // 保持本地终端在会议中在线
  // TODO: 添加时间戳
  virtual void SendOnlineHeartBeatToKeepInConferenceSlot(
    const QString &conference_uri) = 0;

  // 发送离开会议信息
  virtual void SendConferenceLeaveInfoSlot(
    const QString &conference_uri) = 0;

  // 获取终端列表
  virtual void RequestTerminalListSlot(const QString &conference_uri) = 0;

  // 查询当前的主讲人
  virtual void QueryMainSpeakerSlot(const QString &conference_uri) = 0;
  // 查询主席终端
  virtual void QueryChairmanSlot(const QString &conference_uri) = 0;
  // 查询共享屏幕终端
  virtual void QueryShareScreenSlot(const QString &conference_uri) = 0;

  // 发送举手信息
  virtual void SendHandUpMsgSlot(
    const QString &conference_uri,
    const QString &local_uri,
    bool handup) = 0;

  // 发送选择主讲人信息
  virtual void SendAllowSpeakMsgSlot(
    const QString &conference_uri,
    const QString &terminal_uri, // 为本地终端uri或远程终端uri
    bool allow) = 0;

  // 请求远程终端向本地终端发送媒体流
  virtual void RequestStartMediaSlot(const MediaRelayParam &param) = 0;

  // 回复远程终端向本地终端请求发送媒体流的请求
  virtual void ReplyStartMediaRequestSlot(const MediaReplyParam &param) = 0;

  // 请求远程终端向本地终端停止发送媒体流
  virtual void RequestStopMediaSlot(const MediaRelayParam &param) = 0;

  // 回复远程终端向本地终端请求向其停止发送媒体流的请求
  virtual void ReplyStopMediaRequestSlot(const MediaReplyParam &param) = 0;

  // 告知所有参会终端，本地终端共享屏幕的发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  virtual void SendScreenShareControlSlot(const ScreenControlParam &param) = 0;

  // 告知所有参会终端，本地终端媒体发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  virtual void SendMediaControlInfoSlot(const MediaControlParam &param) = 0;

  // 通知TX对目标vuri开始/停止录制
  virtual void SendRecordControlSlot(const RecordControlParam &param) = 0;

  // TX对请求者的回复
  virtual void SendRecordControlAckSlot(const RecordControlAckParam &param) = 0;
};


#endif // MSG_SENDER_INTERFACE_H
