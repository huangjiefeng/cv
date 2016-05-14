#ifndef FILE_PLAY_GRAPH_CONTROLLER_H
#define FILE_PLAY_GRAPH_CONTROLLER_H

#include "IGraphController.h"

#include <streams.h>
#include <atlbase.h>

#include "GraphUtil.h"
#include "GraphContainer.h"
#include "FilterContainer.h"

namespace msdx
{

class FilePlayGraphController :
    public IFilePlayGraphController
{
public:
    FilePlayGraphController(void);
    ~FilePlayGraphController(void);

    virtual dxGraphStatus GetStatus();

    virtual dxStatus Build( FilePlayGraphParam & );

    virtual dxStatus Run();
    virtual dxStatus Stop();
 
    virtual dxStatus Pause(unsigned long op_id);
    virtual dxStatus Resume(unsigned long op_id);
    virtual dxStatus SeekTo(unsigned long op_id, long long t);

    dxStatus SetFilePlayStatCallBack(
      void *obj, FilePlayStatCallback func);

    void ReportMediaFormat();
    void ReportPlayStatus();

protected:
    void Reset();
    dxStatus CreateGraph();
    dxStatus AddVideoLink();
    dxStatus AddAudioLink();

    static VOID CALLBACK CallbackTimerProc(
        HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

private:
    FilePlayGraphParam graph_param_;

    //运行参数
    dxGraphStatus graph_status_; //graph状态

    //graph COM
    GraphContainer *graph_;

    FileSourceContainer *file_source_;
    CComPtr<IBaseFilter> demux_filter_;
    CComPtr<IBaseFilter> play_ctrl_filter_;
    RTPSenderContainer *video_rtp_sender_;
    RTPSenderContainer *audio_rtp_sender_;

    struct FilePlayCallback 
    {
      void * obj;
      FilePlayStatCallback func;
    } file_play_callback_;

    UINT_PTR callback_timer_id_;
    
};


}

#endif // !FILE_PLAY_GRAPH_CONTROLLER_H
