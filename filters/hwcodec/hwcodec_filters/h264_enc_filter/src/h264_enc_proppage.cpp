/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "afxwin.h"
#include "afxcmn.h"
#include "h264_enc_proppage.h"
#include "current_date.h"
#include "mfx_filter_guid.h"
#include "mfx_filter_defs.h"

/////////////////////////////////////////////////////////////////////////////
// Property Page Class
/////////////////////////////////////////////////////////////////////////////

CH264EncPropPage::CH264EncPropPage(IUnknown *pUnk) :
CConfigPropPage(pUnk, IDD_PROPPAGE_CONFIGURE, IDS_PROPPAGE_CONFIGURE)
{
    m_pH264EncProps    = NULL;

    InitDlgItemsArray();
}

CUnknown* WINAPI CH264EncPropPage::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CH264EncPropPage *pNewObject = new CH264EncPropPage(pUnk);
    if (pNewObject == NULL)
    {
        *pHr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

HRESULT CH264EncPropPage::OnConnect(IUnknown *pUnk)
{
    if (pUnk == NULL)
    {
        return E_POINTER;
    }

    ASSERT(m_pH264EncProps == NULL);

    HRESULT hr = pUnk->QueryInterface(IID_IConfigureVideoEncoder, reinterpret_cast<void**>(&m_pH264EncProps));
    CHECK_HRESULT(hr);
    MSDK_CHECK_POINTER(m_pH264EncProps, E_FAIL);

    m_pH264EncProps->GetParams(&m_EncoderParams);
    m_pEncProps = m_pH264EncProps;    

    return hr;
}

HRESULT CH264EncPropPage::FillCombos(void)
{
    HRESULT hr = S_OK;
    TCHAR* strItems[26];
    DWORD    nItemsData[26];

    //encoder presets

    //fill profile combo
    for (mfxU16 i = 0; i < CodecPreset::PresetsNum(); i++)
    {
        strItems[i]   = CodecPreset::Preset2Str(i);
        nItemsData[i] = i;
    }

    hr = FillCombo(IDC_COMBO_PROFILE, strItems, nItemsData, 5, 0);
    CHECK_HRESULT(hr);
    
    //fill throttle policy combo
    strItems[0] = _T("Auto Throttling");     nItemsData[0]  = 0;
    strItems[1] = _T("No Throttling");       nItemsData[1]  = 1;

    hr = FillCombo(IDC_COMBO_THROTTLE_POLICY, strItems, nItemsData, 2, 1);
    CHECK_HRESULT(hr);

    //fill bitrate combo
    strItems[0] = _T("192000");      nItemsData[0]  = 0;
    strItems[1] = _T("226000");      nItemsData[1]  = 1;

    hr = FillCombo(IDC_COMBO_BITRATE, strItems, nItemsData, 2, 0);
    CHECK_HRESULT(hr);

    //fill qp combo
    strItems[0] = _T("0");       nItemsData[0]  = 0;
    strItems[1] = _T("1");       nItemsData[1]  = 1;

    hr = FillCombo(IDC_COMBO_QP, strItems, nItemsData, 2, 0);
    CHECK_HRESULT(hr);

    //encoder params

    //fill profile2 combo
    strItems[0] = _T("Autoselect");  nItemsData[0] = 0;
    strItems[1] = _T("Baseline");    nItemsData[1] = 66;
    strItems[2] = _T("Main");        nItemsData[2] = 77;
    strItems[3] = _T("High");        nItemsData[3] = 100;
    strItems[4] = _T("High10");      nItemsData[4] = 110;
    strItems[5] = _T("High422");     nItemsData[5] = 122;

    hr = FillCombo(IDC_COMBO_PROFILE2, strItems, nItemsData, 6, m_EncoderParams.profile_idc);
    CHECK_HRESULT(hr);

    //fill level combo
    strItems[0]  = _T("Autoselect");    nItemsData[0]  = IConfigureVideoEncoder::Params::LL_AUTOSELECT;  
    strItems[1]  = _T("10");            nItemsData[1]  = IConfigureVideoEncoder::Params::LL_1;           
    strItems[2]  = _T("1b");            nItemsData[2]  = IConfigureVideoEncoder::Params::LL_1b;          
    strItems[3]  = _T("11");            nItemsData[3]  = IConfigureVideoEncoder::Params::LL_11;          
    strItems[4]  = _T("12");            nItemsData[4]  = IConfigureVideoEncoder::Params::LL_12;          
    strItems[5]  = _T("13");            nItemsData[5]  = IConfigureVideoEncoder::Params::LL_13;          
    strItems[6]  = _T("20");            nItemsData[6]  = IConfigureVideoEncoder::Params::LL_2;           
    strItems[7]  = _T("21");            nItemsData[7]  = IConfigureVideoEncoder::Params::LL_21;          
    strItems[8]  = _T("22");            nItemsData[8]  = IConfigureVideoEncoder::Params::LL_22;          
    strItems[9]  = _T("30");            nItemsData[9]  = IConfigureVideoEncoder::Params::LL_3;           
    strItems[10] = _T("31");            nItemsData[10] = IConfigureVideoEncoder::Params::LL_31;          
    strItems[11] = _T("32");            nItemsData[11] = IConfigureVideoEncoder::Params::LL_32;          
    strItems[12] = _T("40");            nItemsData[12] = IConfigureVideoEncoder::Params::LL_4;           
    strItems[13] = _T("41");            nItemsData[13] = IConfigureVideoEncoder::Params::LL_41;          
    strItems[14] = _T("42");            nItemsData[14] = IConfigureVideoEncoder::Params::LL_42;          
    strItems[15] = _T("50");            nItemsData[15] = IConfigureVideoEncoder::Params::LL_5;           
    strItems[16] = _T("51");            nItemsData[16] = IConfigureVideoEncoder::Params::LL_51;          

    hr = FillCombo(IDC_COMBO_LEVEl, strItems, nItemsData, 17, m_EncoderParams.level_idc);
    CHECK_HRESULT(hr);

    //fill PCMethod combo
    strItems[0] = _T("Progressive");     nItemsData[0]  = 0x01;
    strItems[1] = _T("Field TFF");       nItemsData[1]  = 0x02;
    strItems[2] = _T("Field BFF");       nItemsData[2]  = 0x04;

    hr = FillCombo(IDC_COMBO_PCCONTROL, strItems, nItemsData, 3, m_EncoderParams.pc_control);
    CHECK_HRESULT(hr);

    //fill RCMethod combo
    strItems[0] = _T("CBR");         nItemsData[0]  = 1;
    strItems[1] = _T("VBR");         nItemsData[1]  = 2;

    hr = FillCombo(IDC_COMBO_RCCONTROL, strItems, nItemsData, 2, m_EncoderParams.rc_control.rc_method);
    CHECK_HRESULT(hr);

    //fill Target Usage combo
    hr = FillTGUsageCombo(IDC_COMBO_TRGTUSE, m_EncoderParams.target_usage);
    CHECK_HRESULT(hr);

    return S_OK;
};

HRESULT CH264EncPropPage::ConfigureEditsSpins(void)
{
    HRESULT hr = S_OK;

    //edit and spin for PS I-Frame
    ConfigureEditSpin(IDC_SPIN_IFRAME, 0, 65535, m_EncoderParams.ps_control.GopPicSize, true);
    CHECK_HRESULT(hr);

    //edit and spin for PS B-Frame
    ConfigureEditSpin(IDC_SPIN_BFRAME, 0, 16, m_EncoderParams.ps_control.GopRefDist, true);
    CHECK_HRESULT(hr);

    //calculate slices number
    IConfigureVideoEncoder::Statistics statistics;
    DWORD nWidth(0), nHeight(0);

    m_pH264EncProps->GetRunTimeStatistics(&statistics);
    nWidth  = statistics.width;
    nHeight = statistics.height;

    DWORD nSliceX(0),  nSliceY(0);
    if (0 != (nWidth % 16))
    {
        nSliceX = (nWidth / 16) + 1;
    }
    else
    {
        nSliceX = (nWidth / 16);
    }

    if (0 != (nHeight % 16))
    {
        nSliceY= (nHeight / 16) + 1;
    }
    else
    {
        nSliceY = (nHeight / 16);
    }

    //edit and spin for PS Slices
    ConfigureEditSpin(IDC_SPIN_SLICES, 0, nSliceX * nSliceY, m_EncoderParams.ps_control.NumSlice, true);
    CHECK_HRESULT(hr);

    mfxU32 nMin(0), nMax(0), nRecommended(0);

    if (nWidth > 0)
    {
        GetBitrateRanges(nWidth, &nMin, &nMax, &nRecommended);
    }
    else
    {
        nMin         = 100;
        nMax         = 20 * 1000;
        nRecommended =  m_EncoderParams.rc_control.bitrate;
    }

    if (m_EncoderParams.rc_control.bitrate && 
        m_EncoderParams.rc_control.bitrate >= nMin && 
        m_EncoderParams.rc_control.bitrate <= nMax)
    {
        nRecommended = m_EncoderParams.rc_control.bitrate;
    }

    //edit and spin for RC Bitrate
    ConfigureEditSpin(IDC_SPIN_BITRATE, nMin, nMax, nRecommended, true);
    CHECK_HRESULT(hr);

    ConfigureEdit(IDC_EDIT_WIDTH, nWidth);
    ConfigureEdit(IDC_EDIT_HEIGHT, nHeight);
    return S_OK;
};

HRESULT CH264EncPropPage::OnActivate(void)
{
    HRESULT hr = S_OK;

    if (NULL == m_pH264EncProps)
    {
        return S_FALSE;
    }

    //fill all combo boxes
    hr = FillCombos();
    CHECK_HRESULT(hr);

    //fill all edits and spins
    hr = ConfigureEditsSpins();
    CHECK_HRESULT(hr);
    
    SetDlgItemData(IDC_COMBO_PRESET, m_EncoderParams.preset);
    
    UpdateDialogControls( MFX_CODEC_AVC
                        , IDC_COMBO_PROFILE
                        , IDC_COMBO_TRGTUSE
                        , IDC_EDIT_BITRATE
                        , IDC_SPIN_BITRATE);

    //EnableDialogControls();

    //return
    return S_OK;
}

void CH264EncPropPage::InitDlgItemsArray()
{
    CConfigPropPage::ItemVal items[] = 
    {
        {IDC_SPIN_BITRATE,    (DWORD*)0,                                             true},
        {IDC_SPIN_IFRAME,     (DWORD*)0,                                             true},
        {IDC_SPIN_BFRAME,     (DWORD*)0,                                             true},
        {IDC_SPIN_SLICES,     (DWORD*)0,                                             true},
        {IDC_EDIT_BFRAME,     (DWORD*)0,                                             true},

        {IDC_COMBO_PRESET,    (DWORD*)&m_EncoderParams.preset,                       false},

        //Target Usage
        {IDC_COMBO_TRGTUSE,   (DWORD*)&m_EncoderParams.target_usage,                 false},

        //Profile
        {IDC_COMBO_PROFILE2,  (DWORD*)&m_EncoderParams.profile_idc,                  true},

        //Level
        {IDC_COMBO_LEVEl,     (DWORD*)&m_EncoderParams.level_idc,                    true},

        //RCControl
        {IDC_EDIT_BITRATE,    (DWORD*)&m_EncoderParams.rc_control.bitrate,           true},
        {IDC_COMBO_RCCONTROL, (DWORD*)&m_EncoderParams.rc_control.rc_method,         true},

        //PSMethod
        {IDC_EDIT_IFRAME,     (DWORD*)&m_EncoderParams.ps_control.GopPicSize,  true},
        {IDC_EDIT_SLICES,     (DWORD*)&m_EncoderParams.ps_control.NumSlice,        true},

        //PCControl
        {IDC_COMBO_PCCONTROL, (DWORD*)&m_EncoderParams.pc_control,                   true},
        {IDC_EDIT_HEIGHT,     (DWORD*)&m_EncoderParams.frame_control.height,         false},
        {IDC_EDIT_WIDTH,      (DWORD*)&m_EncoderParams.frame_control.width,          false}
    };

    m_pItems = new CConfigPropPage::ItemVal[ARRAY_SIZE(items)];
    if (NULL == m_pItems)
    {
        return;
    }

    memcpy(m_pItems , items, sizeof (items));
    m_nItems = ARRAY_SIZE(items);
}

void CH264EncPropPage::LoadResults(mfxVideoParam * pMfxParams)
{
    CopyMFXToEncoderParams(&m_EncoderParams, pMfxParams);

    CConfigPropPage::LoadResults(pMfxParams);
}

HRESULT CH264EncPropPage::OnDeactivate(void)
{
    SaveResults();
    return S_OK;
}

HRESULT CH264EncPropPage::UpdateECControls()
{
    CComboBox* cwndCombo;
    HWND       hWnd;
    int        nRes = 0;

    hWnd = GetDlgItem(m_Dlg, IDC_COMBO_ECCONTROL);
    cwndCombo = (CComboBox*)CWnd::FromHandle(hWnd);

    nRes = cwndCombo->GetCurSel();
    WEnable(IDC_SPIN_CABAC, (BOOL)nRes);
    WEnable(IDC_EDIT_CABAC_INT, (BOOL)nRes);

    return S_OK;
};

HRESULT CH264EncPropPage::UpdateProfileControls()
{
    CComboBox* cwndCombo;
    HWND       hWnd;

    hWnd = GetDlgItem(m_Dlg, IDC_COMBO_PROFILE2);
    cwndCombo = (CComboBox*)CWnd::FromHandle(hWnd);

    if (1 == cwndCombo->GetCurSel())
    {
        WEnable(IDC_COMBO_ECCONTROL, 0);

        WEnable(IDC_SPIN_CABAC, 0);
        WEnable(IDC_EDIT_CABAC_INT, 0);

        WEnable(IDC_EDIT_BFRAME, FALSE);
        WEnable(IDC_SPIN_BFRAME, FALSE);

        WEnable(IDC_COMBO_PCCONTROL, FALSE);
    }
    else
    {
        CComboBox* cwndCombo;
        HWND       hWnd;
        int        nRes = 0;

        hWnd = GetDlgItem(m_Dlg, IDC_COMBO_ECCONTROL);
        cwndCombo = (CComboBox*)CWnd::FromHandle(hWnd);
        cwndCombo->EnableWindow(TRUE);
        nRes = cwndCombo->GetCurSel();
        WEnable(IDC_SPIN_CABAC, (BOOL)nRes);
        WEnable(IDC_EDIT_CABAC_INT, (BOOL)nRes);

        WEnable(IDC_EDIT_BFRAME, TRUE);
        WEnable(IDC_SPIN_BFRAME, TRUE);

        WEnable(IDC_COMBO_PCCONTROL, TRUE);
    }

    return S_OK;
};

INT_PTR CH264EncPropPage::OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_COMBO_PRESET)
        {
            GetDlgItemData(IDC_COMBO_PRESET, &m_EncoderParams.preset);

            UpdateDialogControls( MFX_CODEC_AVC
                                , IDC_COMBO_PROFILE
                                , IDC_COMBO_TRGTUSE
                                , IDC_EDIT_BITRATE
                                , IDC_SPIN_BITRATE);
        }else if (LOWORD(wParam) == IDC_COMBO_ECCONTROL)
        {
            UpdateECControls();
        }else if (LOWORD(wParam) == IDC_COMBO_RCCONTROL)
        {
            UpdateRateControls();
        }else if (LOWORD(wParam) == IDC_COMBO_PROFILE2)
        {
            UpdateProfileControls();
        }else if (LOWORD(wParam) == IDC_EDIT_BITRATE)
        {
            CSpinButtonCtrl* pcwndSpin;
            HWND             hWnd;
            CEdit*           pEdt;

            //get requested control
            hWnd = GetDlgItem(m_Dlg, IDC_SPIN_BITRATE);
            MSDK_CHECK_POINTER(hWnd, E_POINTER);

            pcwndSpin = (CSpinButtonCtrl*)CWnd::FromHandle(hWnd);  
            MSDK_CHECK_POINTER(pcwndSpin, E_POINTER);

            //get requested control
            hWnd = GetDlgItem(m_Dlg, IDC_EDIT_BITRATE);
            MSDK_CHECK_POINTER(hWnd, E_POINTER);

            pEdt = (CEdit*)CWnd::FromHandle(hWnd); 
            MSDK_CHECK_POINTER(pEdt, E_POINTER);

            int nLower(0), nUpper(0);
            pcwndSpin->GetRange32(nLower, nUpper);

            CString strBuffer;
            pEdt->GetWindowText(strBuffer);

            //in case of spin usage ',' is added for thousands.
            strBuffer.Remove(',');

            int nBitrate = _tstoi(strBuffer.GetBuffer());

            if (EN_KILLFOCUS == HIWORD(wParam) && ((nLower > nBitrate) || (nUpper < nBitrate)))
            {
                pcwndSpin->SetPos32(nLower);
            }
        }else if (LOWORD(wParam) == IDC_COMBO_TRGTUSE)
        {
            UpdateTGUsageControls( IDC_COMBO_TRGTUSE
                                 , IDC_EDIT_BITRATE
                                 , IDC_SPIN_BITRATE);
            //EnableDialogControls();
        }
        
        SetDirty();
        break;
    } // switch

    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}

HRESULT CH264EncPropPage::OnApplyChanges(void)
{
    HRESULT hr = S_OK;

    SaveResults();
    //save Params
    hr = m_pH264EncProps->SetParams(&m_EncoderParams);

    if (FAILED(hr))
    {
        MessageBox(this->m_hwnd, _T("An error occured! Possible reasons: filter is not connected with other filters, MSDK library cannot be found or parameters configuration is unsupported."), 0, 0);
    }
//    m_pH264EncProps->SetUsageProfile(m_UsageProfile);

    return S_OK;
}

HRESULT CH264EncPropPage::OnDisconnect(void)
{
    if (m_pH264EncProps)
    {
        m_pH264EncProps->Release();
        m_pH264EncProps = NULL;
    };

    CWnd::DeleteTempMap();

    return S_OK;
}
