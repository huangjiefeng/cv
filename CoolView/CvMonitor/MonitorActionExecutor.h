#ifndef MONITOR_ACTION_EXECUTOR_H
#define MONITOR_ACTION_EXECUTOR_H

class MonitoredProcess;
class MonitorActionExecutor
{
public:
	static int ExecuteMonitorAction( const MonitoredProcess* process , const unsigned int action );
};

#endif