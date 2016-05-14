/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __CODEC_PRESETS_H
#define __CODEC_PRESETS_H

#include "mfxvideo.h"
#include "tchar.h"
#include "sample_utils.h"

class CodecPreset
{
public:
    enum 
    {
          PRESET_UNKNOWN      =  -1  // undefined preset
        , PRESET_USER_DEFINED        // User Defined (by setting encoder parameters)
        , PRESET_BALANCED            // Optimal speed and quality
        , PRESET_BEST_QUALITY        // Best quality setting
        , PRESET_FAST                // Fast encoding with reasonable quality
        , PRESET_LOW_LATENCY
    };

    //setts up all necessary parameters for encoder according specified preset
    static mfxStatus    VParamsFromPreset(mfxVideoParam & vParams , mfxU32 preset);
    static TCHAR*       Preset2Str(mfxU32);
    static mfxU32       Str2Preset(const TCHAR *);
    static mfxU16       PresetsNum(void);
    static mfxF64       CalcFramerate(mfxU32 nFrameRateExtN , mfxU32 nFrameRateExtD);

protected:
    static mfxF64       CalcBitrate(mfxU32 preset, mfxF64 at, PartiallyLinearFNC * pfnc);
};

#endif//__CODEC_PRESETS_H
