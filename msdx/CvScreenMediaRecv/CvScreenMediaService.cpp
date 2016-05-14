#include "CvScreenMediaService.h"
#include <dbus/channel/type/screenMedia/service/CvScreenMediaAdaptor.h>
#include <log/Log.h>
#include <windows.h>
#include "ScreenSrc.h"

#include <dbus/channelDispatcher/client/ChannelDispatcherIf.h>
#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>


CvScreenMediaService::CvScreenMediaService( QString mediaID )
{
	new CvScreenMediaAdaptor(this);
	QDBusConnection connection = QDBusConnection::sessionBus();
	QString serviceName = getScreenMediaServiceName(mediaID );
	QString objectPath = getScreenMediaObjectPath(mediaID);
	bool ret = connection.registerService(serviceName);
	ret = connection.registerObject(objectPath ,this);

	this->_currentState = Media_NoInitial;
	this->_mediaDirection = MediaDirection_Unknow;
	this->_mediaID = mediaID;

	this->_imageBuffer = NULL;
	this->_imageBufferMaxLength = 0;
	

	_channelDispatcherProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME,
		CHANNEL_DISPATCHER_OBJECT_PATH , QDBusConnection::sessionBus() );
	_channelDispatcherProxy->ChannelStateChanged( mediaID, _mediaID, SCREEN_MEDIA_SERVICE_NAME , Media_NoInitial);
    
	_screenSrc = NULL;

	if( mediaID=="testrecv" )
	{
		ScreenMediaInfo info;
		info._ipAddress = "127.0.0.1";
		info._port = 32831;
		info._screenWnd.setSize(QSize(0,0 ) );
		
		this->createRecvMedia(info);
	}
}

CvScreenMediaService::~CvScreenMediaService()
{
}


void CvScreenMediaService::Run()
{

}


void CvScreenMediaService::Destroy()
{
	//将服务接口从dbus里面去除
	QDBusConnection connection = QDBusConnection::sessionBus();
	QString serviceName = getScreenMediaServiceName( _mediaID );
	QString objectPath = getScreenMediaObjectPath(_mediaID);
	bool ret = connection.unregisterService(serviceName);
	connection.unregisterObject( objectPath );

	_channelDispatcherProxy->ChannelStateChanged( _mediaID, _mediaID, SCREEN_MEDIA_SERVICE_NAME, Media_Destroyed );

	if( _screenSrc )
	{
		delete _screenSrc;
	}
	//if( _captureWindow!=0 )
	//	UnEmbedBrowserObject((HWND)_captureWindow );
	//删除临时文件
	

	LOG_DEBUG("成功销毁屏幕流");

	QCoreApplication::instance()->quit();	
}

void CvScreenMediaService::Pause()
{

}

void CvScreenMediaService::SetMediaInfo( int action_id, const QByteArray &input_garray )
{
	QDataStream in( input_garray );
	in.setVersion( QDataStream::Qt_4_4 );
	switch( action_id )
	{
	case ScreenMediaAction_InitialRecvMedia:
		{
			ScreenMediaInfo info;
			in >> info;
			createRecvMedia(info);
		};break;

	case ScreenMediaAction_ShowConsoleWindow:
		{
			bool is_show = false;
			in >> is_show;
			LOG_DEBUG("Show console window: %s" , is_show ? "true" : "false" );
			HWND console_window = GetConsoleWindow();
			ShowWindow(console_window, is_show ? SW_SHOW : SW_HIDE);
			return;
		};break;
	}

}

void CvScreenMediaService::Stop()
{

}

QByteArray CvScreenMediaService::GetMediaInfo( int info_type )
{
	QByteArray array;
	return array;
}

int CvScreenMediaService::createRecvMedia( const ScreenMediaInfo& info )
{
	LOG_DEBUG("创建接收屏幕流:IP[%s], Port[%d], Wnd[%x]\n", qPrintable(info._ipAddress),
		info._port ,info._screenWnd  );
    
	_ip = info._ipAddress;
	_port = info._port;
	_captureWindow = info._screenWnd;
	_mediaDirection = MediaDirection_In;

	if( _screenSrc==NULL )
		_screenSrc = new ScreenSrc( info._ipAddress , info._port);

	//if( _captureWindow!=0 )
	//	EmbedBrowserObject( (HWND)_captureWindow );

	return 0;
}



void CvScreenMediaService::displayScreen()
{
	////首先对接收到的图片进行拷贝
	//CopyFile( _recvImageFileName.toStdString().c_str() , _displayImageFileName.toStdString().c_str(), false );
	////UnEmbedBrowserObject((HWND)_captureWindow );

	//

	////在指定窗口中打开jpeg图片
	//if( _captureWindow!=0 )
	//{
	//	//DisplayHTMLStr((HWND)_captureWindow, (TCHAR *)("<h1>正在载入，请稍后</h1>"));
	//	DisplayHTMLPage( (HWND)_captureWindow, _displayImageFileName.toLocal8Bit().data() );
	//}
}