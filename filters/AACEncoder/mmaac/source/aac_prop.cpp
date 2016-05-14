//-----------------------------------------------------------------------------
//
//	Monogram AAC Encoder
//
//	Base on GNU LGPL libfaac
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------
#include "stdafx.h"


#define MAX_TEXT_LENGTH			1024
#define WM_UPDATE_VISUALS		(WM_USER + 1003)
#define ITEM(x)					(GetDlgItem(m_Dlg, x))

const	int			BitRateTab[] = {
	32000,
	40000,
	48000,
	56000,
	64000,
	72000,
	80000,
	88000,
	96000,
	104000,
	112000,
	120000,
	128000,
	140000,
	160000,
	192000,
	224000,
	256000
};
const	int			BitRateCount = sizeof(BitRateTab) / sizeof(BitRateTab[0]);

//-----------------------------------------------------------------------------
//
//	CAACPropertyPage class
//
//-----------------------------------------------------------------------------

CUnknown *CAACPropertyPage::CreateInstance(LPUNKNOWN lpUnk, HRESULT *phr)
{
	return new CAACPropertyPage(lpUnk, phr);
}

CAACPropertyPage::CAACPropertyPage(LPUNKNOWN lpUnk, HRESULT *phr) :
	CBasePropertyPage(NAME("AAC Property Page"), lpUnk, IDD_PAGE_AAC, IDS_PAGE_AAC),
	encoder(NULL)
{
}

CAACPropertyPage::~CAACPropertyPage()
{
}

STDMETHODIMP CAACPropertyPage::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IMonogramAACEncoderPropertyPage) {
		return GetInterface((IMonogramAACEncoderPropertyPage*)this, ppv);
	} else
	return __super::NonDelegatingQueryInterface(riid, ppv);
}



HRESULT CAACPropertyPage::OnConnect(IUnknown *pUnknown)
{
	ASSERT(!encoder);

	HRESULT hr = pUnknown->QueryInterface(IID_IMonogramAACEncoder, (void**)&encoder);
	if (FAILED(hr)) return hr;

	IUnknown *pUnk;
		NonDelegatingQueryInterface(IID_IUnknown, (void**)&pUnk);
		encoder->SetPropertyPage(pUnk);
	pUnk->Release();

	// okej
	return NOERROR;
}

HRESULT CAACPropertyPage::OnDisconnect()
{
	if (encoder) {
		encoder->SetPropertyPage(NULL);
		encoder->Release();
		encoder = NULL;
	}
	return NOERROR;
}

HRESULT CAACPropertyPage::OnActivate()
{
	// kazdych 500 milisekund updatneme rychlost stahovania
	SetTimer(m_Dlg, 0, 500, NULL);

	// vytvorime itemy do propery pagesy
	HWND	wndVersion = ITEM(IDC_CB_VERSION);
	ComboBox_AddString(wndVersion, _T("MPEG-4"));
	ComboBox_AddString(wndVersion, _T("MPEG-2"));

	HWND	wndObject = ITEM(IDC_CB_TYPE);
	ComboBox_AddString(wndObject, _T("Main"));
	ComboBox_AddString(wndObject, _T("Low Complexity"));
	ComboBox_AddString(wndObject, _T("Scalable Sampling Rate"));
	ComboBox_AddString(wndObject, _T("Long Term Predictor"));

	HWND	wndBitrate = ITEM(IDC_CB_BITRATE);
	for (int i=0; i<BitRateCount; i++) {
		CString		br;
		br.Format(_T("%d"), BitRateTab[i] / 1000);
		ComboBox_AddString(wndBitrate, br);
	}

	HWND	wndOutput = ITEM(IDC_CB_OUTPUT);
	ComboBox_AddString(wndOutput, _T("Raw"));
	ComboBox_AddString(wndOutput, _T("ADTS"));
	ComboBox_AddString(wndOutput, _T("LATM"));

	UpdateStats();
	UpdateConfig();
	return NOERROR;
}

HRESULT CAACPropertyPage::OnDeactivate()
{
	KillTimer(m_Dlg, 0);
	return NOERROR;
}

HRESULT CAACPropertyPage::OnApplyChanges()
{
	/*
		Updatneme nastavenia bitratu
	*/
	AACConfig	config;
	config.version		= ComboBox_GetCurSel(ITEM(IDC_CB_VERSION));
	config.object_type	= 1 + ComboBox_GetCurSel(ITEM(IDC_CB_TYPE));
	config.bitrate		= BitRateTab[ComboBox_GetCurSel(ITEM(IDC_CB_BITRATE))];
	config.output_type	= ComboBox_GetCurSel(ITEM(IDC_CB_OUTPUT));

	// nastavime
	if (encoder) encoder->SetConfig(&config);

	return NOERROR;
}


INT_PTR CAACPropertyPage::OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		{
			int	cmd = HIWORD(wParam);
			if (cmd == CBN_SELCHANGE) {
				SetDirty();
			}
		}
		break;
	case WM_UPDATE_VISUALS:
		{
			UpdateStats();
			if (wParam == 0) UpdateConfig();
		}
		break;
	case WM_TIMER:
		{
			// updatujeme rychlost stahovania
			UpdateStats();
		}
		break;
	}
	return __super::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

STDMETHODIMP CAACPropertyPage::Update(int info_only)
{
	PostMessage(m_Dlg, WM_UPDATE_VISUALS, info_only, 0);
	return NOERROR;
}

void CAACPropertyPage::UpdateStats()
{
	if (!encoder) return ;
	AACInfo		info;
	CString		v;
	encoder->GetInfo(&info);

	// nastavime info

	// samplerate
	if (info.samplerate != 0) {
		v.Format(_T("%5.3f Khz"), (float)info.samplerate / 1000.0);
	} else {
		v = _T("Unknown");
	}
	Static_SetText(ITEM(IDC_LABEL_SAMPLERATE), v);

	// channels
	if (info.channels != 0) {
		v.Format(_T("%d"), info.channels);
	} else {
		v = _T("Unknown");
	}
	Static_SetText(ITEM(IDC_LABEL_CHANNELS), v);


	// frame size
	if (info.frame_size != 0) {
		v.Format(_T("%d samples"), info.frame_size);
	} else {
		v = _T("Unknown");
	}
	Static_SetText(ITEM(IDC_LABEL_FRAMESIZE), v);

	// frames
	v.Format(_T("%I64d"), info.frames_done);
	Static_SetText(ITEM(IDC_LABEL_FRAMES), v);
}

void CAACPropertyPage::UpdateConfig()
{
	if (!encoder) return ;

	AACConfig	config;
	encoder->GetConfig(&config);

	// nastavime hodnoty
	HWND	wndVersion = ITEM(IDC_CB_VERSION);
	HWND	wndObject = ITEM(IDC_CB_TYPE);
	HWND	wndBitrate = ITEM(IDC_CB_BITRATE);
	HWND	wndOutput = ITEM(IDC_CB_OUTPUT);

	ComboBox_SetCurSel(wndVersion, config.version);
	ComboBox_SetCurSel(wndObject, config.object_type-1);
	ComboBox_SetCurSel(wndOutput, config.output_type);

	// bitrate
	int		i=0;
	while (i<BitRateCount && BitRateTab[i] < config.bitrate) i++;
	ComboBox_SetCurSel(wndBitrate, i);
}

void CAACPropertyPage::SetDirty()
{
	m_bDirty = TRUE;
    if (m_pPageSite) {
        m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
    }
}


