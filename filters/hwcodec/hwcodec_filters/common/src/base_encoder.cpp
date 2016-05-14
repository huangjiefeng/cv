/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/
#include <assert.h>

#include "base_encoder.h"

#include "mfx_vpp_plugin.h"

CBaseEncoder::CBaseEncoder(mfxU16 APIVerMinor, mfxU16 APIVerMajor, mfxStatus *pSts) :
m_pmfxENC(0),
m_Thread(0),
m_bStop(false),
m_pExternalFrameAllocator(NULL),
m_pInternalFrameAllocator(NULL),
m_bUseVPP(false),
m_pmfxVPP(NULL),
m_ppFrameSurfaces(NULL),
m_nRequiredFramesNum(0),
m_nEncoderFramesNum(0),
m_bEncPartialAcceleration(false),
m_bInitialized(false),
m_bSessionHasChild(false),
m_bSurfaceStored(false),
m_bmfxSample(false),
m_bForceGenKeyFrame(false)
{    
    mfxStatus sts = MFX_ERR_NONE;

    m_nAPIVerMinor = APIVerMinor;
    m_nAPIVerMajor = APIVerMajor;
 //   sts = SetAcceleration(MFX_IMPL_AUTO);
 sts = SetAcceleration(MFX_IMPL_HARDWARE);

    if (pSts)
        *pSts = sts;

    memset(&m_InternalAllocResponse, 0, sizeof(mfxFrameAllocResponse));

    InitializeCriticalSectionAndSpinCount(&m_CriticalSection, 0x80000400);
  
    // initialize vpp configuration parameters
    // specify algorithms to be turned off
    m_tabDoNotUseAlg[0] = MFX_EXTBUFF_VPP_DENOISE; 
    m_tabDoNotUseAlg[1] = MFX_EXTBUFF_VPP_SCENE_ANALYSIS; 
    m_tabDoNotUseAlg[2] = MFX_EXTBUFF_VPP_DETAIL;
    m_tabDoNotUseAlg[3] = MFX_EXTBUFF_VPP_PROCAMP;
    // fill VPP external buffer structure
    m_VppExtDoNotUse.Header.BufferId = MFX_EXTBUFF_VPP_DONOTUSE;
    m_VppExtDoNotUse.Header.BufferSz = sizeof(mfxExtVPPDoNotUse);
    m_VppExtDoNotUse.NumAlg = 2;
    m_VppExtDoNotUse.AlgList = (mfxU32* )&m_tabDoNotUseAlg;
    // store the address of VPP external buffer structure
    m_pVppExtBuf = (mfxExtBuffer* )&m_VppExtDoNotUse; 
    
    // fill encoder external buffer structure
    memset(&m_CodingOption, 0, sizeof(m_CodingOption));    
    m_CodingOption.Header.BufferSz = sizeof(m_CodingOption);
    m_CodingOption.Header.BufferId = MFX_EXTBUFF_CODING_OPTION;
    m_CodingOption.MaxDecFrameBuffering = 1;
    // store the address of encoder external buffer structure
    m_pCodingExtBuf = (mfxExtBuffer* )&m_CodingOption;

    return;
};

mfxStatus CBaseEncoder::AllocVPPPlugin()
{
    mfxStatus sts = MFX_ERR_NONE;
    MFXVideoVPPPlugin *pmfxVPPPlugin = dynamic_cast<MFXVideoVPPPlugin *> (m_pmfxVPP);

    if (!pmfxVPPPlugin)
    {
        MSDK_SAFE_DELETE(m_pmfxVPP);
        pmfxVPPPlugin = new MFXVideoVPPPlugin(m_mfxVideoSession);
        MSDK_CHECK_POINTER(pmfxVPPPlugin, MFX_ERR_MEMORY_ALLOC);
        m_pmfxVPP = pmfxVPPPlugin;                
    }
    
    MSDK_CHECK_RESULT(sts = pmfxVPPPlugin->CreatePlugin(CreatePlugin), MFX_ERR_NONE, sts);    
    
    MFXFrameAllocator *pAlloc = m_pInternalFrameAllocator ? m_pInternalFrameAllocator : m_pExternalFrameAllocator;    
    if (pAlloc)
    {
        MSDK_CHECK_RESULT(sts = pmfxVPPPlugin->SetFrameAllocator(pAlloc), MFX_ERR_NONE, sts);                
    }    
    
    return sts;
}

mfxStatus CBaseEncoder::SetAcceleration(mfxIMPL impl)
{
    mfxStatus sts = MFX_ERR_NONE;

    MSDK_SAFE_DELETE(m_pmfxENC);
    MSDK_SAFE_DELETE(m_pmfxVPP);
    m_mfxVideoSession.Close();   
    
    mfxVersion version = {m_nAPIVerMinor, m_nAPIVerMajor};     
    sts = m_mfxVideoSession.Init(impl, &version);
    
    if (MFX_ERR_NONE == sts)
    {
        m_pmfxENC = new MFXVideoENCODE(m_mfxVideoSession);
        MSDK_CHECK_POINTER(m_pmfxENC, MFX_ERR_MEMORY_ALLOC); 

        m_pmfxVPP = new MFXVideoMultiVPP(m_mfxVideoSession);
        MSDK_CHECK_POINTER(m_pmfxVPP, MFX_ERR_MEMORY_ALLOC);
    }    

    return sts;
};

CBaseEncoder::~CBaseEncoder(void)
{
    Close();

    DeleteCriticalSection(&m_CriticalSection);  
    MSDK_SAFE_DELETE(m_pmfxENC);
    MSDK_SAFE_DELETE(m_pmfxVPP);
    MSDK_SAFE_DELETE(m_pInternalFrameAllocator);

    m_mfxVideoSession.Close();
};

mfxU8 CBaseEncoder::FindFreeSurfaceVPPOut()
{
    MSDK_CHECK_POINTER(m_ppFrameSurfaces, 0xFF);

    for (mfxU8 i = 0; i < m_nRequiredFramesNum; i++)
    {
        if (!m_ppFrameSurfaces[i]->Data.Locked)
        {
            return i;
        }
    }

    return 0xFF;
};

mfxStatus CBaseEncoder::Close()
{
    if (m_Thread)
    {
        m_bStop = true;
        WaitForSingleObject(m_Thread, INFINITE);
        CloseHandle(m_Thread);
        m_bStop = false;
        m_Thread = 0;
    }

    EnterCriticalSection(&m_CriticalSection);

    InternalClose();

    LeaveCriticalSection(&m_CriticalSection);

    return MFX_ERR_NONE;
};

mfxStatus CBaseEncoder::InternalClose()
{
    mfxStatus sts = MFX_ERR_NONE;    

    if (m_pmfxENC)
    {
        m_pmfxENC->Close();
    }

    if (m_pmfxVPP)
    {
        m_pmfxVPP->Close();
    }       

    // clear input list (will be empty in case session was joined)
    std::list<sMFXSample>::iterator jt;
    for (jt = m_InputList.begin(); jt != m_InputList.end(); jt++)
    {
        assert(!jt->pmfxSurface->Data.Locked); // all surface should be unlocked
        if (!jt->pmfxSurface->Data.Locked)
        {            
            MSDK_SAFE_DELETE(jt->pmfxSurface);          
            jt->pSample->Release();
        }
    }
    m_InputList.clear();

    MSDK_CHECK_RESULT(sts = m_BitstreamPool.FreeResources(), MFX_ERR_NONE, sts);

    m_ResList.clear();

    if (m_ppFrameSurfaces)
    {
        for (mfxU32 i = 0; i < m_nRequiredFramesNum; i++)
        {
            MSDK_SAFE_DELETE(m_ppFrameSurfaces[i]);
        }

        m_nRequiredFramesNum = 0;

        MSDK_SAFE_DELETE(m_ppFrameSurfaces);   
    }

    if (m_InternalAllocResponse.NumFrameActual)
    {
        MFXFrameAllocator *pAlloc = m_pInternalFrameAllocator ? m_pInternalFrameAllocator : m_pExternalFrameAllocator;
        MSDK_CHECK_POINTER(pAlloc, MFX_ERR_UNDEFINED_BEHAVIOR);
        sts = pAlloc->Free(pAlloc->pthis, &m_InternalAllocResponse);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);     
        memset(&m_InternalAllocResponse, 0, sizeof(mfxFrameAllocResponse));
    }   

    m_bInitialized = false;
    return sts;
};

mfxStatus CBaseEncoder::InternalReset(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams, bool bInited)
{
    mfxStatus               sts = MFX_ERR_NONE;
    mfxFrameAllocRequest    allocRequest;    
    mfxFrameAllocRequest    allocRequestVPP[2];
    MFXFrameAllocator *pAlloc = NULL;

    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);  
    MSDK_CHECK_POINTER(pVPPParams, MFX_ERR_NULL_PTR);  

    InternalClose();

    mfxIMPL impl;
    QueryIMPL(&impl);

    pVPPParams->AsyncDepth = pVideoParams->AsyncDepth;

    // save parameters and update IOPATTERN
    memcpy(&m_VideoParam, pVideoParams, sizeof(mfxVideoParam));
    memcpy(&m_VideoParamVPP, pVPPParams, sizeof(mfxVideoParam));    

    // check if preprocessing is required
    if (memcmp(&m_VideoParamVPP.vpp.In, &m_VideoParamVPP.vpp.Out, sizeof(mfxFrameInfo)))
    {
        memcpy(&m_VideoParam.mfx.FrameInfo, &m_VideoParamVPP.vpp.Out, sizeof(mfxFrameInfo));
        m_bUseVPP = true;

        m_VideoParamVPP.ExtParam = &m_pVppExtBuf; // attach external buffer to VPP mfxVideoParam         
        m_VideoParamVPP.NumExtParam = 1; // specify number of external buffers
    }
    else
    {
        m_bUseVPP = false;
    }
    
    if (impl == MFX_IMPL_HARDWARE && dynamic_cast<D3DFrameAllocator*> (m_pExternalFrameAllocator))
    {  
        m_VideoParamVPP.IOPattern = MFX_IOPATTERN_IN_VIDEO_MEMORY | MFX_IOPATTERN_OUT_VIDEO_MEMORY; 
        m_VideoParam.IOPattern = MFX_IOPATTERN_IN_VIDEO_MEMORY;
    }
    else
    {
        m_VideoParam.IOPattern    = MFX_IOPATTERN_IN_SYSTEM_MEMORY;
        m_VideoParamVPP.IOPattern = MFX_IOPATTERN_IN_SYSTEM_MEMORY | MFX_IOPATTERN_OUT_SYSTEM_MEMORY;        
    }
    
    // if VPP is required we need to allocate frames for sharing between vpp output and encoder input
    if (m_bUseVPP)
    {        
        // create own sys mem frame allocator (once) if no external allocator or if external is not d3d 
        if (!m_pExternalFrameAllocator && !m_pInternalFrameAllocator)
        {
            m_pInternalFrameAllocator = new SysMemFrameAllocator;
            MSDK_CHECK_POINTER(m_pInternalFrameAllocator, MFX_ERR_MEMORY_ALLOC);
            m_pInternalFrameAllocator->Init(NULL);
            sts = m_mfxVideoSession.SetFrameAllocator(m_pInternalFrameAllocator);
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);            
        }

        pAlloc = m_pInternalFrameAllocator ? m_pInternalFrameAllocator : m_pExternalFrameAllocator;
        MSDK_CHECK_POINTER(pAlloc, MFX_ERR_UNDEFINED_BEHAVIOR);

        // calculate required number of frames
        memset(&allocRequest, 0, sizeof(mfxFrameAllocRequest));
        memset(&allocRequestVPP, 0, 2*sizeof(mfxFrameAllocRequest));       

        if (UseCSCPlugin(m_VideoParamVPP))
        {
            MSDK_CHECK_RESULT(sts = AllocVPPPlugin(), MFX_ERR_NONE, sts);

            mfxVideoParam videoParamPlugin = m_VideoParamVPP;            
            videoParamPlugin.vpp.Out = videoParamPlugin.vpp.In;            

            mfxVideoParam videoParamVPP = m_VideoParamVPP;     

            videoParamPlugin.vpp.Out.FourCC = MFX_FOURCC_NV12;
            videoParamVPP.vpp.In.FourCC = MFX_FOURCC_NV12;
                        
            sts = m_pmfxVPP->QueryIOSurf(&videoParamPlugin, allocRequestVPP, 0, &videoParamVPP);                        
        }
        else
        {            
            sts = m_pmfxVPP->QueryIOSurf(&m_VideoParamVPP, allocRequestVPP);
        }
                
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);

        sts = m_pmfxENC->QueryIOSurf(&m_VideoParam, &allocRequest);
        m_bEncPartialAcceleration = (MFX_WRN_PARTIAL_ACCELERATION == sts) ? true : m_bEncPartialAcceleration;
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

        m_nRequiredFramesNum = max(allocRequestVPP[1].NumFrameSuggested, 1) +  max(allocRequest.NumFrameSuggested, 1);        

        // form allocation request
        allocRequest.NumFrameMin = allocRequest.NumFrameSuggested = m_nRequiredFramesNum;
        memcpy(&allocRequest.Info, &m_VideoParam.mfx.FrameInfo, sizeof(mfxFrameInfo)); 
        //allocRequest.Type = MFX_MEMTYPE_EXTERNAL_FRAME | MFX_MEMTYPE_FROM_VPPOUT | MFX_MEMTYPE_FROM_ENCODE;
        allocRequest.Type |= allocRequestVPP[1].Type;
   
        sts = pAlloc->Alloc(pAlloc->pthis, &allocRequest, &m_InternalAllocResponse);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);

        // init array of surfaces shared between VPP and ENCODE        
        m_ppFrameSurfaces = new mfxFrameSurface1 * [m_InternalAllocResponse.NumFrameActual];
        MSDK_CHECK_POINTER(m_ppFrameSurfaces, MFX_ERR_MEMORY_ALLOC);

        for (mfxU32 i = 0; i < m_InternalAllocResponse.NumFrameActual; i++)
        {
            m_ppFrameSurfaces[i] = new mfxFrameSurface1;
            BREAK_ON_EPOINTER(m_ppFrameSurfaces[i], sts);

            memset(m_ppFrameSurfaces[i], 0, sizeof(mfxFrameSurface1));
            memcpy(&(m_ppFrameSurfaces[i]->Info), &m_VideoParam.mfx.FrameInfo, sizeof(mfxFrameInfo));
         
            m_ppFrameSurfaces[i]->Data.MemId = m_InternalAllocResponse.mids[i];                            
        }                
    }

    if (MFX_ERR_NONE == sts)
    {
        mfxVideoParam InitParams;
        memset(&InitParams, 0, sizeof(mfxVideoParam));
        InitParams.mfx.CodecId = m_VideoParam.mfx.CodecId;
        
        sts = m_pmfxENC->Query(&m_VideoParam, &InitParams);
        m_bEncPartialAcceleration = (MFX_WRN_PARTIAL_ACCELERATION == sts) ? true : m_bEncPartialAcceleration;
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);    

        if (MFX_ERR_NONE == sts)
        {            
            if (1 == m_VideoParam.AsyncDepth && 1 == m_VideoParam.mfx.GopRefDist)
            {
                //when AsyncDepth = 1 and m_VideoParam.mfx.GopRefDist = 1, 
                //encoder is expected to work in low latency mode
                //need to add MFX_EXTBUFF_CODING_OPTION external buffer with MaxDecFrameBuffering = 1
                //to use optimal parameters
                InitParams.ExtParam = &m_pCodingExtBuf;
                InitParams.NumExtParam = 1;
            }
            
            sts = m_pmfxENC->Init(&InitParams);
            m_bEncPartialAcceleration = (MFX_WRN_PARTIAL_ACCELERATION == sts) ? true : m_bEncPartialAcceleration;
            MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
            MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

            if (MFX_ERR_NONE == sts)
            {
                //a trick to enable dynamic bitrate change
                if (MFX_ERR_NONE == sts && MFX_RATECONTROL_VBR == InitParams.mfx.RateControlMethod)
                {
                    InitParams.mfx.TargetKbps += 10;
                    InitParams.mfx.MaxKbps += 10;
                    sts = m_pmfxENC->Reset(&InitParams);
                    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
                    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

                    if (MFX_ERR_NONE == sts)
                    {
                        InitParams.mfx.TargetKbps -= 10;
                        InitParams.mfx.MaxKbps -= 10;
                        sts = m_pmfxENC->Reset(&InitParams);
                        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
                        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);
                    }
                }

                if (MFX_ERR_NONE == sts)
                {
                    sts = m_pmfxENC->GetVideoParam(pVideoParams);
                }
            }
        }                           
    }

    if (MFX_ERR_NONE == sts && m_bUseVPP)
    {
        if (UseCSCPlugin(m_VideoParamVPP))
        {
            mfxVideoParam videoParamPlugin = m_VideoParamVPP;            
            videoParamPlugin.vpp.Out = videoParamPlugin.vpp.In;

            mfxVideoParam videoParamVPP = m_VideoParamVPP;     

            videoParamPlugin.vpp.Out.FourCC = MFX_FOURCC_NV12;
            videoParamVPP.vpp.In.FourCC = MFX_FOURCC_NV12;            
            
            sts = m_pmfxVPP->Init(&videoParamPlugin, 0, &videoParamVPP);            
        }
        else
        {            
            sts = m_pmfxVPP->Init(&m_VideoParamVPP);
        }        
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);
        MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
    }   

    if (MFX_ERR_NONE == sts)
    {
        // capacity of bitstream pool is same as number of frame buffers
        m_BitstreamPool.setCapacity(m_nRequiredFramesNum ? m_nRequiredFramesNum : m_nEncoderFramesNum);        
    }

    if (MFX_ERR_NONE == sts)
    {
        m_Thread = (HANDLE)_beginthreadex(0, 0, DeliverNextFrame, this, 0, 0);

        if (0 == m_Thread)
        {
            sts = MFX_ERR_UNKNOWN;
        }
    }

    if (MFX_ERR_NONE != sts)
    {
        InternalClose();
    }
    m_bInitialized = true;

    return sts;
};

mfxU32 _stdcall CBaseEncoder::DeliverNextFrame(void* pvParam)
{
    mfxStatus     sts       = MFX_ERR_NONE;
    CBaseEncoder* pEncoder  = reinterpret_cast<CBaseEncoder *> (pvParam);

    std::list<sOutputBitstream *>::iterator it;
    std::list<sMFXSample>::iterator jt;

    MSDK_CHECK_POINTER(pEncoder, 1);

    for(;!pEncoder->m_bStop;)
    {        
        EnterCriticalSection(&pEncoder->m_CriticalSection);
        
        // Check if some of the input samples can be released
        // InputList will be empty in case session was joined
        for (jt = pEncoder->m_InputList.begin(); jt != pEncoder->m_InputList.end(); jt++)
        {
            if (!jt->pmfxSurface->Data.Locked)
            {               
                MSDK_SAFE_DELETE(jt->pmfxSurface);     
                jt->pSample->Release();                                    
                pEncoder->m_InputList.erase(jt);            

                break;
            }
        }                 

        if (!pEncoder->m_ResList.empty())
        {
            it = pEncoder->m_ResList.begin();

            sts = pEncoder->m_mfxVideoSession.SyncOperation(*((*it)->psyncP), 0);

            LeaveCriticalSection(&pEncoder->m_CriticalSection);

            if ((MFX_ERR_NONE != sts && MFX_WRN_IN_EXECUTION != sts && MFX_ERR_MORE_DATA != sts) || pEncoder->m_bStop)
            {
                pEncoder->m_pmfxENC->Close();

                //need to free 1 element, to make next RunEncode possible, to get higher level know about error
                EnterCriticalSection(&pEncoder->m_CriticalSection);

                if (!pEncoder->m_InputList.empty()) 
                {
                    jt = pEncoder->m_InputList.begin();                    
                    MSDK_SAFE_DELETE(jt->pmfxSurface);                    
                    jt->pSample->Release();
                    pEncoder->m_InputList.erase(jt);                    
                }

                LeaveCriticalSection(&pEncoder->m_CriticalSection);
                
                pEncoder->m_bStop = true;
                return 1;
            }

            if (MFX_ERR_NONE == sts)
            {
                sts = pEncoder->m_pNotifier->OnFrameReady((*it)->pmfxBS);
                MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, 1);
            }

            if (MFX_ERR_NONE == sts || MFX_ERR_MORE_DATA == sts)
            {
                EnterCriticalSection(&pEncoder->m_CriticalSection);
                
                pEncoder->m_BitstreamPool.deleteBitstream(*it);

                pEncoder->m_ResList.pop_front();

                LeaveCriticalSection(&pEncoder->m_CriticalSection);
            }
        }
        else
        {
            LeaveCriticalSection(&pEncoder->m_CriticalSection);
        }

        if (MFX_WRN_IN_EXECUTION == sts || pEncoder->m_ResList.empty())
        {
            MFX_THREAD_WAIT;
        }            
    }

    return 0xedf;
};

mfxStatus CBaseEncoder::Init(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams, INotifier* pNotifier)
{
    mfxStatus sts = MFX_ERR_NONE;

    if (m_Thread)
    {
        m_bStop = true;
        WaitForSingleObject(m_Thread, INFINITE);
        CloseHandle(m_Thread);
        m_bStop = false;
        m_Thread = 0;
    }

    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);        

    EnterCriticalSection(&m_CriticalSection);

    if (MFX_ERR_NONE == sts)
    {
        sts = InternalReset(pVideoParams, pVPPParams, false);
    }

    if (MFX_ERR_NONE == sts)
    {
        m_pNotifier = pNotifier;
    }

    if (MFX_ERR_NONE != sts)
    {
        InternalClose();
    }

    m_VideoParam = *pVideoParams;

    LeaveCriticalSection(&m_CriticalSection);

    return sts;        
};

mfxStatus CBaseEncoder::DynamicBitrateChange(mfxVideoParam* pVideoParams)
{
    mfxStatus sts = MFX_ERR_NONE;

    if (m_bInitialized)
    {
        m_VideoParam.mfx.TargetKbps = 
            m_VideoParam.mfx.MaxKbps = pVideoParams->mfx.TargetKbps;

        if (MFX_ERR_NONE == sts)
        {
            BOOL bExtParamsAdded = FALSE;
            if (1 == m_VideoParam.AsyncDepth && 1 == m_VideoParam.mfx.GopRefDist)
            {
                //when AsyncDepth = 1 and m_VideoParam.mfx.GopRefDist = 1, 
                //encoder is expected to work in low latency mode
                //need to add MFX_EXTBUFF_CODING_OPTION external buffer with MaxDecFrameBuffering = 1
                //to use optimal parameters
                m_VideoParam.ExtParam = &m_pCodingExtBuf;
                m_VideoParam.NumExtParam = 1;

                bExtParamsAdded = TRUE;
            }

            sts = m_pmfxENC->Reset(&m_VideoParam);
            m_bEncPartialAcceleration = (MFX_WRN_PARTIAL_ACCELERATION == sts) ? true : m_bEncPartialAcceleration;
            MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
            MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);    

            if (MFX_ERR_NONE == sts)
            {
                sts = m_pmfxENC->GetVideoParam(pVideoParams);                
            }

            if (bExtParamsAdded)
            {
                m_VideoParam.NumExtParam = 0;
                m_VideoParam.ExtParam = NULL;
            }
        }        
    }

    return sts;
};

mfxStatus CBaseEncoder::Reset(mfxVideoParam* pVideoParams, mfxVideoParam* pVPPParams)
{
    mfxStatus sts = MFX_ERR_NONE;

    if (m_Thread)
    {
        m_bStop = true;
        WaitForSingleObject(m_Thread, INFINITE);
        CloseHandle(m_Thread);
        m_bStop = false;
        m_Thread = 0;
    }

    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR); 
    MSDK_CHECK_POINTER(m_pmfxENC, MFX_ERR_NOT_INITIALIZED);

    EnterCriticalSection(&m_CriticalSection);

    sts = InternalReset(pVideoParams, pVPPParams, true);

    if (MFX_ERR_NONE != sts)
    {
        InternalClose();
    }

    LeaveCriticalSection(&m_CriticalSection);

    return sts;
};

bool CBaseEncoder::UseCSCPlugin(const mfxVideoParam& VPPParams) const
{
    VPPParams; return (m_PluginParams.vOrientation == CSCParam::BOTTOM_UP);
}

mfxStatus CBaseEncoder::GetVideoParams(mfxVideoParam* pVideoParams)
{
    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxENC, MFX_ERR_NOT_INITIALIZED);

    return m_pmfxENC->GetVideoParam(pVideoParams);
};

mfxStatus CBaseEncoder::GetEncodeStat(mfxEncodeStat* pVideoStat)
{
    MSDK_CHECK_POINTER(pVideoStat, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxENC, MFX_ERR_NOT_INITIALIZED);

    return m_pmfxENC->GetEncodeStat(pVideoStat);
};

mfxStatus CBaseEncoder::AllocateBitstream(sOutputBitstream** pOutputBitstream, int bs_size)
{
    MSDK_CHECK_POINTER(pOutputBitstream, MFX_ERR_NULL_PTR);

    int sleep_fund = MSDK_WAIT_INTERVAL;

    for (bool allocated = false; !allocated; 
         // wait if no bitstreams are available
         allocated = *pOutputBitstream ? true : (MFX_THREAD_WAIT, sleep_fund-=MFX_THREAD_WAIT_TIME, false))
    {
        EnterCriticalSection(&m_CriticalSection);
        m_BitstreamPool.newBitstream(pOutputBitstream, bs_size);
        LeaveCriticalSection(&m_CriticalSection);

        if (sleep_fund <= 0) // if we waited for too long
            return MFX_ERR_UNDEFINED_BEHAVIOR;
    }

    return MFX_ERR_NONE;
}

mfxStatus CBaseEncoder::WipeBitstream(sOutputBitstream* pOutputBitstream)
{
    MSDK_CHECK_POINTER(pOutputBitstream, MFX_ERR_NULL_PTR);

    EnterCriticalSection(&m_CriticalSection);
    m_BitstreamPool.deleteBitstream(pOutputBitstream);
    LeaveCriticalSection(&m_CriticalSection);

    return MFX_ERR_NONE;
}

mfxStatus CBaseEncoder::RunEncode(IUnknown *pSample, mfxFrameSurface1* pFrameSurface)
{
    mfxStatus       sts = MFX_ERR_NONE;

    MSDK_CHECK_POINTER(m_pmfxENC, MFX_ERR_NOT_INITIALIZED);
    MSDK_CHECK_POINTER(m_pmfxVPP, MFX_ERR_NOT_INITIALIZED);   

    mfxU16 nIndex = 0xFF; 
    
    if (m_bUseVPP)
    {
        nIndex = FindFreeSurfaceVPPOut();
        while (0xFF == nIndex && !m_bStop)
        {
            nIndex = FindFreeSurfaceVPPOut();
            MFX_THREAD_WAIT;
        }
    }
    
    if (m_bStop)
    {      
        return MFX_ERR_ABORTED;
    }    

    sOutputBitstream *CurrentFrame;
    mfxU64 nBSSize = m_VideoParam.mfx.BufferSizeInKB ? 
                     m_VideoParam.mfx.BufferSizeInKB * 1000 : 
                     m_VideoParam.mfx.FrameInfo.Width * m_VideoParam.mfx.FrameInfo.Height * 4;

    sts = AllocateBitstream(&CurrentFrame, (int)nBSSize);
    MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, sts, WipeBitstream(CurrentFrame));    

    // by Liaokz
    mfxEncodeCtrl * ctrl = NULL;
    if (m_bForceGenKeyFrame)
    {
        ctrl = new mfxEncodeCtrl;
        memset(ctrl, 0, sizeof(mfxEncodeCtrl));
        ctrl->FrameType = MFX_FRAMETYPE_I | MFX_FRAMETYPE_REF | MFX_FRAMETYPE_IDR;
        m_bForceGenKeyFrame = false;
    }
    
    if (m_bUseVPP)
    {
        mfxSyncPoint syncP = {0};
        
        do
        {
            sts = m_pmfxVPP->RunFrameVPPAsync(pFrameSurface, m_ppFrameSurfaces[nIndex], 0, &syncP);
            MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

            if (MFX_WRN_DEVICE_BUSY == sts)
            {
                Sleep(1);                    
            }         
        } while (sts == MFX_WRN_DEVICE_BUSY);        
        
        if (MFX_ERR_NONE == sts)
        {
            m_ppFrameSurfaces[nIndex]->Data.TimeStamp = pFrameSurface->Data.TimeStamp;
            do 
            {
                sts = m_pmfxENC->EncodeFrameAsync(ctrl, m_ppFrameSurfaces[nIndex], CurrentFrame->pmfxBS, CurrentFrame->psyncP);
                if (MFX_WRN_DEVICE_BUSY == sts)
                {
                    Sleep(1);                    
                }         
            } while (sts == MFX_WRN_DEVICE_BUSY);
            
        }
        else if (MFX_ERR_MORE_DATA == sts)
        {
            do 
            {
                sts = m_pmfxENC->EncodeFrameAsync(ctrl, NULL, CurrentFrame->pmfxBS, CurrentFrame->psyncP);
                if (MFX_WRN_DEVICE_BUSY == sts)
                {
                    Sleep(1);                    
                }         
            } while (sts == MFX_WRN_DEVICE_BUSY);
        }                 
    }
    else
    {
        do 
        {
            sts = m_pmfxENC->EncodeFrameAsync(ctrl, pFrameSurface, CurrentFrame->pmfxBS, CurrentFrame->psyncP);
            if (MFX_WRN_DEVICE_BUSY == sts)
            {
                Sleep(1);                    
            }     
        } while (sts == MFX_WRN_DEVICE_BUSY);           
    }

    MSDK_SAFE_DELETE(ctrl);

    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

    if (MFX_ERR_NONE == sts)
    {
        EnterCriticalSection(&m_CriticalSection);
        m_ResList.push_back(CurrentFrame);
        LeaveCriticalSection(&m_CriticalSection);
    }   
    else
    {
        WipeBitstream(CurrentFrame);
    }

    // hold MediaSamples in the case when encoder surfaces uses their memory 
    if (pFrameSurface && pFrameSurface->Data.Locked && !m_bmfxSample)
    {
        // add to InputList
        pSample->AddRef();
        sMFXSample input_sample;
        input_sample.pSample = pSample;
        input_sample.pmfxSurface = pFrameSurface;

        EnterCriticalSection(&m_CriticalSection);
        m_InputList.push_back(input_sample);
        LeaveCriticalSection(&m_CriticalSection);        

        m_bSurfaceStored = true; // mark that caller function must not free surface memory
    }      
    
    if (NULL == pFrameSurface && MFX_ERR_MORE_DATA == sts)
    {
        while (m_ResList.size() && !m_bStop)
        {
            MFX_THREAD_WAIT;
        }
    }
    
    return sts;
};