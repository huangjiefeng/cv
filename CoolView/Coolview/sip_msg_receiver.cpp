#include "sip_msg_receiver.h"

#include <QDebug>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include <dbus/sipwrapper/client/CvSipWrapperIf.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

#include "sip_msg_parser.h"

using std::mem_fn;

SipMsgReceiver::SipMsgReceiver(QObject *parent)
    : IMsgReceiver(parent) {
  sip_wrapper_proxy_ = nullptr;
}

SipMsgReceiver::~SipMsgReceiver() {
  if (sip_wrapper_proxy_) {
    delete sip_wrapper_proxy_;
  }
}

void SipMsgReceiver::Initialize() {
  sip_wrapper_proxy_ = new CvSipWrapperIf(
    SIP_WRAPPER_SERVICE_NAME, 
    SIP_WRAPPER_OBJECT_PATH, 
    QDBusConnection::sessionBus());

  connect(
    sip_wrapper_proxy_, 
    SIGNAL(cvMessageReceivedEvent(const QString&, const QString&)),
    this,
    SLOT(HandleMessageReceivedNotifySlot(const QString&, const QString&)));

  connect(
    sip_wrapper_proxy_, 
    SIGNAL(phoneLineStateChangedSignal(const QString&, int)), 
    this, 
    SIGNAL(NotifySipAccountStateChangedSignal(const QString&, int)));

  RegistHandlers();
}

void SipMsgReceiver::RegistHandlers() {
  type_handler_.insert(
    "TerminalLoginNotify", 
    mem_fn(&SipMsgReceiver::RecvTerminalLoginNotifyHandler));

  type_handler_.insert(
    "Text", mem_fn(&SipMsgReceiver::RecvTextMessageHandler));

  type_handler_.insert(
    "ConfList", mem_fn(&SipMsgReceiver::RecvConferenceListHandler));

  type_handler_.insert(
    "TerminalList", mem_fn(&SipMsgReceiver::RecvTerminalListHandler));

  type_handler_.insert(
    "LoginNotify", mem_fn(&SipMsgReceiver::RecvTerminalLoginHandler));

  type_handler_.insert(
    "loginnotify", mem_fn(&SipMsgReceiver::RecvTerminalLoginHandler));

  type_handler_.insert(
    "LogoutNotify", mem_fn(&SipMsgReceiver::RecvTerminalLogoutHandler));

  type_handler_.insert(
    "logoutnotify", mem_fn(&SipMsgReceiver::RecvTerminalLogoutHandler));

  type_handler_.insert(
    "HandUp", mem_fn(&SipMsgReceiver::RecvTerminalHandUpHandler));

  type_handler_.insert(
    "HandDown", mem_fn(&SipMsgReceiver::RecvTerminalHandDownHandler));

  type_handler_.insert(
    "StartMediaReply", mem_fn(&SipMsgReceiver::RecvStartMediaReplyHandler));

  type_handler_.insert(
    "StartMediaRelay", mem_fn(&SipMsgReceiver::RecvStartMediaRelayHandler));

  type_handler_.insert(
    "StopMediaReply", mem_fn(&SipMsgReceiver::RecvStopMediaReplyHandler));

  type_handler_.insert(
    "StopMediaRelay", mem_fn(&SipMsgReceiver::RecvStopMediaRelayHandler));

  type_handler_.insert(
    "SendMediaControl", mem_fn(&SipMsgReceiver::RecvSendMediaControlHandler));

  type_handler_.insert(
    "MediaControl", mem_fn(&SipMsgReceiver::RecvMediaControlInfoHandler));

  type_handler_.insert("qos", mem_fn(&SipMsgReceiver::RecvQoSMsgHandler));

  type_handler_.insert(
    "SetSpeakerTerminal", 
    mem_fn(&SipMsgReceiver::RecvSetSpeakerTerminalHandler));

  type_handler_.insert(
    "GetSpeakerTerminal",
    mem_fn(&SipMsgReceiver::RecvSpeakerTerminalInfoHandler));

  type_handler_.insert(
    "ForbidSpeaker", mem_fn(&SipMsgReceiver::RecvForbidSpeakerHandler));

  type_handler_.insert(
    "SetChairmanTerminal", 
    mem_fn(&SipMsgReceiver::RecvSetChairmanTerminalHandler));

  type_handler_.insert(
    "GetChairmanTerminal", 
    mem_fn(&SipMsgReceiver::RecvChairmanTerminalInfoHandler));

  type_handler_.insert(
    "GetScreenShareTerminal", 
    mem_fn(&SipMsgReceiver::RecvShareScreenTerminalInfoHandler));

  type_handler_.insert(
    "ShareScreenControl",
    mem_fn(&SipMsgReceiver::RecvShareScreenControlInfoHandler));

  type_handler_.insert(
    "Online",
    mem_fn(&SipMsgReceiver::RecvOnlineMessageInfoHandler));

  type_handler_.insert(
    "RecordControl",
    mem_fn(&SipMsgReceiver::RecvRecordControlHandler));
  
  type_handler_.insert(
    "RecordControlAck",
    mem_fn(&SipMsgReceiver::RecvRecordControlAckHandler));

  type_handler_.insert(
    "TerminalConfig",
    mem_fn(&SipMsgReceiver::RecvTerminalConfigHandler));

  type_handler_.insert(
    "FCSInfo",
    mem_fn(&SipMsgReceiver::RecvFloorControlInfoHandler));
}

void SipMsgReceiver::HandleMessageReceivedNotifySlot(const QString &msg,
                                                    const QString &from) {
  SipMsgParser parser;
  if (!parser.Init(msg)) {
    qDebug() << "SipMsgReceiver: Receive broken sip message";
    return;
  }

  QString command = parser.GetCommandType();
  HandlerDictionary::iterator it = type_handler_.find(command);
  if (it != type_handler_.end()) {
    qDebug() << "SipMsgReceiver: receive sip command: " << command << " from" << from;
    it.value()(*this, parser, from);
  } else {
    qDebug() << "SipMsgReceiver: unknown sip command: " << command;
  }
}

void SipMsgReceiver::RecvTextMessageHandler(const SipMsgParser &parser, 
                                            const QString &from) {
  QString sender = from.mid(0, from.indexOf('@'));
  QString msg = parser.GetTextMessage();
  emit NotifyTextMessageSignal(msg, sender);
}

void SipMsgReceiver::RecvTerminalLoginNotifyHandler(const SipMsgParser &parser, 
                                                    const QString &from) {
  QString valid = parser.GetResult();
  emit NotifyRecvSAVAResultSignal(valid == "true");
}

void SipMsgReceiver::RecvTerminalConfigHandler( const SipMsgParser &parser, const QString &from )
{
  ConfigDict config = parser.GetConfigDict();
  emit NotifyRecvTerminalConfigSignal(config);
}

void SipMsgReceiver::RecvConferenceListHandler(const SipMsgParser &parser,
                                               const QString &from) {
  ConferenceList conferences;
  parser.GetConferenceList(conferences);
  emit NotifyRecvConferenceListSignal(conferences);
}

void SipMsgReceiver::RecvTerminalListHandler(const SipMsgParser &parser, 
                                             const QString &from) {
  TerminalList terminals;
  parser.GetTerminalList(terminals);
  emit NotifyRecvTerminalListSignal(terminals);
}

void SipMsgReceiver::RecvTerminalLoginHandler(const SipMsgParser &parser, 
                                              const QString &from) {
  TerminalList terminals;
  parser.GetLoginTerminals(terminals);
  if (!terminals.empty())
    emit NotifyRecvTerminalLoginSignal(terminals);
}

void SipMsgReceiver::RecvTerminalLogoutHandler(const SipMsgParser &parser,
                                               const QString &from) {
  QString logout_uri = parser.GetURI();
  emit NotifyRecvTerminalLogoutSignal(logout_uri);
}

void SipMsgReceiver::RecvTerminalHandUpHandler(const SipMsgParser &parser,
                                               const QString &from) {
  QString hand_up_uri = parser.GetTerminalURI();
  emit NotifyRecvTerminalHandUpSignal(hand_up_uri);
}

void SipMsgReceiver::RecvTerminalHandDownHandler(const SipMsgParser &parser, 
                                                 const QString &from) {
  QString hand_down_uri = parser.GetTerminalURI();
  emit NotifyRecvTerminalHandDownSignal(hand_down_uri);
}

void SipMsgReceiver::RecvStartMediaRelayHandler(const SipMsgParser &parser,
                                                const QString &from) {
  MediaRelayMsg info;
  info.remote_uri = parser.GetURI();
  info.remote_ip = parser.GetReceiverIP();
  info.local_vuri = parser.GetVirtualURI();
  info.type = ConvertToMediaStreamType(parser.GetType());

  emit NotifyRecvStartMediaRelaySignal(info);
}

void SipMsgReceiver::RecvStartMediaReplyHandler(const SipMsgParser &parser, 
                                                const QString &from) {
  MediaReplyMsg info;
  info.remote_uri = parser.GetURI();
  info.type = ConvertToMediaStreamType(parser.GetType());
  info.permission = parser.GetPermission() == "true";
  info.remote_vuri = parser.GetVirtualURI();
  if (info.remote_vuri.isEmpty()) {
    info.remote_vuri = 
      TerminalHelper::ConstructDefaultVirtualURI(info.remote_uri);
  }
  emit NotifyRecvStartMediaReplySignal(info);
}

void SipMsgReceiver::RecvStopMediaRelayHandler(const SipMsgParser &parser, 
                                               const QString &from) {
  MediaRelayMsg info;
  info.remote_uri = parser.GetURI();
  info.remote_ip = parser.GetReceiverIP();
  info.local_vuri = parser.GetVirtualURI();
  info.type = ConvertToMediaStreamType(parser.GetType());
  emit NotifyRecvStopMediaRelaySignal(info);
}

void SipMsgReceiver::RecvStopMediaReplyHandler(const SipMsgParser &parser,
                                               const QString &from) {
  MediaReplyMsg info;
  QString remote_uri = parser.GetURI();
  info.type = ConvertToMediaStreamType(parser.GetType());
  info.permission = parser.GetPermission() == "true";
  info.remote_vuri = parser.GetVirtualURI();
  if (info.remote_vuri.isEmpty()) {
    info.remote_vuri = TerminalHelper::ConstructDefaultVirtualURI(remote_uri);
  }
  emit NotifyRecvStopMediaReplySignal(info);
}

MediaStreamType 
SipMsgReceiver::ConvertToMediaStreamType(const QString &type) const {
  // note: StartMedia和StopMedia的消息中，小流使用了不同的名称
  if (type == "stream") {
    return kStreamType;
  }
  if (type == "smallStream" || type == "small_stream") {
    return kSmallStreamType;
  }
  if (type == "screen") {
    return kScreenType;
  }
  return kUnknowType;
}

void SipMsgReceiver::RecvSendMediaControlHandler(const SipMsgParser &parser, 
                                                 const QString &from) {
  MediaStatusMsg info;
  info.remote_uri = parser.GetURI();
  info.type = ConvertToMediaControlType(parser.GetType());
  info.send = parser.GetEnable() == "true";
  emit NotifyRecvRemoteMediaStatusSignal(info);
}

void SipMsgReceiver::RecvMediaControlInfoHandler(const SipMsgParser &parser,
                                                 const QString &from) {
  MediaControlMsg info;
  info.terminal_uri = parser.GetTerminalURI();
  info.type = ConvertToMediaControlType(parser.GetType());
  info.send = parser.GetEnable() == "true";
  emit NotifyRecvMediaControlInfoSignal(info);
}

MediaControlType 
SipMsgReceiver::ConvertToMediaControlType( const QString &type ) const {
  if (type == UiStateTypeAudio) {
    return kAudioControlType;
  }
  if (type == UiStateTypeVideo) {
    return kVideoControlType;
  }
  if (type == UiStateTypeScreen) {
    return kScreenControlType;
  }
  if (type == UiStateTypeRecord) {
    return kRecordControlType;
  }
  return kUnknowControlType;
}

void SipMsgReceiver::RecvQoSMsgHandler(const SipMsgParser &parser,
                                       const QString &from) {
  QString qos_type = parser.GetQosType();
  if (qos_type == "LoginResult") {
    //sucess表示允许，正在返回LoginPermission消息
    //failure表示失败，正在与qos服务器协商

  } else if (qos_type == "LoginPermission") {
    LoginPermissionInfo info;
    parser.GetLoginPermissionInfo(info);
    emit NotifyRecvQoSLoginPermissionSignal(info);

  } else if (qos_type == "LoginRejection") {
    LoginRejectionInfo info;
    parser.GetLoginRejectionInfo(info);
    emit NotifyRecvQoSLoginRejectionSignal(info);

  } else if (qos_type == "QoSServerInfo") {
    QoSServerInfo info;
    parser.GetQoSServerInfo(info);
    emit NotifyRecvQoSServerInfoSignal(info);
  }
}

void SipMsgReceiver::RecvSetSpeakerTerminalHandler(const SipMsgParser &parser, 
                                                   const QString &from) {
  QString speaker_uri = parser.GetTerminalURI();
  emit NotifyRecvPermitSpeakSignal(speaker_uri);
}

void SipMsgReceiver::RecvSpeakerTerminalInfoHandler(const SipMsgParser &parser,
                                                    const QString &from) {
  QString speaker_uri = parser.GetTerminalURI();
  emit NotifyRecvSpeakerTerminalSignal(speaker_uri);
}

void SipMsgReceiver::RecvForbidSpeakerHandler(const SipMsgParser &parser,
                                              const QString &from) {
  QString forbid_speak_uri = parser.GetTerminalURI();
  emit NotifyRecvForbidSpeakSignal(forbid_speak_uri);
}

void SipMsgReceiver::RecvSetChairmanTerminalHandler(const SipMsgParser &parser, 
                                                    const QString &from) {
  QString new_chairman_uri = parser.GetTerminalURI();
  emit NotifyRecvChairmanUpdateSignal(new_chairman_uri);
}

void SipMsgReceiver::RecvChairmanTerminalInfoHandler(const SipMsgParser &parser,
                                                     const QString &from) {
  QString chairman_uri = parser.GetTerminalURI();
  emit NotifyRecvChairmanSignal(chairman_uri);
}

void SipMsgReceiver::RecvShareScreenTerminalInfoHandler(const SipMsgParser &parser,
                                                        const QString &from) {
  QString shared_screen_uri = parser.GetTerminalURI();
  emit NotifyRecvSharedScreenTerminalSignal(shared_screen_uri);
}

void SipMsgReceiver::RecvShareScreenControlInfoHandler(const SipMsgParser &parser,
                                                       const QString &from) {
  QString terminal_uri = parser.GetTerminalURI();
  bool enable = parser.GetEnable() == "true";
  emit NotifyRecvSharedScreenControlSignal(terminal_uri, enable);
}

void SipMsgReceiver::RecvOnlineMessageInfoHandler(const SipMsgParser &parser, 
                                                  const QString &from) {
  QString type = parser.GetType();
  emit NotifyRecvOnlineMessageSignal(type, from);
}

void SipMsgReceiver::RecvRecordControlHandler(const SipMsgParser &parser, 
                                              const QString &from) {
  RecordControlMsg msg;
  msg.requester_uri = parser.GetURI();
  msg.target_vuri = parser.GetVirtualURI();
  msg.tx_uri = parser.GetTxURI();
  msg.requester_ip = parser.GetSenderIP();
  msg.transaction_id = parser.GetTransactionId();
  msg.type = parser.GetType().toInt();
  msg.port = parser.GetPort();
  emit NotifyRecvRecordControlSignal(msg, from);
}

void SipMsgReceiver::RecvRecordControlAckHandler(const SipMsgParser &parser, 
                                                 const QString &from) {
  RecordControlAckMsg msg;
  msg.requester_uri = parser.GetURI();
  msg.target_vuri = parser.GetVirtualURI();
  msg.tx_uri = parser.GetTxURI();
  msg.transaction_id = parser.GetTransactionId();
  msg.error_code = parser.GetErrorCode();
  emit NotifyRecvRecordControlAckSignal(msg, from);
}

void SipMsgReceiver::RecvFloorControlInfoHandler( const SipMsgParser &parser, const QString &from )
{
  FloorControlInfoPointer info(new FloorControlInfoDescription);
  parser.GetFloorControlInfo(info);
  emit NotifyRecvFloorControlInfoSignal(info);
}


