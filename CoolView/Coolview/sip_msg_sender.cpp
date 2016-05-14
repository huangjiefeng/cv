#include "sip_msg_sender.h"

#include <QtCore/QtCore>
#include <winsock2.h>

#include <dbus/sipwrapper/common/SipWrapperCommonService.h>
#include <dbus/sipwrapper/client/CvSipWrapperIf.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

#include "sip_msg_builder.h"

SipMsgSender::SipMsgSender(QObject *parent)
    : IMsgSender(parent) {
  sip_wrapper_proxy_ = nullptr;
}

SipMsgSender::~SipMsgSender() {
  if (sip_wrapper_proxy_) {
    delete sip_wrapper_proxy_;
    sip_wrapper_proxy_ = nullptr;
  }
}

void SipMsgSender::Initialize() {
  sip_wrapper_proxy_ = new CvSipWrapperIf(
    SIP_WRAPPER_SERVICE_NAME, 
    SIP_WRAPPER_OBJECT_PATH, 
    QDBusConnection::sessionBus());
}

void SipMsgSender::SendTerminalLoginInfoSlot(const TerminalLoginParam &param) {
  // 清空sipwrapper
  sip_wrapper_proxy_->terminate();

  bool use_ipv6 = IsIpv6Address(param.ip_address);
  sip_wrapper_proxy_->init(use_ipv6);

  sip_wrapper_proxy_->addVirtualLine(
    param.display_name,
    param.username,
    param.identity,
    param.password,
    param.realm,
    param.proxy_server,
    param.register_server,
    use_ipv6 ? AF_INET6 : AF_INET);

  username_ = param.username;

  SipMsgBuilder msg_builder("Register");
  msg_builder.AddArgument("mac", param.mac);
  regist_content_ = msg_builder.GetMsg();

  sip_wrapper_proxy_->registerVirtualLine(param.username, regist_content_);

  ip_addr_ = param.ip_address;
  global_service_ = param.global_service;
}

bool SipMsgSender::IsIpv6Address( const QString &ip ) const {
  // TODO: 完善检查
  return ip.contains(":");
}

void SipMsgSender::SendTerminalInfoForSAVASlot(const TerminalSAVAParam &param) {
  SipMsgBuilder msg_builder("TerminalLogin");
  msg_builder.AddArgument("ip", ip_addr_);
  msg_builder.AddArgument("ipv4", param.ipv4_addr);
  msg_builder.AddArgument("ipv6", param.ipv6_addr);
  msg_builder.AddArgument("version", param.terminal_version);
  msg_builder.AddArgument("screenCount", param.screen_count);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::SendRegisterHeartBeatToSipServerSlot() {
  sip_wrapper_proxy_->registerVirtualLine(username_, regist_content_);
}

void SipMsgSender::SendOnlineHeartBeatToKeepLoginSlot() {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", QString("heartbeat"));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::SendOnlineTestSlot() {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", QString("test"));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::SendTerminalLogoutInfoSlot() {
  SipMsgBuilder msg_builder("TerminalLogout");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
  // TODO: 临时方案
  // 暂时不知道为什么SipWrapper执行了startConference但是没有发送TerminalLogout
  // 通过Sleep，解决sipwrapper在退出时没有发送TerminalLogOut消息的bug
  Sleep(500);
  sip_wrapper_proxy_->removeVirtualLine(username_, true);
  sip_wrapper_proxy_->terminate();
}

void SipMsgSender::SendGetTerminalConfigSlot()
{
  SipMsgBuilder msg_builder("GetTerminalConfig");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::SendReportTerminalConfigSlot( const ConfigDict &param )
{
  SipMsgBuilder msg_builder("ReportTerminalConfig");
  for (auto it = param.begin(); it != param.end(); ++it) {
    msg_builder.AddParameter(it.key(), it.value());
  }
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::RequestConferenceListSlot() {
  SipMsgBuilder msg_builder("GetConfList");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::RequestFloorControlInfoSlot(const QString &cid) {
  SipMsgBuilder msg_builder("GetFCS");
  msg_builder.AddArgument("cid", cid);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->startConference(username_, global_service_, content);
}

void SipMsgSender::SendConferenceJoinInfoSlot( 
    const ConferenceJoinParam &param, 
    const SDPInfo &sdp_info) {
  SipMsgBuilder msg_builder("qos", QString("Login"));
  msg_builder.AddArgument("cid", param.cid);
  msg_builder.AddArgument("focusuri", param.focus_uri);
  msg_builder.AddArgument("uri", param.uri);
  msg_builder.AddArgument("minRate", param.min_rate);
  msg_builder.AddArgument("maxRate", param.max_rate);
  msg_builder.AddArgument("ip", param.ip_addr);
  msg_builder.AddArgument("gateway", param.gateway);
  msg_builder.AddArgument(
    "virtualTerminalCount", param.virtual_terminal_count);
  QString content = msg_builder.GetMsg();

  QByteArray output_bytes;
  QDataStream out(&output_bytes, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_4);
  out << sdp_info;
  sip_wrapper_proxy_->makeJoinConfInvite(
    username_, param.focus_uri, content, output_bytes);
}

void SipMsgSender::SendOnlineHeartBeatToKeepInConferenceSlot(
    const QString &conference_uri) {
  SipMsgBuilder msg_builder("Online");
  msg_builder.AddArgument("type", QString("heartbeat"));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::SendConferenceLeaveInfoSlot( const QString &conference_uri ) {
  sip_wrapper_proxy_->exitConference(conference_uri);
}

void SipMsgSender::RequestTerminalListSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetTerminalList");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::QueryMainSpeakerSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetSpeakerTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::QueryChairmanSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetChairmanTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::QueryShareScreenSlot( const QString &conference_uri ) {
  SipMsgBuilder msg_builder("GetScreenShareTerminal");
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::SendHandUpMsgSlot(const QString &conference_uri,
                                     const QString &local_uri, 
                                     bool handup ) {
  SipMsgBuilder msg_builder(handup ? "HandUp" : "HandDown");
  msg_builder.AddArgument("terminalUri", local_uri);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::SendAllowSpeakMsgSlot(const QString &conference_uri,
                                         const QString &terminal_uri, 
                                         bool allow) {
  SipMsgBuilder msg_builder(allow ? "SetSpeaker" : "ForbidSpeaker");
  msg_builder.AddArgument("terminalUri", terminal_uri);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(conference_uri, content);
}

void SipMsgSender::RequestStartMediaSlot(const MediaRelayParam &param) {
  SipMsgBuilder msg_builder("StartMedia");
  msg_builder.AddArgument("uri", param.remote_uri);
  msg_builder.AddArgument("virtualUri", param.remote_vuri);
  msg_builder.AddArgument("ip", param.local_ip_addr);
  msg_builder.AddArgument("type", ConvertToQString(param.type, true));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

void SipMsgSender::ReplyStartMediaRequestSlot(const MediaReplyParam &param) {
  SipMsgBuilder msg_builder("StartMediaRelayReply");
  msg_builder.AddArgument("uri", param.remote_uri);
  msg_builder.AddArgument("virtualUri", param.local_vuri);
  msg_builder.AddArgument("permission", param.permission);
  msg_builder.AddArgument("type", ConvertToQString(param.type, true));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

void SipMsgSender::RequestStopMediaSlot(const MediaRelayParam &param) {
  SipMsgBuilder msg_builder("StopMedia");
  msg_builder.AddArgument("uri", param.remote_uri);
  msg_builder.AddArgument("virtualUri", param.remote_vuri);
  msg_builder.AddArgument("ip", param.local_ip_addr);
  msg_builder.AddArgument("type", ConvertToQString(param.type, false));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

void SipMsgSender::ReplyStopMediaRequestSlot(const MediaReplyParam &param) {
  SipMsgBuilder msg_builder("StopMediaRelayReply");
  msg_builder.AddArgument("uri", param.remote_uri);
  msg_builder.AddArgument("virtualUri", param.local_vuri);
  msg_builder.AddArgument("permission", param.permission);
  msg_builder.AddArgument("type", ConvertToQString(param.type, false));
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

QString SipMsgSender::ConvertToQString( MediaStreamType type, bool start ) const {
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

void SipMsgSender::SendScreenShareControlSlot(const ScreenControlParam &param) {
  SipMsgBuilder msg_builder("ShareScreenControl");
  msg_builder.AddArgument("terminalUri", param.terminal_uri);
  msg_builder.AddArgument("ip", param.ip_addr);
  msg_builder.AddArgument("port", param.ppt_port);
  msg_builder.AddArgument("enable", param.enable);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

void SipMsgSender::SendMediaControlInfoSlot(const MediaControlParam &param) {
  SipMsgBuilder msg_builder("SendMediaControl");
  msg_builder.AddArgument("uri", param.local_uri);
  msg_builder.AddArgument("type", ConvertToQString(param.type));
  msg_builder.AddArgument("enable", param.enable);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

QString SipMsgSender::ConvertToQString( MediaControlType type ) const {
  switch (type) {
    case kAudioControlType:
      return UiStateTypeAudio; break;
    case kVideoControlType:
      return UiStateTypeVideo; break;
    case kScreenControlType:
      return UiStateTypeScreen; break;
    case kRecordControlType:
      return UiStateTypeRecord; break;
    default:
      return UiStateTypeAny; break;
  }
}

void SipMsgSender::SendRecordControlSlot( const RecordControlParam &param ) {
  SipMsgBuilder msg_builder("RecordControl");
  msg_builder.AddArgument("uri", param.requester_uri);
  msg_builder.AddArgument("ip", param.requester_ip);
  msg_builder.AddArgument("virtualUri", param.target_vuri);
  msg_builder.AddArgument("txUri", param.tx_uri);
  msg_builder.AddArgument("transactionId", param.transaction_id);
  msg_builder.AddArgument("type", param.type);
  msg_builder.AddArgument("port", param.port);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}

void SipMsgSender::SendRecordControlAckSlot( const RecordControlAckParam &param ) {
  SipMsgBuilder msg_builder("RecordControlAck");
  msg_builder.AddArgument("uri", param.requester_uri);
  msg_builder.AddArgument("virtualUri", param.target_vuri);
  msg_builder.AddArgument("txUri", param.tx_uri);
  msg_builder.AddArgument("transactionId", param.transaction_id);
  msg_builder.AddArgument("errorCode", param.error_code);
  QString content = msg_builder.GetMsg();
  sip_wrapper_proxy_->makeInfoCall(param.conference_uri, content);
}


