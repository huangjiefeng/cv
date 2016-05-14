#include "RTCPReportReader.h"
#include <util/sharememory/SingalMemoryMap.h>
#include <log/Log.h>
#include <dbus/msdx/common/MsdxServiceCommon.h>


RTCPReportReader::RTCPReportReader( const RTCPReportType type )
{
	_memoryMap = NULL;
	_reportType = type;
}

RTCPReportReader::~RTCPReportReader()
{

	//释放原有的资源
	if( _memoryMap!=NULL )
	{
		delete _memoryMap;
		_memoryMap = NULL;
	}

}

void RTCPReportReader::setReader( const string& mediaID  )
{
	if( _memoryMap!=NULL )
	{
		delete _memoryMap;
		_memoryMap = NULL;
	}

	string workerID = getWorkerIDFromMediaID(mediaID , _reportType );

	//创建新的共享内存
	string mapFileName = workerID +"_MapFile";
	_memoryMap = new SingalMemoryMap( mapFileName );
	if( _memoryMap==NULL )
	{
		LOG_ERROR("创建共享内存时失败!");
	}
}

void RTCPReportReader::readRtcpReport( RtpStatItem& item )
{
	if( _memoryMap )
	{
		_memoryMap->readData( &item , sizeof(item) );
	}
}