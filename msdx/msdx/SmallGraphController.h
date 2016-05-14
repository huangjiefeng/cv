#ifndef SMALL_GRAPH_CONTROLLER_H
#define SMALL_GRAPH_CONTROLLER_H

#include "IGraphController.h"

#include <streams.h>
#include <atlbase.h>

#include "GraphUtil.h"
#include "GraphContainer.h"
#include "FilterContainer.h"

namespace msdx
{

class SmallGraphController : public ISmallGraphController
{
public:
    SmallGraphController(void);
    ~SmallGraphController(void);

    virtual dxGraphStatus GetStatus();

    virtual dxStatus Build( SmallGraphParam & param);

    virtual dxStatus Run();
    virtual dxStatus Stop();

    virtual dxStatus AddVideoDestination( const NetAddr & );
    virtual dxStatus DeleteVideoDestination( const NetAddr & );

    virtual dxStatus StopSendingVideo( bool stop );

private:
    void Reset();
    dxStatus CreateGraph();

private:
    //构建参数
   SmallGraphParam graph_param_;

    //运行参数
    dxGraphStatus graph_status_; //graph状态

    //graph COM
    GraphContainer *graph_;

    RTPReceiverContainer *video_rtp_receiver_;
    VideoDecoderContainer *video_decoder_;
    VideoEncoderContainer *video_encoder_;
    RTPSenderContainer *video_rtp_sender_;

#ifdef _DEBUG
    DWORD debug_register_;
#endif

};

}

#endif
