#pragma once

#include <Qtcore/QString>
#include <QtCore/QDataStream>
#include <QRect>

#define		SCREEN_MEDIA_SERVICE_NAME			"com.dcampus.coolview.channel.type.screenMedia"
#define		SCREEN_MEDIA_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/channel/type/screenMedia"

#define RECV_SCREEN_MEDIA_PROC_NAME	"CvScreenMediaRecv"
#define SEND_SCREEN_MEDIA_PROC_NAME "CvScreenMediaSend"
#define SEND_SCREEN_MEDIA_ID	"ScreenSend"			//发送屏幕流的mediaID
#define RECV_SCREEN_MEDIA_ID	"ScreenRecv"			//接收屏幕流的mediaID


inline QString getScreenMediaServiceName( const QString& mediaID )
{
	return QString(SCREEN_MEDIA_SERVICE_NAME)+ QString(".") + mediaID;
}

inline QString getScreenMediaObjectPath( const QString& mediaID )
{
	return QString(SCREEN_MEDIA_SERVICE_OBJECT_PATH)+ QString("/") + mediaID;
}


enum ScreenMediaActionID
{
	ScreenMediaAction_InitialRecvMedia,
	ScreenMediaAction_InitialSendMedia,
	ScreenMediaAction_AddSendDst,			//往发送媒体流中添加目标地址
	ScreenMediaAction_RemoveSendDst,		//往发送媒体流中移除目标地址
	ScreenMediaAction_ShowConsoleWindow,
};

class ScreenMediaInfo
{
public:

	///网络信息
	QString		_ipAddress;
	int			_port;

	///截屏的位置信息
	QRect		_screenWnd;		//截屏界面的窗口矩形
	friend QDataStream& operator>>(QDataStream& in, ScreenMediaInfo& data)
	{
		in >> data._ipAddress >> data._port >> data._screenWnd;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const ScreenMediaInfo& data)
	{
		out << data._ipAddress << data._port << data._screenWnd;
		return out;
	}
};