/************************************************************************/
/*   2011-8-19 created 
/*   @author huangkq
/*   description : used to control the ppt by the guest 
/************************************************************************/
#ifndef KINECT_H
#define KINECT_H
#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>
#include <time.h>
#include "MSR_NuiApi.h"
#include "KinectService.h"
using namespace std;
 
#pragma comment(lib,"MSRKinectNUI.lib")

class MyPoint
{
public:
	float fx;
	float fy;
	USHORT depth;
};

class CKinect
{
public:
	CKinect(void);
	virtual ~CKinect(void);

	HRESULT		       Nui_Init();              // 初始化，包括创建线程去响应消息;
	void                    Nui_UnInit( );           // 注销; 
 
	void                    Nui_GotSkeletonAlert( ); // 响应骨架事件;
	void                    Nui_Zero();              // 初始化变量，置零;

	void		              HandlePpt();

	void                    MoveUp();
	void                    MoveDown();
	void                    SetHorizon();
private: 
	
	// create a thread to process the NUI
	static DWORD WINAPI     Nui_ProcessThread(LPVOID pParam);   

	// thread handling;
	HANDLE        m_hThNuiProcess;         // 线程的句柄;
	HANDLE        m_hEvNuiProcessStop;     // 中断线程;

	std::map< int, std::list<MyPoint> > m_History;  
	HANDLE        m_hNextSkeletonEvent;    // 骨架的事件;

	int           m_flagPptShowing;
	
	size_t 	      m_nThreshold;
	DWORD	      m_nUserIndex;	 
	bool          m_bFirstUser;

	int           m_flagPrePptLeftNearToHip;
	int           m_flagNextPptRightNearToHip;

};

#endif
