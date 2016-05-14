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
	logFilePath += "\\log";
	printf("Log File Name:%s" , _logFileName);
	if(strlen(_logFileName)>0 )
		logFilePath = logFilePath + "\\" + QString(_logFileName) + ".log";
	else
		logFilePath = logFilePath + "\\coolview.log";

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

Logger* Logger::p_logger = 0;

Logger::Logger()
{
	m_pComponent = NULL;
}


Logger::~Logger(void)
{
	log.endWrite();
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
* @param classname 调用该函数的类名
* @param message 需要记录的信息
*/
void Logger::debug( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tDebug:" );
	log.write( message );
}
void Logger::info( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tInfo:" );
	log.write( message );
}
void Logger::warn( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tWarn:" );
	log.write( message );
}
void Logger::error( const char *component , const char *classname , const char *message )
{
	if( m_pComponent==NULL || strcmp( m_pComponent , component )!=0 )
	{
		m_pComponent = component;
		log.write( component );
	}
	log.write( "\tError:" );
	log.write( message );
}

/**
* @breif 仿照phapiLogFunction实现msdx的log函数
* s-h
* @param msdx_log_level 记录级别
* @param line 记录函数出现的行数
* @param filename 文件名字
* @param message 记录的消息
*/
void msdxLogFunction(int msdx_log_level, const char* filename, int line,const char* message)
{

	static const char* component = "msdx";
	Logger* logger = Logger::getInstance();
	
	char traceBuf[1024];
	_snprintf(traceBuf,sizeof (traceBuf),"\t%s(%d):%s",filename, line, message);

	printf( "%s \n" , message );

	switch (msdx_log_level) {
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

void logFunction( const char* component , int log_level, const char* filename, int line,const char* message )
{
	char traceBuf[1024];
	_snprintf(traceBuf,sizeof (traceBuf),"\t%s(%d):%s",filename, line, message);
	Logger* logger = Logger::getInstance();
	switch (log_level) {
	case LOG_LEVEL_DEBUG:
		printf("%s \n", traceBuf);
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