#include "ScreenSrc.h"
//#include <dbus/core/client/CvCoreIf.h>
//#include <dbus/core/common/CvCoreServiceCommon.h>
#include "CvScreenMediaService.h"
#include <log/Log.h>
#include <QtWidgets/QApplication>
#include <Windows.h>


ScreenSrc::ScreenSrc( QString ip , int port )
{
	_ip = ip;
	_port = port;
	_remoteSocket = NULL;
	_service = NULL;


	QString fileDir = QApplication::applicationDirPath();
	fileDir.replace("/" , "\\");
	_recvImageTmpFileName = fileDir + "\\ScreenTmp.jpg";
	_recvImageTmpFile = NULL;
	_recvImageFileName = fileDir +  "\\ScreenCaptrueRecv.jpg";

	this->_blockSize = 0;

	if( ip.indexOf(":")!=-1 )
		_tcpServer.listen( QHostAddress::AnyIPv6 , _port );
	else
		_tcpServer.listen( QHostAddress::Any , _port );

	connect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(dealSocketConnection()));

	/*_cvCoreProxy = new CvCoreIf(CVCORE_SERVICE_NAME , CVCORE_SERVICE_OBJECT_PATH , 
		QDBusConnection::sessionBus() );*/
}


ScreenSrc::~ScreenSrc()
{
	if( _remoteSocket )
	{
		_remoteSocket->abort();
	}
	_tcpServer.close();
	disconnect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(dealSocketConnection()));

	DeleteFile(_recvImageTmpFileName.toStdString().c_str() );
	DeleteFile(_recvImageFileName.toStdString().c_str() );
}

void ScreenSrc::dealSocketConnection()
{
	if( _remoteSocket!=NULL )
	{
		_remoteSocket->abort();
		disconnect( _remoteSocket , SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(dealError(QAbstractSocket::SocketError)) );
		disconnect(_remoteSocket, SIGNAL(disconnected()),
			_remoteSocket, SLOT(deleteLater()));
		disconnect(_remoteSocket, SIGNAL(readyRead()) , this , SLOT(imageBufferReady()) );
	}
	
	_remoteSocket = _tcpServer.nextPendingConnection();
	connect( _remoteSocket , SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(dealError(QAbstractSocket::SocketError)) );

	LOG_DEBUG("建立来自%s的连接！\n" ,qPrintable(_remoteSocket->peerAddress().toString()) );

	//_tcpServer->close();		//停止监听其他连接

	connect(_remoteSocket, SIGNAL(disconnected()),
		_remoteSocket, SLOT(deleteLater()));
	connect(_remoteSocket, SIGNAL(readyRead()) , this , SLOT(imageBufferReady()) );

}

void ScreenSrc::dealError( QAbstractSocket::SocketError error )
{
	switch( error )
	{
	case QAbstractSocket::ConnectionRefusedError:
	    LOG_ERROR( "Socket Error on %s with code: %d : %s" ,qPrintable(_ip) , error, "Connection Refused" );
		break;
	case QAbstractSocket::RemoteHostClosedError:
		LOG_ERROR( "Socket Error on %s with code: %d : %s" ,qPrintable(_ip) , error, "Remote Host Closed" );
		disconnect( _remoteSocket , SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(dealError(QAbstractSocket::SocketError)) );
		_remoteSocket->abort();
		_remoteSocket->close();
		_remoteSocket = NULL;
		
		/*QByteArray input_bytes;
		QDataStream dataStream( &input_bytes , QIODevice::WriteOnly );
		dataStream.setVersion(QDataStream::Qt_4_4 );
		_cvCoreProxy->TeleCommand( CoreAction_CloseScreenRecv, input_bytes );*/
		//TODO:上层重构后,取消了CvCore,应该采用其他方式通知上层
		break;
	}
	
	DeleteFile(_recvImageFileName.toStdString().c_str() );
}

int ScreenSrc::processData( const int key , const BYTE*data, const int dataLength )
{
	switch(key)
	{
	case 0x0001:		// file name
		qDebug("Open storage file.");
		if( _recvImageTmpFile!=NULL )
		{
			fclose(_recvImageTmpFile );
			_recvImageTmpFile = NULL;
		}

		//创建保存jpeg文件
		if (_recvImageTmpFile==NULL) 
		{
			_recvImageTmpFile = fopen(_recvImageTmpFileName.toStdString().c_str(), "wb");
			if( _recvImageTmpFile==NULL )
			{
				qDebug("打开存储文件[%s]时出现问题。", qPrintable(_recvImageTmpFileName));
				return -1;
			}
		}
		break;
	case 0x0002:		// file data
		{
			if( _recvImageTmpFile!=NULL )
			{
				fwrite( data, 1 , dataLength , _recvImageTmpFile );
				fflush( _recvImageTmpFile );
			}
			break;
		}
	case 0x0003:		// file EOF
		{
			if( _recvImageTmpFile!=NULL )
			{
				qDebug("close jpeg file" );
				fclose(_recvImageTmpFile );
				_recvImageTmpFile = NULL;

				//拷贝图片
				CopyFile( _recvImageTmpFileName.toStdString().c_str(), _recvImageFileName.toStdString().c_str(), false);
			}
		}
		break;
	default: ;
	}
	return 0;
}

void ScreenSrc::imageBufferReady()
{
	while( _remoteSocket->bytesAvailable() >= sizeof(quint32))
	{
		if( _blockSize == 0)
		{
			if(_remoteSocket->bytesAvailable() < sizeof(quint32))
				return;
			_remoteSocket->read((char*)&_blockSize, sizeof(quint32));  // read blockSize
		}

		if(_remoteSocket->bytesAvailable() < _blockSize)				// have no enugh data
			return;
		quint16 key = 0;
		BYTE* data = NULL;
		int dataLength = _blockSize-sizeof(quint16);
		if( dataLength>0 )
		{
			data = new BYTE[dataLength];
		}

		_remoteSocket->read( (char*)&key , sizeof(quint16) );
		if( data )
		{
			int size = _remoteSocket->read( (char*)data , dataLength );
			if( size!=dataLength )
			{
				qDebug("数据报文出现问题");
			}
		}
		processData( key , data ,dataLength );

		delete data;
		data = NULL;
		_blockSize = 0;
	}
}
