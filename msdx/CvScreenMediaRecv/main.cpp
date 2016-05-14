#include <QtCore/QCoreApplication>
#include <log/Log.h>
#include "CvScreenMediaService.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//更改log文件名
	SetLogComponent("screen_recv" );

	LOG_DEBUG( "启动ScreenMedia[%s]" , RECV_SCREEN_MEDIA_ID );
	//注册服务到DBUS
	CvScreenMediaService service( RECV_SCREEN_MEDIA_ID );
	//CvScreenMediaService service( "testrecv" );			//测试用

	return a.exec();
}
