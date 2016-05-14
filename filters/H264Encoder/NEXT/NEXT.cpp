// NEXT.cpp : 定义 DLL 应用程序的入口点。
//

#pragma warning(disable:4995)

#include "stdafx.h"
#include "NEXT.h"
#include "Dshow.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "rgb2yuv420.h"
#include "ConfigMgr.h"
#include "Log.h"
#include "Util.h"

//struct x264_t;    //还没有解决，是否是调用Lib中的定义？？陈俊霖09-08-21//放假回来先改这个地方---------------------重要

#define DATA_MAX 3000000
uint8_t data[DATA_MAX];

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
//			0x34363248, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);


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
	&CLSID_SCUTX264Encoder,        // Filter CLSID
		L"SCUT H264 Encoder",      // Filter name
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
		, &CLSID_SCUTX264Encoder
		, CTransform::CreateInstance
		, NULL
		, &sudContrast }

};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


CTransform::CTransform(TCHAR *tszName,LPUNKNOWN punk,HRESULT *phr) :
CTransformFilter(tszName, punk, CLSID_SCUTX264Encoder), pBufferDest(NULL)
{
	ASSERT(tszName);
	ASSERT(phr);
	
	isInitEncoder=false;

	Y_pix_Value=NULL;
	U_pix_Value=NULL;
	V_pix_Value=NULL;

	sub_cam=YUY2;//设置默认的媒体输入为YUY2的颜色空间  陈俊霖  09-12-11

		//f_nal=fopen("g:\\nalrecord.nal","rb+");

    data_result=new BYTE[1228800];// 09-09-11 add

	this->m_isWorking = true;		//初始化工作状态为启动  zhenhua.sun 2010-08-06

	//m_pRGBInverse=NULL;
	//初始化临界区 陈俊霖 2011-5-20
	InitializeCriticalSection(&cs_log);
	InitializeCriticalSection(&cs_buffer);
	InitializeCriticalSection(&cs_h264);

	m_quanti = 32;	//默认量化值为32
	m_destWidth = 0;
	m_destHeight = 0;

	hX264 = 0;

	m_dwLastReceivedTime = 0;
	m_rtLastSampleEnd = 0;
	//m_rtSampleStreamDelta = 0;
	m_lEncodedFrame = 0;

	m_hEncoderThread = NULL;
	m_isRunning = false;

	m_nID = getEncID();

	m_pColorChangeSrc = NULL;
	
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
	DestroyEncoder();

    delete [] data_result;// 09-09-11 add

	/*if(m_pRGBInverse)
		delete m_pRGBInverse;
	m_pRGBInverse=NULL;*/

	DeleteCriticalSection(&cs_log);
	DeleteCriticalSection(&cs_buffer);
	DeleteCriticalSection(&cs_h264);
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
BOOL CTransform::DoColorChange(BYTE* pYUV, BYTE* pY, BYTE* pU, BYTE* pV, LONG lWidth, LONG lHeight, LONG lDestWidth, LONG lDestHeight)
{
	PixelFormat nSrcPixFmt = PIX_FMT_NONE;
	int nBytesPerPix = 0;
	
	if(sub_cam==YUY2) 
	{
		nSrcPixFmt = PIX_FMT_YUYV422;
		nBytesPerPix = 2;
		//return YUV422To420(pYUV, pY, pU, pV, lWidth, lHeight);
	}
	else if(sub_cam==RGB24)
	{
		nSrcPixFmt = PIX_FMT_BGR24;
	    nBytesPerPix = 3;
	}
	/*else if(sub_cam==HDYC)
	{
	return HDYC2YUV420P(pYUV, pY, pU, pV, lWidth, lHeight);
	}*/
	else
	{
		return FALSE;
	}

	AVPicture   srcPic;
	AVPicture   destPic;
	memset(&srcPic, 0,sizeof(AVPicture)); 
	memset(&destPic,0,sizeof(AVPicture)); 

	//处理图像翻转。Windows中，RGB24实际是Libav的BGR24格式，且与YUYV422(YUY2)垂直翻转
	if (PIX_FMT_BGR24 == nSrcPixFmt)
	{
		srcPic.data[0] = m_pColorChangeSrc;
		for (int l = 0; l < lHeight; ++l)
		{
			memcpy(srcPic.data[0] + l * lWidth * nBytesPerPix, 
				pYUV + lWidth * (lHeight - l - 1) * nBytesPerPix, 
				lWidth * nBytesPerPix);
		}
		srcPic.linesize[0] = nBytesPerPix * lWidth;
		// 用以上代码代替下面两行代码，下面的代码看似高效，但会引发问题
		// 某些开发者提倡使用以下方法，但查阅swscale源码后发现，其并不支持这种用法，将会导致内存访问越界
		//srcPic.data[0] = pYUV + lWidth * (lHeight-1) * nBytesPerPix;  // 指向图像数据最后一行起始
		//srcPic.linesize[0] = -nBytesPerPix * lWidth; // 倒序往前扫描
	} 
	else
	{
		srcPic.data[0] = pYUV;
		srcPic.linesize[0] = nBytesPerPix * lWidth;
	}

	destPic.data[0]=pY;
	destPic.data[1]=pU;
	destPic.data[2]=pV;

		
	destPic.linesize[0]=lDestWidth;
	destPic.linesize[1]=lDestWidth>>1;
	destPic.linesize[2]=lDestWidth>>1;

	SwsContext		*pYuyToBgraCtx;

	//Initial sws context
	pYuyToBgraCtx = sws_getContext(
			lWidth,				//srcW
			lHeight,			//srcH
			nSrcPixFmt,		    //srcFormat
			lDestWidth,	        //dstW
			lDestHeight,		//dstH
			PIX_FMT_YUV420P,	//dstFormat
			/*SWS_BICUBIC*/SWS_FAST_BILINEAR, NULL, NULL, NULL);

	if (pYuyToBgraCtx)
	{
		//LOG_PRINTF("src_pic=0x%08x\n", pYUV + lWidth * (lHeight-1) * nBytesPerPix);
		sws_scale( pYuyToBgraCtx , srcPic.data , srcPic.linesize , 0, lHeight, 
			destPic.data , destPic.linesize );

		sws_freeContext(pYuyToBgraCtx);
	}

	return TRUE;

}



//
/**********************************************************************************************/
HRESULT CTransform::Transform(IMediaSample *pSource, IMediaSample *pDest)
{
	CheckPointer(pSource,E_POINTER);
	CheckPointer(pDest,E_POINTER);

    //陈俊霖 add 2009-09-11
	BYTE * pBufferSource = NULL;
	int len = 0;

	REFERENCE_TIME pTimeStart;
	REFERENCE_TIME pTimeEnd;
		    
	pSource->GetPointer(&pBufferSource);
	len=pSource->GetActualDataLength();
	

	//if  X264_CSP_BGR
	//memset( &pic, 0, sizeof( x264_picture_t ) );

	x264_picture_t pic;
	x264_picture_t pic_out;
	x264_picture_alloc( &pic, X264_CSP_I420, param.i_width, param.i_height );//陈俊霖add 09-08-21

	
	/*将pBufferSource指向的640*480的YUY2的数据转换成YV12-YUV420的数据存进pic.img.plane中，
	其中plane[0]存放的是Y的数据;plane[1]存放的是U的数据;plane[2]存放的是V的数据
	*/
	if(!DoColorChange(pBufferSource,
		pic.img.plane[0], pic.img.plane[1], pic.img.plane[2],
		m_srcWidth, m_srcHeight, param.i_width, param.i_height))
	{
		printf("can't change the YUYto YUV420");
		exit(1);
	}
    
	/*memcpy(pic.img.plane[0],Y_pix_Value,param.i_width*param.i_height);
	memcpy(pic.img.plane[1],U_pix_Value,param.i_width*param.i_height/4);
	memcpy(pic.img.plane[2],V_pix_Value,param.i_width*param.i_height/4);*/

	pic.img.i_csp=colorSpace;

	//陈俊霖add 09-09-07
	pic.i_type = X264_TYPE_AUTO;
	pic.i_qpplus1 = 0;
	param.i_frame_total++;
	pic.i_pts=(int64_t)param.i_frame_total * param.i_fps_den;


	int        i_nal;
	x264_nal_t *nal;
	/* encode it */
	//x264_encoder_encode( hX264, &nal, &i_nal, &pic, &pic );
	/*陈俊霖09-08-20改为下面代码，添加异常处理*/
	 //使用临界区对编码器的状态进行管理，以便于进行编码器的更新 陈俊霖 2011-5-20
	int nEncRet = 0;
	EnterCriticalSection(&cs_h264);
	if (hX264)
	{
		nEncRet = x264_encoder_encode( hX264, &nal, &i_nal, &pic, &pic_out );
	}
	LeaveCriticalSection(&cs_h264);

	if(nEncRet < 0)  /*09-08-20出现异常*/
	{
		EnterCriticalSection(&cs_log);
		LOG_ENC_WARN("x264 [error]: x264_encoder_encode failed." );
		LeaveCriticalSection(&cs_log);
	}
	else if (nEncRet == 0)
	{
		EnterCriticalSection(&cs_log);
		LOG_ENC_WARN("x264_encoder_encode returns nothing, frame may be buffered.");
		LeaveCriticalSection(&cs_log);
	}

	mux_buffer_size=0;
	pBfOutSize=0;
	for( int i = 0; i < i_nal; i++ )
	{			
		pBfOutSize+=nal[i].i_payload;
	}

	int pTemp=0;
	for( int i = 0; i < i_nal; i++ )
	{
		memcpy(data_result+pTemp,nal[i].p_payload,nal[i].i_payload);
		pTemp+=nal[i].i_payload;
	}
	
	if(pTemp>0)
	{
		pDest->SetActualDataLength(pTemp);
		pDest->GetPointer(&pBufferDest);//这句语句一定要放在最后，避免指针被修改(陈俊霖注--09-09-12)
		CopyMemory(pBufferDest,data_result,pTemp);
	
	}
	else
	{
		pDest->SetActualDataLength(0);
		/*return E_FAIL;*/
	}

	//陈俊霖 add 09-09-08
	// x264_picture_clean( &pic );
		//x264_encoder_close( hX264 );
	pSource->GetTime(&pTimeStart,&pTimeEnd);
	pDest->SetTime(&pTimeStart,&pTimeEnd);

	// Liaokz, 2012-03-29
	// 设置关键帧信息，用于录制MP4文件时生成stss信息，对于FlashPlayer随机访问来说非常重要
	if (IS_X264_TYPE_I(pic_out.i_type))
	{
		pDest->SetSyncPoint(TRUE);
	}
	else
	{
		pDest->SetSyncPoint(FALSE);
	}
	
	x264_picture_clean( &pic ); // 
	

///////////////////////////////////////////		

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
	if(mtIn->subtype!=MEDIASUBTYPE_RGB24 && mtIn->subtype!=MEDIASUBTYPE_YUY2 && mtIn->subtype!=MEDIASUBTYPE_HDYC)
	{
		return E_FAIL;//2009-8-18陈俊霖
	}
	if(mtIn->subtype==MEDIASUBTYPE_YUY2)
	{
		sub_cam=YUY2;
	}
	else if(mtIn->subtype==MEDIASUBTYPE_HDYC)
	{
		sub_cam=HDYC;
	}
	else if(mtIn->subtype==MEDIASUBTYPE_RGB24)
	{
		/*if(m_pRGBInverse!=NULL)
		{
			delete m_pRGBInverse;
			m_pRGBInverse=NULL;
		}*/
		VIDEOINFOHEADER * pVideoInfo=(VIDEOINFOHEADER *)mtIn->pbFormat;
		int Height=pVideoInfo->bmiHeader.biWidth;
		int Width=pVideoInfo->bmiHeader.biHeight;
		//m_pRGBInverse=new BYTE[Height*Width*3];//先申请存储旋转后的图像的内
		
		sub_cam=RGB24;
	}

	return NOERROR;

}
HRESULT CTransform::CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut)
{
	if (mtIn->majortype != MEDIATYPE_Video || !(mtIn->subtype == MEDIASUBTYPE_RGB24 || mtIn->subtype == MEDIASUBTYPE_YUY2))
	{
		return E_FAIL;
	}

	if (mtOut->majortype != MEDIATYPE_Video || mtOut->subtype != MEDIASUBTYPE_H264)
	{
		return E_FAIL;
	}
	return NOERROR;
}


HRESULT CTransform::Receive( IMediaSample *pSample )
{
	if( !this->m_isWorking )
	{
		return NOERROR;
	}

	// 检测上游推送延迟 -- 由于推送延迟是正常现象，故不再记录. By Liaokz, 2012-09
	//DWORD dwCurTime = timeGetTime();
	//if (0 != m_dwLastReceivedTime && dwCurTime - m_dwLastReceivedTime > 100) // 帧频不可能低于10fps.如果曾停止发送视频帧,则可忽略
	//{
	//	EnterCriticalSection(&cs_log);
	//	LOG_ENC_WARN("Too long since last frame arrived(interval=%d). Camera goes wrong.", dwCurTime - m_dwLastReceivedTime);
	//	LeaveCriticalSection(&cs_log);
	//}
	//m_dwLastReceivedTime = dwCurTime;


	/* 以下代码从 DirectShow SDK 的 BaseClass 项目的 transfrm.cpp 中复制并加以修改 */
	/*  Check for other streams and pass them on */
	AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
	if (pProps->dwStreamId != AM_STREAM_MEDIA) {
		return NOERROR;
	}

	++m_lEncodedFrame;

	//修正时间戳 --By Liaokz，2012-09
	REFERENCE_TIME rtStart;
	REFERENCE_TIME rtEnd;
	pSample->GetTime(&rtStart, &rtEnd);

	// 检测流的不连续性 -- 由于留不连续是正常现象，故不再记录. By Liaokz, 2012-09
	//if (0 != m_rtLastSampleTime && (rtStart - m_rtLastSampleTime > 1000000 || rtStart - m_rtLastSampleTime < 0)) // 两帧间隔不正确.如果曾停止发送视频帧,则可忽略
	//{
	//	EnterCriticalSection(&cs_log);
	//	LOG_ENC_WARN("Sample discontinue(jitter=%I64d).", rtStart - m_rtLastSampleTime);
	//	LeaveCriticalSection(&cs_log);
	//}
	//m_rtLastSampleTime = rtStart;

	FILTER_STATE state;
	CRefTime rfStream;
	// 只有FIlter处于Running状态且Graph具有有效时钟(StreamTime返回S_OK)才能正确获得正确流时间
	if (GetState(100, &state) == S_OK && State_Running == state && S_OK == StreamTime(rfStream)) // GetState延迟100，防止长时间阻塞
	{
		//REFERENCE_TIME rtDelta = rfStream.GetUnits() - rtStart;

		if (1 == m_lEncodedFrame)
		{
			//m_rtSampleStreamDelta = rtDelta;
			EnterCriticalSection(&cs_log);
			LOG_ENC_INFO("First sample received in running state(ts=%I64d, streamTime=%I64d, delta=%I64d).", rtStart, rfStream.GetUnits(), rtStart - rfStream.GetUnits());
			LeaveCriticalSection(&cs_log);
		}

		// 检查时间戳偏移
		/*if (m_rtSampleStreamDelta - rtDelta > 500000 || m_rtSampleStreamDelta - rtDelta < -500000)
		{
			EnterCriticalSection(&cs_log);
			LOG_ENC_WARN("Sample timestamp drifts.(ts=%I64d, delta=%I64d).", rtStart, rtStart - rfStream.GetUnits());
			LeaveCriticalSection(&cs_log);
			m_rtSampleStreamDelta = rtDelta;
		}*/

		// 重打时间戳
		REFERENCE_TIME rtBase = rfStream.GetUnits() - 50000 > m_rtLastSampleEnd ? rfStream.GetUnits() - 50000 : m_rtLastSampleEnd;
		rtEnd = rtBase + (rtEnd - rtStart);
		rtStart = rtBase;
	}
	else
	{
		LOG_WARN("Sample[%d] received before graph transit to running state(ts=%I64d).", m_lEncodedFrame, rtStart);
		rtEnd = m_rtLastSampleEnd + (rtEnd - rtStart);
		rtStart = m_rtLastSampleEnd;
	}

	m_rtLastSampleEnd = rtEnd;
	pSample->SetTime(&rtStart, &rtEnd);
	

	/*BYTE * pSrc = NULL;
	const LONG lSrcLength = pSample->GetActualDataLength();
	pSample->GetPointer(&pSrc);

	CSampleCache * pSampleCache = new CSampleCache(pSrc, lSrcLength, rtStart, rtEnd);*/

	EnterCriticalSection(&cs_buffer);
	m_sampleBuffer.push(pSample);

	if (m_sampleBuffer.size() > MAX_BUFFER_SIZE)
	{
		EnterCriticalSection(&cs_log);
		LOG_ENC_WARN("Too many samples in buffer(size=%d).", m_sampleBuffer.size());
		LeaveCriticalSection(&cs_log);

		while (m_sampleBuffer.size() > MAX_BUFFER_SIZE)
		{
			//m_sampleBuffer.front()->Release();
			m_sampleBuffer.pop();
		}
	}

	LeaveCriticalSection(&cs_buffer);

	return NOERROR;
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

	CheckPointer(pMediaType,E_POINTER);	
	
	
	*pMediaType = m_pInput->CurrentMediaType();	
	pMediaType->SetType(&MEDIATYPE_Video);	
	pMediaType->SetSubtype(&MEDIASUBTYPE_H264);
	pMediaType->SetFormatType(&FORMAT_VideoInfo2);

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

	const int nBuffer = MAX_BUFFER_SIZE + 3; //额外用于：编码时占用一个，往下游推送可能共用Allocator占用一个，还有一个用于本地回显 Liaokz, 2012-9-4

	HRESULT hr = NOERROR;
	pProperties->cBuffers   =   nBuffer;   
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
			   pProperties->cbBuffer   =   InProps.cbBuffer*2;   //   改一下！   
			//pProperties->cbBuffer   =   48000   *   4;   //   改成很大的值试一下！（假设你要转换到48k）   
		}   
		pInAlloc->Release();   
	}   

	if(FAILED(hr))   
		return   hr;   

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

	ASSERT(Actual.cBuffers   >=   nBuffer);   

	if(pProperties->cBuffers   >   Actual.cBuffers   ||   
		pProperties->cbBuffer   >   Actual.cbBuffer)   
	{   
		return   E_FAIL;   
	}   

	return   NOERROR;   
	
}


//编码器控制接口......................zhenHua.sun 2010-08-06

void STDMETHODCALLTYPE CTransform::setQuantizer( int quan )
{
	this->m_quanti = quan;

	//以下代码原来用于动态调整H264视频的质量，后来测试的时候有问题，
	//所以不再使用
	//int qp=this->param.rc.i_qp_constant;
	//EnterCriticalSection(&cs_h264);
	//this->param.rc.i_qp_constant=quan;
	//if( ( hX264 = x264_encoder_open( &param ) ) == NULL )
	//{
	//	ASSERT(hX264);
	//	this->param.rc.i_qp_constant=qp;
	//	LeaveCriticalSection(&cs_h264);		
	//}
	//LeaveCriticalSection(&cs_h264);
	

}
int STDMETHODCALLTYPE CTransform::getQuantizer()
{
	return this->m_quanti;
	//return this->param.rc.i_qp_constant;
}

void STDMETHODCALLTYPE CTransform::QuantizerPlusOne()
{
	int qp;
	qp=this->getQuantizer();
	if(qp<51)this->setQuantizer(qp+1);

}
void STDMETHODCALLTYPE CTransform::QuantizerMinerOne()
{
	int qp;
	qp=this->getQuantizer();
	if(qp>1)this->setQuantizer(qp-1);

}


void STDMETHODCALLTYPE CTransform::setEncoderState( bool work )
{
	this->m_isWorking = work;
}
bool STDMETHODCALLTYPE CTransform::getEncoderState()
{
	return this->m_isWorking;
}

//end
// --- ISpecifyPropertyPages ---
STDMETHODIMP CTransform::GetPages(CAUUID *pPages)
{
	return NOERROR;
}

// Basic COM - used here to reveal our own interfaces
STDMETHODIMP CTransform::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	CheckPointer(ppv, E_POINTER);

	if (riid == IID_ISpecifyPropertyPages) 
	{
		return GetInterface((ISpecifyPropertyPages *) this, ppv);
	}
	else if (riid == IID_ISCUTVideoEncoder)
	{
		return GetInterface((ISCUTVideoEncoder *) this, ppv);
	}
	else 
	{
		return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
	}
} // NonDelegatingQueryInterface


int CTransform::InitEncoder()
{
	EnterCriticalSection(&cs_h264);

	DestroyEncoder();	

	CMediaType myMediaType;
	myMediaType.InitMediaType();
	myMediaType=m_pInput->CurrentMediaType();

	if (!myMediaType.IsValid())
	{
		return -1;
	}

	x264_param_default_preset(&param, "ultrafast", "zerolatency");
	//x264_param_default( &param );

	///////////////////////////////////////////////陈俊霖09-08-20添加
	VIDEOINFOHEADER * pVideoInfo=(VIDEOINFOHEADER *)myMediaType.pbFormat;
	m_srcWidth = pVideoInfo->bmiHeader.biWidth;
	m_srcHeight = pVideoInfo->bmiHeader.biHeight;
	param.i_width  = m_destWidth <= 0 ? m_srcWidth : m_destWidth;
	param.i_height = m_destHeight <= 0 ? m_srcHeight : m_destHeight;

	//CConfigMgr::GetValue(CConfigMgr::CFG_X264_FPS, CConfigMgr::TYPE_INT, &param.i_fps_num);
	param.i_fps_num = 1e7/pVideoInfo->AvgTimePerFrame;
	if (30 < param.i_fps_num)
	{
		param.i_fps_num = 30;
	}

	param.i_fps_den=1;
	param.i_keyint_max = param.i_fps_num / param.i_fps_den * 2; // 最多每2s一个关键帧
	param.i_frame_reference = 1;//设置参考图像帧为1
	param.i_scenecut_threshold = 0; // 减少关键帧，降低码率。值1-100,0时每i_keyint_max一个关键帧

	param.b_sliced_threads = 1; // 启用基于片的多线程模式，极大的减少延迟，应用zerolatency后已启用
	param.i_threads = 4;

	//param.b_deblocking_filter = 1;	//可减少噪点，使画面柔和
	//param.i_deblocking_filter_alphac0 = 1;
	//param.i_deblocking_filter_beta = 0;
	CConfigMgr::GetValue(CConfigMgr::CFG_X264_DEBLOCK, CConfigMgr::TYPE_INT, &param.b_deblocking_filter);
	CConfigMgr::GetValue(CConfigMgr::CFG_X264_DEBLOCK_ALPHA, CConfigMgr::TYPE_INT, &param.i_deblocking_filter_alphac0);
	CConfigMgr::GetValue(CConfigMgr::CFG_X264_DEBLOCK_BETA, CConfigMgr::TYPE_INT, &param.i_deblocking_filter_beta);

	param.b_cabac = 1; // 轻微的增加压缩时间，带来10-20%的压缩率提高
	param.i_frame_total=0;
	param.i_log_level = X264_LOG_NONE;

	param.vui.i_overscan = 5;
	param.vui.i_vidformat = 0;
	param.vui.b_fullrange = 2;
	param.vui.i_colmatrix = 0;
	param.vui.i_chroma_loc = 25;

	///////////////////////////////////////////////

	if(myMediaType.subtype==MEDIASUBTYPE_RGB24) {
		colorSpace=X264_CSP_I420;
		//colorSpace=X264_CSP_RGB;
		byteNumber=3;
	}
	else if(myMediaType.subtype==MEDIASUBTYPE_YUY2){
		colorSpace=X264_CSP_I420;
		byteNumber=2;
	}
	else if(myMediaType.subtype==MEDIASUBTYPE_HDYC)
	{
		colorSpace=X264_CSP_I420;
		byteNumber=2;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//CQP - 恒定量化参数；CRF - 恒定位率因子
	CConfigMgr::GetValue(CConfigMgr::CFG_X264_RC_METHOD, CConfigMgr::TYPE_INT, &param.rc.i_rc_method);

	switch (param.rc.i_rc_method)
	{
	case X264_RC_CQP:
		CConfigMgr::GetValue(CConfigMgr::CFG_X264_RC_CQP, CConfigMgr::TYPE_INT, &param.rc.i_qp_constant);
		break;

	case X264_RC_CRF:
		CConfigMgr::GetValue(CConfigMgr::CFG_X264_RC_CRF, CConfigMgr::TYPE_FLOAT, &param.rc.f_rf_constant);
		break;

	default:
		return E_FAIL;
	}

	//param.rc.i_aq_mode=1;	// 自适应量化模，可有效降低码率
	CConfigMgr::GetValue(CConfigMgr::CFG_X264_RC_AQ, CConfigMgr::TYPE_INT, &param.rc.i_aq_mode);


	////	预测模式设置
	param.analyse.i_subpel_refine = 5; // 画面细节调整方法，值1-9，数值越大，画面越好，码率可能会降低，编码速度越慢
	param.analyse.f_psy_rd = 0; // 当i_subpel_refine >= 6 时有效。数值越小，码率越小。默认值1.0，码率大幅提高，画质提升不明显
	param.analyse.f_psy_trellis = 0;

	param.analyse.intra=X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;// 帧内预测模式
	param.analyse.inter=X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
		| X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16;            //zhenHua.sun 2010-06-01
	param.analyse.i_me_method=X264_ME_DIA;//运动补偿的算法选择
	param.analyse.i_me_range = 16;
	param.analyse.b_chroma_me = 0;
	param.analyse.b_mixed_references = 1;
	param.analyse.b_fast_pskip = 0;
	param.analyse.b_dct_decimate = 0;
	param.analyse.b_psnr=0;//是否使用信噪比
	param.analyse.b_ssim = -1;

	////为了让想x264采用基本档（baseline)进行编码，这个值得设为0，参照x264 set.c line81-89
	////....comment by zhenHua.sun 2010-05-23
	param.analyse.b_transform_8x8 = 0;
	param.analyse.i_weighted_pred = 0;
	param.analyse.i_noise_reduction = 1;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	if( ( hX264 = x264_encoder_open( &param ) ) == NULL )
	{
		return -1;
	}

	/*Y_pix_Value=new BYTE[param.i_width*param.i_height];
	U_pix_Value=new BYTE[param.i_width*param.i_height/4];
	V_pix_Value=new BYTE[param.i_width*param.i_height/4];*/

	m_pColorChangeSrc = new BYTE[m_srcWidth * m_srcHeight * 3]; // big enough for RGB24
	
	isInitEncoder=true;

	//const int nMaxBufferedFrame = x264_encoder_maximum_delayed_frames(hX264);

	LOG_ENC_INFO("Init(%dx%d@%d -> %dx%d@%d, qp=%f)"
		, m_srcWidth, m_srcHeight, param.i_fps_num
		, param.i_width, param.i_height, param.i_fps_num
		, param.rc.f_rf_constant);

	LeaveCriticalSection(&cs_h264);

	return 0;

}

int CTransform::DestroyEncoder()
{
	EnterCriticalSection(&cs_h264);

	if (hX264)
	{
		x264_encoder_close(hX264);
		hX264 = NULL;
		isInitEncoder = false;
	}

	/*if(Y_pix_Value){delete [] Y_pix_Value; Y_pix_Value = NULL;}
	if(U_pix_Value){delete [] U_pix_Value; U_pix_Value = NULL;}
	if(V_pix_Value){delete [] V_pix_Value; V_pix_Value = NULL;}*/

	if (m_pColorChangeSrc)
	{
		delete [] m_pColorChangeSrc;
		m_pColorChangeSrc = NULL;
	}

	LeaveCriticalSection(&cs_h264);

	return 0;

}

STDMETHODIMP CTransform::Run( REFERENCE_TIME tStart )
{
	// Init encoder
	if(isInitEncoder==false && InitEncoder() != 0)
	{
		return E_FAIL;
	}

	m_dwLastReceivedTime = 0;
	m_rtLastSampleEnd = 0;
	//m_rtSampleStreamDelta = 0;
	m_lEncodedFrame = 0;

	// Start thread
	if (NULL != m_hEncoderThread)
	{
		return E_FAIL;
	}

	m_isRunning = true;
	m_hEncoderThread = (HANDLE)_beginthreadex(0, 0, Encode, this, 0, 0);

	return CTransformFilter::Run(tStart);
}

STDMETHODIMP CTransform::Stop()
{
	// Stop encoder thread
	m_isRunning = false;

	if (m_hEncoderThread)
	{
		WaitForSingleObject(m_hEncoderThread, 100);
		DWORD dwExitCode = 0;
		GetExitCodeThread(m_hEncoderThread, &dwExitCode);
		if (STILL_ACTIVE == dwExitCode)
		{
			TerminateThread(m_hEncoderThread, 0);
		}
		m_hEncoderThread = NULL;
	}

	// Destroy encoder
	DestroyEncoder();

	// Clear buffer
	while (m_sampleBuffer.size() > 0)
	{
		//m_sampleBuffer.front()->Release();
		m_sampleBuffer.pop();
	}
	if (m_pColorChangeSrc)
	{
		delete [] m_pColorChangeSrc;
		m_pColorChangeSrc = NULL;
	}

	return CTransformFilter::Stop();
}

UINT _stdcall CTransform::Encode( void* pvParam )
{
	CTransform * pObj = (CTransform *)pvParam;
	
	while(pObj->m_isRunning)
	{
		HRESULT hr = pObj->Deliver();
		if (S_FALSE == hr)
		{
			Sleep(10);
		}
		else
		{
			Sleep(1);
		}
	}	

	return 0;
}

HRESULT CTransform::Deliver()
{
	HRESULT hr;

	/* 以下代码从 DirectShow SDK 的 BaseClass 项目的 transfrm.cpp 中复制并加以修改 */
	// If no output to deliver to then no point sending us data

	ASSERT (m_pOutput != NULL) ;

	EnterCriticalSection(&cs_buffer);
	if (m_sampleBuffer.empty())
	{
		LeaveCriticalSection(&cs_buffer);
		return S_FALSE;
	}

	CComPtr<IMediaSample> pSample = m_sampleBuffer.front();
	m_sampleBuffer.pop();
	LeaveCriticalSection(&cs_buffer);

	// Set up the output sample
	DWORD dwPreTime = timeGetTime();
	IMediaSample * pOutSample = NULL;
	hr = InitializeOutputSample(pSample, &pOutSample);

	// Check time consume
	DWORD dwCurTime = timeGetTime();
	if (dwCurTime - dwPreTime > 100)
	{
		EnterCriticalSection(&cs_log);
		LOG_ENC_WARN("Getting sample cost too long(interval=%d).", dwCurTime - dwPreTime);
		LeaveCriticalSection(&cs_log);
	}

	if (FAILED(hr)) {
		return hr;
	}

	dwPreTime = dwCurTime;

	hr = Transform(pSample, pOutSample);

	// Check time consume
	dwCurTime = timeGetTime();
	if (dwCurTime - dwPreTime >= 100)
	{
		EnterCriticalSection(&cs_log);
		LOG_ENC_WARN("Encoding cost too long(interval=%d). Encoder goes wrong.", dwCurTime - dwPreTime);
		LeaveCriticalSection(&cs_log);
		if (0 != InitEncoder())
		{
			isInitEncoder = false; // Filter will stop when next sample arrives
		}
	}

	if (FAILED(hr)) {
		EnterCriticalSection(&cs_log);
		LOG_ENC_ERROR("Transform returns error(hr=0x%08x).", hr);
		LeaveCriticalSection(&cs_log);
	} else {
		// the Transform() function can return S_FALSE to indicate that the
		// sample should not be delivered; we only deliver the sample if it's
		// really S_OK (same as NOERROR, of course.)
		if (hr == NOERROR) {
			dwPreTime = dwCurTime;

			hr = m_pOutput->Deliver(pOutSample);
			m_bSampleSkipped = FALSE;	// last thing no longer dropped

			// Check time consume
			dwCurTime = timeGetTime();
			if (dwCurTime - dwPreTime > 100) // 帧频不可能低于5fps
			{
				EnterCriticalSection(&cs_log);
				LOG_ENC_WARN("Deliver cost too long(interval=%d).", dwCurTime - dwPreTime);
				LeaveCriticalSection(&cs_log);
			}

		} else {
			// S_FALSE returned from Transform is a PRIVATE agreement
			// We should return NOERROR from Receive() in this cause because returning S_FALSE
			// from Receive() means that this is the end of the stream and no more data should
			// be sent.
			if (S_FALSE == hr) {

				//  Release the sample before calling notify to avoid
				//  deadlocks if the sample holds a lock on the system
				//  such as DirectDraw buffers do
				pOutSample->Release();

				m_bSampleSkipped = TRUE;
				if (!m_bQualityChanged) {
					NotifyEvent(EC_QUALITY_CHANGE,0,0);
					m_bQualityChanged = TRUE;
				}
				return NOERROR;
			}
		}
	}

	// release the output buffer. If the connected pin still needs it,
	// it will have addrefed it itself.
	pOutSample->Release();

	return NOERROR;
}

void STDMETHODCALLTYPE CTransform::setDestSize( int width, int height )
{
	m_destWidth = width;
	m_destHeight = height;
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

