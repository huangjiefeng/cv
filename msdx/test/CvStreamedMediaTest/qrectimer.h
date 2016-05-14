#ifndef QRECTIMER_H
#define QRECTIMER_H

#include <QThread>
#include <QTimer>

class QRecTimerHandler : public QObject
{
	Q_OBJECT

public:
	QRecTimerHandler(QObject *parent = NULL);
	~QRecTimerHandler();

signals:
	void start(int msec);
	void stop();

public slots:
	void timerEvent();
	void startTimer(int msec);
	void stopTimer();

private:
	QTimer timer;
	QThread thread;

};

#endif // QRECTIMER_H
