#include <QtCore/QCoreApplication>
#include "KinectService.h"
 
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	KinectService::getInstance();
	return a.exec();
}
