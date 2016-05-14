
#include <streams.h>
#include <dsound.h>
#include "IAECProp.h"
#include <string>

//足够16k音频存1s：16k * 16bits / (8bits/B) = 32kB
#define MAX_RAWBUFFER_SIZE 32000

struct RawBufferStruct
{
	char RawBuffer[MAX_RAWBUFFER_SIZE];//存取从声卡拿到的数据
	int HeadPos;// 数据存储的头索引
	int EndPos;//数据存储的尾索引  
	int nRawBfferSize;//从声卡拿到的暂存数据大小

	RawBufferStruct()
	{
		HeadPos=0;
		EndPos=0;
		nRawBfferSize=0;
		memset(&RawBuffer, 0, MAX_RAWBUFFER_SIZE);
	}
};

class CStaticMediaBuffer : public CBaseMediaBuffer {
public:
	/*STDMETHODIMP_(ULONG) AddRef() {return 2;}
	STDMETHODIMP_(ULONG) Release() {return 1;}*/
    void Init(BYTE *pData, ULONG ulSize, ULONG ulData) {
        m_pData = pData;
        m_ulSize = ulSize;
        m_ulData = ulData;
    }
};

class CAudioAECFilterPin;


class CAudioAECFilter : public CSource
{
public:
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:
    CAudioAECFilter(LPUNKNOWN lpunk, HRESULT *phr);
	~CAudioAECFilter();

}; 


class CAudioAECFilterPin : public CSourceStream,IAMStreamConfig,IAMBufferNegotiation,IAECProperty
{

public:

    CAudioAECFilterPin(HRESULT *phr, CAudioAECFilter *pParent, LPCWSTR pPinName);
    ~CAudioAECFilterPin();

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
    DECLARE_IUNKNOWN;

    HRESULT FillBuffer(IMediaSample *pms);


    HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc,
                             ALLOCATOR_PROPERTIES *pProperties);

    HRESULT SetMediaType(const CMediaType *pMediaType);

    HRESULT CheckMediaType(const CMediaType *pMediaType);
    HRESULT GetMediaType(int iPosition, CMediaType *pmt);

    HRESULT OnThreadCreate(void);
	HRESULT OnThreadDestroy(void);

    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

	//IAMBufferNegotiation
	STDMETHODIMP SuggestAllocatorProperties(const ALLOCATOR_PROPERTIES *pprop);
    STDMETHODIMP GetAllocatorProperties(__out  ALLOCATOR_PROPERTIES *pprop);

	//IAMStreamConfig
	STDMETHODIMP SetFormat( AM_MEDIA_TYPE *pmt);   
	STDMETHODIMP GetFormat(__out  AM_MEDIA_TYPE **ppmt);
	STDMETHODIMP GetNumberOfCapabilities(__out  int *piCount,__out  int *piSize);
	STDMETHODIMP GetStreamCaps(int iIndex, __out  AM_MEDIA_TYPE **ppmt,__out  BYTE *pSCC);

	//IAECProperty
	STDMETHODIMP put_CapDevFriendName(wchar_t* capDevFriendName,int capNameSize,wchar_t* rendDevFriendName,int rendNameSize);
	STDMETHODIMP put_PreviewHandle(HWND hPreviewHandle);

private:

	HRESULT CreateBasicBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8);

private:

	//dsh
	std::wstring m_szCaptureDeviceName;//捕捉设备名称,都是Friendly Name
	std::wstring m_szRenderDeviceName;//捕捉设备名称
	int m_mode;//模式选择，是否回消等
	int m_feat;//是否修改特征，当它为on时，下面几个的设置才会有效
	int m_ns;//是否去噪
	int m_agc;//是否增益
	int m_cntrclip;//是否剪切中部

	IMediaObject* pDMO;//回声对消DMO
	IPropertyStore* pPS;//回声对消DMO属性设置

	RawBufferStruct  m_RawBuffer;

	

	int m_iIntervalTime;//取一帧的休眠时间，跟DecideBufferSize也有关系

	/*FILE * pfMicOutPCM;  // dump output signal using PCM format
	FILE * pfMicSize;

	FILE *pfDeliverPCM;*/
	


    CCritSec m_cSharedState;            // Lock on m_rtSampleTime and m_Ball
    CRefTime m_rtSampleTime;            // The time stamp for each sample

	bool bSetFormat;
	AM_MEDIA_TYPE *m_mediaType;	//use for setformt and getformat

	LPDIRECTSOUND8  m_PlayDev;
	LPDIRECTSOUNDBUFFER8 m_PlayBuffer;
	GUID m_capDevID;
	GUID m_rendDevID;
	HWND m_previewHwnd;

	//FillBuffer
	CStaticMediaBuffer * m_pOutputBuffer;
    DMO_OUTPUT_DATA_BUFFER * m_pOutputBufferStruct;
	BYTE *m_pbOutputBuffer;

	//void recordMessage(char* message);
	//FILE *errorlog;
};
    
