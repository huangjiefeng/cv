/* ****************************************************************************** *\

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2008-2011 Intel Corporation. All Rights Reserved.

File Name: memory_allocator.h

\* ****************************************************************************** */

#ifndef __MEMORY_ALLOCATOR_H
#define __MEMORY_ALLOCATOR_H

#include "mfxdefs.h"
#include "mfxstructures.h"
#include "mfxvideo.h"
#include <memory.h>

#if 1 // ifdef D3D9_SUPPORT
#include <d3d9.h>
#include <mfapi.h>
#include <dxva2api.h>
#include <evr.h>
#include <mferror.h>
#endif

#include "atlbase.h"
#include "d3d_allocator.h"
#include "sysmem_allocator.h"
#include <streams.h>
#include "combase.h"

// {58A9922B-54B4-4386-A185-1FD38213B19F}
static const GUID IID_IMFXSample = 
{ 0x58a9922b, 0x54b4, 0x4386, { 0xa1, 0x85, 0x1f, 0xd3, 0x82, 0x13, 0xb1, 0x9f } };

// {DA8A2F72-334A-4c22-B0C3-C3A18D45FECA}
static const GUID IID_IMFXAllocator = 
{ 0xda8a2f72, 0x334a, 0x4c22, { 0xb0, 0xc3, 0xc3, 0xa1, 0x8d, 0x45, 0xfe, 0xca } };

// additional interface which allows to access mfxFrameSurface
MIDL_INTERFACE("58A9922B-54B4-4386-A185-1FD38213B19F")
IMFXSample : public IUnknown
{
    virtual STDMETHODIMP GetMfxSurfacePointer( 
        /* [out] */ mfxFrameSurface1 **ppSurf) = 0;
    virtual STDMETHODIMP SetMfxSurfacePointer(
        /* [in] */ mfxFrameSurface1 *pSurf) = 0;
};

// additional interface which allows to access MFXFrameAllocator
MIDL_INTERFACE("DA8A2F72-334A-4c22-B0C3-C3A18D45FECA")
IMFXAllocator : public IUnknown
{
    virtual STDMETHODIMP GetMfxFrameAllocator( 
        /* [out] */ MFXFrameAllocator **ppAlloc) = 0;
};

class CMFXSample : public CMediaSample, public IMFXSample
{
public:
    CMFXSample(LPCTSTR pName, CBaseAllocator* pBase, HRESULT *phr, LPBYTE pBuffer, LONG length);

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFXSample methods
    STDMETHODIMP GetMfxSurfacePointer(mfxFrameSurface1 **ppSurf); 
    STDMETHODIMP SetMfxSurfacePointer(mfxFrameSurface1 *pSurf);

protected:
    friend class MFXDSFrameAllocatorSys;    
    mfxFrameSurface1 *m_pmfxSurface;
};

class CDXVA2Sample : public CMFXSample, public IMFGetService
{    

public:
    CDXVA2Sample(CBaseAllocator* pBase, HRESULT *phr);

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);  
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFGetService methods
    STDMETHODIMP GetService(REFGUID guidService, REFIID riid, LPVOID *ppv);   

    // Override GetPointer because this class does not manage a system memory buffer.
    // The EVR uses the MR_BUFFER_SERVICE service to get the Direct3D surface.
    STDMETHODIMP GetPointer(BYTE ** ppBuffer);

    // IMFXSample methods
    STDMETHODIMP GetMfxSurfacePointer(mfxFrameSurface1 **ppSurf); 
    STDMETHODIMP SetMfxSurfacePointer(mfxFrameSurface1 *pSurf);

protected:    
    friend class MFXDSFrameAllocatorD3D;    
    IDirect3DSurface9 *m_pd3dSurface;
};

struct DSAllocatorParams : D3DAllocatorParams
{
    IPin* pPin;
};

class CDSMemAllocator : public CMemAllocator
{
public:
    DECLARE_IUNKNOWN;

    CDSMemAllocator(__in_opt LPCTSTR,  __inout_opt LPUNKNOWN, __inout HRESULT *);
    ~CDSMemAllocator() {};

    // IMemAllocator methods
    STDMETHODIMP SetProperties(ALLOCATOR_PROPERTIES* pRequest , ALLOCATOR_PROPERTIES* pActual);
    STDMETHODIMP GetProperties(ALLOCATOR_PROPERTIES* pActual);
    STDMETHODIMP Commit();
    STDMETHODIMP Decommit();
    STDMETHODIMP ReleaseBuffer(IMediaSample * pSample);
    STDMETHODIMP GetBuffer(__deref_out IMediaSample **ppBuffer, __in_opt REFERENCE_TIME *pStartTime
                                                             , __in_opt REFERENCE_TIME *pEndTime, DWORD dwFlags);
  

    void SetDXVA2 (BOOL bDXVA2) {m_bDXVA2 = bDXVA2;};

protected:        
    BOOL m_bDXVA2;
};

class MFXDSFrameAllocatorD3D 
    : public D3DFrameAllocator, public CDSMemAllocator, public IMFXAllocator
{
public:
    DECLARE_IUNKNOWN;

    MFXDSFrameAllocatorD3D(__in_opt LPCTSTR,  __inout_opt LPUNKNOWN, __inout HRESULT *);

    // INonDelegatingUnknown to add IMFXFrameAllocator interface
    STDMETHODIMP NonDelegatingQueryInterface(const IID & riid, void **ppv);

    // IMFXFrameAllocator
    STDMETHODIMP GetMfxFrameAllocator(MFXFrameAllocator **ppAlloc);    
    
    mfxStatus Init(mfxAllocatorParams *pParams);
    mfxStatus Close();

protected:
    mfxStatus AllocImpl (mfxFrameAllocRequest *request, mfxFrameAllocResponse *response);
    mfxStatus LockFrame (mfxMemId mid, mfxFrameData *ptr);
    mfxStatus UnlockFrame (mfxMemId mid, mfxFrameData *ptr);
    mfxStatus GetFrameHDL (mfxMemId mid, mfxHDL *handle);
    mfxStatus ReleaseResponse (mfxFrameAllocResponse *response);   
};

class MFXDSFrameAllocatorSys : public SysMemFrameAllocator, public CDSMemAllocator
{
public:
    MFXDSFrameAllocatorSys(__in_opt LPCTSTR,  __inout_opt LPUNKNOWN, __inout HRESULT *);  

protected:
    mfxStatus AllocImpl (mfxFrameAllocRequest *request, mfxFrameAllocResponse *response);
    mfxStatus LockFrame (mfxMemId mid, mfxFrameData *ptr);
    mfxStatus UnlockFrame (mfxMemId mid, mfxFrameData *ptr);
    mfxStatus ReleaseResponse (mfxFrameAllocResponse *response);    

    // override to allocate the memory when Commit called
    // in CMemAllocator implementation only change CMediaSample to CMFXSample
    virtual HRESULT Alloc(void); 

    mfxFrameInfo m_FrameInfo;
};

#endif




