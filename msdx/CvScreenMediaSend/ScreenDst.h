#ifndef SCREEN_DST_H
#define SCREEN_DST_H
#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>
///屏幕流的发送目标
class ScreenDist : public QObject
{
	Q_OBJECT
public:
	ScreenDist( QString ip , int port);
	~ScreenDist();

	void resetDst( const QString&ip ,const int port );

	//释放连接
	void release();

private Q_SLOTS:
	void dealError(QAbstractSocket::SocketError error);

public:
	QString		_ip;			//目标IP
	int			_port;			//目标端口
	bool		_isBusy;		//是否正在使用
	QTcpSocket	_socket;		//通信用的socket
};

#endif