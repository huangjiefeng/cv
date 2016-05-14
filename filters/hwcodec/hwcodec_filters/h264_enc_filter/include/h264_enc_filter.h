/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#pragma once

#include "mfx_video_enc_filter.h"
#include "h264_enc_proppage.h"

class CH264EncVideoFilter 
    : public CEncVideoFilter
{
public:

    DECLARE_IUNKNOWN;
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    CH264EncVideoFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
    
    // Fills a counted array of GUID values where each GUID specifies the CLSID of each property page that can be displayed
    STDMETHODIMP        GetPages(CAUUID *pPages);

    HRESULT             CheckInputType(const CMediaType* mtIn);

    // Custom interface IConfigureVideoEncoder functions declarations
    STDMETHODIMP        SetParams(Params *params);

protected:

    // Fill m_EncoderParams with default values
    void                SetDefaultParams(void);
    // Fill m_EncoderParams with values from registry if available, 
    // otherwise write current m_EncoderParams values to registry
    void                ReadParamsFromRegistry(void);
    // Write current m_EncoderParams values to registry
    void                WriteParamsToRegistry(void);
    void                ReadFrameRateFromRegistry(mfxU32 &iFrameRate);
    void                UpdateRegistry(void) 
    {CopyMFXToEncoderParams(&m_EncoderParams, &m_mfxParamsVideo); WriteParamsToRegistry(); CEncVideoFilter::UpdateRegistry();};

    BOOL                CheckOnlyBitrateChanged(mfxVideoParam* paramsVideo1, mfxVideoParam* paramsVideo2, DWORD preset);    
};

