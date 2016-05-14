/**
 * @brief 往共享内存中写入报告
 */

#ifndef RTCP_REPORT_WRITER_H
#define RTCP_REPORT_WRITER_H
#include <Windows.h>


class SingalMemoryMap;
#include "RtpStat.h"
#include "ReporterCommon.h"
#include <string>
using namespace std;
class RTCPReportWriter
{
public:
	/**
	 * @brief writerID采用的是媒体流的ID
	 */
	RTCPReportWriter( const string& writerID , const RTCPReportType type);
	~RTCPReportWriter();

	void writeRTCPReport( const RtpStatItem& item );

private:
	SingalMemoryMap*	_memoryMap;

	///最大的信号量
	static const int	_MAX_SEMAPHORE_COUNT = 1;

	///往共享内存中写入报告的次数
	UINT32				_writeCount;

	///Writer的类型
	RTCPReportType		_reportType;
};

#endif