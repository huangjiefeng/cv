#ifndef VOD_SERVICE_WRAPPER_H
#define VOD_SERVICE_WRAPPER_H

#include <QObject>

#include <memory>

class CvTxWebServiceImpl;
class ILogger;

//该类用于将SOAP对象用QObject包装，以便在单独的QThread中执行
class TxWebServiceWrapper : public QObject
{
  Q_OBJECT

public:
  TxWebServiceWrapper(ILogger *logger, QObject *parent = nullptr);
  ~TxWebServiceWrapper();

  //停止服务，会短时间阻塞-线程安全的
  void StopService();

public Q_SLOTS:
  //调用此方法会阻塞！请将类实例放在单独线程中，并通过信号槽调用
  void HandleStartServiceSlot(int port);

private:
  std::shared_ptr<CvTxWebServiceImpl> service_;
  ILogger *logger_;
};

#endif // VOD_SERVICE_WRAPPER_H
