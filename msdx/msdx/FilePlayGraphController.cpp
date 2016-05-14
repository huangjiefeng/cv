#include "FilePlayGraphController.h"

#include "InterfaceDefination.h"
#include "Util.h"
#include "LogUtil.h"
#include "msdx/config.h"

using namespace msdx;

#define PLAY_STAT_TIMER_INTERVAL 1000

std::map<UINT_PTR, FilePlayGraphController*> timer_owner_table;


FilePlayGraphController::FilePlayGraphController(void)
    : graph_(nullptr)
    , file_source_(nullptr)
    , video_rtp_sender_(nullptr)
    , audio_rtp_sender_(nullptr)
    , callback_timer_id_(0)
{
    Reset();
}


FilePlayGraphController::~FilePlayGraphController(void)
{
    Reset();
}

void FilePlayGraphController::Reset()
{
    Stop();
    graph_status_ = DX_GRAPH_STATUS_UNKNOWN;

    SAFE_DELETE_PTR(file_source_);
    demux_filter_ = nullptr;
    play_ctrl_filter_ = nullptr;
    SAFE_DELETE_PTR(audio_rtp_sender_);
    SAFE_DELETE_PTR(video_rtp_sender_);
    SAFE_DELETE_PTR(graph_);

    file_play_callback_.obj = nullptr;
    file_play_callback_.func = nullptr;
}

dxStatus FilePlayGraphController::CreateGraph()
{
    graph_ = new GraphContainer;
    CHK_FAIL_MSG_RETURN(!graph_, DX_ERR_MEMORY_ALLOC, "Create graph container failed");

    dxStatus status = graph_->InitializeGraph();
    CHK_FAIL_RETURN(!graph_, DX_ERR_MEMORY_ALLOC);

    graph_->AddToRot(L"FilePlay");
    return DX_ERR_NONE;
}

dxGraphStatus FilePlayGraphController::GetStatus()
{
    return graph_status_;
}

dxStatus FilePlayGraphController::Build(FilePlayGraphParam & param)
{
    graph_param_ = param;
    dxStatus status = DX_ERR_NONE;
    HRESULT hr = NOERROR;

    LOG_INFO("Building graph");

    //创建graph builder
    status = CreateGraph();
    CHK_FAIL_RETURN(STATUS_ERR(status), status);
    IGraphBuilder *graph_builder = graph_->GetGraphBuilder();

    //file source
    file_source_ = new FileSourceContainer;
    if (!file_source_) return DX_ERR_MEMORY_ALLOC;

    CHK_FAIL_MSG_RETURN(graph_param_.file_list.size() == 0, DX_ERR_BAD_PARAM,
      "Empty file list");

    status = file_source_->AddFilter(graph_builder, graph_param_.file_list[0]);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    //mpeg-4 demuxer
    hr = GraphUtil::AddFilterByCLSID(graph_builder, L"demuxer", 
        GraphUtil::clsid_gdcl_mp4_demuxer, &demux_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Add demuxer failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder, file_source_->GetFilter(), demux_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Connect demuxer to file source failed: 0x%08x", hr);

    //play ctrl
    hr = GraphUtil::AddFilterByCLSID(graph_builder, L"play ctrl", 
        GraphUtil::clsid_scut_play_ctrl, &play_ctrl_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Add file play failed: 0x%08x", hr);

    CComPtr<IPin> demux_out_pin;
    while (SUCCEEDED(GraphUtil::GetUnconnectedPin(demux_filter_, PINDIR_OUTPUT, &demux_out_pin)))
    {
        //将所有demux输出pin都与play ctrl连接，有几个连接几个
        if (!demux_out_pin) continue; //理论上这不可能发生

        hr = GraphUtil::ConnectFilters(graph_builder, demux_out_pin, play_ctrl_filter_);
        if (FAILED(hr)) { 
            LOG_WARN("Connect a demuxer pin to play ctrl pin failed: 0x%08x", hr);
        } else {
            //每连接一个play ctrl的输入pin，其会产生一个新的对应的输出pin，它们有相同的格式
            //即如果连接的是demux的视频pin，则play ctrl新产生的输出pin也是该视频格式的，
            //从而，只需要检查其最后连接的输入pin的媒体格式，就能获知最后产生的输出pin的媒体格式

            CMediaType connected_type;
            hr = demux_out_pin->ConnectionMediaType(&connected_type);

            if (SUCCEEDED(hr)) {
                if (connected_type.majortype == MEDIATYPE_Video) {
                    //play ctrl起始时没有输出pin，从上面的注释可知，此时play ctrl有且只有一个
                    //未连接的输出pin，因此可确保AddVideoLink中不会连错pin
                    AddVideoLink();
                } else if (connected_type.majortype == MEDIATYPE_Audio) {
                    AddAudioLink();
                } else {
                    OLECHAR* bstrGuid;
                    StringFromCLSID(connected_type.majortype, &bstrGuid);
                    LOG_WARN(L"Unsupported demux output type: %s", bstrGuid);
                    ::CoTaskMemFree(bstrGuid);
                }
            } else {
                LOG_WARN("Get demux out pin connecting type failed: 0x%08x", hr);
            }
        }

        demux_out_pin = nullptr; //COM指针需要释放后才能被二次使用
    }

    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    CHK_FAIL_MSG_RETURN(!play_ctrl, DX_ERR_ABORT, 
        "Set initial delay failed: bad SCUTFilePlayCtrl filter");
    hr = play_ctrl->SetSyncId(graph_param_.sync_id.c_str());
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Set sync id failed: 0x%08x", hr);
    hr = play_ctrl->SetInitialTsOffset(graph_param_.initial_delay);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
        "Set initial delay failed: 0x%08x", hr);

    ReportMediaFormat();

    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::AddVideoLink()
{
    CHK_FAIL_MSG_RETURN(!play_ctrl_filter_, DX_ERR_ABORT, 
        "Try to add video link to null play ctrl");

    if (video_rtp_sender_) {
        //已存在，说明有相同的track已经连接过了，忽略
        //这在目前设计时是不应该发生的，因为只支持音视频各一个track的文件
        LOG_WARN("More than one video track found, ignore.");
        return DX_WARN_OP_IGNORED;
    }

    CComPtr<IGraphBuilder> graph_builder = graph_->GetGraphBuilder();
    dxStatus status;

    //add filter
    video_rtp_sender_ = new RTPSender2Container;
    if (!video_rtp_sender_) return DX_ERR_MEMORY_ALLOC;

    CComPtr<IPin> video_output_pin;
    GraphUtil::GetUnconnectedPin(play_ctrl_filter_, PINDIR_OUTPUT, &video_output_pin);
    CHK_FAIL_MSG_RETURN(!video_output_pin, DX_ERR_ABORT, "Cannot get play ctrl video output pin.");

    status = video_rtp_sender_->AddToGraph(graph_builder, 
        RTPSenderContainer::RTP_TYPE_VIDEO, video_output_pin);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    //add dest
    status = video_rtp_sender_->AddDest(graph_param_.video.addr, nullptr);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::AddAudioLink()
{
    CHK_FAIL_MSG_RETURN(!play_ctrl_filter_, DX_ERR_ABORT, 
        "Try to add audio link to null play ctrl");

    if (audio_rtp_sender_) {
        //已存在，说明有相同的track已经连接过了，忽略
        //这在目前设计时是不应该发生的，因为只支持音视频各一个track的文件
        LOG_WARN("More than one audio track found, ignore.");
        return DX_WARN_OP_IGNORED;
    }

    CComPtr<IGraphBuilder> graph_builder = graph_->GetGraphBuilder();
    dxStatus status;

    //add filter
    audio_rtp_sender_ = new RTPSender2Container;
    if (!audio_rtp_sender_) return DX_ERR_MEMORY_ALLOC;

    CComPtr<IPin> audio_output_pin;
    GraphUtil::GetUnconnectedPin(play_ctrl_filter_, PINDIR_OUTPUT, &audio_output_pin);
    CHK_FAIL_MSG_RETURN(!audio_output_pin, DX_ERR_ABORT, "Cannot get play ctrl audio output pin.");

    status = audio_rtp_sender_->AddToGraph(graph_builder, 
        RTPSenderContainer::RTP_TYPE_AUDIO, audio_output_pin);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    //add dest
    status = audio_rtp_sender_->AddDest(graph_param_.audio.addr, nullptr);
    CHK_FAIL_RETURN(STATUS_ERR(status), status);

    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::Run()
{
    CHK_FAIL_MSG_RETURN((!graph_ || !graph_->GetMediaControl()), 
        DX_ERR_ABORT, "Run graph failed: graph not initialized.");
    CHK_FAIL_MSG_RETURN(DX_GRAPH_STATUS_RUNNING == graph_status_, DX_ERR_NONE, "Graph already running.");

    HRESULT hr = graph_->GetMediaControl()->Run();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Run graph failed: 0x%08x", hr);

    //set timer to report play status
    //注意：timer只有在创建timer的线程存在消息循环时才生效哦
    callback_timer_id_ = SetTimer(NULL, 0, PLAY_STAT_TIMER_INTERVAL, CallbackTimerProc);
    if (callback_timer_id_) timer_owner_table[callback_timer_id_] = this;

    LOG_INFO("Graph started, callback_timer_id=%d", callback_timer_id_);
    graph_status_ = DX_GRAPH_STATUS_RUNNING;
    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::Stop()
{
    if (!graph_ || !graph_->GetMediaControl()) return DX_ERR_NONE;

    if (callback_timer_id_) KillTimer(NULL, callback_timer_id_);
    timer_owner_table.erase(callback_timer_id_);

    HRESULT hr = graph_->GetMediaControl()->Stop();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Stop graph failed: 0x%08x", hr);

    LOG_INFO("Graph stopped");
    graph_status_ = DX_GRAPH_STATUS_STOPPED;
    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::Pause(unsigned long op_id)
{
    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    CHK_FAIL_MSG_RETURN(!play_ctrl, DX_ERR_ABORT, 
        "Pause play failed: bad SCUTFilePlayCtrl filter");
    HRESULT hr = play_ctrl->Operate(op_id, ISCUTFilePlayCtrl::kOpPause, nullptr);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Pause play failed");
    return DX_ERR_NONE;
}

dxStatus FilePlayGraphController::Resume(unsigned long op_id)
{
    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    CHK_FAIL_MSG_RETURN(!play_ctrl, DX_ERR_ABORT, 
        "Resume play failed: bad SCUTFilePlayCtrl filter");
    HRESULT hr = play_ctrl->Operate(op_id, ISCUTFilePlayCtrl::kOpResume, nullptr);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Resume play failed");
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::FilePlayGraphController::SeekTo( unsigned long op_id, long long t )
{
    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    CHK_FAIL_MSG_RETURN(!play_ctrl, DX_ERR_ABORT, 
        "Seek to %d failed: bad SCUTFilePlayCtrl filter", t);

    ISCUTFilePlayCtrl::PlayOpParam param;
    param.seek.t = t;
    HRESULT hr = play_ctrl->Operate(op_id, ISCUTFilePlayCtrl::kOpSeek, &param);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Seek to %d failed", t);

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::FilePlayGraphController::SetFilePlayStatCallBack( 
  void *obj, FilePlayStatCallback func )
{
    file_play_callback_.obj = obj;
    file_play_callback_.func = func;
    return DX_ERR_NONE;
}

void msdx::FilePlayGraphController::ReportMediaFormat()
{
    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    if (!play_ctrl) {
        LOG_ERROR("Report format error: query IFilePlayCtrl failed");
    }

    if (file_play_callback_.func) {
        ISCUTFilePlayCtrl::MediaFormat fmt;
        HRESULT hr = play_ctrl->GetMediaFormat(fmt);
        if (FAILED(hr)) {
          LOG_ERROR("Get media file format error: 0x%08x", hr);
          return;
        }
        ISCUTFilePlayCtrl::ProgressInfo progress;
        hr = play_ctrl->GetProgress(progress);
        if (FAILED(hr)) {
            LOG_ERROR("Get play progress error: 0x%08x", hr);
            return;
        }

        FilePlayStatItem item = {0};
        //item.id由StreamMedia设置
        item.type = FILEPLAY_STAT_MEDIAINFO;
        if (fmt.video_codec == ISCUTFilePlayCtrl::H264) 
          strcpy_s(item.format.video_codec, sizeof(item.format.video_codec), 
                   MSDX_CONF_VIDEO_CODEC_H264);
        item.format.video_width = fmt.video_width;
        item.format.video_height = fmt.video_height;
        item.format.video_fps = fmt.video_fps;
        if (fmt.audio_codec == ISCUTFilePlayCtrl::AAC_LC) 
          strcpy_s(item.format.audio_codec, sizeof(item.format.audio_codec), 
                   MSDX_CONF_AUDIO_CODEC_AAC);
        item.format.audio_sample_rate = fmt.audio_sample_rate;
        item.format.audio_channel = fmt.audio_channel;
        item.format.audio_bit_count = fmt.audio_bit_count;
        item.format.file_duration = progress.duration / 10000;

        (file_play_callback_.func)(file_play_callback_.obj, item);
        LOG_PRINTF("Report file format: \n\tvideo: %dx%d@%dfps %s \n\taudio: %dHz %dch %s"
            "\n\tduration: %I64dms",
          fmt.video_width, fmt.video_height, fmt.video_fps, item.format.video_codec, 
          fmt.audio_sample_rate, fmt.audio_channel, item.format.audio_codec,
          item.format.file_duration);
    } else {
        LOG_INFO("File format not reported since no callback set");
    }
}

void msdx::FilePlayGraphController::ReportPlayStatus()
{
    static long long last_position = -1;
    CComQIPtr<ISCUTFilePlayCtrl> play_ctrl = play_ctrl_filter_;
    if (!play_ctrl) {
        return;
    }

    if (file_play_callback_.func) {
        FilePlayStatItem item = {0};
        //item.id由StreamMedia设置
        item.type = FILEPLAY_STAT_PROGRESS;

        ISCUTFilePlayCtrl::ProgressInfo progress;
        HRESULT hr = play_ctrl->GetProgress(progress);
        if (FAILED(hr)) {
            LOG_ERROR("Get play progress error: 0x%08x", hr);
            return;
        }
        if (progress.position == last_position) return; //进度无变化不报告
        last_position = progress.position;


        printf_s("[msdx] %I64d/%I64d, sts=%d\r", 
            progress.position / 10000000, progress.duration / 10000000, progress.status);
        item.progress.current_pos = progress.position / 10000; // 100ns to ms
        item.progress.duration = progress.duration / 10000;
        (file_play_callback_.func)(file_play_callback_.obj, item);
    }
}

VOID CALLBACK msdx::FilePlayGraphController::CallbackTimerProc( 
    HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
    FilePlayGraphController *owner = timer_owner_table[idEvent];
    if (owner) owner->ReportPlayStatus();
    //LOG_PRINTF("%x, %d, %d", hwnd, uMsg, idEvent);
}
