#include <QtCore/QCoreApplication>
#include <log/Log.h>

#include "CvPerfMonitorService.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	strcpy(Log::_logFileName , "CvMonitor");
	CvPerfMonitorService monitorService;

	return a.exec();
}
