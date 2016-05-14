// msdxSendTest.cpp : 定义控制台应用程序的入口点。
//

#include <conio.h>
#include <windows.h>
#include <assert.h>
#include <process.h>

#include <iostream>
using namespace std;

#include "exportutil.h"
#include "util/report/RtpStat.h"
#include "msdx/config.h"


//#define USAGE "application name  -remote ip -camera name(可选)"
#define HD_VIDEO_CAPTURE_DEVICE "@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}"
#define AUDIO_CAPTURE_DEVICE "Line in (e2eSoft VAudio)"
#define AUDIO_OUTPUT_DEVICE "扬声器 (Realtek High Definition Au"


#define MAX_STR 100

// Win32全局变量:
HINSTANCE hInst;                                      // 当前应用程序实例
char szWindowClass[MAX_STR] = "CoolView";    // 主窗口类名
HWND hWnd = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                CreatePreviewWindow(HINSTANCE hInstance, LPCSTR szTitle, int nWidth, int nHeight, int nCmdShow, HWND &hWnd);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Msdx测试函数
unsigned int __stdcall MsdxTestProc(void * param);  // 用于发送测试指令

// Msdx测试全局变量
const int RTP_COUNT = 100;
char * videoCaptrueDevice = HD_VIDEO_CAPTURE_DEVICE; //视频捕获设备名称
char * audioCaptureDevice = AUDIO_CAPTURE_DEVICE;
char * audioOutputDevice = AUDIO_OUTPUT_DEVICE;
int videoWidth = 1920	;
int videoHeight = 1080;
int audioAec = 0; //是否使用回声对消
int audioRate = 16000;
int audioChannel = 1;
char * audioCodec = MSDX_CONF_AUDIO_CODEC_SPEEX;
int rtpCount = 1; //发送端数量

// 发送端口信息
struct rtpInfo_t
{
    const char* remoteAddr;
    int videoPort;
    int audioPort;
};
rtpInfo_t rtpInfo[RTP_COUNT] = {0};

//定义参数顺序，方便变更
enum ParamIndex {
  kVideoCap = 1, //参数0为程序名
  kAudioCap,
  kAudioOutput,
  kVideoWidth,
  kVideoHeight,
  kAudioAEC,
  kAudioRate,
  kAudioChannel,
  kAudioCodec,
  kRtpCount,
  kRtpAddrBase,
  kRtpVideoPortBase,
  kRtpAudioPortBase,
};



int main(int argc, char* argv[])
{
    // 处理启动参数
    rtpInfo[0].remoteAddr = "127.0.0.1";
    rtpInfo[0].videoPort = 8600;
    rtpInfo[0].audioPort = 8500;

    if (argc >= 2) {
    videoCaptrueDevice = argv[kVideoCap];
  }
  if (argc >= 3) {
    audioCaptureDevice = argv[kAudioCap];
  }
  if (argc >= 4) {
    audioOutputDevice = argv[kAudioOutput];
    }
    if (argc > 4)
    {
        videoWidth = atoi(argv[kVideoWidth]);
        videoHeight = atoi(argv[kVideoHeight]);
    audioAec = atoi(argv[kAudioAEC]);
    audioRate = atoi(argv[kAudioRate]);
    audioChannel = atoi(argv[kAudioChannel]);

    audioCodec = argv[kAudioCodec];
    if (strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_AAC) != 0 &&
        strcmp(audioCodec, MSDX_CONF_AUDIO_CODEC_SPEEX) != 0) {
            printf_s("Error: Invalid audio codec: %s.\n", audioCodec);
            return 1;
    }

        rtpCount = atoi(argv[kRtpCount]);
        if (rtpCount > RTP_COUNT)
        {
            printf_s("Error: Too many rtp send request.\n");
            return 1;
        }

        for (int i = 0; i < rtpCount; ++i)
        {
            rtpInfo[i].remoteAddr = argv[kRtpAddrBase + i * 3];
            rtpInfo[i].videoPort = atoi(argv[kRtpVideoPortBase + i * 3]);
            rtpInfo[i].audioPort = atoi(argv[kRtpAudioPortBase + i * 3]);
        }
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
    sprintf_s(szTitle, "CoolView"); // 不要改变窗口标题，设置AEC时是按照标题找窗口的——蛋疼吧...
    if (!CreatePreviewWindow(hInstance, szTitle, videoWidth, videoHeight, nCmdShow, hWnd))
    {
        std::cout << "Error in CreatePreviewWindow()!" << std::endl;
        return 0;
    }
    std::cout << "hWnd: " << hWnd << std::endl;

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

    // Init
    int result;
    result = msdx_initial();

    msdx_config_set_video_device(videoCaptrueDevice, "", "");

    result = msdx_create_send_graph(false, false);
    CHECK_TEST_RESULT(result==0);

    // Set Rtp Callback function
    /*msdx_send_set_video_rtpstatcallback(NULL,printRtpStat);
    msdx_send_set_audio_rtpstatcallback(NULL,printRtpStat);*/

    // video
    result = msdx_set_send_video_vodec(MSDX_CONF_VIDEO_CODEC_H264, videoWidth, videoHeight, 30);
    CHECK_TEST_RESULT(result==0);

    {
        //VCam不要使用预览，会导致硬编码filter连接出错
        std::string std_str = videoCaptrueDevice;
        if (std_str.find("\\\\?\\root") == std::string::npos) {
          result = msdx_set_send_preview_window(hWnd, 0, 0, videoWidth, videoHeight, 0);
        }
    }

    // audio
    msdx_config_set_audio_device(audioCaptureDevice, audioOutputDevice);
    result = msdx_set_send_audio_codec(audioCodec, audioRate, audioChannel, 16, audioAec != 0);
    CHECK_TEST_RESULT(result==0);

    // connect and add rtp port
    CHECK_TEST_RESULT( msdx_connect_send_graph()==0);

    for (int i = 0; i < rtpCount; ++i)
    {
        result = msdx_add_send_address(rtpInfo[i].remoteAddr, rtpInfo[i].audioPort, rtpInfo[i].remoteAddr, rtpInfo[i].videoPort);
        CHECK_TEST_RESULT(result == 0);
    }

    // run
    CHECK_TEST_RESULT(msdx_run_send_graph() ==0);

    // control test - Pause here
    int audioEncSwitch = 1;
    int videoEncSwitch = 1;
    char c;
	while ((c=getch()) != 'q') 
	{
		switch(c)
		{
		case 'a':
			printf_s("Control audio encoder: %d\n", ++audioEncSwitch%2);
			msdx_control_audio_encoder(audioEncSwitch%2==1);
			break;
		case 'v':
			printf_s("Control video encoder: %d\n", ++videoEncSwitch%2);
			msdx_control_video_encoder(videoEncSwitch%2==1);
			break;
		case 's':
			{
				char ip[128];
				int video_port, audio_port;
				printf_s("Enter new dest(ip, video port, audio port):");
				cin >> ip >> video_port >> audio_port;
				result = msdx_add_send_address(ip, audio_port, ip, video_port);
			}
			break;
		case 'r':
			{
				char ip[128];
				int video_port, audio_port;
				printf_s("Remove dest(ip, video port, audio port):");
				cin >> ip >> video_port >> audio_port;
				result = msdx_delete_send_address(ip, audio_port, ip, video_port);
			}
			break;
		default:
			printf_s("Unknown input %d\n", c);
		}
	}


    // stop
    msdx_destroy_send_graph();
    msdx_uninitial();

    std::cout << "Graph is destroyed." << std::endl;
    SendMessage(hWnd, WM_CLOSE, 0, 0); // quit app

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
