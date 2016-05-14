#ifndef SIP_MSG_MANAGER_H
#define SIP_MSG_MANAGER_H

#include "msg_manager_interface.h"

class SipMsgManager : public IMsgManager {
  Q_OBJECT

public:
  SipMsgManager(QObject *parent);
  virtual ~SipMsgManager();

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
  IMsgSender *sender_;
  IMsgReceiver *receiver_;
  bool has_initialized_;
};

#endif // SIP_MSG_MANAGER_H
