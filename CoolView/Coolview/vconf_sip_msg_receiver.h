#ifndef VCONF_SIP_MSG_RECEIVER_H
#define VCONF_SIP_MSG_RECEIVER_H

#include "sip_msg_receiver.h"

struct MediaRelayParam;

class VConfSipMsgReceiver 
  : public SipMsgReceiver {
  Q_OBJECT

 public:
  VConfSipMsgReceiver(QObject *parent);
  ~VConfSipMsgReceiver();

  void Initialize();

 public Q_SLOTS:

  //虚拟接收操作，使用QueuedConnection模拟更真实

  //发起虚拟会议接收操作，以显示虚拟会议
  void HandleVRecvConferenceListSlot();

  //进入虚拟会议时，模拟进入会议许可
  void HandleVRecvQoSLoginPermissionSlot();

  //模拟接收到终端列表
  void HandleVRecvTerminalListSlot(const TerminalList&);

  //模拟发送端发送媒体流ack
  void HandleVRecvStartMediaReplySlot(const MediaRelayParam &param);

 protected:
  // 重写信号处理handler
  void RecvConferenceListHandler(const SipMsgParser &parser, const QString &from);

 private:
  void AddVirtualConferenceToList(ConferenceList &conference);
};

#endif
