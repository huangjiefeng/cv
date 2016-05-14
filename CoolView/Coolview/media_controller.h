#ifndef MEDIA_CONTROLLER_H
#define MEDIA_CONTROLLER_H

#include <QObject>

#include "stream_receiver_info_manager.h"

class TerminalManager;

class MediaController : public QObject {
  Q_OBJECT

public:
  MediaController(QObject *parent);
  ~MediaController();

private:
  //TerminalManager &terminals_;
  //StreamReceiverInfoManager receivers_;
};

#endif // MEDIA_CONTROLLER_H
