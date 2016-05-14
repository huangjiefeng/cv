#pragma once

#define TELECONTROLLER_SERVICE_NAME			"com.dcampus.coolview.controller.telecontroller"
#define TELECONTROLLER_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/controller/telecontroller"

#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QVector>

inline QString _GetTelecontrollerServiceName(const QString& type )
{
	return QString(TELECONTROLLER_SERVICE_NAME) + "." + type;
}

inline QString _GetTelecontrollerObjectPath( const QString& type )
{
	return QString(TELECONTROLLER_SERVICE_OBJECT_PATH) + "/" + type;
}


enum TelecontrollerInfoIndex
{
	TELE_ExitProcess = 0,
	TELE_Layout = 1,
	TELE_MemberLocation = 2,		//将会议室UI里面所有的布局信息发送给遥控器，通常在遥控器刚开始连接上终端的时候使用
	TELE_Devices = 3,
	TELE_QueryLayout = 4,
	TELE_QueryMemberLocation = 5,
	TELE_QueryDevices = 6,
	TELE_MediaState = 7,
	TELE_RemoveMemeberLocation = 8,	//终端移除接收的成员
	TELE_AddMemberLocation = 9,		//终端添加接收成员
	TELE_ChangeMemberLocation = 10,	//终端更改接收成员的位置
	TELE_CurrentController = 11,     //终端当前的控制权所有者
	TELE_ApplyAccess = 12,           //非控制权所有者请求控制权限
	TELE_ApplyResult = 13,           //返回控制权限请求结果
	TELE_NewNotifyIndex = 14,        //向遥控器发送通告连接索引号
	TELE_PostMeetingInfo = 15,           //终端进入会议时向TeleController传递当前会议信息，如主席账号
	TELE_QueryScreenShareState = 16 , //向终端查询屏幕共享状态 
	TELE_ScreenShareState =17 ,		  //接收到屏幕共享状态信息 0->没有开启本地共享  1->已经开启了本地共享
	TELE_TerminalConfig = 18,        //向遥控器返回终端配置信息
	TELE_PanelState = 19,           //向遥控器返回控制面板信息
};

//记录指定成员媒体流状态的变更
class MediaStateNotify
{
public:
	QString		_memberName;		//用户名，不包含域名的SIP账号名
	QString		_mediaType;			//媒体类型，为audio或video
	QString		_mediaState;		//媒体状态，为stop或run
	friend QDataStream& operator>>(QDataStream& in, MediaStateNotify& data)
	{
		in >> data._memberName >> data._mediaType >> data._mediaState;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, MediaStateNotify& data)
	{
		out << data._memberName << data._mediaType << data._mediaState;
		return out;
	}
};

//记录会议成员在会议室UI里的位置信息
class MemberLocation
{
public:
	QString		_memberName;
	int			_screenIndex;
	int			_seet;
	bool		_audioEnable;
	bool		_videoEnable;

	friend QDataStream& operator>>(QDataStream& in, MemberLocation& data)
	{
		in >> data._memberName >> data._screenIndex >> data._seet >> data._audioEnable >> data._videoEnable;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, MemberLocation& data)
	{
		out << data._memberName << data._screenIndex << data._seet << data._audioEnable << data._videoEnable;
		return out;
	}
};

//记录每块屏幕的信息，目前主要有布局
class ScreenInfo
{
public:
	QString		_layout;			//"1"=1X1,"2"=2X2,"3"=3X3,"L1_5"=L(1+5),"L1_20"=L(1+20)
	int			_screenIndex;
	friend QDataStream& operator>>(QDataStream& in, ScreenInfo& data)
	{
		in >> data._screenIndex >> data._layout;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, ScreenInfo& data)
	{
		out << data._screenIndex << data._layout;
		return out;
	}
};

class CrossBarDevice
{
public:
	QString		_crossbarDevice;
	QStringList	_crossbarTypeList;
	friend QDataStream& operator >> (QDataStream& in , CrossBarDevice&data )
	{
		in >> data._crossbarDevice >> data._crossbarTypeList;
		return in;
	}

	friend QDataStream& operator << (QDataStream& out , const CrossBarDevice&data )
	{
		out << data._crossbarDevice << data._crossbarTypeList;
		return out;
	}
};

class VideoCaptureDevice
{
public:
	QString _captureDevice;
	QStringList _mediaTypeList;			
	friend QDataStream& operator >> (QDataStream& in , VideoCaptureDevice&data )
	{
		in >> data._captureDevice >> data._mediaTypeList;
		return in;
	}

	friend QDataStream& operator << (QDataStream& out , const VideoCaptureDevice&data )
	{
		out << data._captureDevice << data._mediaTypeList;
		return out;
	}
};

class VideoDevice
{
public:
	QVector<VideoCaptureDevice>	_videoCaptureDevices;
	QVector<CrossBarDevice>		_crossbarDevices;
	int			_videoQuality;			//视频质量滑动条的值
	
	friend QDataStream& operator >> ( QDataStream &in, VideoDevice& data)
	{
		int videoCaptureNum;
		in >> videoCaptureNum;
		for( int i=0 ; i<videoCaptureNum ; i++ )
		{
			VideoCaptureDevice device;
			in >> device;
			data._videoCaptureDevices.push_back(device);
		}

		int crossbarNum;
		in >> crossbarNum;
		for( int i=0 ; i<crossbarNum; i++ )
		{
			CrossBarDevice device;
			in >> device;
			data._crossbarDevices.push_back(device);
		}

		in >> data._videoQuality;

		return in;
	}

	friend QDataStream& operator << ( QDataStream &out, const VideoDevice& data)
	{
		int videoCaptureNum = data._videoCaptureDevices.size();
		out << videoCaptureNum;
		for( int i=0 ; i<videoCaptureNum ; i++ )
		{
			out << data._videoCaptureDevices.at(i);
		}

		int crossbarNum = data._crossbarDevices.size();
		out << crossbarNum;
		for( int i=0 ; i<crossbarNum ; i++ )
		{
			out << data._crossbarDevices.at(i);
		}

		out << data._videoQuality;

		return out;
	}

};

class AudioDevice
{
public:
	QStringList		_audioInputDeviceList;
	int				_audioInputVolumn;
	QStringList		_audioOutputDeviceList;
	int				_audioOutputVolumn;
	friend QDataStream& operator >> ( QDataStream &in, AudioDevice& data)
	{
		in >> data._audioInputDeviceList >> data._audioInputVolumn >> data._audioOutputDeviceList >> data._audioOutputVolumn;
		return in;
	}
	friend QDataStream& operator << ( QDataStream &out, const AudioDevice& data)
	{
		out << data._audioInputDeviceList << data._audioInputVolumn << data._audioOutputDeviceList << data._audioOutputVolumn;
		return out;
	}
};

class TerminalDevice
{
public:
	VideoDevice		_videoDevice;
	CrossBarDevice	_crossbarDevice;
	AudioDevice		_audioDevice;
	friend QDataStream&	operator >> (QDataStream& in , TerminalDevice& data )
	{
		in >> data._videoDevice >> data._crossbarDevice >> data._audioDevice;
		return in;
	}
	friend QDataStream& operator << (QDataStream&out , const TerminalDevice& data )
	{
		out << data._videoDevice << data._crossbarDevice << data._audioDevice;
		return out;
	}
};