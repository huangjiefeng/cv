#include <QtCore/QCoreApplication>
#include "CvStreamedMediaService.h"
#include <Windows.h>
#include <log/Log.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	char * mediaID = "test";
	char * randCode = "";

	if( argc>=2 )
	{
		mediaID = argv[1];
	}

    DWORD pid = GetCurrentProcessId();

	//设置console窗口题目，不要更改
	char consoleTitle[_MAX_PATH]={0};
    sprintf_s(consoleTitle, "%s(ID:%s, PID:%d)", 
        STREAMED_MEDIA_PROC_NAME,
        mediaID,
        pid);

	SetConsoleTitleA( consoleTitle );

	//更改log文件名
    QString log_comp = "CvStream_";
    log_comp += mediaID;
	SetLogComponent(log_comp.toLocal8Bit().constData());

	LOG_DEBUG("启动StreamedMedia[%s]" , mediaID );
	//注册服务到DBUS
	CvStreamedMedia service;

	int result = -1;
	if (service.Init(mediaID))
	{
		result = a.exec();
		qDebug("result:%d" , result );
	}

	return result;
}
