/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#pragma once

#include "windows.h"
#include "mfxvideo.h"
#include "mfxvideo++.h"

class CBitstreamAdapter {
public:
    CBitstreamAdapter(void)                                             { m_bInited = false; };

    virtual void        Init(mfxU32 nVideoType)                         { nVideoType; };
    virtual void        Reset()                                         { };

    virtual mfxStatus   Adapt(mfxBitstream mfxBS)                       { mfxBS;   return MFX_ERR_NONE; };    
    virtual mfxU8*      ExtractHeader(mfxU8* pBuffer, mfxU32* pSize)    { pBuffer; *pSize = 0; return NULL; };
    virtual bool        IsKeyFrame(mfxBitstream mfxBS)                  { mfxBS;   return false; };

public:
    bool                m_bInited;
};