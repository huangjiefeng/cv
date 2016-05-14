#pragma  once

#define HD_VIDEO_CAPTURE_DEVICE "Logitech HD Webcam C525"
#define  DEFAULT_VIDEO_CAPTRUE_DEVICE "Logitech HD Webcam C525"    //视频捕获设备名称



void TestNormalVideo()
{
	const char * remoteAddr = "127.0.0.1";//目标地址
	const char*  remoteAddr2 = "211.66.86.101";//目标地址2
	int result;
	result = msdx_initial();
	msdx_config_set_video_device(HD_VIDEO_CAPTURE_DEVICE, "", "");

	result = msdx_create_send_graph(true);//打开小流
	assert(result==0);
	msdx_send_set_video_rtpstatcallback(NULL,printRtpStat);
	msdx_send_set_audio_rtpstatcallback(NULL,printRtpStat);
	result = msdx_set_send_video_vodec("H264", 1280,720 );
	//result = msdx_set_send_video_vodec("H264", 1280, 720);
	assert(result==0);
//	result = msdx_set_send_audio_codec("SPEEX", 16000, 1, 16);
//	assert(result==0);

	assert( msdx_connect_send_graph()==0);
	result = msdx_add_send_address(remoteAddr, 8500, remoteAddr, 8600);


	assert(msdx_run_send_graph() ==0);


	getch();
	result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8600 );
	//msdx_delete_send_address( remoteAddr2 , 8500, remoteAddr2, 8600 );

	getch();

	result = msdx_delete_send_address( remoteAddr , 8500 , remoteAddr , 8600 );

	getch();

	result = msdx_delete_send_address( remoteAddr2 , 8500, remoteAddr2, 8600 );
	
	getch();

	msdx_destroy_send_graph();
	msdx_uninitial();
}