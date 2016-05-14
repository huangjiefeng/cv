#include "sip_msg_manager.h"

#include <cassert>

#include "sip_msg_sender.h"
#include "sip_msg_receiver.h"

SipMsgManager::SipMsgManager(QObject *parent)
    : IMsgManager(parent) {
  sender_ = nullptr;
  receiver_ = nullptr;
  has_initialized_ = false;
}

SipMsgManager::~SipMsgManager() {
}

void SipMsgManager::Initialize() {
  assert(!has_initialized_);
  has_initialized_ = true;
  
  IMsgSender::RegisterMetaType();
  SipMsgSender *sip_msg_sender = new SipMsgSender(this);
  sip_msg_sender->Initialize();
  sender_ = sip_msg_sender;

  IMsgReceiver::RegisterMetaType();
  SipMsgReceiver *sip_msg_receiver = new SipMsgReceiver(this);
  sip_msg_receiver->Initialize();
  receiver_ = sip_msg_receiver;
}
