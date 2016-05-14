#include "sip_controller.h"

#include <ws2def.h>

#include <QDebug>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include <dbus/sipwrapper/client/CvSipWrapperIf.h>

#include "sip_msg_builder.h"

#include "profile/RunningProfile.h"

using std::mem_fn;

SipController::SipController(QObject *parent)
    : QObject(parent) {
  sip_wrapper_proxy_ = nullptr;
}

SipController::~SipController() {
  if (sip_wrapper_proxy_) {
    delete sip_wrapper_proxy_;
  }
}

void SipController::Initialize() {
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

void SipController::RegistHandlers() {
  type_handler_.insert(
    "TerminalLoginNotify", 
    mem_fn(&SipController::RecvTerminalLoginNotifyHandler));

  type_handler_.insert(
    "ConfList", mem_fn(&SipController::RecvConferenceListHandler));

  type_handler_.insert(
    "TerminalList", mem_fn(&SipController::RecvTerminalListHandler));

  type_handler_.insert(
    "LoginNotify", mem_fn(&SipController::RecvTerminalLoginHandler));

  type_handler_.insert(
    "loginnotify", mem_fn(&SipController::RecvTerminalLoginHandler));

  type_handler_.insert(
    "LogoutNotify", mem_fn(&SipController::RecvTerminalLogoutHandler));

  type_handler_.insert(
    "logoutnotify", mem_fn(&SipController::RecvTerminalLogoutHandler));

  type_handler_.insert(
    "HandUp", mem_fn(&SipController::RecvTerminalHandUpHandler));

  type_handler_.insert(
    "HandDown", mem_fn(&SipController::RecvTerminalHandDownHandler));

  type_handler_.insert(
    "StartMediaReply", mem_fn(&SipController::RecvStartMediaReplyHandler));

  type_handler_.insert(
    "StartMediaRelay", mem_fn(&SipController::RecvStartMediaRelayHandler));

  type_handler_.insert(
    "StopMediaReply", mem_fn(&SipController::RecvStopMediaReplyHandler));

  type_handler_.insert(
    "StopMediaRelay", mem_fn(&SipController::RecvStopMediaRelayHandler));

  type_handler_.insert(
    "SendMediaControl", mem_fn(&SipController::RecvSendMediaControlHandler));

  type_handler_.insert(
    "MediaControl", mem_fn(&SipController::RecvMediaControlInfoHandler));

  type_handler_.insert("qos", mem_fn(&SipController::RecvQoSMsgHandler));

  type_handler_.insert(
    "SetSpeakerTerminal", 
    mem_fn(&SipController::RecvSetSpeakerTerminalHandler));

  type_handler_.insert(
    "GetSpeakerTerminal",
    mem_fn(&SipController::RecvSpeakerTerminalInfoHandler));

  type_handler_.insert(
    "ForbidSpeaker", mem_fn(&SipController::RecvForbidSpeakerHandler));

  type_handler_.insert(
    "SetChairmanTerminal", 
    mem_fn(&SipController::RecvSetChairmanTerminalHandler));

  type_handler_.insert(
    "GetChairmanTerminal", 
    mem_fn(&SipController::RecvChairmanTerminalInfoHandler));

  type_handler_.insert(
    "GetScreenShareTerminal", 
    mem_fn(&SipController::RecvShareScreenTerminalInfoHandler));

  type_handler_.insert(
    "ShareScreenControl",
    mem_fn(&SipController::RecvShareScreenControlInfoHandler));

  type_handler_.insert(
    "Online",
    mem_fn(&SipController::RecvOnlineMessageInfoHandler));
    
}

void SipController::HandleMessageReceivedNotifySlot(const QString &msg,
                                                    const QString &from) {
  SipMsgParser parser;
  if (!parser.Init(msg)) {
    qDebug() << "Receive broken sip message";
    return;
  }

  QString command = parser.GetCommandType();
  HandlerDictionary::iterator it = type_handler_.find(command);
  if (it != type_handler_.end()) {
    it.value()(*this, parser, from);
  } else {
    qDebug() << "unknown sip command: " << command;
  }
}

void SipController::RecvTerminalLoginNotifyHandler(const SipMsgParser &parser, 
                                                   const QString &from) {
  QString valid = parser.GetResult();
  emit NotifyRecvSAVAResultSignal(valid == "true");
}

void SipController::RecvConferenceListHandler(const SipMsgParser &parser,
                                              const QString &from) {
  ConferenceList conferences;
  parser.GetConferenceList(conferences);
  emit NotifyRecvConferenceListSignal(conferences);
}

void SipController::RecvTerminalListHandler(const SipMsgParser &parser, 
                                            const QString &from) {
  TerminalList terminals;
  parser.GetTerminalList(terminals);
  emit NotifyRecvTerminalListSignal(terminals);
}

void SipController::RecvTerminalLoginHandler(const SipMsgParser &parser, 
                                             const QString &from) {
  TerminalList terminals;
  parser.GetLoginTerminals(terminals);
  if (!terminals.empty())
    emit NotifyRecvTerminalLoginSignal(terminals);
}

void SipController::RecvTerminalLogoutHandler(const SipMsgParser &parser,
                                              const QString &from) {
  QString logout_uri = parser.GetURI();
  emit NotifyRecvTerminalLogoutSignal(logout_uri);
}

void SipController::RecvTerminalHandUpHandler(const SipMsgParser &parser,
                                              const QString &from) {
  QString hand_up_uri = parser.GetTerminalURI();
  emit NotifyRecvTerminalHandUpSignal(hand_up_uri);
}

void SipController::RecvTerminalHandDownHandler(const SipMsgParser &parser, 
                                                const QString &from) {
  QString hand_down_uri = parser.GetTerminalURI();
  emit NotifyRecvTerminalHandDownSignal(hand_down_uri);
}

void SipController::RecvStartMediaRelayHandler(const SipMsgParser &parser,
                                               const QString &from) {
  QString remote_uri = parser.GetURI();
  QString remote_ip = parser.GetReceiverIP();
  QString local_vuri = parser.GetVirtualURI();
  QString type = parser.GetType();

  emit NotifyRecvStartMediaRelaySignal(
    remote_uri, remote_ip, local_vuri, ConvertToMediaStreamType(type));
}

void SipController::RecvStartMediaReplyHandler(const SipMsgParser &parser, 
                                               const QString &from) {
  QString remote_uri = parser.GetURI();
  QString type = parser.GetType();
  bool permission = parser.GetPermission() == "true";
  QString remote_virtual_uri = parser.GetVirtualURI();
  if (remote_virtual_uri.isEmpty()) {
    remote_virtual_uri = SipMsgParser::ConstructDefaultVirtualURI(remote_uri);
  }
  emit NotifyRecvMediaReplySignal(
    remote_virtual_uri, ConvertToMediaStreamType(type), permission);
}

void SipController::RecvStopMediaRelayHandler(const SipMsgParser &parser, 
                                              const QString &from) {
  QString remote_uri = parser.GetURI();
  QString remote_ip = parser.GetReceiverIP();
  QString local_vuri = parser.GetVirtualURI();
  QString type = parser.GetType();
  emit NotifyRecvStopMediaRelaySignal(
    remote_uri, remote_ip, local_vuri, ConvertToMediaStreamType(type));
}

void SipController::RecvStopMediaReplyHandler(const SipMsgParser &parser,
                                              const QString &from) {
  QString remote_uri = parser.GetURI();
  QString type = parser.GetType();
  bool permission = parser.GetPermission() == "true";
  QString remote_virtual_uri = parser.GetVirtualURI();
  if (remote_virtual_uri.isEmpty()) {
    remote_virtual_uri = SipMsgParser::ConstructDefaultVirtualURI(remote_uri);
  }
  emit NotifyRecvStopMediaReplySignal(
    remote_virtual_uri, ConvertToMediaStreamType(type), permission);
}

void SipController::RecvSendMediaControlHandler(const SipMsgParser &parser, 
                                                const QString &from) {
  QString remote_uri = parser.GetURI();
  QString type = parser.GetType();
  bool send = parser.GetEnable() == "true";
  emit NotifyRecvSendMediaControlSignal(
    remote_uri, ConvertToMediaControlType(type), send);
}

void SipController::RecvMediaControlInfoHandler(const SipMsgParser &parser,
                                                const QString &from) {
  QString uri = parser.GetTerminalURI();
  QString type = parser.GetType();
  bool enable = parser.GetEnable() == "true";
  emit NotifyRecvMediaControlInfoSignal(
    uri, ConvertToMediaControlType(type), enable);
}

void SipController::RecvQoSMsgHandler(const SipMsgParser &parser,
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

void SipController::RecvSetSpeakerTerminalHandler(const SipMsgParser &parser, 
                                                  const QString &from) {
  QString speaker_uri = parser.GetTerminalURI();
  emit NotifyRecvPermitSpeakSignal(speaker_uri);
}

void SipController::RecvSpeakerTerminalInfoHandler(const SipMsgParser &parser,
                                                   const QString &from) {
  QString speaker_uri = parser.GetTerminalURI();
  emit NotifyRecvSpeakerTerminalSignal(speaker_uri);
}

void SipController::RecvForbidSpeakerHandler(const SipMsgParser &parser,
                                             const QString &from) {
  QString forbid_speak_uri = parser.GetTerminalURI();
  emit NotifyRecvForbidSpeakSignal(forbid_speak_uri);
}

void SipController::RecvSetChairmanTerminalHandler(const SipMsgParser &parser, 
                                                   const QString &from) {
  QString new_chairman_uri = parser.GetTerminalURI();
  emit NotifyRecvChairmanUpdateSignal(new_chairman_uri);
}

void SipController::RecvChairmanTerminalInfoHandler(const SipMsgParser &parser,
                                                    const QString &from) {
  QString chairman_uri = parser.GetTerminalURI();
  emit NotifyRecvChairmanSignal(chairman_uri);
}

void SipController::RecvShareScreenTerminalInfoHandler(const SipMsgParser &parser,
                                                       const QString &from) {
  QString shared_screen_uri = parser.GetTerminalURI();
  emit NotifyRecvSharedScreenTerminalSignal(shared_screen_uri);
}

void SipController::RecvShareScreenControlInfoHandler(const SipMsgParser &parser,
                                                      const QString &from) {
  QString uri = parser.GetTerminalURI();
  bool enable = parser.GetEnable() == "true";
  emit NotifyRecvSharedScreenControlSignal(uri, enable);
}

void SipController::RecvOnlineMessageInfoHandler(const SipMsgParser &parser, 
                                                 const QString &from) {
  if (from == global_service_) {
    QString type = parser.GetType();
    emit NotifyRecvOnlineMessageSignal(type);
  }
}

//////////////////////////////////////////////////////////////////////////

void SipController::SendTerminalLoginInfoSlot(const TerminalLoginInfo &info) {
  // 清空sipwrapper
  sip_wrapper_proxy_->terminate();

  bool use_ipv6 = IsIpv6Address(info.ip_address);
  sip_wrapper_proxy_->init(use_ipv6);

  sip_wrapper_proxy_->addVirtualLine(
    info.display_name,
    info.username,
    info.identity,
    info.password,
    info.realm,
    info.proxy_server,
    info.register_server,
    use_ipv6 ? AF_INET6 : AF_INET);

  username_ = info.username;

  SipMsgBuilder msg_builder("Register");
  msg_builder.AddArgument("mac", info.mac);
  regist_content_ = msg_builder.GetMsg();

  sip_wrapper_proxy_->registerVirtualLine(info.username, regist_content_);

  ip_addr_ = info.ip_address;
  global_service_ = "sip:globalService@";
  global_service_ += info.realm;
}

bool SipController::IsIpv6Address( const QString &ip ) const {
  // TODO: 完善检查
  return ip.contains(":");
}

void SipController::SendTerminalInfoForSAVASlot(const TerminalSAVAInfo &info) {
  SipMsgBuilder msg_builder("TerminalLogin");
  msg_builder.AddArgument("ip", ip_addr_);
  msg_builder.AddArgument("ipv4", info.ipv4_addr);
  msg_builder.AddArgument("ipv6", info.ipv6_addr);
  msg_builder.AddArgument("version", info.terminal_version);
  msg_builder.AddArgument("screenCount", info.screen_count);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipController::SendRegisterHeartBeatToSipServerSlot() {
  sip_wrapper_proxy_->registerVirtualLine(username_, regist_content_);
}

void SipController::SendOnlineHeartBeatToKeepLoginSlot() {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", "heartbeat");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipController::SendOnlineTestSlot() {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", "test");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipController::SendTerminalLogoutInfoSlot() {
  SipMsgBuilder msg_builder("TerminalLogOut");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);

  sip_wrapper_proxy_->removeVirtualLine(username_, true);
  sip_wrapper_proxy_->terminate();
}

void SipController::RequestConferenceListSlot() {
  SipMsgBuilder msg_builder("GetConfList");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipController::SendConferenceJoinInfoSlot( 
    const ConferenceJoinInfo &conference_info, 
    const SDPInfo &sdp_info) {
  SipMsgBuilder msg_builder("qos", "Login");
  msg_builder.AddArgument("cid", conference_info.cid);
  msg_builder.AddArgument("focusuri", conference_info.focus_uri);
  msg_builder.AddArgument("uri", conference_info.uri);
  msg_builder.AddArgument("minRate", conference_info.min_rate);
  msg_builder.AddArgument("maxRate", conference_info.max_rate);
  msg_builder.AddArgument("ip", conference_info.ip_addr);
  msg_builder.AddArgument("gateway", conference_info.gateway);
  msg_builder.AddArgument(
    "virtualTerminalCount", 
    conference_info.virtual_terminal_count);
  QString content = msg_builder.GetMsg();

  QByteArray output_bytes;
  QDataStream out(&output_bytes, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_4);
  out << sdp_info;
  sip_wrapper_proxy_->makeJoinConfInvite(
    username_, conference_info.focus_uri, content, output_bytes);
}

void SipController::SendOnlineHeartBeatToKeepInConferenceSlot(
    const QString &conference_uri ) {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", "heartbeat");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::SendConferenceLeaveInfoSlot( const QString &conference_uri ) {
  sip_wrapper_proxy_->exitConference(conference_uri);
}

void SipController::RequestTerminalListSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetTerminalList");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::QueryMainSpeakerSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetSpeakerTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::QueryChairmanSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetChairmanTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::QueryShareScreenSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetScreenShareTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::SendHandUpMsgSlot(const QString &conference_uri,
                                      const QString &local_uri, 
                                      bool handup ) {
  SipMsgBuilder msg_builder(handup ? "HandUp" : "HandDown");
  msg_builder.AddArgument("terminalUri", local_uri);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::SendAllowSpeakMsgSlot(const QString &conference_uri,
                                          const QString &terminal_uri, 
                                          bool allow) {
  SipMsgBuilder msg_builder(allow ? "SetSpeaker" : "ForbidSpeaker");
  msg_builder.AddArgument("terminalUri", terminal_uri);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::RequestStartMediaSlot(const QString &conference_uri,
                                          const QString &remote_uri, 
                                          const QString &remote_vuri,
                                          const QString &local_ip_addr, 
                                          MediaStreamType type) {
  SipMsgBuilder msg_builder("StartMedia");
  msg_builder.AddArgument("uri", remote_uri);
  msg_builder.AddArgument("virtualUri", remote_vuri);
  msg_builder.AddArgument("ip", local_ip_addr);
  msg_builder.AddArgument("type", ConvertToQString(type, true));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::ReplyStartMediaRequestSlot(const QString &conference_uri, 
                                               const QString &remote_uri, 
                                               const QString &local_vuri, 
                                               MediaStreamType type, 
                                               bool permission) {
  if (conference_uri.isEmpty() || remote_uri.isEmpty()) {
    return;
  }

  SipMsgBuilder msg_builder("StartMediaRelayReply");
  msg_builder.AddArgument("uri", remote_uri);
  msg_builder.AddArgument("virtualUri", local_vuri);
  msg_builder.AddArgument("permission", permission);
  msg_builder.AddArgument("type", ConvertToQString(type, true));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::RequestStopMediaSlot(const QString &conference_uri,
                                         const QString &remote_uri, 
                                         const QString &remote_vuri, 
                                         const QString &local_ip_addr, 
                                         MediaStreamType type) {
  SipMsgBuilder msg_builder("StopMedia");
  msg_builder.AddArgument("uri", remote_uri);
  msg_builder.AddArgument("virtualUri", remote_vuri);
  msg_builder.AddArgument("ip", local_ip_addr);
  msg_builder.AddArgument("type", ConvertToQString(type, false));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::ReplyStopMediaRequestSlot(const QString &conference_uri,
                                              const QString &remote_uri, 
                                              const QString &local_vuri, 
                                              MediaStreamType type,
                                              bool permission) {
  SipMsgBuilder msg_builder("StopMediaRelayReply");
  msg_builder.AddArgument("uri", remote_uri);
  msg_builder.AddArgument("virtualUri", local_vuri);
  msg_builder.AddArgument("permission", permission);
  msg_builder.AddArgument("type", ConvertToQString(type, false));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

QString SipController::ConvertToQString( MediaStreamType type, bool start ) const {
  // note: StartMedia和StopMedia的消息中，小流使用了不同的名称
  switch (type) {
    case kStreamType:
      return "stream"; break;
    case kSmallStreamType:
      return start ? "smallStream" : "small_stream"; break;
    case kScreenType:
      return "screen"; break;
    default:
      return ""; break;
  }
}

MediaStreamType
SipController::ConvertToMediaStreamType(const QString &type) {
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
  return MediaStreamType::kUnknowType;
}

void SipController::SendScreenShareControlSlot(const QString &conference_uri,
                                               const QString &ip, 
                                               const QString &ppt_port, 
                                               bool enable) {
  SipMsgBuilder msg_builder("ShareScreenControl");
  msg_builder.AddArgument("ip", ip);
  msg_builder.AddArgument("port", ppt_port);
  msg_builder.AddArgument("enable", enable);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipController::SendMediaControlInfoSlot(const QString &conference_uri,
                                             const QString &local_uri,
                                             MediaControlType type,
                                             bool enable) {
  SipMsgBuilder msg_builder("SendMediaControl");
  msg_builder.AddArgument("uri", local_uri);
  msg_builder.AddArgument("type", ConvertToQString(type));
  msg_builder.AddArgument("enable", enable);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

QString SipController::ConvertToQString( MediaControlType type ) const {
  switch (type) {
  case kAudioControlType:
    return "audio"; break;
  case kVideoControlType:
    return "video"; break;
  default:
    return ""; break;
  }
}

MediaControlType 
SipController::ConvertToMediaControlType( const QString &type ) const {
  if (type == "audio") {
    return kAudioControlType;
  }
  if (type == "video") {
    return kVideoControlType;
  }
  return MediaControlType::kUnknowControlType;
}

