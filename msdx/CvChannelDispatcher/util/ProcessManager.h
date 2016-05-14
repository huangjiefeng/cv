#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <tchar.h>
#include <Windows.h>


enum ProcessPriorityClass
{
    Priority_AboveNormal,
    Priority_BelowNormal,
    Priority_High,
    Priority_Idle,
    Priority_Normal,
    Priority_BackgroundBegin, //not supported in windows XP/2000
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

    static bool isRunning( const HANDLE handle );

    static bool killProcess( const TCHAR* dwProcessName );

    static bool setProcessPriority( const TCHAR* dstProcessName , ProcessPriorityClass priorityClass );

    static HANDLE startProcess( const TCHAR* path , const TCHAR* argumentsList, const bool consoleVisible );

    static DWORD getPIDByHandle(const HANDLE hProcess);

    static HANDLE getHandleByPID(DWORD pid);
};
#endif