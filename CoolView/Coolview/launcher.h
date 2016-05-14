#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QThread>

class ControllerManager;
class UIManager;
class IMsgManager;
class CoreService2;
class HttpMsgManager;

class Launcher : public QObject {
  Q_OBJECT
public:
  Launcher(QObject *parent);
  ~Launcher();

  void Initialize();
  void Start();
  void Stop();

Q_SIGNALS:
  void StartSendStreamMediaSignal();
  void ReceiveAllLocalMediaSignal();

private:
  ControllerManager *controller_manager_;
  UIManager *ui_manager_;
  IMsgManager *msg_manager_;
  CoreService2 *core_service_;
  HttpMsgManager *http_msg_manager_;
  QThread controller_thread_;
  QThread msg_thread_;
  QThread core_service_thread_;
  bool use_multi_thread_;
};

#endif // LAUNCHER_H
