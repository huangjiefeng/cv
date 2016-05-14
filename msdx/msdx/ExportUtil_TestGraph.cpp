/********************************************************************
本源代码文件实现与本地测试Graph相关的API函数。
*********************************************************************/
#pragma warning(disable:4995)
#include "stdafx.h"

#include <assert.h>
#include <string>

#include "DxDefinitions.h"
#include "Exportutil.h"
#include "Util.h"
#include "LogUtil.h"
#include "msdx\config.h"
#include "IGraphController.h"


static msdx::TestGraphParam test_graph_param;
static msdx::ITestGraphController *test_graph_controller;

//建一个辅助对象来维护上述局部变量，以便完成初始化和退出自动销毁
class TestGraphControllerHelper {
public:
    TestGraphControllerHelper()
    {
        SetDefaultParam();
        test_graph_controller = nullptr;
    }

    ~TestGraphControllerHelper()
    {
        if (test_graph_controller)
        {
            delete test_graph_controller;
            test_graph_controller = nullptr;
        }
    }

    void SetDefaultParam()
    {
        test_graph_param.video.crossbar_physical_type = msdx::PhysConn_Auto;
        test_graph_param.video.frame_width = 1280;
        test_graph_param.video.frame_height = 720;
        test_graph_param.video.preview.position_left = 0;
        test_graph_param.video.preview.position_top = 0;
        test_graph_param.video.preview.width = 0;
        test_graph_param.video.preview.height = 0;
        test_graph_param.video.preview.window_handle = 0;
    }
};

static TestGraphControllerHelper test_graph_helper;


//定义在ExportUtil_Common.cpp中
extern int GetAudioSourceDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name);
extern int GetAudioRendererDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name);

int msdx_create_audio_test_graph( const AudioTestType type, const char * inputSrc, const char * outputDevice )
{
    //只允许一个controller存在，故在进行了一次测试后，需要destroy才能进行下次测试
	if (test_graph_controller) return -1;

    //input
    if (type == MSDX_LOCAL_ATYPE_DEVICE)
    {
        //将声音设备FriendlyName转为DisplayName，这都是历史问题了
        if (GetAudioSourceDisplayNameFromFriendlyName(
            inputSrc, test_graph_param.audio.input_device) < 0) {
            return -1;
        }
    } 
    else if (type == MSDX_LOCAL_ATYPE_FILE)
    {
        test_graph_param.audio.input_file = inputSrc;
    }
    else
    {
        LOG_ERROR("Unknown audio test type: %d", type);
    }

    //output
    if (nullptr == outputDevice || strlen(outputDevice) == 0) 
      test_graph_param.audio.output_device = ""; // for Logitech Camera warm-up
    else if (GetAudioRendererDisplayNameFromFriendlyName(
        outputDevice, test_graph_param.audio.output_device) < 0) {
        return -1;
    }

    //build
    test_graph_controller = 
        msdx::GraphControllerFactory::GetInstance().CreateTestGraphController();
    if(test_graph_controller == nullptr) {
        LOG_ERROR("Create graph controller failed.");
        return -1;
    }

    if (STATUS_ERR(test_graph_controller->Build(test_graph_param))) {
        LOG_ERROR("Build graph failed.");
        return -1;
    }

	return static_cast<int>(test_graph_controller->Run());
}


void msdx_destroy_audio_test_graph()
{
	if (!test_graph_controller) return;

	test_graph_controller->Stop();
    delete test_graph_controller;
    test_graph_controller = nullptr;

    test_graph_helper.SetDefaultParam();
}

int MSDXAPI msdx_set_video_test_preview_window(HWND WndHandle, long left, long top, long width, long height)
{
	test_graph_param.video.preview.window_handle = (unsigned long)WndHandle;
    test_graph_param.video.preview.position_top = top;
    test_graph_param.video.preview.position_left = left;
    test_graph_param.video.preview.width = width;
    test_graph_param.video.preview.height = height;

	return 0;
}

int MSDXAPI msdx_create_video_test_graph( const char * video_device, const char*crossbar, const char* cb_input, int width, int height, int fps )
{
	if (test_graph_controller) return -1;

    test_graph_param.video.frame_width = width;
    test_graph_param.video.frame_height = height;
    test_graph_param.video.crossbar = crossbar ? crossbar : "";
    test_graph_param.video.device = video_device ? video_device : "";

    //上层很乱，这里传的是摄像头DisplayName，为了与发送graph一致，要截取出DevicePath
    //test_graph_param.video.device = 
   //     test_graph_param.video.device.substr(test_graph_param.video.device.find_first_of('\\'));//注释by feng
    
    if(cb_input && !test_graph_param.video.crossbar.empty())
    {
        //reference: Working with Crossbars
        //http://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
        if(strcmp(cb_input, "Tuner") == 0) 
            test_graph_param.video.crossbar_physical_type = PhysConn_Video_Tuner;
        else if(strcmp(cb_input, "Composite") == 0)
            test_graph_param.video.crossbar_physical_type = PhysConn_Video_Composite;
        else if(strcmp(cb_input, "SVideo") == 0)
            test_graph_param.video.crossbar_physical_type = PhysConn_Video_SVideo;
        else 
            test_graph_param.video.crossbar_physical_type = msdx::PhysConn_Auto; //这个常量是自定义的
    }
    else
    {
        test_graph_param.video.crossbar_physical_type = msdx::PhysConn_Auto;
    }

    return 0;
}

int MSDXAPI msdx_run_video_test_graph()
{
	if (test_graph_controller) return 0;

    test_graph_controller = 
        msdx::GraphControllerFactory::GetInstance().CreateTestGraphController();
    if(test_graph_controller == nullptr) {
        LOG_ERROR("Create graph controller failed.");
        return -1;
    }

    msdx::dxStatus status = test_graph_controller->Build(test_graph_param);
    if (STATUS_ERR(status)) {
        LOG_ERROR("Build test graph failed.");
    }

	return static_cast<int>(test_graph_controller->Run());
}

void MSDXAPI msdx_destroy_video_test_graph()
{
	if (!test_graph_controller) return;

    test_graph_controller->Stop();
    delete test_graph_controller;
    test_graph_controller = nullptr;

    test_graph_helper.SetDefaultParam();
}

