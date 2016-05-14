#include "SpeexEncInputPin.h"

SpeexEncInputPin::SpeexEncInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName)
: CTransInPlaceInputPin(pObjectName, pFilter, phr, pName)
{
	//add by dsh 10-08-05
	bActive=true;
	//end by dsh
}


// Here complete the work what SpeexEnc::CheckInputType should handles.
HRESULT SpeexEncInputPin::CheckMediaType(const CMediaType *pMediaType)
{

	return m_pTIPFilter->CheckInputType(pMediaType);
	//return S_OK;
}

// Tell the preferred input type.
HRESULT SpeexEncInputPin::GetMediaType(int iPosition,CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);

	if(iPosition<0)
	{
		return E_INVALIDARG;
	}

	if(iPosition>2)
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	pMediaType->SetType(&MEDIATYPE_Audio);
	pMediaType->SetSubtype(&MEDIASUBTYPE_PCM);
	pMediaType->AllocFormatBuffer(sizeof(WAVEFORMATEX));
	pMediaType->SetFormatType(&FORMAT_WaveFormatEx);
	WAVEFORMATEX* pwf = (WAVEFORMATEX*)(pMediaType->Format());

	if(iPosition==0)
	{
		pwf->cbSize = 0;
		pwf->wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		pwf->nBlockAlign = 2;
		pwf->nChannels = 1;
		pwf->wBitsPerSample = 16;
		pwf->nSamplesPerSec = 32000;
	}
	else if(iPosition==1)
	{
		pwf->cbSize = 0;
		pwf->wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		pwf->nBlockAlign = 2;
		pwf->nChannels = 1;
		pwf->wBitsPerSample = 16;
		pwf->nSamplesPerSec = 16000;
	}
	else if(iPosition==2)
	{
		pwf->cbSize = 0;
		pwf->wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		pwf->nBlockAlign = 2;
		pwf->nChannels = 1;
		pwf->wBitsPerSample = 16;
		pwf->nSamplesPerSec = 8000;
	}

	return S_OK;
}


STDMETHODIMP SpeexEncInputPin::NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly)
{
	//UINT32 delayTime = 25;
	HRESULT hr;
	ALLOCATOR_PROPERTIES props;
	pAllocator->GetProperties(&props);
	AM_MEDIA_TYPE mt;
	this->ConnectionMediaType(&mt);
	WAVEFORMATEX wf;
	memcpy(&wf, mt.pbFormat, mt.cbFormat);

	ALLOCATOR_PROPERTIES props2;

	props.cbBuffer = wf.nSamplesPerSec*wf.wBitsPerSample*wf.nChannels*20/8/1000*delayTime;
	props.cbAlign = wf.wBitsPerSample*wf.nChannels/8;

	hr = pAllocator->SetProperties(&props, &props2);
	if(FAILED(hr))
	{
		return E_FAIL;
	}
	// check props2


	pAllocator->AddRef();
	
	if( m_pAllocator != NULL )
		m_pAllocator->Release();
	
	m_pAllocator = pAllocator; 

	return S_OK;
}

STDMETHODIMP SpeexEncInputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
	(*ppEnum) = new CEnumMediaTypes(this, NULL);
	return S_OK;
}

//add by dsh 10-08-05
STDMETHODIMP SpeexEncInputPin::Receive(IMediaSample * pSample)
{
	if(!bActive)
	{
		return NOERROR;
	}
	CTransInPlaceInputPin::Receive(pSample);
}
//end by dsh 