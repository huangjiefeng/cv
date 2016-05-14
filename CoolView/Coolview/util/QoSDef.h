////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 定义跟QoS服务相关的宏、数据、类型等
/// @author	qhf
/// @date	2010-08-15
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	 _QOS_DEFINE_H

//QoS服务器默认的SIP URI
#define	 DEFAULT_QOS_SERVER_SIP_URI  "sip:qosserver@sip.ccnl.scut.edu.cn"

//QoS服务器的主机IP
#define  DEFAULT_QOS_SERVER_HOST_IP  "202.38.192.222"

//QoS服务器的UDP端口，用于接收rtp统计报告
#define  DEFAULT_QOS_SERVER_UDP_PORT    9981

//QoS服务器的TCP端口，用于与QoS客户端通信
#define  DEFAULT_QOS_SERVER_TCP_PORT    5150



#endif	//_QOS_DEFINE_H