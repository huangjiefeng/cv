//此文件包含Renderer和其他的filter实现

#include <atlbase.h>
#include <assert.h>
#include <stdio.h> //sprintf_s
#include <Mfidl.h> //IMFGetService
#include <evr.h> //IMFVideoDisplayControl 

#include "InterfaceDefination.h"
#include "FilterContainer.h"
#include "GraphUtil.h"
#include "LogUtil.h"
#include "Util.h"

using namespace msdx;



//AutoVideoRendererContainer

msdx::AutoVideoRendererContainer::~AutoVideoRendererContainer()
{
    if (graph_builder_)
    {
        if (!evr_renderer_filter_) 
        {
            //普通渲染器要移除父窗口
            CComQIPtr<IVideoWindow> vid_win = graph_builder_;
            if (vid_win)
            {
                vid_win->put_Owner(NULL); //微软说，释放窗口以防止出错
            }
        }
        else
        {
            graph_builder_->RemoveFilter(evr_renderer_filter_);
        }
    }
}

msdx::dxStatus msdx::AutoVideoRendererContainer::AddToGraph( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, bool use_evr)
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Try to render null graph.");
    graph_builder_ = owner_graph;

    HRESULT hr;
    if (use_evr)
    {
        hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"EVR", 
            CLSID_EnhancedVideoRenderer, &evr_renderer_filter_);
        if (FAILED(hr)) LOG_WARN("Add EVR failed, use default video randerer.");
    }

    if (upstream_pin)
    {
        LOG_PRINTF("Rendering video pin.");
        //渲染pin，该方法会尝试查找renderer filter，找到上面添加的filter，就是用之；
        //否则会自动创建一个。所以这个类叫AutoVideoRendererContainer
        hr = graph_builder_->Render(upstream_pin);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Auto render video pin failed");
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::AutoVideoRendererContainer::SetPreviewWindow(const VideoWindow &window)
{
    LOG_INFO("Set preview window:%x x:%d y:%d w:%d h:%d", 
        window.window_handle, window.position_left, window.position_top, window.width, window.height);
    if (evr_renderer_filter_)
    {
        //存在EVR时
        return SetEVRWindow(window.window_handle, 
            window.position_left, window.position_top, window.width, window.height);
    }
    //普通Renderer
    return SetVMRWindow(window.window_handle, 
        window.position_left, window.position_top, window.width, window.height);
}

msdx::dxStatus msdx::AutoVideoRendererContainer::SetVMRWindow( 
    unsigned long preview_window, long left, long top, long width, long height )
{
    //配置渲染器
    CComQIPtr<IVideoWindow> vid_win = graph_builder_;
    CHK_FAIL_MSG_RETURN(!vid_win, DX_ERR_BAD_INTERFACE, "Query IVideoWindow of graph failed.");

    HRESULT hr;
    OAHWND old_win_handle;
    vid_win->get_Owner(&old_win_handle);
    if(old_win_handle && old_win_handle != (OAHWND)preview_window)
    {
        LOG_INFO("preview window change from %x to %x", old_win_handle, preview_window);
        hr = vid_win->NotifyOwnerMessage(old_win_handle, WM_DISPLAYCHANGE, 0, 0);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Call IVideoWindow::NotifyOwnerMessage failed");
    }

    hr = vid_win->put_Owner((OAHWND)preview_window);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Call IVideoWindow::put_Owner failed");

    //监听窗口消息，如双击 ....zhenHua.sun 2010-09-08
    hr = vid_win->put_MessageDrain((OAHWND)preview_window);
    hr = vid_win->put_WindowStyle(WS_CHILD);
    hr = vid_win->SetWindowPosition(left, top, width, height);
    hr = vid_win->put_Visible(OATRUE);
    hr = vid_win->SetWindowForeground(OATRUE);
    hr = vid_win->put_FullScreenMode(OAFALSE);

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::AutoVideoRendererContainer::SetEVRWindow( 
    unsigned long preview_window, long left, long top, long width, long height )
{
    HRESULT hr;
    CComPtr<IMFVideoDisplayControl> display_control;
    CComQIPtr<IMFGetService> mf_get_service = evr_renderer_filter_;
    CHK_FAIL_MSG_RETURN(!mf_get_service, DX_ERR_BAD_INTERFACE, "EVR has no IMFGetService.");

    hr = mf_get_service->GetService(MR_VIDEO_RENDER_SERVICE, 
        IID_IMFVideoDisplayControl, (void**)&display_control);
    CHK_FAIL_MSG_RETURN(!display_control, DX_ERR_BAD_INTERFACE, "EVR has no IMFVideoDisplayControl.");

    hr = display_control->SetVideoWindow((HWND)preview_window);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Set evr window handle failed.");
    hr = display_control->SetAspectRatioMode(MFVideoARMode_PreservePicture);

    RECT rect;
    rect.left = left;
    rect.top = top;
    rect.right = left + width;
    rect.bottom = top + height;

    hr = display_control->SetVideoPosition(NULL, &rect);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Set evr window position failed.");

    hr = display_control->RepaintVideo();

    return DX_ERR_NONE;
}



//AutoAudioRendererContainer

AutoAudioRendererContainer::~AutoAudioRendererContainer()
{
    if (graph_builder_ && audio_renderer_filter_) {
        graph_builder_->RemoveFilter(audio_renderer_filter_);
    }
}

dxStatus AutoAudioRendererContainer::AddFilter(
    IGraphBuilder *owner_graph, IPin *upstream_pin, std::string display_name)
{
    //允许不连接，方便渲染文件时使用，见TestGraphController实现
    //CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect audio renderer to null pin.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add audio renderer to null graph.");
    graph_builder_ = owner_graph;

    LOG_INFO("Add audio render: %s", display_name.c_str());

    HRESULT hr = GraphUtil::GetFilterByDisplayName(
        Util::str2wstr(display_name).c_str(),
        &audio_renderer_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Get audio renderer failed.");

    hr = graph_builder_->AddFilter(audio_renderer_filter_, L"Audio Renderer");
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add audio renderer to graph failed.");

    if (upstream_pin)
    {
        hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, audio_renderer_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect audio renderer failed.");
    }

    return DX_ERR_NONE;
}


//InfPinTeeContainer

msdx::InfPinTeeContainer::~InfPinTeeContainer()
{
    if (graph_builder_ && inf_pin_tee_filter_)
    {
        graph_builder_->RemoveFilter(inf_pin_tee_filter_);
    }
}

msdx::dxStatus msdx::InfPinTeeContainer::AddToGraph( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, const wchar_t *name )
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect inf_pin_tee to null pin.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add inf_pin_tee to null graph.");
    graph_builder_ = owner_graph;

    if (!name || wcslen(name) == 0) filter_name_ = L"Inf";
    else filter_name_ = name;

    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, filter_name_.c_str(), 
        GraphUtil::clsid_inf_pin_tee, &inf_pin_tee_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add inf_pin_tee filter failed.");

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, inf_pin_tee_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect to inf_pin_tee failed.");

    return DX_ERR_NONE;
}

IPin * msdx::InfPinTeeContainer::GetUnconnectedPin()
{
    //msdx模块约定，通过返回值返回COM接口时不增加引用计数，故这里COM指针会释放一个计数
    CComPtr<IPin> pin;
    GraphUtil::GetUnconnectedPin(inf_pin_tee_filter_, PINDIR_OUTPUT, &pin);
    if (!pin)
    {
        LOG_ERROR("Cannot get unconnected pin of inf_pin_tee");
    }
    return pin;
}

