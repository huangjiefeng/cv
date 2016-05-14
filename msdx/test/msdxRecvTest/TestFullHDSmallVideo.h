#ifndef TEST_FULLHD_SMALL_VIDEO_RECV_H
#define TEST_FULLHD_SMALL_VIDEO_RECV_H

#include "Util.h"

void TestFullHDSmallVideoRecv()
{
	//初始化
	int result;
	result = msdx_initial();
	int graphid;


	int videoWidth = 1920;
	int videoHeight = 1080;
	int fps = 25;

	const char* localAddress = "127.0.0.1";

	//创建大流的接受端
	graphid = createRecvGraph( videoWidth ,videoHeight , fps, 8500,8600 );
	//CDXutil::AddToRot(graphid, &result);
	getch();


	//销毁接收端
	destroyRecvGraph( graphid );

	//创建小流的接收端
	graphid = createRecvGraph( 320, 180 , fps/2, 8500,8700 );
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