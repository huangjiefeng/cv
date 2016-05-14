#include "test.h"

Test::Test()
{
	qDebug() << "===========test init===========";
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(changeTest()));
}

QString Test::testStart()
{
	qDebug() << "+++++++QtDBus test++++++++++";
	QString tmp;
	tmp = QString("OFF");
	qDebug() << tmp;
	if (timer->isActive()) {
		timer->stop();
	}
	timer->start(2000);
	return tmp;
}

void Test::changeTest()
{
	QString tmp;
	tmp = QString("ON");
	qDebug() << "+++++++++++++++++++" << tmp;
	emit stateChange(tmp);
}
