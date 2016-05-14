#ifndef MONITOR_CONFIG_H
#define MONITOR_CONFIG_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDataStream>

class	MonitorConfig
{
public:
	static MonitorConfig* getInstance()
	{
		static MonitorConfig config;
		return &config;
	}

	int CpuUsage() const { return _cpuUsage; }
	void CpuUsage(int val) { _cpuUsage = val; }

	int MemoryUsage() const { return _memoryUsage; }
	void MemoryUsage(int val) { _memoryUsage = val; }

	void saveConfig();
	void loadConfig();


private:
	MonitorConfig();
	~MonitorConfig(){};

	QString			_configFilePath;

	int _cpuUsage;

	int _memoryUsage;

};

#endif