#ifndef PERFORMANCE_UTIL_H
#define PERFORMANCE_UTIL_H
#include <Windows.h>
#include <vector>
#include <Iphlpapi.h>
#include "CpuUsage.h"


using namespace std;



class PerformanceUtil
{
public:

	PerformanceUtil();
	//获取指定名称的内存使用数，单位为KB
	int getProcMemoryUsage( const WCHAR* pProcessName  );

	//获取系统已用的内存使用数，单位为KB //add by lzb
	int getToalMemoryUsage();

	//获取系统可用的内存使用数，单位为KB //add by lzb
	int getAvailableMemoryUsage();

	//获取指定名称的CPU使用率
	int getProcCpuUsage( LPCTSTR pProcessName );
	
	int getTotalCPUUsage(){return _cpuUsage.GetCpuUsage();}

	bool isProcessRunning( const LPCTSTR pProcessName );
	
	//获取上次访问到现在的上行和下行带宽， 大小为KB
	bool getNetworkFlow(double & outBandWidth,double & inBandWidth);

	bool isProcessRunningByID(int processID);

private:
	CCpuUsage		_cpuUsage;
	DWORD           _lastInFlow;
	DWORD           _lastOutFlow;
};

#endif