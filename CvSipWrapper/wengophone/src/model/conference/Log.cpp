#include "stdafx.h"
#include ".\log.h"
#include <Windows.h>

int getApplicationDirPath(char * applicationDirPath) 
{
	char szAppPath[MAX_PATH]      = "";
	//char szAppDirectory[MAX_PATH] = "";

	if(!applicationDirPath) 
		return -1;

	if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
	{
		printf("failed to get module file name!\n");
		return -1;
	}

	// Extract directory
	strncpy(applicationDirPath, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
	applicationDirPath[strlen(applicationDirPath)] = '\0';

	return 0;
}

Log::Log(void)
{
	char logFileName[200] = {0};
	getApplicationDirPath(logFileName);
	strcat( logFileName , "\\owphone_log.txt");
	out.open( logFileName,ofstream::out|ofstream::app);
//	startWrite();
}

Log::~Log(void)
{
	out.close();
}

void Log::startWrite()//ʱ�䣬�ļ�����������
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	char now_time[128];
	sprintf(now_time,"%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
	out<<now_time<<endl;
}

void Log::write(char *param)
{
	out<<param<<endl;
}

void Log::write(int param)
{
	out<<param<<endl;
}

void Log::write(void *param)
{
	out<<param<<endl;
}

void Log::endWrite()
{
	out<<"======================================================="<<endl;
}
