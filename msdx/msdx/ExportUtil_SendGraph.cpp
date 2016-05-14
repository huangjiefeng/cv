/*********************************************************************
 重构Graph时，为了将改动局限在msdx模块内部，没有改动导出API。
 因为历史原因，这里很多API都已废弃，或暂时无法使用了。标记为Deprecated
 的API是已经确定不再使用的。而标记为Not implementated的API，意味着其存
 在还有意义，将来可能会重新启用。
                                          —— Liaokz， 2014-11
**********************************************************************/

#include "stdafx.h"
#include "assert.h"

#include <string> //for wstring

#include "Exportutil.h"
#include "Util.h"
#include "log\Log.h"
#include "msdx\config.h"
#include "IGraphController.h"


//定义在ExportUtil_Common.cpp中
extern std::wstring Crossbar[20];
extern int NumberOfCrossbar;
extern HRESULT ListAudioCaptureDevice(std::wstring *pWStringArray, int &NumOfDevices);
extern std::wstring AudioDeviceList[20];
extern int NumberOfAudioDevice;
extern HRESULT ListVideoCrossbar(std::wstring * pWStringArray, int & NumOfDevices);
extern int GetAudioSourceDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name);

//设置音频的函数居然是发送和接收共用的，要在ExportUtil_Common.cpp中引用send_graph_param，所以不能是static
msdx::SendGraphParam send_graph_param;
static msdx::ISendGraphController *send_graph_controller;

//建一个辅助对象来维护上述局部变量，以便完成初始化和退出自动销毁
class SendGraphControllerHelper {
public:
    SendGraphControllerHelper()
    {
        SetDefaultParam();
        send_graph_controller = nullptr;
    }

    ~SendGraphControllerHelper()
    {
        if (send_graph_controller)
        {
            delete send_graph_controller;
            send_graph_controller = nullptr;
        }
    }

    void SetDefaultParam()
    {
        send_graph_param.video.crossbar_physical_type = msdx::PhysConn_Auto;
        send_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264;
        send_graph_param.video.frame_width = 1280;
        send_graph_param.video.frame_height = 720;
        send_graph_param.video.fps = 30;
        send_graph_param.video.enable_hwcodec = true;
        send_graph_param.video.preview.window_handle = 0;
        send_graph_param.video.preview.position_left = 0;
        send_graph_param.video.preview.position_top = 0;
        send_graph_param.video.preview.width = 0;
        send_graph_param.video.preview.height = 0;

        send_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_SPEEX;
        send_graph_param.audio.sample_rate = 16000;
        send_graph_param.audio.channels = 1;
        send_graph_param.audio.bitcount = 16;
        send_graph_param.audio.latency = 40;
        send_graph_param.audio.enable_aec = true;
    }
};

static SendGraphControllerHelper send_graph_helper;


/****** Implemetation of API ******/

void MSDXAPI msdx_set_default_config()
{
    if (send_graph_controller != nullptr)
    {
        LOG_WARN("Init send graph param after graph controller created.");
    }

    send_graph_helper.SetDefaultParam();
}

int MSDXAPI msdx_initial()
{
    //初始化COM
    HRESULT hr;
    hr = CoInitialize(NULL);

    msdx_set_default_config();
    if (FAILED(hr))
    {
        LOG_ERROR("assertion failed.");
        return -1;
    }

    setlocale(LC_CTYPE, ""); //用于宽字符中文wprintf

    return 0;
}


void MSDXAPI msdx_uninitial()
{
    //销毁send graph
    msdx_destroy_send_graph();
    //销毁recv graph
    msdx_destroy_recv_graphs();
    //销毁small graph
    msdx_destroy_smallvideo_graphs();

    //销毁COM
    CoUninitialize();
}

//crossbar是采集卡，现在已经基本不用了，只有VGA采集需要
void MSDXAPI msdx_config_set_video_device(
    const char *device, /*摄像头的设备标示符，现在是设备路径*/
    const char *crossbar, /*crossbar设备名称*/
    const char *cb_input) /*crossbar输入类型*/
{
    if (device) send_graph_param.video.device = device;

    //设置crossbar设备，
    if (crossbar) 
    {
        send_graph_param.video.crossbar = crossbar;

        //以下是旧的方式，以前上层传入的是friendlyName，要转换displayName...
        /*int i = 0;
        ListVideoCrossbar(Crossbar, NumberOfCrossbar);
        for (; i < NumberOfCrossbar; i+=2) 
        {
            if (Crossbar[i] == Util::str2wstr(crossbar)) //偶数索引是friendlyName，紧接着的奇数是对应displayName
            {
                send_graph_param.video.crossbar = Util::wstr2str(Crossbar[i+1]);
                break;
            }
        }
        if (i == NumberOfCrossbar) {
            LOG_ERROR("Ignore invalid crossbar friendlyName: %s", crossbar);
            return;
        }*/
    }

    using namespace msdx;

    //如果crossbar非空,将输入pin的类型传入
    if(cb_input && !send_graph_param.video.crossbar.empty())
    {
        //reference: Working with Crossbars
        //http://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
        if(strcmp(cb_input, "Tuner") == 0) 
            send_graph_param.video.crossbar_physical_type = PhysConn_Video_Tuner;
        else if(strcmp(cb_input, "Composite") == 0)
            send_graph_param.video.crossbar_physical_type = PhysConn_Video_Composite;
        else if(strcmp(cb_input, "SVideo") == 0)
            send_graph_param.video.crossbar_physical_type = PhysConn_Video_SVideo;
        else 
            send_graph_param.video.crossbar_physical_type = PhysConn_Auto; //对于其他情况不进行处理，这个值是自定义的，不是系统定义的哦
    }
    else
    {
        send_graph_param.video.crossbar_physical_type = PhysConn_Auto;
    }
}

void MSDXAPI msdx_config_set_audio_format(int SampleRate, int Channels, int BitsPerSample)
{
    send_graph_param.audio.sample_rate = SampleRate;
    send_graph_param.audio.channels = Channels;
    send_graph_param.audio.bitcount = BitsPerSample;
}

/* 
获取发送Graph的状态
@return
    *0:initial
    *1:running
    *2:stop
    *3:pause
**/
int MSDXAPI msdx_send_get_graph_state()
{
    if (send_graph_controller == nullptr) return -1;
    return static_cast<int>(send_graph_controller->GetStatus());
}

/*@ return Graph ID:-1 indicate there is no send graph*/
int MSDXAPI msdx_get_send_graph()
{
    if(send_graph_controller == nullptr) return -1;
    return 0;
}

//仅记录参数，不实际创建graph
int MSDXAPI msdx_create_send_graph( bool enable_small_video, bool disable_hwcodec)
{
    if(send_graph_controller != nullptr) return -1;

    send_graph_param.video.enable_hwcodec = !disable_hwcodec;
    return 0;
}

int MSDXAPI msdx_pause_send_graph()
{
    //TODO: Not implemented
    return 0;
}

int MSDXAPI msdx_resume_send_graph()
{
    //TODO: Not implemented
    return 0;
}

int MSDXAPI msdx_run_send_graph()
{
    if(send_graph_controller == nullptr) return -1;
    return static_cast<int>(send_graph_controller->Run());
}

int MSDXAPI msdx_stop_send_graph()
{
    if(send_graph_controller == nullptr) return -1;
    return static_cast<int>(send_graph_controller->Stop());
}

int MSDXAPI msdx_destroy_send_graph()
{
    if(send_graph_controller) 
    {
        send_graph_controller->Stop();
        delete send_graph_controller;
        send_graph_controller = nullptr;
    }
    return 0;
}

//by dsh 改，增加采样率等参数　10-07-30
int MSDXAPI msdx_set_send_audio_codec(
    const char *codec_MIME, 
    const int sample_rate, 
    const int channels, 
    const int bits_per_sample, 
    const bool aec_enable)
{
    //Codec
    if (strcmp(codec_MIME, MSDX_CONF_AUDIO_CODEC_SPEEX) == 0)
        send_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_SPEEX;
    else if (strcmp(codec_MIME, MSDX_CONF_AUDIO_CODEC_AAC) == 0)
        send_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_AAC;
    else
        send_graph_param.audio.codec = msdx::DX_AUDIO_CODEC_UNKNOWN;

    //Other
    send_graph_param.audio.sample_rate = sample_rate;
    send_graph_param.audio.channels = channels;
    send_graph_param.audio.bitcount = bits_per_sample;
    send_graph_param.audio.enable_aec = aec_enable;

    return 0;
}

int MSDXAPI msdx_set_send_video_vodec(const char * codecMIME, const int width, const int height, const int fps)
{
    send_graph_param.video.codec = msdx::DX_VIDEO_CODEC_H264; //目前仅支持H264
    send_graph_param.video.frame_width = width;
    send_graph_param.video.frame_height = height;
    send_graph_param.video.fps = fps <= 5 ? 5 : fps; //原则上不接受小于5帧的格式

	return 0;
}

int MSDXAPI msdx_set_send_screen_codec(char * codecMIME)
{
    //Deprecated
    return 0;
}

int MSDXAPI msdx_set_screen_filter_config(int left, int up, int right, int down)
{
    //Deprecated
    return 0;
}

int MSDXAPI msdx_add_send_address(const char * audio_ip, const int audio_port, 
                                  const char * video_ip, const int video_port,
                                  const bool small_video) //小流参数无效
{
    if(send_graph_controller == nullptr) return -1;

    int ret = 0;
    msdx::NetAddr addr;

    if (audio_port != 0) {
        addr.ip = audio_ip;
        addr.port = audio_port;
        ret = static_cast<int>(send_graph_controller->AddAudioDestination(addr));
        if (ret < 0) return ret;
    }

    if (video_port != 0) {
        addr.ip = video_ip;
        addr.port = video_port;
        ret = static_cast<int>(send_graph_controller->AddVideoDestination(addr));
    }
    return ret;
}


int MSDXAPI msdx_add_send_address2(const char * audioIP, const int audioport, const char * videoIP,
                                   const int videoport, const char *ScreenIP, const int ScreenPort)
{
    //Deprecated
    return 0;
}

int MSDXAPI msdx_delete_send_address(const char * audio_ip, const int audio_port, 
                                     const char * video_ip, const int video_port)
{
    if(send_graph_controller == nullptr) return -1;

    int ret = 0;
    msdx::NetAddr addr;

    addr.ip = audio_ip;
    addr.port = audio_port;
    ret = static_cast<int>(send_graph_controller->DeleteAudioDestination(addr));
    //if (ret < 0) return ret; //继续尝试删除视频

    addr.ip = video_ip;
    addr.port = video_port;
    ret = static_cast<int>(send_graph_controller->DeleteVideoDestination(addr));
    return ret;
}

int MSDXAPI msdx_delete_send_address2(const char * audioIP, const int audioport)
{
    //Deprecated
    return 0;
}

int MSDXAPI msdx_delete_screen_send_address( const char *ScreenIP, const int ScreenPort)
{
    //Deprecated
    return 0;
}

//现在仅测试时用，CoolView中本地回显是一个接收端，不是用这个预览的
//所以也没有考虑动态变更窗口的情况。recv端有考虑动态变更窗口，参考recv的实现
int MSDXAPI msdx_set_send_preview_window(HWND WndHandle, long left, long top, long width, long height, int fullScreen)
{
    send_graph_param.video.preview.window_handle = (unsigned long)WndHandle;
    send_graph_param.video.preview.position_left = left;
    send_graph_param.video.preview.position_top = top;
    send_graph_param.video.preview.width = width;
    send_graph_param.video.preview.height = height;
    return 0;
}

int MSDXAPI msdx_connect_send_graph()
{
    //先检查graph是否存在
    if(send_graph_controller != nullptr) return -1;

    //创建graph controller
    send_graph_controller = 
        msdx::GraphControllerFactory::GetInstance().CreateTestSendGraphController();//CreateSendGraphController();HJF 2015.12.15
    if(send_graph_controller == nullptr)
    {
        LOG_ERROR("Create graph controller failed.");
        return -1;
    }

    //如果不启用回声对消，要将声音设备FriendlyName转为DisplayName，这都是历史问题了
    //为了兼容各种历史问题，哥也是蛮拼的对不对？
    if (!send_graph_param.audio.enable_aec) 
    {
        if (GetAudioSourceDisplayNameFromFriendlyName(
            send_graph_param.audio.input_device, send_graph_param.audio.input_device) < 0)
            return -1;
    }

    return static_cast<int>(send_graph_controller->Build(send_graph_param));
}

//直接设置量化步长
int MSDXAPI msdx_set_send_video_quantizer(int quantizer)
{
    //TODO: Not implemented
    return 0;
}

//根据当前码率和目标码率调整码率
void MSDXAPI msdx_adapt_encode_rate(float currentRate, float destRate)
{
    //TODO: Not implemented
}

int MSDXAPI msdx_set_Rtcp_Recv_Filter(char *ListenIP, int ListenPort)
{
    //TODO: Not implemented
    return 0;
}

/**
* @author zhenHua.sun
* @date 2010-08-06
* @brief 设定视频编码器的工作状态
* @param work 如果为true则启动编码器，否则编码器不工作
* @return 0表示工作正常
*/
int MSDXAPI msdx_control_audio_encoder( bool work )
{
    if(send_graph_controller == nullptr) return -1;
    return static_cast<int>(send_graph_controller->StopSendingAudio(!work));
}
/**
* @author zhenHua.sun
* @date 2010-08-06
* @brief 设定视频编码器的工作状态
* @param work 如果为true则启动编码器，否则编码器不工作
* @return 0表示工作正常
*/
int MSDXAPI msdx_control_video_encoder( bool work )
{
    if(send_graph_controller == nullptr) return -1;
    return static_cast<int>(send_graph_controller->StopSendingVideo(!work));
}


int MSDXAPI msdx_delete_Rtcp_Recv_address(char *ListenIP, int ListenPort)
{
    //Deprecated
    return 0;
}

void MSDXAPI msdx_send_set_video_rtpstatcallback( void *obj, void *func )
{
    assert(send_graph_controller != nullptr); //需要先调用msdx_connect_send_graph()
    if(send_graph_controller == nullptr) return;
    send_graph_controller->SetVideoRtpStatCallBack(obj, 
        static_cast<RtpStatCallback>(func));
}

void MSDXAPI msdx_send_set_audio_rtpstatcallback( void *obj, void *func )
{
    assert(send_graph_controller != nullptr); //需要先调用msdx_connect_send_graph()
    if(send_graph_controller == nullptr) return;
    send_graph_controller->SetAudioRtpStatCallBack(obj, 
        static_cast<RtpStatCallback>(func));
}

//by dsh 10-08-06
void MSDXAPI msdx_set_audio_link_state(bool state)
{
    //Deprecated
}


int MSDXAPI msdx_send_set_video_dscp(const int dscp )
{
	//TODO: Not implemented
    return 0;
}

int MSDXAPI msdx_send_set_audio_dscp(const int dscp )
{
	//TODO: Not implemented
    return 0;
}

/**
	*	@brief	统计传输流量情况
	 * @param transtat 传入一个TranStat结构指针变量，统计结果值将保存到该变量中;	TranStat结构的定义请参见RtpStat.h
	 * @see RtpStat.h
	 *	@return 0表示统计成功，负数则统计失败或加载rtpware对应的函数失败
	 */
int  msdx_compute_global_transtat( void * transtat )
{
    //TODO: Not implemented
    return 0;
}


void MSDXAPI msdx_send_enable_adaptive_coderate( bool enable )
{
	//TODO: Not implemented
}


int MSDXAPI msdx_send_cut_file()
{
	//Deprecated
	return 0;
}

/******* Test Code *******/

int MSDXAPI testRoute()
{
    //Deprecated
    return 0;
}
