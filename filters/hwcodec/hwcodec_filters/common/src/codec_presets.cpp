/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "codec_presets.h"
#include "memory.h"

struct 
{
    mfxU32 preset;
    TCHAR  desc[32];
} g_AllPresets[]=
{
    {CodecPreset::PRESET_BEST_QUALITY, _T("Best quality")},
    {CodecPreset::PRESET_BALANCED,     _T("Normal")},
    {CodecPreset::PRESET_FAST,         _T("High Speed")},
    {CodecPreset::PRESET_LOW_LATENCY,  _T("Low Latency")},
    {CodecPreset::PRESET_USER_DEFINED, _T("User Defined...")}
};

mfxU16 CodecPreset::PresetsNum(void)
{
    return sizeof(g_AllPresets)/sizeof(g_AllPresets[0]);
}

TCHAR* CodecPreset::Preset2Str(mfxU32 preset)
{
    for (mfxU16 i = 0; i < sizeof(g_AllPresets)/sizeof(g_AllPresets[0]); i++)
    {
        if (g_AllPresets[i].preset == preset)
        {
            return g_AllPresets[i].desc;
        }
    }
    return NULL;
}

mfxU32 CodecPreset::Str2Preset(const TCHAR * str)
{
    for (mfxU16 i = 0; i < sizeof(g_AllPresets)/sizeof(g_AllPresets[0]); i++)
    {
        if (!_tcscmp(str, g_AllPresets[i].desc))
        {
            return g_AllPresets[i].preset;
        }
    }
    return PRESET_UNKNOWN;
}

mfxStatus CodecPreset::VParamsFromPreset(mfxVideoParam & vParams , mfxU32 preset)
{
    //do not change values at user defined preset
    if (preset == PRESET_USER_DEFINED)
    {
        return MFX_ERR_NONE;
    }
    
    mfxFrameInfo tmpInfo;
    mfxU32       codecId;
    mfxU32       nBitrate;

    memcpy(&tmpInfo, &vParams.mfx.FrameInfo, sizeof(mfxFrameInfo));
    codecId = vParams.mfx.CodecId;
    nBitrate = vParams.mfx.TargetKbps;
    
    memset(&vParams, 0, sizeof(vParams));  

    memcpy(&vParams.mfx.FrameInfo, &tmpInfo, sizeof(mfxFrameInfo));
    vParams.mfx.CodecId = codecId;
    vParams.mfx.TargetKbps = vParams.mfx.MaxKbps = nBitrate;
        
    switch (preset)
    {
        case PRESET_FAST :
        {
            vParams.mfx.TargetUsage = MFX_TARGETUSAGE_BEST_SPEED;
            break;
        }
        case PRESET_BALANCED :
        {
            vParams.mfx.TargetUsage = MFX_TARGETUSAGE_BALANCED;
            break;
        }
        case PRESET_BEST_QUALITY :
        {
            vParams.mfx.TargetUsage = MFX_TARGETUSAGE_BEST_QUALITY;
            break;
        }
        case PRESET_LOW_LATENCY :
        {
            vParams.mfx.TargetUsage = MFX_TARGETUSAGE_BALANCED;
            vParams.mfx.NumRefFrame = 1;
            vParams.mfx.GopRefDist = 1;
            vParams.mfx.RateControlMethod = MFX_RATECONTROL_VBR;
            vParams.AsyncDepth = 1;
            break;
        }
        default:
            return MFX_ERR_UNSUPPORTED;
    }

    //resolution will be used as reference
    if (vParams.mfx.FrameInfo.CropW == 0 || vParams.mfx.FrameInfo.CropH == 0)
    {
        vParams.mfx.FrameInfo.CropW = 720;
        vParams.mfx.FrameInfo.CropH = 576;
    }  

    PartiallyLinearFNC fnc;

    switch (vParams.mfx.CodecId)
    {
        case MFX_CODEC_AVC : 
        {
            fnc.AddPair(25344,  225);
            fnc.AddPair(101376, 1000);
            fnc.AddPair(414720, 4000);
            fnc.AddPair(2058240,5000);

            vParams.mfx.CodecProfile        = MFX_PROFILE_AVC_HIGH;
            vParams.mfx.FrameInfo.PicStruct = MFX_PICSTRUCT_PROGRESSIVE;
            break;
        }
        case MFX_CODEC_MPEG2: 
        {
            fnc.AddPair(0,0);
            fnc.AddPair(414720, 12000);
            vParams.mfx.FrameInfo.PicStruct = MFX_PICSTRUCT_PROGRESSIVE;
            break;        
        }
        default:
            return MFX_ERR_UNSUPPORTED;
    }
    
    mfxF64 fSquare = vParams.mfx.FrameInfo.CropW * vParams.mfx.FrameInfo.CropH;
    mfxF64 ffps    = CalcFramerate(vParams.mfx.FrameInfo.FrameRateExtN, vParams.mfx.FrameInfo.FrameRateExtD);
    if (ffps <= 1) ffps = 30;
    
    if (!(vParams.mfx.TargetKbps && (PRESET_LOW_LATENCY == preset)))
    {
        vParams.mfx.MaxKbps = 
            vParams.mfx.TargetKbps = (mfxU16)CalcBitrate(preset, fSquare * ffps / 30.0, &fnc);
    }    

    return vParams.mfx.TargetKbps == 0 ? MFX_ERR_UNSUPPORTED : MFX_ERR_NONE;
}

mfxF64 CodecPreset::CalcBitrate(mfxU32 preset, mfxF64 at, PartiallyLinearFNC * pfnc)
{
    if (NULL == pfnc)
    {
        return 0;
    }

    switch (preset)
    {
        case PRESET_FAST :
        {
            return  pfnc->at(at) / 2;
        }
        case PRESET_BALANCED :
        case PRESET_LOW_LATENCY :
        case PRESET_USER_DEFINED :
        {
            return  pfnc->at(at) * 0.75;
        }
        case PRESET_BEST_QUALITY :
        {
            return  pfnc->at(at);
        }
    default:
        return 0;
    }
}

mfxF64 CodecPreset::CalcFramerate(mfxU32 nFrameRateExtN , mfxU32 nFrameRateExtD)
{
    if (nFrameRateExtN && nFrameRateExtD)
        return (mfxF64)nFrameRateExtN / nFrameRateExtD;
    else
        return 0;
}
