#include "daemon_controller.h"

#include <windows.h>

#include "CoolviewCommon.h"

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>

const char *procs[] = {
	DBUS_DAEMON_PROC_NAME,
	COOLVIEW_SIP_WRAPPER_RPOC_NAME,
	COOLVIEW_TELECONTROLLER_PROC_NAME,
	COOLVIEW_LOCAL_MONITOR_PROC_NAME,
	COOLVIEW_MONITOR_PROC_NAME,
	PROC_NETWORK_MEASURER_NAME,
	COOLVIEW_TX_MONITOR_NAME,
};


DaemonController::DaemonController(QObject *parent)
  : IDaemonController(parent)
{
  channel_dispatcher_proxy_ = nullptr;
  show_console_ = false;
	first_time_show_console_ = true;
}

DaemonController::~DaemonController()
{

}

void DaemonController::Initialize()
{
  // 不要在此初始化dbus proxy，可能导致CoolView与DBus通信失败
  QString app_dir = QCoreApplication::applicationDirPath();
  QString debug_path = app_dir + "\\debug";
  if(QFile::exists(debug_path)) {
    show_console_ = true;
  } else {
    show_console_ = false;
  }
}

void DaemonController::StartDaemon()
{
	//TODO:应当用该类方法重写startReleativeServices()
  startReleativeServices();
  // TODO: 当CvConferenceRoom.exe丢失信号时，采用下面的临时办法解决
  //QThread::msleep(5000);
}

void DaemonController::StopDaemon()
{
	//TODO:应当用该类方法重写stopReleativeServices()
  stopReleativeServices();
}

void DaemonController::HandleShowConsoleSlot()
{
	if (first_time_show_console_) {
		channel_dispatcher_proxy_ = new ChannelDispatcherIf( 
			CHANNEL_DISPATCHER_SERVICE_NAME, 
			CHANNEL_DISPATCHER_OBJECT_PATH,
			QDBusConnection::sessionBus());

		//根据MSDN中ShowWindow API的说明，应用程序首次调用ShowWindow时，参数应当与
		//创建进程时STATUPINFO中指定的参数一致，故此处以初始状态调用一次。
		//实际测试发现，不执行这一步，第一次调用时有的控制台窗口显隐确实不正确。
		ChangeConsoleShowStatus(show_console_);
		first_time_show_console_ = false;
	}
	show_console_ = !show_console_;
	ChangeConsoleShowStatus(show_console_);
}

void DaemonController::ChangeConsoleShowStatus( bool show )
{
	// channeldispatcher和媒体流进程
	if (channel_dispatcher_proxy_) {
		QByteArray recvByteArray;
		QDataStream out( &recvByteArray , QIODevice::WriteOnly );
		out << show;
		channel_dispatcher_proxy_->ModifyChannel(
			"", "", MMT_ShowConsoleWindow, recvByteArray);
	}

	// 以下操作应该与startReleativeServices中启动的进程一致
	const int flag = show ? SW_SHOW : SW_HIDE;
	HWND console_window = NULL;

	for (int i = 0; i < sizeof(procs)/sizeof(char*); ++i) {
		const char * window_title = procs[i];
		console_window = FindWindowA(NULL, window_title); // 强制使用多字节版本
		if (NULL != console_window) {
			ShowWindow(console_window, flag);
			continue;
		}
		// 有的进程标题是完整路径
		const QString app_path = getProcessPath(procs[i]);
		console_window = FindWindowA(NULL, qPrintable(app_path)); // 强制使用多字节版本
		if (NULL != console_window) {
			ShowWindow(console_window, flag);
		}
	}
}
