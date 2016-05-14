
#include <QtCore/QCoreApplication>
#include <dbus/dbusTest/serviceTestIf.h>
#include "KinectTest.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	KinectTest test;
	test.start();
	return a.exec();
}
