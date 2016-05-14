// PerformanceUtil.cpp : 定义 DLL 应用程序的导出函数。
//

//#include "stdafx.h"

#include "PerformanceUtil.h"
#include "CpuUsage.h"
#define MAX_BUF_SIZE 512

#include <Psapi.h>

#pragma comment(lib, "IPHLPAPI.lib")
#include <tchar.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


PerformanceUtil::PerformanceUtil()
{
	_lastInFlow=0;
	_lastOutFlow=0;

	//getNetworkFlowIndexList(_networkFlowIndexList);
}

//获取指定名称的内存使用数，单位为KB
int PerformanceUtil::getProcMemoryUsage( const WCHAR* pProcessName )
{
	WCHAR chBuf[MAX_BUF_SIZE];
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
	WCHAR processName[512];
	for( int i=0; i<dwProcCount ; i++ )
	{
		DWORD m_processid = dwProcs[i];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
		if(hProcess)
		{
			if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
			{
				GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
				if( wcsstr(processName , pProcessName )!=0 )
				{
					PROCESS_MEMORY_COUNTERS pmc;
					GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));
					return pmc.PagefileUsage/1024;
				}
			}
		}
	}

	return -1;
}

//获取系统已用内存使用数，单位为KB
int PerformanceUtil::getToalMemoryUsage( )
{
	

	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD tom=memStatus.dwTotalPhys/1024;
	DWORD mem=memStatus.dwAvailPhys/1024;
	int TotalMem = (int)tom;
	int ValidMem = (int)mem;
	return TotalMem-ValidMem;

}

//获取系统可用内存使用数，单位为KB
int PerformanceUtil::getAvailableMemoryUsage( )
{

	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD mem=memStatus.dwAvailPhys/1024;
	int ValidMem = (int)mem;
	return ValidMem;

}


int PerformanceUtil::getProcCpuUsage( LPCTSTR pProcessName )
{
	return _cpuUsage.GetCpuUsage( pProcessName );
}

bool PerformanceUtil::isProcessRunning( const LPCTSTR pProcessName )
{
	WCHAR chBuf[MAX_BUF_SIZE];
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
	WCHAR processName[512];
	for( int i=0; i<dwProcCount ; i++ )
	{
		DWORD m_processid = dwProcs[i];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
		if(hProcess)
		{
			if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
			{
				GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
				if( wcscmp(processName , pProcessName )==0 )
				{
					return true;
				}
			}
		}
	}
	return false;
}



bool PerformanceUtil::getNetworkFlow(double & outBandWidth,double & inBandWidth)
{
	
	DWORD currentInFlow   =   0; 
	DWORD currentOutFlow   =   0; 

	PIP_ADAPTER_INFO pAdapterInfo = NULL;  
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
	{
		//printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return  false;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if(pAdapterInfo == NULL)
		{
			//  printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return false;
		}
	}

	if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		//读取成功      
		PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		while(pAdapter)
		{
			MIB_IFROW*   pIfRow   =   (MIB_IFROW*)malloc(sizeof(MIB_IFROW));
			pIfRow-> dwIndex   =   pAdapter-> Index; 
			if (NO_ERROR == GetIfEntry(pIfRow))
			{
				currentInFlow   +=   pIfRow->dwInOctets; 
				currentOutFlow   +=  pIfRow->dwOutOctets; 
			}
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		//  printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
	}
	if(pAdapterInfo)
		FREE(pAdapterInfo);


	inBandWidth =   double((currentInFlow - _lastInFlow)/1024.0); 
	outBandWidth  =   double((currentOutFlow - _lastOutFlow)/1024.0); 
	
	_lastInFlow = currentInFlow;
	_lastOutFlow = currentOutFlow;   
	
	if(inBandWidth<0)
	{
		_lastInFlow=0;
	}

	if(outBandWidth<0)
	{
		_lastOutFlow=0;
	}

	
	return true;
}

bool PerformanceUtil::isProcessRunningByID(int processID)
{
	WCHAR chBuf[MAX_BUF_SIZE];
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
	WCHAR processName[512];
	for( int i=0; i<dwProcCount ; i++ )
	{
		DWORD m_processid = dwProcs[i];
		
		if(m_processid == processID)
		{
			
		  return true;

		}
	}
	return false;

}