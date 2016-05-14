#include "ProcessManager.h"
#include <Psapi.h>
#include <tchar.h>

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
		return true; // 假定进程存在，防止意外重启
	}

	//计算有多少个进程ID
	DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

	HMODULE hMod;
	DWORD arraySize;
	TCHAR processName[512];
	for( int i=0; i<dwProcCount ; i++ )
	{
		DWORD m_processid = dwProcs[i];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
		//HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ , FALSE , m_processid );
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
						result = SetPriorityClass( hProcess , HIGH_PRIORITY_CLASS );
						break;
					case Priority_Idle:
						result = SetPriorityClass( hProcess , IDLE_PRIORITY_CLASS );
						break;
					case Priority_Normal:
						result = SetPriorityClass( hProcess , NORMAL_PRIORITY_CLASS );
						break;
					case Priority_BackgroundBegin:
						result = SetPriorityClass( hProcess , PROCESS_MODE_BACKGROUND_BEGIN );
						break;
					case Priority_BackgroundEnd:
						result = SetPriorityClass( hProcess , PROCESS_MODE_BACKGROUND_END );
						break;
					case Priority_RealTime:
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