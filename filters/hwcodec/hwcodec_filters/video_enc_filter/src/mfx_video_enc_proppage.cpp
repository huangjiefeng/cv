/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2011 Intel Corporation. All Rights Reserved.
//
*/

#include <afxcmn.h>
#include "mfx_video_enc_proppage.h"
#include "mfx_filter_guid.h"
#include "current_date.h"
#include "mfx_filter_defs.h"

/////////////////////////////////////////////////////////////////////////////
// Property Page Class
/////////////////////////////////////////////////////////////////////////////

CVideoEncPropPage::CVideoEncPropPage(IUnknown *pUnk) :
CBasePropertyPage(NAME("VideoProp"), pUnk, IDD_PROPPAGE_VIDEO, IDS_PROPPAGE_NAME)
{
    m_pVideoEncProps    = NULL;
}

CUnknown* WINAPI CVideoEncPropPage::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CVideoEncPropPage *pNewObject = new CVideoEncPropPage(pUnk);
    if (pNewObject == NULL)
    {
        *pHr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

HRESULT CVideoEncPropPage::OnConnect(IUnknown *pUnk)
{
    MSDK_CHECK_POINTER(pUnk, E_POINTER);
    return pUnk->QueryInterface(IID_IConfigureVideoEncoder, reinterpret_cast<void**>(&m_pVideoEncProps));
}

static const UINT_PTR IDT_STATUPDATER = 1;

HRESULT CVideoEncPropPage::OnActivate(void)
{
    MSDK_CHECK_POINTER(m_pVideoEncProps, S_FALSE);

    CAutoLock cObjectLock(&m_critSec);      

    UINT_PTR uResult;

    uResult = SetTimer(m_hwnd, IDT_STATUPDATER, 100, (TIMERPROC)NULL);

    if (!uResult)
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CVideoEncPropPage::OnDeactivate(void)
{
    CAutoLock cObjectLock(&m_critSec);      

    KillTimer(m_hwnd, IDT_STATUPDATER);
    
    return S_OK;
}

INT_PTR CVideoEncPropPage::OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TIMER && wParam == IDT_STATUPDATER)
    {
        IConfigureVideoEncoder::Statistics stStats;

        HRESULT hr = S_OK;
        TCHAR strNumber[100];
        
        if (NULL == m_pVideoEncProps || NULL == m_Dlg)
        {
            return 1;
        }

        hr = m_pVideoEncProps->GetRunTimeStatistics(&stStats);

        if (S_OK != hr)
        {
            return 1;
        }

        _sntprintf(strNumber, 100, _T("%d"), stStats.real_bitrate);
        SetDlgItemText(m_Dlg, IDC_EDIT_DecTime, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.frames_encoded);
        SetDlgItemText(m_Dlg, IDC_EDIT_DecFrame, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.requested_bitrate);
        SetDlgItemText(m_Dlg, IDC_Bitrate, strNumber);
        _sntprintf(strNumber, 100, _T("%.2f"), stStats.frame_rate / (double)100);
        SetDlgItemText(m_Dlg, IDC_Framerate, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.width);
        SetDlgItemText(m_Dlg, IDC_Width, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.height);
        SetDlgItemText(m_Dlg, IDC_Height, strNumber);    
    }
    
    return CBasePropertyPage::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

HRESULT CVideoEncPropPage::OnApplyChanges(void)
{
    return S_OK;
}

HRESULT CVideoEncPropPage::OnDisconnect(void)
{    
    if (m_pVideoEncProps)
    {
        m_pVideoEncProps->Release();
        m_pVideoEncProps = NULL;
    }

    return S_OK;
}
//////////////////////////////////////////////////////////////////////////
//CConfigPropPage
//////////////////////////////////////////////////////////////////////////
CConfigPropPage::CConfigPropPage(IUnknown *pUnk,  int DialogId, int TitleId)
: CBasePropertyPage(NAME("Configure"), pUnk, DialogId, TitleId)
, m_pItems(NULL)
, m_nItems(0)
, m_pEncProps(NULL)
{
}

CConfigPropPage::~CConfigPropPage()
{
    delete [] m_pItems;
    m_pItems = NULL;
}

HRESULT CConfigPropPage::UpdateRateControls(void)
{
    return S_OK;
}

HRESULT CConfigPropPage::UpdateProfileControls(void)
{
    return S_OK;
}

HRESULT CConfigPropPage::UpdateECControls(void)
{
    return S_OK;
}

void CConfigPropPage::UpdateTGUsageControls( int nTGUsageDlgCombobox
                                           , int nBitrateItem
                                           , int nBitrateSpinItem)
{
    DWORD tgUsage;
    if (FAILED(GetDlgItemData(nTGUsageDlgCombobox, (DWORD*)&tgUsage)))
    {
        return ;
    }
    
    if (0 != tgUsage)
    {
        //not user defined
        EnableDialogControls(false);
        WEnable(nBitrateItem, true);
        WEnable(nBitrateSpinItem, true);
        return;
    }else
    {
        EnableDialogControls(true);

        //enables or disables some controls in user mode
        UpdateProfileControls();
        UpdateRateControls();
        UpdateECControls();

    }
}

void CConfigPropPage::UpdateDialogControls( mfxU32 codecID
                                          , int nPresetDlgCombobox
                                          , int nTGUsageDlgCombobox
                                          , int nBitrateItem
                                          , int nBitrateSpinItem)
{
    mfxU32 preset;

    if (FAILED(GetDlgItemData(nPresetDlgCombobox, (DWORD*)&preset)))
    {
        return ;
    }

    //saving preset value in array
    for (WORD i = 0; i < m_nItems; i++)
    {
        if (m_pItems[i].nDlgItem == nPresetDlgCombobox && m_pItems[i].pValue)
        {
            *m_pItems[i].pValue = preset;
        }
    }

    if (preset == CodecPreset::PRESET_USER_DEFINED)
    {
        EnableDialogControls(true);
        WEnable(nTGUsageDlgCombobox, true);
        UpdateTGUsageControls(nTGUsageDlgCombobox, nBitrateItem, nBitrateSpinItem);
        return ;
    }

    if (preset == CodecPreset::PRESET_LOW_LATENCY)
    {
        EnableDialogControls(false);
        WEnable(nTGUsageDlgCombobox, false);
        UpdateTGUsageControls(nTGUsageDlgCombobox, nBitrateItem, nBitrateSpinItem);
        return ;
    }

    EnableDialogControls(false);
    WEnable(nTGUsageDlgCombobox, false);

    mfxVideoParam vParam;
    IConfigureVideoEncoder::Statistics statistics;
    if (!m_pEncProps || FAILED(m_pEncProps->GetRunTimeStatistics(&statistics)))
        return ;

    ZeroMemory(&vParam, sizeof(vParam));
    vParam.mfx.CodecId                 = codecID;
    vParam.mfx.FrameInfo.Width         = (mfxU16)statistics.width;
    vParam.mfx.FrameInfo.Height        = (mfxU16)statistics.height;
    vParam.mfx.FrameInfo.CropX         = 0;
    vParam.mfx.FrameInfo.CropY         = 0;
    vParam.mfx.FrameInfo.CropW         = (mfxU16)statistics.width;
    vParam.mfx.FrameInfo.CropH         = (mfxU16)statistics.height;
    
    mfxU32 fcExtN(0), fcExtD(0);
    ConvertFrameRate( (mfxF64)statistics.frame_rate / 100.0
                    , &fcExtN
                    , &fcExtD);
    
    vParam.mfx.FrameInfo.FrameRateExtN = fcExtN;
    vParam.mfx.FrameInfo.FrameRateExtD = fcExtD;

    if (MFX_ERR_NONE != CodecPreset::VParamsFromPreset(vParam, preset))
        return ;

    LoadResults(&vParam);
}

void CConfigPropPage::LoadResults(mfxVideoParam*)
{
    if (NULL == m_pItems)
    {
        return;
    }
    for (DWORD i = 0; i < m_nItems; i++)
    {
        if (m_pItems[i].pValue != 0)
        {
            SetDlgItemData(m_pItems[i].nDlgItem, *(m_pItems[i].pValue));
        }
    }
}

void CConfigPropPage::SaveResults(void)
{
    if (NULL == m_pItems)
    {
        return;
    }
    for (WORD i = 0; i < m_nItems; i++)
    {
        if (m_pItems[i].pValue != 0)
        {
            GetDlgItemData(m_pItems[i].nDlgItem, m_pItems[i].pValue);
        }
    }
}

void CConfigPropPage::EnableDialogControls(bool bEnable)
{
    if (NULL == m_pItems)
    {
        return;
    }
    for (DWORD i = 0; i < m_nItems; i++)
    {
        if (m_pItems[i].bUseInEnabling)
        {
            WEnable(m_pItems[i].nDlgItem, bEnable);
        }
    }
}

void CConfigPropPage::SetDirty(void)
{
    m_bDirty = TRUE;

    if (m_pPageSite)
    {
        m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
    }
}

HRESULT  CConfigPropPage::FillCombo( int nCmd
                                   , TCHAR** strItems
                                   , DWORD* nItemsData
                                   , size_t nSize
                                   , int nSelected)
{
    HWND       hWnd;
    LRESULT    nRes;
    size_t     nSelectedIndex = 0;

    hWnd = GetDlgItem(m_Dlg, nCmd);
    MSDK_CHECK_POINTER(hWnd, E_POINTER);

    for (size_t i = 0; i < nSize; i++)
    {
        nRes  = SendMessage(hWnd, (UINT) CB_ADDSTRING  , 0, (LPARAM)strItems[i]);
        MSDK_CHECK_ERROR(nRes, CB_ERR, E_FAIL);

        nRes = SendMessage(hWnd, (UINT) CB_SETITEMDATA, (WPARAM)i, (LPARAM)nItemsData[i]);
        MSDK_CHECK_ERROR(nRes, CB_ERR, E_FAIL);

        if ((int)nItemsData[i] == nSelected)
        {
            nSelectedIndex = i;
        }
    }

    nRes = SendMessage(hWnd, (UINT) CB_SETCURSEL, (WPARAM)nSelectedIndex, 0);
    MSDK_CHECK_ERROR(nRes, CB_ERR, E_FAIL);

    return S_OK;
}

HRESULT CConfigPropPage::FillTGUsageCombo(int nTGUsageItem, DWORD nSelected)
{
    TCHAR*  strItems[26];
    DWORD   nItemsData[26];

    //fill Target Usage combo    
    strItems[0] = _T("Best Quality");    nItemsData[0]  = 1;    
    strItems[1] = _T("Balanced");        nItemsData[1]  = 4;    
    strItems[2] = _T("Best Speed");      nItemsData[2]  = 7;

    return FillCombo(nTGUsageItem, strItems, nItemsData, 3, nSelected);
}

HRESULT CConfigPropPage::ConfigureEdit(int nCmd, int nValue)
{
    HWND    hWnd;
    //get requested control
    hWnd = GetDlgItem(m_Dlg, nCmd);
    MSDK_CHECK_POINTER(hWnd, E_POINTER);

    CEdit* cwnd = (CEdit*)CWnd::FromHandle(hWnd);

    cwnd->SetLimitText(4);
    cwnd->ModifyStyle(0,ES_NUMBER,0);

    CString str;
    str.AppendFormat(_T("%d"), nValue);

    cwnd->SetWindowText(str);
    return S_OK;
};
HRESULT CConfigPropPage::ConfigureEditSpin( int nCmd
                                          , int nMin
                                          , int nMax
                                          , int nSelected
                                          , bool bModifyStyle)
{
    HWND             hWnd;
    CSpinButtonCtrl* pcwndSpin;
    CEdit*           pBuddyEdit;

    //get requested control
    hWnd = GetDlgItem(m_Dlg, nCmd);
    MSDK_CHECK_POINTER(hWnd, E_POINTER);

    pcwndSpin = (CSpinButtonCtrl*)CWnd::FromHandle(hWnd);
    MSDK_CHECK_POINTER(pcwndSpin, E_POINTER);

    //check nMin to be less then nMax
    if (nMin > nMax)
    {
        nMin = nMax = 0;
    }

    //check nSelected be in range
    nSelected = max(nSelected, nMin);
    nSelected = min(nSelected, nMax);

    //set parameters
    pcwndSpin->SetRange32(nMin, nMax);
    pcwndSpin->SetPos32(nSelected);

    //get max character count in nMin and Max
    CString strMin, strMax;
    strMin.AppendFormat(_T("%d"), nMin);
    strMax.AppendFormat(_T("%d"), nMax);
    int nTextLen = max(strMin.GetLength(), strMax.GetLength());

    //get spin's buddy
    pBuddyEdit = (CEdit*)pcwndSpin->GetBuddy();
    MSDK_CHECK_POINTER(pBuddyEdit, E_POINTER);

    //set limit to the characters number
    pBuddyEdit->SetLimitText(nTextLen);

    if (bModifyStyle)
    {
        //set only numbers support to the edit
        pBuddyEdit->ModifyStyle(0,ES_NUMBER,0);
    }

    return S_OK;
};

HRESULT CConfigPropPage::GetDlgItemData(int nDlgItem, DWORD* nParam)
{
    HWND             hWnd;
    TCHAR            classname[32];
    
    CheckPointer(nParam, E_POINTER);

    //get requested control
    hWnd = GetDlgItem(m_Dlg, nDlgItem);

    CheckPointer(hWnd, E_POINTER);
    CheckPointer(GetClassName(hWnd, classname, ARRAY_SIZE(classname)), E_POINTER);

    //we support only combobox and editbox
    if (!_tcsnicmp(classname, _T("combobox"), _tcslen(classname)))
    {
        LRESULT nIndex;

        nIndex  = SendMessage(hWnd, (UINT) CB_GETCURSEL  , 0, 0);
        *nParam = static_cast<DWORD> (SendMessage(hWnd, (UINT) CB_GETITEMDATA, (WPARAM)nIndex, 0));
    }
    else
    {
        TCHAR strVal[32];
        TCHAR *pChar;

        GetWindowText(hWnd, strVal, ARRAY_SIZE(strVal));
        //deletion of , and . characters in input str
        while (NULL != (pChar = _tcschr(strVal,_T(','))))
        {
            while (*(pChar+1) != 0 ){ *pChar = *(pChar+1); pChar++;}
            *pChar = *(pChar+1);
        }
        while (NULL != (pChar = _tcschr(strVal,_T('.'))))
        {
            while (*(pChar+1) != 0 ){ *pChar = *(pChar+1); pChar++;}
            *pChar = *(pChar+1);
        }

        *nParam = _tstol(strVal);
    }

    return S_OK;
};

HRESULT CConfigPropPage::SetDlgItemData(int nDlgItem, DWORD nParam)
{

    HWND             hWnd;
    TCHAR            classname[32];

    //get requested control
    hWnd = GetDlgItem(m_Dlg, nDlgItem);

    CheckPointer(hWnd, E_POINTER);
    CheckPointer(GetClassName(hWnd, classname, ARRAY_SIZE(classname)), E_POINTER);

    //we support only combobox and editbox
    if (!_tcsnicmp(classname, _T("combobox"), _tcslen(classname)))
    {
        LRESULT nCount;

        nCount = SendMessage(hWnd, (UINT) CB_GETCOUNT, 0, 0);

        for (mfxI32 i =0; i < nCount; i++)
        {
            if (nParam == (DWORD)SendMessage(hWnd, (UINT) CB_GETITEMDATA, (WPARAM)i, 0))
            {
                SendMessage(hWnd, (UINT) CB_SETCURSEL  , (WPARAM)i, 0);
                return S_OK;
            }
        }    
    }else
    {
        TCHAR value[32];

        _itot_s(nParam, value, ARRAY_SIZE(value), 10);

        SetWindowText(hWnd, value);
    }

    return S_OK;
}

void CConfigPropPage::WEnable(UINT nID, BOOL enable)
{
    HWND pw = ::GetDlgItem(m_Dlg, nID);
    if (pw==NULL)
        return;
    ::EnableWindow(pw, enable);
}

//////////////////////////////////////////////////////////////////////////
// About
//////////////////////////////////////////////////////////////////////////
CAboutPropPage::CAboutPropPage(IUnknown *pUnk) :
CBasePropertyPage(NAME("AboutProp"), pUnk, IDD_PROPPAGE_ABOUT, IDS_PROPPAGE_ABOUT)
{
    m_pAboutProps       = NULL;
}

CUnknown* WINAPI CAboutPropPage::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CAboutPropPage *pNewObject = new CAboutPropPage(pUnk);
    if (pNewObject == NULL)
    {
        *pHr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

HRESULT CAboutPropPage::OnConnect(IUnknown *pUnk)
{
    MSDK_CHECK_POINTER(pUnk, E_POINTER);
    return pUnk->QueryInterface(IID_IAboutPropertyPage, reinterpret_cast<void**>(&m_pAboutProps));
}

HRESULT CAboutPropPage::OnActivate(void)
{
    HRESULT hr = S_OK;

    MSDK_CHECK_POINTER(m_pAboutProps, S_FALSE);

    hr = m_pAboutProps->GetFilterName(&m_bstrFilterName);

    MSDK_CHECK_RESULT(hr, S_OK, S_FALSE);

    SetDlgItemText(m_Dlg, IDC_FilterName, m_bstrFilterName.Detach() );
    SetDlgItemText(m_Dlg, IDC_VERSION,    _T(FILE_VERSION_STRING) );

    const TCHAR* strProductVersion;
    strProductVersion = _tcschr(_T(PRODUCT_VERSION_STRING), _T(','));
    MSDK_CHECK_POINTER(strProductVersion, S_FALSE);
    strProductVersion = _tcschr(strProductVersion + 1, _T(','));
    MSDK_CHECK_POINTER(strProductVersion, S_FALSE);

    TCHAR strMFXVersion[10];

    size_t nMFXVersionLen = _tcslen(_T(PRODUCT_VERSION_STRING))- _tcslen(strProductVersion);

    memcpy(strMFXVersion, _T(PRODUCT_VERSION_STRING), nMFXVersionLen * sizeof(TCHAR));
    strMFXVersion[nMFXVersionLen] = 0;

    SetDlgItemText(m_Dlg, IDC_VERSION_MFX, strMFXVersion);   
   
    TCHAR strCopyright[100] = _T("");
    memcpy(strCopyright, _T(PRODUCT_COPYRIGHT), _tcslen(_T(PRODUCT_COPYRIGHT)) * sizeof(TCHAR));

    SetDlgItemText(m_Dlg, IDC_Copyright, strCopyright);
    SetDlgItemText(m_Dlg, IDC_URL, _T("http://www.intel.com"));

    return S_OK;
}

HRESULT CAboutPropPage::OnDisconnect(void)
{
    if (m_pAboutProps)
    {
        m_pAboutProps->Release();
        m_pAboutProps = NULL;
    };

    return S_OK;
};