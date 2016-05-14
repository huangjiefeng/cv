/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2008-2011 Intel Corporation. All Rights Reserved.

File Name: memory_allocator.cpp

\* ****************************************************************************** */

#include "memory_allocator.h"
#include <stdlib.h>

#include "streams.h"
#include "dvdmedia.h"
#include "mfx_filter_defs.h"

#define ALLOCATOR_PROP_SIZE sizeof(ALLOCATOR_PROPERTIES)

/* CMFXSample */ 
CMFXSample::CMFXSample(LPCTSTR pName, CBaseAllocator* pBase, HRESULT *phr, LPBYTE pBuffer, LONG length) :
CMediaSample(pName, pBase, phr, pBuffer, length),
m_pmfxSurface(NULL)
{
}

HRESULT CMFXSample::QueryInterface(const IID & riid, void **ppv)
{
    if (IsEqualIID(riid, IID_IMFXSample))
    {
        return GetInterface((IMFXSample*)this, ppv);
    }

    return CMediaSample::QueryInterface(riid, ppv);
}

ULONG CMFXSample::AddRef()
{
    return CMediaSample::AddRef();
}

ULONG CMFXSample::Release()
{
    return CMediaSample::Release();
}

HRESULT CMFXSample::GetMfxSurfacePointer(mfxFrameSurface1 **ppSurf)
{
    CheckPointer(ppSurf, E_POINTER);
    *ppSurf = m_pmfxSurface;
    return S_OK;
}

HRESULT CMFXSample::SetMfxSurfacePointer(mfxFrameSurface1 *pSurf)
{
    CheckPointer(pSurf, E_POINTER);
    m_pmfxSurface = pSurf;
    return S_OK;
}

/* CDXVA2Sample */
CDXVA2Sample::CDXVA2Sample(CBaseAllocator* pBase, HRESULT *phr) :
CMFXSample(NAME("CDXVA2Sample"), pBase, phr, NULL, 0),
m_pd3dSurface(NULL)
{
}

HRESULT CDXVA2Sample::QueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv, E_POINTER);

    if (riid == __uuidof(IMFGetService))
    {
        return GetInterface((IUnknown*)static_cast<IMFGetService*>(this), ppv);
    }
    
    return CMFXSample::QueryInterface(riid, ppv);
}

ULONG CDXVA2Sample::AddRef()
{
    return CMFXSample::AddRef();
}

ULONG CDXVA2Sample::Release()
{
    /* Decrement our own private reference count */
    LONG lRef;    

    if (m_cRef == 0)
    {
        return S_OK;
    } 
    else if (m_cRef == 1) 
    {
        lRef = 0;
        m_cRef = 0;
    } 
    else 
    {
        lRef = InterlockedDecrement(&m_cRef);
    }

    ASSERT(lRef >= 0);

    /* Did we release our final reference count */
    if (lRef == 0) 
    {
        /* Free all resources */
        if (m_dwFlags & Sample_TypeChanged) 
        {
            SetMediaType(NULL);
        }
        
        m_dwFlags = 0;
        m_dwTypeSpecificFlags = 0;
        m_dwStreamId = AM_STREAM_MEDIA;
    }

    return (ULONG)lRef;
}

HRESULT CDXVA2Sample::GetService(REFGUID guidService, REFIID riid, LPVOID *ppv)
{
    if (guidService != MR_BUFFER_SERVICE)
    {
        return MF_E_UNSUPPORTED_SERVICE;
    }
    else if (m_pd3dSurface == NULL)
    {
        return E_NOINTERFACE;
    }
    else
    {
        HRESULT hr = m_pd3dSurface->QueryInterface(riid, ppv);

        return hr;
    }
}

HRESULT CDXVA2Sample::GetPointer(BYTE ** ppBuffer)
{
    return E_NOTIMPL;
}

HRESULT CDXVA2Sample::GetMfxSurfacePointer(mfxFrameSurface1 **ppSurf)
{
    CheckPointer(ppSurf, E_POINTER);
    *ppSurf = m_pmfxSurface;
    return S_OK;
}

HRESULT CDXVA2Sample::SetMfxSurfacePointer(mfxFrameSurface1 *pSurf)
{
    CheckPointer(pSurf, E_POINTER);
    m_pmfxSurface = pSurf;
    return S_OK;
}

/* CDSMemAllocator */
CDSMemAllocator::CDSMemAllocator(__in_opt LPCTSTR str,  __inout_opt LPUNKNOWN pUnknown, __inout HRESULT * phr) : 
CMemAllocator(str, pUnknown, phr)
{    
    m_bDXVA2 = false;
};

HRESULT CDSMemAllocator::SetProperties(ALLOCATOR_PROPERTIES *pRequest, ALLOCATOR_PROPERTIES *pActual)
{
    return CMemAllocator::SetProperties(pRequest, pActual);
}

HRESULT CDSMemAllocator::GetProperties(ALLOCATOR_PROPERTIES *pActual)
{
    return CMemAllocator::GetProperties(pActual);
}

HRESULT CDSMemAllocator::Commit() 
{ 
    if (m_bDXVA2)
    {
        return S_OK; 
    }

    return CMemAllocator::Commit(); 
};

HRESULT CDSMemAllocator::Decommit()
{ 
    if (m_bDXVA2)
    {
        return S_OK; 
    }

    return CMemAllocator::Decommit();  
};

HRESULT CDSMemAllocator::GetBuffer(__deref_out IMediaSample **ppBuffer
                                           , __in_opt REFERENCE_TIME *pStartTime
                                           , __in_opt REFERENCE_TIME *pEndTime
                                           , DWORD dwFlags) 
{ 
    if (m_bDXVA2)
    {
        return E_NOTIMPL; // we don't use GetBuffer() for DXVA2
    }  

    return CMemAllocator::GetBuffer(ppBuffer, pStartTime, pEndTime, dwFlags);
} 

HRESULT CDSMemAllocator::ReleaseBuffer(IMediaSample *pSample)  
{ 
    if (m_bDXVA2)
    {
        return E_NOTIMPL; // we don't use ReleaseBuffer() for DXVA2
    }

    return CMemAllocator::ReleaseBuffer(pSample);
} 

MFXDSFrameAllocatorD3D::MFXDSFrameAllocatorD3D(__in_opt LPCTSTR str,  __inout_opt LPUNKNOWN pUnknown, __inout HRESULT * phr) : 
CDSMemAllocator(str, pUnknown, phr)
{    
    m_bDXVA2 = true;
};

HRESULT MFXDSFrameAllocatorD3D::NonDelegatingQueryInterface(const IID & riid, void **ppv)
{
    if (IsEqualIID(riid, IID_IMFXAllocator))
    {
        return GetInterface((IMFXAllocator*)this, ppv);
    }

    return CMemAllocator::NonDelegatingQueryInterface(riid, ppv);
}

mfxStatus MFXDSFrameAllocatorD3D::Init(mfxAllocatorParams *pParams)
{
    mfxStatus                                   sts             = MFX_ERR_NONE;
    HRESULT                                     hr              = S_OK;    
    CComPtr<IMFGetService>                      pGetService(NULL);

    CheckPointer(pParams, MFX_ERR_NULL_PTR);

    DSAllocatorParams *pDSParams = NULL;
    pDSParams = dynamic_cast<DSAllocatorParams*>(pParams);
    MSDK_CHECK_POINTER(pDSParams, MFX_ERR_NULL_PTR);    
   
    hr = pDSParams->pPin->QueryInterface(__uuidof(IMFGetService), (void**)&pGetService);        

    if (SUCCEEDED(hr))
    {
        m_manager.Release();
        hr = pGetService->GetService(MR_VIDEO_ACCELERATION_SERVICE, IID_IDirect3DDeviceManager9, (void**)&m_manager);        
    }

    if (SUCCEEDED(hr))
    {
        pDSParams->pManager = m_manager; 
    }
    else
    {
        sts = MFX_ERR_UNKNOWN;
    }    

    return sts;
};

mfxStatus MFXDSFrameAllocatorD3D::Close()
{  
    return MFX_ERR_NONE;
}

mfxStatus MFXDSFrameAllocatorD3D::AllocImpl(mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    mfxStatus sts = MFX_ERR_NONE;

    mfxFrameAllocResponse Response;

    memcpy(&Response, response, sizeof(mfxFrameAllocResponse));

    sts = D3DFrameAllocator::AllocImpl(request, &Response);

    if (MFX_ERR_NONE == sts)
    {
        response->NumFrameActual = Response.NumFrameActual;

        CDXVA2Sample** pSample = new CDXVA2Sample * [response->NumFrameActual];
        for (mfxU16 i = 0; i<Response.NumFrameActual; i++)
        {
            HRESULT hr = S_OK;
            pSample[i] = new CDXVA2Sample(this, &hr);
            pSample[i]->AddRef();
            pSample[i]->m_pd3dSurface = (IDirect3DSurface9*)Response.mids[i];
        }

        delete[] Response.mids;

        response->mids = (mfxMemId*)pSample;
    }

    return sts;
}

mfxStatus MFXDSFrameAllocatorD3D::LockFrame(mfxMemId mid, mfxFrameData *ptr)
{
    mfxStatus           sts          = MFX_ERR_NONE;
    IDirect3DSurface9*  pD3D9Surface = NULL;

    sts = GetFrameHDL(mid, (mfxHDL*)&pD3D9Surface);

    if (MFX_ERR_NONE == sts)
    {
        sts = D3DFrameAllocator::LockFrame(pD3D9Surface, ptr);
    } 

    return sts;
};

mfxStatus MFXDSFrameAllocatorD3D::UnlockFrame(mfxMemId mid, mfxFrameData *ptr)
{
    mfxStatus           sts          = MFX_ERR_NONE;
    IDirect3DSurface9*  pD3D9Surface = NULL;

    sts = GetFrameHDL(mid, (mfxHDL*)&pD3D9Surface);

    CDXVA2Sample* pSample = (CDXVA2Sample*)mid;

    if (MFX_ERR_NONE == sts)
    {
        sts = D3DFrameAllocator::UnlockFrame(pD3D9Surface, ptr);        
    }

    return MFX_ERR_NONE;
};

mfxStatus MFXDSFrameAllocatorD3D::GetFrameHDL(mfxMemId mid, mfxHDL *handle)
{
    HRESULT             hr          = S_OK;
    mfxStatus           sts         = MFX_ERR_NONE;
    CDXVA2Sample*        pSample     = (CDXVA2Sample*)mid;
    IMFGetService*      pGetService = NULL;
    IDirect3DSurface9*  pD3DSurface = NULL;

    if (!pSample || !handle) 
    {
        return MFX_ERR_NULL_PTR;
    }

    // get IDirect3DSurface9 via IMFGetService
    hr = pSample->QueryInterface(__uuidof(IMFGetService), (void**)&pGetService);

    if (SUCCEEDED(hr))
    {
        hr = pGetService->GetService(MR_BUFFER_SERVICE, __uuidof(IDirect3DSurface9), (void**)&pD3DSurface);        
        pGetService->Release();
    }

    if (SUCCEEDED(hr))
    {       
        *handle = pD3DSurface;
        pD3DSurface->Release();
    }
    else
    {
        sts = MFX_ERR_UNKNOWN;
    }    

    return sts;
};

mfxStatus MFXDSFrameAllocatorD3D::ReleaseResponse(mfxFrameAllocResponse *response)
{
    if (!response)
        return MFX_ERR_NULL_PTR;
    
    if (response->mids)
    {
        for (mfxU32 i = 0; i < response->NumFrameActual; i++)
        {
            if (response->mids[i])
            {
                IDirect3DSurface9* handle = 0;
                if (GetFrameHDL(response->mids[i], (mfxHDL *)&handle) == MFX_ERR_NONE)
                    handle->Release();
            }

            CDXVA2Sample* pSample = (CDXVA2Sample*)response->mids[i];
            pSample->Release();
            delete pSample;
        }
    }    

    delete [] response->mids;
    response->mids = NULL;

    return MFX_ERR_NONE;
};

HRESULT MFXDSFrameAllocatorD3D::GetMfxFrameAllocator( MFXFrameAllocator **ppAlloc)
{
    CheckPointer(ppAlloc, E_POINTER);
    *ppAlloc = (D3DFrameAllocator*)this;
    return S_OK;
}

/* MFXDSFrameAllocatorSys */
MFXDSFrameAllocatorSys::MFXDSFrameAllocatorSys(__in_opt LPCTSTR str,  __inout_opt LPUNKNOWN pUnknown, __inout HRESULT * phr) : 
CDSMemAllocator(str, pUnknown, phr)
{    
    m_bDXVA2 = false;
    MSDK_ZERO_MEMORY(m_FrameInfo);
};

// in CMemAllocator implementation only change CMediaSample to CMFXSample
HRESULT MFXDSFrameAllocatorSys::Alloc(void)
{
    CAutoLock lck(this);

    /* Check he has called SetProperties */
    HRESULT hr = CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    /* If the requirements haven't changed then don't reallocate */
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); // we use this fact in the loop below

    /* Free the old resources */
    if (m_pBuffer) {
        ReallyFree();
    }

    /* Make sure we've got reasonable values */
    if ( m_lSize < 0 || m_lPrefix < 0 || m_lCount < 0 ) {
        return E_OUTOFMEMORY;
    }

    /* Compute the aligned size */
    LONG lAlignedSize = m_lSize + m_lPrefix;

    /*  Check overflow */
    if (lAlignedSize < m_lSize) {
        return E_OUTOFMEMORY;
    }

    if (m_lAlignment > 1) {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) {
            LONG lNewSize = lAlignedSize + m_lAlignment - lRemainder;
            if (lNewSize < lAlignedSize) {
                return E_OUTOFMEMORY;
            }
            lAlignedSize = lNewSize;
        }
    }

    /* Create the contiguous memory block for the samples
    making sure it's properly aligned (64K should be enough!)
    */
    ASSERT(lAlignedSize % m_lAlignment == 0);

    LONGLONG lToAllocate = m_lCount * (LONGLONG)lAlignedSize;

    /*  Check overflow */
    if (lToAllocate > MAXLONG) {
        return E_OUTOFMEMORY;
    }

    m_pBuffer = (PBYTE)VirtualAlloc(NULL,
        (LONG)lToAllocate,
        MEM_COMMIT,
        PAGE_READWRITE);

    if (m_pBuffer == NULL) {
        return E_OUTOFMEMORY;
    }

    LPBYTE pNext = m_pBuffer;
    CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    // Create the new samples - we have allocated m_lSize bytes for each sample
    // plus m_lPrefix bytes per sample as a prefix. We set the pointer to
    // the memory after the prefix - so that GetPointer() will return a pointer
    // to m_lSize bytes.
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {


        pSample = new CMFXSample(
            NAME("CMFXSample"),
            this,
            &hr,
            pNext + m_lPrefix,      // GetPointer() value
            m_lSize);               // not including prefix

        ASSERT(SUCCEEDED(hr));
        if (pSample == NULL) {
            return E_OUTOFMEMORY;
        }

        // This CANNOT fail
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;

}

mfxStatus MFXDSFrameAllocatorSys::AllocImpl(mfxFrameAllocRequest *request, mfxFrameAllocResponse *response)
{
    // this allocator supports only external requests
    if (!(request->Type & MFX_MEMTYPE_EXTERNAL_FRAME))
        return MFX_ERR_UNSUPPORTED;

    if (!(request->Type & MFX_MEMTYPE_SYSTEM_MEMORY))
        return MFX_ERR_UNSUPPORTED;

    mfxStatus sts = MFX_ERR_NONE;     

    safe_array<CMFXSample*> ppSamples = new CMFXSample *[request->NumFrameSuggested];
    MSDK_CHECK_POINTER(ppSamples.get(), MFX_ERR_MEMORY_ALLOC);
    for (mfxU16 i = 0; i < request->NumFrameSuggested; i++)
    {      
        HRESULT hr = GetBuffer((IMediaSample**)&ppSamples.get()[i], NULL, NULL, 0);
        if (FAILED(hr))
            return MFX_ERR_MEMORY_ALLOC;
    }

    response->mids = (mfxMemId*)ppSamples.release();
    response->NumFrameActual = request->NumFrameSuggested;
    // store allocated sizes
    memcpy(&m_FrameInfo, &request->Info, sizeof(mfxFrameInfo));
    return sts;
}

mfxStatus MFXDSFrameAllocatorSys::LockFrame(mfxMemId mid, mfxFrameData *ptr)
{   
    MSDK_CHECK_POINTER(ptr, MFX_ERR_NULL_PTR);    

    HRESULT hr = S_OK;

    CMFXSample *pSample = (CMFXSample *)mid;
    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR);    

    mfxU8 *pBuffer = 0;    
    pSample->GetPointer(&pBuffer);

    mfxU32 BufSize = pSample->GetSize();      
    
    mfxU16 Width = m_FrameInfo.Width;
    mfxU16 Height = m_FrameInfo.Height;
    mfxU16 Pitch = BufSize / Height * 2 / 3;

    // TODO: add size check

    ptr->Y = (mfxU8 *)pBuffer;
    switch (m_FrameInfo.FourCC) 
    {
    case MFX_FOURCC_NV12:
        ptr->U = ptr->Y + Width * Height;
        ptr->V = ptr->U + 1;
        ptr->Pitch = Pitch;
        break;    
    default:
        return MFX_ERR_UNSUPPORTED;    
    }

    return MFX_ERR_NONE;
};

mfxStatus MFXDSFrameAllocatorSys::UnlockFrame(mfxMemId mid, mfxFrameData *ptr)
{
    if (NULL != ptr)
    {
        ptr->Pitch = 0;
        ptr->Y     = 0;
        ptr->U     = 0;
        ptr->V     = 0;
    }        

    return MFX_ERR_NONE;
};

mfxStatus MFXDSFrameAllocatorSys::ReleaseResponse(mfxFrameAllocResponse *response)
{
    MSDK_CHECK_POINTER(response, MFX_ERR_NULL_PTR);

    if (response->mids)
    {
        for (mfxU32 i = 0; i < response->NumFrameActual; i++)
        {
            if (response->mids[i])
            {
                CMFXSample* pSample = (CMFXSample *)response->mids[i];
                if (pSample)
                    pSample->Release();                
            }                   
        }
    }    

    delete [] response->mids;
    response->mids = NULL;

    return MFX_ERR_NONE;
};
