#pragma  once
/************************************************************************/
/* 发送Graph API函数，其中可能包含一些全局函数                                */
/************************************************************************/

#ifdef MSDX_EXPORTS  //主要在项目属性中设置此预处理器
#define MSDXAPI __declspec(dllexport)
#else
#define MSDXAPI __declspec(dllimport)
#endif

//typedef unsigned char bool; //用来表示布尔类型，C语言中并没有bool类型

#ifdef __cplusplus
extern "C"
{
#endif

	enum msdxErrors{
		MSDX_ERROR,
		MSDX_OUTOFMEM,
		MSDX_BADARG,
		MSDX_NOVIDEODEVICE,
		MSDX_NOAUDIODEVICE,
		MSDX_GRAPHFAIL,
		MSDX_CONNECTFAIL,
		MSDX_NOFILTER
	};

	/************************************************************************/
	/* 全局API函数                                                          */
	/************************************************************************/

	int MSDXAPI testRoute();

	// Formal Interface

	int MSDXAPI msdx_initial(); 	
	void MSDXAPI msdx_uninitial();
	
	void MSDXAPI msdx_set_default_config();
	/*config*/
	void MSDXAPI msdx_config_set_video_device(const char * deviceid,const char *crossbar,const char* cb_input);   //junLin.chen 2010-07-20
	void MSDXAPI msdx_config_set_audio_format(int SampleRate,int Channels,int BitsPerSample);

	///设置音频设备...zhenHua.sun 2010-10-19
	void MSDXAPI msdx_config_set_audio_device(const char* inputDevice , const char* outputDevice );

	//判断系统是否支持硬件加速
	bool MSDXAPI msdx_check_hardware_acceleration_support();
	/**
	*	@brief	统计传输流量情况
	 * @param transtat 传入一个TranStat结构指针变量，统计结果值将保存到该变量中;	TranStat结构的定义请参见RtpStat.h
	 * @see RtpStat.h
	 *	@return 0表示统计成功，负数则统计失败或加载rtpware对应的函数失败
	 */
	int MSDXAPI msdx_compute_global_transtat(void * transtat);

	/************************************************************************/
	/* 发送Graph API函数                                                                     */
	/************************************************************************/

	/*send Graph 全局只有一个*/

	/*@ return Graph ID:-1 indicate there is no send graph*/
	int MSDXAPI msdx_get_send_graph();
	/*@return Grapg ID: if fail return -1,else return 0*/

	//获取发送Graph状态
	int MSDXAPI msdx_send_get_graph_state();

	int MSDXAPI msdx_create_send_graph( bool enableSmallVideo=false, bool disable_hwcodec=false);

	int MSDXAPI msdx_pause_send_graph();
	int MSDXAPI msdx_resume_send_graph();
	int MSDXAPI msdx_run_send_graph();
	int MSDXAPI msdx_stop_send_graph();
	int MSDXAPI msdx_destroy_send_graph();

	//by dsh 10-07-30增加了采样率等参数
	int MSDXAPI msdx_set_send_audio_codec(const char * codecMIME,const int SampleRate,const int Channels,const int BitsPerSample, const bool aec_enable);
	//by dsh 10-08-06
	void MSDXAPI msdx_set_audio_link_state(bool state);

	int MSDXAPI msdx_set_send_video_vodec(const char * codecMIME,const int width,const int height,const int fps);
	int MSDXAPI msdx_set_send_screen_codec(char * codecMIME);

	/**
	 * 添加参数smallVideo，如果为1则发送视频小流，如果为0则发送高清视频流......zhenHua.sun 2011-08-10
	 */
	int MSDXAPI msdx_add_send_address(const char * audioIP,const int audioport,const char * videoIP,const int videoport,const bool smallVideo = 0);
	int MSDXAPI msdx_add_send_address2(const char * audioIP,const int audioport,const char * videoIP,const int videoport,const char *ScreenIP,const int ScreenPort);
	

	int MSDXAPI msdx_delete_send_address(const char * audioIP,const int audioport,const char * videoIP,const int videoport);
	int MSDXAPI msdx_delete_send_address2(const char * audioIP,const int audioport);
	int MSDXAPI msdx_delete_screen_send_address(const char *ScreenIP,const int ScreenPort);
	int MSDXAPI msdx_set_send_preview_window(HWND WndHandle,long left,long top,long width,long height,int fullScreen);
	int MSDXAPI msdx_set_screen_filter_config(int left,int up,int right,int down);
	int MSDXAPI msdx_connect_send_graph();
	//直接设置量化步长
	int MSDXAPI msdx_set_send_video_quantizer(int quantizer);
	//根据当前码率和目标码率调整码率
	void MSDXAPI msdx_adapt_encode_rate(float currentRate,float destRate);
	//设置RTCP Filter侦听地址及端口
	int MSDXAPI msdx_set_Rtcp_Recv_Filter(char *ListenIP,int ListenPort);

	//删除RTCP Filter侦听的某个端口
	int MSDXAPI msdx_delete_Rtcp_Recv_address(char *ListenIP,int ListenPort);

	/**
	 * @author zhenHua.sun 
	 * @date 2010-08-06
	 * @brief 设定视频编码器的工作状态
	 * @param work 如果为true则启动编码器，否则编码器不工作
	 * @return 0表示工作正常
	 */
	int MSDXAPI msdx_control_audio_encoder( bool work );
	/**
	 * @author zhenHua.sun 
	 * @date 2010-08-06
	 * @brief 设定视频编码器的工作状态
	 * @param work 如果为true则启动编码器，否则编码器不工作
	 * @return 0表示工作正常
	 */
	int MSDXAPI msdx_control_video_encoder( bool work );


	/************************************************************************/
	/* 设置发送Graph的RTP统计信息回调函数
	pRtpStatCallbackFunc函数类型为RtpStatCallback，这里为了声明方便，只定义为void*，但要传入RtpStatCallback函数类型的指针才算有效
	RtpStatCallback回调函数的声明可查看RtpStat.h
	*/
	/************************************************************************/
	void MSDXAPI msdx_send_set_video_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc);
	void MSDXAPI msdx_send_set_audio_rtpstatcallback( void *pClass, void* pRtpStatCallbackFunc );

	
	/**
	 *	设置发送出去的RTP包的dscp值
	 * @param dscp 有效值是0-63
	 * @return 返回0表示设置成功
	 */
	int MSDXAPI msdx_send_set_video_dscp(const int dscp);

	int MSDXAPI msdx_send_set_audio_dscp(const int dscp);

	//是否启用自适应码率调整
	void MSDXAPI msdx_send_enable_adaptive_coderate(bool enable);

	/************************************************************************/
	/* 接收Graph API函数                                                                     */
	/************************************************************************/

	/*recv graph 全局可以有多个recv graph*/
	/*return graph id >= 1*/
	int MSDXAPI msdx_create_recv_graph(const char * userId = "");
	int MSDXAPI msdx_pause_recv_graph(int graphid);
	int MSDXAPI msdx_resume_recv_graph(int graphid);
	int MSDXAPI msdx_stop_recv_graph(int graphid);
	int MSDXAPI msdx_destroy_recv_graph(int graphid);
	/**
	* @author zhenHua.sun
	* @date  2010-07-23
	* @brief 销毁所有的RecvGraph
	* @return 如果成功则返回0
	*/
	int MSDXAPI msdx_destroy_recv_graphs(  );

	int MSDXAPI msdx_set_recv_audio_codec(int graphid,char * codecMIME,int SampleRate,int Channels,int BitsPerSample);
	int MSDXAPI msdx_set_recv_video_codec(int graphid,char * codecMIME,int width,int height,int fps,int BitCount);
	int MSDXAPI msdx_set_recv_screen_codec(int graphid,char * codecMIME,int width,int height,int fps,int BitCount);
	int MSDXAPI msdx_set_recv_address(const int graphid,const char * audioIP,const int audioport,const char * videoIP,const int videoport);
	int MSDXAPI msdx_set_recv_screen_address(int graphid,const char * ScreenIP,const int ScreenPort);
	int MSDXAPI msdx_set_recv_display_window(int graphid,HWND WndHandle,long left,long top,long width,long height,int fullScreen);
	int MSDXAPI msdx_connect_recv_graph(int graphid);
	int MSDXAPI msdx_run_recv_graph(int graphid);
	int MSDXAPI msdx_clear_recv_buf(int graphid);

	/**
	 * 录制相关方法
	 * by Liaokz, 2013-5-8
	 */
	int MSDXAPI msdx_set_record_file_name(int graphid, const char * fileName);
    int MSDXAPI msdx_set_record_auto_cut(int graphid, int sec);
    int MSDXAPI msdx_set_record_cut_by_episode_change(int graphid, bool enable);
	int MSDXAPI msdx_set_record_callback(int graphid, void * obj, void * func);
  int MSDXAPI msdx_set_record_jobid(int graphid, long job_id);
	/**
	 * 录制端主动分割
	 * by Liaokz, 2013-8-8
	 */
	int MSDXAPI msdx_record_cut_file(int graphid, long job_id, const char * fileName);
	/**
	 * 发送端要求分割
	 * by Liaokz, 2013-8-8
	 */
	int MSDXAPI msdx_send_cut_file();

	/**
	 * @brief 控制音频的接收
	 * @param enable 如果为true则恢复音频的接收，否则关闭音频的接收
	 * @return 如果成功则返回0
	 */
	int MSDXAPI msdx_control_audio_recv( const int graphid, const bool enable );

	/**
	 * @brief 控制视频的接收
	 * @param enable 如果为true则恢复音频的接收，否则关闭音频的接收
	 * @return 如果成功则返回0
	 */
	int MSDXAPI msdx_control_video_recv( const int graphid,  const bool enable );

	/*@return
	*0:initial
	*1:running
	*2:stop
	*3:pause
	**/
	int MSDXAPI msdx_get_graph_state(int graphid);

	/************************************************************************/
	/* 设置接收Graph的RTP统计信息回调函数
	pRtpStatCallbackFunc函数类型为RtpStatCallback，这里为了声明方便，只定义为void*，但要传入RtpStatCallback函数类型的指针才算有效
	RtpStatCallback回调函数的声明可查看RtpStat.h
	*/
	/************************************************************************/
	void MSDXAPI msdx_recv_set_video_rtpstatcallback(int graphid, void *pClass, void* pRtpStatCallbackFunc);
	void MSDXAPI msdx_recv_set_audio_rtpstatcallback(int graphid, void *pClass, void* pRtpStatCallbackFunc );

	/*是否开启自动纠正音视频同步偏差，目标是减小延时，加速播放，注意开启自动同步可能会引起短暂的视频马赛克*/
	void MSDXAPI msdx_recv_all_enable_auto_resync(bool enable);
	
	void MSDXAPI msdx_recv_enable_auto_resync(int graphid, bool enable);



	/************************************************************************/
	/* 本地音视频测试                                                         */
	/************************************************************************/

	enum AudioTestType
	{
		MSDX_LOCAL_ATYPE_DEVICE,
		MSDX_LOCAL_ATYPE_FILE
	};

	int MSDXAPI msdx_create_audio_test_graph(const AudioTestType type, const char * inputSrc, const char * outputDevice);
	void MSDXAPI msdx_destroy_audio_test_graph();

	int MSDXAPI msdx_create_video_test_graph(const char * deviceid, const char*crossbar, const char* cb_input, int width, int height, int fps);
	int MSDXAPI msdx_set_video_test_preview_window(HWND WndHandle, long left, long top, long width, long height);
	int MSDXAPI msdx_run_video_test_graph();
	void MSDXAPI msdx_destroy_video_test_graph();
	/************************************************************************/
	/* smallvideo graph                                                     */
	/************************************************************************/
	int MSDXAPI msdx_create_smallvideo_graph(const char * userId);
    int MSDXAPI msdx_destroy_smallvideo_graphs();
	int MSDXAPI msdx_set_preview_video_codec(char * codecMIME,int width,int height,int fps,int BitCount);
	void MSDXAPI msdx_preview_set_video_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc );
	int MSDXAPI msdx_set_preview_address(const char * audioIP,const int audioport,const char * videoIP,const int videoport);
	/*int MSDXAPI msdx_set_smallvideo_video_vodec(const char * codecMIME, const int width, const int height);*/
	int MSDXAPI msdx_add_smallvideo_address(const char * audioIP, const int audioport, const char * videoIP, const int videoport,const bool smallVideo );
	int MSDXAPI msdx_connect_smallvideo_graph();
	int MSDXAPI msdx_run_smallvideo_graph();
	int MSDXAPI msdx_set_preview_display_window(HWND WndHandle,long left,long top,long width,long height,int fullScreen);
	int MSDXAPI msdx_get_smallvideo_graph();
	int MSDXAPI msdx_delete_smallvideo_address(const char * audioIP, const int audioport, const char * videoIP, const int videoport);

#ifdef __cplusplus
}

#endif

