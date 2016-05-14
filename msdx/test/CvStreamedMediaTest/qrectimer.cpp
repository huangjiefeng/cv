#include "qrectimer.h"
#include <qcoreevent.h>

QRecTimerHandler::QRecTimerHandler(QObject *parent)
	: QObject(parent)
{
	moveToThread(&thread);
	timer.moveToThread(&thread);
	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
	QObject::connect(this, SIGNAL(start(int)), &timer, SLOT(start(int)));
	QObject::connect(this, SIGNAL(stop()), &timer, SLOT(stop()));
}

QRecTimerHandler::~QRecTimerHandler()
{
	timer.stop();
}

void QRecTimerHandler::startTimer( int msec )
{
	thread.start();
	emit start(msec);
}

void QRecTimerHandler::stopTimer()
{
	emit stop();
}

