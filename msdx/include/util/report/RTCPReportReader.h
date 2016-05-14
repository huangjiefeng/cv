/**
 * @brief 采用共享内存的方式获取RTCP报告
 */

#ifndef RTCP_REPORT_READER_H
#define RTCP_REPORT_READER_H
#include <string>
using namespace std;

class SingalMemoryMap;
typedef void*		HANDLE;

#include <util/report/RtpStat.h>

#include "ReporterCommon.h"

class RTCPReportReader
{
public:

	RTCPReportReader( const RTCPReportType type );
	~RTCPReportReader();

	/**
	 * @brief 初始化工作环境
	 */
	void setReader(const string& mediaID);

	/**
	 * @brief 读取rtcp报告
	 */
	void readRtcpReport( RtpStatItem& item  );

private:
	SingalMemoryMap*	_memoryMap;

	///工作线程句柄
	HANDLE				_hThread;

	void*				_pHandlerObject;

	RTCPReportType		_reportType;
};


#endif