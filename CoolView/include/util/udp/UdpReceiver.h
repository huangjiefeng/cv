#ifndef _RTPWARE_UDPRECEIVER_H_
#define _RTPWARE_UDPRECEIVER_H_
#include <WinSock2.h>

class UdpReceiver
{
public:
	bool bind(const char *remoteIP, const int remotePort);
	bool isBinded(){return _isBinded;}

	UdpReceiver();
	~UdpReceiver();

	int receive( void*buf , const int bufLen );
private:
	bool _isBinded;
	SOCKET _udpSocket;
};

#endif