#include "TestGraphController.h"
#include "LogUtil.h"
#include "GraphUtil.h"
#include "Util.h"
#include "msdx/config.h"

using namespace msdx;


TestGraphController::TestGraphController(void)
    : graph_(nullptr)
    , video_crossbar_(nullptr)
    , video_inf_pin_tee_(nullptr)
    , video_source_(nullptr)
    , video_renderer_(nullptr)
    , audio_source_(nullptr)
    , audio_file_source_(nullptr)
    , audio_null_renderer_(nullptr)
    , audio_renderer_(nullptr)
{
}


TestGraphController::~TestGraphController(void)
{
    //让COM指针自己去释放内存吧，懒得写了
}


msdx::dxStatus msdx::TestGraphController::Build( TestGraphParam & param )
{
    LOG_INFO("Building graph");

    //init graph
    graph_ = new GraphContainer;
    CHK_FAIL_MSG_RETURN(!graph_, DX_ERR_MEMORY_ALLOC, "Create graph container failed");

    dxStatus status = graph_->InitializeGraph();
    CHK_FAIL_RETURN(!graph_, DX_ERR_MEMORY_ALLOC);

    graph_->AddToRot(L"LocalTest");

    graph_param_ = param;
    IGraphBuilder *graph_builder = graph_->GetGraphBuilder();
    HRESULT hr = S_OK;

    //video
    if (!graph_param_.video.device.empty())
    {
        //video source
        video_source_ = new VideoSourceContainer;
        if (!video_source_) return DX_ERR_MEMORY_ALLOC;

        status = video_source_->AddToGraph(graph_builder, graph_param_.video.device);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);


        status = video_source_->SetFormat(graph_param_.video.frame_width,graph_param_.video.frame_height, 30);
        //CHK_FAIL_RETURN(STATUS_ERR(status), status);

        //crossbar
        if (!graph_param_.video.crossbar.empty())
        {
            video_crossbar_ = new CrossbarContainer;
            if (!video_crossbar_) return DX_ERR_MEMORY_ALLOC;

            if (graph_param_.video.crossbar == MSDX_CONF_CROSSBAR_AUTO)
            {
                status = video_crossbar_->AddFilterAuto(graph_->GetCaptureGraphBuilder2(), 
                    video_source_->GetFilter());
                CHK_FAIL_RETURN(STATUS_ERR(status), status);
            } 
            else
            {
                status = video_crossbar_->AddFilter(graph_builder, graph_param_.video.crossbar);
                CHK_FAIL_RETURN(STATUS_ERR(status), status);

                video_crossbar_->SetPhysicalType(graph_param_.video.crossbar_physical_type);
                video_crossbar_->ConnectToVideoSource(video_source_->GetFilter());
                CHK_FAIL_RETURN(STATUS_ERR(status), status);
            }
        }
		//graph_builder->Render(video_source_->GetOutputPin()); //delete by hjf 15.11.27

        //inf_pin_tee - 有的摄像头格式没有按照renderer要求对齐，显示错乱，加一个inf系统会自动转换
        video_inf_pin_tee_ = new InfPinTeeContainer;
        if (!video_inf_pin_tee_) return DX_ERR_MEMORY_ALLOC;

        status = video_inf_pin_tee_->AddToGraph(graph_builder, video_source_->GetOutputPin(), L"Video Inf");
        CHK_FAIL_RETURN(STATUS_ERR(status), status);

	

        //video renderer
        video_renderer_ = new AutoVideoRendererContainer;
        if (!video_renderer_) return DX_ERR_MEMORY_ALLOC;

        status = video_renderer_->AddToGraph(graph_builder, 
            video_inf_pin_tee_->GetUnconnectedPin(), true);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);

        status = video_renderer_->SetPreviewWindow(graph_param_.video.preview);
        CHK_FAIL_RETURN(STATUS_ERR(status), status);
    }

    //audio
    if (!graph_param_.audio.input_device.empty() || !graph_param_.audio.input_file.empty())
    {
        CComPtr<IPin> audio_out_pin;

        //audio source
        if (!graph_param_.audio.input_device.empty())
        {
            //device source
            audio_source_ = new MicrophoneSourceContainer;
            if (!audio_source_) return DX_ERR_MEMORY_ALLOC;

            status = audio_source_->AddFilter(graph_builder, 
                graph_param_.audio.input_device.c_str(), 
                graph_param_.audio.output_device.c_str());
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            audio_out_pin = audio_source_->GetOutputPin();
        }
        else if (!graph_param_.audio.input_file.empty())
        {
            //file source
            audio_file_source_ = new FileSourceContainer;
            if (!audio_file_source_) return DX_ERR_MEMORY_ALLOC;

            status = audio_file_source_->AddFilter(graph_builder, graph_param_.audio.input_file);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);

            audio_out_pin = audio_file_source_->GetOutputPin();
        }

        //audio renderer
        if (!graph_param_.audio.output_device.empty())
        {
            audio_renderer_ = new AutoAudioRendererContainer;
            if (!audio_renderer_) return DX_ERR_MEMORY_ALLOC;

            status = audio_renderer_->AddFilter(graph_builder, 
                audio_out_pin, graph_param_.audio.output_device);
            CHK_FAIL_RETURN(STATUS_ERR(status), status);
        } 
        else
        {
            //case: 空输出测试，即仅测试音频输入设备，用于罗技C270摄像头麦克风预热
            hr = GraphUtil::AddFilterByCLSID(graph_builder, L"Null Renderer", 
                GraphUtil::clsid_null_renderer, &audio_null_renderer_);
            CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add null renderer failed.(hr=0x%08x)", hr);

            hr = GraphUtil::ConnectFilters(graph_builder, audio_out_pin, audio_null_renderer_);
            CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect null renderer failed.(hr=0x%08x)", hr);
        }
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::TestGraphController::Run()
{
    CHK_FAIL_MSG_RETURN((!graph_ || !graph_->GetMediaControl()), 
        DX_ERR_ABORT, "Run graph failed: graph not initialized.");

    HRESULT hr = graph_->GetMediaControl()->Run();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Run graph failed: 0x%08x", hr);

    LOG_INFO("Graph started");
    return DX_ERR_NONE;
}

msdx::dxStatus msdx::TestGraphController::Stop()
{
    if (!graph_ || !graph_->GetMediaControl()) return DX_ERR_NONE;

    HRESULT hr = graph_->GetMediaControl()->Stop();
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Stop graph failed: 0x%08x", hr);

    LOG_INFO("Graph stopped");
    return DX_ERR_NONE;
}
