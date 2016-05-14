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

#include <iostream>
#include <fstream>
#include <windows.h>
#include "mfx_plugin_base.h"
#include "sample_opencl_plugin.h"
#include "sample_defs.h"

using namespace std;

#define INTEL_CL_PLATFORM_VENDOR "Intel(R) Corporation"

#define OCL_SOURCE_FILE "ocl_rotate.cl"

// Application uses image2d_t with CL_RGBA channel order, 
// so one kernel call processes 4 pixels.
// Do not modify the OCL_LOOP_UNROLL value without modifying the kernels.
#define OCL_LOOP_UNROLL     4

#define LOCAL_WORKSIZE_X    2
#define LOCAL_WORKSIZE_Y    2

#define OCL_SAFE_CALL(FUNC)             \
{                                       \
    cl_int RET_STS = CL_SUCCESS;        \
    FUNC;                               \
    MSDK_TRACE(#FUNC);                   \
    if (RET_STS != CL_SUCCESS)          \
    {                                   \
        printf("Failed %s, status %d\n", #FUNC, RET_STS);  \
        printf("%s: %d\n", __FILE__, __LINE__);  \
        return MFX_ERR_DEVICE_FAILED;   \
    }                                   \
}

#ifndef SAFE_FREE
#define SAFE_FREE(P, FREE_FUNC)  { if (P) { FREE_FUNC(P); P = NULL; } }
#endif

inline int round_up(int x, int y) 
{
    int mod = x % y;
    return (!mod) ? x : x + (y - mod);
}

////////////////////////////////////

OCLPlugin::OCLPlugin()
{
    // MFX
    m_bInited = false;
    m_pmfxCore = NULL;
    m_pTasks = NULL;
    m_MaxNumTasks = 0;
    memset(&m_Param, 0, sizeof(m_Param));
    memset(&m_mfxParam, 0, sizeof(m_mfxParam));
    m_pAlloc = NULL;

    m_bIsInOpaque = false;
    m_bIsOutOpaque = false;

    // OpenCL
    m_pProgramSource = 0;
    m_clContext = 0;
    m_clCommandQueue = 0;
    m_clProgram = 0;
    m_clKernelRotateY = 0;
    m_clKernelRotateUV = 0;
    m_GlobalWorkSizeY[0] = m_GlobalWorkSizeY[1] = 0;
    m_GlobalWorkSizeUV[0] = m_GlobalWorkSizeUV[1] = 0;
    m_LocalWorkSize[0] = m_LocalWorkSize[1] = 0;

    // fill mfxPluginParam
    m_Param.ThreadPolicy = MFX_THREADPOLICY_SERIAL;
    m_Param.MaxThreadNum = 1;
}

OCLPlugin::~OCLPlugin()
{
    MSDK_TRACE(__FUNCTION__);
    Close();
    mfxPluginClose();
}

// Reports min and suggested number surfaces at input and output.
// Correct request[0].NumFrameMin if plugin uses several frames at input.
// Correct request[1].NumFrameMin if plugin uses several frames at output.
// During processing, do not call function DecreaseReference until surface not needed anymore.
mfxStatus OCLPlugin::QueryIOSurf(mfxVideoParam *par, mfxFrameAllocRequest request[2])
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

// SetAllocator function not supported in this sample
mfxStatus OCLPlugin::SetAllocator(mfxFrameAllocator *pAlloc)
{
    m_pAlloc = pAlloc;
    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::SetHandle(mfxHandleType /*hdlType*/, mfxHDL /*hdl*/)
{
    //dummy function
    return MFX_ERR_NONE;
}

// Initialization
// mfxParam contains input and output video parameters (resolution, etc).
// auxParam contains additional parameters (optional)
mfxStatus OCLPlugin::Init(mfxVideoParam *mfxParam, void* auxParam, int auxParamSize)
{
    MSDK_TRACE(__FUNCTION__);
    mfxStatus sts;

    if (auxParamSize)
    {
        RotateParam *pRotatePar = (RotateParam *)auxParam;
        if (pRotatePar->Angle != 180)
        {
            return MFX_ERR_UNSUPPORTED;
        }
    }

    // check validity of parameters
    MSDK_CHECK_POINTER(mfxParam, MFX_ERR_NULL_PTR);     
    sts = CheckParam(mfxParam);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    // save mfxVideoParam
    m_mfxParam = *mfxParam;

    // map opaque surfaces array in case of opaque surfaces
    m_bIsInOpaque = (m_mfxParam.IOPattern & MFX_IOPATTERN_IN_OPAQUE_MEMORY) ? true : false;
    m_bIsOutOpaque = (m_mfxParam.IOPattern & MFX_IOPATTERN_OUT_OPAQUE_MEMORY) ? true : false;
    mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = NULL;

    if (m_bIsInOpaque || m_bIsOutOpaque)
    {
        pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)GetExtBuffer(m_mfxParam.ExtParam, 
            m_mfxParam.NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
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

    m_MaxNumTasks = m_mfxParam.AsyncDepth + 8;
    //if (!m_MaxNumTasks) m_MaxNumTasks = 16;

    // Picture size
    m_PictureW = mfxParam->vpp.In.Width;
    m_PictureH = mfxParam->vpp.In.Height;

    // Check for width alignment - this sample requires picture width aligned by 4
    MSDK_CHECK_NOT_EQUAL(m_PictureW % OCL_LOOP_UNROLL, 0, MFX_ERR_UNSUPPORTED);
    
    // Get platforms (16 maximum)
    cl_platform_id  clPlatforms[16] = {0};
    cl_uint         numPlatforms = sizeof(clPlatforms)/sizeof(clPlatforms[0]);
    
    {
        cl_int RET_STS = clGetPlatformIDs(numPlatforms, clPlatforms, &numPlatforms);                        
        if (RET_STS != CL_SUCCESS)          
        {                                   
            printf("Failed %s, status %d\n", "clGetPlatformIDs", RET_STS);  
            printf("%s: %d\n", __FILE__, __LINE__);  
            printf("\nFail occured likely because OpenCL DLL was not found. Try reinstalling Intel(R) OpenCL SDK or adding the dll location to the system path manually\n");
            return MFX_ERR_DEVICE_FAILED;   
        }                                   
    }

    // Search for Intel OpenCL
    cl_uint i = 0;
    for (i = 0; i < numPlatforms; i++)
    {
        char platform_name[1024];
        OCL_SAFE_CALL(RET_STS = clGetPlatformInfo(clPlatforms[i], CL_PLATFORM_VENDOR, sizeof(platform_name), platform_name, NULL));
        if (!strcmp(platform_name, INTEL_CL_PLATFORM_VENDOR))
        {
            break;
        }
    }
    if (i >= numPlatforms)
    {
        OCL_SAFE_CALL(RET_STS = (int)"Intel(R) OpenCL not found");
    }

    // Get devices
    cl_uint numDevices = 0;
    cl_device_id cdDevices[1] = {0};
    OCL_SAFE_CALL(RET_STS = clGetDeviceIDs(clPlatforms[i], CL_DEVICE_TYPE_DEFAULT, sizeof(cdDevices)/sizeof(cdDevices[0]), cdDevices, &numDevices));

    // Create context
    OCL_SAFE_CALL(m_clContext = clCreateContext(0, numDevices, cdDevices, NULL, NULL, &RET_STS));

    // Create command queue
    OCL_SAFE_CALL(m_clCommandQueue = clCreateCommandQueue(m_clContext, cdDevices[0], CL_QUEUE_PROFILING_ENABLE, &RET_STS));

    // Read kernel from file
#if 1
    ifstream is;
    is.open(OCL_SOURCE_FILE, ios::binary);
    if(!is.good())
    {
        is.clear(); // set goodbit state to forget previous open operation failure
        // try add program path
        char module_name[MAX_PATH];
        GetModuleFileNameA(0, module_name, MAX_PATH);
        char *p = strrchr(module_name, '\\');
        if (p)
        {
            strncpy_s(p + 1, MAX_PATH - (p + 1 - module_name), OCL_SOURCE_FILE, _TRUNCATE);
            is.open(module_name, ios::binary);
        }
    }
    if(!is.good())
    {
        return MFX_ERR_UNSUPPORTED;
    }

    is.seekg (0, ios::end);
    size_t codeSize = is.tellg();
    is.seekg (0, ios::beg);

    if(codeSize == 0)
    {
        return MFX_ERR_UNSUPPORTED;
    }

    m_pProgramSource = new char[codeSize];
    if(!m_pProgramSource)
    {
        return MFX_ERR_UNSUPPORTED;
    }

    is.read(m_pProgramSource, codeSize);
    is.close();
#endif

    // Create the program object
    OCL_SAFE_CALL(m_clProgram = clCreateProgramWithSource(m_clContext, 1, (const char **)&m_pProgramSource, &codeSize, &RET_STS));

    // Build program
    OCL_SAFE_CALL(RET_STS = clBuildProgram(m_clProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL));

    // Create kernels
    OCL_SAFE_CALL(m_clKernelRotateY = clCreateKernel(m_clProgram, "RotateY", &RET_STS));
    OCL_SAFE_CALL(m_clKernelRotateUV = clCreateKernel(m_clProgram, "RotateUV", &RET_STS));

    // Local work sizes
    m_LocalWorkSize[0] = LOCAL_WORKSIZE_X;
    m_LocalWorkSize[1] = LOCAL_WORKSIZE_Y;

    // Global work size for Y
    m_GlobalWorkSizeY[0] = round_up(m_PictureW/OCL_LOOP_UNROLL, (int)m_LocalWorkSize[0]); // picture width divided by 4 because RotateY kernel processes 4 pixels per call
    m_GlobalWorkSizeY[1] = round_up(m_PictureH, (int)m_LocalWorkSize[1]);

    // Global work size for UV
    m_GlobalWorkSizeUV[0] = round_up(m_PictureW/OCL_LOOP_UNROLL, (int)m_LocalWorkSize[0]);
    m_GlobalWorkSizeUV[1] = round_up(m_PictureH/2, (int)m_LocalWorkSize[1]);

    // Internal task pool
    m_pTasks = new MFXTask[m_MaxNumTasks];
    MSDK_CHECK_POINTER(m_pTasks, MFX_ERR_MEMORY_ALLOC);
    memset(m_pTasks, 0, sizeof(MFXTask) * m_MaxNumTasks);

    // Initialization completed
    m_bInited = true;

    return MFX_ERR_NONE;
}

// internal function, used by Init
mfxStatus OCLPlugin::CheckParam(mfxVideoParam *pParam)
{
    MSDK_CHECK_POINTER(pParam, MFX_ERR_NULL_PTR);      

    // only NV12 color format is supported
    if (MFX_FOURCC_NV12 != pParam->vpp.In.FourCC || MFX_FOURCC_NV12 != pParam->vpp.Out.FourCC)
    {
        return MFX_ERR_UNSUPPORTED;
    }    

    return MFX_ERR_NONE;
}

// internal function, used by Init
mfxStatus OCLPlugin::CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut)
{
    MSDK_CHECK_POINTER(pIn, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pOut, MFX_ERR_NULL_PTR);

    if (pIn->CropW != m_mfxParam.vpp.In.CropW || pIn->CropH != m_mfxParam.vpp.In.CropH || 
        pIn->FourCC != m_mfxParam.vpp.In.FourCC ||
        pOut->CropW != m_mfxParam.vpp.Out.CropW || pOut->CropH != m_mfxParam.vpp.Out.CropH || 
        pOut->FourCC != m_mfxParam.vpp.Out.FourCC)
    {
        return MFX_ERR_INVALID_VIDEO_PARAM;
    }

    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::Close()
{
    // Release OpenCL Objects
    SAFE_FREE(m_clKernelRotateY, clReleaseKernel);
    SAFE_FREE(m_clKernelRotateUV, clReleaseKernel);
    SAFE_FREE(m_clProgram, clReleaseProgram);
    SAFE_FREE(m_clCommandQueue, clReleaseCommandQueue);
    SAFE_FREE(m_clContext, clReleaseContext);
    MSDK_SAFE_DELETE_ARRAY(m_pProgramSource);

    // Release MFXTask array
    MSDK_SAFE_DELETE_ARRAY(m_pTasks);   
    memset(&m_Param, 0, sizeof(m_Param));

    mfxStatus sts = MFX_ERR_NONE;

    mfxExtOpaqueSurfaceAlloc* pluginOpaqueAlloc = NULL;
    
    if (m_bIsInOpaque || m_bIsOutOpaque)
    {
        pluginOpaqueAlloc = (mfxExtOpaqueSurfaceAlloc*)
            GetExtBuffer(m_mfxParam.ExtParam, m_mfxParam.NumExtParam, MFX_EXTBUFF_OPAQUE_SURFACE_ALLOCATION);
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

    return sts;
}

// Find free task
mfxU32 OCLPlugin::FindFreeTaskIdx()
{   
    int i;
    for (i = 0; i < m_MaxNumTasks; i++)
    {
        if (false == m_pTasks[i].bBusy)
        {
            break;
        }
    }

    return i;
}

// Enqueue task
mfxStatus OCLPlugin::EnqueueTask(MFXTask *pTask)
{
    MSDK_TRACE(__FUNCTION__);

    mfxStatus sts = MFX_ERR_NONE;
    if (MFX_ERR_NONE != (sts = LockFrame(pTask->In)))return sts;
    if (MFX_ERR_NONE != (sts = LockFrame(pTask->Out)))
    {
        UnlockFrame(pTask->In);
        return sts;
    }

    mfxFrameData *pInData = &pTask->In->Data;
    mfxFrameData *pOutData = &pTask->Out->Data;

    m_PicturePitch = pTask->In->Data.Pitch;
    m_PictureBytesY = m_PicturePitch * m_PictureH;
    m_PictureBytesUV = m_PictureBytesY / 2;

    cl_image_format image_format = {CL_RGBA, CL_UNSIGNED_INT8};

    const size_t origin[3] = {0, 0, 0};
    const size_t regionY[3] = {m_PictureW/OCL_LOOP_UNROLL, m_PictureH, 1};
    const size_t regionUV[3] = {m_PictureW/OCL_LOOP_UNROLL, m_PictureH/2, 1};
        
    // Create buffers
    if (!pTask->BufferSrcY)
    {
        OCL_SAFE_CALL(pTask->BufferSrcY = clCreateImage2D(m_clContext, CL_MEM_READ_ONLY, &image_format, regionY[0], regionY[1], 0, NULL, &RET_STS));
    }
    if (!pTask->BufferSrcUV)
    {
        OCL_SAFE_CALL(pTask->BufferSrcUV = clCreateImage2D(m_clContext, CL_MEM_READ_ONLY, &image_format, regionUV[0], regionUV[1], 0, NULL, &RET_STS));        
    }
    if (!pTask->BufferDstY)
    {
        OCL_SAFE_CALL(pTask->BufferDstY = clCreateImage2D(m_clContext, CL_MEM_WRITE_ONLY, &image_format, regionY[0], regionY[1], 0, NULL, &RET_STS));        
    }
    if (!pTask->BufferDstUV)
    {
        OCL_SAFE_CALL(pTask->BufferDstUV = clCreateImage2D(m_clContext, CL_MEM_WRITE_ONLY, &image_format, regionUV[0], regionUV[1], 0, NULL, &RET_STS));
    }

    // Set kernel arguments
    OCL_SAFE_CALL(RET_STS = clSetKernelArg(m_clKernelRotateY, 0, sizeof(cl_mem), (void*)&pTask->BufferSrcY));
    OCL_SAFE_CALL(RET_STS = clSetKernelArg(m_clKernelRotateY, 1, sizeof(cl_mem), (void*)&pTask->BufferDstY));
    OCL_SAFE_CALL(RET_STS = clSetKernelArg(m_clKernelRotateUV, 0, sizeof(cl_mem), (void*)&pTask->BufferSrcUV));
    OCL_SAFE_CALL(RET_STS = clSetKernelArg(m_clKernelRotateUV, 1, sizeof(cl_mem), (void*)&pTask->BufferDstUV));

    // Enqueue writing Y data from host to device
    OCL_SAFE_CALL(RET_STS = clEnqueueWriteImage(m_clCommandQueue, pTask->BufferSrcY, CL_FALSE, origin, regionY, m_PicturePitch, 0, pInData->Y, 0, NULL, &pTask->cl_event_WriteBufferY.clEvent));

    // Enqueue writing UV data from host to device
    OCL_SAFE_CALL(RET_STS = clEnqueueWriteImage(m_clCommandQueue, pTask->BufferSrcUV, CL_FALSE, origin, regionUV, m_PicturePitch, 0, pInData->UV, 0, NULL, &pTask->cl_event_WriteBufferUV.clEvent));

    // Flush
    OCL_SAFE_CALL(RET_STS = clFlush(m_clCommandQueue));

    // Enqueue KernelRotateY
    OCL_SAFE_CALL(RET_STS = clEnqueueNDRangeKernel(m_clCommandQueue, m_clKernelRotateY, 2, NULL, m_GlobalWorkSizeY, m_LocalWorkSize, 1, &pTask->cl_event_WriteBufferY.clEvent, &pTask->cl_event_KernelY.clEvent));

    // Enqueue KernelRotateUV
    OCL_SAFE_CALL(RET_STS = clEnqueueNDRangeKernel(m_clCommandQueue, m_clKernelRotateUV, 2, NULL, m_GlobalWorkSizeUV, m_LocalWorkSize, 1, &pTask->cl_event_WriteBufferUV.clEvent, &pTask->cl_event_KernelUV.clEvent));

    // Flush
    OCL_SAFE_CALL(RET_STS = clFlush(m_clCommandQueue));

    // Enqueue reading Y data from device to host
    OCL_SAFE_CALL(RET_STS = clEnqueueReadImage(m_clCommandQueue, pTask->BufferDstY, CL_FALSE, origin, regionY, m_PicturePitch, 0, pOutData->Y, 1, &pTask->cl_event_KernelY.clEvent, &pTask->cl_event_ReadBufferY.clEvent));

    // Enqueue reading UV data from device to host
    OCL_SAFE_CALL(RET_STS = clEnqueueReadImage(m_clCommandQueue, pTask->BufferDstUV, CL_FALSE, origin, regionUV, m_PicturePitch, 0, pOutData->UV, 1, &pTask->cl_event_KernelUV.clEvent, &pTask->cl_event_ReadBufferUV.clEvent));

    // Flush
    OCL_SAFE_CALL(RET_STS = clFlush(m_clCommandQueue));

    UnlockFrame(pTask->In);
    UnlockFrame(pTask->Out);

    return MFX_TASK_WORKING;
}

// Check status
mfxStatus OCLPlugin::QueryStatus(MFXTask *pTask)
{
    MSDK_TRACE(__FUNCTION__);

    {
        cl_int stsY = -1, stsUV = CL_COMPLETE;
        size_t size_ret = 0;

        OCL_SAFE_CALL(RET_STS = clGetEventInfo(pTask->cl_event_ReadBufferY.clEvent, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(stsY), &stsY, &size_ret));
        OCL_SAFE_CALL(RET_STS = clGetEventInfo(pTask->cl_event_ReadBufferUV.clEvent, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(stsUV), &stsUV, &size_ret));

        if (stsY < 0 || stsUV < 0)
        {
            return MFX_ERR_DEVICE_FAILED;
        }
        if (stsY != CL_COMPLETE || stsUV != CL_COMPLETE)
        {
            return MFX_TASK_WORKING;
        }

        //GETTIME(pTask->cl_event_ReadBufferY.timeFinished);
    }

    //SendProfilingEvent(pTask->cl_event_WriteBufferY);
    //SendProfilingEvent(pTask->cl_event_KernelY);
    //SendProfilingEvent(pTask->cl_event_ReadBufferY);

    return MFX_TASK_DONE;
}

/////////////////////////////////////////////////////////////////////
/* Methods required for integration with Media SDK */
mfxStatus OCLPlugin::mfxPluginInit(mfxCoreInterface *core)
{
    MSDK_TRACE(__FUNCTION__);

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

mfxStatus OCLPlugin::mfxPluginClose()
{
    MSDK_TRACE(__FUNCTION__);

    MSDK_SAFE_DELETE(m_pmfxCore);
 
    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::mfxGetPluginParam(mfxPluginParam *par)
{
    MSDK_TRACE(__FUNCTION__);

    MSDK_CHECK_POINTER(par, MFX_ERR_NULL_PTR);    

    *par = *(mfxPluginParam *)&m_Param;

    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::mfxSubmit(const mfxHDL *in, mfxU32 in_num, const mfxHDL *out, mfxU32 out_num, mfxThreadTask *task)
{
    MSDK_TRACE(__FUNCTION__);

    MSDK_CHECK_POINTER(in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(out, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(*in, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(*out, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(task, MFX_ERR_NULL_PTR);    
    MSDK_CHECK_NOT_EQUAL(in_num, 1, MFX_ERR_UNSUPPORTED);
    MSDK_CHECK_NOT_EQUAL(out_num, 1, MFX_ERR_UNSUPPORTED);
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED);  

    mfxFrameSurface1 *opaque_surface_in = (mfxFrameSurface1 *)in[0];
    mfxFrameSurface1 *opaque_surface_out = (mfxFrameSurface1 *)out[0];          
    mfxFrameSurface1 *surface_in = opaque_surface_in;
    mfxFrameSurface1 *surface_out = opaque_surface_out;
    
    mfxStatus sts = MFX_ERR_NONE;

    if (m_bIsInOpaque)
    {
        sts = m_pmfxCore->GetRealSurface(m_pmfxCore->pthis, opaque_surface_in, &surface_in);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    if (m_bIsOutOpaque)
    {
        sts = m_pmfxCore->GetRealSurface(m_pmfxCore->pthis, opaque_surface_out, &surface_out);            
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, MFX_ERR_MEMORY_ALLOC);
    }

    // check validity of parameters
    sts = CheckInOutFrameInfo(&surface_in->Info, &surface_out->Info);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    int ind = FindFreeTaskIdx();

    if (ind >= m_MaxNumTasks)
    {
        return MFX_WRN_DEVICE_BUSY; // currently there are no free tasks available
    }

    MFXTask* pCurrentTask = &m_pTasks[ind];
    memset(pCurrentTask, 0, sizeof(MFXTask));
    pCurrentTask->In = surface_in;
    pCurrentTask->Out = surface_out;
    pCurrentTask->bBusy = true;
    
    *task = (mfxThreadTask)pCurrentTask;    
    
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(surface_in->Data));
    m_pmfxCore->IncreaseReference(m_pmfxCore->pthis, &(surface_out->Data));

    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::mfxExecute(mfxThreadTask task, mfxU32 uid_p, mfxU32 uid_a)
{    
    MSDK_TRACE(__FUNCTION__);

    MFXTask *current_task = (MFXTask *)task;
    mfxStatus sts;

    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED);
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    if (0 == uid_a)
    {
        sts = EnqueueTask(current_task);
    }
    else
    {
        sts = QueryStatus(current_task);
    }

    return sts;        
}

mfxStatus OCLPlugin::mfxFreeResources(mfxThreadTask task, mfxStatus sts)
{
    MSDK_CHECK_ERROR(m_bInited, false, MFX_ERR_NOT_INITIALIZED); 
    MSDK_CHECK_POINTER(m_pmfxCore, MFX_ERR_NOT_INITIALIZED);  

    MFXTask *current_task = (MFXTask *)task;
    
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->In->Data));
    m_pmfxCore->DecreaseReference(m_pmfxCore->pthis, &(current_task->Out->Data));
    current_task->bBusy = false;

    OCL_SAFE_CALL(RET_STS = clReleaseMemObject(current_task->BufferSrcY));
    OCL_SAFE_CALL(RET_STS = clReleaseMemObject(current_task->BufferSrcUV));
    OCL_SAFE_CALL(RET_STS = clReleaseMemObject(current_task->BufferDstY));
    OCL_SAFE_CALL(RET_STS = clReleaseMemObject(current_task->BufferDstUV));

    return MFX_ERR_NONE;
}

mfxStatus OCLPlugin::LockFrame(mfxFrameSurface1 *frame)
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

mfxStatus OCLPlugin::UnlockFrame(mfxFrameSurface1 *frame)
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

///////////////////////////////////////////////////////////////////////////////////
// external function

/////////////////////////////////////////////////////////////////////
// custom methods
MFXPluginBase* CreatePlugin()
{
    return (new OCLPlugin());
}
