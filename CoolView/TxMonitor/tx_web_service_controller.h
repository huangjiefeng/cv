#ifndef VOD_SERVICE_CONTROLLER_H
#define VOD_SERVICE_CONTROLLER_H

#include <QObject>
#include <QThread>

#include <memory>


class TxWebServiceWrapper;
class ILogger;

class TxWebServiceController : public QObject
{
  Q_OBJECT

public:
  TxWebServiceController(QObject *parent = nullptr);
  ~TxWebServiceController();

  void Initialize();

Q_SIGNALS:
  void NotifyStartServiceSignal(int port);

private:
  std::shared_ptr<TxWebServiceWrapper> service_;
  QThread service_thread_;

  ILogger *logger_;
};

#endif // VOD_SERVICE_CONTROLLER_H
