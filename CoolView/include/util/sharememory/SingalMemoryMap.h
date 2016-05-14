
#include <string>
#include <windows.h>

/**
 * @biref 共享内存管理类，负责共享内存的创建，读写，以及销毁
 */
class SingalMemoryMap
{
public:

	/**
	 * @brief 往共享内存中写入数据
	 * @param pData 指向将写入共享内存的数据的指针
	 * @param cDataSize 写入数据的大小
	 * @param offset 写入内存的偏移量
	 * @return NULL
	 */
	virtual void writeData( const void* pData , const int & cDataSize , const int &offset=0 );

	/**
	 * @brief 读取共享内存中的数据
	 * @param pData 存放读取数据的数据指针
	 * @param cDataSize 读取数据的大小
	 * @param offset 读取内存的偏移量
	 * @return NULL
	 */
	virtual void readData( void* pData , const int & cDataSize , const int &offset=0 );

	///构造函数
	SingalMemoryMap(const std::string& memoryMapFileName );
	~SingalMemoryMap();

protected:
	HANDLE				m_hMemoryMap;		//写入的共享内存句柄，也是用户端读取的共享内存句柄
	
	LPBYTE				m_lpData;			//内存映射的内存指针，
	static const int	MAX_SHARE_SIZE = 2048;	//共享内存的最大值，单位为B

	///互斥量，用于不同进程之间的读写同步
	HANDLE				m_mutex;

	///共享内存的映射文件名称
	std::string			m_memoryMapFileName;
	
};

//
//class SingalMemoryMapWriter:public SingalMemoryMap
//{
//public:
//	/**
//	 * @brief 获取运行实例
//	 * @return 该类的唯一一个实例
//	 */
//	static SingalMemoryMapWriter& getInstance(const std::string& mapFileName )
//	{
//		static SingalMemoryMapWriter smWriter( mapFileName );
//		return smWriter;
//	}
//
//	/**
//	 * @brief 往共享内存中写入数据
//	 * @param pData 指向将写入共享内存的数据的指针
//	 * @param cDataSize 写入数据的大小
//	 * @param offset 写入内存的偏移量
//	 * @return NULL
//	 */
//	virtual void writeData( const void* pData , const int & cDataSize , const int &offset=0 );
//
//
//
//private:
//	///构造函数
//	SingalMemoryMapWriter( const std::string& mapFileName);
//
//	//以下函数只定义不实现，防止出现类拷贝
//	//杜绝 SingalMemoryMapWriter smm = SingalMemoryMapWriter::getInstance()的出现
//	SingalMemoryMapWriter( const SingalMemoryMapWriter& );				
//	SingalMemoryMapWriter & operator = (const SingalMemoryMapWriter&);
//
//	UINT32		m_writeCount;
//};
//
//class SingalMemoryMapReader:public SingalMemoryMap
//{
//public:
//	SingalMemoryMapReader(const std::string& mapFileName );
//
//	/**
//	 * @brief 读取共享内存中的数据
//	 * @param pData 存放读取数据的数据指针
//	 * @param cDataSize 读取数据的大小
//	 * @param offset 读取内存的偏移量
//	 * @return NULL
//	 */
//	virtual void readData( void* pData , const int & cDataSize , const int &offset=0 );
//};