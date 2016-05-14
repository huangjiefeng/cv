#ifndef MONITOR_CONFIG_COMMON_H
#define MONITOR_CONFIG_COMMON_H

#include <QtCore/QtCore>

class MonitorConfigInfo
{
public:

int cpuUsage;
int totalMemory;
int availableMemory;
// 带宽的大小为KB/s
double netWorkInBandWidth;
double netWorkOutBandWidth;

#ifndef QT_NO_DATASTREAM
friend QDataStream& operator>>(QDataStream& in, MonitorConfigInfo& data)
{
	in >> data.cpuUsage >> data.totalMemory >> data.availableMemory >> data.netWorkInBandWidth >> data.netWorkOutBandWidth;
	return in;
}
friend QDataStream& operator<<(QDataStream& out, MonitorConfigInfo& data)
{
	out << data.cpuUsage << data.totalMemory << data.availableMemory << data.netWorkInBandWidth << data.netWorkOutBandWidth;
	return out;
}
#endif

};

#endif
