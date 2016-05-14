#ifndef MSDX_SERVICE_COMMON_H
#define MSDX_SERVICE_COMMON_H

#define MSDX_SERVICE_NAME			"com.coolview.msdx.server"
#define MSDX_SERVICE_OBJECT_PATH	"/com/coolview/msdx/server"

#include <QtCore/QtCore>
enum MsdxServiceMethod
{
	CreateSendGraph_Method,
	StopSendGraph_Method,
	CreateRecvGraph_Method,
	StopRecvGraphID_Method,
	StopRecvUserID_Method,
	SetRecvWnd_Method,
	SetPreviewWnd_Method,
	ForbitSpeak_Method,
	PermitSpeak_Method,
	ExitConference_Method,
	ControlMedia_Method,
	SetDscp_Method
};


///创建Graph需要的网络信息
class NetInfo
{
public:
	QString			ip_addr;		//IP地址（v4或v6）
	unsigned short	video_port;		//视频发往的目标端口
	unsigned short	audio_port;		//音频发往的目标端口

#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, NetInfo& data)
	{
		in >> data.ip_addr>> data.video_port >> data.audio_port;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, NetInfo& data)
	{
		out << data.ip_addr << data.video_port << data.audio_port;
		return out;
	}
#endif
};

//end


///创建Graph需要的视频信息
class VideoInfo
{
public:
	QString			codec_name;		//编/解码码类型
	unsigned short	width;			//视频帧的宽度，单位为像素
	unsigned short	height;			//视频帧的高度，单位为像素
	unsigned short	fps;			//视频的帧率，单位为帧/秒
	unsigned short	bit_count;		//每个像素所占的位数
	unsigned short	quality;		//编码器量化值
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, VideoInfo& data)
	{
		in >> data.codec_name >> data.width >> data.height >> data.fps >> data.bit_count >> data.quality;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, VideoInfo& data)
	{
		out << data.codec_name << data.width << data.height << data.fps << data.bit_count << data.quality;
		return out;
	}
#endif
};

//end


///创建Graph需要的音频信息
class AudioInfo
{
public:
	QString			codec_name;			//编码类型
	unsigned short	channels;			//音频的信道数目
	unsigned short	bits_per_sample;	//样本所占的位数
	unsigned int	sample_rate;		//音频采样率
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, AudioInfo& data)
	{
		in >> data.codec_name >> data.channels >> data.bits_per_sample >> data.sample_rate;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, AudioInfo& data)
	{
		out << data.codec_name << data.channels << data.bits_per_sample << data.sample_rate;
		return out;
	}
#endif
};
//end

class UserDevice
{
public:
	QString		video_capture;		//摄像头名称
	QString		video_crossbar;		//crossbar Name。用于支持高清
	QString		video_crossbar_type;//crossbarInputType,设置crossbar的输入类型
	QString		audio_input_device;		//音频输入设备
	QString		audio_output_device;	//音频输出设备
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, UserDevice& data)
	{
		in >> data.video_capture >> data.video_crossbar >> data.video_crossbar_type 
			>> data.audio_input_device >> data.audio_output_device ;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, UserDevice& data)
	{
		out << data.video_capture << data.video_crossbar << data.video_crossbar_type 
			<< data.audio_input_device << data.audio_output_device;
		return out;
	}
#endif
};

//end

class RecvGraphInfo
{
public:
	void initial(const char * AudioIP,const int& AudioPort,
		const char* AudioCodecMIME , const int& AudioSampleRate , const int& AudioBps , const int& AudioChannels,
		const char * VideoIP,const int& VideoPort,
		const int recvWnd, const int& width, const int& height , const char* userId )
	{
		net_info.ip_addr = QString::fromLocal8Bit( VideoIP );
		net_info.video_port = VideoPort;
		net_info.audio_port = AudioPort;

		recv_wnd = (int)recvWnd;

		user_id = QString::fromLocal8Bit(userId );

		video_info.codec_name = QString::fromLocal8Bit("H264");
		video_info.bit_count = 32;
		video_info.fps = 30;
		video_info.width = width;
		video_info.height = height;
		video_info.quality = 32;
		audio_info.bits_per_sample = AudioBps;
		audio_info.channels = AudioChannels;
		audio_info.codec_name = QString::fromLocal8Bit(AudioCodecMIME);
		audio_info.sample_rate = AudioSampleRate;
	}
	int			recv_wnd;				//接收视频的播放窗口
	NetInfo		net_info;
	VideoInfo	video_info;				
	AudioInfo	audio_info;
	QString		user_id;				//用户名，不包含域名信息
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, RecvGraphInfo& data)
	{
		in >> data.recv_wnd >> data.net_info >> data.video_info >> data.audio_info >> data.user_id ;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, RecvGraphInfo& data)
	{
		out << data.recv_wnd << data.net_info << data.video_info << data.audio_info << data.user_id;
		return out;
	}
#endif
};


//end

class SendGraphInfo
{
public:

	void initial(const char* videoCaptrueDevice,const char* audioInputDevice, const char* audioOutputDevice,
		const char * AudioIP,const int& AudioPort,
		const char* Audio_Codec,const int& Audio_SampleRate , const int& Audio_BitsPerSample , const int& Audio_Channels,
		const char * VideoIP,const int& VideoPort,
		const int previewHwnd,const int& width, const int& height,
		const char* crossbarName , const char* crossbarInputType)
	{
		device.video_capture = QString::fromLocal8Bit( videoCaptrueDevice );
		device.video_crossbar = QString::fromLocal8Bit( crossbarName );
		device.video_crossbar_type = QString::fromLocal8Bit( crossbarInputType);

		device.audio_input_device = QString::fromLocal8Bit( audioInputDevice );
		device.audio_output_device = QString::fromLocal8Bit( audioOutputDevice );

		net_info.ip_addr = QString::fromLocal8Bit( VideoIP );
		net_info.video_port = VideoPort;
		net_info.audio_port = AudioPort;

		preview_wnd = (int)previewHwnd;

		video_info.codec_name = QString::fromLocal8Bit("H264");
		video_info.bit_count = 32;
		video_info.fps = 30;
		video_info.width = width;
		video_info.height = height;
		video_info.quality = 32;
		audio_info.bits_per_sample = Audio_BitsPerSample;
		audio_info.channels = Audio_Channels;
		audio_info.codec_name = QString::fromLocal8Bit(Audio_Codec);
		audio_info.sample_rate = Audio_SampleRate;
	}

	int		preview_wnd;				//回显窗口
	NetInfo		net_info;
	VideoInfo	video_info;				
	AudioInfo	audio_info;
	UserDevice device;
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, SendGraphInfo& data)
	{
		in >> data.preview_wnd >> data.net_info >> data.video_info >> data.audio_info >> data.device;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, SendGraphInfo& data)
	{
		out << data.preview_wnd << data.net_info << data.video_info << data.audio_info << data.device;
		return out;
	}
	//end
#endif
};

class WndInfo
{
public:

	void initial( int graphID , const char* userID , int wnd , 
		int xpos , int ypos , int width , int height )
	{
		this->graph_id = graphID;
		this->user_id = userID;
		this->video_hwnd = wnd;
		this->point_x = xpos;
		this->point_y = ypos;
		this->width = width;
		this->height = height;
	}


	int			point_x;			//左上角x坐标
	int			point_y;			//左上角y坐标
	unsigned short	width;				//窗口的宽度，单位为像素
	unsigned short	height;				//窗口的高度，单位为像素
	int				full_screen_flag;	//全屏标志
	int				graph_id;			//RecvGraph对应的ID
	int				video_hwnd;			//视频窗口
	QString			user_id;			//视频窗口的用户名
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, WndInfo& data)
	{
		in >> data.point_x >> data.point_y >> data.width >> data.height >> data.full_screen_flag
			>> data.graph_id >> data.video_hwnd >>data.user_id;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, WndInfo& data)
	{
		out << data.point_x << data.point_y << data.width << data.height << data.full_screen_flag
			<< data.graph_id << data.video_hwnd << data.user_id;
		return out;
	}
	//end
#endif
};

enum MX_MEDIA_TYPE{
	AUDIO_MEDIA = 0,
	VIDEO_MEDIA
};

class MediaControlInfo
{
public:
	int  media_type;			//控制的媒体类型,0为Audio，1为Video
	int  state;							//设置的媒体状态，当前支持起停，0为停，否则为启动
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, MediaControlInfo& data)
	{
		in >> data.media_type >> data.state;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, MediaControlInfo& data)
	{
		out << data.media_type << data.state;
		return out;
	}
	//end
#endif
};

class QosDscpInfo
{
public:
	int dscp;
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, QosDscpInfo& data)
	{
		in >> data.dscp;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, QosDscpInfo& data)
	{
		out << data.dscp;
		return out;
	}
	//end
#endif
};

#include <RtpStat.h>
class RtpStatItemInfo
{
public:
	uint32_t ssrc;                  /*RTP会话标识，一般取自发送端的SSRC*/
	uint32_t payload_type;              /*RTP载荷类型，对应编码类型，比如H264、speex*/
	int media_type;  /*媒体类型，指视频、音频等，不细分到编码格式*/

	int rtp_endpoint_type;  /*参与者角色，以此识别是发送端或接收端的统计数据*/
	QString  rem_addr;  /*RTP传输的目标IP地址，如果自己是发送者，则目标是接收者，反之亦然. 注意，原来该字段是sockaddr_storage类型，但考虑到winsock头文件版本容易引发编译问题，所以改成字符串*/
	int rem_addr_len;           /* IP地址长度*/
	int rem_port;       /*远程主机端口*/
	QString  local_addr;  /*RTP传输的本地IP地址，该值一般是实际IP地址，但也有可能是回环地址或零地址; 组播传输时该值是IP组播地址*/
	int local_addr_len;         /* 本地IP地址长度*/
	int local_port;       /*本地主机端口*/

	/*RTP会话统计参数，参考ortp的rtp_stats结构*/
	uint64_t packet_sent;    /*number of rtp packet sent*/
	uint64_t sent;      /* bytes sent */
	uint64_t packet_recv;   /* number of packets received */
	uint64_t hw_recv;       /* bytes of payload received */
	uint64_t recv;      /* bytes of payload received and delivered in time to the application */
	uint64_t outoftime;     /* number of packets that were received too late */
	uint64_t cum_packet_loss;   /* cumulative number of packet lost */
	uint64_t discarded;      /* incoming packets discarded because the queue exceeds its max size */

	/* IP带宽估计，单位 Kbit/s*/
	float send_bandwidth;       /*发送带宽估计，单位 Kbit/s*/
	float recv_bandwidth;       /*接收带宽估计，单位 Kbit/s*/

	/*rtcp统计参数*/
	float lost;         /*丢包率，单位百分比*/
	uint32_t jitter;        /*抖动，单位ms*/
	uint32_t delay;                     /*延时，单位ms*/

	/*报告相关参数*/
	uint32_t seqnumber;   /* 统计报告的序列号，每新统计一次，则递增1（从0开始）。递增只针对具体的RTP会话而言*/
	uint64_t timestamp;  /* 时间戳，用来描述产生报告的时刻，单位为s，可参考Unix时间戳*/
	uint32_t interval;      /*  采集时间间隔，单位ms，默认值为5000*/

	int graph_id;   /*directshow graph的id，如果使用msdx管理rtp传输，则graphid等于rtp传输所在的（发送/接收）graph的Id；如果与directshow无关，则该值为0*/

	QString user_id;		//存储用户名

#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, RtpStatItemInfo& data)
	{
		in >> data.cum_packet_loss >> data.delay >> data.discarded >> data.graph_id >> data.hw_recv >> data.interval
			>> data.jitter >> data.local_addr >> data.local_addr_len >> data.local_port >> data.lost >> data.media_type
			>> data.outoftime >> data.packet_recv >> data .packet_sent >> data.payload_type >> data.recv >> data.recv_bandwidth
			>> data.rem_addr >> data.rem_addr_len >> data.rem_port >> data.rtp_endpoint_type >> data.send_bandwidth 
			>> data.sent >> data.seqnumber >> data.ssrc >> data.timestamp >>data.user_id;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, RtpStatItemInfo& data)
	{
		out << data.cum_packet_loss << data.delay << data.discarded << data.graph_id << data.hw_recv << data.interval
			<< data.jitter << data.local_addr << data.local_addr_len << data.local_port << data.lost << data.media_type
			<< data.outoftime << data.packet_recv << data .packet_sent << data.payload_type << data.recv << data.recv_bandwidth
			<< data.rem_addr << data.rem_addr_len << data.rem_port << data.rtp_endpoint_type << data.send_bandwidth 
			<< data.sent << data.seqnumber << data.ssrc << data.timestamp << data.user_id;
		return out;
	}
#endif

	static void RtpStatItemInfo2RtpStatItem( const RtpStatItemInfo& info ,RtpStatItem& item )
	{
		item.cum_packet_loss = info.cum_packet_loss;
		item.delay = info.delay;
		item.discarded = info.discarded;
		item.hw_recv = info.hw_recv;
		item.graph_id = info.graph_id;
		item.interval = info.interval;
		item.jitter = info.jitter;
		strncpy( item.local_addr ,  qPrintable(info.local_addr),sizeof( item.local_addr) );
		item.local_addr_len =info.local_addr_len;
		item.local_port = info.local_port;
		item.lost = info.lost;
		item.media_type = static_cast<StreamMediaType>(info.media_type);
		item.outoftime = info.outoftime;
		item.packet_recv = info.packet_recv;
		item.packet_sent = info.packet_sent;
		item.payload_type = info.payload_type;
		item.recv = info.recv;
		item.recv_bandwidth = info.recv_bandwidth;
		strncpy( item.rem_addr ,  qPrintable(info.rem_addr) ,sizeof( item.rem_addr) );
		item.rem_addr_len = info.rem_addr_len;
		item.rem_port = info.rem_port;
		item.rtp_endpoint_type = static_cast<RtpEndpointType>(info.rtp_endpoint_type);
		item.send_bandwidth = info.send_bandwidth;
		item.sent = info.sent;
		item.seqnumber = info.seqnumber;
		item.ssrc = info.ssrc;
		item.timestamp = info.timestamp;
	}

	static void RtpStatItem2RtpStatItemInfo( const RtpStatItem& item ,RtpStatItemInfo& info )
	{
		info.graph_id = item.graph_id;
		if(item.rtp_endpoint_type == eET_Sender)
			info.graph_id = 0 ; //发送端的graph Id都为0

		info.cum_packet_loss = item.cum_packet_loss;
		info.delay = item.delay;
		info.discarded = item.discarded;
		info.hw_recv = item.hw_recv;
		info.interval = item.interval;
		info.jitter = item.jitter;
		info.local_addr = item.local_addr;
		info.local_addr_len =item.local_addr_len;
		info.local_port = item.local_port;
		info.lost = item.lost;
		info.media_type = item.media_type;
		info.outoftime = item.outoftime;
		info.packet_recv = item.packet_recv;
		info.packet_sent = item.packet_sent;
		info.payload_type = item.payload_type;
		info.recv = item.recv;
		info.recv_bandwidth = item.recv_bandwidth;
		info.rem_addr = item.rem_addr;
		info.rem_addr_len = item.rem_addr_len;
		info.rem_port = item.rem_port;
		info.rtp_endpoint_type = item.rtp_endpoint_type;
		info.send_bandwidth = item.send_bandwidth;
		info.sent = item.sent;
		info.seqnumber = item.seqnumber;
		info.ssrc = item.ssrc;
		info.timestamp = item.timestamp;
	}
	
};

/*
* @brief 传输统计，可以是多个连接的传输统计
*/
class TranStatElement
{
public:
    float bandwidth;			 /*带宽，单位kb/s*/
    float lost;					/*丢包率，单位百分比%*/
    uint32_t connectionCount;    /*连接数，即RTP会话数*/

#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, TranStatElement& data)
	{
		in >> data.bandwidth >> data.lost >> data.connectionCount;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, TranStatElement& data)
	{
		out << data.bandwidth << data.lost << data.connectionCount;
		return out;
	}
#endif

	const TranStatElement& operator= ( const TranStatItem& item )
	{
		bandwidth = item.bandwidth;
		lost = item.lost;
		connectionCount = item.connectionCount;
	}
};

/*
* @brief 总传输统计，指应用程序所有传输的统计结果
*/
class TranStatInfo
{
public:
	TranStatElement totalStat;                 /*总统计*/
	TranStatElement totalSendStat;         /*总发送统计*/
	TranStatElement totalReceiveStat;      /*总接收统计*/
	TranStatElement videoSendStat;     /*视频发送统计*/
	TranStatElement videoReceiveStat;      /*视频接收统计*/
	TranStatElement audioSendStat;     /*音频发送统计*/
	TranStatElement audioReceiveStat;  /*音频接收统计*/
#ifndef QT_NO_DATASTREAM
	friend QDataStream& operator>>(QDataStream& in, TranStatInfo& data)
	{
		in >> data.audioReceiveStat >> data.audioSendStat >> data.totalReceiveStat >> data.totalSendStat
			>> data.totalStat >> data.videoReceiveStat >> data.videoSendStat;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, TranStatInfo& data)
	{
		out << data.audioReceiveStat << data.audioSendStat << data.totalReceiveStat << data.totalSendStat
			<< data.totalStat << data.videoReceiveStat << data.videoSendStat;
		return out;
	}
#endif

	static void TranStat2TranStatInfo( const TranStat& tranState , TranStatInfo& info )
	{
		info.audioReceiveStat.bandwidth = tranState.audioReceiveStat.bandwidth;
		info.audioReceiveStat.connectionCount = tranState.audioReceiveStat.connectionCount;
		info.audioReceiveStat.lost = tranState.audioReceiveStat.lost;

		info.audioSendStat.bandwidth = tranState.audioSendStat.bandwidth;
		info.audioSendStat.connectionCount = tranState.audioSendStat.connectionCount;
		info.audioSendStat.lost = tranState.audioSendStat.lost;

		info.videoReceiveStat.bandwidth = tranState.videoReceiveStat.bandwidth;
		info.videoReceiveStat.connectionCount = tranState.videoReceiveStat.connectionCount;
		info.videoReceiveStat.lost = tranState.videoReceiveStat.lost;
		info.videoSendStat.bandwidth = tranState.videoSendStat.bandwidth;
		info.videoSendStat.connectionCount = tranState.videoSendStat.connectionCount;
		info.videoSendStat.lost = tranState.videoSendStat.lost;

		info.totalStat.bandwidth = tranState.totalStat.bandwidth;
		info.totalStat.connectionCount = tranState.totalStat.connectionCount;
		info.totalStat.lost = tranState.totalStat.lost;

		info.totalSendStat.bandwidth = tranState.totalSendStat.bandwidth;
		info.totalSendStat.connectionCount = tranState.totalSendStat.connectionCount;
		info.totalSendStat.lost = tranState.totalSendStat.lost;

		info.totalReceiveStat.bandwidth = tranState.totalReceiveStat.bandwidth;
		info.totalReceiveStat.connectionCount = tranState.totalReceiveStat.connectionCount;
		info.totalReceiveStat.lost = tranState.totalReceiveStat.lost;	
	}

	static void TranStatInfo2TranStat( const TranStatInfo& info , TranStat& state )
	{
		state.audioReceiveStat.bandwidth = info.audioReceiveStat.bandwidth;
		state.audioReceiveStat.connectionCount = info.audioReceiveStat.connectionCount;
		state.audioReceiveStat.lost = info.audioReceiveStat.lost;

		state.audioSendStat.bandwidth = info.audioSendStat.bandwidth;
		state.audioSendStat.connectionCount = info.audioSendStat.connectionCount;
		state.audioSendStat.lost = info.audioSendStat.lost;

		state.videoReceiveStat.bandwidth = info.videoReceiveStat.bandwidth;
		state.videoReceiveStat.connectionCount = info.videoReceiveStat.connectionCount;
		state.videoReceiveStat.lost = info.videoReceiveStat.lost;
		state.videoSendStat.bandwidth = info.videoSendStat.bandwidth;
		state.videoSendStat.connectionCount = info.videoSendStat.connectionCount;
		state.videoSendStat.lost = info.videoSendStat.lost;

		state.totalStat.bandwidth = info.totalStat.bandwidth;
		state.totalStat.connectionCount = info.totalStat.connectionCount;
		state.totalStat.lost = info.totalStat.lost;

		state.totalSendStat.bandwidth = info.totalSendStat.bandwidth;
		state.totalSendStat.connectionCount = info.totalSendStat.connectionCount;
		state.totalSendStat.lost = info.totalSendStat.lost;

		state.totalReceiveStat.bandwidth = info.totalReceiveStat.bandwidth;
		state.totalReceiveStat.connectionCount = info.totalReceiveStat.connectionCount;
		state.totalReceiveStat.lost = info.totalReceiveStat.lost;	
	}
};


//=================================返回值============================

#define		EVENT_R_JOB_DONE				1		//工作已完成
#define		EVENT_R_OK						0	
#define		EVENT_R_NO_INITIAL				-1		//事件没有被创建
#define		EVENT_R_DATA_UNKNOW				-2		//共享内存里的数据无法解析
#define		EVENT_R_TIME_OUT				-3		//等待的事件超时
#define		EVENT_R_THREAD_ERROR			-4		//监听线程出现错误
#define		EVENT_R_THREAD_TERMINATE_FAIL	-5		//监听线程退出失败	
#define		EVENT_R_JOB_FAIL				-6		//工作失败




#endif
