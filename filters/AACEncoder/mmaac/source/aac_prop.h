//-----------------------------------------------------------------------------
//
//	Monogram AAC Encoder
//
//	Base on GNU LGPL libfaac
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//
//	CAACPropertyPage class
//
//-----------------------------------------------------------------------------
class CAACPropertyPage : 
	public CBasePropertyPage,
	public IMonogramAACEncoderPropertyPage
{
public:

	// enkoder
	IMonogramAACEncoder		*encoder;

	void UpdateStats();
	void UpdateConfig();
public:
	CAACPropertyPage(LPUNKNOWN lpUnk, HRESULT *phr);
	virtual ~CAACPropertyPage();
	static CUnknown *WINAPI CreateInstance(LPUNKNOWN lpUnk, HRESULT *phr);

	DECLARE_IUNKNOWN;
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	// CBasePropertyPage
	HRESULT OnConnect(IUnknown *pUnknown);
	HRESULT OnDisconnect();
	HRESULT OnActivate();
	HRESULT OnDeactivate();
	HRESULT OnApplyChanges();


	// message
	INT_PTR OnReceiveMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// IMonogramAACEncoderPropertyPage
	STDMETHODIMP Update(int info_only);
	void SetDirty();

};

