////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file	STcpSender.h
///
/// @author	qhf
/// @date	2011/1/1
/// @brief	Declares the tcp sender class. 
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_RW_STCPSENDER_H
#define  _RW_STCPSENDER_H

#include <string>
using namespace std;

class   STcpSender
{
public:
	STcpSender(void);
	~STcpSender(void);
	
	bool connect(const char * serverIP, const int serverPort);

	int send(const unsigned char * message,const int length);

	void close();

	bool isConnected() const { return _isConnected; }

private:

	unsigned int _sock;
	bool _isConnected;

	string _serverIP;
	int _serverPort;
};


#endif	//_RW_STCPSENDER_H