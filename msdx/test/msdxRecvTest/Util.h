#pragma  once

/**
 * @brief 根据参数创建接收graph
 */
int createRecvGraph( const int videoWidth, const int videoHeight , const int fps, const int audioPort , const int videoPort )
{
	int graphid=-1;
	const char* localAddress = "127.0.0.1";

	int result;
	//创建大流的接受端
	//vedio graph
	assert( (graphid = msdx_create_recv_graph())>0 );
	result = msdx_set_recv_video_codec(graphid, "H264", videoWidth,videoHeight, fps, 16);
	result = msdx_set_recv_audio_codec(graphid, "SPEEX", 16000, 1, 16);

	assert(result==0);

	result = msdx_set_recv_address(graphid,localAddress, audioPort, localAddress, videoPort);

	msdx_set_record_file_name(graphid, "msdxRecvTest_000.mp4");

	result = msdx_connect_recv_graph(graphid);
	assert(result==0);
	result = msdx_run_recv_graph(graphid);
	/*msdx_recv_set_video_rtpstatcallback(graphid,NULL,printRtpStat);
	msdx_recv_set_audio_rtpstatcallback(graphid,NULL,printRtpStat);*/

	return graphid;
}

/**
 * @brief 销毁接收流
 */
void destroyRecvGraph( const int graphID )
{
	msdx_stop_recv_graph( graphID );
	msdx_destroy_recv_graph( graphID );
}