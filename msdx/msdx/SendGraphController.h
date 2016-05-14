#ifndef SEND_GRAPH_CONTROLLER_H
#define SEND_GRAPH_CONTROLLER_H

#include "IGraphController.h"

#include <streams.h>
#include <atlbase.h>

#include "GraphUtil.h"
#include "GraphContainer.h"
#include "FilterContainer.h"

namespace msdx
{

class SendGraphController : public ISendGraphController
{
public:
    SendGraphController(void);
    virtual ~SendGraphController(void);

    virtual dxGraphStatus GetStatus();

    virtual dxStatus Build( SendGraphParam & );
	
    virtual dxStatus Run();
    virtual dxStatus Stop();

    virtual dxStatus AddVideoDestination( const NetAddr & );
    virtual dxStatus DeleteVideoDestination( const NetAddr & );

    virtual dxStatus AddAudioDestination( const NetAddr & );
    virtual dxStatus DeleteAudioDestination( const NetAddr & );

    virtual dxStatus StopSendingVideo( bool stop );
    virtual dxStatus StopSendingAudio( bool stop );

    virtual dxStatus SetVideoRtpStatCallBack( void *obj, RtpStatCallback func );
    virtual dxStatus SetAudioRtpStatCallBack( void *obj, RtpStatCallback func );

protected:
    void Reset();
    dxStatus CreateGraph();

protected:
    //构建参数
    SendGraphParam graph_param_;

    //运行参数
    dxGraphStatus graph_status_; //graph状态

    //graph
    GraphContainer *graph_;

    CrossbarContainer *video_crossbar_;
    VideoSourceContainer *video_source_;
    InfPinTeeContainer *video_inf_pin_tee_;
    VideoEncoderContainer *video_encoder_;
    RTPSenderContainer *video_rtp_sender_;
    VideoRendererContainer *video_renderer_;
    AudioSourceContainer *audio_source_;
    AudioEncoderContainer *audio_encoder_;
    RTPSenderContainer *audio_rtp_sender_;

#ifdef _DEBUG
    DWORD debug_register_;
#endif

    //callback
    RtpCallback video_rtp_callback_;
    RtpCallback audio_rtp_callback_;
};

class TestSendGraphController:public SendGraphController
{
public:
	TestSendGraphController(){};
	virtual ~TestSendGraphController(){};
	virtual dxStatus Build(SendGraphParam&);
};
}
#endif

