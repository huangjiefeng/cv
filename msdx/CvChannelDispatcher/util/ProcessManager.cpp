#include "ProcessManager.h"
#include <Psapi.h>
#include <stdio.h>

bool ProcessManager::isRunning( const TCHAR* dstProcessName )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        wsprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return false;
    }

    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                if( _tcscmp(processName , dstProcessName )==0 )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ProcessManager::isRunning( const TCHAR* dstProcessName , int* procCount )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        wsprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return false;
    }

    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    *procCount = 0;
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                if( _tcscmp(processName , dstProcessName )==0 )
                {
                    (*procCount)++;
                }
            }
        }
    }
    if( *procCount>0 )
        return true;
    else
        return false;
}

bool ProcessManager::isRunning( const HANDLE handle )
{
    DWORD dwProcStatus;
    if (GetExitCodeProcess(handle, &dwProcStatus))
    {
        //成功获得状态
        return STILL_ACTIVE == dwProcStatus;
    }
    //否则可能进程句柄已关闭,可认为进程已结束
    return false;
}

bool ProcessManager::killProcess( const TCHAR* dwProcessName )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        wsprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return false;
    }

    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                if( _tcscmp(processName , dwProcessName )==0 )
                {
                    return ::TerminateProcess( hProcess,4);
                }
            }
        }
    }

    //尝试发送关闭动作
    HWND wnd = ::FindWindow( NULL , dwProcessName );
    ::SendMessage( wnd , WM_CLOSE , 0 , 0 );
    return false;
}

bool ProcessManager::setProcessPriority( const TCHAR* dstProcessName , ProcessPriorityClass priorityClass )
{
    TCHAR chBuf[MAX_BUF_SIZE];
    ZeroMemory( chBuf , MAX_BUF_SIZE );
    DWORD dwProcs[1024*2];
    DWORD dwNeeded;
    if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
    {
        //输出出错信息
        _stprintf( chBuf , TEXT("EnumProcesses Failed (%d).\n") , GetLastError() ) ;
        OutputDebugString(chBuf);
        return false;
    }

    //计算有多少个进程ID
    DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

    HMODULE hMod;
    DWORD arraySize;
    TCHAR processName[512];
    for( int i=0; i<dwProcCount ; i++ )
    {
        DWORD m_processid = dwProcs[i];
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_SET_INFORMATION,FALSE,m_processid); 
        if(hProcess)
        {
            if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
            {
                GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
                bool result ;
                if( _tcscmp(processName , dstProcessName )==0 )
                {
                    switch( priorityClass )
                    {
                    case Priority_AboveNormal:
                        result =  SetPriorityClass( hProcess , ABOVE_NORMAL_PRIORITY_CLASS );
                        break;
                    case Priority_BelowNormal:
                        result = SetPriorityClass( hProcess , BELOW_NORMAL_PRIORITY_CLASS );
                        break;
                    case Priority_High:
                        printf("HIGH_PRIORITY_CLASS:%x" , HIGH_PRIORITY_CLASS);
                        result = SetPriorityClass( hProcess , HIGH_PRIORITY_CLASS );
                        break;
                    case Priority_Idle:
                        result = SetPriorityClass( hProcess , IDLE_PRIORITY_CLASS );
                        break;
                    case Priority_Normal:
                        result = SetPriorityClass( hProcess , NORMAL_PRIORITY_CLASS );
                        break;
                    //case Priority_BackgroundBegin:        not support in xp
                    //    result = SetPriorityClass( hProcess , PROCESS_MODE_BACKGROUND_BEGIN );
                    //case Priority_BackgroundEnd:
                    //    result = SetPriorityClass( hProcess , PROCESS_MODE_BACKGROUND_END );
                    case Priority_RealTime:
                        printf("REALTIME_PRIORITY_CLASS:%x" , REALTIME_PRIORITY_CLASS);
                        result = SetPriorityClass( hProcess , REALTIME_PRIORITY_CLASS );
                        break;
                    }
                    DWORD errCode;
                    if( result==false )
                    {
                        errCode = GetLastError();
                    }
                    return result;
                }
            }
        }
    }
    return false;
}

HANDLE ProcessManager::startProcess( const TCHAR* path , const TCHAR* argumentsList, const bool consoleVisible )
{
    STARTUPINFO si;     
    ZeroMemory(&si, sizeof(si));     
    si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		if( consoleVisible )
				si.wShowWindow = SW_SHOW;
		else
				si.wShowWindow = SW_HIDE;

    TCHAR command[MAX_BUF_SIZE] = {0};
    _tcscpy_s(command, sizeof(command), path);
    int len = _tcslen(command);
    command[len] = ' ';
    command[len + 1] = '\0';
    _tcscat_s(command, argumentsList);

		PROCESS_INFORMATION pi;    
		DWORD flag = HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE; 
		// 设置STARTUPINFO中的dwFlags和wShowWindow来实现控制台初始隐藏
		// 这样在必要时候可以调出控制台调试。直接设置CREATE_NO_WINDOW
		// 将导致不创建控制台，不便于调试。 --by Liaokz，2014-1
    /*DWORD flag = HIGH_PRIORITY_CLASS;
    if( consoleVisible )
        flag = flag | CREATE_NEW_CONSOLE;
    else
        flag = flag | CREATE_NO_WINDOW;*/

    BOOL res = CreateProcess(NULL,     
        command,
        NULL,     
        NULL,     
        NULL,     
        flag ,
        NULL,     
        NULL,     
        &si,     
        &pi);             
    if (TRUE == res)     
    {     
        return pi.hProcess;
    }
    else
    {
        return INVALID_HANDLE_VALUE;
    }
}

DWORD ProcessManager::getPIDByHandle( const HANDLE hProcess )
{
    DWORD pid = GetProcessId(hProcess);
    return pid;
}

HANDLE ProcessManager::getHandleByPID( DWORD pid )
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (NULL == hProcess)
    {
        /*DWORD err = GetLastError();
        LOG_ERROR("Open process(PID=%d) failed: code %d", pid, err);*/
        hProcess = INVALID_HANDLE_VALUE;
    }
    return hProcess;
}
