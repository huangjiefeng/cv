#include "MonitorConfig.h"
#include <QtCore/QCoreApplication>

MonitorConfig::MonitorConfig()
{
	QString appDir = QCoreApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	_configFilePath = appDir + "\\monitor.conf";


	//º”‘ÿ≈‰÷√
	loadConfig();
}

void MonitorConfig::saveConfig()
{

	QFile confFile(_configFilePath);
	QDataStream out( &confFile );
	if( confFile.open( QIODevice::WriteOnly ) )
	{
		out << _cpuUsage << _memoryUsage;
	}
}

void MonitorConfig::loadConfig()
{
	QFile confFile(_configFilePath);
	QDataStream in( &confFile );
	if( confFile.open( QIODevice::ReadOnly ) )
	{
		in >>_cpuUsage >> _memoryUsage;
	}
	
	if(_cpuUsage == 0)
	{
		_cpuUsage=90;
	}
	if(_memoryUsage == 0)
	{
		_memoryUsage=1800;
	}

}