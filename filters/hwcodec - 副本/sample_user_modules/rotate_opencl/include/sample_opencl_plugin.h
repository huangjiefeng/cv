//*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2010-2011 Intel Corporation. All Rights Reserved.
//
//
//*/

#include <CL/opencl.h>

#include "mfx_plugin_base.h"
#include "rotate_plugin_api.h"

typedef struct
{
    char        *name;
    cl_event    clEvent;
    mfxU64      timeEnqueue;
    mfxU64      timeFinished;
    mfxU64      id;
} clProfilingEvent;

typedef struct {
    mfxFrameSurface1 *In;
    mfxFrameSurface1 *Out;
    cl_mem          BufferSrcY;
    cl_mem          BufferSrcUV;
    cl_mem          BufferDstY;
    cl_mem          BufferDstUV;
    bool bBusy;
    clProfilingEvent    cl_event_WriteBufferY;
    clProfilingEvent    cl_event_WriteBufferUV;
    clProfilingEvent    cl_event_KernelY;
    clProfilingEvent    cl_event_KernelUV;
    clProfilingEvent    cl_event_ReadBufferY;
    clProfilingEvent    cl_event_ReadBufferUV;
} MFXTask;

class OCLPlugin : public MFXPluginBase
{
public:
    OCLPlugin();
    ~OCLPlugin();

    mfxStatus mfxPluginInit(mfxCoreInterface *core);
    mfxStatus mfxPluginClose();
    mfxStatus mfxGetPluginParam(mfxPluginParam *par);
    mfxStatus mfxSubmit(const mfxHDL *in, mfxU32 in_num, const mfxHDL *out, mfxU32 out_num, mfxThreadTask *task);
    mfxStatus mfxExecute(mfxThreadTask task, mfxU32 uid_p, mfxU32 uid_a);
    mfxStatus mfxFreeResources(mfxThreadTask task, mfxStatus sts);

    virtual mfxStatus QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2]);
    virtual mfxStatus SetAllocator(mfxFrameAllocator *pAlloc);
    virtual mfxStatus SetHandle(mfxHandleType , mfxHDL );
    virtual mfxStatus Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize);
    virtual mfxStatus Close();    

protected:
    mfxStatus EnqueueTask(MFXTask *pTask);
    mfxStatus QueryStatus(MFXTask *pTask);
    mfxStatus CheckParam(mfxVideoParam *pParam);
    mfxStatus CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut);
    mfxU32    FindFreeTaskIdx();

    //locks frame or report of an error
    mfxStatus LockFrame(mfxFrameSurface1 *frame);
    mfxStatus UnlockFrame(mfxFrameSurface1 *frame);

    //
    bool                m_bInited;
    mfxCoreInterface    *m_pmfxCore;
    mfxPluginParam      m_Param;
    mfxVideoParam       m_mfxParam;
    MFXTask             *m_pTasks;
    int                 m_MaxNumTasks;
    int                 m_PictureW;
    int                 m_PictureH;
    int                 m_PicturePitch;
    size_t              m_PictureBytesY;
    size_t              m_PictureBytesUV;
    mfxFrameAllocator   *m_pAlloc;

    // for opaque memory support
    bool m_bIsInOpaque;
    bool m_bIsOutOpaque;

    // OpenCL vars
    char                *m_pProgramSource;
    cl_context          m_clContext;
    cl_command_queue    m_clCommandQueue;
    cl_program          m_clProgram;
    cl_kernel           m_clKernelRotateY;
    cl_kernel           m_clKernelRotateUV;
    size_t              m_GlobalWorkSizeY[2];
    size_t              m_GlobalWorkSizeUV[2];
    size_t              m_LocalWorkSize[2];
};
