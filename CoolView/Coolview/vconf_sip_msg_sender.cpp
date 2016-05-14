#include "vconf_sip_msg_sender.h"

#include "util\ini\CVIniConfig.h"

VConfSipMsgSender::VConfSipMsgSender(QObject *parent)
  : SipMsgSender(parent)
{

}

VConfSipMsgSender::~VConfSipMsgSender()
{

}

void VConfSipMsgSender::Initialize()
{
  IMsgSender::RegisterMetaType();
  SipMsgSender::Initialize();
}

void VConfSipMsgSender::RequestConferenceListSlot()
{
  SipMsgSender::RequestConferenceListSlot();

  // 在发出获取会议列表请求的同时先显示本地模拟会议，不然如果终端未加入任何会议，
  // 服务器不会返回会议列表，也就无法触发刷新会议列表动作了
  if (CVIniConfig::getInstance().isVirtualConferenceEnable()) {
    emit NotifyVRequestConferenceListSignal();
  }
}

void VConfSipMsgSender::SendConferenceJoinInfoSlot( 
  const ConferenceJoinParam &param, 
  const SDPInfo &sdp_info) {
  if (ConferenceHelper::IsVirtualConference(param.focus_uri)) {
    emit NotifyVSendConferenceJoinInfoSignal();
  } else {
    SipMsgSender::SendConferenceJoinInfoSlot(param, sdp_info);
  }
}

void VConfSipMsgSender::RequestTerminalListSlot( const QString &conference_uri )
{
  if (!ConferenceHelper::IsVirtualConference(conference_uri)) {
    SipMsgSender::RequestTerminalListSlot(conference_uri);
  }
}

void VConfSipMsgSender::SendConferenceLeaveInfoSlot( const QString &conference_uri )
{
  if (!ConferenceHelper::IsVirtualConference(conference_uri)) {
    SipMsgSender::SendConferenceLeaveInfoSlot(conference_uri);
  }
}

void VConfSipMsgSender::RequestStartMediaSlot( const MediaRelayParam &param )
{
  if (!ConferenceHelper::IsVirtualConference(param.conference_uri)) {
    SipMsgSender::RequestStartMediaSlot(param);
  } else {
    emit NotifyVRequestStartMediaSlot(param);
  }
}
