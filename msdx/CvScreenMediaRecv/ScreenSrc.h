#ifndef SCREEN_SRC_H
#define SCREEN_SRC_H

#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <io.h>
typedef unsigned char BYTE;


//class CvCoreIf;
class CvScreenMediaService;
///屏幕流的发送目标
class ScreenSrc : public QObject
{
	Q_OBJECT
public:
	ScreenSrc( QString ip , int port );
	~ScreenSrc();

//	void resetDst( const QString&ip ,const int port );

	//释放连接
//	void release();

private Q_SLOTS:
	void dealError(QAbstractSocket::SocketError error);

	void dealSocketConnection();

	void imageBufferReady();
	int processData( const int key , const BYTE*data, const int dataLength );

public:
	QString		_ip;			//目标IP
	int			_port;			//目标端口
	QTcpServer  _tcpServer;			//监听远程连接
	QTcpSocket*	_remoteSocket;		//与远程用户建立的连接socket



	///Socket通信的block的大小
	quint32					_blockSize;

	//存储接收到的图片数据的临时文件
	QString					_recvImageTmpFileName;
	FILE*					_recvImageTmpFile;

	//Qt程序显示图片所用的图片文件名
	QString					_recvImageFileName;

	CvScreenMediaService*	_service;

	///Coolview Core proxy
	//CvCoreIf*		_cvCoreProxy;

};
#endif