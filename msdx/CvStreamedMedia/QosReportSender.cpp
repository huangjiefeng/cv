#include "QosReportSender.h"
#include "UdpSender.h"
#include "NConfig.h"
#include "util/report/ReporterCommon.h"
#include "log/Log.h"

const int MAX_MSG_SIZE = 1500;

QosReportSender::QosReportSender(const char* remoteIP, const int port )
{
  _reportSender = nullptr;
	init( remoteIP , port );
}

QosReportSender::~QosReportSender()
{
	delete _reportSender;
	_reportSender = NULL;
}

void QosReportSender::HandleRtpStat( const RtpStatItem& rtpstat )
{
	if(_reportSender == NULL || !_reportSender->isConnected())
		return;
	char msg[MAX_MSG_SIZE];
	int msgsize = MAX_MSG_SIZE;
	convertRtpStatReport(rtpstat, msg, msgsize);
	if(msgsize != -1)
	{
		_reportSender->send(msg, msgsize + 1);
	}
}

void QosReportSender::convertRtpStatReport( const RtpStatItem& rtpstat, char *msg, int &msgsize )
{
	char *fmtQosReport = \
		"<?xml version=\"1.0\"?>"
		"<coolview command=\"qos\" type=\"clientQosReport\">"
		"<cid>%d</cid>"
		"<messageSenderIP>%s</messageSenderIP>"
		"<seq>%d</seq>"
		"<endpointType>%s</endpointType>"
		"<senderIP>%s</senderIP>"
		//"<senderPort>%d</senderPort>"
		"<receiverIP>%s</receiverIP>"
		//"<receiverPort>%d</receiverPort>"
		"<flowClass>%s</flowClass>"
		"<bandwidth>%d</bandwidth>"
		"<lossRate>%d</lossRate>"
		"<delay>%d</delay>"
		"<jitter>%d</jitter>"
		"<interval>%d</interval>"
		"<timestamp>%I64d</timestamp>"
		"</coolview>";
	msgsize = sprintf(msg, fmtQosReport,
		_currentConferenceId,
		rtpstat.local_addr,
		rtpstat.seqnumber,
		rtpstat.rtp_endpoint_type == eET_Sender ? "sender" : "receiver",
		rtpstat.rtp_endpoint_type == eET_Sender ? rtpstat.local_addr : rtpstat.rem_addr,
		rtpstat.rtp_endpoint_type == eET_Sender ? rtpstat.rem_addr : rtpstat.local_addr,
		rtpstat.media_type == eMT_Video ? "video" : (rtpstat.media_type ==eMT_Audio ? "audio" : "unknown"),
		rtpstat.rtp_endpoint_type == eET_Sender ? (int)rtpstat.send_bandwidth : (int)rtpstat.recv_bandwidth,
		(int)rtpstat.lost,
		rtpstat.delay,
		rtpstat.jitter,
		rtpstat.interval,
		rtpstat.timestamp
		);
}

void QosReportSender::init( const char* remoteIP , const int port )
{
	if(_reportSender != NULL)
	{
		delete _reportSender;
		_reportSender = NULL;
	}
	_reportSender = new UdpSender();
	if(_reportSender->connect( remoteIP,port ) == false)
	{
		delete _reportSender;
		_reportSender = NULL;
    LOG_ERROR("init QosReportSender failed, ip:%s, port:%d", remoteIP, port);
	}
}

/**
 * @brief 将rtcp的二进制流发送给本机其他进程
 */
void QosReportSender::sendRtpStat( const RtpStatItem &stat )
{
    UDPReportHeader header = {
        UDPReportType::UdpReportType_RTP, 
        sizeof(RtpStatItem)
    };

    sendStatPackage(header, reinterpret_cast<const char *>(&stat));
}

void QosReportSender::sendRecStat( const RecStatItem &stat )
{
    UDPReportHeader header = {
        UDPReportType::UdpReportType_Record, 
        sizeof(RecStatItem)
    };
    
    sendStatPackage(header, reinterpret_cast<const char *>(&stat));
}

void QosReportSender::sendFilePlayStat( const FilePlayStatItem &stat )
{
    UDPReportHeader header = {
        UDPReportType::UdpReportType_FilePlay, 
        sizeof(FilePlayStatItem)
    };

    sendStatPackage(header, reinterpret_cast<const char *>(&stat));
}

void QosReportSender::sendStatPackage( const UDPReportHeader &header, const char * data )
{
    const unsigned int msgSize = header.size;
    const unsigned int headerSize = sizeof(UDPReportHeader);
    char * msg = new char[headerSize + msgSize];
    memcpy_s(msg, headerSize, reinterpret_cast<const char *>(&header), headerSize);
    memcpy_s(msg + headerSize, msgSize, data, msgSize);
    if( _reportSender )
    {
        _reportSender->send( msg, headerSize + msgSize );
    }
    delete [] msg;
}
