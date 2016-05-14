#ifndef TEST_GRAPH_CONTROLLER_H
#define TEST_GRAPH_CONTROLLER_H

#include "IGraphController.h"

#include <streams.h>
#include <atlbase.h>

#include "GraphUtil.h"
#include "GraphContainer.h"
#include "FilterContainer.h"

namespace msdx
{

class TestGraphController :
    public ITestGraphController
{
public:
    TestGraphController(void);
    ~TestGraphController(void);

    virtual dxStatus Build( TestGraphParam & );

    virtual dxStatus Run();
    virtual dxStatus Stop();

private:
    TestGraphParam graph_param_;

    //graph COM
    GraphContainer *graph_;

    VideoSourceContainer *video_source_;
    CrossbarContainer *video_crossbar_;
    InfPinTeeContainer *video_inf_pin_tee_;
    VideoRendererContainer *video_renderer_;

    FileSourceContainer *audio_file_source_;
    AudioSourceContainer *audio_source_;
    AudioRendererContainer *audio_renderer_;
    CComPtr<IBaseFilter> audio_null_renderer_;

};


}

#endif // !TEST_GRAPH_CONTROLLER_H