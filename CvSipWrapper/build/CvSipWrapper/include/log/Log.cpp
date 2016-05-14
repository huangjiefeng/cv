#include "log.h"
#include <Windows.h>

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

char Log::_logFileName[128] = {0};
//int getApplicationDirPath(char * applicationDirPath) 
//{
//	char szAppPath[MAX_PATH]      = "";
//	//char szAppDirectory[MAX_PATH] = "";
//
//	if(!applicationDirPath) 
//		return -1;
//
//	if(!::GetModuleFileNameA(0, szAppPath, sizeof(szAppPath) - 1))
//	{
//		printf("failed to get module file name!\n");
//		return -1;
//	}
//
//	// Extract directory
//	strncpy(applicationDirPath, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
//	applicationDirPath[strlen(applicationDirPath)] = '\0';
//
//	return 0;
//}

Log::Log(void)
{
	QString logFilePath = QCoreApplication::applicationDirPath();
	logFilePath.replace("/" , "\\");
	printf("Log File Name:%s" , _logFileName);
	if(strlen(_logFileName)>0 )
		logFilePath = logFilePath + "\\log\\" + QString(_logFileName) + ".log";
	else
		logFilePath = logFilePath + "\\log\\coolview.log";

	out.open( qPrintable(logFilePath), ofstream::out|ofstream::app);
	startWrite();
}

Log::~Log(void)
{
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
	out<<"======================================================="<<endl;
}

SipWrapperLogger* SipWrapperLogger::p_logger = 0;

SipWrapperLogger::SipWrapperLogger()
{
	m_pComponent = NULL;
}


SipWrapperLogger::~SipWrapperLogger(void)
{
	log.endWrite();
}

SipWrapperLogger* SipWrapperLogger::getInstance()
{
	if( SipWrapperLogger::p_logger==NULL )
	{
		SipWrapperLogger::p_logger = new SipWrapperLogger();
	}
	return p_logger;
}

/**
* @brief Debug 信息记录函数
* @param component 调用该函数的组件
* @param classname 调用该函数的类名
* @param message 需要记录的信息
*/
void SipWrapperLogger::debug( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tDebug:" );
	log.write( message );
}
void SipWrapperLogger::info( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tInfo:" );
	log.write( message );
}
void SipWrapperLogger::warn( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tWarn:" );
	log.write( message );
}
void SipWrapperLogger::error( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tError:" );
	log.write( message );
}


void logFunction( const char* component , int log_level, const char* filename, int line,const char* message )
{
	char traceBuf[1024];
	_snprintf(traceBuf,sizeof (traceBuf),"\t%s(%d):%s",filename, line, message);
	SipWrapperLogger* logger = SipWrapperLogger::getInstance();
	switch (log_level) {
	case LOG_LEVEL_DEBUG:
		logger->debug(component, "", traceBuf);
		break;
	case LOG_LEVEL_INFO:
		logger->info(component, "", traceBuf);
		break;
	case LOG_LEVEL_WARN:
		logger->warn(component, "", traceBuf);
		break;
	case LOG_LEVEL_ERROR:
		logger->error(component, "", traceBuf);
		break;
	}
}