#pragma  once

#ifdef MSDXCLIENT_EXPORTS
#define MSDX_CLIENT_API _declspec(dllexport)	 //导出
#else
#define MSDX_CLIENT_API _declspec(dllimport)   //导入
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief 开启msdx client的服务接口。需要在main函数里调用
	 */
	int MSDX_CLIENT_API msdx_client_startCallbackService( );

	/**
	 * @brief 关闭MSDX服务进程
	 * @param waitTime 阻塞等待的时间，如果为负则永远等待 , 默认值为500ms
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_closeMsdxSvr( const int waitTime );

	/**
	 * @brief 往MSDX服务端发送创建SendGraph的事件，并等待处理结果
	 * @param audioInputDevice 音频输入设备
	 * @param audioOutputDevice 音频输出设备
	 * @param AudioIP 音频报文的目标IP
	 * @param AudioPort	音频报文的目标端口
	 * @param Audio_SampleRate 音频采样率
	 * @param Audio_BitsPerSample 样本的位数
	 * @param Audio_Channels 音频的的声道数
	 * @param VideoIP 视频报文的目标IP
	 * @param VideoPort 视频报文的目标端口
	 * @param previewHwnd 视频回放的窗口
	 * @param width 视频的宽
	 * @param height 视频的高
	 * @return 如果非负则表示服务器端已完成指定事件。
	 */
	int MSDX_CLIENT_API msdx_client_createSendGraph( const char* videoCaptrueDevice,
		const char* audioInputDevice, const char* audioOutputDevice,
		const char * AudioIP,const int AudioPort, const char* AudioCodec,
		const int Audio_SampleRate , const int Audio_BitsPerSample , const int Audio_Channels,
		const char * VideoIP,const int VideoPort,
		const HWND previewHwnd,const int width, const int height,
		const char* crossbarName , const char* crossbarInputType );

	/**
	 * @brief 往MSDX服务端发送停止向指定IP发送媒体流，并等待处理结果
	 * @param AudioIP 音频报文的目标IP
	 * @param AudioPort	音频报文的目标端口
	 * @param VideoIP 视频报文的目标IP
	 * @param VideoPort 视频报文的目标端口
	  * @return 如果非负则表示服务器端已完成指定事件。
	 */
	int MSDX_CLIENT_API msdx_client_stopSend( const char * AudioIP,const int AudioPort,
		const char * VideoIP,const int VideoPort );

	/**
	 * @brief 往MSDX服务端发送创建RecvGraph的事件，并等待处理结果
	 * @param AudioIP 音频报文的目标IP
	 * @param AudioPort	音频报文的目标端口
	 * @param VideoIP 视频报文的目标IP
	 * @param VideoPort 视频报文的目标端口
	 * @param previewHwnd 视频回放的窗口
	 * @param width 视频的宽
	 * @param height 视频的高
	 * @return 如果成功则返回GraphID ， 否则返回负数。
	 */
	int MSDX_CLIENT_API msdx_client_createRecvGraph( const char * AudioIP,const int AudioPort,
		const char* AudioCodecMIME , const int AudioSampleRate , const int AudioBps , const int AudioChannels,
		const char * VideoIP,const int VideoPort,
		const HWND recvWnd, const int width, const int height );

	/**
	 * @brief 停止并销毁graphID对应的RecvGraph
	 * @graphID graph id
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_stopRecv( const int graphID );

	/**
	 * @brief 向指定目的共享桌面
	 * @param ScreenIP 目的IP
	 * @param ScreenPort 目的端口
	 * @param x 左上角 x坐标
	 * @param y 左上角 y坐标
	 * @param width 区域宽度
	 * @param height 区域高度
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_startScreenShare( const char* ScreenIP, const int ScreenPort,
		const int x,const int y,const int width,const int height );

	/**
	 * @brief 取消指定目的的桌面共享
	 * @param ScreenIP 目的IP
	 * @param ScreenPort 目的端口
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_stopScreenShare( const char* screenIP , const int screenPort );

	/**
	 * @brief 创建共享桌面RecvGraph
	 * @param ScreenIP 目的IP
	 * @param ScreenPort 目的端口
	 * @param remoteHwnd 播放窗口
	 * @param videoWidth 宽度
	 * @param videoHeight 高度
	 * @return 如果成功则返回GraphID，否则返回负数
	 */
	int MSDX_CLIENT_API  msdx_client_createShareScreenRecvGraph( const char * ScreenIP, const int ScreenPort, 
		const HWND remoteHwnd, const int videoWidth, const int videoHeight );

	/**
	 * @brief 删除共享桌面RecvGraph
	 * @param graphID 对应的graphID
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_stopShareScreenRecvGraph( const int graphID );

	/**
	 * @brief 重置视频接收窗口
	 * @param graphID 需要调整的RecvgraphID
	 * @param remoteHwnd 接收的窗口句柄
	 * @param xpos 左上角的x坐标
	 * @param ypos 左上角的y坐标
	 * @param width 窗口的宽度
	 * @param height 窗口的高度
	 * @param fullscreeflag 全屏标志
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_setRecvDisplayWindow( const int graphID,const HWND remoteHwnd,
		const long xpos, const long ypos, 
		const int width, const int height, const int fullscreenflag);
		
	/**
	 * @brief 重置视频窗口
	 * @param videoHwnd 接收的窗口句柄
	 * @param xpos 左上角的x坐标
	 * @param ypos 左上角的y坐标
	 * @param width 窗口的宽度
	 * @param height 窗口的高度
	 * @param fullscreeflag 全屏标志
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_setDisplayWindow( const HWND videoHwnd,
		const long xpos, const long ypos, 
		const int width, const int height, const int fullscreenflag);

	/**
	 * @brief 停止向指定的地址发送音频信息
	 * @param info 音频媒体的网络相关信息
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_forbidSpeak( const char* ip , const int port );

	/**
	 * @brief 允许向指定的地址发送音频信息
	 * @param info 音频媒体的网络相关信息
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_permitSpeak( const char* ip , const int port );

	/**
	 * @brief 退出会议。销毁RecvGraph
	 * @return 非负表示工作正常
	 */
	int MSDX_CLIENT_API msdx_client_exitConference( void );

	/**
	 * @brief 控制音视频的发送
	 * @param mediaType 媒体类型，0为音频，1为视频
	 * @param state 控制信息，如果为0则停止，否则启动
	 */
	int MSDX_CLIENT_API msdx_client_controlMediaSend( int mediaType , int state );



	/* 设置发送Graph的RTP统计信息回调函数（全局的）
		pRtpStatCallbackFunc函数类型为RtpStatCallback，这里为了声明方便，只定义为void*，但要传入RtpStatCallback函数类型的指针才算有效
		RtpStatCallback回调函数的声明可查看RtpStat.h
	*/
	void MSDX_CLIENT_API msdx_client_send_set_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc);
	void MSDX_CLIENT_API msdx_client_send_set_video_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc);
	void MSDX_CLIENT_API msdx_client_send_set_audio_rtpstatcallback( void *pClass, void* pRtpStatCallbackFunc );

	/*
		设置单路接收Graph的RTP统计信息回调函数
	*/
	void MSDX_CLIENT_API msdx_client_recv_set_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc);
	void MSDX_CLIENT_API msdx_client_recv_set_video_rtpstatcallback(const int graphId, void *pClass, void* pRtpStatCallbackFunc);
	void MSDX_CLIENT_API msdx_client_recv_set_audio_rtpstatcallback(const int graphId, void *pClass, void* pRtpStatCallbackFunc );

	/**
	 *	设置dscp值
	 * @param dscp 有效值是0-63
	 * @return 返回0表示设置成功
	 */
	int MSDX_CLIENT_API msdx_client_send_set_dscp(const int dscp);

	//设置传输统计回调函数，pTranStatCallbackFunc要求必须是TranStatCallback类型，请参见RtpStat.h
	void MSDX_CLIENT_API msdx_client_set_transtatcallback(void * pClass, void * pTranStatCallbackFunc);

	
	/**
	*	@brief	获取传输统计情况。本函数读取到的数据不是实时的，建议回调方式。
	 * @param transtat 传入一个TranStat结构指针变量，统计结果值将保存到该变量中;	TranStat结构的定义请参见RtpStat.h
	 * @see RtpStat.h
	 */
	void  MSDX_CLIENT_API msdx_client_get_global_transtat(void * transtat);


#ifdef __cplusplus
}
#endif