#include "streams.h"
#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

typedef  UINT32 uint_32;
typedef void (*funcPtr)(void *pClass,uint_32 delaytime,uint_32 jitter, uint_32 lossrate,uint_32 ipAddr); //声明函数指针


//RtpWare3 Interfaces
const CLSID IID_ISCUTRtpSender={0xa0904dec, 0x71df, 0x4af4, {0x8b, 0x7d, 0x3f, 0xdb, 0x20, 0x67, 0xfd, 0xb6}};

MIDL_INTERFACE("A0904DEC-71DF-4AF4-8B7D-3FDB2067FDB6")
ISCUTRtpSender : public IUnknown
{
public:

    struct ConnectionParam {
        const char * ip_addr;
        unsigned short port;
        //callback
        void *callback_obj;
        RtpStatCallback callback_func;
        //QoS(reserved)
        unsigned int dscp;
    };

public:
    //Add connection.
    virtual HRESULT AddConnection(const ConnectionParam &param) = 0;

    //Remove the connection specified. Return S_FALSE if not exist.
    //Specially, if the ip is NULL, all connection will be removed.
    virtual HRESULT RemoveConnection(const char *ip, unsigned short port) = 0;

    virtual HRESULT RemoveAllConnection() = 0;

    //Get current connection count. If not return S_OK, the count is indeterminate.
    virtual unsigned long ConnectionCount() = 0;

    //Start or stop sending to the specified ip and port. Returns E_FAIL if connection 
    //not exist. Specially, it controls all connection if ip is NULL.
    virtual HRESULT ControlSending(const char *ip, unsigned short port, bool send) = 0;

    virtual HRESULT ControlAllSending(bool send) = 0;

};

const CLSID IID_ISCUTRtpReceiver={0xbcfa2135, 0x67fa, 0x44ae, {0xb4, 0x90, 0xbc, 0x51, 0x82, 0xc8, 0xae, 0xd0}};
MIDL_INTERFACE("BCFA2135-67FA-44AE-B490-BC5182C8AED0")
ISCUTRtpReceiver : public IUnknown
{
public:
    //Set id
    virtual HRESULT SetId(const char * id) = 0;
    virtual const char *GetId() = 0;

    //Set receiving port
    virtual HRESULT SetLocalAddr(const char *ip, unsigned short port) = 0;
    virtual unsigned int GetSourcePort() = 0;

    //Set callback, use null to clear callback
    virtual HRESULT SetCallback(void *callback_obj, RtpStatCallback callback_func) = 0;

    //Set media type
    virtual HRESULT SetMediaType(const CMediaType &type) = 0;
    virtual HRESULT GetMediaType(CMediaType &type) = 0;

    //Start or Stop receive
    virtual HRESULT ControlRecv(bool recv) = 0;

};


//RTPOut Interface
// {B46A2203-E4D1-4dd4-AB37-157D5B0ABF1E}
const CLSID IID_ISCUTNCRTPOut={0xb46a2203, 0xe4d1, 0x4dd4,{ 0xab, 0x37, 0x15, 0x7d, 0x5b, 0xa, 0xbf, 0x1e}};

MIDL_INTERFACE("B46A2203-E4D1-4dd4-AB37-157D5B0ABF1E")
ISCUTNCRTPOut : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE setMainNet(const char* ipstr,int port,bool isStartRTCP) = 0;
	virtual HRESULT STDMETHODCALLTYPE updateIPList(const char * ipstr,int port)=0;
	virtual HRESULT STDMETHODCALLTYPE setStreamType(int level)=0;

	/*
		by qhf，	新增几个接口方法
	*/
	//获取目标地址
	virtual const char * STDMETHODCALLTYPE getRemoteAddr() = 0;
	//获取目标端口
	virtual  int STDMETHODCALLTYPE getRemotePort() = 0;
	//设置QoS DSCP值
	virtual void STDMETHODCALLTYPE setDSCP(unsigned int dscp)=0;
	//设置RTP统计回调函数
	virtual void STDMETHODCALLTYPE setRtpStatCallback(void *pClass, RtpStatCallback pRtpStatCallbackFunc)=0;

	//停止RTP发送器
	virtual void STDMETHODCALLTYPE stopSender() = 0;

	//启动RTP发送器，返回值0表示启动成功，-1则表示启动失败（例如缺少参数）
	// 正常Filter流程时不需要调用该方法, 该方法供stopSender后重启Sender
	virtual int STDMETHODCALLTYPE startSender() = 0;

	//判断RTP发送器是否在运行，即是否已经启动，正在发送处理数据
	virtual bool STDMETHODCALLTYPE isSenderRunning() =0;

	/*
	 * @brief	 获取码率和当前网络丢包率
	 * @param [in,out]	codeRate	If non-null, the code rate.  单位 Kbps
	 * @param [in,out]	lostRatio	If non-null, the lost ratio.   单位 %
	*/
	virtual void  STDMETHODCALLTYPE getCodeRate_LostRatio(int * codeRate, float * lostRatio) =0;

	virtual void  STDMETHODCALLTYPE startNewEpisode(unsigned int index) =0;

};

// RTPRecv Interface
const CLSID IID_ISCUTNCRTPRecv={ 0xBD1EB878, 0x3EB8, 0x4764,{ 0xa5, 0xbb, 0xe9, 0xe4, 0x3d, 0x96, 0xc1, 0x9f}};

MIDL_INTERFACE("BD1EB878-3EB8-4764-A5BB-E9E43D96C19F")
ISCUTNCRTPRecv : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE setId( const char * id )= 0;
	virtual HRESULT STDMETHODCALLTYPE setAddr( const char * ipstr,/* [in] */ int port )= 0;
	virtual HRESULT STDMETHODCALLTYPE startListen(bool isStartRTCP) = 0;	
	virtual HRESULT STDMETHODCALLTYPE setFormat(const CMediaType * pMediaType)=0;
	virtual void STDMETHODCALLTYPE clearBuf() = 0;

	//获取ID
	virtual const char * STDMETHODCALLTYPE getId()=0;
	//获取监听地址
	virtual const char * STDMETHODCALLTYPE getLocalAddr()=0;
	//获取监听端口
	virtual  int STDMETHODCALLTYPE getLocalPort()=0;
	//设置RTP统计回调函数
	virtual void STDMETHODCALLTYPE setRtpStatCallback(void *pClass, RtpStatCallback pRtpStatCallbackFunc)=0;

	//重新设置同步,包括清空缓冲、重设参考帧等操作
	virtual void STDMETHODCALLTYPE resetSync() = 0;

	//停止RTP接收器
	virtual void STDMETHODCALLTYPE stopReceiver() = 0;

	//启动RTP接收器，返回值0表示启动成功，-1则表示启动失败（例如缺少参数）
	virtual int STDMETHODCALLTYPE startReceiver() = 0;

	//判断RTP接收器是否在运行，即是否已经启动，正在接收处理数据
	virtual bool STDMETHODCALLTYPE isReceiverRunning() =0;
};



#define CODEC_MPG4_MIME_STRING "MPG4"
#define CODEC_H264_MIME_STRING "H264"
#define CODEC_H263_MIME_STRING "H263"
#define CODEC_H263P_MIME_STRING "H263-1998"

const CLSID MEDIASUBTYPE_XVID = {0x44495658,0x0000,0x0010,{0x80,0x00,0x00,0xaa,0x00,0x38,0x9b,0x71}};

const CLSID IID_IMULTISMARTTEE= {0x457321b,0x76ae, 0x42a3,{0xaf, 0x12, 0xd8, 0x5e, 0x8a, 0x13, 0xe7, 0xae}};
MIDL_INTERFACE("0457321B-76AE-42a3-AF12-D85E8A13E7AE")
IMULTISMARTTEE : public IUnknown
{
public:
	virtual CBaseOutputPin * STDMETHODCALLTYPE GetPinNFromList(int n)=0;
	virtual HRESULT STDMETHODCALLTYPE GetFreePin(IPin ** ppIPin)=0;
	virtual HRESULT STDMETHODCALLTYPE IsConnect(int * result)=0;
	virtual HRESULT STDMETHODCALLTYPE StartTransfer(IBaseFilter * pDownstreamFilter)=0;
	virtual HRESULT STDMETHODCALLTYPE StopTransfer(IBaseFilter * pDownstreamFilter)=0;
    virtual HRESULT STDMETHODCALLTYPE SetOverallTransfer(bool transfer)=0;
};



const CLSID IID_ISCUTNCRTCPRECV={0x34192C80, 0xCD67, 0x47f9,{ 0x9C, 0x8A, 0x90, 0x09, 0x04, 0x61, 0x05, 0xBE}};

MIDL_INTERFACE("34192C80-CD67-47f9-9C8A-9009046105BE")
ISCUTNCRTCPRecv : public IUnknown
{
public:

	virtual HRESULT STDMETHODCALLTYPE setAddr(char * ipstr, int inPort ,bool AddIt) = 0;
	virtual void STDMETHODCALLTYPE setWait(UINT32 time) = 0;
	virtual HRESULT STDMETHODCALLTYPE setHWND(HWND m_hWnd) = 0;
	virtual HRESULT STDMETHODCALLTYPE startScan() = 0;
	virtual HRESULT STDMETHODCALLTYPE stopScan() = 0;
	virtual HRESULT STDMETHODCALLTYPE setCallbak(void *pClass,funcPtr pFunc) = 0; //设置回调函数，pClass创建此Filter的类，pRecvInfo收到的RR消息
};

//SCUT_MPEG4_Encoder
// {5A226BA1-E472-4c9a-8769-8894AD1829DB}

const CLSID IID_ISCUTMPEG4Encoder = {0x5a226ba1, 0xe472, 0x4c9a, {0x87, 0x69, 0x88, 0x94, 0xad, 0x18, 0x29, 0xdb}};

MIDL_INTERFACE("5A226BA1-E472-4c9a-8769-8894AD1829DB")
SCUTMPEG4Encoder : public IUnknown
{
public:

	virtual void STDMETHODCALLTYPE setQuantizer( int quan )= 0;
	virtual int STDMETHODCALLTYPE getQuantizer()= 0;
};

// {BDFB8696-9F5F-42d4-964F-DC0855034067}
const CLSID IID_ISCUTNCIMAGESOURCE = {0xbdfb8696, 0x9f5f, 0x42d4, {0x96, 0x4f, 0xdc, 0x8, 0x55, 0x3, 0x40, 0x67}};
MIDL_INTERFACE("BDFB8696-9F5F-42d4-964F-DC0855034067")
ISCUTNCIMAGESOURCE : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE setPosition(int left,int up,int right,int down) = 0;
	virtual HRESULT STDMETHODCALLTYPE setFrameRate(int FramesPerSecond )= 0;
	virtual HRESULT STDMETHODCALLTYPE setJPEGQuality(int lever)=0;
};


//请同SPEEX编解码Filter项目中定义保持一致
//自定义的SPEEX类型GUID，Data1为MAKEFOURCC('S','P','X','R')，SPXR是参照VLC解码器得到的
//其余部分参照MEDIASUBTYPE_PCM
const CLSID MEDIASUBTYPE_SPEEX = {0x52585053, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};

//by dsh 10-07-29
// {08af6540-4f21-11cf-aacb-0020af0b1992}
const CLSID IID_SpeexEncIProp = {0x08af6540, 0x4f21, 0x11cf, {0xaa, 0xcb, 0x00, 0x20, 0xaf, 0x0b, 0x19, 0x92}};
MIDL_INTERFACE("08af6540-4f21-11cf-aacb-0020af0b1992")
SpeexEncIProp : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE put_SampleRate(UINT32 sampleRate) = 0;
	virtual HRESULT STDMETHODCALLTYPE get_SampleRate(UINT32* pSampleRate)= 0;
	virtual HRESULT STDMETHODCALLTYPE put_Quality(UINT32 quality)=0;
	virtual HRESULT STDMETHODCALLTYPE get_Quality(UINT32* pQuality)=0;
	virtual HRESULT STDMETHODCALLTYPE put_State(UINT32 state)=0;
	virtual HRESULT STDMETHODCALLTYPE get_State(UINT32* pState)=0;
};

//{782416A6-EB09-4c14-A836-55D4D4E44AA4}
const CLSID IID_SpeexDecIProp = {0x782416a6, 0xeb09, 0x4c14, {0xa8, 0x36, 0x55, 0xd4, 0xd4, 0xe4, 0x4a, 0xa4}};
MIDL_INTERFACE("782416a6-eb09-4c14-a836-55d4d4e44aa4")
SpeexDecIProp : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE put_SampleRate(UINT32 sampleRate) = 0;
	virtual HRESULT STDMETHODCALLTYPE get_SampleRate(UINT32* pSampleRate)= 0;
	virtual HRESULT STDMETHODCALLTYPE put_Quality(UINT32 quality)=0;
	virtual HRESULT STDMETHODCALLTYPE get_Quality(UINT32* pQuality)=0;
};
//end by dsh 10-07-29/////


const CLSID IID_IHWVideoEncoder = {0xA4912AE3,0x4AD1,0x4c7f,{0xB9,0xC6,0xAE,0x15,0x66,0xF8,0xDE,0x5D}};
MIDL_INTERFACE("A4912AE3-4AD1-4c7f-B9C6-AE1566F8DE5D")
HWVideoEncoder :  public IUnknown
{
public:
	virtual void STDMETHODCALLTYPE setEncoderState( bool work );
};//Patrick

// {A7B65F66-5354-48c6-8E7E-E1D1154B45D1}
const CLSID IID_ISCUTVideoEncoder = {0xa7b65f66, 0x5354, 0x48c6, {0x8e, 0x7e, 0xe1, 0xd1, 0x15, 0x4b, 0x45, 0xd1}};
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

//by dsh 2010-10-18
//echo cancel
const CLSID IID_IAECProperty = {0x3ee64499, 0xf3c2, 0x4074, {0x8e, 0x41, 0xfe, 0xe9, 0x1c, 0x8b, 0xe3, 0x6f}};
MIDL_INTERFACE("3EE64499-F3C2-4074-8E41-FEE91C8BE36F")
IAECProperty : public IUnknown
{
public:

	STDMETHOD(put_CapDevFriendName) (wchar_t* capDevFriendName,int capNameSize,
			wchar_t* rendDevFriendName,int rendNameSize)=0;
	STDMETHOD(put_PreviewHandle)(HWND hPreviewHandle)=0;

};

//VideoScalerControl ..zhenHua.sun 2011-04-12
const CLSID IID_IVideoScalerControl = {0x8f3594f4, 0x59d3, 0x47ea, {0xb0, 0x90, 0xcb, 0x5f, 0x6c, 0x67, 0xfa, 0x93}};
MIDL_INTERFACE("8F3594F4-59D3-47ea-B090-CB5F6C67FA93")
IVideoScalerControl : public IUnknown
{
public:

	//设置视频缩放后的高和宽
	virtual HRESULT STDMETHODCALLTYPE SetOutputParams( const int width , const int height ) =0;

	//获取视频缩放后的高和宽
	virtual HRESULT STDMETHODCALLTYPE GetOutputParams( int* width , int* height )=0;
};


MIDL_INTERFACE("DE58BFED-53EB-494a-85F4-4C0020B92D86")
ISCUTMpeg4Mux : public IUnknown
{
  virtual HRESULT SetCallback(__in void * obj, __in RecordStatCallback func);

  virtual HRESULT SetInitialJobId(__in long nJobId) = 0;

  // NO more than one %d could be used for episode num. e.g. _T("Video%03d.mp4")
  virtual HRESULT SetFileName(__in LPCTSTR szName) = 0;

  // Set szName to NULL to retrieve the string size required
  virtual HRESULT GetFileName(__out_opt LPTSTR szName, __inout UINT &nLength) = 0;

  // when called, file will be cut at once and new file referred by szFileName created
  // nJobId should never repeat!
  virtual HRESULT CutMovieFile(__in long nJobId, __in_opt LPCTSTR szNewFileName) = 0; 
};

//FilePlayCtrl
const CLSID IID_IFilePlayCtrl = {0x238b36e2, 0x87fc, 0x4087, {0xa1, 0x9f, 0x3b, 0x8a, 0xc, 0x3d, 0x90, 0x13}};
MIDL_INTERFACE("238B36E2-87FC-4087-A19F-3B8A0C3D9013")
ISCUTFilePlayCtrl : public IUnknown
{
public:
    // sync id
    virtual HRESULT SetSyncId(const char *id) = 0;

    // initial timestamp offset
    virtual HRESULT SetInitialTsOffset(REFERENCE_TIME offset) = 0;

    // play operation code and param
    enum PlayOpCode { kOpPause, kOpResume, kOpSeek };
    union PlayOpParam {
        struct {
            long long t;
        } seek;
    };
    // play operation. each time call this method, op_id must larger than previous 
    virtual HRESULT Operate(unsigned long op_id, PlayOpCode op, PlayOpParam *param) = 0;

    enum VideoCodec { VIDEO_NONE = 0, H264 };
    enum AudioCodec { AUDIO_NONE = 0, AAC_LC };
    struct MediaFormat
    {
        VideoCodec video_codec;
        long video_width;
        long video_height;
        long video_fps;
        AudioCodec audio_codec;
        long audio_sample_rate;
        long audio_channel;
        long audio_bit_count;
    };
    //call this method to get media infomation
    virtual HRESULT GetMediaFormat(MediaFormat &fmt) = 0;

    enum PlayStatus { ST_STOP = 0, ST_PLAYING, ST_PAUSE, ST_WAIT, ST_EOS };
    struct ProgressInfo
    {
        PlayStatus status;
        long long position;
        long long duration;
    };
    //call this method to get play progress info
    virtual HRESULT GetProgress(ProgressInfo &info) = 0;
};
