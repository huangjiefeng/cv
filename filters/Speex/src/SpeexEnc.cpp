#include <streams.h>     // DirectShow (includes windows.h)
#include <initguid.h>    // declares DEFINE_GUID to declare an EXTERN_C const.
#include <stdio.h>
#include <atlbase.h>
//#include <iostream.h>

#if (1100 > _MSC_VER)
#include <olectlid.h>
#else
#include <olectl.h>
#endif

#include "speex/speex.h"
#include "speex/speex_bits.h"
#include "speex/speex_types.h"

#include "SpeexEncInputPin.h"
#include "SpeexEncOutputPin.h"
#include "GUIDs.h"
#include "SpeexEncIProp.h"
#include "SpeexEncProp.h"
#include "CommonSetupData.h"
#include "Log.h"

const AMOVIESETUP_FILTER sudNullNull =
{ 
  &CLSID_SpeexEnc                 // clsID
, L"SpeexEnc"                 // strName
, MERIT_DO_NOT_USE                // dwMerit
, 2                               // nPins
, psudPins                        // lpPin
};



class CSpeexEnc : public CTransInPlaceFilter, ISpecifyPropertyPages, SpeexEncIProp
{

public:
    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
    DECLARE_IUNKNOWN;

private:

	void *enc_state;
	
	UINT32 sampleRate;
	UINT32 quality;
	UINT32 delayTime;

	UINT32 encodedFrames;

	// 采集设备所打的时间戳，有时会出现错误，以下成员根据 时间戳-流时间 的差修正时间戳
	//REFERENCE_TIME m_rtSampleStreamDelta;		// 最后一次记录的误差

	REFERENCE_TIME m_rtLastSampleEnd; // 上一个样本的结束时间


    // Constructor - just calls the base class constructor
    CSpeexEnc(TCHAR *tszName, LPUNKNOWN punk, HRESULT* phr)
        : CTransInPlaceFilter (tszName, punk, CLSID_SpeexEnc, phr)
		, enc_state(NULL)
		, sampleRate(32),quality(10),delayTime(4)
		, encodedFrames(0)
		/*, m_rtSampleStreamDelta(0)*/
		, m_rtLastSampleEnd(0)
    {
		put_SampleRate(sampleRate);
	}

	~CSpeexEnc()
	{
		if (enc_state)
		{
			speex_encoder_destroy(enc_state);
		}
	}

	STDMETHODIMP Run(REFERENCE_TIME tStart)
	{
		encodedFrames = 0;
		return CTransInPlaceFilter::Run(tStart);
	}

    HRESULT Transform(IMediaSample *pIn)
	{
		++encodedFrames;

		BYTE* buf;
		pIn->GetPointer(&buf);

    //参照Linphone的调用方式实现标准化的Speex编码，Liaokz，2014-5

    //获得编码后每帧的大小
		int encframe_size;
		speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,(void*)&encframe_size); 
    //每一个数据包的帧数，以20ms为一帧，如80ms就是4帧
    int frame_per_packet = delayTime;
    //编码后每数据包大小
    int enc_packet_size = frame_per_packet * encframe_size * 2;//每sample 16bit
    //Input
    int input_total_size = pIn->GetActualDataLength();
		int input_size_per_sample=input_total_size/delayTime;

    SpeexBits bits;
    speex_bits_init(&bits);

		for(int i=0; i<frame_per_packet; ++i)
		{
			speex_encode_int(enc_state, (spx_int16_t*)(buf+i*input_size_per_sample), &bits);
		}
    //printf("%d:%d", bits.charPtr, bits.bitPtr);
    speex_bits_insert_terminator(&bits);
    //printf("-> %d:%d %d\n", bits.charPtr, bits.bitPtr, bits.chars[bits.charPtr-1]);
    int actual_enc_size = speex_bits_write(&bits, (char*)(buf), enc_packet_size);
    speex_bits_destroy(&bits);

		pIn->SetActualDataLength(actual_enc_size);
        
		return NOERROR;
	}

public:

    HRESULT CheckInputType(const CMediaType *pMediaType)
	{
		CheckPointer(pMediaType,E_POINTER);
		
		// check major type and sub type first!
		WAVEFORMATEX format;
		memcpy(&format, pMediaType->Format(), pMediaType->FormatLength());
		if(format.nChannels!=1 || (format.nSamplesPerSec!=sampleRate*1000))
		{
			return E_UNEXPECTED;
	}	
		return S_OK;
	}

	CBasePin * GetPin(int n)
	{
		HRESULT hr = S_OK;

		// Create an input pin if not already done
		
		if (m_pInput == NULL) {
			
			m_pInput = new SpeexEncInputPin( NAME("SpeexEnc input pin")
				, this        // Owner filter
				, &hr         // Result code
				, L"Input"    // Pin name
				);

			((SpeexEncInputPin*)m_pInput)->delayTime = delayTime;
			// Constructor for CTransInPlaceInputPin can't fail
			ASSERT(SUCCEEDED(hr));
		}
		
		// Create an output pin if not already done
		
		if (m_pInput!=NULL && m_pOutput == NULL) {
			
			m_pOutput = new SpeexEncOutputPin( NAME("SpeexEnc output pin")
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

	STDMETHODIMP GetPages(CAUUID *pPages) 
	{
		CheckPointer(pPages,E_POINTER);
		
		pPages->cElems = 1;
		pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
		
		if (pPages->pElems == NULL)
			return E_OUTOFMEMORY;
		
		pPages->pElems[0] = CLSID_SpeexEncProp;
		
		return NOERROR;
		
	}
	
	
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv)
	{
		CheckPointer(ppv,E_POINTER);
		
		if (riid == IID_SpeexEncIProp) {
			return GetInterface((SpeexEncIProp *) this, ppv);
		}
		else if (riid == IID_ISpecifyPropertyPages) {
			return GetInterface((ISpecifyPropertyPages *) this, ppv);
		}
		else {
			return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
		}
		
	}

	STDMETHODIMP put_SampleRate(unsigned int rate)
	{
		if(rate==8 || rate==16 || rate==32)
		{
			//by dsh 10-07-29
			/*if(sampleRate!=rate)
			{*/
				sampleRate = rate;

				if (enc_state)
				{
					speex_encoder_destroy(enc_state);
				}

				printf_s("[SpeexEnc] Init encode state(sampleRate=%d).\n", sampleRate);

				switch(sampleRate)
				{
				case 8:
					enc_state = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
					break;
				case 16:
					enc_state = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_WB));
					break;
				case 32:
					enc_state = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_UWB));
					break;
				}
				
				spx_int32_t tmp;
				tmp=0; 
				speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &tmp); 
				tmp=10; 
				speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &tmp); 
				tmp=1; 
				speex_encoder_ctl(enc_state, SPEEX_SET_COMPLEXITY, &tmp); 
				tmp=0; 
				speex_encoder_ctl(enc_state, SPEEX_SET_HIGHPASS, &tmp);

				
				int sampleRatevar=sampleRate*1000;
				speex_encoder_ctl(enc_state, SPEEX_SET_SAMPLING_RATE, &sampleRatevar);
			//}
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
			speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &quality);
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

	//add by dsh 10-08-05
	STDMETHODIMP put_State(UINT32 state)
	{
		((SpeexEncInputPin*)m_pInput)->bActive=state;
		return S_OK;
	}
	
	STDMETHODIMP get_State(UINT32* pState)
	{
		CheckPointer(pState, E_POINTER);
		*pState = ((SpeexEncInputPin*)m_pInput)->bActive;
		return S_OK;
	}
	//end by dsh 
};


//----------------------------------------------------------------
// Do not modify the following block of codes.
// You can ignore the following codes, no useful code anymore. 

// Class factory template - needed for the CreateInstance mechanism
CFactoryTemplate g_Templates[]=
    {   

		{ L"SpeexEnc"
        , &CLSID_SpeexEnc
        , CSpeexEnc::CreateInstance
        , NULL
        , &sudNullNull }
		,
		{ L"SpeexEnc Property Page"
		, &CLSID_SpeexEncProp
        , SpeexEncProp::CreateInstance}
    };

int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//
// CreateInstance
//
// Provide the way for COM to create a CNullNull object
CUnknown * WINAPI CSpeexEnc::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CheckPointer(phr,NULL);
    
    CSpeexEnc *pNewObject = 
        new CSpeexEnc(NAME("SpeexEnc"), punk, phr );

    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance




#include "CommonDllFunctions.h"
