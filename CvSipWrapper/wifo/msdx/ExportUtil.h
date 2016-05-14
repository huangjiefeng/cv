#pragma  once
/************************************************************************/
/* 发送Graph API函数，其中可能包含一些全局函数                                */
/************************************************************************/

#ifdef MSDX_EXPORTS  //主要在项目属性中设置此预处理器
#define MSDXAPI __declspec(dllexport)
#else
#define MSDXAPI __declspec(dllimport)
#endif

typedef unsigned char bool_t; //用来表示布尔类型，C语言中并没有bool类型

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
	/* 全局API函数                                                                     */
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

	int MSDXAPI msdx_create_send_graph();

	int MSDXAPI msdx_pause_send_graph();
	int MSDXAPI msdx_resume_send_graph();
	int MSDXAPI msdx_run_send_graph();
	int MSDXAPI msdx_stop_send_graph();
	int MSDXAPI msdx_destroy_send_graph();

	//by dsh 10-07-30增加了采样率等参数
	int MSDXAPI msdx_set_send_audio_codec(const char * codecMIME,const int SampleRate,const int Channels,const int BitsPerSample);
	//by dsh 10-08-06
	void MSDXAPI msdx_set_audio_link_state(bool_t state);

	int MSDXAPI msdx_set_send_video_vodec(const char * codecMIME,const int width,const int height);
	int MSDXAPI msdx_set_send_screen_codec(char * codecMIME);

	int MSDXAPI msdx_add_send_address(const char * audioIP,const int audioport,const char * videoIP,const int videoport);
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
	//设置Rtcp回调函数
	int MSDXAPI msdx_set_Rtcp_Callbak_func(void *pClass,void *func);
	//删除RTCP Filter侦听的某个端口
	int MSDXAPI msdx_delete_Rtcp_Recv_address(char *ListenIP,int ListenPort);

	/**
	 * @author zhenHua.sun 
	 * @date 2010-08-06
	 * @brief 设定视频编码器的工作状态
	 * @param work 如果为true则启动编码器，否则编码器不工作
	 * @return 0表示工作正常
	 */
	int MSDXAPI msdx_control_audio_encoder( bool_t work );
	/**
	 * @author zhenHua.sun 
	 * @date 2010-08-06
	 * @brief 设定视频编码器的工作状态
	 * @param work 如果为true则启动编码器，否则编码器不工作
	 * @return 0表示工作正常
	 */
	int MSDXAPI msdx_control_video_encoder( bool_t work );


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
	int MSDXAPI msdx_send_set_dscp(const int dscp);



	/************************************************************************/
	/* 接收Graph API函数                                                                     */
	/************************************************************************/

	/*recv graph 全局可以有多个recv graph*/
	/*return graph id >= 1*/
	int MSDXAPI msdx_create_recv_graph();
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

#ifdef __cplusplus
}
#endif

