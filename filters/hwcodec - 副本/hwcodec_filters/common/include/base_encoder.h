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

#include <list>
#include <process.h>

#include "windows.h"
#include "mfxvideo.h"
#include "mfxvideo++.h"
#include "mfx_multi_vpp.h"

#include "sample_utils.h"
#include "memory_allocator.h"
#include "bitstream_pool.h"

#include "plugin_csc.h"

#define MFX_THREAD_WAIT_TIME 1
#define MFX_THREAD_WAIT Sleep(MFX_THREAD_WAIT_TIME)
#define BREAK_ON_EPOINTER(P, STS) {if(!(P)) { STS = MFX_ERR_MEMORY_ALLOC; break;}}

//#define MFX_EMULATE_HW

struct sMFXSample
{
    IUnknown*           pSample;
    mfxFrameSurface1*   pmfxSurface;
};

interface INotifier
{
    virtual mfxStatus OnFrameReady(mfxBitstream* pBS) = 0;
};


class CBaseEncoder 
{
    friend class CEncVideoFilter;
    friend class CEncoderInputPin;

public:
    CBaseEncoder(mfxU16 APIVerMinor = 1, mfxU16 APIVerMajor = 1, mfxStatus *pSts = NULL);
    ~CBaseEncoder(void);

    mfxStatus   Init(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams, INotifier* pNotifier);
    mfxStatus   RunEncode(IUnknown *pSample, mfxFrameSurface1* pFrameSurface);
    mfxStatus   Reset(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams);
    mfxStatus   Close();

    mfxStatus   GetVideoParams(mfxVideoParam* pVideoParams);
    mfxStatus   GetEncodeStat(mfxEncodeStat* pVideoStat);

    mfxStatus   SetAcceleration(mfxIMPL impl);

    mfxStatus   DynamicBitrateChange(mfxVideoParam* pVideoParams);

    mfxStatus   SetPluginParams(const CSCParam &pluginParams) { m_PluginParams = pluginParams; return MFX_ERR_NONE;};

    mfxStatus   AllocVPPPlugin();

    // TODO:强制输出关键帧，暂时放这里 by Liaokz
    mfxStatus   ForceGenerateKeyFrame() { m_bForceGenKeyFrame = true;  return MFX_ERR_NONE;}; 
protected:

    mfxStatus   InternalReset(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams, bool bInited);
    mfxStatus   InternalClose();
    mfxU8       FindFreeSurfaceVPPOut();
    bool        UseCSCPlugin(const mfxVideoParam &VPPParams) const; // returns whether color space convert plugin 
                                                                    // should be use    

    static mfxU32 _stdcall  DeliverNextFrame(void* pvParam);

    mfxStatus AllocateBitstream(sOutputBitstream** pOutputBitstream, int bs_size);
    mfxStatus WipeBitstream(sOutputBitstream* pOutputBitstream);

    mfxStatus   SetExtFrameAllocator(MFXFrameAllocator* pAllocator)
    {
        // set allocator and device handle once
        if (!m_pExternalFrameAllocator)
        {
            m_pExternalFrameAllocator = pAllocator;
            mfxStatus sts = m_mfxVideoSession.SetFrameAllocator(m_pExternalFrameAllocator);
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
            
            D3DFrameAllocator* pAlloc = dynamic_cast<D3DFrameAllocator*> (m_pExternalFrameAllocator);
            if (pAlloc)
            {
                sts = m_mfxVideoSession.SetHandle(MFX_HANDLE_D3D9_DEVICE_MANAGER, pAlloc->GetDeviceManager());
                MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
            }
        }
        
        return MFX_ERR_NONE;
    }

    mfxStatus QueryIMPL(mfxIMPL *impl) 
    {
        mfxStatus sts = m_mfxVideoSession.QueryIMPL(impl); 

#ifdef MFX_EMULATE_HW
        *impl = MFX_IMPL_HARDWARE;
#endif
        return sts;
    };
    
protected:

    mfxVideoParam                   m_VideoParam;

    // parameters for VPP configuration
    CSCParam                        m_PluginParams;
    mfxVideoParam                   m_VideoParamVPP;
    mfxExtBuffer*                   m_pVppExtBuf;        // pointer to external buffer for VPP, stores the address of m_VppExtDoNotUse
    mfxExtVPPDoNotUse               m_VppExtDoNotUse;    // external buffer to configure VPP, used to turn off certain algorithms
    mfxU32                          m_tabDoNotUseAlg[4]; // list of vpp algorithms ID's (default algorithms will be turned off)

    // parameters for encoder configuration
    mfxExtBuffer*                   m_pCodingExtBuf;
    mfxExtCodingOption              m_CodingOption;

    MFXVideoSession                 m_mfxVideoSession;

    MFXVideoENCODE*                 m_pmfxENC;
    MFXVideoMultiVPP*               m_pmfxVPP;          

    BitstreamPool                   m_BitstreamPool;

    std::list<sMFXSample>           m_InputList; // input list
    std::list<sOutputBitstream *>   m_ResList; // output list
    mfxFrameSurface1**              m_ppFrameSurfaces; // frames shared by VPP output and ENC input
    mfxU16                          m_nRequiredFramesNum; // required number of frames
    
    MFXFrameAllocator*              m_pExternalFrameAllocator; 
    MFXFrameAllocator*              m_pInternalFrameAllocator;    
    mfxFrameAllocResponse           m_InternalAllocResponse; // allocation response for frames shared by VPP output and ENC input

    INotifier*                      m_pNotifier;

    HANDLE                          m_Thread;

    bool                            m_bStop;

    CRITICAL_SECTION                m_CriticalSection;

    bool                            m_bUseVPP;                  

    mfxU16                          m_nEncoderFramesNum;       // required number of frames for encoding
    bool                            m_bEncPartialAcceleration; 
    bool                            m_bInitialized;
    bool                            m_bSessionHasChild;
    bool                            m_bSurfaceStored; // flag shows that mfxSurface was added to input list

    bool m_bmfxSample;

    // Media SDK API version the object requires
    mfxU16 m_nAPIVerMinor;
    mfxU16 m_nAPIVerMajor;

    bool m_bForceGenKeyFrame; // by Liaokz

private:
    DISALLOW_COPY_AND_ASSIGN(CBaseEncoder);
};