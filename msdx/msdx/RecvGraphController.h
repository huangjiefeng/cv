#ifndef RECV_GRAPH_CONTROLLER_H
#define RECV_GRAPH_CONTROLLER_H

#include "IGraphController.h"

#include <streams.h>
#include <atlbase.h>

#include "GraphUtil.h"
#include "GraphContainer.h"
#include "FilterContainer.h"

namespace msdx
{

class RecvGraphController :
    public IRecvGraphController
{
public:
    RecvGraphController(void);
    ~RecvGraphController(void);

    virtual dxGraphStatus GetStatus();

    virtual dxStatus Build(RecvGraphParam &param);

    virtual dxStatus Run();
    virtual dxStatus Stop();

    virtual dxStatus StopReceivingVideo( bool stop );
    virtual dxStatus StopReceivingAudio( bool stop );

    virtual dxStatus ChangeVideoWindow(VideoWindow window);

    virtual dxStatus StartRecordingNewFile( long job_id, std::string new_file_name );
    virtual dxStatus SetRecordStatCallBack( void *obj, RecordStatCallback func );

    virtual dxStatus SetVideoRtpStatCallBack( void *obj, RtpStatCallback func );
    virtual dxStatus SetAudioRtpStatCallBack( void *obj, RtpStatCallback func );

private:
    void Reset();
    dxStatus CreateGraph();
    dxStatus AddAudioDecoder();
    dxStatus AddRecord();

private:
    //构建参数
    RecvGraphParam graph_param_;

    //运行参数
    dxGraphStatus graph_status_; //graph状态

    //graph COM
    GraphContainer *graph_;

    RTPReceiverContainer *video_rtp_receiver_;
    VideoDecoderContainer *video_decoder_;
    VideoRendererContainer *video_renderer_;
    RTPReceiverContainer *audio_rtp_receiver_;
    AudioDecoderContainer *audio_decoder_;
    AudioRendererContainer *audio_renderer_;

    AudioEncoderContainer *record_audio_encoder_;
    CComPtr<IBaseFilter> record_mux_writer_; //录像只有这里用到，不包成Container了

    //callback
    RtpCallback video_rtp_callback_;
    RtpCallback audio_rtp_callback_;

    struct RecordCallback 
    {
        void * obj;
        RecordStatCallback func;
    } record_muxer_callback;
};


}

#endif