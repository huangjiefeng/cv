#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
using namespace std;


static char __global_msg[1024];		//日志信息

enum{
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR
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

public:
	static char _logFileName[128];
};

/**
 * zhenHua.sun 2010-05-26
 * @brief 用于记录各种日志记录
 */
class SipWrapperLogger
{
public:

	~SipWrapperLogger(void);
	
	/** Singleton. */
	static SipWrapperLogger* getInstance();

	/**
	 * @brief Debug 信息记录函数
	 * @param component 调用该函数的组件
	 * @param classname 调用该函数的类名
	 * @param message 需要记录的信息
	 */
	void debug( const char *component , const char *classname , const char *message );

	void info( const char *component , const char *classname , const char *message );

	void warn( const char *component , const char *classname , const char *message );

	void error( const char *component , const char *classname , const char *message );
private:

	SipWrapperLogger();
	class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例  
	{  
		public:  
			~CGarbo()  
			{  
				if (SipWrapperLogger::p_logger)  
				{
					delete SipWrapperLogger::p_logger;
					p_logger = NULL;
				}
			}  
	};   
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数 

private:
	Log				log;
	static SipWrapperLogger*	p_logger;
	const char*		m_pComponent;
};
void logFunction( const char* component , int log_level, const char* filename, int line,const char* message);

#define SIP_LOG_ERROR(message) \
	logFunction( "SipWrapper" , LOG_LEVEL_ERROR, \
	__FILE__, \
	__LINE__,\
	message) 

#define SIP_LOG_INFO(message)\
	logFunction( "SipWrapper" ,LOG_LEVEL_INFO, \
	__FILE__, \
	__LINE__,\
	message)

//debug模式则记录日志，release模式则把调用去除，即不记录日志
#ifdef _DEBUG 
#define SIP_LOG_DEBUG(message)\
	logFunction( "SipWrapper" ,LOG_LEVEL_DEBUG, \
	__FILE__, \
	__LINE__,\
	message)
#else
#define SIP_LOG_DEBUG
#endif // _DEBUG

#endif