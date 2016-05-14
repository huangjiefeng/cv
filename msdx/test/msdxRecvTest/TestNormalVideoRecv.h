#pragma once

#include "Util.h"
void TestNormalVideoRecv()
{
	//初始化
	int result;
	result = msdx_initial();
	int graphid;


	int videoWidth = 320;
	int videoHeight = 240;

	const char* localAddress = "127.0.0.1";

	//创建大流的接受端
	graphid = createRecvGraph( videoWidth ,videoHeight , 10, 8500,8600 );
	//CDXutil::AddToRot(graphid, &result);
	getch();

	//销毁接收
	destroyRecvGraph( graphid );

	//结束
	msdx_uninitial();
}