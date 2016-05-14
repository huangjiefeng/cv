#ifndef MEDIA_MONITOR_THREAD_H
#define MEDIA_MONITOR_THREAD_H

#include <QtCore/QtCore>
#include <QVector>
#include "PerformanceUtil.h"

class MediaMonitoredProcess;
class MediaMonitorThread : public QThread
{
	Q_OBJECT
public:
	MediaMonitorThread();

	volatile bool EndMark() const { return _endMark; }
	void EndMark(volatile bool val) { _endMark = val; }

	void addMediaMonitoredProcess(MediaMonitoredProcess& mediaProcess);
	void removeMediaMonitoredProcess(int &processID );

protected:
	void run();

private:
	
	int searchMeidaMonitoredProcess(int & processid);
	
	QVector<MediaMonitoredProcess*> mediaMonitoredProcessList;
	
	PerformanceUtil		_perforUtil;
	
	volatile bool		_endMark;

};


#endif