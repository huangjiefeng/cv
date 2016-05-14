// msdxRecvTest.cpp : 定义控制台应用程序的入口点。
//

#include <conio.h>
#include <windows.h>
#include <process.h>
#include <assert.h>
#include <iostream>
using namespace std;
#include "exportutil.h"
#include "util/report/RtpStat.h"

#include "TestSmallVideoRecv.h"
#include "TestNormalVideoRecv.h"
#include "TestFullHDSmallVideo.h"


#define MAX_STR 100

// Win32全局变量:
HINSTANCE hInst;                                      // 当前应用程序实例
char szWindowClass[MAX_STR] = "MSDX Recv Preview";    // 主窗口类名
HWND hWnd1 = NULL;
HWND hWnd2 = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                CreatePreviewWindow(HINSTANCE hInstance, LPCSTR szTitle, int nWidth, int nHeight, int nCmdShow, HWND &hWnd);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Msdx测试函数
unsigned int __stdcall MsdxTestProc(void * param);  // 用于发送测试指令

// Msdx测试全局变量
char* localAddr = "127.0.0.1";//监听地址
int videoPort = 8600;
int audioPort = 8500;
int videoWidth = 1920;
int videoHeight = 1080;
int audioRate = 16000;
int audioChannel = 1;
char * audioCodec = MSDX_CONF_AUDIO_CODEC_SPEEX;
char * audioRenderer = "";

//定义参数顺序，方便变更
enum ParamIndex {
  kAddr = 1, //参数0为程序名
  kVideoPort,
  kAudioPort,
  kVideoWidth,
  kVideoHeight,
  kAudioRate,
  kAudioChannel,
  kAudioCodec,
  kAudioRenderer,
};


int main(int argc, char* argv[])
{
	// 处理启动参数
	if(argc >= 4) {
        localAddr = argv[kAddr];  //可以是组播地址
        videoPort = atoi(argv[kVideoPort]);
        audioPort = atoi(argv[kAudioPort]);
    }
	if (argc >= 9)
	{
		videoWidth = atoi(argv[kVideoWidth]);
		videoHeight = atoi(argv[kVideoHeight]);
		audioRate = atoi(argv[kAudioRate]);
		audioChannel = atoi(argv[kAudioChannel]);

        audioCodec = argv[kAudioCodec];
        if (strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_AAC) != 0 &&
            strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_SPEEX) != 0) {
                printf_s("Error: Invalid audio codec: %s.\n", audioCodec);
                return 1;
        }
	}
    if (argc >= 10) 
    {
        audioRenderer = argv[kAudioRenderer];
    }

	// 创建Win32窗口对象
	std::cout << "================================" << std::endl;

	HINSTANCE hInstance = NULL;
	const int nCmdShow  = SW_SHOW;      // 该变量取值参见MSDN

	hInstance = GetModuleHandle(NULL);
	std::cout << "hInstance: " << hInstance << std::endl;
	std::cout << "hInstance->unused: " << hInstance->unused << std::endl;

	MyRegisterClass(hInstance);

	// 初始化窗口:
	char szTitle[MAX_STR] = {0};
	sprintf_s(szTitle, "MSDX Recv Preview %d", 1);
	if (!CreatePreviewWindow(hInstance, szTitle, videoWidth, videoHeight, nCmdShow, hWnd1))
	{
		std::cout << "Error in CreatePreviewWindow()!" << std::endl;
		return 0;
	}
	std::cout << "hWnd1: " << hWnd1 << std::endl;

	sprintf_s(szTitle, "MSDX Recv Preview %d", 2);
	if (!CreatePreviewWindow(hInstance, szTitle, videoWidth, videoHeight, nCmdShow, hWnd2)) 
	{
		std::cout << "Error in CreatePreviewWindow()!" << std::endl;
		return 0;
	}
	std::cout << "hWnd2: " << hWnd2 << std::endl;

	std::cout << "================================" << std::endl << std::endl;


	_beginthreadex(NULL, 0, MsdxTestProc, NULL, 0, NULL);

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

#define CHECK_TEST_RESULT(_CONDITION) if (!(_CONDITION)) { printf_s("Assert failed at line %d\n", __LINE__); return -1; }

unsigned int __stdcall MsdxTestProc( void * param )
{
	std::cout << "Msdx test start..." << std::endl;

	//初始化
	int result;
	result = msdx_initial();

	//创建大流的接受端
	int graphid=-1;
	const char* localAddress = "127.0.0.1";

	//创建大流的接受端
	//vedio graph
	CHECK_TEST_RESULT( (graphid = msdx_create_recv_graph("recv_test"))>0 );
	result = msdx_set_recv_video_codec(graphid, MSDX_CONF_VIDEO_CODEC_H264, videoWidth,videoHeight, 25, 16);
    msdx_config_set_audio_device("", audioRenderer);
	result = msdx_set_recv_audio_codec(graphid, audioCodec, audioRate, audioChannel, 16);

	CHECK_TEST_RESULT(result==0);

	result = msdx_set_recv_address(graphid,localAddress, audioPort, localAddress, videoPort);

	result = msdx_connect_recv_graph(graphid);
	CHECK_TEST_RESULT(result==0);

	msdx_set_recv_display_window(graphid, (HWND)hWnd1, 0, 0, videoWidth, videoHeight, 0);

	result = msdx_run_recv_graph(graphid);
	CHECK_TEST_RESULT(result == 0);

	//硬编码时更换窗口句柄后画面静止
	std::cout << "Preview window changed. Press a/v stop control recv, c to change wnd, q to quit" << std::endl;
	
    // control test - Pause here
    int audioEncSwitch = 1;
    int videoEncSwitch = 1;
    int curWndIdx = 0;
    HWND hWnds[] = {hWnd1, hWnd2, (HWND)-1};
    char c;
    while ((c=getch()) != 'q') 
    {
        switch(c)
        {
        case 'a':
            printf_s("Control audio recv: %d\n", ++audioEncSwitch%2);
            msdx_control_audio_recv(graphid, audioEncSwitch%2==1);
            break;
        case 'v':
            printf_s("Control video recv: %d\n", ++videoEncSwitch%2);
            msdx_control_video_recv(graphid, videoEncSwitch%2==1);
            break;
        case 'w':
            curWndIdx = (++curWndIdx) % (sizeof(hWnds)/sizeof(HWND));
            printf_s("Change to wnd: %x\n", hWnds[curWndIdx]);
            msdx_set_recv_display_window(graphid, (HWND)hWnds[curWndIdx], 0, 0, videoWidth, videoHeight, 0);
            break;
        default:
            printf_s("Unknown input %d\n", c);
        }
    }

	//销毁接收
	msdx_stop_recv_graph( graphid );
	msdx_destroy_recv_graph( graphid );

	//结束
	msdx_uninitial();

	std::cout << "Graph is destroyed." << std::endl;
	SendMessage(hWnd1, WM_CLOSE, 0, 0); // quit app

	return 0;
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC)WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = NULL;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = NULL;

	return RegisterClassEx(&wcex);
}


//
//   函数: InitInstance(HANDLE, int)
//
//   目的: 保存实例句柄并创建主窗口。
//
BOOL CreatePreviewWindow(HINSTANCE hInstance, LPCSTR szTitle, int nWidth, int nHeight, int nCmdShow, HWND &hWnd)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, unsigned, WORD, LONG)
//
//  目的: 处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_CREATE:
		std::cout << "A preview window created." << std::endl;
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		std::cout << "A preview window destroyed." << std::endl;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
