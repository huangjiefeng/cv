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
#include "mfx_video_enc_filter_utils.h"

// {C2ADDA23-674B-4000-A174-B715877CB90B}
static const GUID IID_IH246EncPropertyPage =
{ 0xc2adda23, 0x674b, 0x4000, { 0xa1, 0x74, 0xb7, 0x15, 0x87, 0x7c, 0xb9, 0xb } };

// {AD6FCBD1-3915-45e0-8EBA-E4B2ABEDE6CE}
static const GUID CLSID_H246EncPropertyPage =
{ 0xad6fcbd1, 0x3915, 0x45e0, { 0x8e, 0xba, 0xe4, 0xb2, 0xab, 0xed, 0xe6, 0xce } };

#include "resource_enc.h"
#include "atlstr.h"
#include "mfx_video_enc_proppage.h"

class CH264EncPropPage : public CConfigPropPage
{
public:

    CH264EncPropPage(IUnknown *pUnk);

    HRESULT                     OnConnect(IUnknown *pUnk);
    HRESULT                     OnActivate(void);
    HRESULT                     OnDeactivate(void);
    INT_PTR                     OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HRESULT                     OnApplyChanges(void);
    HRESULT                     OnDisconnect(void);

    static CUnknown* WINAPI     CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);

private:

    static unsigned int         _stdcall ThreadProc(void* ptr);

    HRESULT                     FillCombos(void);
    HRESULT                     ConfigureEditsSpins(void);

    //from CConfigPropPage
    virtual void                LoadResults(mfxVideoParam*);
    virtual void                InitDlgItemsArray(void);
    virtual HRESULT             UpdateProfileControls(void);
    virtual HRESULT             UpdateECControls(void);

private:

    IConfigureVideoEncoder*                  m_pH264EncProps;    // Pointer to the filter's custom interface.
    IConfigureVideoEncoder::Params           m_EncoderParams;
};
