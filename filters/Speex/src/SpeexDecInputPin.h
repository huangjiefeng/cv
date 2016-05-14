#include <streams.h>

class SpeexDecInputPin : public CTransInPlaceInputPin
{
public:
	SpeexDecInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);

	// Provide an enumerator for media types by getting one from downstream
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );
	
    // Say whether media type is acceptable.
    HRESULT CheckMediaType(const CMediaType* pmt);

	HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);
	
    // get told which allocator the upstream output pin is actually
    // going to use.
    //STDMETHODIMP NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly);
};