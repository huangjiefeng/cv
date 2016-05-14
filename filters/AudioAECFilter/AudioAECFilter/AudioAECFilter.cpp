// AudioAECFilter.cpp : 定义 DLL 应用程序的导出函数。
//

/*
Edit by dsh
音频数据采集及回声消除Filter
*/

#include "stdafx.h"
#include <strmif.h> 
#include <reftime.h>

#include <dmo.h>
#include <Mmsystem.h>
#include <objbase.h>
#include <mediaobj.h>
#include <uuids.h>
#include <propidl.h>
#include <wmcodecdsp.h>


#include <atlbase.h>
#include <ATLComCli.h>
#include <audioclient.h>
#include <MMDeviceApi.h>
#include <AudioEngineEndPoint.h>
#include <DeviceTopology.h>
#include <propkey.h>
#include <strsafe.h>
#include <conio.h>


#include <olectl.h>
#include <initguid.h>
#include "mediabuf.h"
#include "AecKsBinder.h"
#include "AudioAECFilter.h"




#pragma warning(disable:4710)  // 'function': function not inlined (optimzation)


// {31C942CE-E32C-4aef-95B4-86F9E697AFD8}
DEFINE_GUID(CLSID_AudioAEC, 
0x31c942ce, 0xe32c, 0x4aef, 0x95, 0xb4, 0x86, 0xf9, 0xe6, 0x97, 0xaf, 0xd8);



// Setup data

const AMOVIESETUP_MEDIATYPE sudOpPinTypes =
{
	&MEDIATYPE_Audio,       // Major type
    &MEDIASUBTYPE_PCM      // Minor type
};

const AMOVIESETUP_PIN sudOpPin =
{
    L"Output",              // Pin string name
    FALSE,                  // Is it rendered
    TRUE,                   // Is it an output
    FALSE,                  // Can we have none
    FALSE,                  // Can we have many
    &CLSID_NULL,            // Connects to filter
    NULL,                   // Connects to pin
    1,                      // Number of types
    &sudOpPinTypes };       // Pin details

const AMOVIESETUP_FILTER sudBallax =
{
    &CLSID_AudioAEC,    // Filter CLSID
    L"AudioAEC",       // String name  //dsh
    MERIT_DO_NOT_USE,       // Filter merit
    1,                      // Number pins
    &sudOpPin               // Pin details
};


// COM global table of objects in this dll

CFactoryTemplate g_Templates[] = {
  { L"AudioAEC"//dsh
  , &CLSID_AudioAEC
  , CAudioAECFilter::CreateInstance
  , NULL
  , &sudBallax }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//
// DllRegisterServer
//
// Exported entry points for registration and unregistration
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2(TRUE);

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2(FALSE);

} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  dwReason, 
                      LPVOID lpReserved)
{
    setlocale(LC_CTYPE, ""); //用于宽字符中文wprintf
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}



#define SAFE_ARRAYDELETE(p) {if (p) delete[] (p); (p) = NULL;}
#define SAFE_RELEASE(p) {if (NULL != p) {(p)->Release(); (p) = NULL;}}

#define VBFALSE (VARIANT_BOOL)0
#define VBTRUE  (VARIANT_BOOL)-1

#define CHECK_RET(hr, message, ...) if (FAILED(hr)) { printf_s("[AEC] error(hr=0x%08x): "message"\n", hr, __VA_ARGS__); }
#define CHECKHR(x) hr = x; if (FAILED(hr)) { printf_s("[AEC] error hr=0x%08x at line %d\n", hr, __LINE__); }
#define CHECK_ALLOC(pb, message, ...) if (NULL == pb) { printf_s("[AEC] error alloc: "message"\n", hr, __VA_ARGS__); }

#define LOG(fmt, ...)  printf_s ("[AEC] "fmt"\n", __VA_ARGS__);
#define LOGW(fmt, ...) wprintf_s(L"[AEC] "fmt L"\n", __VA_ARGS__);


// AEC支持的采样率
// 参考：http://msdn.microsoft.com/en-us/library/windows/desktop/ff819492(v=vs.85).aspx
const int g_sampleRate_table[] = {22050, 16000, 11025, 8000};

BOOL IsSampleRateSupport(const int nSampleRate)
{
	for (int i = 0; i < sizeof(g_sampleRate_table)/sizeof(int); ++i)
	{
		if (nSampleRate == g_sampleRate_table[i]) return TRUE;
	}
	return FALSE;
}


CUnknown * WINAPI CAudioAECFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);

    CUnknown *punk = new CAudioAECFilter(lpunk, phr);
    if(punk == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;
    }
    return punk;

} // CreateInstance

CAudioAECFilter::CAudioAECFilter(LPUNKNOWN lpunk, HRESULT *phr) :
    CSource(NAME("AudioAEC"), lpunk, CLSID_AudioAEC)//dsh
{
    ASSERT(phr);
    CAutoLock cAutoLock(&m_cStateLock);

    m_paStreams = (CSourceStream **) new CAudioAECFilterPin*[1];
    if(m_paStreams == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }

    m_paStreams[0] = new CAudioAECFilterPin(phr, this, L"AudioAECPin");//dsh
    if(m_paStreams[0] == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }

}

CAudioAECFilter::~CAudioAECFilter()
{
	if (m_paStreams)
	{
		if (m_paStreams[0])
		{
			delete m_paStreams[0];
		}
		delete [] m_paStreams;
		m_paStreams = NULL;
	}
}

CAudioAECFilterPin::CAudioAECFilterPin(HRESULT *phr,
                         CAudioAECFilter *pParent,
						 LPCWSTR pPinName):
    CSourceStream(NAME("AudioAEC"),phr, pParent, pPinName)
{
    ASSERT(phr);
    CAutoLock cAutoLock(&m_cSharedState);


	bSetFormat=false;
	m_mediaType= (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
	m_mediaType->pbFormat= (PBYTE)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
	memset(m_mediaType->pbFormat, 0, sizeof(WAVEFORMATEX));

	m_mediaType->cbFormat=sizeof(WAVEFORMATEX);
	m_mediaType->pUnk=NULL;
	m_mediaType->majortype = MEDIATYPE_Audio;
	m_mediaType->subtype = MEDIASUBTYPE_PCM;
	m_mediaType->lSampleSize = 0;
	m_mediaType->bFixedSizeSamples = TRUE;
	m_mediaType->bTemporalCompression = FALSE;
	m_mediaType->formattype = FORMAT_WaveFormatEx;


	m_PlayDev = NULL;
	pDMO = NULL;
	m_PlayBuffer = NULL;

	m_iIntervalTime=80;//80ms
	m_previewHwnd=NULL;

	HRESULT hr = S_OK;
    CoInitialize(NULL);

	m_mode=SINGLE_CHANNEL_AEC;//模式选择，是否回消等
	m_feat=0;//是否修改特征，当它为on时，下面几个的设置才会有效
	m_ns=1;//是否去噪
	m_agc=0;//是否增益
	m_cntrclip=0;;//是否剪切中部

	int  iMicDevIdx = 0;               // microphone device index
    int  iSpkDevIdx = 0;               // speaker device index
   
    //WAVEFORMATEX wfxOut = {WAVE_FORMAT_PCM, 1, 8000, 16000, 2, 16, 0};
    AUDIO_DEVICE_INFO *pCaptureDeviceInfo = NULL, *pRenderDeviceInfo = NULL;


    HANDLE currThread = NULL;
    HANDLE currProcess = NULL;
    BOOL iRet;
    currProcess = GetCurrentProcess ();
    currThread = GetCurrentThread ();

    iRet = SetPriorityClass (currProcess, HIGH_PRIORITY_CLASS);
    if ( 0 == iRet )
    {
        LOG("failed to set process priority");
    }

    // DMO initialization
    CHECKHR(CoCreateInstance(CLSID_CWMAudioAEC, NULL, CLSCTX_INPROC_SERVER, IID_IMediaObject, (void**)&pDMO));
    CHECKHR(pDMO->QueryInterface(IID_IPropertyStore, (void**)&pPS));
/*#ifdef _DEBUG 
	pfMicOutPCM=fopen("e://aec_mic.pcm","wb");
	pfDeliverPCM=fopen("e://aec_deliver.pcm","wb");
	pfMicSize=fopen("e://aec_mic_size.txt","wb");
	errorlog=fopen("e://aec_errorlog.txt","wb");
#endif*/

    // 以下Property设置参考http://msdn.microsoft.com/en-us/library/windows/desktop/ff819492(v=vs.85).aspx

    {
        PROPVARIANT pvSysMode;
        PropVariantInit(&pvSysMode);
        pvSysMode.vt = VT_I4;
        pvSysMode.lVal = (LONG)(m_mode);
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, pvSysMode));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, &pvSysMode));
        PropVariantClear(&pvSysMode);

        // 关闭音量自动增益，否则系统会自动调节麦克风音量
        PROPVARIANT pvMicGain;
        PropVariantInit(&pvMicGain);
        pvMicGain.vt = VT_BOOL;
        pvMicGain.boolVal = VARIANT_FALSE;
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_MIC_GAIN_BOUNDER, pvMicGain));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_MIC_GAIN_BOUNDER, &pvMicGain));
        PropVariantClear(&pvMicGain);
    }    

    if (iMicDevIdx >= 0 || iSpkDevIdx >= 0)
    {
        PROPVARIANT pvDeviceId;
        PropVariantInit(&pvDeviceId);
        pvDeviceId.vt = VT_I4;
        pvDeviceId.lVal = (unsigned long)(iSpkDevIdx<<16) + (unsigned long)(0x0000ffff & iMicDevIdx);
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_DEVICE_INDEXES, pvDeviceId));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_DEVICE_INDEXES, &pvDeviceId));
        PropVariantClear(&pvDeviceId);
    }

    if ( m_feat )
    {
        PROPVARIANT pvFeatrModeOn;
        PropVariantInit(&pvFeatrModeOn);
        pvFeatrModeOn.vt = VT_BOOL;
        pvFeatrModeOn.boolVal = m_feat? VBTRUE : VBFALSE;
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_FEATURE_MODE, pvFeatrModeOn));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_FEATURE_MODE, &pvFeatrModeOn));
        printf("%20s %5d \n", "Feature Mode is", pvFeatrModeOn.boolVal);
        PropVariantClear(&pvFeatrModeOn);

        // Turn on/off noise suppression
        PROPVARIANT pvNoiseSup;
        PropVariantInit(&pvNoiseSup);
        pvNoiseSup.vt = VT_I4;
        pvNoiseSup.lVal = (LONG)m_ns;
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_FEATR_NS, pvNoiseSup));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_FEATR_NS, &pvNoiseSup));
        printf("%20s %5d \n", "Noise suppresion is", pvNoiseSup.lVal);
        PropVariantClear(&pvNoiseSup);

        // Turn on/off AGC
        PROPVARIANT pvAGC;
        PropVariantInit(&pvAGC);
        pvAGC.vt = VT_BOOL;
        pvAGC.boolVal = m_agc ? VBTRUE : VBFALSE;
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_FEATR_AGC, pvAGC));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_FEATR_AGC, &pvAGC));
        printf("%20s %5d \n", "AGC is", pvAGC.boolVal);
        PropVariantClear(&pvAGC);

        // Turn on/off center clip
        PROPVARIANT pvCntrClip;
        PropVariantInit(&pvCntrClip);
        pvCntrClip.vt = VT_BOOL;
        pvCntrClip.boolVal = m_cntrclip? VBTRUE : VBFALSE;
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_FEATR_CENTER_CLIP, pvCntrClip));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_FEATR_CENTER_CLIP, &pvCntrClip));
        printf("%20s %5d \n", "Center clip is", (BOOL)pvCntrClip.boolVal);
        PropVariantClear(&pvCntrClip);
    }

    // 以下代码要在设置设备后执行，放到OnThreadCreate中去，不然会崩溃
    // by Liaokz, 2013-10

    // Set DMO output format
	/*DMO_MEDIA_TYPE mt = {0};
    hr = MoInitMediaType(&mt, sizeof(WAVEFORMATEX));
    CHECK_RET(hr, "MoInitMediaType failed");
    
    mt.majortype = MEDIATYPE_Audio;
    mt.subtype = MEDIASUBTYPE_PCM;
    mt.lSampleSize = 0;
    mt.bFixedSizeSamples = TRUE;
    mt.bTemporalCompression = FALSE;
    mt.formattype = FORMAT_WaveFormatEx;
    memcpy(mt.pbFormat, &wfxOut, sizeof(WAVEFORMATEX));*/
    
    /*hr = pDMO->SetOutputType(0, &mt, 0); 
    CHECK_RET(hr, "SetOutputType failed");*/
    //MoFreeMediaType(&mt);

    /*hr = pDMO->AllocateStreamingResources();
    CHECK_RET(hr, "AllocateStreamingResources failed");*/

    // end by Liaokz

    int iFrameSize;
    PROPVARIANT pvFrameSize;
    PropVariantInit(&pvFrameSize);
    CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_FEATR_FRAME_SIZE, &pvFrameSize));
    iFrameSize = pvFrameSize.lVal;
    PropVariantClear(&pvFrameSize);

	m_pOutputBuffer = new CStaticMediaBuffer;
	//memset(m_pOutputBuffer,0,sizeof(CStaticMediaBuffer)); DO NOT DO THIS! There's virtual func table in it!

	m_pOutputBufferStruct = new DMO_OUTPUT_DATA_BUFFER;
	memset(m_pOutputBufferStruct,0,sizeof(DMO_OUTPUT_DATA_BUFFER));
    m_pOutputBufferStruct->pBuffer = static_cast<IMediaBuffer *>(m_pOutputBuffer);

	m_pbOutputBuffer = new BYTE[MAX_RAWBUFFER_SIZE];
} // (Constructor)


//
// Destructor
//
CAudioAECFilterPin::~CAudioAECFilterPin()
{
    CAutoLock cAutoLock(&m_cSharedState);

	SAFE_RELEASE(m_PlayBuffer);
	SAFE_RELEASE(m_PlayDev);
	DeleteMediaType(m_mediaType);

	if (m_pOutputBufferStruct)
	{
		delete m_pOutputBufferStruct;
		m_pOutputBufferStruct = NULL;
	}
	if (m_pOutputBuffer)
	{
		delete m_pOutputBuffer;
		m_pOutputBuffer = NULL;
	}
 /*#ifdef _DEBUG
	fclose(pfMicOutPCM);
	fclose(pfDeliverPCM);
	
	fclose(pfMicSize);
	fclose(errorlog);
#endif*/
	SAFE_ARRAYDELETE(m_pbOutputBuffer);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pDMO);

    CoUninitialize();

} // (Destructor)


HRESULT CAudioAECFilterPin::FillBuffer(IMediaSample *pms)
{
    CheckPointer(pms,E_POINTER);
	if (NULL == pDMO)
	{
		return E_FAIL;
	}

    BYTE *pData = NULL;
    long lDataLen;
    DMO_MEDIA_TYPE mt = {0};

    ULONG cbProduced = 0;
    DWORD dwStatus;
	HRESULT hr;

	WAVEFORMATEX *pwav = (WAVEFORMATEX *) m_mt.Format();

    pms->GetPointer(&pData);
    lDataLen = pms->GetSize();//大小多少

    //ZeroMemory(pData, lDataLen);
    {
        CAutoLock cAutoLockShared(&m_cSharedState);		      
		while(m_RawBuffer.nRawBfferSize<lDataLen)
		{
			Sleep(m_iIntervalTime/2);
			do{
				m_pOutputBuffer->Init((byte*)m_pbOutputBuffer, MAX_RAWBUFFER_SIZE, 0);
				m_pOutputBufferStruct->dwStatus = 0;

				DWORD dwIn, dwOut, dwSize, dwAlign, dwBufLen;
				pDMO->GetStreamCount(&dwIn, &dwOut);
				pDMO->GetOutputSizeInfo(0, &dwSize, &dwAlign);

				m_pOutputBufferStruct->pBuffer->GetBufferAndLength(NULL, &dwBufLen);
				if (dwOut > 1 || dwBufLen + dwSize > MAX_RAWBUFFER_SIZE)
				{
					LOG("Warning: output stream count: %d, buffer size of stream[0]: %d, buffer used: %d", 
						dwOut, dwSize, dwBufLen);
				}

				if (m_pOutputBufferStruct->pBuffer == NULL)
				{
					LOG("Warning: NULL pointer to output buffer");
				}

				hr = pDMO->ProcessOutput(DMO_PROCESS_OUTPUT_DISCARD_WHEN_NO_BUFFER, 1, m_pOutputBufferStruct, &dwStatus);
				CHECK_RET (hr, "ProcessOutput failed");
				//fprintf(errorlog,"%lld\n",OutputBufferStruct.rtTimestamp);

				if (hr == S_FALSE) {
					cbProduced = 0;
				} else {
					hr = m_pOutputBuffer->GetBufferAndLength(NULL, &cbProduced);
					CHECK_RET (hr, "GetBufferAndLength failed");
				}
				//将从声卡拿到的新数据复制到暂存缓冲区
				if(m_RawBuffer.EndPos+cbProduced>MAX_RAWBUFFER_SIZE)
				{
					// case: 缓冲区循环使用
          int nLeft = MAX_RAWBUFFER_SIZE-m_RawBuffer.EndPos;
					memcpy(&m_RawBuffer.RawBuffer[m_RawBuffer.EndPos],m_pbOutputBuffer,nLeft);
					memcpy(m_RawBuffer.RawBuffer,m_pbOutputBuffer+nLeft,cbProduced-nLeft);
					m_RawBuffer.EndPos=cbProduced-nLeft;
				}
				else
				{
					memcpy(&m_RawBuffer.RawBuffer[m_RawBuffer.EndPos],m_pbOutputBuffer,cbProduced);
					m_RawBuffer.EndPos=m_RawBuffer.EndPos+cbProduced;
				}
				m_RawBuffer.nRawBfferSize+=cbProduced;
				if(m_RawBuffer.nRawBfferSize>MAX_RAWBUFFER_SIZE)
				{
					m_RawBuffer.nRawBfferSize=MAX_RAWBUFFER_SIZE;
					m_RawBuffer.HeadPos=m_RawBuffer.EndPos;
					LOG("Temporary buffer is full.");
				}

			} while (m_pOutputBufferStruct->dwStatus & DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE);
		}

		//将暂存缓冲区的新数据复制到Sample中
		if(m_RawBuffer.HeadPos+lDataLen>MAX_RAWBUFFER_SIZE)
		{
			int nLeft = MAX_RAWBUFFER_SIZE-m_RawBuffer.HeadPos;
			memcpy(pData, &m_RawBuffer.RawBuffer[m_RawBuffer.HeadPos], nLeft);
			memcpy(pData+nLeft, m_RawBuffer.RawBuffer, lDataLen-nLeft);
		}
		else
		{
			memcpy(pData,&m_RawBuffer.RawBuffer[m_RawBuffer.HeadPos],lDataLen);
		}
		//memcpy(pData,&m_RawBuffer.RawBuffer[m_RawBuffer.HeadPos],lDataLen);
		m_RawBuffer.nRawBfferSize-=lDataLen;
		m_RawBuffer.HeadPos=(m_RawBuffer.HeadPos + lDataLen) % MAX_RAWBUFFER_SIZE;
		pms->SetActualDataLength(lDataLen);
			

		//打时间戳
        // The current time is the sample's start
        CRefTime rtStart = m_rtSampleTime;

        // Increment to find the finish time
        m_rtSampleTime += (__int64)m_iIntervalTime*(__int64)10000;//化为100ns级别

        pms->SetTime((REFERENCE_TIME *) &rtStart,(REFERENCE_TIME *) &m_rtSampleTime);
    }

    //pms->SetSyncPoint(TRUE);
    return NOERROR;

} // FillBuffer


//
// Notify
//
// Alter the repeat rate according to quality management messages sent from
// the downstream filter (often the renderer).  Wind it up or down according
// to the flooding level - also skip forward if we are notified of Late-ness
//
STDMETHODIMP CAudioAECFilterPin::Notify(IBaseFilter * pSender, Quality q)
{

    return NOERROR;

}

HRESULT CAudioAECFilterPin::GetMediaType(int iPosition, CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());

    if(iPosition < 0)
    {
        return E_INVALIDARG;
	}

	WAVEFORMATEX *pwav = (WAVEFORMATEX *) pmt->AllocFormatBuffer(sizeof(WAVEFORMATEX));
	if(NULL == pwav)
		return(E_OUTOFMEMORY);

	ZeroMemory(pwav, sizeof(WAVEFORMATEX));
	if(bSetFormat)//had Set The Format,so only one Format is avaliable
	{
		if(iPosition>0)
			return VFW_S_NO_MORE_ITEMS;

		WAVEFORMATEX *psetFormat=(WAVEFORMATEX *)m_mediaType->pbFormat;
		pwav->wFormatTag=WAVE_FORMAT_PCM;

		pwav->nSamplesPerSec=psetFormat->nSamplesPerSec;
		pwav->nChannels=psetFormat->nChannels;
		pwav->wBitsPerSample=psetFormat->wBitsPerSample;
		pwav->nAvgBytesPerSec=psetFormat->nAvgBytesPerSec;
		pwav->nBlockAlign=psetFormat->nBlockAlign;
	}
	else
	{
		AM_MEDIA_TYPE * mytm = NULL;
		HRESULT hr = GetStreamCaps(iPosition, &mytm, NULL);
		if (FAILED(hr) || VFW_S_NO_MORE_ITEMS == hr)
		{
			return hr;
		}
		*pmt = *mytm;
		MoFreeMediaType(mytm);
	}

    pmt->SetType(&MEDIATYPE_Audio);
	pmt->SetSubtype(&MEDIASUBTYPE_PCM);
	pmt->SetFormatType(&FORMAT_WaveFormatEx);
    pmt->SetTemporalCompression(FALSE);
	pmt->bFixedSizeSamples = TRUE;
    pmt->SetSampleSize(pwav->nBlockAlign);
	pmt->pUnk = NULL;

    return NOERROR;

} // GetMediaType



HRESULT CAudioAECFilterPin::CheckMediaType(const CMediaType *pMediaType)
{
    CheckPointer(pMediaType,E_POINTER);

    if(*(pMediaType->Type()) != MEDIATYPE_Audio)
    {                                                  
        return E_INVALIDARG;
    }

    // Check for the subtypes we support
    const GUID *SubType = pMediaType->Subtype();
    if(*SubType != MEDIASUBTYPE_PCM)
    {
        return E_INVALIDARG;
    }

    // Get the format area of the media type
    WAVEFORMATEX *pwav = (WAVEFORMATEX *) pMediaType->Format();
    if(pwav == NULL)
        return E_INVALIDARG;
	if((pwav->nChannels!=1)||
	   (pwav->wBitsPerSample!=16)||
	   (!IsSampleRateSupport(pwav->nSamplesPerSec)))
    {
        return E_INVALIDARG;
    }
    return S_OK; 
} // CheckMediaType


HRESULT CAudioAECFilterPin::DecideBufferSize(IMemAllocator *pAlloc,
                                      ALLOCATOR_PROPERTIES *pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    WAVEFORMATEX *pwav = (WAVEFORMATEX *) m_mt.Format();
    pProperties->cBuffers = 1;
	pProperties->cbBuffer =  pwav->nSamplesPerSec * pwav->nChannels * (pwav->wBitsPerSample/8) * m_iIntervalTime/1000;
	pProperties->cbAlign = (pwav->wBitsPerSample/8) * pwav->nChannels;
	pProperties->cbPrefix= 0;

    ASSERT(pProperties->cbBuffer);

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    if(Actual.cbBuffer < pProperties->cbBuffer)
    {
        return E_FAIL;
    }
    return NOERROR;

} // DecideBufferSize


//
// SetMediaType
//
// Called when a media type is agreed between filters
//
HRESULT CAudioAECFilterPin::SetMediaType(const CMediaType *pMediaType)
{
    CAutoLock cAutoLock(m_pFilter->pStateLock());

	HRESULT hr = CSourceStream::SetMediaType(pMediaType);

	//HRESULT hr=S_OK;
    if(SUCCEEDED(hr))
    {
		DMO_MEDIA_TYPE mt = {0};
		   // Set DMO output format
		hr = MoInitMediaType(&mt, sizeof(WAVEFORMATEX));
		CHECK_RET(hr, "MoInitMediaType failed");	
		mt.majortype =MEDIATYPE_Audio;
		mt.subtype = MEDIASUBTYPE_PCM;
		mt.lSampleSize = 0;
		mt.bFixedSizeSamples = TRUE;//这里是true,还是false;
		mt.bTemporalCompression = FALSE;
		mt.formattype = FORMAT_WaveFormatEx;
		memcpy(mt.pbFormat,m_mediaType->pbFormat, sizeof(WAVEFORMATEX));
	    
		hr = pDMO->SetOutputType(0, &mt, 0); 
		CHECK_RET(hr, "SetOutputType failed");
		MoFreeMediaType(&mt);
        return NOERROR;
    } 

	return hr;

} // SetMediaType

HRESULT CAudioAECFilterPin::CreateBasicBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8) 
{ 
  //WAVEFORMATEX wfx; 
  DSBUFFERDESC dsbdesc; 
  LPDIRECTSOUNDBUFFER pDsb = NULL;
  HRESULT hr; 
 
  // Set up WAV format structure. 
  WAVEFORMATEX *pwfx = (WAVEFORMATEX *)m_mediaType->pbFormat;
  LOG("Start capture. SampleRate=%d, Channel=%d, BitsPerSample=%d"
	  , pwfx->nSamplesPerSec, pwfx->nChannels, pwfx->wBitsPerSample);

  /*memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
  wfx.wFormatTag = WAVE_FORMAT_PCM; 
  wfx.nSamplesPerSec = psetFormat->nSamplesPerSec;  
  wfx.nChannels = psetFormat->nChannels;  
  wfx.wBitsPerSample = psetFormat->wBitsPerSample;
  wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; */
 
  // Set up DSBUFFERDESC structure. 
 
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
  dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
  dsbdesc.dwFlags = 
    DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; //  流buffer
  dsbdesc.dwBufferBytes = 3 * pwfx->nAvgBytesPerSec; // 足够3秒
  dsbdesc.lpwfxFormat = pwfx; 
 
  // Create buffer. 
 
  hr = lpDirectSound->CreateSoundBuffer(&dsbdesc, &pDsb, NULL); 
  if (SUCCEEDED(hr)) 
  { 
     hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) ppDsb8);
     pDsb->Release();
  } 
  return hr;
} 

//
// OnThreadCreate
//
// As we go active reset the stream time to zero
//
HRESULT CAudioAECFilterPin::OnThreadCreate() 
{
    CAutoLock cAutoLockShared(&m_cSharedState);
    m_rtSampleTime = 0;
	HRESULT hr;
	hr=DirectSoundCreate8(&m_rendDevID, &m_PlayDev, NULL);
	CHECK_RET(hr, "CreateDEV failed");
	if (FAILED(hr))
	{
		m_PlayDev = NULL;
		return E_FAIL;
	}

	if(m_previewHwnd)	
	{
		hr = m_PlayDev->SetCooperativeLevel(m_previewHwnd, DSSCL_PRIORITY);
		CHECK_RET(hr, "SetCooperativeLevel failed");
	}

	hr = CreateBasicBuffer(m_PlayDev,&m_PlayBuffer);
	CHECK_RET(hr, "CreateSoundBuffer failed");
	if (FAILED(hr))
	{
		m_PlayBuffer = NULL;
		return E_FAIL;
	}

	LPVOID lpvWrite;
	DWORD  dwLength;

	if (DS_OK == m_PlayBuffer->Lock(
		  0,          // Offset at which to start lock.
		  0,          // Size of lock; ignored because of flag.
		  &lpvWrite,  // Gets address of first part of lock.
		  &dwLength,  // Gets size of first part of lock.
		  NULL,       // Address of wraparound not needed. 
		  NULL,       // Size of wraparound not needed.
		  DSBLOCK_ENTIREBUFFER))  // Flag.
	{
	  memset(lpvWrite,0,dwLength);
	  m_PlayBuffer->Unlock(
		  lpvWrite,   // Address of lock start.
		  dwLength,   // Size of lock.
		  NULL,       // No wraparound portion.
		  0);         // No wraparound size.
	}
	else
	{
		printf("Lock failed!\n");
	}
	m_PlayBuffer->SetCurrentPosition(0);
	hr = m_PlayBuffer->Play(
    0,  // Unused.
    0,  // Priority for voice management.
    DSBPLAY_LOOPING);
	CHECK_RET(hr, "play failed");


	hr = pDMO->SetOutputType(0, m_mediaType, 0); 
	CHECK_RET(hr, "SetOutputType failed");
	if (FAILED(hr)) return E_FAIL;

	hr = pDMO->AllocateStreamingResources();
	CHECK_RET(hr, "AllocateStreamingResources failed");
	if (FAILED(hr)) return E_FAIL;

    return S_OK;

} // OnThreadCreate

HRESULT CAudioAECFilterPin::OnThreadDestroy()
{
	if (m_PlayBuffer)
	{
		m_PlayBuffer->Stop();
	}
	return NOERROR;
}

STDMETHODIMP CAudioAECFilterPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);

    if (riid == IID_IAMBufferNegotiation) {
        return GetInterface((IAMBufferNegotiation *) this, ppv);

    } else if (riid == IID_IAMStreamConfig) {
        return GetInterface((IAMStreamConfig *) this, ppv);

    }else if (riid == IID_IAECProperty) {
        return GetInterface((IAECProperty *) this, ppv);

    }  else {
        return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
    }

} // NonDelegatingQueryInterface



HRESULT CAudioAECFilterPin::SuggestAllocatorProperties(const ALLOCATOR_PROPERTIES *pprop)
{
	 CAutoLock cAutoLock(m_pFilter->pStateLock());
	  //暂无实现 如要实现，在这里可通过改变m_iIntervalTime，即可改变每次发送音频数据的大小，即通过pprop的大小来确定m_iIntervalTime的大小
/*	HRESULT hr;
	 ALLOCATOR_PROPERTIES Actual;
    hr = m_pAllocator->SetProperties((ALLOCATOR_PROPERTIES *)pprop,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    if(Actual.cbBuffer < pprop->cbBuffer)
    {
        return E_FAIL;
    }*/
    return NOERROR;
}

HRESULT CAudioAECFilterPin::GetAllocatorProperties(
	__out  ALLOCATOR_PROPERTIES *pprop)
{
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	return S_OK;
}

HRESULT CAudioAECFilterPin::SetFormat(AM_MEDIA_TYPE *pmt)
{
	 CAutoLock cAutoLock(m_pFilter->pStateLock());
	WAVEFORMATEX *pwav = (WAVEFORMATEX *) pmt->pbFormat;
	int SampleRate=pwav->nSamplesPerSec;
	int Channels=pwav->nChannels;
	int BitsPerSample=pwav->wBitsPerSample;
	if(!IsSampleRateSupport(SampleRate) || (Channels != 1) || (BitsPerSample != 16))
    {
		printf("设置参数不符合回声消除要求,设置失败!(SampleRate=%d, Channels=%d, BitsPerSample=%d)\n", 
			SampleRate, Channels, BitsPerSample);
        return E_FAIL;
    }
	WAVEFORMATEX *psetFormat=(WAVEFORMATEX *)m_mediaType->pbFormat;

	psetFormat->nSamplesPerSec=pwav->nSamplesPerSec;
	psetFormat->nChannels=pwav->nChannels;
	psetFormat->wBitsPerSample=pwav->wBitsPerSample;
	psetFormat->nAvgBytesPerSec=pwav->nSamplesPerSec*pwav->wBitsPerSample*pwav->nChannels/8;
	psetFormat->nBlockAlign=pwav->wBitsPerSample*pwav->nChannels/8;
	psetFormat->wFormatTag=WAVE_FORMAT_PCM;

	m_mediaType->lSampleSize = psetFormat->nBlockAlign;

	bSetFormat=true;
	return S_OK;
}
        
HRESULT CAudioAECFilterPin::GetFormat( __out  AM_MEDIA_TYPE **ppmt)
{
	CAutoLock cAutoLock(m_pFilter->pStateLock());
	return GetStreamCaps(0, ppmt, NULL);   
}
        
HRESULT CAudioAECFilterPin::GetNumberOfCapabilities(
            __out  int *piCount,
            __out  int *piSize)
{
	  CAutoLock cAutoLock(m_pFilter->pStateLock());
	  CheckPointer(piCount, E_POINTER);
	  CheckPointer(piSize, E_POINTER);

	  *piCount = sizeof(g_sampleRate_table)/sizeof(int);
	  *piSize = sizeof(AUDIO_STREAM_CONFIG_CAPS);

	  return S_OK;
}
        
HRESULT CAudioAECFilterPin::GetStreamCaps( int iIndex,
            __out  AM_MEDIA_TYPE **ppmt,
            __out  BYTE *pSCC)
{
	CAutoLock cAutoLock(m_pFilter->pStateLock());

	if (0 > iIndex)
	{
		return E_INVALIDARG;
	}
	if (iIndex >= sizeof(g_sampleRate_table)/sizeof(int))
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	DWORD nSamplesPerSec = g_sampleRate_table[iIndex];

	*ppmt=CreateMediaType(this->m_mediaType);
	HRESULT hr = MoInitMediaType(*ppmt, sizeof(WAVEFORMATEX));
	CHECK_RET(hr, "MoInitMediaType failed");	

	LPWAVEFORMATEX pfmt = (LPWAVEFORMATEX)(*ppmt)->pbFormat;
	pfmt->cbSize = 0;
	pfmt->wFormatTag = 0x1;
	pfmt->nSamplesPerSec = nSamplesPerSec;
	pfmt->nChannels = 1;
	pfmt->wBitsPerSample = 16;
	pfmt->nBlockAlign = pfmt->nChannels * pfmt->wBitsPerSample / 8;
	pfmt->nAvgBytesPerSec = pfmt->nSamplesPerSec * pfmt->nBlockAlign;

	(*ppmt)->majortype =MEDIATYPE_Audio;
	(*ppmt)->subtype = MEDIASUBTYPE_PCM;
	(*ppmt)->lSampleSize = pfmt->nBlockAlign;
	(*ppmt)->bFixedSizeSamples = TRUE;
	(*ppmt)->bTemporalCompression = FALSE;
	(*ppmt)->formattype = FORMAT_WaveFormatEx;

	return S_OK;
}

//注意，输入设备名称字符串可能是不完整的，因此str2长度可能小于str1长度
//但只要前部相等即可
//返回0表示字符串相等
int CompareStr(wchar_t *str1,int length1,wchar_t *str2,int length2)
{
	//wstring specialSign = L"?";
	int index=0;
	while(index < length1 && index < length2)
	{
		if(str2[index]==L'?')//由于界面提供的名称可能包含?号，会出现不匹配的情况
		{
			index++;
			continue;
		}
		if(str1[index]!=str2[index])
			return 1;//不相等
		index++;
	}
	return index == 0; //index为0返回1，说明有字符串为空
}

HRESULT CAudioAECFilterPin::put_CapDevFriendName(wchar_t* capDevFriendName,int capNameSize,wchar_t* rendDevFriendName,int rendNameSize)
{
	CAutoLock cAutoLock(m_pFilter->pStateLock());

	UINT uCapDevCount = 0;
    UINT uRenDevCount = 0;
	HRESULT hr;
	int  iMicDevIdx = -2;               // microphone device index
    int  iSpkDevIdx = -2;               // speaker device index
	int i;

    m_szCaptureDeviceName = capDevFriendName;
    m_szRenderDeviceName = rendDevFriendName;

    //播放设备如果是DirectSound设备，找到其对应的波形设备
    {
        wchar_t directsound_prefix[] = L"DirectSound: ";
        if (m_szRenderDeviceName.find(directsound_prefix) == 0) {
            m_szRenderDeviceName = m_szRenderDeviceName.substr(wcslen(directsound_prefix));
        }
    }

	AUDIO_DEVICE_INFO *pCaptureDeviceInfo = NULL, *pRenderDeviceInfo = NULL;
    hr = GetCaptureDeviceNum(uCapDevCount);
    pCaptureDeviceInfo = new AUDIO_DEVICE_INFO[uCapDevCount + 1]; // for safety
    hr = EnumCaptureDevice(uCapDevCount, pCaptureDeviceInfo);
    for (i=0; i<(int)uCapDevCount; i++)
    {
		//注：由于QT提供的名称并不完整，故要做部分匹配
		if(CompareStr(pCaptureDeviceInfo[i].szDeviceName, wcslen(pCaptureDeviceInfo[i].szDeviceName),
			const_cast<wchar_t *>(m_szCaptureDeviceName.c_str()), m_szCaptureDeviceName.size())==0)
		{
			iMicDevIdx=i;
			m_capDevID=pCaptureDeviceInfo[i].DeviceGUID;

			OLECHAR* bstrGuid;
			StringFromCLSID(m_capDevID, &bstrGuid);
			LOGW(L"Capture device: %s\n\tGuid:%s", m_szCaptureDeviceName.c_str(), bstrGuid);
			CoTaskMemFree((LPVOID)bstrGuid);			
			break;
		}
	}
	if (i == (int)uCapDevCount)
	{
		LOGW(L"Invalid capture device: %s", capDevFriendName);
	}
    SAFE_ARRAYDELETE(pCaptureDeviceInfo);
    
    if( m_mode == SINGLE_CHANNEL_AEC ||
        m_mode == ADAPTIVE_ARRAY_AND_AEC ||
        m_mode == OPTIBEAM_ARRAY_AND_AEC )
    {
        hr = GetRenderDeviceNum(uRenDevCount);
        pRenderDeviceInfo = new AUDIO_DEVICE_INFO[uRenDevCount + 1];
        hr = EnumRenderDevice(uRenDevCount, pRenderDeviceInfo);
        for (i=0; i<(int)uRenDevCount; i++)
        {
			if(CompareStr(pRenderDeviceInfo[i].szDeviceName, wcslen(pRenderDeviceInfo[i].szDeviceName), 
				const_cast<wchar_t *>(m_szRenderDeviceName.c_str()), m_szRenderDeviceName.size())==0)
			{
				iSpkDevIdx=i;
				m_rendDevID=pRenderDeviceInfo[i].DeviceGUID;

				OLECHAR* bstrGuid;
				StringFromCLSID(m_rendDevID, &bstrGuid);
				LOGW(L"Render device: %s\n\tGuid:%s", m_szRenderDeviceName.c_str(), bstrGuid);
				CoTaskMemFree((LPVOID)bstrGuid);
				break;
			}
        }
		if (i == (int)uRenDevCount)
		{
			LOGW(L"Invalid render device: %s", rendDevFriendName);
		}
    }
	else
	{
        iSpkDevIdx = -1;
    }
    SAFE_ARRAYDELETE(pRenderDeviceInfo);
 
    PROPVARIANT pvSysMode;
    PropVariantInit(&pvSysMode);
    pvSysMode.vt = VT_I4;
    pvSysMode.lVal = (LONG)(m_mode);
    CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, pvSysMode));
    CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, &pvSysMode));
    PropVariantClear(&pvSysMode);

	//dsh temp
	/*iMicDevIdx=0;
	iSpkDevIdx=0;*/


    if (iMicDevIdx >= 0 || iSpkDevIdx >= 0)
    {
        PROPVARIANT pvDeviceId;
        PropVariantInit(&pvDeviceId);
        pvDeviceId.vt = VT_I4;
        pvDeviceId.lVal = ((unsigned long)(iSpkDevIdx<<16)) | ((unsigned long)(0x0000ffff & iMicDevIdx));
        CHECKHR(pPS->SetValue(MFPKEY_WMAAECMA_DEVICE_INDEXES, pvDeviceId));
        CHECKHR(pPS->GetValue(MFPKEY_WMAAECMA_DEVICE_INDEXES, &pvDeviceId));
        PropVariantClear(&pvDeviceId);
    }
	/*FILE *fp;
	fp=fopen("c://audio_dev.txt","at");

	fprintf(fp,"iMicDevIdx %d, iSpkDevIdx %d\n",iMicDevIdx,iSpkDevIdx);
	fclose(fp);*/
	return S_OK;
}

HRESULT CAudioAECFilterPin::put_PreviewHandle(HWND hPreviewHandle)
{
	CAutoLock cAutoLock(m_pFilter->pStateLock());

	m_previewHwnd=hPreviewHandle;
	return S_OK;
}
//void CAudioAECFilterPin::recordMessage(char* message)
//{
//#ifdef _DEBUG
//	fprintf(errorlog,"%s\n",message);
//#endif
//}

