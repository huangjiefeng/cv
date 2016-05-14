#include "CoolviewCommon.h"

#include <windows.h>
#include <shellapi.h>

#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/performance/process/client/CvPerfProcessIf.h>
#include <dbus/performance/process/common/PerformanceProcessCommon.h>
#include <dbus/telecontroller/client/TelecontrollerIf.h>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
#include <dbus/performance/monitor/client/CvPerfMonitorIf.h>
#include <dbus/performance/monitor/common/CvPerfMonitorServiceCommon.h>

#include "util/ProcessManager.h"

using std::string;
using std::vector;

void TransformStringVectorToQStringList( const vector<string>* stringVector , QStringList* qStringList)
{
	qStringList->clear();

	for (vector<string>::const_iterator it = stringVector->begin(); it!=stringVector->end(); it++ )
	{
		QString item = QString::fromLocal8Bit( (*it).c_str() );
		*qStringList << item;
	}
}


void ResizeWidgetInMiddle( QWidget* widget )
{
	QRect rect = QApplication::desktop()->availableGeometry();
	int x_pos = ( rect.width() - widget->width() ) /2;
	int y_pos = ( rect.height() - widget->height() )/2;
	widget->setGeometry( x_pos , y_pos , widget->width() , widget->height() );
}

bool openExe( const QString& name , QString dir/*=""*/ , QString param/*=""*/  )
{
	STARTUPINFOA si;     
	ZeroMemory(&si, sizeof(si));     
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;

	//如果存在debug文件则
	QString appDir = QCoreApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	QString debugPath = appDir + "\\debug";
	QFile debugFile( debugPath );
	if( QFile::exists( debugPath)) {
		si.wShowWindow = SW_SHOW;
	} else {
		si.wShowWindow = SW_HIDE;
	}

	if( dir.isEmpty() ) {
		dir = appDir + "\\";
	}
	QString cmd = dir + name + " " + param;
	char cmdLine[1024] = {0};
	strcpy_s(cmdLine, qPrintable(cmd));

	PROCESS_INFORMATION pi;    
	DWORD flag = HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE; 
	// 设置STARTUPINFO中的dwFlags和wShowWindow来实现控制台初始隐藏
	// 这样在必要时候可以调出控制台调试。直接设置CREATE_NO_WINDOW
	// 或使用ShellExecute将导致不创建控制台，不便于调试。 
	// --by Liaokz，2014-1
	bool res = CreateProcessA(NULL, cmdLine, NULL, NULL, NULL, flag,
		NULL, // 继承父进程环境变量
		qPrintable(dir), // 使用其自己的目录作为当前文件夹
		&si, &pi);
	return res;

	/*int result = 0;
	{
		result = (int)ShellExecute(NULL , TEXT("open") , path.toStdWString().c_str() , 
			param.toStdWString().c_str(), dir.toStdWString().c_str(),  SW_SHOW);
	}else
	{
		result = (int)ShellExecute(NULL , TEXT("open") , path.toStdWString().c_str() , 
			param.toStdWString().c_str(), dir.toStdWString().c_str(),  SW_HIDE);
	}
	if( result> 32 )
	{
		return true;
	}else
	{
		return false;
	}*/
}



//dbus session....zhenHua.sun 2010-10-07
int startDBusService()
{
	if( !ProcessManager::isRunning(TEXT(DBUS_DAEMON_PROC_NAME) ) )
	{
		QString appDir = QApplication::applicationDirPath();
		appDir.replace("/" , "\\" );

		QString dbusDir= appDir + "\\dbus\\bin\\";
		bool result = openExe( DBUS_DAEMON_PROC_NAME, dbusDir, "--session" );

		//等待dbus-daemon启动
		int retryTimes = 4;
		while( !ProcessManager::isRunning(TEXT(DBUS_DAEMON_PROC_NAME)) && retryTimes>0 )
		{
			Sleep(1000);
			retryTimes--;
		}	
		
		if (  ProcessManager::isRunning(TEXT(DBUS_DAEMON_PROC_NAME)) )
			return 1;
		else
			return -1;
	}else
	{
		return 1;
	}
}

int startMsdxSvr()
{
	int result = 1;
	if( !ProcessManager::isRunning(TEXT(COOLVIEW_MSDX_PROC_NAME)))
	{
		QString appDir = QApplication::applicationDirPath();
		appDir.replace("/" , "\\" );
		appDir = appDir + "\\module_4_4\\";
		result = openExe( COOLVIEW_MSDX_PROC_NAME , appDir );
	}else
	{
		////如果CvChannelDispatcher在运行，说明Coolview异常退出了。应该销毁channelDispatcher的资源
		//ChannelDispatcherIf channelDispatcherProxy( CHANNEL_DISPATCHER_SERVICE_NAME , CHANNEL_DISPATCHER_OBJECT_PATH ,
		//	QDBusConnection::sessionBus() );
		//channelDispatcherProxy.Destroy();
	}
	//设置ChannelDispatcher的CPU优先级
	ProcessManager::setProcessPriority(TEXT(COOLVIEW_PROC_NAME),Priority_RealTime );
	return result;
}

int startSipWrapper()
{
	int result = 0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_SIP_WRAPPER_RPOC_NAME)))
	{
		stopSipWrapper();
	}
	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );	
	appDir = appDir + "\\";
	return openExe( COOLVIEW_SIP_WRAPPER_RPOC_NAME, appDir );
	//return openExe( COOLVIEW_SIP_WRAPPER_RPOC_NAME );
}

void stopSipWrapper()
{
	CvPerfProcessIf  processProxy( __GetCvPerfProcessServiceName(CV_SIPWRAPPER_PROC_NAME) ,
		__GetCvPerfProcessObjectPath(CV_SIPWRAPPER_PROC_NAME) , QDBusConnection::sessionBus()) ;
	processProxy.ExitProcess();
}

int startTelecontroller( const QString& terminalName)
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_TELECONTROLLER_PROC_NAME)))
	{
		return true;
	}

	return openExe( COOLVIEW_TELECONTROLLER_PROC_NAME  );
}

void stopTelecontroller()
{
	TelecontrollerIf proxy( _GetTelecontrollerServiceName("sock"),_GetTelecontrollerObjectPath("sock"),
		QDBusConnection::sessionBus());
	QByteArray output_array;
	proxy.TeleInfo( TELE_ExitProcess , -1, output_array ); 
	//关闭Telecontroller
	//ProcessManager::killProcess( TEXT(COOLVIEW_TELECONTROLLER_PROC_NAME) );

}

int startUpgrader()
{
	int result = 0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_UPGRADER_PROC_NAME)))
	{
		ProcessManager::killProcess(TEXT(COOLVIEW_UPGRADER_PROC_NAME) );
	}

	return openExe( COOLVIEW_UPGRADER_PROC_NAME);
}

int startLocalMonitor()
{
	int result = 0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_LOCAL_MONITOR_PROC_NAME)) )
	{
		ProcessManager::killProcess( TEXT(COOLVIEW_LOCAL_MONITOR_PROC_NAME) );
	}
	return openExe( COOLVIEW_LOCAL_MONITOR_PROC_NAME );
}

int startJabberProc()
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_JABBER_PROC_NAME)))
	{
		ProcessManager::killProcess( TEXT(COOLVIEW_JABBER_PROC_NAME) );
	}
	return openExe( COOLVIEW_JABBER_PROC_NAME );
}


int startConferenceRoom() 
{
	////启动msdx，如果已经启动了，那么则会清空所有的媒体资源
	//startMsdxSvr();

	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_CONFERENCE_ROOM_PROC_NAME)))
	{
		return 1;
	}

	//QString appDir = QApplication::applicationDirPath();
	//appDir.replace("/" , "\\" );	
	//appDir = appDir + "\\module_4_4\\";
	//return openExe( COOLVIEW_CONFERENCE_ROOM_PROC_NAME , appDir );
	return openExe(COOLVIEW_CONFERENCE_ROOM_PROC_NAME);
}

int startNetworkMeasurerProcess()
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(PROC_NETWORK_MEASURER_NAME)))
	{
		return 1;
	}
	//openExe( PROC_NETWORK_MEASURER_NAME );
	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );	
	QString dstDir = appDir + "\\";
	QString dstPath = dstDir + "\\" + PROC_NETWORK_MEASURER_NAME;
	ShellExecuteA(NULL , "open" ,dstPath.toStdString().c_str() , "Network Measurer" ,appDir.toStdString().c_str(), SW_HIDE);

	//向CvMonitor注册消息
	MonitoredProcess process;
	process._processName = "NetworkMeasurer";
	process._monitorInterval = 3000;	//睡眠600s，即10分钟
	QString dirPath = QCoreApplication::applicationDirPath();
	dirPath.replace("/" , "\\" );
	process._processDirPath = dirPath;
	process.AddTrigger( Trigger::PROCESS_ALIVE ,0 , MonitorTriggerAction::RECOVER_PROCESS );
	QByteArray output_array;
	QDataStream out(&output_array , QIODevice::WriteOnly );
	out.setVersion(QDataStream::Qt_4_4);
	out << process;

	CvPerfMonitorIf proxy(PERFORMANCE_MONITOR_SERVICE_NAME , PERFORMANCE_MONITOR_OBJECT_PATH,
		QDBusConnection::sessionBus() );
	proxy.RegisterProcess( output_array );

	return 1;
}

int startCvMonitor()
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_MONITOR_PROC_NAME)))
	{
		ProcessManager::killProcess( TEXT(COOLVIEW_MONITOR_PROC_NAME) );
	}
	return openExe( COOLVIEW_MONITOR_PROC_NAME );
}

void stopCvMonitor()
{
	ProcessManager::killProcess( TEXT(COOLVIEW_MONITOR_PROC_NAME));
}

int startKinectDaemon()
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_KINECT_PROC_NAME) ) )
	{
		ProcessManager::killProcess( TEXT(COOLVIEW_KINECT_PROC_NAME)  );
	}
	return openExe( COOLVIEW_KINECT_PROC_NAME );
}

void stopKinectDaemon()
{
	ProcessManager::killProcess( TEXT(COOLVIEW_KINECT_PROC_NAME)  );
}

int startTxMonitor()
{
	int result =0;
	if( ProcessManager::isRunning(TEXT(COOLVIEW_TX_MONITOR_NAME)))
	{
		ProcessManager::killProcess( TEXT(COOLVIEW_TX_MONITOR_NAME) );
	}
	return openExe( COOLVIEW_TX_MONITOR_NAME );
}

void stopTxMonitor()
{
	ProcessManager::killProcess( TEXT(COOLVIEW_TX_MONITOR_NAME));
}

int startWeblibUploader()
{
  return 0;
	int result =0;
	if( ProcessManager::isRunning(TEXT(WEBLIB_UPLOADER_NAME)))
	{
		ProcessManager::killProcess( TEXT(WEBLIB_UPLOADER_NAME) );
	}
	return openExe( WEBLIB_UPLOADER_DIR "\\" WEBLIB_UPLOADER_NAME );
}

void stopWeblibUploader()
{
	ProcessManager::killProcess( TEXT(WEBLIB_UPLOADER_NAME));
}

void stopBfcpClient()
{
  if (ProcessManager::isRunning(TEXT(COOLVIEW_BFCP_CLIENT))) 
  {
    ProcessManager::killProcess(TEXT(COOLVIEW_BFCP_CLIENT));
  }
}

/**
 * @brief 开启Coolview所需的相关进程
 */
int startReleativeServices()
{

	//清空channelDispatcher生成的临时进程文件
	QDir appDir(QCoreApplication::applicationDirPath());
	QStringList exeFiles = appDir.entryList( QDir::Files|QDir::Executable);

	int result =0;
	for( int i=0 ; i<exeFiles.size() ; i++ )
	{
		QString fileName = exeFiles.at(i);
		if( fileName.indexOf("CvStreamedMedia_")!=-1 )
			result = QFile::remove( fileName );
	}

	//DBusService
	result = startDBusService();
	if( result<=0 )
	{
		QMessageBox::critical(
			NULL , QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("打开DBUS时发生错误！") ,
			QMessageBox::Ok);
		return result;
	}

	//msdxsvr
	//result = startMsdxSvr();
	//if( result<=0 )
	//{
	//	QMessageBox::critical(
	//		NULL , QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("开启ChannelDispatcher时发生错误！") ,
	//		QMessageBox::Ok);
	//	return result;
	//}

	//开启sip模块
	result = startSipWrapper();
	if( result<=0 )
	{
		QMessageBox::critical(
			NULL , QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("开启SipWrapper时发生错误！") ,
			QMessageBox::Ok);
		return result;
	}

	//telecontroller
	result = startTelecontroller();
	if( result<=0 )
	{
		QMessageBox::critical(
			NULL , QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("开启Telecontroller时发生错误！") ,
			QMessageBox::Ok);
		return result;
	}

	//result = startJabberProc();

	//远程更新不是必须的组件，因此不对执行结果进行判断
	result = startUpgrader();
	result = startLocalMonitor();	
	result = startConferenceRoom();

	//启动网络测量进程
	startNetworkMeasurerProcess();
	//if( result<0 )
	//{
	//	QMessageBox::critical(
	//		NULL , QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("开启Coolview更新程序时发生错误！") ,
	//		QMessageBox::Ok);
	//	return result;
	//}

	// Tx录制监控进程
	result = startTxMonitor();

	ProcessManager::setProcessPriority( TEXT(COOLVIEW_PROC_NAME ) , Priority_High);
	return 1;
}

/**
 * @brief 关闭Coolview相关的所有进程
 */
void stopReleativeServices()
{

	////关闭SIP模块
	//ProcessManager::killProcess( TEXT(COOLVIEW_SIP_WRAPPER_RPOC_NAME ) );
	stopSipWrapper();

	//关闭LocalMonitor
	ProcessManager::killProcess( TEXT(COOLVIEW_LOCAL_MONITOR_PROC_NAME ) );

	//关闭远程更新
	ProcessManager::killProcess( TEXT(COOLVIEW_UPGRADER_PROC_NAME) );

    //关闭Bfcp客户端
    stopBfcpClient();

	//关闭Telecontroller
	stopTelecontroller();

	//关闭监控进程
	stopCvMonitor();

	// Tx_temp 关闭Tx监控进程
	stopTxMonitor();

	//关闭会议室
	ProcessManager::killProcess( TEXT(COOLVIEW_CONFERENCE_ROOM_PROC_NAME) );

	//关闭channelDispatcher
	//ProcessManager::killProcess( TEXT(COOLVIEW_MSDX_PROC_NAME) );

	//关闭CvMessenger
	ProcessManager::killProcess( TEXT(COOLVIEW_JABBER_PROC_NAME ));

	//关闭NetworkMeasurer进程
	ProcessManager::killProcess( TEXT(PROC_NETWORK_MEASURER_NAME));

	//关闭dbus，必须保证最后关闭
	//ProcessManager::killProcess(TEXT("dbus-daemon.exe"));
	//ProcessManager::killProcess(TEXT("dbus-monitor.exe"));

}

QString getProcessPath( const QString &proc_name )
{
  QString appPath = QCoreApplication::applicationDirPath();
  appPath.replace("/", "\\");
  if (DBUS_DAEMON_PROC_NAME == proc_name)
  {
    appPath += "\\dbus\\bin\\";
  } 
  else
  {
    appPath += "\\";
  }
  appPath += proc_name;
  return appPath;
}


