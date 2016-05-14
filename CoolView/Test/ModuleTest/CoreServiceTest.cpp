#include "CoreServiceTest.h"
#include <dbus/core/common/CvCoreServiceCommon.h>
#include <dbus/core/client/CvCoreIf.h>
#include <QtDBus/QtDBus>
#include <iostream>
#include <Windows.h>
using namespace std;
void CoreServiceTest()
{
	CvCoreIf coreProxy( CVCORE_SERVICE_NAME,CVCORE_SERVICE_OBJECT_PATH,
		 QDBusConnection::sessionBus());
	
	QString UserSip = "scut_t_xzh2@sip.ccnl.scut.edu.cn";
	int ScreenIndex = 0;
	int Seet = 1;
	QByteArray SubData;
	QDataStream SubOut(&SubData, QIODevice::WriteOnly );
	SubOut << UserSip << ScreenIndex << Seet;

	/*
	cout << "测试接收音视频之后立马释放媒体流" << endl;
	//接收音视频
	coreProxy.TeleCommand(CoreAction_StartRecvStreamedMedia,SubData);


	//取消接收音视频
	coreProxy.TeleCommand(CoreAction_StopRecvStreamedMedia,SubData);

	getchar();
	*/

	//接收音视频流
	coreProxy.TeleCommand(CoreAction_StartRecvStreamedMedia,SubData);

	getchar();

	const int initialTimes = 5;
	int tryTimes = initialTimes;
	while(true)
	{
		while(tryTimes>0 )
		{
			cout << "测试释放媒体流之后立马接收音视频" <<endl;

			coreProxy.TeleCommand(CoreAction_StopRecvStreamedMedia,SubData);
			coreProxy.TeleCommand(CoreAction_StartRecvStreamedMedia,SubData);
			tryTimes--;

			Sleep(100);
		}

		tryTimes = initialTimes*2;
		getchar();
	}
}