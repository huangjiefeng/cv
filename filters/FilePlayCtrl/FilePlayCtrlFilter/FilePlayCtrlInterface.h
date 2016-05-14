#ifndef FILE_PLAY_CTRL_INETRFACE_H
#define FILE_PLAY_CTRL_INETRFACE_H

//GUID must by the same as the definition in the guid.h
MIDL_INTERFACE("238B36E2-87FC-4087-A19F-3B8A0C3D9013")
ISCUTFilePlayCtrl : public IUnknown
{
public:
    // sync id
    virtual HRESULT SetSyncId(const char *id) = 0;
 
    // initial timestamp offset
    virtual HRESULT SetInitialTsOffset(REFERENCE_TIME offset) = 0;

    // play operation code and param
    enum PlayOpCode { kOpPause, kOpResume, kOpSeek };
    union PlayOpParam {
        struct {
            long long t;
        } seek;
    };
    // play operation. each time call this method, op_id must larger than previous 
    virtual HRESULT Operate(unsigned long op_id, PlayOpCode op, PlayOpParam *param) = 0;
    
    enum VideoCodec { VIDEO_NONE = 0, H264 };
    enum AudioCodec { AUDIO_NONE = 0, AAC_LC };
    struct MediaFormat
    {
        VideoCodec video_codec;
        long video_width;
        long video_height;
        long video_fps;
        AudioCodec audio_codec;
        long audio_sample_rate;
        long audio_channel;
        long audio_bit_count;
    };
    //call this method to get media infomation
    virtual HRESULT GetMediaFormat(MediaFormat &fmt) = 0;

    enum PlayStatus { ST_STOP = 0, ST_PLAYING, ST_PAUSE, ST_WAIT, ST_EOS };
    struct ProgressInfo
    {
        PlayStatus status;
        long long position;
        long long duration;
    };
    //call this method to get play progress info
    virtual HRESULT GetProgress(ProgressInfo &info) = 0;
};

#endif
