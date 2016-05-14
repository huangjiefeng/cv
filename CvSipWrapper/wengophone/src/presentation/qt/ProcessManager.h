#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Windows.h>
#include <tchar.h>
#include <cstring>
using namespace std;
#include "Psapi.h"

class ProcessManager
{
public:
	const static int MAX_BUF_SIZE = 512;
public:
	static bool isRunning( TCHAR* dstProcessName )
	{
		TCHAR chBuf[MAX_BUF_SIZE];
		ZeroMemory( chBuf , MAX_BUF_SIZE );
		DWORD dwProcs[1024*2];
		DWORD dwNeeded;
		if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
		{
			//输出出错信息
			wsprintf( chBuf , _T("EnumProcesses Failed (%d).\n") , GetLastError() );
			OutputDebugString(chBuf);
			return -1;
		}

		//计算有多少个进程ID
		DWORD dwProcCount =  dwNeeded / sizeof( DWORD );
		printf( "Processes Count:%d\n" , dwProcCount) ;

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
};
#endif