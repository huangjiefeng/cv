#include "CvTestMediaService.h"
#include <dbus/channel/type/testMedia/service/CvTestMediaAdaptor.h>
#include <Windows.h>
#include <stdio.h>
#include <msdx/ExportUtil.h>
#include <QtCore/QCoreApplication>
#include <log/Log.h>
#include <process.h>

//msdx client dbus proxy

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>


char g_log_msg[512];

/**
 * @brief 创建实例
 * @param mediaID 用于标识每一路媒体流的身份，可以是用户ID
 */
CvTestMedia::CvTestMedia( const QString mediaID )
{
	msdx_initial();
	new CvTestMediaAdaptor(this);
	QDBusConnection connection = QDBusConnection::sessionBus();
	QString serviceName = QString(TEST_MEDIA_SERVICE_NAME) + QString(".") + mediaID;
	QString objectPath = QString(TEST_MEDIA_SERVICE_OBJECT_PATH) + QString("/") + mediaID;
	bool ret = connection.registerService(serviceName);
	ret = connection.registerObject(objectPath ,this);

	this->_currentState = TestMedia_NoInitial;
	this->_instance = this;
	this->_mediaID = mediaID;


	_channelDispatcher = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME,
		CHANNEL_DISPATCHER_OBJECT_PATH, QDBusConnection::sessionBus() );

	_channelDispatcher->ChannelStateChanged( mediaID, _mediaID, TEST_MEDIA_SERVICE_NAME, TestMedia_NoInitial);
}

CvTestMedia::~CvTestMedia()
{
	msdx_uninitial();
}

/**
 * @brief 退出进程
 */
void CvTestMedia::Destroy()
{
	qDebug("Destroy media %s" , _mediaID );

	if (_mediaID == VIDEO_PREVIEW_MEDIA_ID)
	{
		msdx_destroy_video_test_graph();
	}
	else
	{
		msdx_destroy_audio_test_graph();
	}

	QCoreApplication::instance()->quit();	
}

void CvTestMedia::Stop()
{
	return Destroy();
}

void CvTestMedia::Pause()
{
	
}

void CvTestMedia::Run()
{
	qDebug( "Run Graph Command\n");
	int result = -1;
	if( _currentState!=TestMedia_Running  )
	{
		if (_mediaID == VIDEO_PREVIEW_MEDIA_ID)
		{
			result = msdx_run_video_test_graph();
		}
	}
	if( result==0 )
	{
		_currentState = TestMedia_Running;
	}
}

void CvTestMedia::SetMediaInfo( int action_id, const QByteArray &input_garray )
{
	QDataStream out( input_garray );
	out.setVersion(QDataStream::Qt_4_4 );
	switch( action_id )
	{
	case TestMediaAction_InitialDeviceTest:
	case TestMediaAction_InitialFileTest:
		{
			qDebug("Initial Device Test Graph\n");
			if( _currentState!=TestMedia_NoInitial )
			{
				LOG_DEBUG("Test Graph is already initialized");
				return;
			}

			TestMediaInfo info;
			out >> info;

			int result = 0;
			result = CreateAudioTestGraph( info );
			if( result>=0)
			{
				_currentState = TestMedia_Running;

				//调整优先级为实时
				::SetPriorityClass( ::GetCurrentProcess() , HIGH_PRIORITY_CLASS);
			}
			return;
		}
		break;

	case TestMediaAction_InitialPreviewTest:
		{
			qDebug("Initial Preview Test Graph\n");
			if( _currentState!=TestMedia_NoInitial )
			{
				LOG_DEBUG("Test Graph initialized");
				return;
			}

			VideoPreviewInfo info;
			out >> info;
			
			int result = 0;
			result = CreateVideoTestGraph( info );
			if( result>=0)
			{
				_currentState = TestMedia_Running;

				//调整优先级为实时
				::SetPriorityClass( ::GetCurrentProcess() , HIGH_PRIORITY_CLASS);
			}
			return;
		}
		break;
	case TestMediaAction_ShowConsoleWindow:
		{
			bool is_show = false;
			out >> is_show;
			LOG_DEBUG("Show console window: %s" , is_show ? "true" : "false" );
			HWND console_window = GetConsoleWindow();
			ShowWindow(console_window, is_show ? SW_SHOW : SW_HIDE);
			return;
		}
		break;
	}
}

int CvTestMedia::CreateAudioTestGraph( const TestMediaInfo& info )
{
	LOG_PRINTF("Local Audio Test");
	LOG_PRINTF("\tAudioInputDevice: %s \n\tAudioOutputDevice: %s\n"
		, info.audioIutputDevice.toLocal8Bit().data()
		, info.audioOutputDevice.toLocal8Bit().data());

	AudioTestType type;

	if (_mediaID == FILE_TEST_MEDIA_ID)
	{
		type = MSDX_LOCAL_ATYPE_FILE;
	}
	else if (_mediaID == DEVICE_TEST_MEDIA_ID)
	{
		type = MSDX_LOCAL_ATYPE_DEVICE;
	}

	int rs;
	rs = msdx_create_audio_test_graph(type, qPrintable(info.audioIutputDevice), qPrintable(info.audioOutputDevice));
	if(rs < 0)
	{
		qCritical("Create audio test Graph error");
		msdx_destroy_audio_test_graph();
		return -1;
	}

	return 0;
}

int CvTestMedia::CreateVideoTestGraph( const VideoPreviewInfo &info )
{
	LOG_PRINTF("Local Video Test");
	LOG_PRINTF("\tInputDevice: %s \n\tCrossbarDevice: %s \n\tCrossbarType: %s \n\tFormat: %dx%d@%d \n\tWnd: 0x%08x\n"
		, info.videoCaptureDevice.toLocal8Bit().data()
		, info.videoCrossbar.toLocal8Bit().data()
		, info.videoCrossbarType.toLocal8Bit().data()
		, info.width, info.height, info.fps
		, info.preview_wnd);

	int rs = 0;
	rs = msdx_create_video_test_graph(qPrintable(info.videoCaptureDevice), qPrintable(info.videoCrossbar), qPrintable(info.videoCrossbarType), info.width, info.height, info.fps);
	if(rs < 0)
	{
		qCritical("Create video test Graph error");
		msdx_destroy_video_test_graph();
		return -1;
	}

	if (info.preview_wnd != 0)
	{
		RECT WindowRect;
		int xpos = 0;
		int ypos = 0;

		GetWindowRect((HWND)info.preview_wnd, &WindowRect);
		int width = WindowRect.right - WindowRect.left;
		int height = WindowRect.bottom - WindowRect.top;
		if(width != 0 && height != 0)
		{
			float ratio = (float)info.width / info.height;
			if((width / height) >= ratio )
			{
				xpos = (width - height * ratio) / 2;
				width = height * ratio;
			}
			else
			{
				ypos = (height - width / ratio) / 2;
				height = width * 1 / ratio;
			}
		}

		rs = msdx_set_video_test_preview_window((HWND)info.preview_wnd, xpos, ypos, width, height);
		if(rs < 0)
		{
			qCritical("Set preview window of video test Graph error");
			msdx_destroy_audio_test_graph();
			return -1;
		}
	}

	rs = msdx_run_video_test_graph();
	if(rs < 0)
	{
		qCritical("Run video test Graph error");
		msdx_destroy_audio_test_graph();
		return -1;
	}

	return 0;
}


QByteArray CvTestMedia::GetMediaInfo( int info_type )
{
	//没有实现
	QByteArray byteArray;
	return byteArray;
}
