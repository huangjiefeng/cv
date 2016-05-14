#ifndef _RTPWARE_UDPSENDER_H_
#define _RTPWARE_UDPSENDER_H_

class UdpSender
{
public:
	bool connect(const char *remoteIP, const int remotePort);
	int send(const char *msg, const int msgLen);
	bool isConnected(){return _isConnected;}
	void close();

	UdpSender();
	~UdpSender();
private:
	bool _isConnected;
	unsigned int _udpSocket;
};

#endif