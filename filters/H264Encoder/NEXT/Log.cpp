
#pragma warning(disable: 4995)

#include "StdAfx.h"
#include "log.h"
#include "Util.h"
#include <Windows.h>


/************************************************************************/
/*  Definition of Log                                                   */
/************************************************************************/

Log::Log(void)
{
	char logFileName[200] = {0};

	getApplicationDirPath(logFileName);
	strcat( logFileName , "\\log\\x264Enc.log");

	out.open( logFileName,ofstream::out|ofstream::app);
	startWrite();
}

Log::~Log(void)
{
	endWrite();
	out.close();
}

void Log::startWrite()//时间，文件名，函数等
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	char now_time[128];
	sprintf(now_time,"%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
	out<<now_time<<endl;
}

void Log::write(const char *param)
{
	out<<param<<endl;
}

void Log::write(const int param)
{
	out<<param<<endl;
}

void Log::write(const void *param)
{
	out<<param<<endl;
}

void Log::endWrite()
{
	out<<"======================================================="<<endl<<endl;
	out.flush();
}


/************************************************************************/
/*  Definition of Logger                                                */
/************************************************************************/

Logger* Logger::p_logger = 0;
Logger::CGarbo Logger::Garbo;

Logger::Logger()
{
	m_pComponent = NULL;
}


Logger::~Logger(void)
{
}

Logger* Logger::getInstance()
{
	if( Logger::p_logger==NULL )
	{
		Logger::p_logger = new Logger();
	}
	return p_logger;
}

/**
* @brief Debug 信息记录函数
* @param component 调用该函数的组件
* @param message 需要记录的信息
*/
void Logger::write( int msdx_log_level, const char *component , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}

	DWORD dwPID = GetCurrentProcessId();
	char traceBuf[1024];

	switch (msdx_log_level) {
	case MSDX_LOG_LEVEL_DEBUG:
		sprintf_s(traceBuf, sizeof (traceBuf), "\t[PID:%5d] Debug:", dwPID);
		break;
	case MSDX_LOG_LEVEL_INFO:
		sprintf_s(traceBuf, sizeof (traceBuf), "\t[PID:%5d] Info :", dwPID);
		break;
	case MSDX_LOG_LEVEL_WARN:
		sprintf_s(traceBuf, sizeof (traceBuf), "\t[PID:%5d] Warn :", dwPID);
		break;
	case MSDX_LOG_LEVEL_ERROR:
		sprintf_s(traceBuf, sizeof (traceBuf), "\t[PID:%5d] Error:", dwPID);
		break;
	}

	log.write(traceBuf);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf_s(traceBuf, sizeof (traceBuf),"\t%02d:%02d:%02d.%03d : %s"
		, sysTime.wHour
		, sysTime.wMinute
		, sysTime.wSecond
		, sysTime.wMilliseconds
		, message);
	
	log.write( traceBuf );

}


/**
* @breif 仿照phapiLogFunction实现msdx的log函数
* s-h
* @param msdx_log_level 记录级别
* @param line 记录函数出现的行数
* @param filename 文件名字
* @param message 记录的消息
*/
void msdxLogFunction(int msdx_log_level, const char* filename, int line, const char* message, ...)
{
	static const char* component = "SCUT_X264_ENC";
	Logger* logger = Logger::getInstance();
	
	char msgBuf[1024];
	if (message == NULL)
	{
		strcpy_s(msgBuf, sizeof(msgBuf), "\n");
	}
	else
	{
		va_list va_args;
		va_start(va_args, message);
		vsprintf_s(msgBuf, sizeof(msgBuf), message, va_args);
		va_end(va_args);
	}

	logger->write(msdx_log_level, component, msgBuf);
	printf_s("[%s] %s\n", component, msgBuf);

}

//void msdxLogResult( msdxResult rs, const char* message, ... )
//{
//	static const char* component = "msdxDVR";
//	Logger* logger = Logger::getInstance();
//
//	char msgBuf[2048];
//	if (message == NULL)
//	{
//		char cause[2048];
//		strcpy_s(cause, sizeof(cause), (rs.errDesc->ToString("\t\t")).c_str());
//		strcpy_s(msgBuf, sizeof(msgBuf), "Caused by: \n");
//		strcat_s(msgBuf, cause);
//	}
//	else
//	{
//		char cause[2048];
//		strcpy_s(cause, sizeof(cause), (rs.errDesc->ToString("\t\t")).c_str());
//
//		va_list va_args;
//		va_start(va_args, message);
//		vsprintf_s(msgBuf, sizeof(msgBuf), message, va_args);
//		va_end(va_args);
//
//		strcat_s(msgBuf, " Caused by: \n");
//		strcat_s(msgBuf, cause);
//
//	}
//
//	logger->write(MSDX_LOG_LEVEL_ERROR, component, msgBuf);
//
//}
