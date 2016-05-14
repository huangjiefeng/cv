#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Windows.h>
#include <cstring>

typedef unsigned long       DWORD;

enum ProcessPriorityClass
{
	Priority_AboveNormal,
	Priority_BelowNormal,
	Priority_High,
	Priority_Idle,
	Priority_Normal,
	Priority_BackgroundBegin,		//not supported in windows XP/2000
	Priority_BackgroundEnd,
	Priority_RealTime
};

class ProcessManager
{
public:
	const static int MAX_BUF_SIZE = 512;
public:
	static bool isRunning( const TCHAR* dstProcessName );

	static bool isRunning( const TCHAR* dstProcessName , int* procCount );

	static bool killProcess( const TCHAR* dwProcessName );

	static bool setProcessPriority( const TCHAR* dstProcessName , ProcessPriorityClass priorityClass );
};
#endif