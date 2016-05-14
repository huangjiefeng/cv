//-----------------------------------------------------------------------------
//
//	Monogram AAC Encoder
//
//	Base on GNU LGPL libfaac
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------
#include "stdafx.h"

#define AAC_PRINTF(fmt, ...) printf("[AACEnc] "fmt, __VA_ARGS__)
//启用一下宏以启用该Filter原来的推送方式，及每产生一个编码数据就推送
//注意，这将导致RTP接收端短时间收到过多音频帧从而造成错误的丢帧 --Liaokz，2014-3
#define DELIVER_ASAP

//-----------------------------------------------------------------------------
//
//	CAACEncoder
//
//-----------------------------------------------------------------------------
CAACEncoder::CAACEncoder(LPUNKNOWN pUnk, HRESULT *phr) :
	CTransformFilter(_T("MONOGRAM AAC Encoder"), pUnk, CLSID_MonogramAACEncoder),
	property_page(NULL),
    outsize(32*1024)
{
	// sicko je oukey
	if (phr) *phr = NOERROR;

	// vyrobime piny
	m_pInput = new CTransformInputPin(NAME("Input"), this, phr, L"In");
	m_pOutput = new CTransformOutputPin(NAME("Output"), this, phr, L"Out");

	// defaultne veci
	config.version		= AAC_VERSION_MPEG4;
	config.object_type	= AAC_OBJECT_LOW;
	config.output_type	= AAC_OUTPUT_RAW;
	config.bitrate		= 128000;

	// info
	info.samplerate		= 0;
	info.channels		= 0;
	info.frame_size		= 0;
	info.frames_done	= 0;

	// enkoder
	encoder				= NULL;
	extradata			= NULL;
	extradata_size		= 0;

	// buffer na data
	buffer				= (short*)malloc(10 * 2 * 48000 * sizeof(short));
	samples				= 0;

    // outbuf
    outbuf = (BYTE*)malloc(outsize);
}

CAACEncoder::~CAACEncoder()
{
	// releasujeme pagesu
	if (property_page) {
		property_page->Release();
		property_page = NULL;
	}

	free(buffer);
    free(outbuf);
}

CUnknown *WINAPI CAACEncoder::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
	CAACEncoder *enc = new CAACEncoder(pUnk, phr);
	if (!enc) {
		if (phr) *phr = E_OUTOFMEMORY;
	}
	return enc;
}

STDMETHODIMP CAACEncoder::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_ISpecifyPropertyPages) {
		return GetInterface((ISpecifyPropertyPages*)this, ppv);
	} else
	if (riid == IID_IMonogramAACEncoder) {
		return GetInterface((IMonogramAACEncoder*)this, ppv);
	} else
		return __super::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CAACEncoder::GetPages(CAUUID *pPages)
{
    CheckPointer(pPages,E_POINTER);

    pPages->cElems = 1;
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
    if (pPages->pElems == NULL) {
        return E_OUTOFMEMORY;
    }

	*(pPages->pElems) = CLSID_MonogramAACEncoderPropertypage;
    return NOERROR;

} // GetPages


HRESULT CAACEncoder::CheckInputType(const CMediaType *mtIn)
{
	/*
		Podporujeme len niektore typy
	*/
	if (mtIn->majortype != MEDIATYPE_Audio) return E_FAIL;
	if (mtIn->subtype != MEDIASUBTYPE_PCM) return E_FAIL;
	if (mtIn->formattype != FORMAT_WaveFormatEx) return E_FAIL;

	return NOERROR;
}

HRESULT CAACEncoder::CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
{
	HRESULT hr = CheckInputType(mtIn);
	if (FAILED(hr)) return hr;

	// aj vystup by mal byt slusny
	if (mtOut->majortype != MEDIATYPE_Audio) return E_FAIL;
	if (mtOut->subtype != MEDIASUBTYPE_AAC) return E_FAIL;

	return NOERROR;
}

HRESULT CAACEncoder::GetMediaType(int iPosition, CMediaType *pmt)
{
	if (iPosition < 0) return E_INVALIDARG;
	if (iPosition > 0) return VFW_S_NO_MORE_ITEMS;

	// mal by byt zapojeny input pin a mal by byt aspon volajaky enkoder otvoreny
	if (m_pInput->IsConnected() == FALSE) return E_FAIL;
	if (!encoder) return E_FAIL;

	pmt->majortype	= MEDIATYPE_Audio;
	pmt->subtype	= MEDIASUBTYPE_AAC;
	pmt->formattype	= FORMAT_WaveFormatEx;

	WAVEFORMATEX *wfx	= (WAVEFORMATEX*)pmt->AllocFormatBuffer(sizeof(*wfx) + extradata_size);
	memset(wfx, 0, sizeof(*wfx));
	wfx->cbSize			= extradata_size;
	wfx->nChannels		= info.channels;
	wfx->nSamplesPerSec	= info.samplerate;
	wfx->wFormatTag		= 0xff;					// AAC

	// decoder specific info
	BYTE		*ex		= ((BYTE*)wfx) + sizeof(*wfx);
	memcpy(ex, extradata, extradata_size);

	return NOERROR;
}

HRESULT CAACEncoder::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp)
{
	/*
		Volime si buffer na prenos
	*/

	pProp->cbBuffer		= 64 * 1024;			// pevne dane a hotovo
	pProp->cBuffers		= 2;

	ALLOCATOR_PROPERTIES	act;
	HRESULT hr = pAlloc->SetProperties(pProp, &act);
	if (FAILED(hr)) return hr;

	// len tak pre istotu
	if (act.cbBuffer < pProp->cbBuffer) return E_FAIL;
	return NOERROR;
}


HRESULT CAACEncoder::SetMediaType(PIN_DIRECTION direction, const CMediaType *pmt)
{
	if (direction == PINDIR_INPUT) {
		/*
			Musime odchytit vstupny format, aby sme vedeli dobre
			inicializovat enkoder
		*/
		WAVEFORMATEX	*wfx = (WAVEFORMATEX*)pmt->pbFormat;
		if (!wfx) return E_FAIL;

		// teraz nas zaujima vacero veci:
		if (wfx->wBitsPerSample != 16) return E_FAIL;
		info.channels	= wfx->nChannels;
		info.samplerate	= wfx->nSamplesPerSec;

		if (encoder) {
			CloseEncoder();
		}

		// otvorime novy enkoder
		int ret = OpenEncoder();
		if (ret < 0) {
			CloseEncoder();
			return E_FAIL;
		}

	} else
	if (direction == PINDIR_OUTPUT) {
	}
	return NOERROR;
}

HRESULT CAACEncoder::BreakConnect(PIN_DIRECTION dir)
{
	if (dir == PINDIR_INPUT) {
		// tu zrusime instanciu enkodera a odpojime output pin
		CloseEncoder();

		// odpojime
		if (m_pOutput->IsConnected()) {
			m_pOutput->GetConnected()->Disconnect();
			m_pOutput->Disconnect();
		}

		info.channels = 0;
		info.samplerate = 0;

	} else
	if (dir == PINDIR_OUTPUT) {
		// tu nerobime nic specialne - len zrusime pocitadla
		CAutoLock	lck(&lock_info);
		info.frames_done = 0;
	}
	return __super::BreakConnect(dir);
}


HRESULT CAACEncoder::Receive(IMediaSample *pSample)
{
	// odchytime si casy
	REFERENCE_TIME		rtStart, rtStop;
	HRESULT hr = pSample->GetTime(&rtStart, &rtStop);
	if (!got_time) {
		if (hr == NOERROR) {
			start = rtStart;
			got_time = true;
		} else {
			return NOERROR;
		}
	}
	//AAC_PRINTF("Receive ts=%I64d ", rtStart);

	if (!m_pOutput->IsConnected()) {
		return NOERROR;
	}

	ASSERT(encoder);
	HRESULT	hrRet = NOERROR;

	/**************************************************************************
	**
	**	Vydolujeme data
	**
	***************************************************************************/

	BYTE	*buf;
	long	size;
	pSample->GetPointer(&buf);
	size = pSample->GetActualDataLength();

	short	*in_buf = (short*)buf;
	long	in_samples = size / sizeof(short);

	memcpy(buffer + samples, in_buf, in_samples * sizeof(short));
	samples += in_samples;
	sample_received += in_samples;

	/**************************************************************************
	**
	**	Enkodovanie packetov
	**
	***************************************************************************/

    short	*cur_buf	= buffer;
	int		cur_sample	= 0;

#ifdef DELIVER_ASAP

    /*int		outsize		= 32*1024;
	BYTE	*outbuf		= (BYTE*)malloc(outsize);*/

	while (cur_sample + info.frame_size <= samples) {
		// dame zakodovat frame
		int	ret = faacEncEncode(encoder, (int*)cur_buf, info.frame_size, outbuf, outsize);
		if (ret > 0) {

			REFERENCE_TIME		rtStart, rtStop;
			IMediaSample		*sample = NULL;
            HRESULT				hr;

			// dorucime data
			hr = GetDeliveryBuffer(&sample);
			if (FAILED(hr)) {
				hrRet = hr;
				break;
			}

			// spocitame timestampy
			int		fs = info.frame_size / info.channels;

			rtStart = (info.frames_done*fs*10000000)/info.samplerate;
			rtStop  = ((((info.frames_done+1)*fs)-1)*10000000)/info.samplerate;
			rtStart += start;
			rtStop  += start;

			sample->SetTime(&rtStart, &rtStop);

			// napiseme data
			BYTE	*out;
			sample->GetPointer(&out);
			memcpy(out, outbuf, ret);
			sample->SetActualDataLength(ret);

			hr = m_pOutput->Deliver(sample);
			sample->Release();

			if (FAILED(hr)) {
				hrRet = hr;
				break;
			}

			// pripocitame spracovany frame
			info.frames_done ++;
        } else {
            AAC_PRINTF(".\n");
        }
        
        // na dalsi frame
        cur_sample	+= info.frame_size;
        cur_buf		+= info.frame_size;
    }

    //free(outbuf);

#else // DELIVER_ASAP

    //一次发送尽可能多的数据，可减少RTP发送的Overhead，同时降低RTP接收端的丢包概率
    // --Liaokz, 2014-3
    while (cur_sample + info.frame_size <= samples) {

        IMediaSample *sample = NULL;
        HRESULT hr;
        hr = GetDeliveryBuffer(&sample);
        if (FAILED(hr)) {
            hrRet = hr;
        } else {

            const long outsize = sample->GetSize();
            BYTE *outbuf = (BYTE*)malloc(outsize);
            unsigned long cur_size = 0; //记录已经编码的数据大小
            int frames_encoded = 0; //本次编码的帧数

            while (cur_size + max_output_size <= (unsigned long)outsize &&  //缓冲区大小足够
                cur_sample + info.frame_size <= samples) { //或编码数据足够，同外层while条件

                int	ret = faacEncEncode(encoder, (int*)cur_buf, info.frame_size, 
                    outbuf + cur_size, 
                    outsize - cur_size);
                if (ret > 0) {
                    cur_size += ret;
                    frames_encoded ++;
                } else {
                    AAC_PRINTF(".\n");
                }

                cur_sample	+= info.frame_size;
                cur_buf		+= info.frame_size;
            }

            if (cur_size > 0) {
                //计算时间戳
                REFERENCE_TIME rtStart, rtStop;
                int fs = info.frame_size / info.channels;
                rtStart = (info.frames_done*fs*10000000)/info.samplerate;
                rtStop  = (((info.frames_done+frames_encoded)*fs)*10000000)/info.samplerate-1;
                rtStart += start;
                rtStop  += start;

                sample->SetTime(&rtStart, &rtStop);

                //推送Sample
                BYTE	*out;
                sample->GetPointer(&out);
                memcpy(out, outbuf, cur_size);
                sample->SetActualDataLength(cur_size);

                hr = m_pOutput->Deliver(sample);
                if (FAILED(hr)) {
                    AAC_PRINTF("Delivered sample failed. ts=%I64d\n", rtStart);
                } else {
                    AAC_PRINTF("Delivered %d samples, ts=%I64d-%I64d\n", frames_encoded, rtStart, rtStop);
                }

                //重置数据
                info.frames_done += frames_encoded;
                frames_encoded = 0;
                cur_size = 0;
            }

            sample->Release();
            free(outbuf);
        }
    }

#endif // DELIVER_ASAP

	// discardujeme stare data
	if (cur_sample > 0) {
		int	left	= samples - cur_sample;
		if (left > 0) {
			memcpy(buffer, buffer+cur_sample, left*sizeof(short));
		}
		samples = left;
	}

	return hrRet;
}

HRESULT CAACEncoder::GetDeliveryBuffer(IMediaSample **sample)
{
    IMediaSample *pOutSample;
    HRESULT hr = m_pOutput->GetDeliveryBuffer(&pOutSample, NULL, NULL, 0);
    *sample = pOutSample;
    if (FAILED(hr)) {
        return hr;
    }

	// ak sa zmenil type, tak aktualizujeme nase info
	AM_MEDIA_TYPE *mt;
	if (pOutSample->GetMediaType(&mt) == NOERROR) {
		CMediaType _mt(*mt);
		SetMediaType(PINDIR_OUTPUT, &_mt);
		DeleteMediaType(mt);
	}

	return NOERROR;
}

STDMETHODIMP CAACEncoder::SetPropertyPage(IUnknown *pUnk)
{
	CAutoLock	lck(&lock_info);

	if (property_page) {
		property_page->Release();
		property_page = NULL;
	}

	if (pUnk) {
		return pUnk->QueryInterface(IID_IMonogramAACEncoderPropertyPage, (void**)&property_page);
	}

	return NOERROR;
}

STDMETHODIMP CAACEncoder::GetConfig(AACConfig *config)
{
	CAutoLock	lck(&lock_info);
	config->version		= this->config.version;
	config->object_type	= this->config.object_type;
	config->output_type	= this->config.output_type;
	config->bitrate		= this->config.bitrate;
	return NOERROR;
}

STDMETHODIMP CAACEncoder::SetConfig(AACConfig *config)
{
	if (m_State != State_Stopped) {
		MessageBox(NULL, _T("Must be stopped to apply settings"), _T("Info"), 0);
		return E_FAIL;
	}
	if (m_pOutput->IsConnected()) {
		MessageBox(NULL, _T("Must not be connected to apply settings"), _T("Info"), 0);
		return E_FAIL;
	}

	CAutoLock	lck(&lock_info);
	this->config.version		= config->version;
	this->config.object_type	= config->object_type;
	this->config.output_type	= config->output_type;
	this->config.bitrate		= config->bitrate;

	if (m_pInput->IsConnected()) {
		CloseEncoder();
		int ret = OpenEncoder();
		if (ret < 0) {
			CloseEncoder();
		}
	}

	return NOERROR;
}

STDMETHODIMP CAACEncoder::GetInfo(AACInfo *info)
{
	CAutoLock	lck(&lock_info);
	info->samplerate	= this->info.samplerate;
	info->channels		= this->info.channels;
	info->frame_size	= this->info.frame_size;
	info->frames_done	= this->info.frames_done;	
	return NOERROR;
}

void CAACEncoder::UpdatePage(int info_only) 
{ 
	CAutoLock	lck(&lock_info);
	if (property_page) property_page->Update(info_only);
}


void CAACEncoder::CloseEncoder()
{
	if (!encoder) return ;
	faacEncClose(encoder);
	encoder = NULL;
	info.frame_size = 0;
	info.frames_done = 0;

	// decoder specific veci
	if (extradata) {
		free(extradata);
		extradata = NULL;
	}
	extradata_size = 0;
}

int CAACEncoder::OpenEncoder()
{
	// neotvarame 2x a checkneme na sprostosti
	if (encoder) return -1;
	if (info.samplerate == 0 || info.channels == 0) return -1;

	unsigned long	framesize;
	unsigned long	ob;

	// spravime enkoder
	encoder = faacEncOpen(info.samplerate, info.channels, &framesize, &ob);
	if (!encoder) return -1;

	// nastavime veci
	info.frame_size  = framesize;
	info.frames_done = 0;
    max_output_size = ob;

	// parametre kodovania
	faacEncConfigurationPtr	conf;
	conf = faacEncGetCurrentConfiguration(encoder);
	if (!conf) {
		faacEncClose(encoder);
		encoder = NULL;
		info.frame_size = 0;
		return -1;
	}

	conf->mpegVersion	= config.version;
	conf->aacObjectType	= config.object_type;
	conf->allowMidside	= 0;
	conf->bandWidth		= 0;
	conf->bitRate		= config.bitrate / info.channels;
	conf->inputFormat	= FAAC_INPUT_16BIT;
	conf->useLfe		= 0;
	conf->useTns		= 0;
	conf->shortctl		= SHORTCTL_NORMAL;
	conf->outputFormat	= (config.output_type > 0 ? 1 : 0);
	faacEncSetConfiguration(encoder, conf);

	// nastavime extradata
	BYTE *ex_ptr = NULL;
	unsigned long ex_size;
	faacEncGetDecoderSpecificInfo(encoder, &ex_ptr, &ex_size);
	if (ex_ptr && ex_size > 0) {
		extradata_size = ex_size;
		extradata = (BYTE*)malloc(ex_size);
		memcpy(extradata, ex_ptr, ex_size);
	}

	//Debug info add by Liaokz, 2014-3
	AAC_PRINTF("Encoder opened. frameSize=%d, maxOutputBytes=%d\n", 
		framesize, ob);

	return 0;
}

HRESULT CAACEncoder::StartStreaming()
{
	// zresetujeme veci
	got_time = false;
	start = 0;
	samples = 0;
	info.frames_done = 0;

	return NOERROR;
}

HRESULT CAACEncoder::EndFlush()
{
	got_time = false;
	start = 0;
	samples = 0;
	info.frames_done = 0;
	return __super::EndFlush();
}


