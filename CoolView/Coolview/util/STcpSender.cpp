#include "STcpSender.h"
#include <WinSock2.h>
#include <stdio.h>

STcpSender::STcpSender(void)
{	
	_sock=0;
	_isConnected =false;
	_serverIP.clear();
	_serverPort =0;
}


STcpSender::~STcpSender(void)
{
	close();
	_sock=0;
	_isConnected =false;
}


bool STcpSender::connect( const char * serverIP, const int serverPort )
{
	if (serverIP==NULL || serverPort<=0)
		return false;

	SOCKET sock;
	// Create a new socket to make a client connection.
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))== INVALID_SOCKET)
	{
		printf("create socket failed with error %d\n", WSAGetLastError());	
		return false;
	}

	SOCKADDR_IN          serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);    
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);

	if (::connect(sock, (SOCKADDR *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		printf("connect to %s:%d failed with error %d\n", inet_ntoa(serverAddr.sin_addr), htons(serverAddr.sin_port),
			WSAGetLastError());
		closesocket(sock);
		return false;
	} 
	//关闭掉原来的连接
	this->close();
	//更新变量
	_serverIP = serverIP;
	_serverPort = serverPort;
	_sock = sock;
	_isConnected =true;

	return true;
}

int STcpSender::send( const unsigned char * message,const int length )
{
	if (message==NULL || length<=0)
		return -1;
	if (!_isConnected || _sock==0)
		return -1;
	char *	sendbuf = (char *) message;
	int rc, nleft, idx;
	nleft = length;
	idx = 0;
	while(nleft > 0)
	{
		rc = ::send(_sock, &sendbuf[idx], nleft, 0);
		if(rc == SOCKET_ERROR)
		{
			break;
		}
		else
		{
			nleft -= rc;
			idx += rc;
		}
	}
	if(rc == SOCKET_ERROR)
	{
		printf("send tcp data failed: %d\n", WSAGetLastError());
		return -1;
	}
	return idx;
}


void STcpSender::close()
{
	if (_sock)
    {
        shutdown(_sock,SD_SEND);
        closesocket(_sock);
    }
	_sock =0;
	_isConnected = false;
}