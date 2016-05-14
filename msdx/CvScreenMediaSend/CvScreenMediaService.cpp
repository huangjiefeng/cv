#include "CvScreenMediaService.h"
#include <dbus/channel/type/screenMedia/service/CvScreenMediaAdaptor.h>
#include <log/Log.h>
#include <windows.h>
#include "CSnapScreen.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QRect>
#include <QtNetwork/QHostAddress>
#include "ScreenDst.h"

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
	this->_sendImageTimer = new QTimer(this );
	connect( _sendImageTimer , SIGNAL(timeout() ) , this ,SLOT(sendImage() ) );
	
	
	_snapper = new ScreenSnapping( );
	QString filePath = QApplication::applicationDirPath();
	filePath.replace("/" , "\\");
	filePath += "\\ScreenCaptrue.jpg";
	_snapper->setStorageFilePath( filePath.toStdString() );

	_channelDispatcherProxy = new ChannelDispatcherIf( CHANNEL_DISPATCHER_SERVICE_NAME , CHANNEL_DISPATCHER_OBJECT_PATH,
		QDBusConnection::sessionBus() );

	_channelDispatcherProxy->ChannelStateChanged( mediaID , mediaID, SCREEN_MEDIA_SERVICE_NAME , Media_NoInitial );

	
	if( mediaID=="testsend" )
	{
		ScreenMediaInfo info;
		info._ipAddress = "127.0.0.1";
		info._port = 32831;
		info._screenWnd.setSize(QSize(0,0 ) );

		this->createSendMedia(info );	

		this->addSendMedia("127.0.0.1" );
	}
}

CvScreenMediaService::~CvScreenMediaService()
{

}


void CvScreenMediaService::Run()
{
	RECT WindowRect;
	if( _mediaDirection==MediaDirection_out )
	{
		_currentState = Media_Running;

		this->sendImage();

		_sendImageTimer->start( 1000 / _snapper->frameRate );
		//captureScreen();

	}
}


void CvScreenMediaService::Destroy()
{
	//将服务接口从dbus里面去除
	QDBusConnection connection = QDBusConnection::sessionBus();
	QString serviceName = getScreenMediaServiceName( _mediaID );
	QString objectPath = getScreenMediaObjectPath(_mediaID);
	bool ret = connection.unregisterService(serviceName);
	connection.unregisterObject( objectPath );

	LOG_DEBUG("开始销毁屏幕流");
	if( _sendImageTimer )
	{
		_sendImageTimer->stop();
		delete _sendImageTimer;
		_sendImageTimer = NULL;
	}

	if( _snapper )
	{
		delete _snapper;
		_snapper = NULL;
	}

	_mutext.lock();
	//清空所有发送Socket
	ScreenDistList::iterator it = _sendDistList.begin();
	for( ; it!=_sendDistList.end() ; it++ )
	{
		(*it)->release();
	}

	_mutext.unlock();

	_channelDispatcherProxy->ChannelStateChanged(_mediaID, _mediaID, SCREEN_MEDIA_SERVICE_NAME,
		Media_Destroyed );

	LOG_DEBUG("成功销毁屏幕流");

	QCoreApplication::instance()->quit();	
}

void CvScreenMediaService::Pause()
{
	_currentState = Media_Pause;
	_sendImageTimer->stop();
}

void CvScreenMediaService::SetMediaInfo( int action_id, const QByteArray &input_garray )
{
	QDataStream in( input_garray );
	in.setVersion( QDataStream::Qt_4_4 );
	switch( action_id )
	{
	case ScreenMediaAction_InitialSendMedia:
		{
			ScreenMediaInfo info;
			in >> info;
			createSendMedia(info);
		};break;
	case ScreenMediaAction_AddSendDst:
		{
			QString dstIP;
			in >> dstIP;
			addSendMedia( dstIP );
			
		};break;
	case ScreenMediaAction_RemoveSendDst:
		{
			QString dstIP;
			in >> dstIP;
			removeSendMedia( dstIP );
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
	_currentState = Media_Stop;
	_sendImageTimer->stop();
}

QByteArray CvScreenMediaService::GetMediaInfo( int info_type )
{
	QByteArray array;
	return array;
}

int CvScreenMediaService::createSendMedia( const ScreenMediaInfo& info )
{
	LOG_DEBUG("创建发送屏幕流:IP[%s], Port[%d], Wnd[%x]\n", qPrintable(info._ipAddress),
		info._port ,info._screenWnd  );
    
	_port = info._port;
	_captureWindow = info._screenWnd;		//屏幕截取的地方

	_mediaDirection = MediaDirection_out;

	this->Run();

	return 0;
}


int CvScreenMediaService::addSendMedia( const QString& dstIP )
{
	LOG_DEBUG("添加发送目标:%s" , qPrintable(dstIP)  );

	ScreenDist* pScreenDist = NULL;
	ScreenDistList::iterator it = _sendDistList.begin();
	for( ; it!=_sendDistList.end(); it++ )
	{
		pScreenDist = *it;
		if( pScreenDist->_ip==dstIP )
		{
			if( pScreenDist->_isBusy )
			{
				//如果IP已经存在且正在发送数据，那么直接返回
				LOG_DEBUG("目标已经存在并且正在发送数据.");
			}else
			{
				//否则启动该socket并返回
				LOG_DEBUG("目标已经存在，重新启动发送");
				pScreenDist->resetDst( dstIP , pScreenDist->_port );
			}
			return 0;
		}
	}

	_mutext.lock();

	//获取空闲的网络资源
	it = _sendDistList.begin();
	for( ; it!=_sendDistList.end(); it++ )
	{
		pScreenDist = *it;
		if( pScreenDist->_isBusy==false )
		{
			LOG_DEBUG("获取空闲的网络资源并重新建立连接");
			pScreenDist->resetDst( dstIP , _port );
			break;
		}
	}

	if( it==_sendDistList.end() )
	{
		//没有空闲的资源，则创建相应的socket并连接
		LOG_DEBUG("创建新的网络连接");
		ScreenDist* dst = new ScreenDist( dstIP , _port);
		_sendDistList.push_back( dst );
	}


	_mutext.unlock();

	LOG_DEBUG("添加目标成功！");

	return 0;
}

int CvScreenMediaService::sendImage( )
{
	if( _sendDistList.size()==0 )
	{
		//没有发送目标，不进行截屏
		return 0;
	}

	if( captureScreen()>0 )
	{
		//同步
		_mutext.lock();

		BYTE*	data = _snapper->_imageBuffer;
		quint32 dataLength = _snapper->_imageLength;
		ScreenDist*	pScreenDst = NULL;
		ScreenDistList::iterator it = _sendDistList.begin();
		for( ; it!=_sendDistList.end();it++ )
		{
			pScreenDst = *it;
			if( pScreenDst->_isBusy )
			{
				QTcpSocket* socket = &(pScreenDst->_socket);
				if( socket->isValid()&&socket->state()==QAbstractSocket::ConnectedState )
				{
					qDebug("Send Screen to %s,%d" , qPrintable(socket->peerAddress().toString()) , socket->peerPort() );

					quint32 packetCount = 0;
					quint32 blockSize = 5120;

					//发送开始传送文件的标志
					quint16 key = 0x0001;
					quint32 packetSize = sizeof(key);
					socket->write((char*)&packetSize , sizeof(quint32) );
					socket->write( (char*)&key, sizeof(quint16) );
					if(!socket->waitForBytesWritten(-1))
						qDebug() << "Send Start File Error: ";
					else
						qDebug() << "Send Start File";

					//如果该socket已经准备好了，那么直接发送数据
					do
					{
						//0x0002表明后面的数据是图像内容
						key = 0x0002;
						packetSize =0;		//单次写入socket的数据大小
						quint32 sendDataLength = 0;

						qint32 offset = packetCount*blockSize;
						if( offset>dataLength )
							break;

						if( offset+blockSize<dataLength )
							sendDataLength = blockSize;
						else
							sendDataLength = dataLength - offset;	//将剩余的内容进行打包

						packetSize = sizeof(key) + sendDataLength;

						//首先写入数据的长度
						socket->write( (char*)&packetSize , sizeof(quint32) );

						//写入头部的key，然后写入数据内容
						socket->write( (char*)&key , sizeof(quint16) );
						if( offset+blockSize< dataLength )
							socket->write( (char*)(data+offset) , sendDataLength );
						else
							socket->write( (char*)(data+offset) , sendDataLength);		
						packetCount++;

						if(!socket->waitForBytesWritten(-1))
						{
							LOG_ERROR("writen Bytes error ");
							break;
						}
					}while(true);


					//发送数据结束符
					key = 0x0003;
					packetSize = sizeof(key);
					socket->write((char*)&packetSize , sizeof(quint32) );
					socket->write( (char*)&key, sizeof(quint16) );
					if(!socket->waitForBytesWritten(-1))
						qDebug() << "Send EOF Error: ";
					else
						qDebug() << "Send EOF";

				}else
				{
					if( socket->isValid()&&socket->state()==QAbstractSocket::UnconnectedState )
					{
						qDebug("Retry to connect remote client %s,%d" , qPrintable(pScreenDst->_ip), pScreenDst->_port );
						//重新尝试连接远程终端
						pScreenDst->resetDst( pScreenDst->_ip , pScreenDst->_port );
					}
				}
			}

		}

		_mutext.unlock();
	
		return _snapper->_imageLength;
	}else
	{
		qDebug("Can't capture screen ");
	}

	return -1;

}

int CvScreenMediaService::removeSendMedia( const QString& dstIP )
{
	LOG_DEBUG("移除发送目标:%s" , qPrintable(dstIP)  );

	_mutext.lock();

	ScreenDist* pScreenDst = NULL;
	ScreenDistList::iterator it = _sendDistList.begin();
	for( ; it!=_sendDistList.end() ;it++ )
	{
		pScreenDst = *it;
		if( pScreenDst->_ip == dstIP)
		{
			LOG_DEBUG("成功释放资源");
			pScreenDst->release();
			break;
		}
	}
	if( it==_sendDistList.end() )
	{
		LOG_ERROR("目标不存在");
	}
	_mutext.unlock();

	return 0;

}

int CvScreenMediaService::captureScreen()
{
	//进行截屏
	//if( _captureWindow.left()>=0 && _captureWindow.top()>=0 && _captureWindow.right()>0&&_captureWindow.bottom()>0 )
	//{
		_snapper->setPosition( _captureWindow.left() , _captureWindow.top(), _captureWindow.right(),_captureWindow.bottom()  );
	//}else
	//{
	//	QRect rect = QApplication::desktop()->availableGeometry();
	//	_snapper->setPosition(rect.left() , rect.top() , QApplication::desktop()->width() , rect.bottom());
	//	//_snapper->setPosition(0,0,1280,720);
	//}

	_mutext.lock();
	int result = _snapper->snapScreen();
	_mutext.unlock();

	return result;
}
