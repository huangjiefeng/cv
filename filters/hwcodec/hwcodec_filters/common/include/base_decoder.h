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

#include <list>

#include "sample_utils.h"
#include "memory_allocator.h"
#include "mfx_filter_defs.h"

#define MFX_THREAD_WAIT_TIME 1
#define MFX_THREAD_WAIT Sleep(MFX_THREAD_WAIT_TIME)
#define BREAK_ON_EPOINTER(P, STS) {if(!(P)) { STS = MFX_ERR_MEMORY_ALLOC; break;}}

//#define MFX_EMULATE_HW

class DXVA2Sample; 

class CBaseDecoder 
{
    friend class CDecVideoFilter;

public:
    CBaseDecoder(mfxU16 APIVerMinor = 1, mfxU16 APIVerMajor = 1, mfxStatus *pSts = NULL);
    ~CBaseDecoder();

    mfxStatus   Init(mfxVideoParam* pVideoParams, mfxU32 nPitch);
    mfxStatus   RunDecode(mfxBitstream* pBS, mfxFrameSurface1* &pFrameSurface);
    mfxStatus   Reset(mfxVideoParam* pVideoParams, mfxU32 nPitch);
    mfxStatus   Close();

    mfxStatus   GetVideoParams(mfxVideoParam* pVideoParams);
    mfxStatus   GetDecodeStat(mfxDecodeStat* pVideoStat);

    mfxU32      GetIOSurfNum(mfxVideoParam* pVideoParams);
    
    mfxStatus   SetFrameAllocator(MFXFrameAllocator* pAllocator)
    {
        m_pMfxFrameAllocator = pAllocator;
        FreeFrames();
        mfxStatus sts = m_mfxVideoSession.SetFrameAllocator(m_pMfxFrameAllocator);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        return MFX_ERR_NONE;
    }

    mfxStatus   AllocFrames(mfxVideoParam* pVideoParams, mfxU32 nPitch);
    mfxStatus   FreeFrames();

    mfxStatus    SetAcceleration(mfxIMPL impl);

    mfxStatus  SetHandle(DSAllocatorParams* pParams)
    {
        mfxIMPL impl;
        QueryIMPL(&impl);
        mfxStatus sts = MFX_ERR_NONE;

        if (m_bd3dAlloc)
        {
            DSAllocatorParams *pDSParams = NULL;
            pDSParams = dynamic_cast<DSAllocatorParams *>(pParams);
            if (!pDSParams)
            {
                return MFX_ERR_NOT_INITIALIZED;
            }

           sts = m_mfxVideoSession.SetHandle(MFX_HANDLE_D3D9_DEVICE_MANAGER, pDSParams->pManager);
           MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        }

        return MFX_ERR_NONE;
    }

    mfxStatus   QueryIMPL(mfxIMPL *impl) 
    {
        m_mfxVideoSession.QueryIMPL(impl); 

#ifdef MFX_EMULATE_HW
        *impl = MFX_IMPL_HARDWARE;
#endif

        return MFX_ERR_NONE; 
    };

    mfxStatus DecodeHeader(mfxBitstream *bs, mfxVideoParam *par) 
    {
        mfxStatus sts = MFX_ERR_NONE;
        MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NULL_PTR);
        MSDK_CHECK_POINTER(bs, MFX_ERR_NULL_PTR);
        MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);

        if (MFX_CODEC_JPEG == par->mfx.CodecId)
        {
            sts = MJPEG_AVI_ParsePicStruct(bs);
            if (MFX_ERR_NONE != sts)
            {
                m_bHeaderDecoded = false;
                return sts;
            }
            sts = m_pmfxDEC->DecodeHeader(bs, par); 
            par->mfx.FrameInfo.PicStruct = bs->PicStruct;
            if (bs->PicStruct == MFX_PICSTRUCT_FIELD_TFF || bs->PicStruct == MFX_PICSTRUCT_FIELD_BFF)
            {
                par->mfx.FrameInfo.CropH *= 2;
                par->mfx.FrameInfo.Height = MSDK_ALIGN16(par->mfx.FrameInfo.CropH);                
            }
        }
        else
        {
            sts = m_pmfxDEC->DecodeHeader(bs, par); 
        }
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);

        m_bHeaderDecoded = (MFX_ERR_NONE == sts) ? true : false;

        return sts; 
    };

    void HeaderDecoded(bool bParse)
    {
        m_bHeaderDecoded = bParse;
    };

    bool HeaderDecoded()
    {
        return m_bHeaderDecoded;
    }

    mfxStatus  SetAuxFramesNum(mfxU16 nFrames)
    {
        m_nAuxFramesNum = nFrames;

        return MFX_ERR_NONE;
    }

    void SetAdditionalMemTypeFlags(mfxU16 flags)
    {
        m_nAdditionalMemTypeFlags = flags;        
    }

    bool GetHeaderDecoded() 
    { 
        return m_bHeaderDecoded;
    }

    bool GetMemoryType()
    {
        return m_bd3dAlloc;
    };
        
protected:

    mfxStatus   InternalReset(mfxVideoParam* pVideoParams, mfxU32 nPitch, bool bInited);   
    mfxU8       FindFreeSurface();

protected:

    MFXVideoSession         m_mfxVideoSession;
    
    MFXVideoDECODE*         m_pmfxDEC;

    mfxFrameSurface1**      m_ppFrameSurfaces;

    mfxFrameAllocator*      m_pAllocExt;
    bool                    m_bd3dAlloc;
    mfxFrameAllocResponse   m_AllocResponse;
    
    mfxU16                  m_nRequiredFramesNum;

    bool                    m_bHeaderDecoded;
    mfxU32                  m_nCodecId; 

    mfxVideoParam*          m_pVideoParams;
    
    MFXFrameAllocator*      m_pMfxFrameAllocator;

    mfxU16                  m_nAuxFramesNum;   

    bool                    m_bDecPartialAcceleration; 

    mfxU16                  m_nAdditionalMemTypeFlags;
    bool                    m_bSessionHasParent;    

    // Media SDK API version the object requires
    mfxU16                  m_nAPIVerMinor;
    mfxU16                  m_nAPIVerMajor;

private:
   DISALLOW_COPY_AND_ASSIGN(CBaseDecoder);
};