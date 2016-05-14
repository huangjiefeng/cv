//* ////////////////////////////////////////////////////////////////////////////// */
//*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2011 Intel Corporation. All Rights Reserved.
//
//
//*/

#include <stdio.h>
#include <utility>
#include <algorithm>
#include "plugin_csc.h"

// disable "unreferenced formal parameter" warning
#pragma warning(disable : 4100) 

using namespace std;

/* CSCPlugin class implementation */
CSCPlugin::CSCPlugin() 
    : m_pTasks(NULL)
    , m_bInited(false)
    , m_pAlloc()
{
    m_MaxNumTasks = 0;
    
    memset(&m_VideoParam, 0, sizeof(m_VideoParam));    

    m_pmfxCore = NULL;
    memset(&m_PluginParam, 0, sizeof(m_PluginParam));
    m_PluginParam.MaxThreadNum = 1;
    m_PluginParam.ThreadPolicy = MFX_THREADPOLICY_SERIAL;
}

CSCPlugin::~CSCPlugin()
{    
    mfxPluginClose();   
    Close();    
}

/* Methods required for integration with Media SDK */
mfxStatus CSCPlugin::mfxPluginInit(mfxCoreInterface *core)
{
    MSDK_CHECK_POINTER(core, MFX_ERR_NULL_PTR);       
        
    MSDK_SAFE_DELETE(m_pmfxCore);

    m_pmfxCore = new mfxCoreInterface; 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_MEMORY_ALLOC);
    *m_pmfxCore = *core;  

    // if external allocator not set use the one from core interface
    if (!m_pAlloc && m_pmfxCore->FrameAllocator.pthis)
        m_pAlloc = &m_pmfxCore->FrameAllocator;

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::mfxPluginClose()
{
    MSDK_SAFE_DELETE(m_pmfxCore);
 
    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::mfxGetPluginParam(mfxPluginParam *par)
{
    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);    

    *par = m_PluginParam;

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::mfxSubmit(const mfxHDL *in, mfxU32 in_num, const mfxHDL *out, mfxU32 out_num, mfxThreadTask *task)
{
    MSDK_CHECK_POINTER(in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(out, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(*in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(*out, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(task, MFX_ERR_NULL_PTR);    
    MSDK_CHECK_NOT_EQUAL(in_num, 1, MFX_ERR_UNSUPPORTED);
    MSDK_CHECK_NOT_EQUAL(out_num, 1, MFX_ERR_UNSUPPORTED);
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED);  

    mfxFrameSurface1 *surface_in = (mfxFrameSurface1 *)in[0];
    mfxFrameSurface1 *surface_out = (mfxFrameSurface1 *)out[0]; 
    
    mfxStatus sts = MFX_ERR_NONE;
    
    // check validity of parameters
    sts = CheckInOutFrameInfo(&surface_in->Info, &surface_out->Info);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
           
    mfxU32 ind = FindFreeTaskIdx();

    if (ind >= m_MaxNumTasks)
    {
        return MFX_WRN_DEVICE_BUSY; // currently there are no free tasks available
    }
    
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(surface_in->Data));
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(surface_out->Data));

    m_pTasks[ind].In = surface_in;
    m_pTasks[ind].Out = surface_out;
    m_pTasks[ind].bBusy = true;

    m_pTasks[ind].pProcessor = new CSCProcessor;
    MSDK_CHECK_POINTER(m_pTasks[ind].pProcessor, MFX_ERR_MEMORY_ALLOC);
    
    m_pTasks[ind].pProcessor->SetAllocator(m_pAlloc);
    m_pTasks[ind].pProcessor->Init(surface_in, surface_out);
    
    *task = (mfxThreadTask)&m_pTasks[ind];    

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::mfxExecute(mfxThreadTask task, mfxU32 uid_p, mfxU32 uid_a)
{    
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED);
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    mfxStatus sts = MFX_ERR_NONE;
    CSCTask *current_task = (CSCTask *)task;    

    // 0,...,NumChunks - 2 calls return TASK_WORKING, NumChunks - 1,.. return TASK_DONE
    if (uid_a < m_NumChunks)
    {
        // there's data to process
        sts = current_task->pProcessor->Process(&m_pChunks[uid_a]);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
        // last call?
        sts = ((m_NumChunks - 1) == uid_a) ? MFX_TASK_DONE : MFX_TASK_WORKING;
    }
    else
    {
        // no data to process
        sts =  MFX_TASK_DONE;
    }

    return sts;        
}

mfxStatus CSCPlugin::mfxFreeResources(mfxThreadTask task, mfxStatus sts)
{
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED); 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    CSCTask *current_task = (CSCTask *)task;
    
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->In->Data));
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->Out->Data));
    current_task->bBusy = false;
    MSDK_SAFE_DELETE(current_task->pProcessor);

    return MFX_ERR_NONE;
}

/* Custom methods */
mfxStatus CSCPlugin::Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize)
{
    MSDK_CHECK_POINTER(mfxParam, MFX_ERR_NULL_PTR); 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NULL_PTR);

    // check validity of parameters
    mfxStatus sts = CheckParam(mfxParam);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
    
    m_VideoParam = *mfxParam;
        
    m_MaxNumTasks = m_VideoParam.AsyncDepth;
    if (m_MaxNumTasks < 2) m_MaxNumTasks = 2;

    m_pTasks = new CSCTask [m_MaxNumTasks];
    MSDK_CHECK_POINTER(m_pTasks, MFX_ERR_MEMORY_ALLOC);
    memset(m_pTasks, 0, sizeof(CSCTask) * m_MaxNumTasks);

    m_NumChunks = m_PluginParam.MaxThreadNum;
    m_pChunks = new DataChunk [m_NumChunks];
    MSDK_CHECK_POINTER(m_pChunks, MFX_ERR_MEMORY_ALLOC);
    memset(m_pChunks, 0, sizeof(DataChunk) * m_NumChunks);

    // divide frame into data chunks
    mfxU32 num_lines_in_chunk = mfxParam->vpp.In.CropH / m_NumChunks; // integer division
    mfxU32 remainder_lines = mfxParam->vpp.In.CropH % m_NumChunks; // get remainder
    // remaining lines are distributed among first chunks (+ extra 1 line each)
    for (mfxU32 i = 0; i < m_NumChunks; i++)
    {
        m_pChunks[i].StartLine = (i == 0) ? 0 : m_pChunks[i-1].EndLine + 1; 
        m_pChunks[i].EndLine = (i < remainder_lines) ? (i + 1) * num_lines_in_chunk : (i + 1) * num_lines_in_chunk - 1;
    }   

    m_bInited = true;

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::Close()
{
    
    if (!m_bInited)
        return MFX_ERR_NONE;      

    MSDK_SAFE_DELETE_ARRAY(m_pTasks); 

    mfxStatus sts = MFX_ERR_NONE;
    
    m_bInited = false;

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2])
{
    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);

    request[0].Info = par->vpp.In;
    request[0].NumFrameMin = 1;
    request[0].NumFrameSuggested = request[0].NumFrameMin + par->AsyncDepth;

    request[1].Info = par->vpp.Out;
    request[1].NumFrameMin = 1;
    request[1].NumFrameSuggested = request[1].NumFrameMin + par->AsyncDepth;

    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::SetAllocator(mfxFrameAllocator * pAlloc)
{
    m_pAlloc = pAlloc;
    return MFX_ERR_NONE;
}

mfxStatus CSCPlugin::SetHandle(mfxHandleType /*type*/, mfxHDL /*hdl*/)
{
    //dummy function
    return MFX_ERR_NONE;
}


/* Internal methods */
mfxU32 CSCPlugin::FindFreeTaskIdx()
{   
    mfxU32 i;
    for (i = 0; i < m_MaxNumTasks; i++)
    {
        if (false == m_pTasks[i].bBusy)
        {
            break;
        }
    }

    return i;
}

mfxStatus CSCPlugin::CheckParam(mfxVideoParam *mfxParam)
{
    MSDK_CHECK_POINTER(mfxParam, MFX_ERR_NULL_PTR);      

    mfxInfoVPP *pParam = &mfxParam->vpp;

    if (MFX_FOURCC_RGB4 == pParam->In.FourCC && MFX_FOURCC_NV12 == pParam->Out.FourCC)
    {
        return MFX_ERR_NONE;
    }  

    return MFX_ERR_INVALID_VIDEO_PARAM;
}

mfxStatus CSCPlugin::CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut)
{
    MSDK_CHECK_POINTER(pIn, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pOut, MFX_ERR_NULL_PTR);

    if (pIn->CropW != m_VideoParam.vpp.In.CropW || pIn->CropH != m_VideoParam.vpp.In.CropH || 
        pIn->FourCC != m_VideoParam.vpp.In.FourCC ||
        pOut->CropW != m_VideoParam.vpp.Out.CropW || pOut->CropH != m_VideoParam.vpp.Out.CropH || 
        pOut->FourCC != m_VideoParam.vpp.Out.FourCC)
    {
        return MFX_ERR_INVALID_VIDEO_PARAM;
    }

    return MFX_ERR_NONE;
}

/* Processor class implementation */
CSCProcessor::CSCProcessor() 
    : m_pIn(NULL)
    , m_pOut(NULL)
    , m_pAlloc(NULL)
{
}

CSCProcessor::~CSCProcessor()
{
}

mfxStatus CSCProcessor::SetAllocator(mfxFrameAllocator *pAlloc)
{
    m_pAlloc = pAlloc;
    return MFX_ERR_NONE;
}

mfxStatus CSCProcessor::Init(mfxFrameSurface1 *frame_in, mfxFrameSurface1 *frame_out)
{
    MSDK_CHECK_POINTER(frame_in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(frame_out, MFX_ERR_NULL_PTR);

    m_pIn = frame_in;
    m_pOut = frame_out;    

    return MFX_ERR_NONE;        
}

mfxStatus CSCProcessor::LockFrame(mfxFrameSurface1 *frame)
{
    MSDK_CHECK_POINTER(frame, MFX_ERR_NULL_PTR);
    //double lock impossible
    if (frame->Data.Y != 0 && frame->Data.MemId !=0)
        return MFX_ERR_UNSUPPORTED;
    //no allocator used, no need to do lock
    if (frame->Data.Y != 0)
        return MFX_ERR_NONE;
    //lock required
    MSDK_CHECK_POINTER(m_pAlloc, MFX_ERR_NULL_PTR);
    return m_pAlloc->Lock(m_pAlloc->pthis, frame->Data.MemId, &frame->Data);
}

mfxStatus CSCProcessor::UnlockFrame(mfxFrameSurface1 *frame)
{
    MSDK_CHECK_POINTER(frame, MFX_ERR_NULL_PTR);
    //unlock not possible, no allocator used
    if (frame->Data.Y != 0 && frame->Data.MemId ==0)
        return MFX_ERR_NONE;
    //already unlocked
    if (frame->Data.Y == 0)
        return MFX_ERR_NONE;
    //unlock required
    MSDK_CHECK_POINTER(m_pAlloc, MFX_ERR_NULL_PTR);
    return m_pAlloc->Unlock(m_pAlloc->pthis, frame->Data.MemId, &frame->Data);
}

void CSCProcessor::ownBGRToYCbCr420_8u_AC4P3R( const mfxU8* pSrc, 
                                 int srcStep, 
                                 mfxU8* pDst[2],
                                 int dstStep[2], 
                                 std::pair<int,int> roiSize )
{
    mfxStatus sts = MFX_ERR_NONE;
    int h,w;
    int dstStepY = dstStep[0];
    int width2  = roiSize.first  & ~1;
    int height2 = roiSize.second >> 1;
    mfxU8* pDstUV = pDst[1];  
    
    const mfxI32 kry0 = 0x000041cb;
    const mfxI32 kry1 = 0x00008106;
    const mfxI32 kry2 = 0x00001917;
    const mfxI32 kry3 = 0x000025e3;
    const mfxI32 kry4 = 0x00004a7f;
    const mfxI32 kry5 = 0x00007062;
    const mfxI32 kry6 = 0x00005e35;
    const mfxI32 kry7 = 0x0000122d;

    for( h = 0; h < height2; h++ )
    {
        const mfxU8* src;
        mfxU8* dsty,*dstu,*dstv;
        src  = pSrc   + h * 2 * srcStep;
        dsty = pDst[0]+ h * 2 * dstStepY;

        dstu = pDstUV  + h * dstStep[1];
        dstv = dstu + 1;

        for( w = 0; w < width2; w += 2 )
        {
            int g,g1,g2,g3;
            int r,r1,r2,r3;
            int b,b1,b2,b3;
            b = src[0];g = src[1];r = src[2];
            b1= src[4];g1= src[5];r1= src[6];
            b2= src[0+srcStep];g2= src[1+srcStep];r2= src[2+srcStep];
            b3= src[4+srcStep];g3= src[5+srcStep];r3= src[6+srcStep];
            src += 8;
            dsty[0] = ( mfxU8 )(( kry0 * r  + kry1 *  g + kry2 *  b + 0x108000) >> 16 );
            dsty[1] = ( mfxU8 )(( kry0 * r1 + kry1 * g1 + kry2 * b1 + 0x108000) >> 16 );
            dsty[0+dstStepY] = ( mfxU8 )(( kry0 * r2 + kry1 * g2 + kry2 * b2 + 0x108000) >> 16 );
            dsty[1+dstStepY] = ( mfxU8 )(( kry0 * r3 + kry1 * g3 + kry2 * b3 + 0x108000) >> 16 );
            dsty += 2;
            r += r1;r += r2;r += r3;
            b += b1;b += b2;b += b3;
            g += g1;g += g2;g += g3;

            *dstu = ( mfxU8 )((-kry3 * r - kry4 * g + kry5 * b + 0x2008000)>> 18 ); // Cb 
            *dstv = ( mfxU8 )(( kry5 * r - kry6 * g - kry7 * b + 0x2008000)>> 18 ); // Cr 

            dstu += 2;
            dstv += 2;
        }    
    } 
} 

mfxStatus CSCProcessor::RGB4_to_NV12( mfxFrameData* inData,  mfxFrameInfo* inInfo,
                                      mfxFrameData* outData, mfxFrameInfo* outInfo,                       
                                      DataChunk *chunk)
{  
    std::pair<int, int>  roiSize(0,0);
    mfxU16  cropX = 0, cropY = 0;

    mfxU32  inOffset0 = 0;
    mfxU32  outOffset0 = 0, outOffset1 = 0;

    roiSize.first = inInfo->CropW;
    roiSize.second = chunk->EndLine - chunk->StartLine + 1;

    cropX = inInfo->CropX; 
    cropY = inInfo->CropY;

    // start offset in input frame
    inOffset0  = cropX*4  + (inInfo->CropH - 1 - chunk->StartLine)*inData->Pitch;  

    cropX = outInfo->CropX; 
    cropY = outInfo->CropY;

    outOffset0  = cropX + cropY*outData->Pitch;
    outOffset1  = (cropX) + (cropY>>1)*(outData->Pitch);

    MSDK_CHECK_POINTER(inData->R, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(inData->G, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(inData->B, MFX_ERR_NULL_PTR);  
    
    mfxU8* ptrStart = min( min(inData->R, inData->G), inData->B );

    const mfxU8* pSrc[1] = {ptrStart + inOffset0};

    mfxI32 pSrcStep[1] = {inData->Pitch >> 0};

    mfxU8* pDst[2]   = {(mfxU8*)outData->Y + outOffset0, (mfxU8*)outData->UV + outOffset1};

    mfxI32 pDstStep[2] = {outData->Pitch, outData->Pitch >> 0};

    // negative step to go in bottom-up direction
    ownBGRToYCbCr420_8u_AC4P3R( pSrc[0], -pSrcStep[0], pDst, pDstStep, roiSize );

    return MFX_ERR_NONE;
}

mfxStatus CSCProcessor::Process(DataChunk *chunk)
{
    MSDK_CHECK_POINTER(chunk, MFX_ERR_NULL_PTR);

    mfxStatus sts = MFX_ERR_NONE;
    if (MFX_ERR_NONE != (sts = LockFrame(m_pIn)))return sts;
    if (MFX_ERR_NONE != (sts = LockFrame(m_pOut)))
    {
        UnlockFrame(m_pIn);
        return sts;
    }
    
    switch (m_pIn->Info.FourCC)
    {
    case MFX_FOURCC_RGB4:
        {   
            RGB4_to_NV12( &m_pIn->Data,  &m_pIn->Info,
                          &m_pOut->Data, &m_pOut->Info,                               
                          chunk);
        }
        break;
    default:
        sts = MFX_ERR_UNSUPPORTED;
    }    

    UnlockFrame(m_pIn);
    UnlockFrame(m_pOut);

    return sts;
}

MFXPluginBase* CreatePlugin()
{
    return (new CSCPlugin());
}