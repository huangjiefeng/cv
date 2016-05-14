#pragma  once

#define HD_VIDEO_CAPTURE_DEVICE "Logitech HD Webcam C270"
#define  DEFAULT_VIDEO_CAPTRUE_DEVICE "USB2.0 UVC PC Camera"    //视频捕获设备名称
//#define HD_VIDEO_CAPTURE_DEVICE "e2eSoft VCam"
//#define  DEFAULT_VIDEO_CAPTRUE_DEVICE "e2eSoft VCam"    //视频捕获设备名称
#define USAGE "application name  -remote ip -camera name(可选)"

void SunTestCase()
{
	const char * remoteAddr = "202.38.254.146";//目标地址
	const char*  remoteAddr2 = "202.38.254.152";//目标地址2

	int result;
    result = msdx_initial();
    msdx_config_set_video_device(HD_VIDEO_CAPTURE_DEVICE, "", "");

    result = msdx_create_send_graph();
	assert(result==0);
	msdx_send_set_video_rtpstatcallback(NULL,printRtpStat);
	msdx_send_set_audio_rtpstatcallback(NULL,printRtpStat);
	result = msdx_set_send_video_vodec("H264", 1280,720 );
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
	result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8700,true);
	assert(msdx_run_send_graph() ==0);
	
	//msdx_delete_send_address( remoteAddr2 , 8500, remoteAddr2, 8600 );

	getch();

	
	assert( msdx_stop_send_graph()==0);

	assert( msdx_destroy_send_graph()==0);

    msdx_uninitial();
}