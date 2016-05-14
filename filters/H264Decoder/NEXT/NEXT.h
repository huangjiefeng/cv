// Transform.h : Transform DLL 的主头文件
//

#pragma once
#include <streams.h>
#include <stdio.h>

#include <windows.h>
#include <initguid.h>
#include <olectl.h>

//start------------------from sdk example, added by zhenHua.sun
extern "C"{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libavutil\avutil.h>
#include <libswscale\swscale.h>
}

#include <stdlib.h>
#include <time.h>

//end--------

#if (1100 > _MSC_VER)
#include <olectlid.h>
#endif

// {7398BAAE-0BB0-4565-99A6-4480DFD4A973}
DEFINE_GUID(CLSID_SCUTH264Decoder, 
			0x7398baae, 0xbb0, 0x4565, 0x99, 0xa6, 0x44, 0x80, 0xdf, 0xd4, 0xa9, 0x73);

class CTransform : public CTransformFilter
{

public:

	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	// Reveals IContrast & ISpecifyPropertyPages
	//STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	DECLARE_IUNKNOWN;

	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Stop();

	HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
	HRESULT CheckInputType(const CMediaType *mtIn);
	HRESULT CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut);
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	HRESULT DecideBufferSize(IMemAllocator *pAlloc,
		ALLOCATOR_PROPERTIES *pProperties);

    ~CTransform();

private:

	// Constructor
	CTransform(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	CMediaType _GetOutputType(const CMediaType &mtIn);

	HRESULT _InitDecoder();
	HRESULT _DestroyDecoder();
	
	AVCodecContext  *m_pCodecCtx;
	AVCodec         *m_pCodec;
	AVFrame			*m_pFrame;
	SwsContext		*m_pYuvToOutFmtCtx;

	int				 m_nActualWidth;	// YUY2 输出到Renderer时会调整宽度进行显示对齐
	bool			 m_bUseSurfaceStride;

	//UINT8			*buffer;
	bool			 m_bInitDecoder;
	CCritSec		 m_ContrastLock;


}; // CContrast

