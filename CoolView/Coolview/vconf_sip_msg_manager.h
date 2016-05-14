#ifndef VCONF_SIP_MSG_MANAGER_H
#define VCONF_SIP_MSG_MANAGER_H

#include "msg_manager_interface.h"
#include "vconf_sip_msg_sender.h"
#include "vconf_sip_msg_receiver.h"

class VConfSipMsgManager : public IMsgManager {
  Q_OBJECT

public:
  VConfSipMsgManager(QObject *parent);
  virtual ~VConfSipMsgManager();

  void Initialize();

  IMsgSender * GetMsgSender() override {
    return sender_;
  }

  IMsgSender * GetMsgSender() const override {
    return sender_;
  }

  IMsgReceiver * GetMsgReceiver() override {
    return receiver_;
  }

  IMsgReceiver * GetMsgReceiver() const override {
    return receiver_;
  }

private:
  bool has_initialized_;
  VConfSipMsgSender *sender_;
  VConfSipMsgReceiver *receiver_;
};

#endif // VCONF_SIP_MSG_MANAGER_H
