#include <streams.h>

class SpeexEncOutputPin : public CTransInPlaceOutputPin
{
public:
	SpeexEncOutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);

	// Provide an enumerator for media types by getting one from downstream
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );
	
    // Say whether media type is acceptable.
	HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);	
    HRESULT CheckMediaType(const CMediaType* pmt);
    HRESULT CompleteConnect(IPin *pReceivePin);
};