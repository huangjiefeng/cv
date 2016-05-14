#include <QtWidgets/QApplication>
#include <log/Log.h>
#include "CvScreenMediaService.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//更改log文件名
	SetLogComponent("screen_send" );

	LOG_DEBUG( "启动ScreenMedia[%s]" ,SEND_SCREEN_MEDIA_ID );
	//注册服务到DBUS
	CvScreenMediaService service( SEND_SCREEN_MEDIA_ID );
	//CvScreenMediaService service( "testsend" );			//测试用

	return a.exec();
}
