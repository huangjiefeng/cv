#include "RecvGraphController.h"
#include "LogUtil.h"
#include "GraphUtil.h"
#include "Util.h"
#include "MediaTypeUtil.h"
#include "3rd/faac/aac_types.h" //MEDIASUBTYPE_AAC
#include "InterfaceDefination.h" //MEDIASUBTYPE_SPEEX

using namespace msdx;

RecvGraphController::RecvGraphController(void)
    : graph_(nullptr)
    , video_rtp_receiver_(nullptr)
    , video_decoder_(nullptr)
    , video_renderer_(nullptr)
    , audio_rtp_receiver_(nullptr)
    , audio_decoder_(nullptr)
    , audio_renderer_(nullptr)
    , record_audio_encoder_(nullptr)
{
    Reset();
}


RecvGraphController::~RecvGraphController(void)
{
    Reset();
}

void msdx::RecvGraphController::Reset()
{
    Stop();
    graph_status_ = DX_GRAPH_STATUS_UNKNOWN;

    record_mux_writer_ = nullptr;
    SAFE_DELETE_PTR(video_renderer_);
    SAFE_DELETE_PTR(video_decoder_);
    SAFE_DELETE_PTR(video_rtp_receiver_);
    SAFE_DELETE_PTR(audio_renderer_);
    SAFE_DELETE_PTR(record_audio_encoder_);
    SAFE_DELETE_PTR(audio_decoder_);
    SAFE_DELETE_PTR(audio_rtp_receiver_);
    SAFE_DELETE_PTR(graph_);

    video_rtp_callback_.obj = nullptr;
    video_rtp_callback_.func = nullptr;
    audio_rtp_callback_.obj = nullptr;
    audio_rtp_callback_.func = nullptr;
    record_muxer_callback.obj = nullptr;
    record_muxer_callback.func = nullptr;
}

msdx::dxStatus msdx::RecvGraphController::CreateGraph()
{
    graph_ = new GraphContainer;
    CHK_FAIL_MSG_RETURN(!graph_, DX_ERR_MEMORY_ALLOC, "Create graph container failed");

    dxStatus status = graph_->InitializeGraph();
    CHK_FAIL_RETURN(!graph_, DX_ERR_MEMORY_ALLOC);

    std::wstring graph_name = Util::str2wstr(graph_param_.graph_name);
    graph_->AddToRot(graph_name.c_str());
    return DX_ERR_NONE;
}

msdx::dxGraphStatus msdx::RecvGraphController::GetStatus()
{
    return graph_status_;
}

dxStatus RecvGraphController::Build(RecvGraphParam &param)
{
    graph_param_ = param;
    dxStatus status = DX_ERR_NONE;

    LOG_INFO("Building graph");

    //创建graph builder
    status = CreateGraph();
    CHK_FAIL_RETURN(STATUS_ERR(status), status);
    IGraphBuilder *graph_builder = graph_->GetGraphBuilder();

    //video
    if (graph_param_.video.addr.port != 0)
    {
        //rtp
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

            status = video_rtp_receiver_->AddFilter(graph_builder, media_type, 
                graph_param_.graph_name.c_str());
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            status = video_rtp_receiver_->SetSource(graph_param_.video.addr);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            status = video_rtp_receiver_->SetCallback(&video_rtp_callback_);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);
        }
        
        if (!graph_param_.used_as_record)
        {
            //非录像时解码播放
            //codec
            if (Util::CheckIntelHwCodecSupport())
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

            //renderer
            video_renderer_ = new AutoVideoRendererContainer;
            if (!video_renderer_) return DX_ERR_MEMORY_ALLOC;

            {
                CComPtr<IPin> decoder_output_pin;
                GraphUtil::GetUnconnectedPin(video_decoder_->GetFilter(), 
                    PINDIR_OUTPUT, &decoder_output_pin);
                CHK_FAIL_MSG_RETURN(!decoder_output_pin, DX_ERR_ABORT, "Cannot get video decoder output pin.");

                status = video_renderer_->AddToGraph(graph_builder, decoder_output_pin, true);
                CHK_FAIL_RETURN(STATUS_ERR(status), status);
            }

            if (graph_param_.video.preview.window_handle != 0)
            {
                status = video_renderer_->SetPreviewWindow(graph_param_.video.preview);
                CHK_FAIL_RETURN(STATUS_ERR(status), status);
            }
        }
    }

    //audio
    if (graph_param_.audio.addr.port != 0)
    {
        //rtp
        audio_rtp_receiver_ = new RTPReceiver2Container;
        if (!audio_rtp_receiver_) return DX_ERR_MEMORY_ALLOC;

        {
            MediaTypeUtil::AudioTypeParam get_audio_type_param;
            if (graph_param_.audio.codec == DX_AUDIO_CODEC_AAC)
                get_audio_type_param.media_subtype = MEDIASUBTYPE_AAC;
            else
                get_audio_type_param.media_subtype = MEDIASUBTYPE_SPEEX;
            get_audio_type_param.sample_rate = graph_param_.audio.sample_rate;
            get_audio_type_param.channels = graph_param_.audio.channels;
            get_audio_type_param.bits_per_sample = graph_param_.audio.bitcount;

            CMediaType media_type;
            status = MediaTypeUtil::GetAudioType(get_audio_type_param, media_type);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            status = audio_rtp_receiver_->AddFilter(graph_builder, media_type, 
                graph_param_.graph_name.c_str());
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            status = audio_rtp_receiver_->SetSource(graph_param_.audio.addr);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            status = audio_rtp_receiver_->SetCallback(&audio_rtp_callback_);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);
        }

        if (!graph_param_.used_as_record && !graph_param_.audio.output_device.empty())
        {
            //非录像时解码播放
            //codec
            AddAudioDecoder();

            //renderer
            {
                audio_renderer_ = new AutoAudioRendererContainer;
                if (!audio_renderer_) return DX_ERR_MEMORY_ALLOC;

                CComPtr<IPin> pin;
                GraphUtil::GetUnconnectedPin(audio_decoder_->GetFilter(), PINDIR_OUTPUT, &pin);

                status = audio_renderer_->AddFilter(graph_builder, pin, graph_param_.audio.output_device);
                CHK_FAIL_RETURN(STATUS_ERR(status), status);
            }
        }
    }

    //record
    if (graph_param_.used_as_record)
    {
        status = AddRecord();
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    graph_status_ = DX_GRAPH_STATUS_INITIALIZED;
    return DX_ERR_NONE;
}

dxStatus RecvGraphController::AddAudioDecoder()
{
    if (!audio_rtp_receiver_ || !audio_rtp_receiver_->GetFilter())
        return DX_ERR_NULL_PTR;

    if (graph_param_.audio.codec == DX_AUDIO_CODEC_AAC) {
        audio_decoder_ = new AACDecoderContainer;
    }
    else {
        audio_decoder_ = new SpeexDecoderContainer;
    }
    if (!audio_decoder_) return DX_ERR_NULL_PTR;

    {
        dxStatus status = audio_decoder_->AddFilter(graph_->GetGraphBuilder(), 
            audio_rtp_receiver_->GetOutputPin(),
            audio_rtp_receiver_->GetType());
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    return DX_ERR_NONE;
}

dxStatus RecvGraphController::AddRecord()
{
    CComPtr<IPin> video_out_pin;
    CComPtr<IPin> audio_out_pin;
    HRESULT hr;
    dxStatus status;
    IGraphBuilder *graph_builder = graph_->GetGraphBuilder();

    if (graph_param_.record.file_path_name.empty()) {
        LOG_ERROR("Need to specify record file name.");
        return DX_ERR_BAD_PARAM;
    }

    if (video_rtp_receiver_ && video_rtp_receiver_->GetFilter()) {
        video_out_pin = video_rtp_receiver_->GetOutputPin();
        CHK_FAIL_MSG_RETURN(!video_out_pin, DX_ERR_NULL_PTR, 
            "No unconnected video rtp pin for rec.");
    }

    if (audio_rtp_receiver_ && audio_rtp_receiver_->GetFilter()) {
        if (graph_param_.audio.codec == DX_AUDIO_CODEC_SPEEX) {
            status = AddAudioDecoder();
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            //本来应该从speex dec输出pin获取类型，但是太麻烦了，这样一样的
            //其实aac现在只检查subtype是PCM就行
            MediaTypeUtil::AudioTypeParam param;
            param.media_subtype = MEDIASUBTYPE_PCM;
            param.sample_rate = graph_param_.audio.sample_rate;
            param.channels = graph_param_.audio.channels;
            param.bits_per_sample = graph_param_.audio.bitcount;

            CMediaType type;
            MediaTypeUtil::GetAudioType(param, type);

            //get speex dec output pin
            CComPtr<IPin> speex_pin;
            GraphUtil::GetUnconnectedPin(audio_decoder_->GetFilter(),
                PINDIR_OUTPUT, &speex_pin);

            record_audio_encoder_ = new AACEncoderContainer;
            status = record_audio_encoder_->AddFilter(graph_builder, speex_pin, type);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            hr = GraphUtil::GetUnconnectedPin(record_audio_encoder_->GetFilter(),
                PINDIR_OUTPUT, &audio_out_pin);
            CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
                "No unconnected speex_dec pin for rec.");
        }
        else {
            audio_out_pin = audio_rtp_receiver_->GetOutputPin();
            CHK_FAIL_MSG_RETURN(!audio_out_pin, DX_ERR_NULL_PTR, 
                "No unconnected audio rtp pin for rec.");
        }
    }

    hr = GraphUtil::AddFilterByCLSID(graph_builder, L"Muxer", 
        GraphUtil::clsid_gdcl_mp4_muxer, &record_mux_writer_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Add muxer failed: 0x%08x", hr);

    {
        CComQIPtr<ISCUTMpeg4Mux> muxer_config = record_mux_writer_;
        CHK_FAIL_MSG_RETURN(!muxer_config, DX_ERR_BAD_INTERFACE, "Query muxer ISCUTMpeg4Mux failed.");

        hr = muxer_config->SetFileName(graph_param_.record.file_path_name.c_str());
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Set muxer file path failed: 0x%08x", hr);

        muxer_config->SetInitialJobId(graph_param_.record.job_id);
        muxer_config->SetCallback(record_muxer_callback.obj, record_muxer_callback.func);
    }
    
    if (video_out_pin) {
        hr = GraphUtil::ConnectFilters(graph_builder, video_out_pin, record_mux_writer_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Connect muxer to video pin failed: 0x%08x", hr);
    }
    if (audio_out_pin) {
        hr = GraphUtil::ConnectFilters(graph_builder, audio_out_pin, record_mux_writer_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Connect muxer to audio pin failed: 0x%08x", hr);
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::Run()
{
    CHK_FAIL_MSG_RETURN((!graph_ || !graph_->GetMediaControl()), 
        DX_ERR_ABORT, "Run graph failed: graph not initialized.");
    CHK_FAIL_MSG_RETURN(DX_GRAPH_STATUS_RUNNING == graph_status_, DX_ERR_NONE, "Graph already running.");

    IMediaControl *media_control = graph_->GetMediaControl();
    //接收端不使用时钟，我们有自己的同步机制
    //rtp接收端都已改成不用参考时钟模式，若不取消参考时钟，视频会很卡，或旧RTP会buffer full哦
    CComQIPtr<IMediaFilter> media_filter = media_control;
    if (media_filter)
    {
        media_filter->SetSyncSource(NULL);
    }

    HRESULT hr = media_control->Run();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Run graph failed: 0x%08x", hr);

    LOG_INFO("Graph started");
    graph_status_ = DX_GRAPH_STATUS_RUNNING;
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::Stop()
{
    if (!graph_ || !graph_->GetMediaControl() || graph_status_ == DX_GRAPH_STATUS_STOPPED) 
      return DX_ERR_NONE;

    StopReceivingVideo(true);
    StopReceivingAudio(true);

    HRESULT hr = graph_->GetMediaControl()->Stop();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Stop graph failed: 0x%08x", hr);

    LOG_INFO("Graph stopped");
    graph_status_ = DX_GRAPH_STATUS_STOPPED;
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::StopReceivingVideo( bool stop )
{
    if (video_rtp_receiver_)
    {
        dxStatus status = video_rtp_receiver_->StopReceiving(stop);
        CHK_FAIL_MSG_RETURN(STATUS_ERR(status), DX_ERR_ABORT, 
            "Stop or resume video receiver failed.");
    }
    else
    {
        LOG_WARN("Stop null video receiver.");
    }
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::StopReceivingAudio( bool stop )
{
    if (audio_rtp_receiver_)
    {
        dxStatus status = audio_rtp_receiver_->StopReceiving(stop);
        CHK_FAIL_MSG_RETURN(STATUS_ERR(status), DX_ERR_ABORT, 
            "Stop or resume audio receiver failed.");
    }
    else
    {
        LOG_WARN("Stop null audio receiver.");
    }
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::ChangeVideoWindow( VideoWindow window )
{
    CHK_FAIL_MSG_RETURN(!video_renderer_, DX_ERR_NULL_PTR, "Change preview failed: null renderer");

    graph_param_.video.preview = window; //保存参数
    dxStatus status = video_renderer_->SetPreviewWindow(graph_param_.video.preview);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::StartRecordingNewFile( long job_id, std::string new_file_name )
{
    LOG_PRINTF("Request recording new file: %s", new_file_name.c_str());
    CComQIPtr<ISCUTMpeg4Mux> pMux = record_mux_writer_;
    if (pMux) {
        pMux->CutMovieFile(job_id, new_file_name.c_str());
    } else {
        LOG_ERROR("Cannot record to new file: no ISCUTMpeg4Mux or muxer not added.");
        return DX_ERR_BAD_INTERFACE;
    }
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::SetRecordStatCallBack( void *obj, RecordStatCallback func )
{
    record_muxer_callback.obj = obj;
    record_muxer_callback.func = func;

    //是否需要动态变更
    CComQIPtr<ISCUTMpeg4Mux> muxer_config = record_mux_writer_;
    if (muxer_config) {
        muxer_config->SetCallback(obj, func);
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::SetVideoRtpStatCallBack( void *obj, RtpStatCallback func )
{
    video_rtp_callback_.obj = obj;
    video_rtp_callback_.func = func;

    //是否需要动态变更
    if (video_rtp_receiver_) {
        video_rtp_receiver_->SetCallback(&video_rtp_callback_);
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::RecvGraphController::SetAudioRtpStatCallBack( void *obj, RtpStatCallback func )
{
    audio_rtp_callback_.obj = obj;
    audio_rtp_callback_.func = func;

    //是否需要动态变更
    if (audio_rtp_receiver_) {
        audio_rtp_receiver_->SetCallback(&audio_rtp_callback_);
    }

    return DX_ERR_NONE;
}
