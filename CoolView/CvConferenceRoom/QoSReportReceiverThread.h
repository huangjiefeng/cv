#pragma once
#include <QObject>
#include <QTimer>
#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

class UdpReceiver;
class QtConfrenceVideoWindow;

// TODO:应该共用CoolView的报告接收模块
// 必须使用MoveToThread将其移动到线程中

class QoSReportReceiverThread :public QObject
{
  Q_OBJECT

public:
	QoSReportReceiverThread();
	~QoSReportReceiverThread();

  void Initialize(const QString&bindIP , const int port);

	//阻塞等待接收数据
	void Start();

	//停止工作线程
	void Stop()
	{	
		_terminate = true;
	}


Q_SIGNALS:
  void SendUDPQoSReportSignal(const RtpStatItem &item);
  void SendUDPRecReportSignal(const RecStatItem &item);
  void StartSignal();

protected slots:
	void run();

private:
	UdpReceiver*	_udpServer;
	bool	_terminate;
};
