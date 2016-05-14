#ifndef CV_PERF_MONITOR_SERVICE_H
#define CV_PERF_MONITOR_SERVICE_H

#include <QtCore/QtCore>
#include <dbus/core/client/CvCoreIf.h>
#include <dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h>
#include <dbus/performance/monitor/common/MonitorConfigCommon.h>
#include "PerformanceUtil.h"


typedef QMap<QString , MonitoredProcess*>	MonitoredProcessMap;

class MediaMonitorThread;

class CvPerfMonitorService : public QObject
{
	Q_OBJECT
public:
	CvPerfMonitorService();
	~CvPerfMonitorService();

public Q_SLOTS: // METHODS
	void CreateTestMedia(const QString &media_id, const QByteArray &input_garray);
	void HelloInfo(int processID, const QByteArray &processImage);
	void RegisterProcess(const QByteArray &processInfo);
	void UnregisterProcess(int processID);
	void ReleaseTestMedia(const QString &media_id);
  
	void RegisterMediaProcess(const QByteArray &processInfo);
    void UnregisterMediaProcess(int processID);


	void UpgradeMonitorCofInfoSlot();
    void UpgradeMonitorAirServerSlot();

Q_SIGNALS: // SIGNALS
	void cvMonitorCofMessageReceivedEvent(const QByteArray &MonitorCofInfo);
private:
    bool RecoverAirServer();

	MonitoredProcessMap		_processMap;

	PerformanceUtil		_perforUtil;
	MonitorConfigInfo  _monitorComfigInfo;
	QTimer * _monitorCofInfoTimer;
	CvCoreIf *_coreProxy;
	MediaMonitorThread * _mediaMonitorThread;
    QTimer *_monitorAirServerTimer;
    bool _findAirServer;
};



#endif