#pragma  once

//#define HD_VIDEO_CAPTURE_DEVICE "Logitech HD Webcam C270"
#define  DEFAULT_VIDEO_CAPTRUE_DEVICE "USB2.0 UVC PC Camera"    //视频捕获设备名称
#define HD_VIDEO_CAPTURE_DEVICE "e2eSoft VCam"
//#define  DEFAULT_VIDEO_CAPTRUE_DEVICE "e2eSoft VCam"    //视频捕获设备名称
#define USAGE "application name  -remote ip -camera name(可选)"

#define FULL_HD_VIDEO_CAPTURE_DEVICE "Timeleak hd capture device"
#define CROSS_BAR_DEVICE "HD70a BDA Analog Xbar Secondary"
#define CROSS_BAR_TYPE "Composite"

char * videoCaptrueDevice; //视频捕获设备名称

void TestSmallVideo()
{
	int result;
	result = msdx_initial();

	const char * remoterAddr;//目标地址
	const char*  remoteAddr2 = "202.38.254.146";//目标地址2

	int videoWidth;
	int videoHeight;
	int choise;
	cout<<"选择视频类型:\n1.高清;\n2全高清"<<endl;
	cin>> choise;
	if( choise==2 )
	{
		videoWidth = 1920;
		videoHeight = 1080;
		msdx_config_set_video_device(FULL_HD_VIDEO_CAPTURE_DEVICE, CROSS_BAR_DEVICE, CROSS_BAR_TYPE);
	}else
	{
		videoWidth = 1280;
		videoHeight = 720;
		msdx_config_set_video_device(HD_VIDEO_CAPTURE_DEVICE, "", "");
	}
  

    result = msdx_create_send_graph(true);
	assert(result==0);
	msdx_send_set_video_rtpstatcallback(NULL,printRtpStat);
	msdx_send_set_audio_rtpstatcallback(NULL,printRtpStat);
	result = msdx_set_send_video_vodec("H264", videoWidth, videoHeight );
	//result = msdx_set_send_video_vodec("H264", 1280, 720);
	assert(result==0);
	result = msdx_set_send_audio_codec("SPEEX", 16000, 1, 16, false);
	assert(result==0);

	assert( msdx_connect_send_graph()==0);
    result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8600);


	assert(msdx_run_send_graph() ==0);

	getch();
	result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8700,true);
	//msdx_delete_send_address( remoteAddr2 , 8500, remoteAddr2, 8600 );

	getch();
	
	result = msdx_add_send_address(remoteAddr2, 8500, remoteAddr2, 8600 );

/*
	const char* addr3 = "202.38.254.154";
	result = msdx_add_send_address(addr3,8500,addr3,8600,true );

	getch();
	result = msdx_delete_send_address( addr3, 8500, addr3, 8600 );

	//msdx_send_enable_adaptive_coderate(true);

	//getch();

	//msdx_add_send_address(remoterAddr, 4466, remoterAddr, 8822);
	//getch();
	//msdx_add_send_address("202.38.250.230", 8246, "202.38.250.230", 5664);
	//getch();


	int qp = 15;
	
	msdx_set_send_video_quantizer(qp);
*/    
	getch();

	
	assert( msdx_stop_send_graph()==0);

	assert( msdx_destroy_send_graph()==0);

    msdx_uninitial();
}