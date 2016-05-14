#include "stdafx.h"

#include <assert.h>
#include <string>

#include "Exportutil.h"
#include "Util.h"
#include "LogUtil.h"
#include "msdx\config.h"
#include "IGraphController.h"


static msdx::SmallGraphParam small_graph_param;
static msdx::ISmallGraphController *small_graph_controller;

//建一个辅助对象来维护上述局部变量，以便完成初始化和退出自动销毁
class SmallGraphControllerHelper {
public:
    SmallGraphControllerHelper()
    {
        SetDefaultParam();
        small_graph_controller = nullptr;
    }

    ~SmallGraphControllerHelper()
    {
        if (small_graph_controller)
        {
            delete small_graph_controller;
            small_graph_controller = nullptr;
        }
    }

    void SetDefaultParam()
    {
        small_graph_param.video.addr.port = 0;
        small_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264;
        small_graph_param.video.frame_width = 1280;
        small_graph_param.video.frame_height = 720;
        small_graph_param.video.fps = 30;
        small_graph_param.video.enable_hwcodec = true;
    }
};

static SmallGraphControllerHelper small_graph_helper;


int MSDXAPI msdx_create_smallvideo_graph(const char * userId)
{
    //do nothing
    return  0;
}

int MSDXAPI msdx_destroy_smallvideo_graphs()
{
    if (small_graph_controller) {
        small_graph_controller->Stop();
        delete small_graph_controller;
        small_graph_controller = nullptr;
    }
    return 0;
}

int MSDXAPI msdx_set_preview_address(const char * audioIP,const int audioport,const char * videoIP,const int videoport)
{
    small_graph_param.video.addr.ip = videoIP;
    small_graph_param.video.addr.port = videoport;
    return 0;
}

int MSDXAPI msdx_set_preview_video_codec(char * codecMIME,int width,int height,int fps,int BitCount)
{
	small_graph_param.video.frame_width = width;
    small_graph_param.video.frame_height = height;
    small_graph_param.video.fps = fps;
    small_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264;

	return 0;
}

void MSDXAPI msdx_preview_set_video_rtpstatcallback(void *pClass, void* pRtpStatCallbackFunc )
{
    //not implemented
}

int MSDXAPI msdx_add_smallvideo_address(const char * audioIP, const int audioport, const char * videoIP, const int videoport,const bool smallVideo )
{
	if (!small_graph_controller) return -1;

    if (videoport != 0) {
        msdx::NetAddr addr;
        addr.ip = videoIP;
        addr.port = videoport;
        return static_cast<int>(small_graph_controller->AddVideoDestination(addr));
    }
    return 0;
}


int MSDXAPI msdx_connect_smallvideo_graph()
{
	if(small_graph_controller != nullptr) return -1;

    //创建graph controller
    small_graph_controller = 
        msdx::GraphControllerFactory::GetInstance().CreateSmallGraphController();
    if(small_graph_controller == nullptr)
    {
        LOG_ERROR("Create graph controller failed.");
        return -1;
    }

	return static_cast<int>(small_graph_controller->Build(small_graph_param));
}

int MSDXAPI msdx_run_smallvideo_graph()
{
	if (!small_graph_controller) return -1;
	return static_cast<int>(small_graph_controller->Run());
}

int MSDXAPI msdx_set_preview_display_window(HWND WndHandle,long left,long top,long width,long height,int fullScreen)
{
	//not implemented
	return 0;
}

int MSDXAPI msdx_get_smallvideo_graph()
{
	if(!small_graph_controller)
		// do not exist
		return -1;
	else
		return 0;
}

int MSDXAPI msdx_delete_smallvideo_address(const char * audioIP, const int audioport, const char * videoIP, const int videoport)
{
    if (!small_graph_controller) return -1;

    msdx::NetAddr addr;
    addr.ip = videoIP;
    addr.port = videoport;

    return static_cast<int>(small_graph_controller->DeleteVideoDestination(addr));
	return 0;
}
