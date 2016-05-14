// Transform.h : Transform DLL 的主头文件
//

#pragma once
//#include "stdafx.h"
//#ifndef __AFXWIN_H__
//#error include 'stdafx.h' before including this file for PCH
//#endif

#include "resource.h"		// 主符号
#include <streams.h>
#include <stdio.h>
#include <time.h>

#include "stdint.h"
extern "C"
{
#include "x264.h"
#include "x264_config.h"
};

extern "C"{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libavutil\avutil.h>
#include <libswscale\swscale.h>
}


#include <windows.h>
#include <initguid.h>
#include <olectl.h>
#include <atlbase.h>

#include <queue>

#if (1100 > _MSC_VER)
#include <olectlid.h>
#endif



#pragma warning(disable:4238)  // nonstd extension used: class rvalue used as lvalue


// {E2C7D3A5-819A-4881-87BA-F1C93319D18A}
DEFINE_GUID(CLSID_SCUTX264Encoder, 
			0xe2c7d3a5, 0x819a, 0x4881, 0x87, 0xba, 0xf1, 0xc9, 0x33, 0x19, 0xd1, 0x8a);

/****************************************************************************************
功能：定义高清接口的媒体类型
作者：陈俊霖  2009-12-08
*****************************************************************************************
*/
#ifndef OUR_GUID_ENTRY
    #define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#endif
//HDYC{43594448-0000-0010-8000-00AA00389B71}///陈俊霖 add 09-12-08
OUR_GUID_ENTRY(MEDIASUBTYPE_HDYC,
0x43594448, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

//定义一些颜色空间转换相关的    陈俊霖 2009-12-13
#define SCALEBITS 8
#define ONE_HALF  (1 << (SCALEBITS - 1))
#define FIX(x)    ((int) ((x) * (1L<<SCALEBITS) + 0.5))
typedef unsigned char uint8_t;



//编码器控制接口..........zhenHua.sun 2010-08-06

// {A7B65F66-5354-48c6-8E7E-E1D1154B45D1}
DEFINE_GUID(IID_ISCUTVideoEncoder, 
	0xa7b65f66, 0x5354, 0x48c6, 0x8e, 0x7e, 0xe1, 0xd1, 0x15, 0x4b, 0x45, 0xd1);


MIDL_INTERFACE("A7B65F66-5354-48c6-8E7E-E1D1154B45D1")
ISCUTVideoEncoder : public IUnknown
{
public:

	virtual void STDMETHODCALLTYPE setQuantizer( int quan )= 0;
	virtual int  STDMETHODCALLTYPE getQuantizer()= 0;

	/**
	 * @brief 设置编码器的工作状态
	 * @param work true则正常工作，false表示停止工作
	 */
	virtual void STDMETHODCALLTYPE setEncoderState( bool work );
	virtual bool STDMETHODCALLTYPE getEncoderState();

	//设置码率调整 陈俊霖 2011-5-20
	virtual void STDMETHODCALLTYPE QuantizerPlusOne()=0;
	virtual void STDMETHODCALLTYPE QuantizerMinerOne()=0; 

	//设置输出视频分辨率
	virtual void STDMETHODCALLTYPE setDestSize( int width, int height );

};
//end

/****************************************************************************************/

class CTransform : public CTransformFilter, public ISCUTVideoEncoder, public ISpecifyPropertyPages
{

public:

	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	~CTransform();

	// Reveals IContrast & ISpecifyPropertyPages
	//STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	DECLARE_IUNKNOWN;

	//HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
	HRESULT CheckInputType(const CMediaType *mtIn);
	HRESULT CheckTransform(const CMediaType *mtIn,const CMediaType *mtOut);
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	HRESULT DecideBufferSize(IMemAllocator *pAlloc,
		ALLOCATOR_PROPERTIES *pProperties);

	BOOL DoColorChange(BYTE* pYUV, BYTE* pY, BYTE* pU, BYTE* pV, LONG lWidth, LONG lHeight, LONG lDestWidth, LONG lDestHeight);


	//scut encoder interface  .....zhenHua.sun 2010-08-06
	void STDMETHODCALLTYPE setQuantizer( int quan );
	int STDMETHODCALLTYPE getQuantizer();

	//****************************************************//
	//rate-control interface  chenjunlin 2011-5-20
	void STDMETHODCALLTYPE QuantizerPlusOne();
	void STDMETHODCALLTYPE QuantizerMinerOne();

	void STDMETHODCALLTYPE setEncoderState( bool work );
	bool STDMETHODCALLTYPE getEncoderState();

	void STDMETHODCALLTYPE setDestSize( int width, int height );
	//end

	//重载receive函数，用于编码端的控制
	HRESULT Receive( IMediaSample *pSample );

	STDMETHODIMP Run(REFERENCE_TIME tStart);

	STDMETHODIMP Stop();

		// --- ISpecifyPropertyPages ---
	STDMETHODIMP GetPages(CAUUID *pPages);

	// Basic COM - used here to reveal our own interfaces
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);


private:

	int InitEncoder();
	int DestroyEncoder();

	// Constructor
	CTransform(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	// 编码工作线程
	static UINT _stdcall Encode(void* pvParam);
	
	HRESULT Deliver();

	HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);

	int pictureSize;
	x264_t * hX264;
	x264_param_t param;

	int colorSpace;
	int byteNumber;
//	cli_opt_t opt;
	bool isInitEncoder;
	CCritSec m_ContrastLock;

	int mux_buffer_size; //陈俊霖add 09-08-20
	int pBfOutSize;
	//uint8_t *mux_buffer;//陈俊霖add 09-08-20
	BYTE * pBufferDest;

	//陈俊霖add 09-09-03
		BYTE* Y_pix_Value;
		BYTE* U_pix_Value;
		BYTE* V_pix_Value;
	//定义输入摄像头的色彩空间类型 陈俊霖 2009-12-11
	enum sub_mediatype
	{
		YUY2=0,
		HDYC=1,
		RGB24=2
	};
	int sub_cam;


	//分配给下一filter传递的内存空间，在构造函数中分配，在析构函数中释放 --陈俊霖 09-09-11 add
	BYTE * data_result;


	//zhenhua.sun 2010-08-06

	///如果为true则表示正在进行视频编码，否则为false -- 用于决定是否接收帧
	bool m_isWorking;

	//编码器进行编码时的临界区 陈俊霖 2011-5-20
	CRITICAL_SECTION cs_h264;

	///视频量化值。
	UINT m_quanti;

	int m_srcWidth;
	int m_srcHeight;
	int m_destWidth;
	int m_destHeight;


	//dsh 　　2010-11-22　为了减少延迟，将new的次数减少
	//BYTE *m_pRGBInverse;//当视频采集为RGB24时，图像为倒像，因此要先旋转，用于存储旋转后的数据


	//liaokz 2012-07-31
	DWORD m_dwLastReceivedTime;

	REFERENCE_TIME m_rtLastSampleEnd;

	//REFERENCE_TIME m_rtSampleStreamDelta;

	long m_lEncodedFrame;

	// 队列最大容量（防止未输出导致内存耗尽）
	static const int MAX_BUFFER_SIZE = 3;

	typedef std::queue<CComPtr<IMediaSample>> SampleBuffer;
	SampleBuffer m_sampleBuffer;

	HANDLE m_hEncoderThread;

	bool m_isRunning;

	//日志临界区保护 Liaokz 2012-09-04
	CRITICAL_SECTION cs_log;

	CRITICAL_SECTION cs_buffer;

	int m_nID;

	BYTE * m_pColorChangeSrc;


}; // CContrast

