#include "vconf_sip_msg_manager.h"

#include <cassert>

#include "sip_msg_sender.h"
#include "sip_msg_receiver.h"

VConfSipMsgManager::VConfSipMsgManager(QObject *parent)
  : IMsgManager(parent) {
    sender_ = nullptr;
    receiver_ = nullptr;
    has_initialized_ = false;
}

VConfSipMsgManager::~VConfSipMsgManager() {
}

void VConfSipMsgManager::Initialize() {
  assert(!has_initialized_);
  has_initialized_ = true;

  MediaHelper::RegisterMetaType();
  TerminalHelper::RegisterMetaType();
  ConferenceHelper::RegisterMetaType();

  receiver_ = new VConfSipMsgReceiver(this);
  receiver_->Initialize();
  sender_ = new VConfSipMsgSender(this);
  sender_->Initialize();

  //使用QueuedConnection，可保证发送调用完全结束后，下一次线程循环才执行接收，模拟更真实，也避免了不必要的问题
  connect(sender_, &VConfSipMsgSender::NotifyVRequestConferenceListSignal,
    receiver_, &VConfSipMsgReceiver::HandleVRecvConferenceListSlot, Qt::QueuedConnection);
  connect(sender_, &VConfSipMsgSender::NotifyVSendConferenceJoinInfoSignal,
    receiver_, &VConfSipMsgReceiver::HandleVRecvQoSLoginPermissionSlot, Qt::QueuedConnection);
  connect(sender_, &VConfSipMsgSender::NotifyVRequestStartMediaSlot,
    receiver_, &VConfSipMsgReceiver::HandleVRecvStartMediaReplySlot, Qt::QueuedConnection);
}
