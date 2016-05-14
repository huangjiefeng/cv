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
#include "codec_presets.h"

#include "mfx_video_enc_filter_utils.h"

// {36CF5834-D643-48b7-BB4D-95C1A910C419}
static const GUID IID_IVideoPropertyPage =
{ 0x36cf5834, 0xd643, 0x48b7, { 0xbb, 0x4d, 0x95, 0xc1, 0xa9, 0x10, 0xc4, 0x19 } };

// {718913B6-A51C-4784-9762-8CE3BD0595A0}
static const GUID CLSID_VideoPropertyPage =
{ 0x718913b6, 0xa51c, 0x4784, { 0x97, 0x62, 0x8c, 0xe3, 0xbd, 0x5, 0x95, 0xa0 } };

// {D826B2E2-52B6-4f51-8C6F-8C29FB6F1F3D}
static const GUID IID_IAboutPropertyPage =
{ 0xd826b2e2, 0x52b6, 0x4f51, { 0x8c, 0x6f, 0x8c, 0x29, 0xfb, 0x6f, 0x1f, 0x3d } };

// {A7064786-4BF6-4a5a-B975-5D78303E7C56}
static const GUID CLSID_AboutPropertyPage = 
{ 0xa7064786, 0x4bf6, 0x4a5a, { 0xb9, 0x75, 0x5d, 0x78, 0x30, 0x3e, 0x7c, 0x56 } };

#include "resource.h"
#include "atlstr.h"

#define CHECK_RESULT_FAIL(P) {if((P) < 0) return E_FAIL;}
#define CHECK_HRESULT(P) {if(S_OK != (P)) return E_FAIL;}


interface IVideoEncoderProperty : public IUnknown
{

};

interface IAboutProperty : public IUnknown
{
    STDMETHOD(GetFilterName)(BSTR* pName) = 0;
};

class CVideoEncPropPage : public CBasePropertyPage
{
public:

    CVideoEncPropPage(IUnknown *pUnk);

    HRESULT                     OnConnect(IUnknown *pUnk);
    HRESULT                     OnActivate(void);
    HRESULT                     OnDeactivate(void);
    INT_PTR                     OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HRESULT                     OnApplyChanges(void);
    HRESULT                     OnDisconnect(void);

    static CUnknown* WINAPI     CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);    
private:

    IConfigureVideoEncoder*     m_pVideoEncProps;    // Pointer to the filter's custom interface.
    
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

class CConfigPropPage : public CBasePropertyPage
{

protected:
    CConfigPropPage(IUnknown *pUnk, int DlgId, int TitleId);
    ~CConfigPropPage();

    //should fill both m_pItems Array and m_nItems variable
    virtual void                InitDlgItemsArray(void) = 0;

    // disables or enable all dlgitems that in m_pItems array
    void                        EnableDialogControls(bool bEnable);

    //sets parameters to dlg controls
    virtual void                LoadResults(mfxVideoParam*);
    //retrieves parameters from dlg controls
    void                        SaveResults(void);

    //updates controls according to selected preset - template method
            void                UpdateDialogControls( mfxU32 codecID
                                                    , int nPresetDlgCombobox
                                                    , int nTGUsageDlgCombobox
                                                    , int nBitrateItem
                                                    , int nBitrateSpinItem);
            void                UpdateTGUsageControls( int nTGUsageDlgCombobox
                                                     , int nBitrateItem
                                                     , int nBitrateSpinItem);
    virtual HRESULT             UpdateRateControls(void);
    virtual HRESULT             UpdateProfileControls(void);
    virtual HRESULT             UpdateECControls(void);

    //set/get data from comboboxes and editboxes
    HRESULT                     GetDlgItemData(int nDlgItem, DWORD* pParam);
    HRESULT                     SetDlgItemData(int nDlgItem, DWORD nParam);

    //enables/disables dlg item
    void                        WEnable(UINT nID, BOOL enable);
    void                        SetDirty(void);

    //fils specified combo box with string values
    HRESULT                     FillCombo( int nCmd
                                         , TCHAR** strItems
                                         , DWORD* nItemsData
                                         , size_t nSize
                                         , int nSelected);
    
    HRESULT                     ConfigureEditSpin( int nCmd
                                                 , int nMin
                                                 , int nMax
                                                 , int nSelected
                                                 , bool bModifyStyle);
    HRESULT                     ConfigureEdit(int nCmd, int nValue);
    
    HRESULT                     FillTGUsageCombo(int nTGUsageItem, DWORD nSelected);

protected:

    struct  ItemVal
    {
        int     nDlgItem;
        DWORD * pValue;
        bool    bUseInEnabling;
    }*                          m_pItems;
    DWORD                       m_nItems;
    IConfigureVideoEncoder    * m_pEncProps;

private:
    DISALLOW_COPY_AND_ASSIGN(CConfigPropPage);
 };
 