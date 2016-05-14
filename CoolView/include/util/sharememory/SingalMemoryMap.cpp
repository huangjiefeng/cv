#include "SingalMemoryMap.h"
#include <log/Log.h>


SingalMemoryMap::SingalMemoryMap( const std::string& mapFileName )
{
	m_memoryMapFileName = mapFileName;
	std::string mutexName = m_memoryMapFileName+"Mutex";
	m_mutex = CreateMutexA( NULL , false , mutexName.c_str() );
	if( m_mutex == NULL )
	{
		_snprintf(__global_msg , sizeof(__global_msg), "创建共享内存的互斥锁[%s]时发生错误",mutexName.c_str() );
		MSDX_LOG_ERROR( __global_msg);
	}

	m_hMemoryMap = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE | SEC_COMMIT, 0, MAX_SHARE_SIZE, m_memoryMapFileName.c_str());

	m_lpData = (LPBYTE)MapViewOfFile( m_hMemoryMap , FILE_MAP_WRITE|FILE_MAP_READ , 0 , 0 , 0 );

	if (m_lpData == NULL)
	{
		throw "invalid situation.";
	}

}

SingalMemoryMap::~SingalMemoryMap()
{
	if (m_lpData != NULL)
	{
		UnmapViewOfFile(m_lpData);
		m_lpData = NULL;
	}
	
	if (m_hMemoryMap != NULL)
	{
		CloseHandle(m_hMemoryMap);
		m_hMemoryMap = NULL;
	}

	if( m_mutex!=NULL )
	{
		CloseHandle( m_mutex );
		m_mutex = NULL;
	}

}



/**
* @brief 读取共享内存中的数据
* @param pData 存放读取数据的数据指针
* @param cDataSize 读取数据的大小 
* @return NULL
*/
void SingalMemoryMap::readData( void* pData , const int& cDataSize , const int& offset )
{

	WaitForSingleObject( m_mutex , INFINITE );

	if( cDataSize>MAX_SHARE_SIZE )
		MSDX_LOG_ERROR( "读取的数据大小大于共享内存！" );

	if( m_lpData )
		memcpy(pData,m_lpData+offset,cDataSize);

	if (! ReleaseMutex(m_mutex)) 
		MSDX_LOG_ERROR( "释放互斥锁时发生错误！" );
}

/**
* @brief 往共享内存中写入数据
* @param pData 指向将写入共享内存的数据的指针
* @param cDataSize 写入数据的大小
* @return NULL
*/
void SingalMemoryMap::writeData( const void*pData , const int& cDataSize , const int& offset)
{
	WaitForSingleObject( m_mutex , INFINITE );

	if( cDataSize>MAX_SHARE_SIZE )
		MSDX_LOG_ERROR( "写入的数据大小大于共享内存！" );

	if( m_lpData )
		memcpy(m_lpData+offset,pData,cDataSize);

	if (! ReleaseMutex(m_mutex)) 
		MSDX_LOG_ERROR( "释放互斥锁时发生错误！" );
}
//
//SingalMemoryMapWriter::SingalMemoryMapWriter( const std::string& mapFileName ):SingalMemoryMap(mapFileName)
//{
//	m_writeCount = 0;
//	m_memoryMapFileName = mapFileName;
//	m_hMemoryMap = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, 
//		PAGE_READWRITE | SEC_COMMIT, 0, MAX_SHARE_SIZE, SingalMemoryMap::m_memoryMapFileName.c_str());
//
//	m_lpData = (LPBYTE)MapViewOfFile( m_hMemoryMap , FILE_MAP_WRITE , 0 , 0 , 0 );
//
//	if (m_lpData == NULL)
//	{
//		CloseHandle(m_hMemoryMap);
//		m_hMemoryMap = NULL;
//	}
//
//	if( m_lpData==NULL||m_hMemoryMap==NULL )
//	{
//		_snprintf( __global_msg , sizeof(__global_msg) , "创建共享内存[%s]失败!", SingalMemoryMap::m_memoryMapFileName.c_str());
//		MSDX_LOG_ERROR(__global_msg);
//	}
//}
//
//void SingalMemoryMapWriter::writeData( const void* pData , const int & cDataSize , const int &offset/*=0 */ )
//{
//	//如果不是第一次写数据，则需要等待写信号量
//	if( m_writeCount>0 )
//		WaitForSingleObject( m_writerSemaphore , INFINITE );
//
//
//	printf("Write to shareMemory %d times\n" , m_writeCount );
//
//	if( cDataSize>MAX_SHARE_SIZE )
//		MSDX_LOG_ERROR( "写入的数据大小大于共享内存！" );
//
//	if( m_lpData )
//		memcpy(m_lpData+offset,pData,cDataSize);
//
//	if( m_writerSemaphore!=NULL )
//	{
//		if( ReleaseSemaphore(m_writerSemaphore,1,NULL)==0 )
//			MSDX_LOG_ERROR("释放写信号量的时候发生错误");
//	}else
//	{
//		MSDX_LOG_ERROR("异常情况：写信号量不存在！");
//	}
//	m_writeCount++;
//}

//
//SingalMemoryMapReader::SingalMemoryMapReader(const std::string& mapFileName ):SingalMemoryMap(mapFileName)
//{
//	m_memoryMapFileName = mapFileName;
//	m_hMemoryMap = OpenFileMapping(FILE_MAP_READ , FALSE , m_memoryMapFileName.c_str());
//
//	if( m_hMemoryMap==NULL )
//	{
//		_snprintf(__global_msg , sizeof(__global_msg) , "对应的[%s]writer没有打开！",SingalMemoryMap::m_memoryMapFileName.c_str() );
//		MSDX_LOG_ERROR( __global_msg );
//	}
//	m_lpData = (LPBYTE)MapViewOfFile( m_hMemoryMap , FILE_MAP_READ, 0 , 0 , 0 );
//
//	if (m_lpData == NULL)
//	{
//		CloseHandle(m_hMemoryMap);
//		m_hMemoryMap = NULL;
//	}
//}
//
//void SingalMemoryMapReader::readData( void* pData , const int & cDataSize , const int &offset/*=0 */ )
//{
//	WaitForSingleObject( m_writerSemaphore , INFINITE );
//
//	if( cDataSize>MAX_SHARE_SIZE )
//		MSDX_LOG_ERROR( "读取的数据大小大于共享内存！" );
//
//	if( m_lpData )
//		memcpy(pData,m_lpData+offset,cDataSize);
//
//	if( ! ReleaseSemaphore( m_writerSemaphore , 1,NULL) )
//	{
//		MSDX_LOG_ERROR( "释放互斥锁时发生错误！" );
//	}
//}