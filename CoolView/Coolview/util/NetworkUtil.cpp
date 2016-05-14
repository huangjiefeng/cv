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

#include <QtNetwork/QNetworkInterface>

#include "CoolviewCommon.h"

/**
 * @brief  根据主机IP获取网关地址，保存到参数pGateway中，pGateway的长度至少为16.
 * @param pHostIP       主机IP地址
 * @param pGateway  网关地址输出字符串
 * @return  返回是否成功获取网关地址
 */
bool GetGatewayByHostIP(const char* pHostIP, char* pGateway)
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
		// 读取成功      
		PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		while(pAdapter)
		{
			// 如果地址不匹配，则继续找
			if(strcmp(pHostIP, pAdapter->IpAddressList.IpAddress.String) != 0)
			{
				pAdapter = pAdapter->Next;
				continue;
			}
			// 地址匹配，则提取default gateway信息
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

QStringList GetHostIPList() {
  std::vector<std::string> ip_list;
  GetIPList( AF_INET , &ip_list );
  GetIPList( AF_INET6 , &ip_list );

  QStringList ip_string_list;
  TransformStringVectorToQStringList( &ip_list ,&ip_string_list );
  return ip_string_list;
}

void GetHostIPs( QString &ipv4, QString &ipv6 ) {
  std::vector<std::string> ip_list;
  GetIPList( AF_INET , &ip_list );

  ipv4 = ip_list.empty() ? QString() : QString::fromStdString(ip_list.front());

  ip_list.clear();
  GetIPList(AF_INET6, &ip_list);
  ipv6 = ip_list.empty() ? QString() : QString::fromStdString(ip_list.front());
}

bool GetIPList( int family , std::vector<std::string>* ipList ) {
  struct addrinfo *addrf;
  SOCKET sock = socket(family, SOCK_DGRAM, 0);
  if( sock ==INVALID_SOCKET ) {
	int error_code = WSAGetLastError();
	return false;
  }
  const int size = 64;
  char address[size];
  memset(address, size, 0);

  if (family == AF_INET) {
	getaddrinfo("217.12.3.11", NULL, NULL, &addrf);
  } else if (family == AF_INET6) {
	getaddrinfo("2001:638:500:101:2e0:81ff:fe24:37c6", NULL, NULL, &addrf);
  }

  if (addrf == NULL) {
	closesocket(sock);
	return false;
  }

  int local_addr_len;
  SOCKADDR_STORAGE local_addr;
  int res = WSAIoctl(sock, SIO_ROUTING_INTERFACE_QUERY,
                     addrf->ai_addr, addrf->ai_addrlen,
                     &local_addr, sizeof(local_addr),
                     (LPDWORD)&local_addr_len,
                     NULL, NULL);
  if (res != 0) {
	closesocket(sock);
	freeaddrinfo(addrf);
	return false;
  }

  closesocket(sock);
  freeaddrinfo(addrf);

  if (getnameinfo((const struct sockaddr *) &local_addr,
	  local_addr_len, address, size, NULL, 0, NI_NUMERICHOST)) {
      return false;   
  }
  std::string sAddress( address );
  ipList->push_back( sAddress );
  return true;
}

bool IsIPAddressEqual(const std::string &address1, const std::string &address2) {
  if (IsIpv4Address(address1) && IsIpv4Address(address2)) {
    //IPv4
    IN_ADDR addr1;
    IN_ADDR addr2;
    inet_pton(AF_INET, address1.c_str(), &addr1);
    inet_pton(AF_INET, address2.c_str(), &addr2);

    return addr1.s_addr == addr2.s_addr;
  }

  if (IsIpv6Address(address1) && IsIpv6Address(address2)) {
    //IPv6
    IN6_ADDR addr1;
    IN6_ADDR addr2;
    inet_pton(AF_INET6, address1.c_str(), &addr1);
    inet_pton(AF_INET6, address2.c_str(), &addr2);
    bool is_same = true;
    for (int i=0; i<8; ++i) {
      if (addr1.u.Word[i] != addr2.u.Word[i]) {
        is_same = false;
        break;
      }
    }
    return is_same;
  }

  return false;
}

bool IsIpv6Address(const std::string &ip) {
  return ip.find(':') != std::string::npos;
}

bool IsIpv4Address(const std::string &ip) {
  return ip.find('.') != std::string::npos;
}

QString GetMacAddress( const QString &ip_address ) {
  QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
  for (QList<QNetworkInterface>::iterator it = interfaces.begin();
       it != interfaces.end(); ++it) {
    QList<QNetworkAddressEntry> address_list = it->addressEntries();
    for (QList<QNetworkAddressEntry>::iterator address_it=address_list.begin();
         address_it != address_list.end(); ++address_it) {
      QString ip_addr = address_it->ip().toString(); 
      if (IsIPAddressEqual(ip_address.toStdString(), ip_addr.toStdString())) {
        return it->hardwareAddress();
      }
    }
  }
  return QString("");
}

