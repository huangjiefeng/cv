/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2008-2011 Intel Corporation. All Rights Reserved.

\* ****************************************************************************** */

#include <assert.h>
#include "base_allocator.h"

MFXFrameAllocator::MFXFrameAllocator()
{    
    pthis = this;
    Alloc = Alloc_;
    Lock  = Lock_;
    Free  = Free_;
    Unlock = Unlock_;
    GetHDL = GetHDL_;
}

MFXFrameAllocator::~MFXFrameAllocator()
{
}

mfxStatus MFXFrameAllocator::Alloc_(mfxHDL pthis, mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXFrameAllocator& self = *(MFXFrameAllocator *)pthis;

    return self.AllocFrames(request, response);
}

mfxStatus MFXFrameAllocator::Lock_(mfxHDL pthis, mfxMemId mid, mfxFrameData *ptr)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXFrameAllocator& self = *(MFXFrameAllocator *)pthis;

    return self.LockFrame(mid, ptr);
}

mfxStatus MFXFrameAllocator::Unlock_(mfxHDL pthis, mfxMemId mid, mfxFrameData *ptr)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXFrameAllocator& self = *(MFXFrameAllocator *)pthis;

    return self.UnlockFrame(mid, ptr);
}

mfxStatus MFXFrameAllocator::Free_(mfxHDL pthis, mfxFrameAllocResponse *response)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXFrameAllocator& self = *(MFXFrameAllocator *)pthis;

    return self.FreeFrames(response);
}

mfxStatus MFXFrameAllocator::GetHDL_(mfxHDL pthis, mfxMemId mid, mfxHDL *handle)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXFrameAllocator& self = *(MFXFrameAllocator *)pthis;

    return self.GetFrameHDL(mid, handle);
}

BaseFrameAllocator::BaseFrameAllocator()
{
}

BaseFrameAllocator::~BaseFrameAllocator()
{
}

mfxStatus BaseFrameAllocator::CheckRequestType(mfxFrameAllocRequest *request)
{
    if (0 == request)
        return MFX_ERR_NULL_PTR;

    // check that Media SDK component is specified in request 
    if ((request->Type & MEMTYPE_FROM_MASK) != 0)
        return MFX_ERR_NONE;
    else
        return MFX_ERR_UNSUPPORTED;
}

mfxStatus BaseFrameAllocator::AllocFrames(mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    if (0 == request || 0 == response || 0 == request->NumFrameSuggested)
        return MFX_ERR_MEMORY_ALLOC; 

    if (MFX_ERR_NONE != CheckRequestType(request))
        return MFX_ERR_UNSUPPORTED;

    mfxStatus sts = MFX_ERR_NONE;

    if ( (request->Type & MFX_MEMTYPE_EXTERNAL_FRAME) && (request->Type & MFX_MEMTYPE_FROM_DECODE) )
    {
        // external decoder allocations
        
        if (m_externalDecoderResponse.m_refCount > 0)
        {            
            // check if enough frames were allocated
            if (request->NumFrameMin > m_externalDecoderResponse.m_response.NumFrameActual)
                return MFX_ERR_MEMORY_ALLOC;

            m_externalDecoderResponse.m_refCount++;
            // return existing response
            *response = m_externalDecoderResponse.m_response;
        }
        else 
        {
            sts = AllocImpl(request, response);
            if (sts == MFX_ERR_NONE)
            {
                m_externalDecoderResponse.m_response = *response;
                m_externalDecoderResponse.m_refCount = 1;
                m_externalDecoderResponse.m_type = request->Type & MEMTYPE_FROM_MASK;
            }           
        }
    }
    else
    {
        // internal allocations

        // reserve space before allocation to avoid memory leak
        m_responses.push_back(mfxFrameAllocResponse());

        sts = AllocImpl(request, response);
        if (sts == MFX_ERR_NONE)
        {
            m_responses.back() = *response;
        }
        else
        {
            m_responses.pop_back();
        }
    }

    return sts;
}

bool BaseFrameAllocator::IsSame(const mfxFrameAllocResponse& l, const mfxFrameAllocResponse& r)
{
    return l.mids != 0 && r.mids != 0 && l.mids[0] == r.mids[0] && l.NumFrameActual == r.NumFrameActual;
}

mfxStatus BaseFrameAllocator::FreeFrames(mfxFrameAllocResponse *response)
{
    if (response == 0)
        return MFX_ERR_INVALID_HANDLE;

    mfxStatus sts = MFX_ERR_NONE;
    
    // check whether response is m_externalDecoderResponse
    if (m_externalDecoderResponse.m_refCount > 0 && IsSame(*response, m_externalDecoderResponse.m_response))
    {
        if (--m_externalDecoderResponse.m_refCount == 0)
        {
            sts = ReleaseResponse(response);
            m_externalDecoderResponse.Reset();
        }
        return sts;
    }

    // if not found so far, then search in internal responses
    for (Iter i = m_responses.begin(); i != m_responses.end(); ++i)
    {
        if (IsSame(*response, *i))
        {
            sts = ReleaseResponse(response);
            m_responses.erase(i);
            return sts;
        }
    }

    // not found anywhere, report an error
    return MFX_ERR_INVALID_HANDLE;
}

mfxStatus BaseFrameAllocator::Close()
{
    ReleaseResponse(&(m_externalDecoderResponse.m_response));
    m_externalDecoderResponse.Reset();
    
    while (!m_responses.empty())
    {        
        ReleaseResponse(&(*m_responses.begin()));
        m_responses.pop_front();
    }
    
    return MFX_ERR_NONE;
}

MFXBufferAllocator::MFXBufferAllocator()
{    
    pthis = this;
    Alloc = Alloc_;
    Lock  = Lock_;
    Free  = Free_;
    Unlock = Unlock_;
}

MFXBufferAllocator::~MFXBufferAllocator()
{
}

mfxStatus MFXBufferAllocator::Alloc_(mfxHDL pthis, mfxU32 nbytes, mfxU16 type, mfxMemId *mid)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXBufferAllocator& self = *(MFXBufferAllocator *)pthis;

    return self.AllocBuffer(nbytes, type, mid);
}

mfxStatus MFXBufferAllocator::Lock_(mfxHDL pthis, mfxMemId mid, mfxU8 **ptr)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXBufferAllocator& self = *(MFXBufferAllocator *)pthis;

    return self.LockBuffer(mid, ptr);
}

mfxStatus MFXBufferAllocator::Unlock_(mfxHDL pthis, mfxMemId mid)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXBufferAllocator& self = *(MFXBufferAllocator *)pthis;

    return self.UnlockBuffer(mid);
}

mfxStatus MFXBufferAllocator::Free_(mfxHDL pthis, mfxMemId mid)
{
    if (0 == pthis)
        return MFX_ERR_MEMORY_ALLOC;

    MFXBufferAllocator& self = *(MFXBufferAllocator *)pthis;

    return self.FreeBuffer(mid);
}

