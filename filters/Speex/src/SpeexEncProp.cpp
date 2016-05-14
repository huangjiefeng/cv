//#include <afxwin.h>
#include <streams.h>
//#include <windowsx.h>
#include <commctrl.h>
#include <olectl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>


#include "resource.h"    // ids used in the dialog
#include "GUIDs.h"       

#include "SpeexEncIProp.h"

#include "SpeexEncProp.h"

CUnknown * WINAPI SpeexEncProp::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);
    
    CUnknown *punk = new SpeexEncProp(lpunk, phr);
	
    if (punk == NULL) {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }
	
    return punk;
}

SpeexEncProp::SpeexEncProp(LPUNKNOWN pUnk, HRESULT *phr)
    : CBasePropertyPage(NAME("SpeexEnc Property Page\0"),pUnk, IDD_DIALOG1, IDS_TITLE), m_sampleRate(0), m_pFilter(NULL)
{
    ASSERT(phr);
}


void GetString(UINT32 num, char* buf)
{
	
}


INT_PTR SpeexEncProp::OnReceiveMessage(HWND hwnd,
                                        UINT uMsg,
                                        WPARAM wParam,
                                        LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            // get the hWnd of the list box
            m_hwndSR1 = GetDlgItem(hwnd, IDC_RADIO_SR1);
			m_hwndSR2 = GetDlgItem(hwnd, IDC_RADIO_SR2);
			m_hwndSR3 = GetDlgItem(hwnd, IDC_RADIO_SR3);

			m_hwndSLD = GetDlgItem(hwnd, IDC_SLIDER2);
			m_hwndLBLQ = GetDlgItem(hwnd, IDC_QUAL);

			SendMessage(m_hwndSLD, TBM_SETRANGE, TRUE, MAKELPARAM(0, 10));
			
            return (LRESULT) 1;
        }

        case WM_COMMAND:
        {
            //if (HIWORD(wParam) == LBN_SELCHANGE)
                //SetDirty();

			if((UINT32)lParam==(UINT32)m_hwndSR1)
			{
				m_pFilter->put_SampleRate(8);
			}
			else if((UINT32)lParam==(UINT32)m_hwndSR2)
			{
				m_pFilter->put_SampleRate(16);
			}
			else if((UINT32)lParam==(UINT32)m_hwndSR3)
			{
				m_pFilter->put_SampleRate(32);
			}

            return (LRESULT) 1;
        }

		case WM_HSCROLL:
			{
				UINT32 qual = SendMessage(m_hwndSLD, TBM_GETPOS, 0, 0);
				m_pFilter->put_Quality(qual);

				char str[5] = "10";
				if(qual<10)
				{
					str[0] = qual + '0';
					str[1] = 0;
				}
				SendMessage(m_hwndLBLQ, WM_SETTEXT, 0, (long)str);
				return (LRESULT) 1;
			}
    }

    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);

} // OnReceiveMessage



HRESULT SpeexEncProp::OnConnect(IUnknown *pUnknown)
{
    CheckPointer(pUnknown,E_POINTER);
	pUnknown->QueryInterface(IID_SpeexEncIProp, (void**)(&m_pFilter));
	return S_OK;
} // OnConnect


HRESULT SpeexEncProp::OnDisconnect()
{
	if(m_pFilter)
	{
		m_pFilter->Release();
		m_pFilter = NULL;
	}
    return NOERROR;

} // OnDisconnect



HRESULT SpeexEncProp::OnActivate()
{
	UINT32 sr;
	m_pFilter->get_SampleRate(&sr);
	switch (sr)
	{
	case 8:
		SendMessage(m_hwndSR1, BM_SETCHECK, BST_CHECKED, 0);
		break;
	case 16:
		SendMessage(m_hwndSR2, BM_SETCHECK, BST_CHECKED, 0);
		break;
	case 32:
		SendMessage(m_hwndSR3, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}

	UINT32 q;
	m_pFilter->get_Quality(&q);
	SendMessage(m_hwndSLD, TBM_SETPOS, TRUE, q);
	char str[5] = "10";
	if(q<10)
	{
		str[0] = q + '0';
		str[1] = 0;
	}
	SendMessage(m_hwndLBLQ, WM_SETTEXT, 0, (long)str);

    return NOERROR;

} // Activate



HRESULT SpeexEncProp::OnApplyChanges()
{


    return NOERROR;

} // OnApplyChanges


