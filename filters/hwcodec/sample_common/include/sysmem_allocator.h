/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2008-2011 Intel Corporation. All Rights Reserved.

\* ****************************************************************************** */

#ifndef __SYSMEM_ALLOCATOR_H__
#define __SYSMEM_ALLOCATOR_H__

#include "base_allocator.h"

struct sBuffer
{
    mfxU32      id;
    mfxU32      nbytes;
    mfxU16      type;
};

struct sFrame
{
    mfxU32          id;
    mfxFrameInfo    info;
};

struct SysMemAllocatorParams : mfxAllocatorParams
{
    MFXBufferAllocator *pBufferAllocator;
}; 

class SysMemFrameAllocator: public BaseFrameAllocator
{
public:
    SysMemFrameAllocator();
    virtual ~SysMemFrameAllocator(); 

    virtual mfxStatus Init(mfxAllocatorParams *pParams);
    virtual mfxStatus Close();

protected:
    virtual mfxStatus LockFrame(mfxMemId mid, mfxFrameData *ptr);
    virtual mfxStatus UnlockFrame(mfxMemId mid, mfxFrameData *ptr);
    virtual mfxStatus GetFrameHDL(mfxMemId mid, mfxHDL *handle);       

    virtual mfxStatus CheckRequestType(mfxFrameAllocRequest *request);
    virtual mfxStatus ReleaseResponse(mfxFrameAllocResponse *response);
    virtual mfxStatus AllocImpl(mfxFrameAllocRequest *request, mfxFrameAllocResponse *response);

    MFXBufferAllocator *m_pBufferAllocator;
    bool m_bOwnBufferAllocator;
};

class SysMemBufferAllocator : public MFXBufferAllocator
{
public:
    SysMemBufferAllocator();
    virtual ~SysMemBufferAllocator();  

protected:
    virtual mfxStatus AllocBuffer(mfxU32 nbytes, mfxU16 type, mfxMemId *mid);
    virtual mfxStatus LockBuffer(mfxMemId mid, mfxU8 **ptr);
    virtual mfxStatus UnlockBuffer(mfxMemId mid);    
    virtual mfxStatus FreeBuffer(mfxMemId mid); 
};

#endif // __SYSMEM_ALLOCATOR_H__