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
#include "plugin_rotate.h"

// disable "unreferenced formal parameter" warning - 
// not all formal parameters of interface functions will be used by sample plugin
#pragma warning(disable : 4100) 

#define SWAP_BYTES(a, b) {mfxU8 tmp; tmp = a; a = b; b = tmp;}

/* Rotate class implementation */
Rotate::Rotate() : 
m_pTasks(NULL),
m_bInited(false),
m_pAlloc(NULL),
m_bIsInOpaque(false),
m_bIsOutOpaque(false)
{
    m_MaxNumTasks = 0;
    
    memset(&m_VideoParam, 0, sizeof(m_VideoParam));
    memset(&m_Param, 0, sizeof(m_Param));

    m_pmfxCore = NULL;
    memset(&m_PluginParam, 0, sizeof(m_PluginParam));
    m_PluginParam.MaxThreadNum = 1;
    m_PluginParam.ThreadPolicy = MFX_THREADPOLICY_SERIAL;
}

Rotate::~Rotate()
{    
    mfxPluginClose();   
    Close();    
}

/* Methods required for integration with Media SDK */
mfxStatus Rotate::mfxPluginInit(mfxCoreInterface *core)
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

mfxStatus Rotate::mfxPluginClose()
{
    MSDK_SAFE_DELETE(m_pmfxCore);
 
    return MFX_ERR_NONE;
}

mfxStatus Rotate::mfxGetPluginParam(mfxPluginParam *par)
{
    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);    

    *par = m_PluginParam;

    return MFX_ERR_NONE;
}

mfxStatus Rotate::mfxSubmit(const mfxHDL *in, mfxU32 in_num, const mfxHDL *out, mfxU32 out_num, mfxThreadTask *task)
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
    mfxFrameSurface1 *real_surface_in = surface_in;
    mfxFrameSurface1 *real_surface_out = surface_out;

    mfxStatus sts = MFX_ERR_NONE;

    if (m_bIsInOpaque)
    {
        sts = m_pmfxCore->GetRealSurface(m_pmfxCore->pthis, surface_in, &real_surface_in);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    if (m_bIsOutOpaque)
    {
        sts = m_pmfxCore->GetRealSurface(m_pmfxCore->pthis, surface_out, &real_surface_out);            
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }
    
    // check validity of parameters
    sts = CheckInOutFrameInfo(&real_surface_in->Info, &real_surface_out->Info);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);
           
    mfxU32 ind = FindFreeTaskIdx();

    if (ind >= m_MaxNumTasks)
    {
        return MFX_WRN_DEVICE_BUSY; // currently there are no free tasks available
    }
    
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(real_surface_in->Data));
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(real_surface_out->Data));

    m_pTasks[ind].In = real_surface_in;
    m_pTasks[ind].Out = real_surface_out;
    m_pTasks[ind].bBusy = true;

    switch (m_Param.Angle)
    {
    case 180:
        m_pTasks[ind].pProcessor = new Rotator180;
        MSDK_CHECK_POINTER(m_pTasks[ind].pProcessor, MFX_ERR_MEMORY_ALLOC);
        break;
    default:
        return MFX_ERR_UNSUPPORTED;
    }
    
    m_pTasks[ind].pProcessor->SetAllocator(m_pAlloc);
    m_pTasks[ind].pProcessor->Init(real_surface_in, real_surface_out);
    
    *task = (mfxThreadTask)&m_pTasks[ind];    

    return MFX_ERR_NONE;
}

mfxStatus Rotate::mfxExecute(mfxThreadTask task, mfxU32 uid_p, mfxU32 uid_a)
{    
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED);
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    mfxStatus sts = MFX_ERR_NONE;
    RotateTask *current_task = (RotateTask *)task;    

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

mfxStatus Rotate::mfxFreeResources(mfxThreadTask task, mfxStatus sts)
{
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED); 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    RotateTask *current_task = (RotateTask *)task;
    
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->In->Data));
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->Out->Data));
    current_task->bBusy = false;
    MSDK_SAFE_DELETE(current_task->pProcessor);

    return MFX_ERR_NONE;
}

/* Custom methods */
mfxStatus Rotate::Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize)
{
    RotateParam *pRotatePar = (RotateParam *)auxParam;
    MSDK_CHECK_POINTER(pRotatePar, MFX_ERR_NULL_PTR);     
    MSDK_CHECK_POINTER(mfxParam, MFX_ERR_NULL_PTR); 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NULL_PTR);

    // check validity of parameters
    mfxStatus sts = CheckParam(mfxParam, pRotatePar);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    m_Param = *pRotatePar;
    m_VideoParam = *mfxParam;

    // map opaque surfaces array in case of opaque surfaces
    m_bIsInOpaque = (m_VideoParam.IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY) ? true : false;
    m_bIsOutOpaque = (m_VideoParam.IOPattern & MFX_IOPATTERN_OUT_OPAQUE_MEMORY) ? true : false;
    mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = NULL;

    if (m_bIsInOpaque || m_bIsOutOpaque)
    {
        pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)GetExtBuffer(m_VideoParam.ExtParam, 
            m_VideoParam.NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
        MSDK_CHECK_POINTER(pluginOpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);
    }

    // check existence of corresponding allocs
    if ((m_bIsInOpaque && ! pluginOpaqueAlloc->In.Surfaces) || (m_bIsOutOpaque && !pluginOpaqueAlloc->Out.Surfaces))
       return MFX_ERR_INVALID_VIDEO_PARAM;        
   
    if (m_bIsInOpaque)
    {
        sts = m_pmfxCore->MapOpaqueSurface(m_pmfxCore->pthis, pluginOpaqueAlloc->In.NumSurface, 
            pluginOpaqueAlloc->In.Type, pluginOpaqueAlloc->In.Surfaces);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    if (m_bIsOutOpaque)
    {
        sts = m_pmfxCore->MapOpaqueSurface(m_pmfxCore->pthis, pluginOpaqueAlloc->Out.NumSurface, 
            pluginOpaqueAlloc->Out.Type, pluginOpaqueAlloc->Out.Surfaces);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    m_MaxNumTasks = m_VideoParam.AsyncDepth;
    if (m_MaxNumTasks < 2) m_MaxNumTasks = 2;

    m_pTasks = new RotateTask [m_MaxNumTasks];
    MSDK_CHECK_POINTER(m_pTasks, MFX_ERR_MEMORY_ALLOC);
    memset(m_pTasks, 0, sizeof(RotateTask) * m_MaxNumTasks);

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

mfxStatus Rotate::Close()
{
    
    if (!m_bInited)
        return MFX_ERR_NONE;
    
    memset(&m_Param, 0, sizeof(RotateParam));

    MSDK_SAFE_DELETE_ARRAY(m_pTasks); 

    mfxStatus sts = MFX_ERR_NONE;

    mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = NULL;
    
    if (m_bIsInOpaque || m_bIsOutOpaque)
    {
        pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)
            GetExtBuffer(m_VideoParam.ExtParam, m_VideoParam.NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
        MSDK_CHECK_POINTER(pluginOpaqueAlloc, MFX_ERR_INVALID_VIDEO_PARAM);
    }

    // check existence of corresponding allocs
    if ((m_bIsInOpaque && ! pluginOpaqueAlloc->In.Surfaces) || (m_bIsOutOpaque && !pluginOpaqueAlloc->Out.Surfaces))
        return MFX_ERR_INVALID_VIDEO_PARAM;        

    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NULL_PTR);
    if (m_bIsInOpaque)
    {
        sts = m_pmfxCore->UnmapOpaqueSurface(m_pmfxCore->pthis, pluginOpaqueAlloc->In.NumSurface, 
            pluginOpaqueAlloc->In.Type, pluginOpaqueAlloc->In.Surfaces);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    if (m_bIsOutOpaque)
    {
        sts = m_pmfxCore->UnmapOpaqueSurface(m_pmfxCore->pthis, pluginOpaqueAlloc->Out.NumSurface, 
            pluginOpaqueAlloc->Out.Type, pluginOpaqueAlloc->Out.Surfaces);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    m_bInited = false;

    return MFX_ERR_NONE;
}

mfxStatus Rotate::QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2])
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

mfxStatus Rotate::SetAllocator(mfxFrameAllocator * pAlloc)
{
    m_pAlloc = pAlloc;
    return MFX_ERR_NONE;
}

mfxStatus Rotate::SetHandle(mfxHandleType /*type*/, mfxHDL /*hdl*/)
{
    //dummy function
    return MFX_ERR_NONE;
}


/* Internal methods */
mfxU32 Rotate::FindFreeTaskIdx()
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

mfxStatus Rotate::CheckParam(mfxVideoParam *mfxParam, RotateParam *pRotatePar)
{
    MSDK_CHECK_POINTER(mfxParam, MFX_ERR_NULL_PTR);      

    mfxInfoVPP *pParam = &mfxParam->vpp;    

    // only NV12 color format is supported
    if (MFX_FOURCC_NV12 != pParam->In.FourCC || MFX_FOURCC_NV12 != pParam->Out.FourCC)
    {
        return MFX_ERR_UNSUPPORTED;
    }    

    return MFX_ERR_NONE;
}

mfxStatus Rotate::CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut)
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
Processor::Processor() 
    : m_pIn(NULL)
    , m_pOut(NULL)
    , m_pAlloc(NULL)
{
}

Processor::~Processor()
{
}

mfxStatus Processor::SetAllocator(mfxFrameAllocator *pAlloc)
{
    m_pAlloc = pAlloc;
    return MFX_ERR_NONE;
}

mfxStatus Processor::Init(mfxFrameSurface1 *frame_in, mfxFrameSurface1 *frame_out)
{
    MSDK_CHECK_POINTER(frame_in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(frame_out, MFX_ERR_NULL_PTR);

    m_pIn = frame_in;
    m_pOut = frame_out;    

    return MFX_ERR_NONE;        
}

mfxStatus Processor::LockFrame(mfxFrameSurface1 *frame)
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

mfxStatus Processor::UnlockFrame(mfxFrameSurface1 *frame)
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


/* 180 degrees rotator class implementation */
Rotator180::Rotator180() : Processor()
{
}

Rotator180::~Rotator180()
{
}

mfxStatus Rotator180::Process(DataChunk *chunk)
{
    MSDK_CHECK_POINTER(chunk, MFX_ERR_NULL_PTR);

    mfxStatus sts = MFX_ERR_NONE;
    if (MFX_ERR_NONE != (sts = LockFrame(m_pIn)))return sts;
    if (MFX_ERR_NONE != (sts = LockFrame(m_pOut)))
    {
        UnlockFrame(m_pIn);
        return sts;
    }

    mfxU32 i, j, in_pitch, out_pitch, h, w;

    in_pitch = m_pIn->Data.Pitch;
    out_pitch = m_pOut->Data.Pitch;
    h = m_pIn->Info.CropH;
    w = m_pIn->Info.CropW;
    mfxU8 *in_luma = m_pIn->Data.Y + m_pIn->Info.CropY * in_pitch + m_pIn->Info.CropX;
    mfxU8 *out_luma = m_pOut->Data.Y + m_pOut->Info.CropY * out_pitch + m_pOut->Info.CropX; 

    mfxU8 *in_chroma = m_pIn->Data.VU + m_pIn->Info.CropY / 2 * in_pitch + m_pIn->Info.CropX;
    mfxU8 *out_chroma = m_pOut->Data.VU + m_pOut->Info.CropY / 2 * out_pitch + m_pOut->Info.CropX; 

    mfxU8 *cur_line = 0; // current line in the destination image
    
    switch (m_pIn->Info.FourCC)
    {
    case MFX_FOURCC_NV12: 
          for (i = chunk->StartLine; i <= chunk->EndLine; i++)
          {
              // rotate Y plane
              cur_line = out_luma + (h-1-i) * out_pitch;

              // i-th line images into h-1-i-th line, w bytes in line
              memcpy(cur_line,  in_luma + i * in_pitch, w);        
              
              // mirror line's elements with respect to the middle element, element=Yj                          
              for (j = 0; j < w / 2; j++)
              {              
                  SWAP_BYTES(cur_line[j], cur_line[w-1-j]); 
              }              

              // rotate VU plane, contains h/2 lines
              cur_line = out_chroma + (h/2-1-i/2) * out_pitch;

              // i-th line images into h-1-i-th line, w bytes in line
              memcpy(cur_line,  in_chroma + i/2 * in_pitch, w);        

              // mirror line's elements with respect to the middle element, element=VjUj
              for (j = 0; j < w/2 - 1; j = j + 2)
              {              
                  SWAP_BYTES(cur_line[j], cur_line[w-1-j-1]); // 0 -> -1                  
                  SWAP_BYTES(cur_line[j+1], cur_line[w-1-j]); // 1 -> -0                     
              }             
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
    return (new Rotate());
}