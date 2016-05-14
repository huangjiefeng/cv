#ifndef REPORTER_COMMON_H
#define REPORTER_COMMON_H

#include <string>

enum class UDPReportType
{
    UdpReportType_RTP,
    UdpReportType_Record,
    UdpReportType_FilePlay,
};

struct UDPReportHeader
{
    UDPReportType type; // 消息类型
    unsigned int size; // 消息包大小(包含头部)
};

enum class RTCPReportType
{
	ReportType_video,
	ReportType_audio
};

inline std::string getWorkerIDFromMediaID( const std::string &mediaID, const RTCPReportType type )
{
	if( type == RTCPReportType::ReportType_video )
		return mediaID + "_video";
	else if( type == RTCPReportType::ReportType_audio )
		return mediaID + "_audio";

	return mediaID+"_unknow";
}

#endif