#pragma once

#include <Qtcore/QString>
#include <QtCore/QDataStream>

#define		STREAMED_MEDIA_SERVICE_NAME			"com.dcampus.coolview.channel.type.streamedMedia"
#define		STREAMED_MEDIA_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/channel/type/streamedMedia"

#define STREAMED_MEDIA_PROC_NAME	"CvStreamedMedia"
#define SEND_STREAMED_MEDIA_ID		"send"			//发送媒体流的mediaID
#define SMALL_VIDEO_MEDIA_ID		"smallvideo"	//发送小流的mediaID
#define LOCAL_RECORD_MEDIA_ID		"localrecord"	//本地录制的mediaID
#define LOCAL_PREVIEW_MEDIA_ID		"localpreview"	//本地回显的mediaID
#define FILE_PLAY_MEDIA_ID		"fileplay"	//文件播放的mediaID


//inner functions
//DO NOT call these function outside this file!
inline QString getMediaID( const int virtualIndex, const QString baseID)
{
	if (0 >= virtualIndex)
	{
		return baseID;
	}
	return baseID + QString::number(virtualIndex);
}

//DO NOT call these function outside this file!
inline int getVirtualIndexFromMediaID(const QString id, const QString baseID)
{
	if (id.length() <= baseID.length())
	{
		return 0;
	}
	QString indexStr = id.mid(baseID.length());
	return indexStr.toInt();
}

//DO NOT call these function outside this file!
inline bool isSpecifiedMediaID(const QString id, const QString baseID)
{
	return id.indexOf(baseID) == 0;
}

//send
inline QString getSendMediaID( const int virtualIndex)
{
	return getMediaID(virtualIndex, SEND_STREAMED_MEDIA_ID);
}

inline int getVirtualIndexFromSendMediaID(const QString id)
{
	return getVirtualIndexFromMediaID(id, SEND_STREAMED_MEDIA_ID);
}

inline bool isSendMediaID(const QString id)
{
	return isSpecifiedMediaID(id, SEND_STREAMED_MEDIA_ID);
}

//small video
inline QString getSmallVideoMediaID( const int virtualIndex)
{
	return getMediaID(virtualIndex, SMALL_VIDEO_MEDIA_ID);
}

inline int getVirtualIndexFromSmallMediaID(const QString id)
{
	return getVirtualIndexFromMediaID(id, SMALL_VIDEO_MEDIA_ID);
}

inline bool isSmallVideoMediaID(const QString id)
{
	return isSpecifiedMediaID(id, SMALL_VIDEO_MEDIA_ID);
}

//local record
inline QString getLocalRecordMediaID( const int virtualIndex)
{
	return getMediaID(virtualIndex, LOCAL_RECORD_MEDIA_ID);
}

inline int getVirtualIndexFromLocalRecordMediaID(const QString id)
{
	return getVirtualIndexFromMediaID(id, LOCAL_RECORD_MEDIA_ID);
}

inline bool isLocalRecordMediaID(const QString id)
{
	return isSpecifiedMediaID(id, LOCAL_RECORD_MEDIA_ID);
}

//local preview
inline QString getLocalPreviewMediaID( const int virtualIndex)
{
	return getMediaID(virtualIndex, LOCAL_PREVIEW_MEDIA_ID);
}

inline int getVirtualIndexFromLocalPreviewMediaID(const QString id)
{
	return getVirtualIndexFromMediaID(id, LOCAL_PREVIEW_MEDIA_ID);
}

inline bool isLocalPreviewMediaID(const QString id)
{
	return isSpecifiedMediaID(id, LOCAL_PREVIEW_MEDIA_ID);
}

//file play
inline QString getFilePlayMediaID( const int virtualIndex, const QString &suffix)
{
  //e.g. fileplay0_first
	return FILE_PLAY_MEDIA_ID + QString::number(virtualIndex) + "_" + suffix;
}

inline int getVirtualIndexFromFilePlayMediaID(const QString id)
{
  unsigned long base_len = QString(FILE_PLAY_MEDIA_ID).length();
  if (id.length() <= base_len)
  {
    return 0;
  }
  unsigned long pos = id.indexOf("_");
  QString indexStr = id.mid(base_len, pos);
  return indexStr.toInt();
}

inline bool isFilePlayMediaID(const QString id)
{
	return isSpecifiedMediaID(id, FILE_PLAY_MEDIA_ID);
}

//recv
inline QString getRecvMediaID( const QString& userID )
{
	QString mediaID = QString("Recv_") + userID;

	mediaID.replace("." , "_");
	mediaID.replace("-" , "_");
	mediaID.replace("#" , "_");

	return mediaID;
}

#define userID2MediaID getRecvMediaID //TODO

//util functions
inline QString getStreamedMediaServiceName( const QString& mediaID )
{
	return QString(STREAMED_MEDIA_SERVICE_NAME)+ QString(".") + mediaID;
}

inline QString getStreamedMediaObjectPath( const QString& mediaID )
{
	return QString(STREAMED_MEDIA_SERVICE_OBJECT_PATH)+ QString("/") + mediaID;
}

/************************************************************************/
/* 注释：以下枚举最好有设置值，以避免枚举的值随着定义的次序变化而改变， */
/* 即每个枚举从定义起就有个固定的值                                     */
/*  -- by qhf                                                           */
/************************************************************************/

enum StreamedMediaState
{
	StreamedMedia_NoInitial,
	StreamedMedia_Running,
	StreamedMedia_Pause,
	StreamedMedia_Stop,
	StreamedMedia_Destroyed
};


enum StreamedMediaActionID
{
	Action_InitialRecvGraph,
	Action_InitialSendGraph,
	Action_SetVideoWindow,		//设置媒体流的回显或播放窗口
	Action_AddSendDst,			//往发送媒体流中添加目标地址
	Action_RemoveSendDst,		//往发送媒体流中移除目标地址
	Action_SetQoSInfo,			//设置发送报文的dscp
	Action_ControlVideoSend,	//控制视频流的发送
	Action_ControlAudioSend,	//控制音频流的发送
	Action_EnableAutoResync = 9,   //开启或关闭接收端的自动同步纠正，Action结构值是个bool值
	Action_ControlVideoRecv = 10,		//控制视频的接收，参数为bool值
	Action_ControlAudioRecv = 11,		//控制音频的接收，参数为bool值
	Action_AdaptiveControlCodeRate = 12, //自适应码率控制，参数是bool值
	Action_InitialSmallVideoGraph,
	Action_InitialFilePlayGraph,
	Action_CutRecordingVideo,
    Action_ShowConsoleWindow,
    Action_PlayControl,
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