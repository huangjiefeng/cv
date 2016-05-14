/* ////////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
//
//
*/

#pragma once

#include "mfx_filter_externals.h"
#include "sample_utils.h"

#include "mfxvideo.h"
#include "mfxvideo++.h"
#include "memory.h"

#include "strmif.h"

#include <stdio.h>
#include <tchar.h>
#include <list>

#include "bs_adapter.h"
#include "codec_presets.h"

#include <d3d9.h>
#include <mfapi.h>
#include <dxva2api.h>
#include <evr.h>
#include <mferror.h>

mfxStatus   InitMfxFrameSurface     (mfxFrameSurface1* pSurface, mfxFrameInfo* pInfo, int nSurfaceSize);

mfxStatus   GetBitrateRanges        (mfxU32 nWidth, mfxU32* nMin, mfxU32* nMax, mfxU32* nRecommended);

//functions for converting mfxVideoParam to filters internal format
//definitions are here, implementation are in particular filter
mfxStatus CopyMFXToEncoderParams(IConfigureVideoEncoder::Params* pParams, mfxVideoParam* pmfxParams);
mfxStatus CopyEncoderToMFXParams(IConfigureVideoEncoder::Params* pParams, mfxVideoParam* pmfxParams);

mfxU64 ConvertReferenceTime2MFXTime(REFERENCE_TIME rtTime);

class CTimeManager
{
public:

    CTimeManager() : m_dFrameRate(0), m_nFrameNumber(0), m_nBitrate(0) {};

    void Init(DOUBLE dFrameRate)
    {
        m_dFrameRate = dFrameRate;
    };
    void Reset()
    {
        m_nFrameNumber = 0;
    };
    INT GetEncodedFramesCount()
    {
        return m_nFrameNumber;
    };
    DOUBLE GetFrameRate()
    {
        return m_dFrameRate;
    };
    INT GetBitrate()
    {
        return m_nBitrate;
    };
    void SetEncodedFramesCount(INT nFrameNumber)
    {
        m_nFrameNumber = nFrameNumber;
    };

    void SetBitrate(INT nBitrate)
    {
        m_nBitrate = nBitrate;
    };
    HRESULT GetTime(REFERENCE_TIME* dStart, REFERENCE_TIME* dStop)
    {
        if (!dStart || !dStop)
            return E_POINTER;        
        if (!m_dFrameRate) 
            return E_FAIL;

        m_nFrameNumber++;

        *dStart = (REFERENCE_TIME)((1.0/m_dFrameRate * (m_nFrameNumber - 1)) * 1e7);
        *dStop  = (REFERENCE_TIME)(*dStart + (1.0/m_dFrameRate) * 1e7);

        return S_OK;
    };

private:
    DOUBLE      m_dFrameRate;
    INT         m_nFrameNumber;
    INT         m_nBitrate;
};
