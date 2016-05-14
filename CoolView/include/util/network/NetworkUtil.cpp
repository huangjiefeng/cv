////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   network\NetworkUtil.cpp
///
/// @brief  Implements the network util class. 
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetworkUtil.h"       
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")



#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/**
 * @brief  根据主机IP获取网关地址，保存到参数pGateway中，pGateway的长度至少为16.
 * @param pHostIP       主机IP地址
 * @param pGateway  网关地址输出字符串
 * @return  返回是否成功获取网关地址
 */
bool NetworkUtil::getGatewayByHostIP(const char* pHostIP, char* pGateway)
{
    if(pHostIP == NULL)
        return false;   
	PIP_ADAPTER_INFO pAdapterInfo = NULL;  
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
	{
		//printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return  false;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if(pAdapterInfo == NULL)
		{
			//  printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return false;
		}
	}
	bool succeed = false;
	if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		//读取成功      
		PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		while(pAdapter)
		{
			//如果地址不匹配，则继续找
			if(strcmp(pHostIP, pAdapter->IpAddressList.IpAddress.String) != 0)
			{
				pAdapter = pAdapter->Next;
				continue;
			}
			//地址匹配，则提取default gateway信息
			//strncpy(pGateway, pAdapter->GatewayList.IpAddress.String, strlen(pAdapter->GatewayList.IpAddress.String));
			sprintf(pGateway, "%s", pAdapter->GatewayList.IpAddress.String);
			succeed = true;
			break;
		}
	}
	else
	{
		//  printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
	}
	if(pAdapterInfo)
		FREE(pAdapterInfo);
	return succeed;
}


bool NetworkUtil::getIPList( int family , vector<string>* ipList )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we couldn't find a useable */
		/* winsock.dll. */
		return false;
	}

	SOCKET sock;
	char *address;
	int size;

	SOCKADDR_STORAGE local_addr;

	int local_addr_len;

	struct addrinfo *addrf;

	//	address[0] = '\0';
	sock = socket (family, SOCK_DGRAM, 0);
	if( sock ==INVALID_SOCKET )
	{
		int errCode = WSAGetLastError();
		WSACleanup();
		return false;
	}
	size = 64;
	address = new char[size];
	memset(address,size,0);

	if (family == AF_INET)
	{
		getaddrinfo ("217.12.3.11", NULL, NULL, &addrf);
	} else if (family == AF_INET6)
	{
		getaddrinfo ("2001:638:500:101:2e0:81ff:fe24:37c6", NULL, NULL, &addrf);
	}

	if (addrf == NULL)
	{
		closesocket (sock);
		WSACleanup();
		return false;
	}

	if (WSAIoctl
		(sock, SIO_ROUTING_INTERFACE_QUERY, addrf->ai_addr, addrf->ai_addrlen,
		&local_addr, sizeof (local_addr), (LPDWORD)&local_addr_len, NULL, NULL) != 0)
	{
		closesocket (sock);
		freeaddrinfo (addrf);
		WSACleanup();
		return false;
	}

	closesocket (sock);
	freeaddrinfo (addrf);

	if (getnameinfo ((const struct sockaddr *) &local_addr,
		local_addr_len, address, size, NULL, 0, NI_NUMERICHOST))
	{
		WSACleanup();
		return false;   
	}
	string sAddress( address );
	delete []address;
	address = NULL;
	ipList->push_back( sAddress );
	WSACleanup();
	return  true;
	
	
}

vector<string> NetworkUtil::getHostIPList()
{
	std::vector<std::string> ipList;
	getIPList( AF_INET , &ipList );
	getIPList( AF_INET6 , &ipList );

	return  ipList;
}