#include "stdafx.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   network\NetworkUtil.cpp
///
/// @brief  Implements the network util class. 
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NetworkUtil.h"       
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

#if _MSC_VER >= 1400 // for vc8, or vc9
#define  SUPPORT_GATEWAY_IPV6			//支持查询IPv6版本的网关
#endif

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3


bool getGatewayV4( const char* pHostIP, char* pGateway )
{
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

#ifdef SUPPORT_GATEWAY_IPV6
bool getGatewayV6(const char* pHostIP, char* pGateway)
{

    /* Declare and initialize variables */
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS ;//GAA_FLAG_INCLUDE_PREFIX;

    // default to ipv6 address family
    ULONG family = AF_INET6;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;
    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do
    {
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL)
        {
            printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            return false;
        }
        dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            FREE(pAddresses);
            pAddresses = NULL;
        }
        else
        {
            break;
        }
        Iterations++; //重试次数

    }
    while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    bool succeed = false;
    char ipBuffer[40]; //longer than or equal
    DWORD bufflen = 40;
    PSOCKADDR_IN6 addr;   //Important! It switches the normal sockaddr into the IPv6 type.
    if (dwRetVal == NO_ERROR)
    {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses)
        {
            //获取主机地址
            addr= (struct sockaddr_in6 *)pAddresses->FirstUnicastAddress->Address.lpSockaddr;  // Switch ing it!
            inet_ntop(AF_INET6,  &addr->sin6_addr,ipBuffer,40);
            //ipBuffer is the IPv6 Address String that you want to get!
            //匹配主机地址
            if(/*strcmp(pHostIP,ipBuffer)==0 &&*/ pAddresses->FirstGatewayAddress!= NULL )
            {
                //获取默认网关地址
                addr= (struct sockaddr_in6 *)pAddresses->FirstGatewayAddress->Address.lpSockaddr;  // Switch ing it!
                inet_ntop(AF_INET6,  &addr->sin6_addr,ipBuffer,40);
                strcpy(pGateway, ipBuffer);
                //跳出循环
                succeed = true;
                break;
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else
    {
        printf("Call to GetAdaptersAddresses failed with error: %d\n",dwRetVal);
    }

    if (pAddresses)
    {
        FREE(pAddresses);
    }

    return succeed;
}
#endif

/**
 * @brief  根据主机IP获取网关地址，保存到参数pGateway中，pGateway的长度至少为16.
 * @param pHostIP       主机IP地址
 * @param pGateway  网关地址输出字符串
 * @return  返回是否成功获取网关地址
 */
bool getGatewayByHostIP(const char* pHostIP, char* pGateway)
{
    if(pHostIP == NULL)
        return false;   
	#ifdef SUPPORT_GATEWAY_IPV6
	if (strstr(pHostIP,":")==NULL)	//ipv4
		return getGatewayV4(pHostIP, pGateway);
	else
	return getGatewayV6(pHostIP, pGateway);
#else
	return getGatewayV4(pHostIP, pGateway);
#endif
}


