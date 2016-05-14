/************************************************************************/
/* 构建各种MediaType的方法                                              */
/************************************************************************/

#ifndef MEDIA_TYPE_UTIL_H
#define MEDIA_TYPE_UTIL_H

#include <streams.h>

#include "DxDefinitions.h"


namespace msdx
{


class MediaTypeUtil
{
private:
    MediaTypeUtil(void) {};
    ~MediaTypeUtil(void) {};

public:

    struct VideoTypeParam
    {
        CLSID media_subtype;
        int width;
        int height;
        int fps;
    };
    static dxStatus GetVideoType(VideoTypeParam param, CMediaType &media_type);

    struct AudioTypeParam
    {
        CLSID media_subtype;
        int sample_rate;
        int channels;
        int bits_per_sample;
    };
    static dxStatus GetAudioType(const AudioTypeParam &param, CMediaType &media_type);
};

}

#endif
