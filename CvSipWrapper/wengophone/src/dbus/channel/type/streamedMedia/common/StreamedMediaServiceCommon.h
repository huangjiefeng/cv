#pragma once

#define		STREAMED_MEDIA_SERVICE_NAME			"com.dcampus.coolview.channel.type.streamedMedia"
#define		STREAMED_MEDIA_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/channel/type/streamedMedia"

#define STREAMED_MEDIA_PROC_NAME	"CvStreamedMedia"
#define SEND_STREAMED_MEDIA_ID		"send"			//发送媒体流的mediaID

inline QString userID2MediaID( const QString& userID )
{
	QString mediaID = QString("Recv_") + userID;

	mediaID.replace("." , "_");
	mediaID.replace("-" , "_");

	return mediaID;
}

inline QString getServiceName( const QString& mediaID )
{
	return QString(STREAMED_MEDIA_SERVICE_NAME)+ QString(".") + mediaID;
}

inline QString getObjectPath( const QString& mediaID )
{
	return QString(STREAMED_MEDIA_SERVICE_OBJECT_PATH)+ QString("/") + mediaID;
}

enum StreamedMediaState
{
	StreamedMedia_NoInitial,
	StreamedMedia_Running,
	StreamedMedia_Pause,
	StreamedMedia_Stop,
	StreamedMedia_Destroyed
};

enum MediaDirection
{
	MediaDirection_Unknow,
	MediaDirection_In,
	MediaDirection_out
};

enum StreamedMediaActionID
{
	Action_InitialRecvGraph,
	Action_InitialSendGraph,
	Action_SetVideoWindow,		//设置媒体流的回显或播放窗口
	Action_AddSendDst,			//往发送媒体流中添加目标地址
	Action_RemoveSendDst,		//往发送媒体流中移除目标地址
	Action_SetQoSInfo				//设置发送报文的dscp
};

enum QoSNotify
{
	QoSNotify_RtpState,			//Rtcp信息
	QoSNotify_TranState			//总的网络信息
};

enum MediaInfoType
{
	Info_TranState				//传输统计信息
};