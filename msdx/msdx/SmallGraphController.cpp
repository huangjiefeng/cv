#include "SmallGraphController.h"
#include "LogUtil.h"
#include "GraphUtil.h"
#include "Util.h"
#include "MediaTypeUtil.h"
#include "msdx/config.h" //小流画面大小

using namespace msdx;

SmallGraphController::SmallGraphController(void)
    : graph_(nullptr)
    , video_rtp_receiver_(nullptr)
    , video_decoder_(nullptr)
    , video_encoder_(nullptr)
    , video_rtp_sender_(nullptr)
{
    Reset();
}


SmallGraphController::~SmallGraphController(void)
{
    Reset();
}

msdx::dxGraphStatus msdx::SmallGraphController::GetStatus()
{
    return graph_status_;
}

void SmallGraphController::Reset()
{
    Stop();
    graph_status_ = DX_GRAPH_STATUS_UNKNOWN;

    SAFE_DELETE_PTR(video_rtp_receiver_);
    SAFE_DELETE_PTR(video_decoder_);
    SAFE_DELETE_PTR(video_encoder_);
    SAFE_DELETE_PTR(video_rtp_sender_);
    SAFE_DELETE_PTR(graph_);
}

dxStatus SmallGraphController::Build(SmallGraphParam &param)
{
    graph_param_ = param;
    dxStatus status = DX_ERR_NONE;

    LOG_INFO("Building graph");

    //创建graph builder
    status = CreateGraph();
    CHK_FAIL_RETURN(STATUS_ERR(status), status);
    IGraphBuilder *graph_builder = graph_->GetGraphBuilder();

    //rtp receiver
    video_rtp_receiver_ = new RTPReceiver2Container;
    if (!video_rtp_receiver_) return DX_ERR_MEMORY_ALLOC;

    {
        MediaTypeUtil::VideoTypeParam get_video_type_param;
        get_video_type_param.media_subtype = MEDIASUBTYPE_H264;
        get_video_type_param.width = graph_param_.video.frame_width;
        get_video_type_param.height = graph_param_.video.frame_height;
        get_video_type_param.fps = graph_param_.video.fps;

        CMediaType media_type;
        status = MediaTypeUtil::GetVideoType(get_video_type_param, media_type);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);

        status = video_rtp_receiver_->AddFilter(graph_builder, media_type, "small_recv");
        CHK_FAIL_RETURN(STATUS_ERR(status), status);

        status = video_rtp_receiver_->SetSource(graph_param_.video.addr);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    //video decoder
    if (graph_param_.video.enable_hwcodec && Util::CheckIntelHwCodecSupport())
    {
        //硬编码
        video_decoder_ = new IntelH264DecoderContainer;
    }
    else
    {
        video_decoder_ = new ScutH264DecoderContainer;
    }
    CHK_FAIL_RETURN(!video_decoder_, DX_ERR_MEMORY_ALLOC);

    status = video_decoder_->AddFilter(graph_builder, video_rtp_receiver_->GetOutputPin());
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    //video encoder
    if (graph_param_.video.enable_hwcodec && Util::CheckIntelHwCodecSupport())
    {
        video_encoder_ = new IntelH264EncoderContainer;
    }
    else
    {
        video_encoder_ = new ScutH264EncoderContainer;
    }
    if (!video_encoder_) return DX_ERR_MEMORY_ALLOC;

    {
        CComPtr<IPin> decoder_output_pin;
        GraphUtil::GetUnconnectedPin(video_decoder_->GetFilter(), 
            PINDIR_OUTPUT, &decoder_output_pin);
        CHK_FAIL_MSG_RETURN(!decoder_output_pin, DX_ERR_ABORT, "Cannot get video decoder output pin.");

        CMediaType mt;
        mt.subtype == MEDIASUBTYPE_YUY2; //下面传入mt是为了确定是否转换格式，这项只要不是RGB24就行
        status = video_encoder_->AddToGraph(graph_builder, decoder_output_pin, mt);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);

        VideoEncoderContainer::EncoderParam encoder_param = {0}; //保留默认设置
        encoder_param.width = MSDX_CONF_SMALL_VIDEO_WIDTH;
        encoder_param.height = MSDX_CONF_SMALL_VIDEO_HEIGHT;
        encoder_param.target_kbps = 400;
        status = video_encoder_->Config(encoder_param);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    //rtp sender
    video_rtp_sender_ = new RTPSender2Container;
    if (!video_rtp_sender_) return DX_ERR_MEMORY_ALLOC;

    {  //增加作用于减少局部变量暴漏
        CComPtr<IPin> encoder_output_pin;
        GraphUtil::GetUnconnectedPin(video_encoder_->GetFilter(), 
            PINDIR_OUTPUT, &encoder_output_pin);
        CHK_FAIL_MSG_RETURN(!encoder_output_pin, DX_ERR_ABORT, "Cannot get video encoder output pin.");

        status = video_rtp_sender_->AddToGraph(graph_builder, 
            RTPSenderContainer::RTP_TYPE_VIDEO, encoder_output_pin);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::SmallGraphController::CreateGraph()
{
    graph_ = new GraphContainer;
    CHK_FAIL_MSG_RETURN(!graph_, DX_ERR_MEMORY_ALLOC, "Create graph container failed");

    dxStatus status = graph_->InitializeGraph();
    CHK_FAIL_RETURN(!graph_, DX_ERR_MEMORY_ALLOC);

    graph_->AddToRot(L"Small");
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::SmallGraphController::Run()
{
    CHK_FAIL_MSG_RETURN((!graph_ || !graph_->GetMediaControl()), 
        DX_ERR_ABORT, "Run graph failed: graph not initialized.");
    CHK_FAIL_MSG_RETURN(DX_GRAPH_STATUS_RUNNING == graph_status_, DX_ERR_NONE, "Graph already running.");

    IMediaControl *media_control = graph_->GetMediaControl();
    //rtp接收端都已改成不用参考时钟模式，若不取消参考时钟，旧RTP会buffer full哦
    CComQIPtr<IMediaFilter> pMediaFilter = media_control;
    if (pMediaFilter) {
        pMediaFilter->SetSyncSource(NULL);
    }

    HRESULT hr = media_control->Run();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Run graph failed: 0x%08x", hr);

    LOG_INFO("Graph started");
    graph_status_ = DX_GRAPH_STATUS_RUNNING;
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::SmallGraphController::Stop()
{
    if (!graph_ || !graph_->GetMediaControl()) return DX_ERR_NONE;
    IMediaControl *media_control = graph_->GetMediaControl();

    HRESULT hr = media_control->Stop();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Stop graph failed: 0x%08x", hr);

    LOG_INFO("Graph stopped");
    graph_status_ = DX_GRAPH_STATUS_STOPPED;
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::SmallGraphController::AddVideoDestination(const NetAddr & addr)
{
    if (!video_rtp_sender_)
    {
        LOG_WARN("Add video destination failed: null video rtp sender.");
        return DX_WARN_OP_IGNORED;
    }
    return video_rtp_sender_->AddDest(addr, nullptr);
}

msdx::dxStatus msdx::SmallGraphController::DeleteVideoDestination(const NetAddr & addr)
{
    if (!video_rtp_sender_)
    {
        LOG_WARN("Delete video destination failed: null video rtp sender.");
        return DX_WARN_OP_IGNORED;
    }
    return video_rtp_sender_->RemoveDest(addr);
}

msdx::dxStatus msdx::SmallGraphController::StopSendingVideo( bool stop )
{
    CHK_FAIL_MSG_RETURN(!video_encoder_, DX_ERR_ABORT, "Cannot stop null video encoder.");

    return video_encoder_->PauseEncoding(stop);
}
