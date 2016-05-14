#include <streams.h>

class SpeexEncInputPin : public CTransInPlaceInputPin
{
public:
	SpeexEncInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);

	// Provide an enumerator for media types by getting one from downstream
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );
	
    // Say whether media type is acceptable.
    HRESULT CheckMediaType(const CMediaType* pmt);

	HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);
	
    // get told which allocator the upstream output pin is actually
    // going to use.
    STDMETHODIMP NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly);

	//add by dsh 10-08-05
	STDMETHODIMP Receive(IMediaSample * pSample);
	bool bActive;
	//end by dsh 

	UINT32 delayTime;
};