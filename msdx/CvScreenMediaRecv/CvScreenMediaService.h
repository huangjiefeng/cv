#ifndef CV_SCREEN_MEDIA_SERVICE_H
#define CV_SCREEN_MEDIA_SERVICE_H

#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h>
#include <dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h>

typedef unsigned char BYTE;

class ScreenSrc;
class ScreenSnapping;
class ChannelDispatcherIf;

class CvScreenMediaService:public QObject
{
	friend class ScreenSrc;
	Q_OBJECT
public: // PROPERTIES
	CvScreenMediaService( QString mediaID );
	~CvScreenMediaService( );

public Q_SLOTS: // METHODS
	void Destroy();
	QByteArray GetMediaInfo(int info_type);
	void Pause();
	void Run();
	void SetMediaInfo(int action_id, const QByteArray &input_garray);
	void Stop();
Q_SIGNALS: // SIGNALS
	void RunningStateChanged(const QString &media_id, int current_state);


private:
	
	int createRecvMedia( const ScreenMediaInfo& info );
	/**
	 * @brief 在_captureWindow上显示接收到的图片流
	 */
	void displayScreen();
	
	MediaState				_currentState;
	MediaDirection			_mediaDirection;
	QString					_mediaID;
	QRect					_captureWindow;		//将要进行截屏的窗口
	QString					_ip;				//屏幕流的监听IP
	int						_port;				//屏幕流占用的端口
	ScreenSrc*				_screenSrc;
	///存放接收到的图片数据
	BYTE*					_imageBuffer;			//图片缓冲区
	unsigned int			_imageBufferMaxLength;	//图片缓冲区的大小
	unsigned int			_imageLength;			//实际图片的长度


	//用于显示的图片名称
	QString					_displayImageFileName;

	//ChannelDispatcher proxy
	ChannelDispatcherIf*	_channelDispatcherProxy;

	///同步锁。
	QMutex					_mutext;
};


#endif