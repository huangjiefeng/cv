#include "ScreenDst.h"
#include <log/Log.h>

ScreenDist::ScreenDist( QString ip , int port )
{ 
	_isBusy = true;
	_ip = ip;
	_port = port;
	_socket.connectToHost( ip , _port, QIODevice::WriteOnly );
	connect( &_socket , SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(dealError(QAbstractSocket::SocketError)) );
}

ScreenDist::~ScreenDist()
{
	connect( &_socket , SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(dealError(QAbstractSocket::SocketError)) );
	_socket.abort();
	_socket.close();
}

void ScreenDist::dealError( QAbstractSocket::SocketError error )
{
	switch( error )
	{
	case QAbstractSocket::ConnectionRefusedError:
		LOG_ERROR( "Socket Error on %s with code:%d :" ,qPrintable(_ip) , error, "Connection Refused" );
		break;
	case QAbstractSocket::RemoteHostClosedError:
		LOG_ERROR( "Socket Error on %s with code:%d :" ,qPrintable(_ip), error, "Remote Host Closed" );
		_isBusy = false;
		break;
	}
}

void ScreenDist::resetDst( const QString&ip ,const int port )
{
	_ip = ip;
	_port = port;
	_socket.abort();
	_socket.connectToHost( ip , _port, QIODevice::WriteOnly );
	_isBusy = true;
}

void ScreenDist::release()
{
	_socket.abort();
	_isBusy = false;
}