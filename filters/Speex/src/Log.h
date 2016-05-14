#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <stdarg.h>
//#include "msdxResult.h"
using namespace std;

enum{
	MSDX_LOG_LEVEL_DEBUG,
	MSDX_LOG_LEVEL_INFO,
	MSDX_LOG_LEVEL_WARN,
	MSDX_LOG_LEVEL_ERROR
};	//zhenHua.sun 2010-05-26

class Log
{
public:
	Log(void);
	~Log(void);

	void startWrite();//时间，文件名，函数等

	void write(const char *param);
	void write(const int param);
	void write(const void *p);
	void endWrite();

private:
	ofstream out;
};

/**
 * zhenHua.sun 2010-05-26
 * @brief 用于记录各种日志记录
 */
class Logger
{
public:

	~Logger(void);
	
	/** Singleton. */
	static Logger* getInstance();

	/**
	 * @brief Debug 信息记录函数
	 * @param component 调用该函数的组件
	 * @param message 需要记录的信息
	 */
	void write( int msdx_log_level, const char *component , const char *message );

private:

	Logger();
	class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例  
	{  
		public:
			~CGarbo()  
			{  
				if (Logger::p_logger)  
				{
					delete Logger::p_logger;
					p_logger = NULL;
				}
			}  
	};   
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数 

private:
	Log				log;
	static Logger*	p_logger;
	const char*		m_pComponent;
};


void msdxLogFunction(int msdx_log_level, const char* filename, int line,const char* message, ...);

#define LOG_ERROR(message, ...) \
    msdxLogFunction(MSDX_LOG_LEVEL_ERROR, \
            __FILE__, \
            __LINE__,\
			message, \
			__VA_ARGS__)

#define LOG_WARN(message, ...) \
	msdxLogFunction(MSDX_LOG_LEVEL_WARN, \
			__FILE__, \
			__LINE__,\
			message, \
			__VA_ARGS__)

#define LOG_INFO(message, ...) \
	msdxLogFunction(MSDX_LOG_LEVEL_INFO, \
			__FILE__, \
			__LINE__,\
			message, \
			__VA_ARGS__)

#ifdef _DEBUG 

#define LOG_DEBUG(message, ...) \
	msdxLogFunction(MSDX_LOG_LEVEL_DEBUG, \
			__FILE__, \
			__LINE__,\
			message, \
			__VA_ARGS__)

#define LOG_PRINTF	printf

#else
#define LOG_DEBUG(message)
#define LOG_PRINTF
#endif // _DEBUG


#endif