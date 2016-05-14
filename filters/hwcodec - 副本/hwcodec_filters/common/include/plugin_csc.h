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

#ifndef __SAMPLE_PLUGIN_CSC_H__
#define __SAMPLE_PLUGIN_CSC_H__

#include <stdlib.h>
#include <memory.h>

#include "mfx_plugin_base.h"
#include "mfx_filter_defs.h"

struct CSCParam
{    
    enum evOrientation
    {
        TOP_DOWN,
        BOTTOM_UP
    } vOrientation;    

    CSCParam(evOrientation orient  = TOP_DOWN) 
        : vOrientation(orient)        
    {
    }
};

typedef struct {
    mfxU32 StartLine;
    mfxU32 EndLine;
} DataChunk;

// Color Space Converter - converts frames from RGB4 to NV12, assuming input frame is stored bottom-up
class CSCProcessor 
{
public:
    CSCProcessor();
    virtual ~CSCProcessor();
    virtual mfxStatus SetAllocator(mfxFrameAllocator *pAlloc);
    virtual mfxStatus Init(mfxFrameSurface1 *frame_in, mfxFrameSurface1 *frame_out);
    virtual mfxStatus Process(DataChunk *chunk);

protected:
    //locks frame or report of an error
    mfxStatus LockFrame(mfxFrameSurface1 *frame);
    mfxStatus UnlockFrame(mfxFrameSurface1 *frame);   

    mfxStatus RGB4_to_NV12(mfxFrameData* inData,  mfxFrameInfo* inInfo,
                           mfxFrameData* outData, mfxFrameInfo* outInfo,                       
                           DataChunk *chunk);

    void ownBGRToYCbCr420_8u_AC4P3R(const mfxU8* pSrc, 
                                    int srcStep, 
                                    mfxU8* pDst[2],
                                    int dstStep[2], 
                                    std::pair<int,int> roiSize );

    mfxFrameSurface1  *m_pIn;
    mfxFrameSurface1  *m_pOut;
    mfxFrameAllocator *m_pAlloc;
};

typedef struct {
    mfxFrameSurface1 *In;
    mfxFrameSurface1 *Out;
    bool bBusy;    
    CSCProcessor *pProcessor;    
} CSCTask;

// If input frames are stored bottom-up (such as those from most webcams)
// we need this plugin to perform conversion from RGB4 to NV12.
// Frames converted in VPP will be flipped, because VPP 
// always assumes that frames are stored top-down.
class CSCPlugin : public MFXPluginBase
{
public:
    CSCPlugin();
    virtual ~CSCPlugin();      

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

    CSCTask        *m_pTasks;
    mfxU32          m_MaxNumTasks;

    mfxFrameAllocator * m_pAlloc;
    DataChunk *m_pChunks;

    mfxU32 m_NumChunks;

    mfxStatus CheckParam(mfxVideoParam *mfxParam);
    mfxStatus CheckInOutFrameInfo(mfxFrameInfo *pIn, mfxFrameInfo *pOut);
    mfxU32 FindFreeTaskIdx();
};

#endif __SAMPLE_PLUGIN_CSC_H__
