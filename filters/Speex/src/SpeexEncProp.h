
class SpeexEncProp : public CBasePropertyPage
{
	
public:
	
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
    DECLARE_IUNKNOWN;
	
private:
	
    INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	
    HRESULT OnConnect(IUnknown *pUnknown);
    HRESULT OnDisconnect();
    HRESULT OnActivate();
    HRESULT OnApplyChanges();

	HWND m_hwndSR1;
	HWND m_hwndSR2;
	HWND m_hwndSR3;
	HWND m_hwndSLD;
	HWND m_hwndLBLQ;

	UINT32 m_sampleRate;
	SpeexEncIProp * m_pFilter;
	
    SpeexEncProp(LPUNKNOWN lpunk, HRESULT *phr);
};
