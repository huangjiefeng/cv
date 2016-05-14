//* ////////////////////////////////////////////////////////////////////////////// */
//*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
//
//
//*/

#ifndef __SAMPLE_PLUGIN_H__
#define __SAMPLE_PLUGIN_H__

#include <stdlib.h>
#include <memory.h>

#include "mfx_plugin_base.h"
#include "rotate_plugin_api.h"
#include "sample_defs.h"

typedef struct {
    mfxU32 StartLine;
    mfxU32 EndLine;
} DataChunk;

class Processor
{
public:
    Processor();
    virtual ~Processor();
    virtual mfxStatus SetAllocator(mfxFrameAllocator *pAlloc);
    virtual mfxStatus Init(mfxFrameSurface1 *frame_in, mfxFrameSurface1 *frame_out);
    virtual mfxStatus Process(DataChunk *chunk) = 0;

protected:
    //locks frame or report of an error
    mfxStatus LockFrame(mfxFrameSurface1 *frame);
    mfxStatus UnlockFrame(mfxFrameSurface1 *frame);
    

    mfxFrameSurface1  *m_pIn;
    mfxFrameSurface1  *m_pOut;
    mfxFrameAllocator *m_pAlloc;
};

class Rotator180 : public Processor
{
public:
    Rotator180();
    virtual ~Rotator180();
    
    virtual mfxStatus Process(DataChunk *chunk);
};

typedef struct {
    mfxFrameSurface1 *In;
    mfxFrameSurface1 *Out;
    bool bBusy;    
    Processor *pProcessor;    
} RotateTask;

class Rotate : public MFXPluginBase
{
public:
    Rotate();
    virtual ~Rotate();      

    // methods to be called by Media SDK
    virtual mfxStatus mfxPluginInit(mfxCoreInterface *core);
    virtual mfxStatus mfxPluginClose();
    virtual mfxStatus mfxGetPluginParam(mfxPluginParam *par);
    virtual mfxStatus mfxSubmit(const mfxHDL *in, mfxU32 in_num, const mfxHDL *out, mfxU32 out_num, mfxThreadTask *task);
    virtual mfxStatus mfxExecute(mfxThreadTask task, mfxU32 uid_p, mfxU32 uid_a);
    virtual mfxStatus mfxFreeResources(mfxThreadTask task, mfxStatus sts); 

    // methods to be called by application    
    virtual mfxStatus QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2]);
    virtual mfxStatus SetAllocator(mfxFrameAllocator *pAlloc);
    virtual mfxStatus SetHandle(mfxHandleType type, mfxHDL handle);
    virtual mfxStatus Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize);
    virtual mfxStatus Close(); 
  
protected:
    bool m_bInited;    

    mfxCoreInterface *m_pmfxCore;

    mfxVideoParam   m_VideoParam;
    mfxPluginParam  m_PluginParam;
    RotateParam     m_Param;

    RotateTask      *m_pTasks;
    mfxU32          m_MaxNumTasks;

    mfxFrameAllocator * m_pAlloc;
    DataChunk *m_pChunks;

    mfxU32 m_NumChunks;

    mfxStatus CheckParam(mfxVideoParam *mfxParam, RotateParam *pRotatePar);
    mfxStatus CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut);
    mfxU32 FindFreeTaskIdx();

    bool m_bIsInOpaque;
    bool m_bIsOutOpaque;
};

#endif __SAMPLE_PLUGIN_H__
