#include "RTCPReportWriter.h"
#include <util/sharememory/SingalMemoryMap.h>
#include <log/Log.h>
#include <util/report/RtpStat.h>

RTCPReportWriter::RTCPReportWriter( const string& mediaID, const RTCPReportType type )
{
	string workerID = getWorkerIDFromMediaID(mediaID,type );
	_reportType = type;
	string mapFileName = workerID+"_MapFile";
	_memoryMap = new SingalMemoryMap( mapFileName );
	if( _memoryMap==NULL )
	{
		LOG_ERROR("创建共享内存时失败!");
	}

	LOG_DEBUG("MapFileName:%s ", mapFileName.c_str() );
}

RTCPReportWriter::~RTCPReportWriter()
{
	if( _memoryMap!=NULL )
	{
		delete _memoryMap;
		_memoryMap = NULL;
	}
}

void RTCPReportWriter::writeRTCPReport( const RtpStatItem&item )
{
	_memoryMap->writeData( &item ,sizeof(item));
}