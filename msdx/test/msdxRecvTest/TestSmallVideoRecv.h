#ifndef TEST_SMALL_VIDEO_RECV_H
#define TEST_SMALL_VIDEO_RECV_H

#include "Util.h"
#include <msdx/config.h>
void TestSmallVideoRecv()
{
	//初始化
	int result;
	result = msdx_initial();
	int graphid;
	int videoWidth;
	int videoHeight;
	int fps = 25;

	int choise;
	cout<<"请选择视频类型:\n1.高清(1280x720)\n2.全高清(1920x1080)";
	cin>>choise;

	if( choise==2 )
	{
		videoWidth = 1920;
		videoHeight = 1080;
	}else
	{
		videoWidth = 1280;
		videoHeight = 720;
	}

	const char* localAddress = "127.0.0.1";
	
	//创建大流的接受端
	graphid = createRecvGraph( videoWidth ,videoHeight , fps, 8500,8600 );
	//CDXutil::AddToRot(graphid, &result);
	getch();


	//销毁接收端
	destroyRecvGraph( graphid );

	//创建小流的接收端
	graphid = createRecvGraph( MSDX_CONF_SMALL_VIDEO_WIDTH, MSDX_CONF_SMALL_VIDEO_HEIGHT, fps/2, 8500,8700 );
	getch();

	//销毁接收端
	destroyRecvGraph( graphid );

	//重新接收大流
	graphid = createRecvGraph( videoWidth , videoHeight , fps, 8500, 8600 );
	getch();

	//销毁接收
	destroyRecvGraph( graphid );

	//结束
	msdx_uninitial();
}

#endif