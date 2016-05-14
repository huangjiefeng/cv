#ifndef SIP_MSG_SENDER_H
#define SIP_MSG_SENDER_H

#include "msg_sender_interface.h"

class CvSipWrapperIf;

class SipMsgSender : public IMsgSender {
  Q_OBJECT

public:
  SipMsgSender(QObject *parent);
  virtual ~SipMsgSender();

  void Initialize();

public Q_SLOTS:
  // 终端向sip服务器登陆
  void SendTerminalLoginInfoSlot(const TerminalLoginParam &param) override;

  // 终端向服务器请求终端真实源地址认证消息
  void SendTerminalInfoForSAVASlot(const TerminalSAVAParam &param) override;

  // 用于登陆后，与Sip服务器的保活
  void SendRegisterHeartBeatToSipServerSlot() override;

  // 用于登陆后，与会议服务器的保活
  void SendOnlineHeartBeatToKeepLoginSlot() override;

  // 用于登陆后，向会议服务器发送在线测试信息
  void SendOnlineTestSlot() override;

  // 终端向sip服务器登出
  void SendTerminalLogoutInfoSlot() override;

  // 发送请求配置信息
  void SendGetTerminalConfigSlot() override;

  // 发送终端配置信息
  void SendReportTerminalConfigSlot(const ConfigDict &param) override;

  // 请求会议列表
  void RequestConferenceListSlot() override;

  // 请求发言权控制信息
  void RequestFloorControlInfoSlot(const QString &cid) override;

  // 请求加入会议
  void SendConferenceJoinInfoSlot(
    const ConferenceJoinParam &param,
    const SDPInfo &sdp_info) override;

  // 保持本地终端在会议中在线
  void SendOnlineHeartBeatToKeepInConferenceSlot(
    const QString &conference_uri) override;

  // 发送离开会议信息
  void SendConferenceLeaveInfoSlot(
    const QString &conference_uri) override;

  // 获取终端列表
  void RequestTerminalListSlot(const QString &conference_uri) override;

  // 查询当前的主讲人
  void QueryMainSpeakerSlot(const QString &conference_uri) override;
  // 查询主席终端
  void QueryChairmanSlot(const QString &conference_uri) override;
  // 查询共享屏幕终端
  void QueryShareScreenSlot(const QString &conference_uri) override;

  // 发送举手信息
  void SendHandUpMsgSlot(
    const QString &conference_uri,
    const QString &local_uri,
    bool handup) override;

  // 发送选择主讲人信息
  void SendAllowSpeakMsgSlot(
    const QString &conference_uri,
    const QString &terminal_uri,
    bool allow) override;

  // 请求远程终端向本地终端发送媒体流
  void RequestStartMediaSlot(const MediaRelayParam &param) override;

  // 回复远程终端向本地终端请求发送媒体流的请求
  void ReplyStartMediaRequestSlot(const MediaReplyParam &param) override;

  // 请求远程终端向本地终端停止发送媒体流
  void RequestStopMediaSlot(const MediaRelayParam &param) override;

  // 回复远程终端向本地终端请求向其停止发送媒体流的请求
  void ReplyStopMediaRequestSlot(const MediaReplyParam &param) override;

  // 告知所有参数终端，本地终端共享屏幕的发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  void SendScreenShareControlSlot(const ScreenControlParam &param) override;

  // 告知所有参会终端，本地终端媒体发送状态
  // enable为true，则为启动状态
  // enable为false，则为停止状态
  void SendMediaControlInfoSlot(const MediaControlParam &param) override;

  void SendRecordControlSlot( const RecordControlParam &param ) override;

  void SendRecordControlAckSlot( const RecordControlAckParam &param ) override;

private:
  // StartMedia和StopMedia的消息中，小流使用了不同的名称
  // 参数start用于设置对应的情形
  QString ConvertToQString(MediaStreamType type, bool start) const;
  QString ConvertToQString(MediaControlType type) const;

  // 判断ip地址是不是ipv6地址
  bool IsIpv6Address(const QString &ip) const;

private:
  CvSipWrapperIf *sip_wrapper_proxy_; 
  
  QString username_;
  QString regist_content_;
  QString ip_addr_;
  QString global_service_;
};

#endif // SIP_MSG_SENDER_H
