/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2011 Intel Corporation. All Rights Reserved.
//
*/
   
#include "mfx_video_dec_proppage.h"
#include "current_date.h"
#include "mfx_filter_defs.h"

#pragma warning( disable : 4748 )

/////////////////////////////////////////////////////////////////////////////
// Property Page Class
/////////////////////////////////////////////////////////////////////////////

CVideoDecPropPage::CVideoDecPropPage(IUnknown *pUnk) :
CBasePropertyPage(NAME("VideoProp"), pUnk, IDD_PROPPAGE_VIDEO, IDS_PROPPAGE_NAME)
{
    m_pVideoDecProps    = NULL;   
}

CUnknown* WINAPI CVideoDecPropPage::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CVideoDecPropPage *pNewObject = new CVideoDecPropPage(pUnk);
    if (pNewObject == NULL)
    {
        *pHr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

HRESULT CVideoDecPropPage::OnConnect(IUnknown *pUnk)
{
    MSDK_CHECK_POINTER(pUnk, E_POINTER);
    return pUnk->QueryInterface(IID_IConfigureVideoDecoder, reinterpret_cast<void**>(&m_pVideoDecProps));
}

static const UINT_PTR IDT_STATUPDATER = 1;

HRESULT CVideoDecPropPage::OnActivate(void)
{
    MSDK_CHECK_POINTER(m_pVideoDecProps, S_FALSE);

    CAutoLock cObjectLock(&m_critSec);  

    UINT_PTR uResult;

    uResult = SetTimer(m_hwnd, IDT_STATUPDATER, 100, (TIMERPROC)NULL);

    if (!uResult)
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CVideoDecPropPage::OnDeactivate(void)
{
    CAutoLock cObjectLock(&m_critSec);     
    
    KillTimer(m_hwnd, IDT_STATUPDATER);

    return S_OK;
}

INT_PTR CVideoDecPropPage::OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TIMER && wParam == IDT_STATUPDATER)
    {
        IConfigureVideoDecoder::Statistics stStats;

        HRESULT hr = S_OK;
        TCHAR strNumber[100];

        if (NULL == m_pVideoDecProps || NULL == m_Dlg)
        {
            return 1;
        }

        hr = m_pVideoDecProps->GetRunTimeStatistics(&stStats);

        if (S_OK != hr)
        {
            return 1;
        }

        _sntprintf(strNumber, 100, _T("%.2f"), stStats.decode_time / (double)1e2);
        SetDlgItemText(m_Dlg, IDC_EDIT_DecTime, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.frames_decoded);
        SetDlgItemText(m_Dlg, IDC_EDIT_DecFrame, strNumber);
        _sntprintf(strNumber, 100, _T("%.2f"), stStats.frame_rate / (double)100);
        SetDlgItemText(m_Dlg, IDC_Framerate, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.width);
        SetDlgItemText(m_Dlg, IDC_Width, strNumber);
        _sntprintf(strNumber, 100, _T("%d"), stStats.height);
        SetDlgItemText(m_Dlg, IDC_Height, strNumber);
    }
    
    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}

HRESULT CVideoDecPropPage::OnApplyChanges(void)
{
    return S_OK;
}

HRESULT CVideoDecPropPage::OnDisconnect(void)
{
    if (m_pVideoDecProps)
    {
        m_pVideoDecProps->Release();
        m_pVideoDecProps = NULL;
    }

    return S_OK;
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