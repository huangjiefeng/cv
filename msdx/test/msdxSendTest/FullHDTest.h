#pragma  once

#define FULL_HD_VIDEO_CAPTURE_DEVICE "Timeleak hd capture device"
#define CROSS_BAR_DEVICE "HD70a BDA Analog Xbar Secondary"
#define CROSS_BAR_TYPE "Composite"


void FullHDTest()
{
	const char * remoteAddr = "202.38.254.146";//目标地址
	const char*  remoteAddr2 = "202.38.254.152";//目标地址2

	int result;
    result = msdx_initial();
    msdx_config_set_video_device(FULL_HD_VIDEO_CAPTURE_DEVICE,CROSS_BAR_DEVICE, CROSS_BAR_TYPE);

    result = msdx_create_send_graph();
	assert(result==0);
	msdx_send_set_video_rtpstatcallback(NULL,printRtpStat);
	msdx_send_set_audio_rtpstatcallback(NULL,printRtpStat);
	result = msdx_set_send_video_vodec("H264", 1920,1080 );
	//result = msdx_set_send_video_vodec("H264", 1280, 720);
	assert(result==0);
	result = msdx_set_send_audio_codec("SPEEX", 16000, 1, 16, false);
	assert(result==0);

	assert( msdx_connect_send_graph()==0);

	/*
	//发送两路大流的视频
    result = msdx_add_send_address(remoteAddr, 8500, remoteAddr, 8600);
	result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8600);
	*/
	//发送一大一小视频
	result = msdx_add_send_address(remoteAddr, 8500, remoteAddr, 8600);
	//result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8700,true);
	assert(msdx_run_send_graph() ==0);
	
	//msdx_delete_send_address( remoteAddr2 , 8500, remoteAddr2, 8600 );

	getch();

	
	assert( msdx_stop_send_graph()==0);

	assert( msdx_destroy_send_graph()==0);

    msdx_uninitial();
}