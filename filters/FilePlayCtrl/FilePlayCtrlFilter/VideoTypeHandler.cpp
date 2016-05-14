#include "VideoTypeHandler.h"
#include "Log/Log.h"

#include <dvdmedia.h>


VideoTypeHandler::VideoTypeHandler(void)
{
}


VideoTypeHandler::~VideoTypeHandler(void)
{
}

HRESULT VideoTypeHandler::CheckMediaType( const CMediaType &mt )
{
    if (mt.majortype != MEDIATYPE_Video) return E_FAIL;

    if (mt.formattype == FORMAT_VideoInfo || mt.formattype == FORMAT_VideoInfo2)
    {
        return S_OK;
    }

    return E_FAIL;
}

HRESULT VideoTypeHandler::CalcSampleSize( const CMediaType &mt, long &size )
{
    size = 0;
    const long bytes_per_pixel = 3; //3byte足够RGB24
    if (mt.majortype != MEDIATYPE_Video) return E_FAIL;

    if (mt.formattype == FORMAT_VideoInfo)
    {
        VIDEOINFOHEADER* pVid = (VIDEOINFOHEADER *)mt.pbFormat;
        //什么值都不可靠，只有图像大小可靠
        size = pVid->bmiHeader.biWidth * pVid->bmiHeader.biHeight * bytes_per_pixel;
        LOG_INFO("Video sample size=%d*%d*%d=%dBytes", 
            pVid->bmiHeader.biWidth, pVid->bmiHeader.biHeight, bytes_per_pixel, size);
    }
    else if (mt.formattype == FORMAT_VideoInfo2)
    {
        VIDEOINFOHEADER2* pVid = (VIDEOINFOHEADER2 *)mt.pbFormat;
        size = pVid->bmiHeader.biWidth * pVid->bmiHeader.biHeight * bytes_per_pixel; 
        LOG_INFO("Video sample size=%d*%d*%d=%dBytes", 
            pVid->bmiHeader.biWidth, pVid->bmiHeader.biHeight, bytes_per_pixel, size);  
    }
    else
    {
        size = 1920 * 1080 * bytes_per_pixel;
        LOG_INFO("Using default video sample size=%dbytes", size);
    }

    return S_OK;
}

bool VideoTypeHandler::GetVideoResolution( 
  const CMediaType &mt, long &width, long &height, long &fps)
{
    if (mt.majortype != MEDIATYPE_Video) return false;

    if (mt.formattype == FORMAT_VideoInfo)
    {
        VIDEOINFOHEADER* pVid = (VIDEOINFOHEADER *)mt.pbFormat;
        width = pVid->bmiHeader.biWidth;
        height = pVid->bmiHeader.biHeight;
        fps = pVid->AvgTimePerFrame == 0 ? 30 : (long)(1e7/pVid->AvgTimePerFrame);
    }
    else if (mt.formattype == FORMAT_VideoInfo2)
    {
        VIDEOINFOHEADER2* pVid = (VIDEOINFOHEADER2 *)mt.pbFormat;
        width = pVid->bmiHeader.biWidth;
        height = pVid->bmiHeader.biHeight;
        fps = pVid->AvgTimePerFrame == 0 ? 30 : (long)(1e7/pVid->AvgTimePerFrame);
    } 

    return true;
}
