#ifndef _RTPWARE_UDPSENDER_H_
#define _RTPWARE_UDPSENDER_H_
#include <WinSock2.h>

class UdpSender
{
public:
	bool connect(const char *remoteIP, const int remotePort);
	int send(const char *msg, const int msgLen);
	bool isConnected(){return _isConnected;}

	UdpSender();
	~UdpSender();
private:
	bool _isConnected;
	SOCKET _udpSocket;
};

#endif