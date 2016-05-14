#ifndef SIP_MSG_RECEIVER_H
#define SIP_MSG_RECEIVER_H

#include <functional>
#include <QHash>
#include "msg_receiver_interface.h"

class CvSipWrapperIf;
class SipMsgParser;

class SipMsgReceiver : public IMsgReceiver {
  Q_OBJECT

public:
  SipMsgReceiver(QObject *parent);
  virtual ~SipMsgReceiver();

  void Initialize();

private Q_SLOTS:
  // 解析从SipWrapper接收到的信息
  void HandleMessageReceivedNotifySlot(const QString &msg, const QString &from);

protected:
  MediaStreamType ConvertToMediaStreamType(const QString &type) const;
  MediaControlType ConvertToMediaControlType(const QString &type) const;

  // 判断ip地址是不是ipv6地址
  bool IsIpv6Address(const QString &ip) const;

  // 注册所有的sip消息解析方法
  virtual void RegistHandlers();

  // 解析本地终端登录的认证的信息
  virtual void RecvTerminalLoginNotifyHandler(const SipMsgParser &parser, const QString &from);

  // 解析终端配置的信息
  void RecvTerminalConfigHandler(const SipMsgParser &parser, const QString &from);

  // 解析会议相关的信息
  virtual void RecvConferenceListHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvTerminalListHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvTerminalLoginHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvTerminalLogoutHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvTerminalHandUpHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvTerminalHandDownHandler(const SipMsgParser &parser, const QString &from);

  // 解析媒体流相关的信息
  virtual void RecvStartMediaReplyHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvStartMediaRelayHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvStopMediaReplyHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvStopMediaRelayHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvSendMediaControlHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvMediaControlInfoHandler(const SipMsgParser &parser, const QString &from);

  // 解析QoS相关的信息
  virtual void RecvQoSMsgHandler(const SipMsgParser &parser, const QString &from);

  // 解析主讲人相关的信息
  virtual void RecvSetSpeakerTerminalHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvSpeakerTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvForbidSpeakerHandler(const SipMsgParser &parser, const QString &from);
  // 解析主席终端相关的信息
  virtual void RecvSetChairmanTerminalHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvChairmanTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  // 解析屏幕共享者相关的信息
  virtual void RecvShareScreenTerminalInfoHandler(const SipMsgParser &parser, const QString &from);
  virtual void RecvShareScreenControlInfoHandler(const SipMsgParser &parser, const QString &from);

  // 解析服务器的心跳信息
  virtual void RecvOnlineMessageInfoHandler(const SipMsgParser &parser, const QString &from);

  virtual void RecvTextMessageHandler(const SipMsgParser &parser, const QString &from);

  // 解析录制控制的信息
  virtual void RecvRecordControlHandler(const SipMsgParser &parser, const QString &from);
  // 解析录制控制回复的信息
  virtual void RecvRecordControlAckHandler(const SipMsgParser &parser, const QString &from);

  // 解析发言权控制信息
  virtual void RecvFloorControlInfoHandler(const SipMsgParser &parser, const QString &from);

protected:
  typedef std::function<
    void(SipMsgReceiver&, const SipMsgParser&, const QString&)
  > Handler;

  typedef QHash<QString, Handler> HandlerDictionary;

  CvSipWrapperIf *sip_wrapper_proxy_;
  HandlerDictionary type_handler_;
};

#endif // SIP_MSG_RECEIVER_H
