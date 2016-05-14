#ifndef TEST_MEDIA_H
#define TEST_MEDIA_H

#include <Qtcore/QString>
#include <QtCore/QDataStream>

#define		TEST_MEDIA_SERVICE_NAME			"com.dcampus.coolview.channel.type.testMedia"
#define		TEST_MEDIA_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/channel/type/testMedia"

#define TEST_MEDIA_PROC_NAME	"CvTestMedia"

#define FILE_TEST_MEDIA_ID   "TestAudioOutput"
#define DEVICE_TEST_MEDIA_ID "TestAudioCapture"
#define VIDEO_PREVIEW_MEDIA_ID "TestVideoCapture"


inline QString getTestMediaServiceName( const QString& mediaID )
{
	return QString(TEST_MEDIA_SERVICE_NAME)+ QString(".") + mediaID;
}

inline QString getTestMediaObjectPath( const QString& mediaID )
{
	return QString(TEST_MEDIA_SERVICE_OBJECT_PATH)+ QString("/") + mediaID;
}


enum TestMediaState
{
	TestMedia_NoInitial,
	TestMedia_Running,
	TestMedia_Pause,
	TestMedia_Stop,
	TestMedia_Destroyed
};


enum TestMediaActionID
{
	TestMediaAction_InitialFileTest,
	TestMediaAction_InitialDeviceTest,
    TestMediaAction_InitialPreviewTest,
    TestMediaAction_ShowConsoleWindow,
};

class TestMediaInfo
{
public:

	///输入设备名称或声音文件设备
	QString		audioIutputDevice;
	//声音输出设备名称
	QString     audioOutputDevice;


	friend QDataStream& operator>>(QDataStream& in, TestMediaInfo& data)
	{
		in >> data.audioIutputDevice >> data.audioOutputDevice;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const TestMediaInfo& data)
	{
		out << data.audioIutputDevice << data.audioOutputDevice;
		return out;
	}
};

class VideoPreviewInfo
{
public:
	
	QString     videoCaptureDevice;
	QString		videoCrossbar;		        //crossbar Name。用于支持高清
	QString		videoCrossbarType;          //crossbarInputType,设置crossbar的输入类型
	unsigned short	width;			        //视频帧的宽度，单位为像素
	unsigned short	height;			        //视频帧的高度，单位为像素
	unsigned short	fps;			        //视频的帧率，单位为帧/秒
	int			preview_wnd;				//预览的播放窗口
	        

	friend QDataStream& operator>>(QDataStream& in, VideoPreviewInfo& data)
	{
		in >> data.videoCaptureDevice >> data.videoCrossbar >> data.videoCrossbarType 
			>> data.width >> data.height >> data.fps >>data.preview_wnd ;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const VideoPreviewInfo& data)
	{
		out << data.videoCaptureDevice << data.videoCrossbar << data.videoCrossbarType 
			<< data.width << data.height << data.fps <<data.preview_wnd ;

		return out;
	}
};



#endif