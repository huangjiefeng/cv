// NEXT.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "NEXT.h"
#include <stdio.h>

#define _OUTPUT_FMT_YUY2

#define DEC_LOG(x, ...) printf_s("[SCUT_X264_DEC] " x, __VA_ARGS__)


typedef struct tagVIDEOINFOHEADER2 {
	RECT rcSource;
	RECT rcTarget;
	DWORD dwBitRate;
	DWORD dwBitErrorRate;
	REFERENCE_TIME AvgTimePerFrame;
	DWORD dwInterlaceFlags;
	DWORD dwCopyProtectFlags;
	DWORD dwPictAspectRatioX;
	DWORD dwPictAspectRatioY; 
	DWORD dwReserved1;
	DWORD dwReserved2;
	BITMAPINFOHEADER bmiHeader;
} VIDEOINFOHEADER2;

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_NULL,       // Major type
		&MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN psudPins[] =
{
	{
		L"Input",           // String pin name
			FALSE,              // Is it rendered
			FALSE,              // Is it an output
			FALSE,              // Allowed none
			FALSE,              // Allowed many
			&CLSID_NULL,        // Connects to filter
			L"Output",          // Connects to pin
			1,                  // Number of types
			&sudPinTypes },     // The pin details
		{ L"Output",          // String pin name
		FALSE,              // Is it rendered
		TRUE,               // Is it an output
		FALSE,              // Allowed none
		FALSE,              // Allowed many
		&CLSID_NULL,        // Connects to filter
		L"Input",           // Connects to pin
		1,                  // Number of types
		&sudPinTypes        // The pin details
		}
};

const AMOVIESETUP_FILTER sudContrast =
{
	&CLSID_SCUTH264Decoder,        // Filter CLSID
		L"SCUT H264 Decoder",      // Filter name
		MERIT_DO_NOT_USE,       // Its merit
		2,                      // Number of pins
		psudPins                // Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[1] = 
{
	{ L"Video Contrast"
		, &CLSID_SCUTH264Decoder
		, CTransform::CreateInstance
		, NULL
		, &sudContrast }

};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


CTransform::CTransform(TCHAR *tszName,LPUNKNOWN punk,HRESULT *phr) :
CTransformFilter(tszName, punk, CLSID_SCUTH264Decoder)
{
	ASSERT(tszName);
	ASSERT(phr);

	m_nActualWidth = 0;
	m_bUseSurfaceStride = false;
	m_bInitDecoder=false;
	//buffer=NULL;
	m_pCodecCtx=NULL;
	m_pFrame = NULL;
	m_pYuvToOutFmtCtx = NULL;

} // Contrast


CUnknown * WINAPI CTransform::CreateInstance(LPUNKNOWN punk, HRESULT *phr) 
{
	ASSERT(phr);

	CTransform *pNewObject = new CTransform(NAME("Tranform"), punk, phr);
	if (pNewObject == NULL) {
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;

} // CreateInstance

CTransform::~CTransform()
{

}

//

HRESULT CTransform::Transform(IMediaSample *pSource, IMediaSample *pDest)
{
	CheckPointer(pSource,E_POINTER);
	CheckPointer(pDest,E_POINTER);

	HRESULT hr = NOERROR;

	// 检查输出媒体格式变更
	AM_MEDIA_TYPE * pmtOut = NULL;
	pDest->GetMediaType(&pmtOut);

	if (NULL != pmtOut)
	{
		if (pmtOut->formattype == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2 *pInfo = (VIDEOINFOHEADER2 *)pmtOut->pbFormat;
			m_nActualWidth = pInfo->bmiHeader.biWidth;
		}
		else if (pmtOut->formattype == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER *pInfo = (VIDEOINFOHEADER *)pmtOut->pbFormat;
			m_nActualWidth = pInfo->bmiHeader.biWidth;
		}

		m_bUseSurfaceStride = true;
		DEC_LOG("Downstream filter request surface stride. video_width=%d\n", m_nActualWidth);
	}

	// 检查输入媒体格式变更
	const CMediaType *pmtIn = NULL;
	pSource->GetMediaType((AM_MEDIA_TYPE **)&pmtIn);

	if (NULL != pmtIn)
	{
		DEC_LOG("Upstream filter notify that media type changed.\n");

		hr = CheckInputType(pmtIn);
		if (FAILED(hr)) return hr;

		hr = _InitDecoder();
		if (FAILED(hr)) return hr;

		const CMediaType mtOut = _GetOutputType(*pmtIn);
		pDest->SetMediaType((AM_MEDIA_TYPE *)&mtOut);

	}

	if(m_bInitDecoder==false)
	{
		return E_UNEXPECTED;
	}

	BYTE  *pBufferSource;
	BYTE  *pBufferDest;
	int frameFinished;
	pSource->GetPointer(&pBufferSource);
	pDest->GetPointer(&pBufferDest);	
	int usedBytes=0;
	long size=pSource->GetActualDataLength();

	//picture存储了解码出来的数据，bufferSize如果大于0表示解码成功，
	//pBufferSource是接收到的编码之后的数据，size是需要进行解码的数据位流大小
	//added by zhenHua.sun 10-03-23
	AVPacket packet;
	av_init_packet(&packet);
	packet.data=pBufferSource;
	packet.size=size;

	usedBytes=avcodec_decode_video2(m_pCodecCtx,m_pFrame,&frameFinished,&packet);

	//不知道为何，在调用第二次的时候才能获得解码帧，因此出现了两次同样的函数。暂时先这样
	//以后有问题的时候再修改......added by zhenHua.sun 10-03-29
	if( usedBytes==size && frameFinished==0 )
	{
		avcodec_decode_video2(m_pCodecCtx,m_pFrame,&frameFinished,&packet);	
	}
	
////////////////////////////////////////
	if( frameFinished  >   0 ){ 

		AVPicture   targetFrame; 
		DWORD   dwActualDstLength; 

		memset(&targetFrame,0,sizeof(AVPicture)); 


		// 处理YUY到RGB的图像翻转, by Liaokz, 2012-05
#ifdef _OUTPUT_FMT_RGB24
		const int nBytePerPix = 3;

		dwActualDstLength   =   m_pCodecCtx->width * m_pCodecCtx->height * nBytePerPix; 

		avpicture_fill(&targetFrame,pBufferDest,PIX_FMT_BGR24,m_pCodecCtx->width,m_pCodecCtx->height); 
		targetFrame.data[0] = targetFrame.data[0] + (m_pCodecCtx->width * (m_pCodecCtx->height - 1) * nBytePerPix);
		targetFrame.linesize[0] = -targetFrame.linesize[0];
#else
		{
			const int nBytePerPix = 2;
			const int nWidth = 0 == m_nActualWidth ? m_pCodecCtx->width : m_nActualWidth;

			dwActualDstLength = nWidth * m_pCodecCtx->height * nBytePerPix; 

			avpicture_fill(&targetFrame,pBufferDest,PIX_FMT_YUYV422,m_pCodecCtx->width,m_pCodecCtx->height); 
			targetFrame.linesize[0] = nWidth * nBytePerPix;
		}
#endif

		sws_scale( m_pYuvToOutFmtCtx , m_pFrame->data , m_pFrame->linesize , 0 , m_pCodecCtx->height , 
			targetFrame.data , targetFrame.linesize );

		pDest->SetActualDataLength(dwActualDstLength); 
		
	}

///////////////////////////////////////////

	//add by dsh 
	REFERENCE_TIME rtStart,rtEnd;
	pSource->GetTime(&rtStart,&rtEnd);
	pDest->SetTime(&rtStart,&rtEnd);
	//end by dsh 		

	return NOERROR;
}

HRESULT CTransform::CheckInputType(const CMediaType *mtIn)
{
	VIDEOINFOHEADER     info;

	ZeroMemory(&info, sizeof(VIDEOINFOHEADER));
	info=*((VIDEOINFOHEADER *)mtIn->pbFormat);
	if(mtIn->majortype==MEDIATYPE_Video)return NOERROR;
	return E_FAIL;
}

HRESULT CTransform::CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut)
{
#ifdef _OUTPUT_FMT_RGB24

	if (mtOut->subtype == MEDIASUBTYPE_RGB24)
	{
		return NOERROR;
	}

#else

	if (mtOut->subtype == MEDIASUBTYPE_YUY2)
	{
		return NOERROR;
	}

#endif

	return E_FAIL;
}

HRESULT CTransform::GetMediaType(int iPosition, CMediaType *pMediaType)
{

	// This should never happen

	if(iPosition < 0)
	{
		return E_INVALIDARG;
	}

	// Do we have more items to offer

	if(iPosition > 0)
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	CheckPointer(pMediaType, E_POINTER);	
	
	const CMediaType mtIn = m_pInput->CurrentMediaType();

	*pMediaType = _GetOutputType(mtIn);

	return NOERROR;
}

HRESULT CTransform::DecideBufferSize(IMemAllocator *pAlloc,
									 ALLOCATOR_PROPERTIES *pProperties)
{
	CheckPointer(pAlloc,E_POINTER);
	CheckPointer(pProperties,E_POINTER);

	// Is the input pin connected

	if(m_pInput->IsConnected() == FALSE)
	{
		return E_UNEXPECTED;
	}

	HRESULT hr = NOERROR;
	pProperties->cBuffers = 1;   
	pProperties->cbAlign = 1; 

	/*const CMediaType &mtIn = m_pInput->CurrentMediaType();
	BITMAPINFOHEADER * pbmiHeader = NULL;

	if (mtIn.formattype == FORMAT_VideoInfo2)
	{
		pbmiHeader = &((VIDEOINFOHEADER2 *)mtIn.pbFormat)->bmiHeader;
	}
	else if (mtIn.formattype == FORMAT_VideoInfo)
	{
		pbmiHeader = &((VIDEOINFOHEADER *)mtIn.pbFormat)->bmiHeader;
	}
	else
	{
		return E_UNEXPECTED;
	}*/

#ifdef _OUTPUT_FMT_RGB24
	const int nBytePerPix = 3; 
#else
	const int nBytePerPix = 2;
#endif

	//pProperties->cbBuffer = pbmiHeader->biWidth * pbmiHeader->biHeight * nBytePerPix; 

	pProperties->cbBuffer = 1920 * 1080 * nBytePerPix;

	//   Get   input   pin's   allocator   size   and   use   that   
	/*ALLOCATOR_PROPERTIES   InProps;   
	IMemAllocator   *   pInAlloc   =   NULL;   

	hr   =   m_pInput->GetAllocator(&pInAlloc);   
	if(SUCCEEDED(hr))   
	{   
		hr   =   pInAlloc->GetProperties(&InProps);   
		if(SUCCEEDED(hr))   
		{   
			   pProperties->cbBuffer   =   InProps.cbBuffer*2; 
		}   
		pInAlloc->Release();   
	}   

	if(FAILED(hr))   
		return   hr;  */ 

	ASSERT(pProperties->cbBuffer);   

	//   Ask   the   allocator   to   reserve   us   some   sample   memory,   NOTE   the   function   
	//   can   succeed   (that   is   return   NOERROR)   but   still   not   have   allocated   the   
	//   memory   that   we   requested,   so   we   must   check   we   got   whatever   we   wanted   

	ALLOCATOR_PROPERTIES   Actual;   
	hr   =   pAlloc->SetProperties(pProperties,&Actual);   
	if(FAILED(hr))   
	{   
		return   hr;   
	}   

	ASSERT(Actual.cBuffers == 1);   

	if(pProperties->cBuffers > Actual.cBuffers   ||   
		pProperties->cbBuffer > Actual.cbBuffer)   
	{   
		return E_FAIL;   
	}   

	return NOERROR;   
	
}

STDMETHODIMP CTransform::Run( REFERENCE_TIME tStart )
{
	m_nActualWidth = 0;
	m_bUseSurfaceStride = false;

	HRESULT hr = _InitDecoder();
	if (FAILED(hr))
	{
		return hr;
	}
	return CTransformFilter::Run(tStart);

}

STDMETHODIMP CTransform::Stop()
{
	HRESULT hr = CTransformFilter::Stop();
	if (FAILED(hr))
	{
		return hr;
	}

	m_nActualWidth = 0;
	m_bUseSurfaceStride = false;

	return _DestroyDecoder();

}

HRESULT CTransform::_InitDecoder()
{
	if (!m_pInput->IsConnected())
	{
		return S_FALSE;
	}

	_DestroyDecoder();

	const CMediaType myMediaType = m_pInput->CurrentMediaType();

	BITMAPINFOHEADER * pbmiHeader = NULL;

	if (myMediaType.formattype == FORMAT_VideoInfo2)
	{
		pbmiHeader = &((VIDEOINFOHEADER2 *)myMediaType.pbFormat)->bmiHeader;
	}
	else if (myMediaType.formattype == FORMAT_VideoInfo)
	{
		pbmiHeader = &((VIDEOINFOHEADER *)myMediaType.pbFormat)->bmiHeader;
	}
	else
	{
		return E_UNEXPECTED;
	}

	//start-----------------------modified by zhenHua.sun 10-3-23-----

	// Register all formats and codecs
	av_register_all();

	// Find the decoder for the video stream
	m_pCodec = avcodec_find_decoder(CODEC_ID_H264);
	if (m_pCodec == NULL)
		return E_FAIL; // Codec not found
    
	//指定解码器为H264
	m_pCodecCtx=avcodec_alloc_context3(m_pCodec);	
	
	m_pCodecCtx->width = pbmiHeader->biWidth;
	m_pCodecCtx->height = pbmiHeader->biHeight;

	// Inform the codec that we can handle truncated bitstreams -- i.e.,
	// bitstreams where frame boundaries can fall in the middle of packets
	if(m_pCodec->capabilities & CODEC_CAP_TRUNCATED)
		m_pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
	
	// Open codec
	m_pCodecCtx->thread_count = 1;			//设置线程数...added by zhenHua.sun 10-04-06
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
		return E_FAIL; // Could not open codec

	//// Allocate video frame
	//picture = avcodec_alloc_frame();
	//if(picture == NULL)
	//	return E_FAIL;

	m_pCodec->init( m_pCodecCtx );

	/*
	//固化解码参数......added by zhenHua.sun 09-11-06
	//sps:00 00 00 01 67 4d 40 33 92 54 05 01 ed 08 00 00 03 00 08 00 00 03 01 94 78 c1 95,27bytes
	//pps:00 00 00 01 68 ee 3c 80,8bytes
	char sps_pps[] = {0x00,0x00,0x00,0x01,0x67,0x4d,0x40,0x33,0x92,0x54,0x05,0x01,0xed,0x08,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x03,0x01,0x94,0x78,0xc1,0x95,
		0x00,0x00,0x00,0x01,0x68,0xee,0x3c,0x80};
	pCodecCtx->extradata_size = 35;
	pCodecCtx->extradata = new uint8_t[pCodecCtx->extradata_size];
	memcpy( pCodecCtx->extradata , sps_pps , 35 );
	*/

	//Initial sws context
	m_pYuvToOutFmtCtx = sws_getContext(
        m_pCodecCtx->width,				//srcW
        m_pCodecCtx->height,			//srcH
		PIX_FMT_YUV420P,			//srcFormat
        m_pCodecCtx->width,				//dstW
        m_pCodecCtx->height,			//dstH
#ifdef _OUTPUT_FMT_RGB24
		PIX_FMT_BGR24,					//dstFormat
#else
		PIX_FMT_YUYV422,
#endif
        SWS_FAST_BILINEAR, NULL, NULL, NULL);
	if (m_pYuvToOutFmtCtx == NULL) 
	{
		fprintf(stderr, "Cannot get resampling context\n");
		return E_FAIL;
	}
	//Allocate buffer
	//buffer=new BYTE[(pVideoInfo->bmiHeader.biWidth+2)*(pVideoInfo->bmiHeader.biHeight+2)*4+200];  //200算是pad，防止溢出的
	m_pFrame = avcodec_alloc_frame();
	m_bInitDecoder=true;

	return NOERROR;

}

HRESULT CTransform::_DestroyDecoder()
{
	// Free SWS Context
	if ( m_pYuvToOutFmtCtx )
	{
		sws_freeContext( m_pYuvToOutFmtCtx );
		m_pYuvToOutFmtCtx = NULL;
	}
	// Free the YUV frame
	if ( m_pFrame )
	{
		av_free(m_pFrame);
		m_pFrame = NULL;
	}
	// Close the codec
	if ( m_pCodecCtx )
	{
		avcodec_close(m_pCodecCtx);
		m_pCodecCtx = NULL;
	}

	m_bInitDecoder = false;

	return S_OK;

}

CMediaType CTransform::_GetOutputType( const CMediaType &mtIn )
{
	CMediaType mtOut;

#ifdef _OUTPUT_FMT_RGB24
	const int nBitCount = 24;
#else
	const int nBitCount = 16;
#endif

	if(mtIn.formattype==FORMAT_VideoInfo2){

		VIDEOINFOHEADER2 * preInfo=(VIDEOINFOHEADER2 *)mtIn.pbFormat;
		VIDEOINFOHEADER info = {0};

		info.rcSource.right = preInfo->bmiHeader.biWidth;
		info.rcSource.bottom = preInfo->bmiHeader.biHeight;
		info.rcTarget.right = preInfo->bmiHeader.biWidth;
		info.rcTarget.bottom = preInfo->bmiHeader.biHeight;
		info.AvgTimePerFrame = preInfo->AvgTimePerFrame;
		info.dwBitRate = preInfo->bmiHeader.biWidth * preInfo->bmiHeader.biHeight * nBitCount * ((REFERENCE_TIME)1e7 / preInfo->AvgTimePerFrame);

		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth  = m_bUseSurfaceStride ? ((((preInfo->bmiHeader.biWidth * nBitCount) + 31) & ~31) >> 3) : preInfo->bmiHeader.biWidth;
		info.bmiHeader.biHeight = preInfo->bmiHeader.biHeight;
		info.bmiHeader.biBitCount  = nBitCount;
		info.bmiHeader.biSizeImage = preInfo->bmiHeader.biWidth * preInfo->bmiHeader.biHeight * info.bmiHeader.biBitCount / 8;

#ifdef _OUTPUT_FMT_RGB24
		info.bmiHeader.biCompression = BI_RGB;
#else
		info.bmiHeader.biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');
#endif

		info.bmiHeader.biPlanes = 1;

		/*char sps_pps[23]={0x4d,0x40,0x33,0x9a,0x74,0x0a,0x0f,0xd0,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x19,0x47,0x8c,0x19,0x50,0xee,0x3c,0x80};
		memcpy(info+sizeof(VIDEOINFOHEADER),sps_pps,23);*/
		mtOut.SetFormat((BYTE*)&info, sizeof(VIDEOINFOHEADER));
		mtOut.SetSampleSize(info.bmiHeader.biSizeImage);

	}else if(mtIn.formattype==FORMAT_VideoInfo){

		VIDEOINFOHEADER * preInfo=(VIDEOINFOHEADER *)mtIn.pbFormat;
		VIDEOINFOHEADER   info = {0};

		info.rcSource.right = preInfo->bmiHeader.biWidth;
		info.rcSource.bottom = preInfo->bmiHeader.biHeight;
		info.rcTarget.right = preInfo->bmiHeader.biWidth;
		info.rcTarget.bottom = preInfo->bmiHeader.biHeight;
		info.AvgTimePerFrame = preInfo->AvgTimePerFrame;
		info.dwBitRate = preInfo->bmiHeader.biWidth * preInfo->bmiHeader.biHeight * nBitCount * ((REFERENCE_TIME)1e7 / preInfo->AvgTimePerFrame);

		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth  = m_bUseSurfaceStride ? ((((preInfo->bmiHeader.biWidth * nBitCount) + 31) & ~31) >> 3) : preInfo->bmiHeader.biWidth;
		info.bmiHeader.biHeight = preInfo->bmiHeader.biHeight;
		info.bmiHeader.biBitCount  = nBitCount;
		info.bmiHeader.biSizeImage = info.bmiHeader.biWidth * info.bmiHeader.biHeight * info.bmiHeader.biBitCount / 8;

#ifdef _OUTPUT_FMT_RGB24
		info.bmiHeader.biCompression = BI_RGB;
#else
		info.bmiHeader.biCompression = MAKEFOURCC('Y', 'U', 'Y', '2');
#endif

		info.bmiHeader.biPlanes = 1;	

		mtOut.SetFormat((BYTE*)&info, sizeof(VIDEOINFOHEADER));
		mtOut.SetSampleSize(info.bmiHeader.biSizeImage);

	}

	mtOut.SetType(&MEDIATYPE_Video);

#ifdef _OUTPUT_FMT_RGB24
	mtOut.SetSubtype(&MEDIASUBTYPE_RGB24);
#else
	mtOut.SetSubtype(&MEDIASUBTYPE_YUY2);
#endif

	mtOut.SetFormatType(&FORMAT_VideoInfo);
	mtOut.bFixedSizeSamples = TRUE;
	mtOut.bTemporalCompression = FALSE;

	return mtOut;

}




////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterServer
//
// Handle registration of this filter
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
return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

