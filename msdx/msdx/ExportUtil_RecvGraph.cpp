/********************************************************************
本源代码文件实现与接收Graph相关的API函数。
*********************************************************************/
#include "stdafx.h"

#include <assert.h>
#include <string>

#include "Exportutil.h"
#include "Util.h"
#include "LogUtil.h"
#include "msdx\config.h"
#include "IGraphController.h"


//不用static原因同send_graph_param
msdx::RecvGraphParam recv_graph_param;
static msdx::IRecvGraphController *recv_graph_controller;

//建一个辅助对象来维护上述局部变量，以便完成初始化和退出自动销毁
class RecvGraphControllerHelper {
public:
    RecvGraphControllerHelper()
    {
        SetDefaultParam();
        recv_graph_controller = nullptr;
    }

    ~RecvGraphControllerHelper()
    {
        if (recv_graph_controller)
        {
            delete recv_graph_controller;
            recv_graph_controller = nullptr;
        }
    }

    void SetDefaultParam()
    {
        recv_graph_param.graph_name = "recv";
        recv_graph_param.used_as_record = false;
        recv_graph_param.video.addr.port = 0;
        recv_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264;
        recv_graph_param.video.frame_width = 1280;
        recv_graph_param.video.frame_height = 720;
        recv_graph_param.video.fps = 30;
        recv_graph_param.video.preview.window_handle = 0;
        recv_graph_param.video.preview.position_left = 0;
        recv_graph_param.video.preview.position_top = 0;
        recv_graph_param.video.preview.width = 0;
        recv_graph_param.video.preview.height = 0;
        recv_graph_param.audio.addr.port = 0;
        recv_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_SPEEX;
        recv_graph_param.audio.sample_rate = 16000;
        recv_graph_param.audio.channels = 1;
        recv_graph_param.audio.bitcount = 16;
    }
};

static RecvGraphControllerHelper recv_graph_helper;


// @param user_id 用于区分系统中的多个recv graph
// @return graph_id >= 1, 现在一个进程只有一个recv_graph，始终返回1
int MSDXAPI msdx_create_recv_graph(const char * userId /*= ""*/)
{
	//新的实现方式，仅记录graph标识
    if (userId)
    {
        recv_graph_param.graph_name = userId;
    }
	return 1;
}

int MSDXAPI msdx_pause_recv_graph(int graphid)
{
    //TODO: Not implemented
    return 0;
}

int MSDXAPI msdx_resume_recv_graph(int graphid)
{
    //TODO: Not implemented
    return 0;
}

int MSDXAPI msdx_stop_recv_graph(int graphid)
{
    if(recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->Stop());
}

int MSDXAPI msdx_destroy_recv_graph(int graphid)
{
    if(recv_graph_controller) 
    {
        recv_graph_controller->Stop();
        delete recv_graph_controller;
        recv_graph_controller = nullptr;
    }
    return 0;
}

//这个也是以前的老接口，没用了
/**
* @author zhenHua.sun
* @date  2010-07-23
* @brief 销毁所有的RecvGraph
* @return 如果成功则返回0
*/
int MSDXAPI msdx_destroy_recv_graphs( void  )
{
    msdx_destroy_recv_graph(1);
	return 0;
}

int MSDXAPI msdx_set_recv_audio_codec(int graphid,char * codec_MIME,int sample_rate,int channels,int bits_per_sample)
{
    //Codec
    if (strcmp(codec_MIME, MSDX_CONF_AUDIO_CODEC_SPEEX) == 0)
        recv_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_SPEEX;
    else if (strcmp(codec_MIME, MSDX_CONF_AUDIO_CODEC_AAC) == 0)
        recv_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_AAC;
    else
        recv_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_UNKNOWN;

    //Other
    recv_graph_param.audio.sample_rate = sample_rate;
    recv_graph_param.audio.channels = channels;
    recv_graph_param.audio.bitcount = bits_per_sample;

	return 0;
}

int MSDXAPI msdx_set_recv_video_codec(int graphid,char * codecMIME,
                                      int width,int height,int fps,int BitCount)
{
    if (strcmp(MSDX_CONF_VIDEO_CODEC_H264, codecMIME) == 0)
        recv_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264;
    else
        recv_graph_param.video.codec = msdx::DX_VIDEO_CODEC_UNKNOWN;

    recv_graph_param.video.frame_width = width;
    recv_graph_param.video.frame_height = height;
    recv_graph_param.video.fps = fps;

	return 0;
}

int MSDXAPI msdx_set_recv_screen_codec(int graphid,char * codecMIME,int width,int height,int fps,int BitCount)
{
    //Deprecated
    return 0;
}

int MSDXAPI msdx_set_recv_address(const int graphid,
                                  const char * audioIP,const int audioport,
                                  const char * videoIP,const int videoport)
{
	recv_graph_param.audio.addr.ip = audioIP;
    recv_graph_param.audio.addr.port = audioport;
    recv_graph_param.video.addr.ip = videoIP;
    recv_graph_param.video.addr.port = videoport;

	return 0;
}

int MSDXAPI msdx_set_recv_screen_address(int graphid,const char * ScreenIP,const int ScreenPort)
{
    //Deprecated
    return 0;
}

//需要考虑动态变更窗口的情况
int MSDXAPI msdx_set_recv_display_window(int graphid,HWND WndHandle,long left,long top,long width,long height,int fullScreen)
{
    if (recv_graph_controller)
    {
        //当controller已创建时，需要动态变更窗口
        msdx::VideoWindow window = {0};

        window.window_handle = (unsigned long)WndHandle;
        window.position_top = top;
        window.position_left = left;
        window.width = width;
        window.height = height;

        return static_cast<int>(recv_graph_controller->ChangeVideoWindow(window));
    } 
    else
    {
        recv_graph_param.video.preview.window_handle = (unsigned long)WndHandle;
        recv_graph_param.video.preview.width = width;
        recv_graph_param.video.preview.height = height;
        recv_graph_param.video.preview.position_left = left;
        recv_graph_param.video.preview.position_top = top;
    }

	return 0;
}

//定义在ExportUtil_Common.cpp中
extern int GetAudioRendererDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name);

int MSDXAPI msdx_connect_recv_graph(int graphid)
{
    //先检查graph是否存在
    if(recv_graph_controller != nullptr) return -1;

    //外部传入的是FriendlyName，我们需要DisplayName
    //并且，要使用DirectSound设备，以支持混音，否则多路视频播放时只有一路可以出声
    //现在上层设置是只显示真实波形设备的，故先找到设定的设备对应的DirectSound设备的DisplayName
    if (!recv_graph_param.audio.output_device.empty() && GetAudioRendererDisplayNameFromFriendlyName(
        recv_graph_param.audio.output_device, recv_graph_param.audio.output_device) < 0) {
        return -1;
    }

    //创建graph controller
    recv_graph_controller = 
        msdx::GraphControllerFactory::GetInstance().CreateRecvGraphController();
    if(recv_graph_controller == nullptr)
    {
        LOG_ERROR("Create graph controller failed.");
        return -1;
    }

	return static_cast<int>(recv_graph_controller->Build(recv_graph_param));
}

int MSDXAPI msdx_run_recv_graph(int graphid)
{
    if(recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->Run());
}




/*@return
    *0:initial
    *1:running
    *2:stop
    *3:pause
**/
int MSDXAPI msdx_get_graph_state(int graphid)
{
    if (recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->GetStatus());
}

int MSDXAPI msdx_clear_recv_buf(int graphid)
{
    //Deprecated
    return 0;
}

void MSDXAPI msdx_recv_set_video_rtpstatcallback( int graphid, void *obj, void* func )
{
    //需要先调用msdx_connect_recv_graph()
    if(recv_graph_controller == nullptr) {
        LOG_WARN("Set video rtp callback before call msdx_connect_recv_graph(), ignored.");
        return;
    }
    recv_graph_controller->SetVideoRtpStatCallBack(obj, 
        static_cast<RtpStatCallback>(func));
}

void MSDXAPI msdx_recv_set_audio_rtpstatcallback( int graphid, void *obj, void* func )
{
    //需要先调用msdx_connect_recv_graph()
    if(recv_graph_controller == nullptr) {
        LOG_WARN("Set audio rtp callback before call msdx_connect_recv_graph(), ignored.");
        return;
    }
    recv_graph_controller->SetAudioRtpStatCallBack(obj, 
        static_cast<RtpStatCallback>(func));
}

/*是否开启自动纠正音视频同步偏差，目标是减小延时，加速播放，注意开启自动同步可能会引起短暂的视频马赛克*/
void MSDXAPI msdx_recv_all_enable_auto_resync(bool enable)
{
    //Deprecated
}


void MSDXAPI msdx_recv_enable_auto_resync( int graphid, bool enable )
{
    //Deprecated
}

/**
	* @brief 控制音频的接收
	* @param enable 如果为true则恢复音频的接收，否则关闭音频的接收
	* @return 如果成功则返回0
	*/
int MSDXAPI msdx_control_audio_recv( const int graphid, const bool enable )
{
    if(recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->StopReceivingAudio(!enable));
}

/**
	* @brief 控制视频的接收
	* @param enable 如果为true则恢复音频的接收，否则关闭音频的接收
	* @return 如果成功则返回0
	*/
int MSDXAPI msdx_control_video_recv( const int graphid, const bool enable )
{
    if(recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->StopReceivingVideo(!enable));
}

//fileName设为null或""表示不使用录像，而是显示图像
int MSDXAPI msdx_set_record_file_name( int graphid, const char * fileName )
{
    if (!fileName || strlen(fileName) == 0)
    {
        recv_graph_param.used_as_record = false;
        recv_graph_param.record.file_path_name = "";
        return 0;
    }

    recv_graph_param.used_as_record = true;
    recv_graph_param.record.file_path_name = fileName;
    return 0;
}

int MSDXAPI msdx_set_record_auto_cut(int graphid, int sec)
{
    //Deprecated
    return 0;
}


int MSDXAPI msdx_set_record_cut_by_episode_change( int graphid, bool enable )
{
    //Deprecated
    return 0;
}


int MSDXAPI msdx_record_cut_file(int graphid, long job_id, const char * fileName)
{
    //允许fileName为nullptr，表示让mux自动生成下一集文件名
    if(recv_graph_controller == nullptr) return -1;
    return static_cast<int>(recv_graph_controller->StartRecordingNewFile(job_id, fileName ? fileName : ""));
}


int MSDXAPI msdx_set_record_jobid(int graphid, long job_id)
{
    recv_graph_param.record.job_id = job_id;
    return 0;
}


int MSDXAPI msdx_set_record_callback( int graphid, void * obj, void * func )
{
    if(recv_graph_controller == nullptr) {
        LOG_WARN("Set record callback before call msdx_connect_recv_graph(), ignored.");
        return -1;
    }
	return static_cast<int>(recv_graph_controller->SetRecordStatCallBack(
        obj, (RecordStatCallback)func));
    return 0;
}


