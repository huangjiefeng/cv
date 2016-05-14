#ifndef AUDIO_TYPE_HANDLER_H
#define AUDIO_TYPE_HANDLER_H

#include "itypehandler.h"

class AudioTypeHandler :
    public ITypeHandler
{
public:
    AudioTypeHandler(void);
    ~AudioTypeHandler(void);

    virtual HRESULT CheckMediaType(const CMediaType &mt);
    virtual HRESULT CalcSampleSize(const CMediaType &mt, long &size);
    virtual bool IsVideo() { return false; }
    virtual bool IsAudio() { return true; }
};

#endif
