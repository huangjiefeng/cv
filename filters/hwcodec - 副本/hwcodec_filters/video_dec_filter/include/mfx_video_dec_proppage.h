/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2011 Intel Corporation. All Rights Reserved.
//
*/

#pragma once

#include "streams.h"
#include "atlbase.h"
#include "mfx_video_dec_filter_utils.h"

#include "mfxdefs.h"
#include "mfxstructures.h"
#include "mfxvideo++.h"


// {36CF5834-D643-48b7-BB4D-95C1A910C419}
static const GUID IID_IVideoPropertyPage =
{ 0x36cf5834, 0xd643, 0x48b7, { 0xbb, 0x4d, 0x95, 0xc1, 0xa9, 0x10, 0xc4, 0x19 } };

// {F8D88E75-35AA-4828-A034-E4EC7FF25664}
static const GUID CLSID_VideoPropertyPage =
{ 0xf8d88e75, 0x35aa, 0x4828, { 0xa0, 0x34, 0xe4, 0xec, 0x7f, 0xf2, 0x56, 0x64 } };

// {D826B2E2-52B6-4f51-8C6F-8C29FB6F1F3D}
static const GUID IID_IAboutPropertyPage =
{ 0xd826b2e2, 0x52b6, 0x4f51, { 0x8c, 0x6f, 0x8c, 0x29, 0xfb, 0x6f, 0x1f, 0x3d } };

// {CB0EA180-378D-4dc7-9853-5FC3021B962C}
static const GUID CLSID_AboutPropertyPage =
{ 0xcb0ea180, 0x378d, 0x4dc7, { 0x98, 0x53, 0x5f, 0xc3, 0x2, 0x1b, 0x96, 0x2c } };

#include "resource.h"
#include "atlstr.h"
#include "mfx_filter_externals.h"
#include "mfx_filter_guid.h"

interface IVideoDecoderProperty : public IUnknown
{

};

interface IAboutProperty : public IUnknown
{
    STDMETHOD(GetFilterName)(BSTR* pName) = 0;
};

class CVideoDecPropPage : public CBasePropertyPage
{
public:

    CVideoDecPropPage(IUnknown *pUnk);

    HRESULT                     OnConnect(IUnknown *pUnk);
    HRESULT                     OnActivate(void);
    HRESULT                     OnDeactivate(void);
    INT_PTR                     OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HRESULT                     OnApplyChanges(void);
    HRESULT                     OnDisconnect(void);

    static CUnknown* WINAPI     CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);

private:

    static unsigned int _stdcall ThreadProc(void* ptr);

private:

    IConfigureVideoDecoder*     m_pVideoDecProps;    // Pointer to the filter's custom interface.
    LONG                        m_lVal;
    LONG                        m_lNewVal;
    
    CCritSec                    m_critSec;       
};

class CAboutPropPage : public CBasePropertyPage
{
public:

    CAboutPropPage(IUnknown *pUnk);

    HRESULT                     OnConnect(IUnknown *pUnk);
    HRESULT                     OnActivate(void);

    HRESULT                     OnDisconnect(void);

    static CUnknown* WINAPI     CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);

private:
    IAboutProperty*             m_pAboutProps;    // Pointer to the filter's custom interface.
    CComBSTR                    m_bstrFilterName;
};