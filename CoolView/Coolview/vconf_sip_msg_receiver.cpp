#include "vconf_sip_msg_receiver.h"

#include "sip_msg_parser.h"
#include "msg_sender_interface.h"

VConfSipMsgReceiver::VConfSipMsgReceiver(QObject *parent)
  : SipMsgReceiver(parent)
{

}

VConfSipMsgReceiver::~VConfSipMsgReceiver()
{

}

void VConfSipMsgReceiver::Initialize()
{
  IMsgReceiver::RegisterMetaType();
  SipMsgReceiver::Initialize();
}

void VConfSipMsgReceiver::RecvConferenceListHandler( 
  const SipMsgParser &parser, const QString &from )
{
  ConferenceList conferences;
  parser.GetConferenceList(conferences);
  AddVirtualConferenceToList(conferences); //加入虚拟会议
  emit NotifyRecvConferenceListSignal(conferences);
}

void VConfSipMsgReceiver::HandleVRecvConferenceListSlot()
{
  ConferenceList local_conferences;
  AddVirtualConferenceToList(local_conferences);
  emit NotifyRecvConferenceListSignal(local_conferences);
}

void VConfSipMsgReceiver::AddVirtualConferenceToList(ConferenceList &conference)
{
  ConferencePointer local_play_conf = ConferenceHelper::GetVodPlayVirtualConference();
  conference.push_back(local_play_conf);
}

void VConfSipMsgReceiver::HandleVRecvQoSLoginPermissionSlot()
{
  LoginPermissionInfo info;
  info.dscp = 0;
  info.rate = 0;
  info.user_uri = ""; //没用到
  emit NotifyRecvQoSLoginPermissionSignal(info);
}

void VConfSipMsgReceiver::HandleVRecvTerminalListSlot(const TerminalList& terminals)
{
  emit NotifyRecvTerminalListSignal(terminals);
}

void VConfSipMsgReceiver::HandleVRecvStartMediaReplySlot(const MediaRelayParam &param)
{
  MediaReplyMsg info;
  info.remote_uri = param.remote_uri;
  info.type = param.type;
  info.permission = true;
  info.remote_vuri = param.remote_vuri;
  if (info.remote_vuri.isEmpty()) {
    info.remote_vuri = 
      TerminalHelper::ConstructDefaultVirtualURI(info.remote_uri);
  }
  emit NotifyRecvStartMediaReplySignal(info);
}

