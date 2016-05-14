////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief RTP统计参数，与RTCP相结合。该头文件可能被多个Project使用，因此需要尽量减少其依赖的项目数，
/// 同时将其代码实现也写在本头文件中。
/// @author qhf
/// @date   2010-07-15
/// modify history:
/// 2011-6-20  添加组播地址字段
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef  RTP_STAT_H
#define  RTP_STAT_H

#include <stdio.h>
#include <string.h>

#define MAX_IP_STRING_LENGTH 60

//重定义一些数据结构
typedef unsigned char bool_t;
typedef unsigned __int64 uint64_t;
typedef  unsigned int uint32_t;

/************************************************************************/
/*  定义一些载荷类型，其值要与RTPInfo.h中定义的RTP载荷类型相等，可根据
该类型来判断是否音视频数据。
这里再进行定义是为了避免依赖RTPInfo.h文件。
以下定义都是系统目前支持的媒体类型
*/
/************************************************************************/
#define MEDIA_PT_VIDEO_MP4T     35  //MPEG4视频数据
#define MEDIA_PT_VIDEO_H264     43  //H264视频数据 
#define MEDIA_PT_AUDIO              50  //H264视频数据 
#define MEDIA_PT__IMAGE    44                    //截屏的图像


#ifdef __cplusplus
extern "C"
{
#endif

    //RTP会话参与者角色类型，发送者或接收者
    enum RtpEndpointType
    {
        eET_Sender = 1,       //发送者
        eET_Receiver = 2     //接收者          
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief  网络流的媒体类型（不涉及编码格式），如表示成视频、音频、或其他
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    enum StreamMediaType
    {
        eMT_Video = 0, //视频
        eMT_Audio = 1, //音频
        eMT_Other = 2 //其他，如PPT、白板等		
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief     rtp统计项，包括发送（接收）速率、丢包率、延时、抖动等参数，其值取自RTP会话某个时刻的
    ///  快照。本结构一般比较固定，不轻易改变。
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    typedef struct _RtpStatItem
    {
        uint32_t ssrc;                  /*RTP会话标识，一般取自发送端的SSRC*/
        uint32_t payload_type;              /*RTP载荷类型，对应编码类型，比如H264、speex*/
        enum StreamMediaType media_type;  /*媒体类型，指视频、音频等，不细分到编码格式*/

        enum RtpEndpointType rtp_endpoint_type;  /*参与者角色，以此识别是发送端或接收端的统计数据*/
        char rem_addr[MAX_IP_STRING_LENGTH];  /*RTP传输的目标IP地址，如果自己是发送者，则目标是接收者，反之亦然. 注意，原来该字段是sockaddr_storage类型，但考虑到winsock头文件版本容易引发编译问题，所以改成字符串*/
        int rem_addr_len;           /* IP地址长度*/
        int rem_port;       /*远程主机端口*/
        char local_addr[MAX_IP_STRING_LENGTH];  /*RTP传输的本地IP地址，该值一般是实际IP地址，但也有可能是回环地址或零地址*/
        int local_addr_len;         /* 本地IP地址长度*/
        int local_port;       /*本地主机端口*/
		bool_t  isMulticast;	  //是否组播
		char  mcast_addr[MAX_IP_STRING_LENGTH];   //组播地址
		int  mcast_addr_len;          // mcast_addr长度
		
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
        uint64_t timestamp;  /* 时间戳，用来描述产生报告的时刻，单位为ms*/
        uint32_t interval;      /*  采集时间间隔，单位ms，默认值为5000*/

        int graph_id;   /*directshow graph的id，如果使用msdx管理rtp传输，则graphid等于rtp传输所在的（发送/接收）graph的Id；如果与directshow无关，则该值一般为0，或作为用处*/
		char member_id[MAX_IP_STRING_LENGTH];	/* 媒体流成员的账号名称，如果是发送媒体流则为send*/

    } RtpStatItem;

    //回调函数所需要的参数
    typedef struct _RtpStatCallbackParam
    {
        void* pClass;
        RtpStatItem* pRtpStat;
    } RtpStatCallbackParam;

    /*
     * @brief 传输统计，可以是多个连接的传输统计
     */
    typedef struct  _TranStatItem
    {
        float bandwidth;    /*带宽，单位kb/s*/
        float lost;             /*丢包率，单位百分比%*/
        uint32_t connectionCount;       /*连接数，即RTP会话数*/
    } TranStatItem;

    /*
     * @brief 总传输统计，指应用程序所有传输的统计结果
     */
    typedef struct _TranStat
    {
        TranStatItem totalStat;                 /*总统计*/
        TranStatItem totalSendStat;         /*总发送统计*/
        TranStatItem totalReceiveStat;      /*总接收统计*/
        TranStatItem videoSendStat;     /*视频发送统计*/
        TranStatItem videoReceiveStat;      /*视频接收统计*/
        TranStatItem audioSendStat;     /*音频发送统计*/
        TranStatItem audioReceiveStat;  /*音频接收统计*/
    } TranStat;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief  rtp统计项回调函数指针类型，用于传递统计结果.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef void (*RtpStatCallback) (void* pClass, const RtpStatItem* rtpstat);

	/**
	 * @brief 传输总统计回调函数指针类型，
	 *				pClass用于保存上层传递下来的一个指针，下层将不做修改，抛回给上层。
	 */
	typedef void (*TranStatCallback)(void * pClass, const TranStat * transtat);

	////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief   Rtp stat item initialise.
    /// @param [in,out] rtpstat If non-null, the rtpstat.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static inline  void rtpStatItem_init(RtpStatItem* rtpstat)
    {
        memset(rtpstat, 0, sizeof(*rtpstat));
        rtpstat->ssrc = 0;
        rtpstat->payload_type = 0;
        rtpstat->media_type = eMT_Video;

        rtpstat->rtp_endpoint_type = eET_Sender;
        //memset(rtpstat->remaddr, 0, sizeof(rtpstat->remaddr));
        rtpstat->rem_addr_len = 0;
        rtpstat->rem_port = 0;
        //memset(rtpstat->localaddr, 0, sizeof(rtpstat->localaddr));
        rtpstat->local_addr_len = 0;
        rtpstat->local_port = 0;
		rtpstat->isMulticast = 0;
		//memset(rtpstat->mcast_addr, 0, sizeof(rtpstat->mcast_addr));
		rtpstat->mcast_addr_len =0;

        rtpstat->packet_sent = 0;
        rtpstat->sent = 0;
        rtpstat->packet_recv = 0;
        rtpstat->hw_recv = 0;
        rtpstat->recv = 0;
        rtpstat->outoftime = 0;
        rtpstat->cum_packet_loss = 0;
        rtpstat->discarded = 0;

        rtpstat->send_bandwidth = 0;
        rtpstat->recv_bandwidth = 0;

        rtpstat->lost = 0;
        rtpstat->jitter = 0;
        rtpstat->delay = 0;

        rtpstat->seqnumber = 0;
        rtpstat->timestamp = 0;
        rtpstat->interval = 5000;

        rtpstat->graph_id = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief  重新设置RtpStatItem的统计参数值，但不包括ssrc、pt、remaddr等固定值（相对而言）
    /// @param [in,out] rtpstat If non-null, the rtpstat.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static inline  void rtpStatItem_reset_stat(RtpStatItem* rtpstat)
    {
		rtpstat->isMulticast = 0;
		memset(rtpstat->mcast_addr, 0, sizeof(rtpstat->mcast_addr));
		rtpstat->mcast_addr_len =0;

        rtpstat->packet_sent = 0;
        rtpstat->sent = 0;
        rtpstat->packet_recv = 0;
        rtpstat->hw_recv = 0;
        rtpstat->recv = 0;
        rtpstat->outoftime = 0;
        rtpstat->cum_packet_loss = 0;
        rtpstat->discarded = 0;

        rtpstat->send_bandwidth = 0;
        rtpstat->recv_bandwidth = 0;

        rtpstat->lost = 0;
        rtpstat->jitter = 0;
        rtpstat->delay = 0;

        rtpstat->seqnumber = 0;
        rtpstat->timestamp = 0;
    }


    //初始化TranStatItem结构变量
    static inline void tranStatItem_init(TranStatItem* item)
    {
        item->bandwidth = 0;
        item->lost = 0;
        item->connectionCount = 0;
    }

    //初始化TranStat结构变量
    static inline void tranStat_init(TranStat* stat)
    {
        tranStatItem_init(&stat->totalStat);
        tranStatItem_init(&stat->totalSendStat);
        tranStatItem_init(&stat->totalReceiveStat);
        tranStatItem_init(&stat->videoSendStat);
        tranStatItem_init(&stat->videoReceiveStat);
        tranStatItem_init(&stat->audioSendStat);
        tranStatItem_init(&stat->audioReceiveStat);
    }

    //打印RtpStatItem信息，输出到控制台
    static void printRtpStat(void* pClass, const RtpStatItem* rtpstat)
    {
        printf("\n====================================\n");
        printf("current rtp stat:\n");
        printf("rtp session ssrc:\t %0x \n", rtpstat->ssrc);
        printf("endpoint type:\t%s\n", rtpstat->rtp_endpoint_type == eET_Sender ? "sender" : "receiver");
        printf("media type:\t%s\n", rtpstat->media_type == eMT_Video ? "video" : "audio");
        printf("payload type:\t %d \n", rtpstat->payload_type);
        printf("remote ip:\t %s:%d \n", rtpstat->rem_addr, rtpstat->rem_port);
        printf("local ip:\t %s:%d \n", rtpstat->local_addr, rtpstat->local_port);
		printf("is multicast transmission:\t %s \n", rtpstat->isMulticast?"true":"false");
		if (rtpstat->isMulticast)
		{
			printf("multicast ip:\t %s \n", rtpstat->mcast_addr);
		}

        printf("send_bandwidth:\t %.2f kb/s\n", rtpstat->send_bandwidth);
        printf("recv_bandwidth:\t %.2f kb/s\n", rtpstat->recv_bandwidth);

        printf("lost:\t %.2f %% \n", rtpstat->lost);
        printf("jitter:\t %d \n", rtpstat->jitter);
        printf("delay:\t %d \n", rtpstat->delay);

        printf("packet_sent:\t %I64d \n", rtpstat->packet_sent);
        printf("sent:\t %I64d bytes\n", rtpstat->sent);
        printf("packet_recv:\t %I64d \n", rtpstat->packet_recv);
        printf("hw_recv:\t %I64d bytes\n", rtpstat->packet_recv);
        printf("recv:\t %I64d bytes\n", rtpstat->recv);
        printf("outoftime:\t %I64d \n", rtpstat->outoftime);
        printf("cum_packet_loss:\t %I64d \n", rtpstat->cum_packet_loss);
        printf("discarded:\t %I64d \n", rtpstat->discarded);

        printf("stat seqnumber:\t %d \n", rtpstat->seqnumber);
        printf("stat timestamp:\t %I64d \n", rtpstat->timestamp);
        printf("stat interval:\t %d \n", rtpstat->interval);

		printf("identifier:\t %d \n", rtpstat->graph_id);

        printf("====================================\n\n");
    }

    //输出统计信息（TranStat）到控制台
    static void printTranStat(void* pClass, const TranStat* transtat)
    {
        printf("\n*********************Global Transmit Stat*********************\n");
        printf("1、total stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->totalStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->totalStat.lost);
        printf("  connection count:\t %d \n", transtat->totalStat.connectionCount);

        printf("2、total send stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->totalSendStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->totalSendStat.lost);
        printf("  connection count:\t %d \n", transtat->totalSendStat.connectionCount);

        printf("3、total receive stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->totalReceiveStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->totalReceiveStat.lost);
        printf("  connection count:\t %d \n", transtat->totalReceiveStat.connectionCount);

        printf("4、total video send stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->videoSendStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->videoSendStat.lost);
        printf("  connection count:\t %d \n", transtat->videoSendStat.connectionCount);

        printf("5、total video receive stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->videoReceiveStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->videoReceiveStat.lost);
        printf("  connection count:\t %d \n", transtat->videoReceiveStat.connectionCount);

        printf("6、total audio send stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->audioSendStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->audioSendStat.lost);
        printf("  connection count:\t %d \n", transtat->audioSendStat.connectionCount);

        printf("7、total audio receive stat:\n");
        printf("  bandwidth:\t %.2f kb/s \n", transtat->audioReceiveStat.bandwidth);
        printf("  lost:\t  %.2f %% \n", transtat->audioReceiveStat.lost);
        printf("  connection count:\t %d \n", transtat->audioReceiveStat.connectionCount);

        printf("**************************************************************\n\n");
    }

#ifdef __cplusplus
}
#endif

#endif