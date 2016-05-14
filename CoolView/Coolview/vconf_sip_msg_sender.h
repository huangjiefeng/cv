#ifndef VCONF_SIP_MSG_SENDER_H
#define VCONF_SIP_MSG_SENDER_H

#include "sip_msg_sender.h"
#include "vconf_sip_msg_receiver.h"


class VConfSipMsgSender 
  : public SipMsgSender {
  Q_OBJECT

 public:
  VConfSipMsgSender(QObject *parent);
  ~VConfSipMsgSender();

  void Initialize();

 public Q_SLOTS:
  //重写发送信令有关方法，处理虚拟会议
  void RequestConferenceListSlot() override;
  void RequestStartMediaSlot(const MediaRelayParam &param) override;
  void RequestTerminalListSlot(const QString &conference_uri) override;

  void SendConferenceJoinInfoSlot(
    const ConferenceJoinParam &param, 
    const SDPInfo &sdp_info) override;
  void SendConferenceLeaveInfoSlot(const QString &conference_uri) override;

 Q_SIGNALS:
  void NotifyVRequestConferenceListSignal();
  void NotifyVSendConferenceJoinInfoSignal();
  void NotifyVRequestStartMediaSlot(const MediaRelayParam &param);

};

#endif
