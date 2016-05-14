#ifndef MONITOR_THREAD_H
#define MONITOR_THREAD_H

#include <QtCore/QtCore>
#include "PerformanceUtil.h"
class MonitoredProcess;
class MonitorThread : public QThread
{
	Q_OBJECT
public:
	MonitorThread( MonitoredProcess* monitoredProcess );

	volatile bool EndMark() const { return _endMark; }
	void EndMark(volatile bool val) { _endMark = val; }

protected:
	void run();

	MonitoredProcess*	_monitoredProcess;

	volatile bool		_endMark;
	PerformanceUtil		_perforUtil;

};


#endif