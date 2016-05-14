
#include <stdio.h>
#include "UdpSender.h"
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
UdpSender::UdpSender()
{
	_isConnected = false;
	_udpSocket = INVALID_SOCKET;
}

UdpSender::~UdpSender()
{
	if(_udpSocket != INVALID_SOCKET)
	{
		closesocket(_udpSocket);
	}
	_isConnected = false;
}

int UdpSender::send(const char *msg, const int msgLen)
{
	if(_udpSocket == INVALID_SOCKET || !isConnected())
		return -1;
	return ::send(_udpSocket, msg, msgLen, 0);
}

bool UdpSender::connect(const char *remoteIP, const int remotePort)
{
	if(isConnected())
		return false;
	bool retVal = false;
	addrinfo *remoteAddressInfo = NULL;
	char portString[20];
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
				_udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
				if(_udpSocket != INVALID_SOCKET)
				{	
					if(::connect(_udpSocket, (sockaddr *)sockIPv4, sizeof(sockaddr)) == SOCKET_ERROR)
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
					if(::connect(_udpSocket, (sockaddr *)sockIPv6, sizeof(sockaddr_in6)) == SOCKET_ERROR)
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
		_isConnected = true;
	return retVal;
}