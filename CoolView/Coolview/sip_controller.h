#ifndef SIP_CONTROLLER_H
#define SIP_CONTROLLER_H

#include <functional>

#include <QObject>
#include <QString>
#include <QHash>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>

class CvSipWrapperIf;

#include "media_description.h"
#include "sip_msg_parser.h"

class SipController : public QObject {
  Q_OBJECT
public:
  struct TerminalLoginInfo {
    QString display_name; // 终端显示名
    QString username; // 终端用户名
    QString identity; // 终端标识
    QString password; // 密码
    QString realm;    // 会议服务器域名
    QString proxy_server;    // 代理服务器地址
    QString register_server; // 注册服务器地址
    QString ip_address; // 终端的ip地址
    QString mac;        // 终端的mac地址
  };

  struct TerminalSAVAInfo {
    QString ipv4_addr; // 终端的ipv4地址
    QString ipv6_addr; // 终端的ipv6地址
    QString terminal_version; // 终端的版本号
    int screen_count; // 终端的显示器数目
  };

  struct ConferenceJoinInfo {
    QString cid; // 会议id
    QString focus_uri; // 会议的uri
    QString uri;  // 终端的uri
    int min_rate; // 最低带宽值
    int max_rate; // 最大带宽值
    QString ip_addr;   // 终端的ip地址
    QString gateway; // 终端的网关
    int virtual_terminal_count; // 终端的虚拟终端数
  };

public:
  SipController(QObject *parent);
  ~SipController();

  void Initialize();

signals:
  // 接收到请求登陆Sip服务器的状态回应
  // state的值参考enum SipAccountState
  void NotifySipAccountStateChangedSignal(const QString &name, int state);

  // 接收到本地终端是否获得真实源地址认证
  void NotifyRecvSAVAResultSignal(bool valid);

  // 接收到会议信息
  void NotifyRecvConferenceListSignal(const ConferenceList &conferences);
  void NotifyRecvTerminalListSignal(const TerminalList &terminals);
  void NotifyRecvTerminalLoginSignal(const TerminalList &terminals);
  void NotifyRecvTerminalLogoutSignal(const QString &uri);
  void NotifyRecvTerminalHandUpSignal(const QString &uri);
  void NotifyRecvTerminalHandDownSignal(const QString &uri);

  // 接收到远程终端向本地终端请求向其发送媒体流
  void NotifyRecvStartMediaRelaySignal(
    const QString &remote_uri,
    const QString &remote_ip,
    const QString &local_vuri,
    MediaStreamType type);

  // 接收到远程终端对本地终端向其请求发送媒体流的回复
  void NotifyRecvMediaReplySignal(
    const QString &remote_vuri,
    MediaStreamType type,
    bool permission);

  // 接收到远程终端向本地终端请求停止向其发送媒体流
  void NotifyRecvStopMediaRelaySignal(
    const QString &remote_uri,
    const QString &remote_ip,
    const QString &local_vuri,
    MediaStreamType type);
  
  // 接收到远程终端对本地终端向其请求停止发送媒体流的回复
  void NotifyRecvStopMediaReplySignal(
    const QString &remote_vuri,
    MediaStreamType type,
    bool permission);

  // 接收到远程终端的媒体流发送状态
  void NotifyRecvSendMediaControlSignal(
    const QString &remote_uri,
    MediaControlType type,
    bool send);

  // 接收到媒体流控制信息
  void NotifyRecvMediaControlInfoSignal(
    const QString &terminal_uri, // 远程终端或本地终端
    MediaControlType type,
    bool send);

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
  void NotifyRecvSharedScreenControlSignal(const QString &uri, bool enable);

  // 收到sip服务器的心跳信息
  // type为test或heartbeat
  void NotifyRecvOnlineMessageSignal(const QString &type);
  
public slots:
  // 终端向sip服务器登陆
  void SendTerminalLoginInfoSlot(
    const SipController::TerminalLoginInfo &info);

  // 终端向服务器请求终端真实源地址认证消息
  void SendTerminalInfoForSAVASlot(
    const SipController::TerminalSAVAInfo &info);

  // 用于登陆后，与Sip服务器的保活
  void SendRegisterHeartBeatToSipServerSlot();

  // 用于登陆后，与会议服务器的保活
  void SendOnlineHeartBeatToKeepLoginSlot();

  // 用于登陆后，向会议服务器发送在线测试信息
  void SendOnlineTestSlot();

  // 终端向sip服务器登出
  void SendTerminalLogoutInfoSlot();

  // 请求会议列表
  void RequestConferenceListSlot();

  // 请求加入会议
  void SendConferenceJoinInfoSlot(
    const SipController::ConferenceJoinInfo &conference_info,
    const SDPInfo &sdp_info);

  // 保持本地终端在会议中在线
  void SendOnlineHeartBeatToKeepInConferenceSlot(
    const QString &conference_uri);
  
  // 发送离开会议信息
  void SendConferenceLeaveInfoSlot(
    const QString &conference_uri);

  // 获取终端列表
  void RequestTerminalListSlot(const QString &conference_uri);

  // 查询当前的主讲人
  void QueryMainSpeakerSlot(const QString &conference_uri);
  // 查询主席终端
  void QueryChairmanSlot(const QString &conference_uri);
  // 查询共享屏幕终端
  void QueryShareScreenSlot(const QString &conference_uri);

  // 发送举手信息
  void SendHandUpMsgSlot(
    const QString &conference_uri,
    const QString &local_uri,
    bool handup);

  // 发送选择主讲人信息
  void SendAllowSpeakMsgSlot(
    const QString &conference_uri,
    const QString &terminal_uri,
    bool allow);

  // 请求远程终端向本地终端发送媒体流
  void RequestStartMediaSlot(
    const QString &conference_uri,
    const QString &remote_uri,
    const QString &remote_vuri,
    const QString &local_ip_addr,
    MediaStreamType type);

  // 回复远程终端向本地终端请求发送媒体流的请求
  void ReplyStartMediaRequestSlot(
    const QString &conference_uri,
    const QString &remote_uri,
    const QString &local_vuri,
    MediaStreamType type,
    bool permission);

  // 请求远程终端向本地终端停止发送媒体流
  void RequestStopMediaSlot(
    const QString &conference_uri,
    const QString &remote_uri,
    const QString &remote_vuri,
    const QString &local_ip_addr,
    MediaStreamType type);

  // 回复远程终端向本地终端请求向其停止发送媒体流的请求
  void ReplyStopMediaRequestSlot(
    const QString &conference_uri,
    const QString &remote_uri,
    const QString &local_vuri,
    MediaStreamType type,
    bool permission);

  // 告知所有参数终端，本地终端共享屏幕的发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  void SendScreenShareControlSlot(
    const QString &conference_uri,
    const QString &ip,
    const QString &ppt_port, 
    bool enable);

  // 告知所有参会终端，本地终端媒体发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  void SendMediaControlInfoSlot(
    const QString &conference_uri,
    const QString &local_uri, // 本地终端uri
    MediaControlType type,
    bool enable);

private slots:
  // 解析从SipWrapper接收到的信息
  void HandleMessageReceivedNotifySlot(const QString &msg, const QString &from);

private:
  // StartMedia和StopMedia的消息中，小流使用了不同的名称
  // 参数start用于设置对应的情形
  QString ConvertToQString(MediaStreamType type, bool start) const;
  MediaStreamType ConvertToMediaStreamType(const QString &type);

  QString ConvertToQString(MediaControlType type) const;
  MediaControlType ConvertToMediaControlType(const QString &type) const;

  // 判断ip地址是不是ipv6地址
  bool IsIpv6Address(const QString &ip) const;

  // 注册所有的sip消息解析方法
  void RegistHandlers();

  // 解析本地终端登录的认证的信息
  void RecvTerminalLoginNotifyHandler(const SipMsgParser &parser, const QString &from);

  // 解析会议相关的信息
  void RecvConferenceListHandler(const SipMsgParser &parser, const QString &from);
  void RecvTerminalListHandler(const SipMsgParser &parser, const QString &from);
  void RecvTerminalLoginHandler(const SipMsgParser &parser, const QString &from);
  void RecvTerminalLogoutHandler(const SipMsgParser &parser, const QString &from);
  void RecvTerminalHandUpHandler(const SipMsgParser &parser, const QString &from);
  void RecvTerminalHandDownHandler(const SipMsgParser &parser, const QString &from);

  // 解析媒体流相关的信息
  void RecvStartMediaReplyHandler(const SipMsgParser &parser, const QString &from);
  void RecvStartMediaRelayHandler(const SipMsgParser &parser, const QString &from);
  void RecvStopMediaReplyHandler(const SipMsgParser &parser, const QString &from);
  void RecvStopMediaRelayHandler(const SipMsgParser &parser, const QString &from);
  void RecvSendMediaControlHandler(const SipMsgParser &parser, const QString &from);
  void RecvMediaControlInfoHandler(const SipMsgParser &parser, const QString &from);

  // 解析QoS相关的信息
  void RecvQoSMsgHandler(const SipMsgParser &parser, const QString &from);

  // 解析主讲人相关的信息
  void RecvSetSpeakerTerminalHandler(const SipMsgParser &parser, const QString &from);
  void RecvSpeakerTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  void RecvForbidSpeakerHandler(const SipMsgParser &parser, const QString &from);
  // 解析主席终端相关的信息
  void RecvSetChairmanTerminalHandler(const SipMsgParser &parser, const QString &from);
  void RecvChairmanTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  // 解析屏幕共享者相关的信息
  void RecvShareScreenTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  void RecvShareScreenControlInfoHandler(const SipMsgParser &parser, const QString &from);
  
  // 解析服务器的心跳信息
  void RecvOnlineMessageInfoHandler(const SipMsgParser &parser, const QString &from);

private:
  typedef std::function<
    void(SipController&, const SipMsgParser&, const QString&)
  > Handler;

  typedef QHash<QString, Handler> HandlerDictionary;

  CvSipWrapperIf *sip_wrapper_proxy_;
  HandlerDictionary type_handler_;

  QString username_;
  QString regist_content_;
  QString ip_addr_;
  QString global_service_;
};

#endif // SIP_CONTROLLER_H
