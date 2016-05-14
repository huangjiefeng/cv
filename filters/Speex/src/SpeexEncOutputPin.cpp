#include "SpeexEncOutputPin.h"
#include "GUIDs.h"

SpeexEncOutputPin::SpeexEncOutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName)
: CTransInPlaceOutputPin(pObjectName, pFilter, phr, pName)
{}

// Tell the preferred input type.
HRESULT SpeexEncOutputPin::GetMediaType(int iPosition,CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);

	CTransInPlaceInputPin* pIn = (CTransInPlaceInputPin*)(this->m_pTIPFilter->GetPin(0));
	if(pIn->IsConnected())
	{
		if(iPosition==0)
		{
			pIn->ConnectionMediaType(pMediaType);
	    pMediaType->SetSubtype(&MEDIASUBTYPE_SPEEX);

			//WAVEFORMATEX* pwf = (WAVEFORMATEX*)(pMediaType->Format());
			//pwf->wBitsPerSample = 0;
			return S_OK;
		}
		return VFW_S_NO_MORE_ITEMS;
	}

  return VFW_S_NO_MORE_ITEMS;

  /*if(iPosition<0)
	{
		return E_INVALIDARG;
	}

	if(iPosition>2)
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	pMediaType->SetType(&MEDIATYPE_Audio);
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
	}*/

	return S_OK;
}


/*STDMETHODIMP SpeexEncOutputPin::NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly)
{
	HRESULT hr;
	ALLOCATOR_PROPERTIES props;
	pAllocator->GetProperties(&props);
	AM_MEDIA_TYPE mt;
	this->ConnectionMediaType(&mt);
	WAVEFORMATEX wf;
	memcpy(&wf, mt.pbFormat, mt.cbFormat);

	ALLOCATOR_PROPERTIES props2;

	props.cbBuffer = wf.nSamplesPerSec*wf.wBitsPerSample*wf.nChannels*20/8/1000;
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

STDMETHODIMP SpeexEncOutputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
	(*ppEnum) = new CEnumMediaTypes(this, NULL);
	return S_OK;
}

HRESULT SpeexEncOutputPin::CheckMediaType( const CMediaType* pmt )
{
  if (pmt->majortype != MEDIATYPE_Audio || pmt->subtype != MEDIASUBTYPE_SPEEX) {
    return E_FAIL;
  }
  return S_OK;
}

HRESULT SpeexEncOutputPin::CompleteConnect( IPin *pReceivePin )
{
    //编解码器本不该使用TransInPlaceFilter，这种Filter是用来做同格式数据处理的
    //编解码后，输入输出格式不一样，会导致调用CTransInPlaceFilter::CompleteConnect时
    //断开已连接的输入Pin连接，为了避免这一问题，直接调CBaseOutputPin::CompleteConnect
    //Liaokz， 2014-5
    return CBaseOutputPin::CompleteConnect(pReceivePin);
}

