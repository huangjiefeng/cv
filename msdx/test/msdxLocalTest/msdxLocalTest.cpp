// msdxLocalTest.cpp : 测试“设备测试”相关的模块，以及本地视频播放。
//

#include <conio.h>
#include <windows.h>
#include <assert.h>
#include <process.h>

#include <iostream>
using namespace std;

#include "exportutil.h"
#include "IGraphController.h"


//audio 0: 音频输入设备测试
//      1: 音频文件播放测试
//video: 视频输入设备测试
//play : 本地视频文件播放测试

#define USAGE "Usage 1: audio input_type input_src [output_device]\n" \
    "Usage 2: video display_name width height\n" \
    "Usage 3: play file_path ip video_port audio_port [init_delay_in_sec]"

//audio test相关变量
AudioTestType audioTestType = MSDX_LOCAL_ATYPE_DEVICE;
char * audioCaptureSrc = NULL;
char * audioOutputDevice = NULL;

//video test相关变量
char * videoDisplayName = NULL;
int videoWidth = 0;
int videoHeight = 0;

//play test相关变量
msdx::FilePlayGraphParam filePlayParam;
msdx::IFilePlayGraphController *fileplay_ctrl = nullptr;

// Win32全局变量:
HINSTANCE hInst;                                      // 当前应用程序实例
char szWindowClass[256] = "CoolView";    // 主窗口类名
HWND hWnd = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                CreatePreviewWindow(HINSTANCE hInstance, LPCSTR szTitle, int nWidth, int nHeight, int nCmdShow, HWND &hWnd);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void TestAudio();
void TestFilePlay();

// Msdx测试函数
unsigned int __stdcall TestVideoProc(void * param);  // 用于发送测试指令
unsigned int __stdcall TestFilePlayProc(void * param);

int main(int argc, char* argv[])
{

	//if (argc < 4)
	//{
	//	printf(USAGE);
	//	return 0;
	//}
	argv[1]="video";
	argv[2]="@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}";
	argv[3]="1920";
	argv[4]="1080";
    msdx_initial();

    if (strcmp(argv[1], "audio") == 0) 
    {
        audioTestType = (AudioTestType)atoi(argv[2]);
        //audioCaptureSrc = "C:\\Users\\Public\\Music\\Sample Music\\Kalimba.mp3";
		audioCaptureSrc = argv[3];
		if (argc >= 5) audioOutputDevice = argv[4];

        TestAudio();
	}
	else if (strcmp(argv[1], "video") == 0 || strcmp(argv[1], "play") == 0) 
	{
        if (strcmp(argv[1], "video") == 0) 
        {
		    videoDisplayName = argv[2];
            videoWidth = atoi(argv[3]);
            videoHeight = atoi(argv[4]);

            // 创建Win32窗口对象
            std::cout << "================================" << std::endl;

            HINSTANCE hInstance = NULL;
            const int nCmdShow  = SW_SHOW;      // 该变量取值参见MSDN

            hInstance = GetModuleHandle(NULL);
            std::cout << "hInstance: " << hInstance << std::endl;
            std::cout << "hInstance->unused: " << hInstance->unused << std::endl;

            MyRegisterClass(hInstance);

            // 初始化窗口:
            char szTitle[256] = {0};
            sprintf(szTitle, "LocalTest"); // 不要改变窗口标题，设置AEC时是按照标题找窗口的——蛋疼吧...
            if (!CreatePreviewWindow(hInstance, szTitle, videoWidth, videoHeight, nCmdShow, hWnd))
            {
                std::cout << "Error in CreatePreviewWindow()!" << std::endl;
            }
            std::cout << "hWnd: " << hWnd << std::endl;

            std::cout << "================================" << std::endl << std::endl;

            _beginthreadex(NULL, 0, TestVideoProc, NULL, 0, NULL);
        }
        else
        {
            filePlayParam.file_list.push_back(argv[2]);
            filePlayParam.video.addr.ip = argv[3];
            filePlayParam.video.addr.port = atoi(argv[4]);
            filePlayParam.audio.addr.ip = argv[3];
            filePlayParam.audio.addr.port = atoi(argv[5]);
            if (argc >= 7) filePlayParam.initial_delay = atoi(argv[6]) * 10000000LL;
            else filePlayParam.initial_delay = 0;

            TestFilePlay();
            _beginthreadex(NULL, 0, TestFilePlayProc, NULL, 0, NULL);
        }

        MSG msg;

        // 主消息循环:
        while (GetMessage(&msg, NULL, 0, 0)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
	}
    else
    {
        printf("invalid test type: %s\n", argv[1]);
    }

    msdx_uninitial();
    return 0;
}

unsigned int __stdcall TestVideoProc( void * param )
{
    //每个线程都要初始化COM
    msdx_initial();

    //build graph
    msdx_set_video_test_preview_window(hWnd, 0, 0, videoWidth, videoHeight);
    msdx_create_video_test_graph(videoDisplayName, "", "", videoWidth, videoHeight, 30);
    msdx_run_video_test_graph();

    getch();

    msdx_destroy_video_test_graph();

    msdx_uninitial();

    return 0;
}

void TestAudio()
{
    msdx_create_audio_test_graph(audioTestType, audioCaptureSrc, audioOutputDevice);

    getch();

    msdx_destroy_audio_test_graph();
}

void FilePlayCallback(void * obj, FilePlayStatItem &stat){}
void TestFilePlay()
{
    fileplay_ctrl = 
        msdx::GraphControllerFactory::GetInstance().CreateFilePlayGraphController();
    fileplay_ctrl->Build(filePlayParam);
    fileplay_ctrl->SetFilePlayStatCallBack(nullptr, FilePlayCallback);
    fileplay_ctrl->Run(); //确保与消息循环在同一个线程timer才能生效
}

unsigned int __stdcall TestFilePlayProc(void * param)
{
    // test op
    bool paused = false;
    char c;
    unsigned long op_id = 1;
    while ((c=getch()) != 'q') 
    {
        switch(c)
        {
        case ' ':
            // 32(space) to pause/resume
            if (paused) {
                printf_s("Resume\n");
                fileplay_ctrl->Resume(op_id++);
            } else {
                printf_s("Pause\n");
                fileplay_ctrl->Pause(op_id++);
            }
            paused = !paused;
            break;
        case 's':
            printf_s("Input time to seek to(in seconds): ");
            {
                long t = 31;
                scanf_s("%d", &t);
                fileplay_ctrl->SeekTo(op_id++, t * 10000000LL);
            }
            break;
        default:
            printf_s("Unknown input %d\n", c);
        }
    }

    fileplay_ctrl->Stop();
    delete fileplay_ctrl;

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
