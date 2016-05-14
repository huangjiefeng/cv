
#include <stdio.h>
#include "UdpReceiver.h"
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
UdpReceiver::UdpReceiver()
{
	_isBinded = false;
	_udpSocket = INVALID_SOCKET;
}

UdpReceiver::~UdpReceiver()
{
	if(_udpSocket != INVALID_SOCKET)
	{
		closesocket(_udpSocket);
	}
	_isBinded = false;
}

bool UdpReceiver::bind(const char *remoteIP, const int remotePort)
{
	if(isBinded())
		return false;
	bool retVal = false;
	addrinfo *remoteAddressInfo = NULL;
	char portString[20]={0};
	sprintf(portString, "%d", remotePort);
	if(GetAddrInfoA(remoteIP,  portString, NULL, &remoteAddressInfo) == 0)
	{
		if(remoteAddressInfo != NULL)
		{
			switch(remoteAddressInfo->ai_family)
			{
			case AF_INET:
			{
				sockaddr_in *sockIPv4 = (sockaddr_in *)remoteAddressInfo->ai_addr;
        sockIPv4->sin_addr.S_un.S_addr = htonl(INADDR_ANY); //为了能接收远程TX发来的UDP数据包，要使用任意地址
				_udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
				if(_udpSocket != INVALID_SOCKET)
				{	
					if(::bind(_udpSocket, (sockaddr *)sockIPv4, sizeof(sockaddr)) == SOCKET_ERROR)
					{
						closesocket(_udpSocket);
						_udpSocket = INVALID_SOCKET;
					}
					else
						retVal = true;
				}// end if
				break;
			}
			case AF_INET6:
			{
				sockaddr_in6 *sockIPv6 = (sockaddr_in6 *)remoteAddressInfo->ai_addr;
				_udpSocket = socket(AF_INET6, SOCK_DGRAM, 0);
				if(_udpSocket != INVALID_SOCKET)
				{	
					if(::bind(_udpSocket, (sockaddr *)sockIPv6, sizeof(sockaddr_in6)) == SOCKET_ERROR)
					{
						closesocket(_udpSocket);
						_udpSocket = INVALID_SOCKET;
					}
					else
						retVal = true;
				}//end if
				break;
			}
			}//end switch
			FreeAddrInfoA(remoteAddressInfo);
		}//end if
	}//end if
	if(retVal)
		_isBinded = true;
	return retVal;
}


int UdpReceiver::receive( void*buf , const int bufLen )
{
	if( !isBinded() )
	{
		return -1;
	}

	SOCKADDR addrClient;
	int len=sizeof(SOCKADDR);
	int recvBytes =  recvfrom( _udpSocket , (char*)buf , bufLen,0,&addrClient , &len );
	return recvBytes;
}