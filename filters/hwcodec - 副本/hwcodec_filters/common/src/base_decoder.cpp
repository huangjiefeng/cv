/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "base_decoder.h"

CBaseDecoder::CBaseDecoder(mfxU16 APIVerMinor, mfxU16 APIVerMajor, mfxStatus *pSts) :
m_ppFrameSurfaces(NULL),
m_pmfxDEC(0),
m_nRequiredFramesNum(0),
m_pAllocExt(0),
m_bHeaderDecoded(false),
m_nCodecId(0),
m_pMfxFrameAllocator(NULL),
m_nAuxFramesNum(0),
m_pVideoParams(0),
m_bDecPartialAcceleration(false),
m_nAdditionalMemTypeFlags(0),
m_bSessionHasParent(false),
m_bd3dAlloc(false)
{
    mfxStatus sts = MFX_ERR_NONE;

    m_nAPIVerMinor = APIVerMinor;
    m_nAPIVerMajor = APIVerMajor;

    sts = SetAcceleration(MFX_IMPL_AUTO);

    if (pSts)
        *pSts = sts;

    memset(&m_AllocResponse, 0, sizeof(mfxFrameAllocResponse));

    return;
};

mfxStatus CBaseDecoder::SetAcceleration(mfxIMPL impl)
{
    mfxStatus sts = MFX_ERR_NONE;

    Close();

    m_mfxVideoSession.Close();

    mfxVersion version = {m_nAPIVerMinor, m_nAPIVerMajor};     
    sts = m_mfxVideoSession.Init(impl, &version);

    if (MFX_ERR_NONE == sts)
    {
        m_pmfxDEC = new MFXVideoDECODE(m_mfxVideoSession);

        if (!m_pmfxDEC)
        {
            sts = MFX_ERR_MEMORY_ALLOC;
        }
    }

    return sts;
};

mfxU8 CBaseDecoder::FindFreeSurface()
{
    mfxIMPL   impl;
    QueryIMPL(&impl);

    MSDK_CHECK_POINTER(m_ppFrameSurfaces, 0xFF);
    //10 seconds cycle
    for (int j = 0; j < 10000; j++)
    {
        for (mfxU8 i = 0; i < m_nRequiredFramesNum; i++)
        {
            if (!m_ppFrameSurfaces[i]->Data.Locked)
            {               
                // if a MediaSample is bound with mfxFrameSurface
                if (dynamic_cast<IMemAllocator*>(m_pMfxFrameAllocator))
                {
                    CMediaSample* pOutSample;
                    pOutSample = (CMediaSample*)m_ppFrameSurfaces[i]->Data.MemId;


                    if (1 < pOutSample->m_cRef)
                    {
                        continue;
                    }
                }

                return i;
            }
        }

        MFX_THREAD_WAIT;
    }

    return 0xFF;
};

CBaseDecoder::~CBaseDecoder()
{
    Close();

    MSDK_SAFE_DELETE(m_pmfxDEC);
};

mfxStatus CBaseDecoder::Close()
{
    mfxStatus sts = MFX_ERR_NONE;    

    if (m_pmfxDEC)
    {
        m_pmfxDEC->Close();
    }    

    FreeFrames();    

    return sts;
};

mfxStatus CBaseDecoder::AllocFrames(mfxVideoParam* pVideoParams, mfxU32 nPitch)
{
    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);
    MSDK_CHECK_POINTER(m_pMfxFrameAllocator, MFX_ERR_NOT_INITIALIZED);

    mfxStatus sts = MFX_ERR_NONE;
    mfxFrameAllocRequest allocRequest;

    mfxIMPL impl;
    QueryIMPL(&impl);

    FreeFrames();

    //prepare and create allocator
    memset(&allocRequest, 0, sizeof(allocRequest)); 

    sts =  m_pmfxDEC->QueryIOSurf(pVideoParams, &allocRequest);
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);   
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    allocRequest.NumFrameSuggested += m_nAuxFramesNum;
    m_nRequiredFramesNum = allocRequest.NumFrameSuggested;

    allocRequest.Type = MFX_MEMTYPE_EXTERNAL_FRAME | MFX_MEMTYPE_FROM_DECODE | m_nAdditionalMemTypeFlags;                
    allocRequest.Type |= m_bd3dAlloc ? MFX_MEMTYPE_VIDEO_MEMORY_DECODER_TARGET : MFX_MEMTYPE_SYSTEM_MEMORY;     

    memcpy(&allocRequest.Info, &pVideoParams->mfx.FrameInfo, sizeof(mfxFrameInfo));

    allocRequest.Info.Width = (mfxU16)nPitch;
    allocRequest.Info.Height = (allocRequest.Info.Height + 31) &~ 31; // allocate frames with H aligned at 32 for both progressive and interlaced content

    sts = m_pMfxFrameAllocator->Alloc(m_pMfxFrameAllocator->pthis, &allocRequest, &m_AllocResponse);
   
    if (MFX_ERR_NONE == sts)
    {
        m_ppFrameSurfaces = new mfxFrameSurface1 * [m_AllocResponse.NumFrameActual];
        MSDK_CHECK_POINTER(m_ppFrameSurfaces, MFX_ERR_NULL_PTR);

        m_nRequiredFramesNum = m_AllocResponse.NumFrameActual;

        if (m_ppFrameSurfaces)
        {
            for (mfxU32 i = 0; i < m_AllocResponse.NumFrameActual; i++)
            {
                m_ppFrameSurfaces[i] = new mfxFrameSurface1;
                BREAK_ON_EPOINTER(m_ppFrameSurfaces[i], sts);

                memset(m_ppFrameSurfaces[i], 0, sizeof(mfxFrameSurface1));
                memcpy(&(m_ppFrameSurfaces[i]->Info), &pVideoParams->mfx.FrameInfo, sizeof(mfxFrameInfo));   
                m_ppFrameSurfaces[i]->Info.Height = (m_ppFrameSurfaces[i]->Info.Height + 31) &~ 31;

                m_ppFrameSurfaces[i]->Data.MemId = m_AllocResponse.mids[i];                 
            }
        }
        else
        {
            sts = MFX_ERR_MEMORY_ALLOC;
        }
    }

    return sts;
};

mfxStatus CBaseDecoder::FreeFrames()
{
    if (m_pMfxFrameAllocator)
    {
        m_pMfxFrameAllocator->Free(m_pMfxFrameAllocator->pthis, &m_AllocResponse);
        memset(&m_AllocResponse, 0, sizeof(mfxFrameAllocResponse));        
    }

    if (m_ppFrameSurfaces)
    {
        for (mfxU32 i = 0; i < m_nRequiredFramesNum; i++)
        {
            MSDK_SAFE_DELETE(m_ppFrameSurfaces[i]);
        }
    }

    m_nRequiredFramesNum = 0;

    MSDK_SAFE_DELETE(m_ppFrameSurfaces);

    return MFX_ERR_NONE;
};

mfxStatus CBaseDecoder::InternalReset(mfxVideoParam* pVideoParams, mfxU32 nPitch, bool bInited)
{
    mfxStatus sts = MFX_ERR_NONE;
   
    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);
    
    m_nCodecId = pVideoParams->mfx.CodecId;
    m_pVideoParams = pVideoParams;

    if (bInited)
    {
        sts = m_pmfxDEC->Reset(pVideoParams);       

        // Reset could be not enough, then Close->Init should be called
        if (MFX_ERR_NONE != sts)
        {
            bInited = false;           
        }
    }

    if (!bInited)
    {
        m_pmfxDEC->Close();
        
        sts = AllocFrames(pVideoParams, nPitch);

        if (MFX_ERR_NONE == sts)
        {
            sts = m_pmfxDEC->Init(pVideoParams);            
        }
    }

    m_bDecPartialAcceleration = (MFX_WRN_PARTIAL_ACCELERATION == sts) ? true : m_bDecPartialAcceleration;
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_PARTIAL_ACCELERATION);
    MSDK_IGNORE_MFX_STS(sts, MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);

    return sts;
};

mfxStatus CBaseDecoder::Init(mfxVideoParam* pVideoParams, mfxU32 nPitch)
{  
    return InternalReset(pVideoParams, nPitch, false);     
};

mfxStatus CBaseDecoder::Reset(mfxVideoParam* pVideoParams, mfxU32 nPitch)
{
    return InternalReset(pVideoParams, nPitch, true);     
};

mfxStatus CBaseDecoder::GetVideoParams(mfxVideoParam* pVideoParams)
{
    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);

    return m_pmfxDEC->GetVideoParam(pVideoParams);
};

mfxStatus CBaseDecoder::GetDecodeStat(mfxDecodeStat* pVideoStat)
{
    MSDK_CHECK_POINTER(pVideoStat, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);

    return m_pmfxDEC->GetDecodeStat(pVideoStat);
};

mfxU32 CBaseDecoder::GetIOSurfNum(mfxVideoParam* pVideoParams)
{
    mfxFrameAllocRequest allocRequest;

    MSDK_CHECK_POINTER(pVideoParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);

    //prepare and create allocator
    memset(&allocRequest, 0, sizeof(allocRequest));

    m_pmfxDEC->QueryIOSurf(pVideoParams, &allocRequest);

    return allocRequest.NumFrameSuggested;
};

mfxStatus CBaseDecoder::RunDecode(mfxBitstream* pBS, mfxFrameSurface1* &pFrameSurface)
{
    mfxStatus sts = MFX_ERR_NONE;
    mfxSyncPoint syncp;

    MSDK_CHECK_POINTER(m_pmfxDEC, MFX_ERR_NOT_INITIALIZED);    

    mfxU8 nIndex = FindFreeSurface();

    while (0xFF == nIndex)
    {
        nIndex = FindFreeSurface();
        MFX_THREAD_WAIT;
    }

    do
    {
        sts = m_pmfxDEC->DecodeFrameAsync(pBS, m_ppFrameSurfaces[nIndex], &pFrameSurface, &syncp);
        
        if (MFX_WRN_DEVICE_BUSY == sts)
        {
            Sleep(1);
        }
    } while (MFX_WRN_DEVICE_BUSY == sts);

    // if decoder's session was joined to a downstream session synchronization is not needed
    if (MFX_ERR_NONE == sts && !m_bSessionHasParent) 
    {
        sts = m_mfxVideoSession.SyncOperation(syncp, 0xFFFF);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);            
    }
    else if (MFX_ERR_INCOMPATIBLE_VIDEO_PARAM == sts)
    {        
        m_bHeaderDecoded = false;
    }

    return sts;
};