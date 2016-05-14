//此文件包含RTP有关的filter实现

#include <atlbase.h>
#include <assert.h>
#include <stdio.h> //sprintf_s

#include "InterfaceDefination.h"
#include "FilterContainer.h"
#include "GraphUtil.h"
#include "LogUtil.h"
#include "Util.h"

using namespace msdx;



//RTPSender1Container

RTPSender1Container::RTPSender1Container()
{
    rtp_type_ = RTPSenderContainer::RTP_TYPE_NONE;
}

RTPSender1Container::~RTPSender1Container()
{
    Clear();
}

std::string msdx::RTPSender1Container::GetKey( const NetAddr &addr )
{
    char buf[128];
    sprintf_s(buf, sizeof(buf), "%s:%d", addr.ip.c_str(), addr.port);
    return buf;
}

//对于第一代RTP而言，并非实际添加RTP filter，而是添加一个multi smart tee;
//每AddDest一次添加一个RTP filter
msdx::dxStatus msdx::RTPSender1Container::AddToGraph( IGraphBuilder *owner_graph, RtpType type, IPin *upstream_pin )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add rtp to null graph.");
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect rtp to null pin.");
    graph_builder_ = owner_graph;
    rtp_type_ = type;

    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Multi Smart Tee", 
        GraphUtil::clsid_scut_multi_smart_tee, &multi_smart_tee_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add multi_smart_tee_filter to graph failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, multi_smart_tee_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect multi_smart_tee_filter failed.");

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RTPSender1Container::AddDest( NetAddr addr, RtpCallback *callback )
{
    if (!graph_builder_ || rtp_type_ == RTPSenderContainer::RTP_TYPE_NONE)
    {
        LOG_ERROR("Add rtp destination before initialized.");
        return DX_ERR_ABORT;
    }

    if (addr.ip.empty() || addr.port == 0) 
    {
        LOG_WARN("Adding invalid %s ports %s:%d, ignored.",
            RtpTypeName(), 
            addr.ip.c_str(), addr.port);
        return DX_ERR_NONE;
    }

    std::string rtp_key = GetKey(addr);
    LOG_INFO("Adding rtp %s dest: %s",
        RtpTypeName(), 
        rtp_key.c_str());

    //检查重复性
    if (working_filters_.find(rtp_key) != working_filters_.end())
    {
        LOG_WARN("Add %s rtp dest failed: %s exist.", 
            RtpTypeName(), rtp_key.c_str());
        return DX_WARN_OP_IGNORED;
    }

    //add filter to graph
    IBaseFilter *rtp_filter = GetIdleFilter();
    if (!rtp_filter)
    {
        LOG_ERROR("Add %s rtp dest failed: cannot get idle rtp sender filter.", 
            RtpTypeName());
    }

    //此时filter已经连接到graph中，无论接下来配置成功与否都视为工作中
    working_filters_[rtp_key] = rtp_filter;

    //config filter
    CComQIPtr<ISCUTNCRTPOut> rtp_out_config = rtp_filter;
    CHK_FAIL_MSG_RETURN(!rtp_out_config, DX_ERR_BAD_INTERFACE, "rtp sender has no ISCUTNCRTPOut.");

    HRESULT hr = rtp_out_config->updateIPList(addr.ip.c_str(), addr.port);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Config rtp sender failed: %s", rtp_key.c_str());

    //设置回调函数
    if (callback) {
        rtp_out_config->setRtpStatCallback(callback->obj, callback->func);
    }

    //run filter
    FILTER_STATE state = State_Stopped;
    multi_smart_tee_filter_->GetState(100, &state);
    if(state == State_Running)
    {
        REFERENCE_TIME graph_start_time = 0;
        CComQIPtr<IGraphConfig> graph_config = graph_builder_;
        if (graph_config) graph_config->GetStartTime(&graph_start_time);
        rtp_filter->Run(graph_start_time);
    }

    //important: start pin of multi smart tee!
    CComQIPtr<IMULTISMARTTEE> smart_tee_config = multi_smart_tee_filter_;
    if (smart_tee_config)
    {
        hr = smart_tee_config->StartTransfer(rtp_filter);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Start transfer to %s failed.", rtp_key.c_str());
    }

    PrintPorts();

    return DX_ERR_NONE;
}

IBaseFilter *RTPSender1Container::GetIdleFilter()
{
    IBaseFilter *rtp_filter = NULL;

    //首先尝试从空闲filter中获取一个
    if (!idle_filters_.empty())
    {
        rtp_filter = idle_filters_.back();
        idle_filters_.pop_back();
        return rtp_filter;
    }

    //没有空闲的则创建
    GUID rtp_clsid = (rtp_type_ == RTPSenderContainer::RTP_TYPE_VIDEO ? 
        GraphUtil::clsid_scut_rtp_out_v : GraphUtil::clsid_scut_rtp_out_a);

    wchar_t name[64] = {0};
    swprintf_s(name, L"%s RTPOut", Util::str2wstr(RtpTypeName()).c_str());

    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, name, rtp_clsid, &rtp_filter);
    CHK_FAIL_MSG_RETURN(FAILED(hr), nullptr, "Add rtp out filter to graph failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, multi_smart_tee_filter_, rtp_filter);
    CHK_FAIL_MSG_RETURN(FAILED(hr), nullptr, "Connect rtp out filter failed.");

    return rtp_filter;
}

msdx::dxStatus msdx::RTPSender1Container::RemoveDest( NetAddr addr )
{
    if (!graph_builder_ || rtp_type_ == RTPSenderContainer::RTP_TYPE_NONE)
    {
        LOG_ERROR("Remove rtp destination before initialized.");
        return DX_ERR_ABORT;
    }

    std::string rtp_key = GetKey(addr);
    LOG_INFO("Removing rtp %s dest: %s",
        RtpTypeName(), 
        rtp_key.c_str());

    if (working_filters_.find(rtp_key) == working_filters_.end())
    {
        LOG_WARN("Remove %s rtp dest failed: %s not exist.", 
            RtpTypeName(), rtp_key.c_str());
        return DX_WARN_OP_IGNORED;
    }

    HRESULT hr;
    IBaseFilter *rtp_filter = working_filters_[rtp_key];

    //important: stop pin of multi smart tee first!
    CComQIPtr<IMULTISMARTTEE> smart_tee_config = multi_smart_tee_filter_;
    if (smart_tee_config)
    {
        hr = smart_tee_config->StopTransfer(rtp_filter);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Stop transfer to %s failed.", rtp_key.c_str());
    }

    //stop sending
    CComQIPtr<ISCUTNCRTPOut> rtp_out_config = rtp_filter;
    CHK_FAIL_MSG_RETURN(!rtp_out_config, DX_ERR_BAD_INTERFACE, "rtp sender has no ISCUTNCRTPOut.");

    rtp_out_config->stopSender();
    rtp_out_config->setRtpStatCallback(nullptr, nullptr);

    //move filter to idle list
    working_filters_.erase(rtp_key);
    idle_filters_.push_back(rtp_filter);

    PrintPorts();

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RTPSender1Container::Clear()
{
    if (!graph_builder_)
    {
        return DX_ERR_NONE;
    }
    while (!idle_filters_.empty())
    {
        IBaseFilter *filter = idle_filters_.back();
        idle_filters_.pop_back();
        graph_builder_->RemoveFilter(filter);
        filter->Release();
    }
    while (!working_filters_.empty())
    {
        WorkingFilterMap::iterator it = working_filters_.begin();
        IBaseFilter *filter = it->second;
        working_filters_.erase(it);
        graph_builder_->RemoveFilter(filter);
        filter->Release();
    }
    if (multi_smart_tee_filter_)
    {
        graph_builder_->RemoveFilter(multi_smart_tee_filter_);
        multi_smart_tee_filter_ = NULL;
    }

    LOG_INFO("Clear all %s rtp sending ports.", 
        RtpTypeName());

    return DX_ERR_NONE;
}

dxStatus RTPSender1Container::PauseOverallSending(bool stop)
{
    //使用一代RTP时，通过设置上游的multi_smart_tee的全局分发来控制是否停止发送音视频
    //SetOverallTransfer设置，不影响针对每一个分发端口的发送设置，就是说：
    //全局停止发送并恢复后，处于idle列表中的rtp仍然不会收到数据，只有working map中的
    //rtp可收到数据。
    CHK_FAIL_MSG_RETURN(!multi_smart_tee_filter_, DX_ERR_NULL_PTR, 
        "Cannot stop rtp sending: null multi_smart_tee.");

    CComQIPtr<IMULTISMARTTEE> smart_tee_config = multi_smart_tee_filter_;
    if (smart_tee_config)
    {
        HRESULT hr = smart_tee_config->SetOverallTransfer(!stop);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Set multi_smart_tee overall transfer state failed.");

        LOG_INFO("%s sending %s data to remote.",
            (stop ? "Stop" : "Resume"),
            RtpTypeName());
    }
    else
    {
        LOG_ERROR("Cannot stop rtp sending: multi_smart_tee has no control interface.");
        return DX_ERR_BAD_INTERFACE;
    }

    return DX_ERR_NONE;
}

unsigned long msdx::RTPSender1Container::GetCount()
{
    return working_filters_.size();
}

char *RTPSender1Container::RtpTypeName()
{
    switch (rtp_type_)
    {
    case msdx::RTPSenderContainer::RTP_TYPE_NONE:
        return "None";
        break;
    case msdx::RTPSenderContainer::RTP_TYPE_VIDEO:
        return "Video";
        break;
    case msdx::RTPSenderContainer::RTP_TYPE_AUDIO:
        return "Audio";
        break;
    default:
        break;
    }
    return "Unknown";
}

void RTPSender1Container::PrintPorts()
{
    std::string msg = "%s rtp sending ports:\n";
    for (auto &pair : working_filters_)
    {
        //pair.first就是key，就是ip端口对
        msg += (std::string("\t\t") + pair.first + "\n");
    }
    msg += "\tIdle ports count: %d";
    //注意格式化信息要与上面生成的一致哦
    LOG_INFO(msg.c_str(), 
        RtpTypeName(),
        idle_filters_.size());
}


//RTPReceiver1Container

RTPReceiver1Container::~RTPReceiver1Container()
{
    if (graph_builder_ && rtp_receiver_filter_)
    {
        graph_builder_->RemoveFilter(rtp_receiver_filter_);
    }
}

msdx::dxStatus msdx::RTPReceiver1Container::AddFilter( 
    IGraphBuilder *owner_graph, CMediaType media_type, const char *rtp_id)
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Cannot add rtp receiver to null graph.");
    graph_builder_ = owner_graph;
    media_type_ = media_type;
    std::wstring filter_name;
    CLSID filter_clsid;

    //check input
    if (media_type_.majortype == MEDIATYPE_Video) {
        filter_clsid = GraphUtil::clsid_scut_rtp_in_v;
        filter_name = L"Video Receiver";
    }
    else if (media_type_.majortype == MEDIATYPE_Audio) {
        filter_clsid = GraphUtil::clsid_scut_rtp_in_a;
        filter_name = L"Audio Receiver";
    }
    else {
        LOG_ERROR("Add rtp receiver for unknown media type.");
        return DX_ERR_BAD_PARAM;
    }

    //add filter
    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, filter_name.c_str(), 
        filter_clsid, &rtp_receiver_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Add rtp receiver filter to graph failed: 0x%08x", hr);

    //set media type
    CComQIPtr<ISCUTNCRTPRecv> rtp_receive_config = rtp_receiver_filter_;
    if (rtp_receive_config)
    {
        rtp_receive_config->setId(rtp_id);
        hr = rtp_receive_config->setFormat(&media_type_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Set rtp receiver format failed: 0x%08x", hr);
    }
    else
    {
        LOG_ERROR("Query ISCUTNCRTPRecv of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RTPReceiver1Container::SetSource( NetAddr addr )
{
    CHK_FAIL_MSG_RETURN(!rtp_receiver_filter_, DX_ERR_NULL_PTR, "Cannot config null rtp receiver.");

    CComQIPtr<ISCUTNCRTPRecv> rtp_receive_config = rtp_receiver_filter_;
    if (!rtp_receive_config)
    {
        LOG_ERROR("Query ISCUTNCRTPRecv of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    HRESULT hr = rtp_receive_config->setAddr(addr.ip.c_str(), addr.port);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Set rtp receiver source failed: 0x%08x", hr);

    LOG_INFO("Set %s receiver source: ip:%s, port:%d",
        media_type_.majortype == MEDIATYPE_Video ? "video" : "audio",
        addr.ip.c_str(), addr.port);

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RTPReceiver1Container::SetCallback( RtpCallback *callback )
{
    CComQIPtr<ISCUTNCRTPRecv> rtp_receive_config = rtp_receiver_filter_;
    if (!rtp_receive_config)
    {
        LOG_ERROR("Query ISCUTNCRTPRecv of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    if (callback)
    {
        rtp_receive_config->setRtpStatCallback(callback->obj, callback->func);
    }
    else
    {
        rtp_receive_config->setRtpStatCallback(nullptr, nullptr);
    }
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RTPReceiver1Container::StopReceiving( bool stop )
{
    CHK_FAIL_MSG_RETURN(!rtp_receiver_filter_, DX_ERR_NULL_PTR, "Cannot stop null rtp receiver.");

    LOG_INFO("%s receiving %s data", 
        (stop ? "Stop" : "Resume"),
        (media_type_.majortype == MEDIATYPE_Video ? "video" : "audio"));

    CComQIPtr<ISCUTNCRTPRecv> rtp_receive_config = rtp_receiver_filter_;
    if (!rtp_receive_config)
    {
        LOG_ERROR("Query ISCUTNCRTPRecv of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    if (stop) rtp_receive_config->stopReceiver();
    else rtp_receive_config->startReceiver();

    return DX_ERR_NONE;
}

IPin * msdx::RTPReceiver1Container::GetOutputPin()
{
    //msdx模块约定，通过返回值返回COM接口时不增加引用计数，故这里COM指针会释放一个计数
    CComPtr<IPin> out_pin;
    if (rtp_receiver_filter_)
    {
        HRESULT hr = GraphUtil::GetUnconnectedPin(rtp_receiver_filter_, PINDIR_OUTPUT, &out_pin);
        if (FAILED(hr))
        {
            LOG_ERROR("Get rtp receiver output pin failed: 0x%08x", hr);
        }
    }
    return out_pin;
}



// RTPSender2Container

RTPSender2Container::~RTPSender2Container()
{
    if (graph_builder_ && rtp_sender_filter_)
    {
        graph_builder_->RemoveFilter(rtp_sender_filter_);
    }
}


msdx::dxStatus RTPSender2Container::AddToGraph( 
    IGraphBuilder *owner_graph, RtpType type, IPin *upstream_pin )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add rtp to null graph.");
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect rtp to null pin.");
    graph_builder_ = owner_graph;
    rtp_type_ = type;

    //二代RTP不区分音视频，其内部会处理
    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"RTP Sender", 
        GraphUtil::clsid_scut_rtp_sender, &rtp_sender_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add rtp_sender_filter to graph failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, rtp_sender_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect rtp_sender_filter failed.");

    return DX_ERR_NONE;
}

msdx::dxStatus RTPSender2Container::AddDest( NetAddr addr, RtpCallback *callback )
{
    LOG_INFO("Adding rtp %s dest: %s:%d",
        RtpTypeName(), addr.ip.c_str(), addr.port);

    CComQIPtr<ISCUTRtpSender> sender_config = rtp_sender_filter_;
    if (sender_config) {
        ISCUTRtpSender::ConnectionParam param;
        param.ip_addr = addr.ip.c_str();
        param.port = addr.port;
        param.callback_obj = callback ? callback->obj : nullptr;
        param.callback_func = callback ? callback->func : nullptr;
        param.dscp = 0; //暂时无用，保留

        HRESULT hr = sender_config->AddConnection(param);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
            "Add rtp %s dest failed: 0x%08x", RtpTypeName(), hr);
    } else {
        LOG_ERROR("Add rtp %s dest failed: invalid rtp config interface");
        return DX_ERR_BAD_INTERFACE;
    }
    return DX_ERR_NONE;
}

msdx::dxStatus RTPSender2Container::RemoveDest( NetAddr addr )
{
    LOG_INFO("Removing rtp %s dest: %s:%d",
        RtpTypeName(), addr.ip.c_str(), addr.port);

    CComQIPtr<ISCUTRtpSender> sender_config = rtp_sender_filter_;
    if (sender_config) {
        HRESULT hr = sender_config->RemoveConnection(addr.ip.c_str(), addr.port);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
            "Remove rtp %s dest failed: 0x%08x", RtpTypeName(), hr);
    }
    return DX_ERR_NONE;
}

msdx::dxStatus RTPSender2Container::Clear()
{
    LOG_INFO("Remove all %s sending dest.", RtpTypeName());
    CComQIPtr<ISCUTRtpSender> sender_config = rtp_sender_filter_;
    if (sender_config) {
        sender_config->RemoveAllConnection();
    }
    return DX_ERR_NONE;
}

msdx::dxStatus RTPSender2Container::PauseOverallSending( bool stop )
{
    LOG_INFO("%s sending %s data.", stop ? "Pause" : "Resume", RtpTypeName());
    CComQIPtr<ISCUTRtpSender> sender_config = rtp_sender_filter_;
    if (sender_config) {
        sender_config->ControlAllSending(!stop);
    }
    return DX_ERR_NONE;
}

unsigned long RTPSender2Container::GetCount()
{
    CComQIPtr<ISCUTRtpSender> sender_config = rtp_sender_filter_;
    if (sender_config) {
        return sender_config->ConnectionCount();
    }
    return 0;
}

char * RTPSender2Container::RtpTypeName()
{
    switch (rtp_type_)
    {
    case msdx::RTPSenderContainer::RTP_TYPE_NONE:
        return "None";
        break;
    case msdx::RTPSenderContainer::RTP_TYPE_VIDEO:
        return "Video";
        break;
    case msdx::RTPSenderContainer::RTP_TYPE_AUDIO:
        return "Audio";
        break;
    default:
        break;
    }
    return "Unknown";
}


// RTPReceiver2Container

RTPReceiver2Container::~RTPReceiver2Container()
{
    if (graph_builder_ && rtp_receiver_filter_)
    {
        graph_builder_->RemoveFilter(rtp_receiver_filter_);
    }
}

msdx::dxStatus RTPReceiver2Container::AddFilter( IGraphBuilder *owner_graph, 
                                                CMediaType media_type, 
                                                const char *rtp_id )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Cannot add rtp receiver to null graph.");
    graph_builder_ = owner_graph;
    media_type_ = media_type;
    std::wstring filter_name;

    //check input
    if (media_type_.majortype == MEDIATYPE_Video) {
        filter_name = L"Video Receiver";
    }
    else if (media_type_.majortype == MEDIATYPE_Audio) {
        filter_name = L"Audio Receiver";
    }
    else {
        LOG_ERROR("Add rtp receiver for unknown media type.");
        return DX_ERR_BAD_PARAM;
    }

    //add filter
    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, filter_name.c_str(), 
        GraphUtil::clsid_scut_rtp_receiver, &rtp_receiver_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Add rtp %s receiver filter failed: 0x%08x", RtpTypeName(), hr);

    //set media type
    CComQIPtr<ISCUTRtpReceiver> receiver_config = rtp_receiver_filter_;
    if (receiver_config)
    {
        receiver_config->SetId(rtp_id);
        hr = receiver_config->SetMediaType(media_type_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Set rtp %s receiver media type failed: 0x%08x", RtpTypeName(), hr);
    }
    else
    {
        LOG_ERROR("Query interface of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    return DX_ERR_NONE;
}

msdx::dxStatus RTPReceiver2Container::SetSource( NetAddr addr )
{
    CComQIPtr<ISCUTRtpReceiver> receiver_config = rtp_receiver_filter_;
    if (!receiver_config)
    {
        LOG_ERROR("Query interface of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    HRESULT hr = receiver_config->SetLocalAddr(addr.ip.c_str(), addr.port);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Set rtp %s local addr failed: 0x%08x", RtpTypeName(), hr);

    LOG_INFO("Set %s receiver local addr: ip:%s, port:%d",
        media_type_.majortype == MEDIATYPE_Video ? "video" : "audio",
        addr.ip.c_str(), addr.port);

    return DX_ERR_NONE;
}

msdx::dxStatus RTPReceiver2Container::SetCallback( RtpCallback *callback )
{
    CComQIPtr<ISCUTRtpReceiver> receiver_config = rtp_receiver_filter_;
    if (!receiver_config)
    {
        LOG_ERROR("Query interface of rtp receiver failed.");
        return DX_ERR_ABORT;
    }

    if (callback)
    {
        receiver_config->SetCallback(callback->obj, callback->func);
    }
    else
    {
        receiver_config->SetCallback(nullptr, nullptr);
    }
    return DX_ERR_NONE;
}

msdx::dxStatus RTPReceiver2Container::StopReceiving( bool stop )
{
    CHK_FAIL_MSG_RETURN(!rtp_receiver_filter_, DX_ERR_NULL_PTR, "Cannot stop null rtp receiver.");

    LOG_INFO("%s receiving %s data", 
        (stop ? "Stop" : "Resume"), RtpTypeName());

    CComQIPtr<ISCUTRtpReceiver> receiver_config = rtp_receiver_filter_;
    if (receiver_config) {
        receiver_config->ControlRecv(!stop);
    } else {
        LOG_ERROR("Query interface of rtp receiver failed");
        return DX_ERR_BAD_INTERFACE;
    }
    return DX_ERR_NONE;
}

IPin * RTPReceiver2Container::GetOutputPin()
{
    //msdx模块约定，通过返回值返回COM接口时不增加引用计数，故这里COM指针会释放一个计数
    CComPtr<IPin> out_pin;
    if (rtp_receiver_filter_)
    {
        HRESULT hr = GraphUtil::GetUnconnectedPin(rtp_receiver_filter_, PINDIR_OUTPUT, &out_pin);
        if (FAILED(hr))
        {
            LOG_ERROR("Get rtp receiver output pin failed: 0x%08x", hr);
        }
    }
    return out_pin;
}

char * RTPReceiver2Container::RtpTypeName()
{
    if (media_type_.majortype == MEDIATYPE_Video) {
        return "Video";
    }
    if (media_type_.majortype == MEDIATYPE_Audio) {
        return "Audio";
    }
    return "Unknown";
}

