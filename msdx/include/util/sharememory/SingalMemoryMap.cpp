#include "SingalMemoryMap.h"
#include <log/Log.h>

SingalMemoryMap::SingalMemoryMap( const std::string& mapFileName )
{
	m_memoryMapFileName = mapFileName;
	std::string mutexName = m_memoryMapFileName+"Mutex";
	m_mutex = CreateMutex( NULL , false , mutexName.c_str() );
	if( m_mutex == NULL )
	{
		LOG_ERROR("创建共享内存的互斥锁[%s]时发生错误",mutexName.c_str() );
	}

	m_hMemoryMap = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE | SEC_COMMIT, 0, MAX_SHARE_SIZE, m_memoryMapFileName.c_str());

	m_lpData = (LPBYTE)MapViewOfFile( m_hMemoryMap , FILE_MAP_WRITE|FILE_MAP_READ , 0 , 0 , 0 );

	ZeroMemory( m_lpData, MAX_SHARE_SIZE );

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
		LOG_ERROR( "读取的数据大小大于共享内存！" );

	if( m_lpData )
		memcpy(pData,m_lpData+offset,cDataSize);

	if (! ReleaseMutex(m_mutex)) 
		LOG_ERROR( "释放互斥锁时发生错误！" );
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
		LOG_ERROR( "写入的数据大小大于共享内存！" );

	if( m_lpData )
		memcpy(m_lpData+offset,pData,cDataSize);

	if (! ReleaseMutex(m_mutex)) 
		LOG_ERROR( "释放互斥锁时发生错误！" );
}