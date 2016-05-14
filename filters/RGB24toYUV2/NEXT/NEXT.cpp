// NEXT.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"

#include "Dshow.h"

extern "C"
{
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
}

#include "NEXT.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>


//struct x264_t;    //还没有解决，是否是调用Lib中的定义？？陈俊霖09-08-21//放假回来先改这个地方---------------------重要

#define DATA_MAX 3000000
uint8_t data[DATA_MAX];

//typedef struct tagVIDEOINFOHEADER2 {
//	RECT rcSource;
//	RECT rcTarget;
//	DWORD dwBitRate;
//	DWORD dwBitErrorRate;
//	REFERENCE_TIME AvgTimePerFrame;
//	DWORD dwInterlaceFlags;
//	DWORD dwCopyProtectFlags;
//	DWORD dwPictAspectRatioX;
//	DWORD dwPictAspectRatioY; 
//	DWORD dwReserved1;
//	DWORD dwReserved2;
//	BITMAPINFOHEADER bmiHeader;
//} VIDEOINFOHEADER2;


typedef struct tagRGB24{
        unsigned char    b;
        unsigned char    g;
        unsigned char    r;
} RGB24;

/*BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}*/

// 73646976-0000-0010-8000-00AA00389B71  'vids' == MEDIATYPE_Video
//DEFINE_GUID(MEDIASUBTYPE_H264,
//			   0x3436324c, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);//原来是这样的，只为了配合硬解码改的　dsh 10-11-28
//DEFINE_GUID(MEDIASUBTYPE_H264,
//			0x34363248, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);//windows sdk中包含的dshow已经内置h264类型


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
	&CLSID_RGB24TOYUV2,        // Filter CLSID
		L"RGB24 to YUV2",      // Filter name
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
		, &CLSID_RGB24TOYUV2
		, CTransform::CreateInstance
		, NULL
		, &sudContrast }

};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


CTransform::CTransform(TCHAR *tszName,LPUNKNOWN punk,HRESULT *phr) :
CTransformFilter(tszName, punk, CLSID_RGB24TOYUV2), pBufferDest(NULL)
{
	ASSERT(tszName);
	ASSERT(phr);
	
	init = false;
	width = 1920;
	height = 1080;

	YUV2_Value = NULL;

	sub_cam=YUY2;//设置默认的媒体输入为YUY2的颜色空间  陈俊霖  09-12-11

	data_result = new BYTE[1900000];

	//m_pRGBInverse=NULL;
	
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
    delete [] data_result;// 09-09-11 add
	if(YUV2_Value)delete [] YUV2_Value;

	/*if(m_pRGBInverse)
	delete m_pRGBInverse;*/
	//m_pRGBInverse=NULL;
}
/*----------YUY2转换为YV12-YUV420P转换程序----------*/
BOOL YUV422To420(BYTE* pYUV, BYTE* pY, BYTE* pU, BYTE* pV, LONG lWidth, LONG lHeight) 
{ 

int i,j; 

BYTE* pYUVTemp = pYUV; 
BYTE* pYUVTempNext = pYUV+lWidth*2; 

for(i=0; i <lHeight; i+=2) 
{ 
	for(j=0; j <lWidth; j+=2) 
	{ 
		pY[j] = *pYUVTemp++; 
        pY[j+lWidth] = *pYUVTempNext++; 
        pU[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2; 
        pYUVTemp++; 
        pYUVTempNext++; 

        pY[j+1] = *pYUVTemp++; 
        pY[j+1+lWidth] = *pYUVTempNext++; 

        pV[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2; 
        pYUVTemp++; 
        pYUVTempNext++; 
	} 
	pYUVTemp+=lWidth*2; 
    pYUVTempNext+=lWidth*2; 
    pY+=lWidth*2; 
    pU+=lWidth/2; 
    pV+=lWidth/2; 
} 

return TRUE; 
}
/*-------------------------------------------------*/

/***********************************************************************************************
功能：HDYC(UYVY,高清)转换为YV12(YUV420P)
接口：
         BYTE* HDYC        ：输入转换像素内容的指针；
		 BYTE* pY          ：转换后YUV420P中的像素Y的像素序列的指针；
		 BYTE* pU          ：转换后YUV420P中的像素U的像素序列的指针；
		 BYTE* pV          ：转换后YUV420P中的像素V的像素序列的指针；
         lWidth            ：参加转换的视频图像的宽度
		 lHeight           ：参加转换的视频图像的高度
作者：陈俊霖
时间：2009-12-10
************************************************************************************************
*/
BOOL HDYC2YUV420P(BYTE* HDYC, BYTE* pY, BYTE* pU, BYTE* pV, LONG lWidth, LONG lHeight)
{
	int i,j; 

BYTE* pYUVTemp = HDYC; 
BYTE* pYUVTempNext = HDYC+lWidth*2; 

for(i=0; i <lHeight; i+=2) 
{ 
	for(j=0; j <lWidth; j+=2) 
	{ 
		pU[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2;
		*pYUVTemp++;
		pYUVTempNext++;
		pY[j] = *pYUVTemp++; 
		pY[j+lWidth] = *pYUVTempNext++;

		pV[j/2] =(*(pYUVTemp) + *(pYUVTempNext))/2;
		*pYUVTemp++;
		pYUVTempNext++;
		pY[j+1] = *pYUVTemp++; 
		pY[j+1+lWidth] = *pYUVTempNext++;
		
	} 
	pYUVTemp+=lWidth*2; 
    pYUVTempNext+=lWidth*2; 
    pY+=lWidth*2; 
    pU+=lWidth/2; 
    pV+=lWidth/2; 
}
	return TRUE;
}

/**********************************************************************************************/
/***********************************************************************************************
功能：RGB24转换为YV12(YUV420P)
接口：
         BYTE* RGB24        ：输入转换像素内容的指针；
		 BYTE* pY          ：转换后YUV420P中的像素Y的像素序列的指针；
		 BYTE* pU          ：转换后YUV420P中的像素U的像素序列的指针；
		 BYTE* pV          ：转换后YUV420P中的像素V的像素序列的指针；
         lWidth            ：参加转换的视频图像的宽度
		 lHeight           ：参加转换的视频图像的高度
作者：陈俊霖
时间：2009-12-10
************************************************************************************************
*/
BOOL RGB242YUV420P(BYTE* RGB24, BYTE* pY, BYTE* pU, BYTE* pV, LONG lWidth, LONG lHeight)
{
int wrap, wrap3, x, y;
    int r, g, b, r1, g1, b1;
    BYTE *p;
	BYTE *src,*lum,*cb,*cr;
	int width,height;
	src=RGB24;
	lum=pY;
	cb=pU;
	cr=pV;
	width=lWidth;
	height=lHeight;


    wrap = width;
    wrap3 = width * 3;
    p = src;
    for(y=0;y<height;y+=2) {
        for(x=0;x<width;x+=2) {
            r = p[0];
            g = p[1];
            b = p[2];
            r1 = r;
            g1 = g;
            b1 = b;
            lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g +
                      FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            r = p[3];
            g = p[4];
            b = p[5];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g +
                      FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            p += wrap3;
            lum += wrap;

            r = p[0];
            g = p[1];
            b = p[2];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g +
                      FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            r = p[3];
            g = p[4];
            b = p[5];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g +
                      FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

            cb[0] = ((- FIX(0.16874) * r1 - FIX(0.33126) * g1 +
                      FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
            cr[0] = ((FIX(0.50000) * r1 - FIX(0.41869) * g1 -
                     FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;

            cb++;
            cr++;
            p += -wrap3 + 2 * 3;
            lum += -wrap + 2;
        }
        p += wrap3;
        lum += wrap;
    }

	return TRUE;
}
/**********************************************************************************************/

/***********************************************************************************************
功能：颜色空间转换函数，根据输入视频的颜色空间进行相应的转换，sub_cam为全局的颜色空间类型
接口：
         BYTE* pYUV        ：输入转换像素内容的指针；
		 BYTE* pY          ：转换后YUV420P中的像素Y的像素序列的指针；
		 BYTE* pU          ：转换后YUV420P中的像素U的像素序列的指针；
		 BYTE* pV          ：转换后YUV420P中的像素V的像素序列的指针；
         lWidth            ：参加转换的视频图像的宽度
		 lHeight           ：参加转换的视频图像的高度
作者：陈俊霖
时间：2009-12-10
************************************************************************************************
*/
BOOL CTransform::DoColorChange(BYTE* pYUV, BYTE* pOUT, LONG lWidth, LONG lHeight)
{
	 if(sub_cam==RGB24)
	 {
		 /*for(int i=lHeight-1,j=0;i>=0;i--,j++)
		 {
		 memcpy(m_pRGBInverse+j*lWidth*3,pYUV+i*lWidth*3,lWidth*3);
		 }*/
		AVPicture   srcPic;
		AVPicture   destPic;
		memset(&srcPic,0,sizeof(AVPicture)); 

		//处理图像翻转。Windows中，RGB24实际是Libav的BGR24格式，且与YUYV(YUY2)垂直翻转
		srcPic.data[0] = pYUV + lWidth * (lHeight - 1) * 3;  // 指向图像数据最后一行起始
		srcPic.linesize[0] = -3 * lWidth; // 倒序往前扫描

		destPic.data[0] = pOUT;
		destPic.linesize[0] = 2*lWidth;


		//Initial sws context
		pYuyToBgraCtx = sws_getContext(
			lWidth,				//srcW
			lHeight,			//srcH
			PIX_FMT_BGR24,		//srcFormat
			lWidth,				//dstW
			lHeight,			//dstH
			PIX_FMT_YUYV422,	//dstFormat
			/*SWS_BICUBIC*/SWS_FAST_BILINEAR, NULL, NULL, NULL);


		sws_scale( pYuyToBgraCtx , srcPic.data , srcPic.linesize , 0, lHeight , 
			destPic.data , destPic.linesize );

		if (pYuyToBgraCtx) sws_freeContext(pYuyToBgraCtx);

		return true;
	 }
	 return false;
}



//
/**********************************************************************************************/
HRESULT CTransform::Transform(IMediaSample *pSource, IMediaSample *pDest)
{
	CheckPointer(pSource,E_POINTER);
	CheckPointer(pDest,E_POINTER);

    //陈俊霖 add 2009-09-11
	BYTE * pBufferSource = NULL;

	if (!init)
	{
		CMediaType myMediaType;
		myMediaType=m_pInput->CurrentMediaType();

		VIDEOINFOHEADER * pVideoInfo=(VIDEOINFOHEADER *)myMediaType.pbFormat;
		width=pVideoInfo->bmiHeader.biWidth;
		height=pVideoInfo->bmiHeader.biHeight;

		if(myMediaType.subtype==MEDIASUBTYPE_RGB24)
		{
			sub_cam = RGB24;
		}
		else if(myMediaType.subtype==MEDIASUBTYPE_YUY2)
		{
			sub_cam = YUY2;
		}

		YUV2_Value = new BYTE[ 2 * width * height ];
		init = true;
	}

	if( sub_cam == RGB24 )
	{
		REFERENCE_TIME pTimeStart;
		REFERENCE_TIME pTimeEnd;
			    
		int len;
		pSource->GetPointer(&pBufferSource);
		len=pSource->GetActualDataLength();
		//pDest->GetPointer(&pBufferDest);	
			
		/*将pBufferSource指向的640*480的YUY2的数据转换成YV12-YUV420的数据存进pic.img.plane中，
		其中plane[0]存放的是Y的数据;plane[1]存放的是U的数据;plane[2]存放的是V的数据
		*/
		int pWidth=0;
		int pHeight=0;
			    
		if(!DoColorChange(pBufferSource, YUV2_Value, width, height))
		{
			printf("can't change the YUYto YUV420");
			exit(1);
		}

		int mPic = width * height; 
		int mSize=2* mPic ;
		memcpy(data_result,YUV2_Value,mSize);

		
		if(mSize>0)
		{
			pDest->SetActualDataLength(mSize);
			pDest->GetPointer(&pBufferDest);//这句语句一定要放在最后，避免指针被修改(陈俊霖注--09-09-12)
			CopyMemory(pBufferDest,data_result,mSize);
		}
		else
		{
			pDest->SetActualDataLength(0);
		}

		//陈俊霖 add 09-09-08
		pSource->GetTime(&pTimeStart,&pTimeEnd);
		pDest->SetTime(&pTimeStart,&pTimeEnd);		
	}
	else if( sub_cam == YUY2 )
	{
		int datalength = pSource->GetActualDataLength();
		pSource->GetPointer(&pBufferSource);
		pDest->GetPointer(&pBufferDest);
		CopyMemory(pBufferDest,pBufferSource,datalength);
		pDest->SetActualDataLength(datalength);
	}
	else
		return E_FAIL;
	
	return NOERROR;
}
HRESULT CTransform::CheckInputType(const CMediaType *mtIn)
 {

	/* 在msdx中连接此Filter时出现问题,mtIn->pbFormat为空指针。因此在此
	 * 把相关代码进行了注释.......commented by zhenHua.sun 2010-04-12
	VIDEOINFOHEADER2     info;
	ZeroMemory(&info, sizeof(VIDEOINFOHEADER));
	info=*((VIDEOINFOHEADER2 *)mtIn->pbFormat);	
	*/
	if(mtIn->majortype!=MEDIATYPE_Video)
	{
		return E_FAIL;
	}
	//支持 RGB24 && YUV 4:2:2 &&HDYC(UYVY,高清)的摄像头和DV摄像机的视频输入
	if(mtIn->subtype==MEDIASUBTYPE_YUY2)
	{
		sub_cam=YUY2;
	}
	/*else if(mtIn->subtype==MEDIASUBTYPE_HDYC)
	{
	sub_cam=HDYC;
	}*/
	else if(mtIn->subtype==MEDIASUBTYPE_RGB24)
	{
		VIDEOINFOHEADER * pVideoInfo=(VIDEOINFOHEADER *)mtIn->pbFormat;
		int Height=pVideoInfo->bmiHeader.biWidth;
		int Width=pVideoInfo->bmiHeader.biHeight;
		//if(m_pRGBInverse!=NULL)
		//{
		//	delete m_pRGBInverse;
		//	m_pRGBInverse=NULL;
		//}
		//m_pRGBInverse=new BYTE[Height*Width*3];//先申请存储旋转后的图像的内
		
		sub_cam=RGB24;
	}
	else
	{
		return E_FAIL;
	}

	return NOERROR;
}

HRESULT CTransform::CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut)
{
	return NOERROR;
}


HRESULT CTransform::Receive( IMediaSample *pSample )
{
	return CTransformFilter::Receive( pSample );
}

HRESULT CTransform::GetMediaType(int iPosition, CMediaType *pMediaType)
{
	// Is the input pin connected

	
	if(m_pInput->IsConnected() == FALSE)
	{
		return E_UNEXPECTED;
	}

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
	CMediaType tempType;
	tempType.InitMediaType();
	tempType=m_pInput->CurrentMediaType();
	VIDEOINFOHEADER * preInfo=(VIDEOINFOHEADER *)tempType.pbFormat;


	CheckPointer(pMediaType,E_POINTER);	
	HRESULT hr = m_pInput->ConnectionMediaType(pMediaType);
//	ASSERT(pMediaType->formattype == FORMAT_VideoInfo);
	pMediaType->SetSubtype(&MEDIASUBTYPE_YUY2);
	VIDEOINFOHEADER *pVih = reinterpret_cast <VIDEOINFOHEADER*> (pMediaType->pbFormat); 
	pVih->bmiHeader.biWidth = preInfo->bmiHeader.biWidth;
	pVih->bmiHeader.biHeight = preInfo->bmiHeader.biHeight;
	pVih->bmiHeader.biCompression = MAKEFOURCC('Y','U','V','2');  
//	pVih->bmiHeader.biBitCount = 16;

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
	pProperties->cBuffers   =   23;  //IntelMediaSDK h264 Filter对buffer数量有要求,否则无法连接 
	pProperties->cbAlign     =   1;  
	//   Get   input   pin's   allocator   size   and   use   that   
	ALLOCATOR_PROPERTIES   InProps;   
	IMemAllocator   *   pInAlloc   =   NULL;   

	hr   =   m_pInput->GetAllocator(&pInAlloc);   
	if(SUCCEEDED(hr))   
	{   
		hr   =   pInAlloc->GetProperties(&InProps);   
		if(SUCCEEDED(hr))   
		{   
			   pProperties->cbBuffer   =   InProps.cbBuffer;   //   改一下！   
			//pProperties->cbBuffer   =   48000   *   4;   //   改成很大的值试一下！（假设你要转换到48k）   
		}   
		pInAlloc->Release();   
	}   

	if(FAILED(hr))   
		return   hr;   

	//ASSERT(pProperties->cbBuffer);   

	//   Ask   the   allocator   to   reserve   us   some   sample   memory,   NOTE   the   function   
	//   can   succeed   (that   is   return   NOERROR)   but   still   not   have   allocated   the   
	//   memory   that   we   requested,   so   we   must   check   we   got   whatever   we   wanted   

	ALLOCATOR_PROPERTIES   Actual;   
	hr   =   pAlloc->SetProperties(pProperties,&Actual);   
	if(FAILED(hr))   
	{   
		return   hr;   
	}   

//	ASSERT(Actual.cBuffers   ==   1);   

	if(pProperties->cBuffers   >   Actual.cBuffers   ||   
		pProperties->cbBuffer   >   Actual.cbBuffer)   
	{   
		return   E_FAIL;   
	}   

	return   NOERROR;   
	
}


//end
// --- ISpecifyPropertyPages ---
//STDMETHODIMP CTransform::GetPages(CAUUID *pPages)
//{
//	return NOERROR;
//}
//
//// Basic COM - used here to reveal our own interfaces
//STDMETHODIMP CTransform::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
//{
//	CheckPointer(ppv, E_POINTER);
//
//	if (riid == IID_ISpecifyPropertyPages) 
//	{
//		return GetInterface((ISpecifyPropertyPages *) this, ppv);
//	}
//	else if (riid == IID_ISCUTVideoEncoder)
//	{
//		return GetInterface((SCUTVideoEncoder *) this, ppv);
//	}
//	else 
//	{
//		return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
//	}
//} // NonDelegatingQueryInterface

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

