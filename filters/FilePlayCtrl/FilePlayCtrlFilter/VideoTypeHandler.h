#ifndef VIDEO_TYPE_HANDLER_H
#define VIDEO_TYPE_HANDLER_H

#include "itypehandler.h"


class VideoTypeHandler :
    public ITypeHandler
{
public:
    VideoTypeHandler(void);
    ~VideoTypeHandler(void);

    virtual HRESULT CheckMediaType(const CMediaType &mt);
    virtual HRESULT CalcSampleSize(const CMediaType &mt, long &size);
    virtual bool IsVideo() { return true; }
    virtual bool IsAudio() { return false; }

    //特定的类型方法
    static bool GetVideoResolution(const CMediaType &mt, long &width, long &height, long &fps);
};


#endif
