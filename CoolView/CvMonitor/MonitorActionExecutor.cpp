#include "MonitorActionExecutor.h"
#include <dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h>
#include "util/ProcessManager.h"
#include <QtCore/QtCore>
#include <log/Log.h>
bool openExe( const QString& name , QString dir/*=""*/ )
{
	if( dir=="" )
	{
		dir = QCoreApplication::applicationDirPath();
	}

	dir.replace("/" , "\\");
	if( !dir.endsWith("\\") )
	{
		dir.append("\\");
	}

	QString path = dir + name;
	int result;

	//如果存在debug文件则
	QString appDir = QCoreApplication::applicationDirPath();
	QString debugPath = appDir + "/debug";
	QFile debugFile( debugPath );
	if( QFile::exists( debugPath)) 
	{
		result = (int)ShellExecute(NULL , TEXT("open") , path.toStdWString().c_str() , 
			NULL ,dir.toStdWString().c_str(),  SW_SHOW);
	}else
	{
		result = (int)ShellExecute(NULL , TEXT("open") , path.toStdWString().c_str() , 
			NULL ,dir.toStdWString().c_str(),  SW_HIDE);
	}


	if( result> 32 )
	{
		return true;
	}else
	{
		return false;
	}

}


int MonitorActionExecutor::ExecuteMonitorAction( const MonitoredProcess* process , const unsigned int action )
{
	switch( action )
	{
	case MonitorTriggerAction::RECOVER_PROCESS:
		{
			//恢复指定进程
			sprintf_s( __global_msg , sizeof(__global_msg) , "Process[%s] Recover Action Triggered" , qPrintable(process->_processName) );
			MONITOR_LOG_INFO(__global_msg);

			//首先关闭僵死的进程
			QString processName = process->_processName;
			if( process->_processName.indexOf(".exe") <0 )
			{
				processName = processName + ".exe";
			}
			if( ProcessManager::isRunning(processName.toStdWString().c_str()) )
			{
				sprintf_s( __global_msg , sizeof(__global_msg) , "Process[%s] Still Running .Kill it!" , qPrintable(process->_processName) );
				MONITOR_LOG_INFO(__global_msg);
				ProcessManager::killProcess( processName.toStdWString().c_str() );
			}

			//然后开启对应的进程
			return openExe( processName , process->_processDirPath );
		};break;
	case MonitorTriggerAction::RESTART_COMPUTOR:
		{
			qDebug("Trigger Action [Restart Computer]" );

			sprintf_s( __global_msg , sizeof(__global_msg) , "Process[%s] Restart Computer Triggered" , qPrintable(process->_processName) );
			MONITOR_LOG_INFO(__global_msg);

			return 1;
		};break;
	case MonitorTriggerAction::NO_ACTION:
		return 0;
	default:
		return 0;
	}
}