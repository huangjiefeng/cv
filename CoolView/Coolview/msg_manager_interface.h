#ifndef MSG_MANAGER_INTERFACE_H
#define MSG_MANAGER_INTERFACE_H

#include <QObject>

#include "msg_receiver_interface.h"
#include "msg_sender_interface.h"

class IMsgManager : public QObject {
  Q_OBJECT

public:
  IMsgManager(QObject *parent) : QObject(parent) {}
  virtual ~IMsgManager() {}

  virtual IMsgSender *GetMsgSender() = 0;
  virtual IMsgSender *GetMsgSender() const = 0;

  virtual IMsgReceiver *GetMsgReceiver() = 0;
  virtual IMsgReceiver *GetMsgReceiver() const = 0;
};

#endif // MSG_MANAGER_INTERFACE_H
