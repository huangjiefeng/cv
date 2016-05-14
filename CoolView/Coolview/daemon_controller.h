#ifndef DAEMON_CONTROLLER_H
#define DAEMON_CONTROLLER_H

#include "daemon_controller_interface.h"

class ChannelDispatcherIf;

// DaemonController
// 负责后台进程的启动、停止等相关工作
class DaemonController : public IDaemonController
{
  Q_OBJECT

public:
  DaemonController(QObject *parent = nullptr);
  ~DaemonController();

  void Initialize();

  virtual void StartDaemon() override;
  virtual void StopDaemon() override;

public slots:
  void HandleShowConsoleSlot();

private:
	void ChangeConsoleShowStatus(bool show);

private:
  ChannelDispatcherIf *channel_dispatcher_proxy_;

  bool show_console_;
	bool first_time_show_console_;
};

#endif // DAEMON_CONTROLLER_H
