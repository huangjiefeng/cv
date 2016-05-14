#include <streams.h>     // DirectShow (includes windows.h)
#include <initguid.h>    // declares DEFINE_GUID to declare an EXTERN_C const.
#include <stdio.h>
#include <atlbase.h>
//#include <iostream.h>
#include "speex/speex.h"
#include "speex/speex_bits.h"

#include "SpeexDecInputPin.h"
#include "GUIDs.h"

#include "SpeexDecIProp.h"
#include "CommonSetupData.h"

const AMOVIESETUP_FILTER sudNullNull =
{ 
  &CLSID_SpeexDec                 // clsID
, L"SpeexDec"                 // strName
, MERIT_DO_NOT_USE                // dwMerit
, 2                               // nPins
, psudPins                        // lpPin
};
//----------------------------------------------------------------



class SpeexDecOutputPin : public CTransInPlaceOutputPin
{
public:
    SpeexDecOutputPin(TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);

    // Say whether media type is acceptable.
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
    HRESULT CheckMediaType(const CMediaType* pmt);
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );
    HRESULT CompleteConnect(IPin *pReceivePin);

};


// CNullNull
//
class CSpeexDec
    : public CTransInPlaceFilter,SpeexDecIProp
{
	void *dec_state;
	UINT delayTime;
	UINT sampleRate;
	UINT32 quality;


public:

    friend class SpeexDecOutputPin;

public:
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
    DECLARE_IUNKNOWN;

private:

    // Constructor - just calls the base class constructor
    CSpeexDec(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
        : CTransInPlaceFilter (tszName, punk, CLSID_SpeexDec, phr), delayTime(4), sampleRate(32000)
    {
		
		dec_state = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_UWB));

		spx_int32_t tmp;
		//spx_int32_t skip_group_delay; 

		tmp=1; 
		speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &tmp); 

		tmp=0; 
		speex_decoder_ctl(dec_state, SPEEX_SET_HIGHPASS, &tmp); 
		
		/*speex_encoder_ctl(enc_state, SPEEX_GET_LOOKAHEAD, &skip_group_delay); 
		speex_decoder_ctl(dec_state, SPEEX_GET_LOOKAHEAD, &tmp); 
		skip_group_delay += tmp; */

		speex_decoder_ctl(dec_state, SPEEX_SET_SAMPLING_RATE, &sampleRate);
	}

	~CSpeexDec()
	{
		speex_decoder_destroy(dec_state);
	}

    HRESULT Transform(IMediaSample *pSample)
	{
    //input
		BYTE* buf;
		pSample->GetPointer(&buf);
		int enc_stream_size = pSample->GetActualDataLength();

    //计算相关参数
    int dec_frame_size;
    speex_decoder_ctl(dec_state,SPEEX_GET_FRAME_SIZE,(void*)&dec_frame_size);
    int bytes_per_dec_frame = dec_frame_size * 2;
    BYTE dec_buf[1280*30] = {0};
    int dec_total_bytes = 0;
    int rem_bits = 0;

    //输入待解码流
    SpeexBits bits;
    speex_bits_init(&bits);
    speex_bits_read_from(&bits, (char*)buf, enc_stream_size);
    //解码，每次一帧，直至解码整个流
    do {
      int err = speex_decode_int(dec_state, &bits, (spx_int16_t*)(dec_buf + dec_total_bytes));
      if (err == 0){
        dec_total_bytes += bytes_per_dec_frame;
      }
    } while ((rem_bits = speex_bits_remaining(&bits)) > 10);

    speex_bits_destroy(&bits);

    //设置输出缓冲区
		memcpy(buf, dec_buf, dec_total_bytes);
		if(NOERROR==pSample->SetActualDataLength(dec_total_bytes))
		{
			int a=1;
		}

		return NOERROR;
	}

public:

    HRESULT CheckInputType(const CMediaType *pMediaType)
	{
		
		return S_OK;
	}

	CBasePin * GetPin(int n)
	{
		HRESULT hr = S_OK;
		
		// Create an input pin if not already done
		
		if (m_pInput == NULL) {
			
			m_pInput = new SpeexDecInputPin( NAME("SpeexDec input pin")
				, this        // Owner filter
				, &hr         // Result code
				, L"Input"    // Pin name
				);
			
			// Constructor for CTransInPlaceInputPin can't fail
			ASSERT(SUCCEEDED(hr));
		}
		
		// Create an output pin if not already done
		
		if (m_pInput!=NULL && m_pOutput == NULL) {
			
			m_pOutput = new SpeexDecOutputPin( NAME("SpeexDec output pin")
				, this       // Owner filter
				, &hr        // Result code
				, L"Output"  // Pin name
				);
			
			// a failed return code should delete the object
			
			ASSERT(SUCCEEDED(hr));
			if (m_pOutput == NULL) {
				delete m_pInput;
				m_pInput = NULL;
			}
		}
		
		// Return the appropriate pin
		
		ASSERT (n>=0 && n<=1);
		if (n == 0) {
			return m_pInput;
		} else if (n==1) {
			return m_pOutput;
		} else {
			return NULL;
		}
		
	}

	//by dsh 10-07-29

		STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv)
	{
		CheckPointer(ppv,E_POINTER);
		
		if (riid == IID_SpeexDecIProp) {
			return GetInterface((SpeexDecIProp *) this, ppv);
		}
		else {
			return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
		}
		
	}

	STDMETHODIMP put_SampleRate(unsigned int rate)
	{
		if(rate==8000 || rate==16000 || rate==32000)
		{
			//by dsh 10-07-29
			if(sampleRate!=rate)
			{
				sampleRate = rate;
				speex_decoder_destroy(dec_state);
				switch(sampleRate)
				{
				case 8000:
					dec_state = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
					break;
				case 16000:
					dec_state = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));
					break;
				case 32000:
					dec_state = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_UWB));
					break;
				}
				
				spx_int32_t tmp;
				tmp=1; 
				speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &tmp); 
				tmp=0; 
				speex_decoder_ctl(dec_state, SPEEX_SET_HIGHPASS, &tmp); 
				speex_decoder_ctl(dec_state, SPEEX_SET_SAMPLING_RATE, &sampleRate);
				
				int sampleRatevar=sampleRate*1000;
				speex_decoder_ctl(dec_state, SPEEX_SET_SAMPLING_RATE, &sampleRatevar);
			}
			return S_OK;
		//end by dsh 
		}
		return E_INVALIDARG;
	}

	STDMETHODIMP get_SampleRate(unsigned int* pRate)
	{
		CheckPointer(pRate, E_POINTER);
		*pRate = sampleRate;
		return S_OK;
	}

	STDMETHODIMP put_Quality(UINT32 Quality)
	{
		if(Quality)
		{
			quality = Quality;
			speex_decoder_ctl(dec_state, SPEEX_SET_QUALITY, &quality);
			return S_OK;
		}
		return E_INVALIDARG;
	}
	
	STDMETHODIMP get_Quality(UINT32* pQuality)
	{
		CheckPointer(pQuality, E_POINTER);
		*pQuality = quality;
		return S_OK;
	}
	//end by dsh 10-07-29
};


//----------------------------------------------------------------


SpeexDecOutputPin::SpeexDecOutputPin( TCHAR *pObjectName, CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName )
    : CTransInPlaceOutputPin(pObjectName, pFilter, phr, pName)
{
}

HRESULT SpeexDecOutputPin::GetMediaType( int iPosition, CMediaType *pMediaType )
{
    CheckPointer(pMediaType,E_POINTER);

    CTransInPlaceInputPin* pIn = (CTransInPlaceInputPin*)(this->m_pTIPFilter->GetPin(0));
    /*if(pIn->IsConnected())
    {*/
        if(iPosition==0)
        {
            int sample_rate = ((CSpeexDec*)m_pTIPFilter)->sampleRate;
            int frame_count = ((CSpeexDec*)m_pTIPFilter)->delayTime;

            pMediaType->InitMediaType();
            pMediaType->SetType(&MEDIATYPE_Audio);
            pMediaType->SetSubtype(&MEDIASUBTYPE_PCM);
            pMediaType->SetTemporalCompression(FALSE);
            pMediaType->AllocFormatBuffer(sizeof(WAVEFORMATEX));
            pMediaType->SetFormatType(&FORMAT_WaveFormatEx);
            WAVEFORMATEX* pwf = (WAVEFORMATEX*)(pMediaType->Format());

            pwf->cbSize = 0;
            pwf->wFormatTag = pMediaType->subtype.Data1;
            pwf->wBitsPerSample = 16;
            pwf->nChannels = 1;
            pwf->nSamplesPerSec = sample_rate;
            pwf->nBlockAlign = pwf->wBitsPerSample / 8 * pwf->nChannels;
            pwf->nAvgBytesPerSec = pwf->nBlockAlign * sample_rate;

            pMediaType->SetSampleSize(pwf->nBlockAlign);

            return S_OK;
        }
        return VFW_S_NO_MORE_ITEMS;
    /*}

    return VFW_S_NO_MORE_ITEMS;*/
}

HRESULT SpeexDecOutputPin::CheckMediaType( const CMediaType* pmt )
{
    if (pmt->majortype == MEDIATYPE_Audio && pmt->subtype == MEDIASUBTYPE_PCM) {
        return S_OK;
    }
    return E_FAIL;
}

STDMETHODIMP SpeexDecOutputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
    (*ppEnum) = new CEnumMediaTypes(this, NULL);
    return S_OK;
}

HRESULT SpeexDecOutputPin::CompleteConnect( IPin *pReceivePin )
{
    //编解码器本不该使用TransInPlaceFilter，这种Filter是用来做同格式数据处理的
    //编解码后，输入输出格式不一样，会导致调用CTransInPlaceFilter::CompleteConnect时
    //断开已连接的输入Pin连接，为了避免这一问题，直接调CBaseOutputPin::CompleteConnect
    //Liaokz， 2014-5
    return CBaseOutputPin::CompleteConnect(pReceivePin);
}



//----------------------------------------------------------------
// Do not modify the following block of codes.
// You can ignore the following codes, no useful code anymore. 

// Class factory template - needed for the CreateInstance mechanism
CFactoryTemplate g_Templates[]=
    {   { L"SpeexDec"
        , &CLSID_SpeexDec
        , CSpeexDec::CreateInstance
        , NULL
        , &sudNullNull }
    };

int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// CreateInstance
//
// Provide the way for COM to create a CNullNull object
CUnknown * WINAPI CSpeexDec::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CheckPointer(phr,NULL);
    
    CSpeexDec *pNewObject = 
        new CSpeexDec(NAME("SpeexDec"), punk, phr );

    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance

#include "CommonDllFunctions.h"