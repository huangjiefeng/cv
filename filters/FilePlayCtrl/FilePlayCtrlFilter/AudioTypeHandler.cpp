#include "AudioTypeHandler.h"
#include "Log/Log.h"

AudioTypeHandler::AudioTypeHandler(void)
{
}


AudioTypeHandler::~AudioTypeHandler(void)
{
}

HRESULT AudioTypeHandler::CheckMediaType( const CMediaType &mt )
{
    if (mt.majortype != MEDIATYPE_Audio) return E_FAIL;
    return S_OK;
}

HRESULT AudioTypeHandler::CalcSampleSize( const CMediaType &mt, long &size )
{
    size = 0;
    if (mt.majortype != MEDIATYPE_Audio) return E_FAIL;

    if (mt.formattype == FORMAT_WaveFormatEx)
    {
        WAVEFORMATEX * pFmt = (WAVEFORMATEX *)mt.pbFormat;
        //计算足够1秒的大小
        //size = pFmt->nAvgBytesPerSec; //这个值不靠谱儿，可能是0
        size = pFmt->nSamplesPerSec * pFmt->nChannels * pFmt->wBitsPerSample / 8;
        LOG_INFO("Audio sample size=%dHz*%dch*%dbits=%dBytes", 
            pFmt->nSamplesPerSec, pFmt->nChannels, pFmt->wBitsPerSample, size);
    }
    else
    {
        size = 96000 * 2 * 16 / 8; //足够96k双声道16bit音频1秒
        LOG_INFO("Using default audio sample size=%dBytes", size);
    }

    return S_OK;
}
