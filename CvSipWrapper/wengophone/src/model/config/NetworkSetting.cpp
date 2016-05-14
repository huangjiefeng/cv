#include "stdafx.h"

#include ".\networksetting.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QtGui/QtGui>

NetworkSetting::NetworkSetting(void)
{
}

NetworkSetting::~NetworkSetting(void)
{
}

QStringList NetworkSetting::getIPList(int family)
{
	QStringList ipList;
	SOCKET sock;
	char *address;
	int size;

	SOCKADDR_STORAGE local_addr;

	int local_addr_len;

	struct addrinfo *addrf;

//	address[0] = '\0';
	sock = socket (family, SOCK_DGRAM, 0);
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
		return ipList;
	}

	if (WSAIoctl
		(sock, SIO_ROUTING_INTERFACE_QUERY, addrf->ai_addr, addrf->ai_addrlen,
		&local_addr, sizeof (local_addr), (LPDWORD)&local_addr_len, NULL, NULL) != 0)
	{
		closesocket (sock);
		freeaddrinfo (addrf);
		return ipList;
	}

	closesocket (sock);
	freeaddrinfo (addrf);

	if (getnameinfo ((const struct sockaddr *) &local_addr,
		local_addr_len, address, size, NULL, 0, NI_NUMERICHOST))
	{
		return ipList;
	}
	ipList.append(address);
	return ipList;
}
