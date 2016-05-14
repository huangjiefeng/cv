#include "SpeexDecInputPin.h"
#include "GUIDs.h"

SpeexDecInputPin::SpeexDecInputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName)
: CTransInPlaceInputPin(pObjectName, pFilter, phr, pName)
{}


// Here complete the work what SpeexEnc::CheckInputType should handles.
HRESULT SpeexDecInputPin::CheckMediaType(const CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);
	
	// check major type and sub type first!
  if (*pMediaType->Type() != MEDIATYPE_Audio ||
      *pMediaType->Subtype() != MEDIASUBTYPE_SPEEX) {
        return E_FAIL;
  }

	WAVEFORMATEX format;
	memcpy(&format, pMediaType->Format(), pMediaType->FormatLength());
	if(
		format.nChannels!=1 || 
		(format.nSamplesPerSec!=8000 && format.nSamplesPerSec!=16000 && format.nSamplesPerSec!=32000)/*
		 ||
				format.wBitsPerSample !=0*/
		)
	{
		return E_UNEXPECTED;
	}
	
	return S_OK;
}

// Tell the preferred input type.
HRESULT SpeexDecInputPin::GetMediaType(int iPosition,CMediaType *pMediaType)
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
	pMediaType->SetSubtype(&MEDIASUBTYPE_SPEEX);
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


/*STDMETHODIMP SpeexDecInputPin::NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly)
{
	HRESULT hr;
	ALLOCATOR_PROPERTIES props;
	pAllocator->GetProperties(&props);
	AM_MEDIA_TYPE mt;
	this->ConnectionMediaType(&mt);
	WAVEFORMATEX wf;
	memcpy(&wf, mt.pbFormat, mt.cbFormat);

	ALLOCATOR_PROPERTIES props2;

	//props.cbBuffer = wf.nSamplesPerSec*wf.wBitsPerSample*wf.nChannels*20/8/1000;
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
}*/

STDMETHODIMP SpeexDecInputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
	(*ppEnum) = new CEnumMediaTypes(this, NULL);
	return S_OK;
}