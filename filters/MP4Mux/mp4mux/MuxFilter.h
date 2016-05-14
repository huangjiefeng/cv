//
// MuxFilter.h
//
// Declaration of classes for DirectShow MPEG-4 Multiplexor filter
//
// Geraint Davies, May 2004
//
// Copyright (c) GDCL 2004-6. All Rights Reserved. 
// You are free to re-use this as the basis for your own filter development,
// provided you retain this copyright notice in the source.
// http://www.gdcl.co.uk
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovieWriter.h"
#include "alloc.h"
#include "IScutMpeg4Mux.h"
#include "FileWriter.h"

#include <map>
#include <vector>
#include <memory>

// forward declarations
class Mpeg4Mux;
class MuxInput;
class MuxOutput;
class MuxAllocator;

class Notifier;

// override the standard allocator to 
// force use of a larger number of buffers. 
// We use the input buffers to queue the chunks for
// interleaving, so the input connection must allow
// us to hold at least 2 seconds of data.
class MuxAllocator : public CMemAllocator
{
public:
    MuxAllocator(LPUNKNOWN pUnk, HRESULT* phr, long cMaxBuffer);

    // we override this just to increase the requested buffer count
    STDMETHODIMP SetProperties(
            ALLOCATOR_PROPERTIES* pRequest,
            ALLOCATOR_PROPERTIES* pActual);
private:
	long m_cMaxBuffer;
};

// input pin, receives data corresponding to one
// media track in the file.
// Pins are created and deleted dynamically to 
// ensure that there is always one unconnected pin.
class MuxInput 
: public CBaseInputPin,
  public IAMStreamControl
{
public:
    MuxInput(Mpeg4Mux* pFilter, CCritSec* pLock, HRESULT* phr, LPCWSTR pName, int index);
	~MuxInput();

    // lifetime management for pins is normally delegated to the filter, but
    // we need to be able to create and delete them independently, so keep 
    // a separate refcount.
    STDMETHODIMP_(ULONG) NonDelegatingRelease()
    {
        return CUnknown::NonDelegatingRelease();
    }
    STDMETHODIMP_(ULONG) NonDelegatingAddRef()
    {
        return CUnknown::NonDelegatingAddRef();
    }

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID iid, void** ppv)
	{
		if (iid == IID_IAMStreamControl)
		{
			return GetInterface((IAMStreamControl*) this, ppv);
		}
		return __super::NonDelegatingQueryInterface(iid, ppv);
	}

    // CBasePin overrides
    HRESULT CheckMediaType(const CMediaType* pmt);
    HRESULT GetMediaType(int iPosition, CMediaType* pmt);
    
    // input
    STDMETHODIMP Receive(IMediaSample* pSample);
    STDMETHODIMP EndOfStream();
    STDMETHODIMP BeginFlush();
    STDMETHODIMP EndFlush();

    // state change
    HRESULT Active();
    HRESULT Inactive();

    // connection management -- used to maintain one free pin
    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);

    // support custom allocator
    STDMETHODIMP GetAllocator(IMemAllocator** ppAllocator);
	STDMETHODIMP NotifyAllocator(IMemAllocator* pAlloc, BOOL bReadOnly);

	// IAMStreamControl methods
	STDMETHOD(StartAt)(const REFERENCE_TIME* ptStart, DWORD dwCookie);
	STDMETHOD(StopAt)(const REFERENCE_TIME* ptStop, BOOL bSendExtra, DWORD dwCookie);
	STDMETHOD(GetInfo)(AM_STREAM_INFO* pInfo);

    int GetIndex() { return m_index; }

private:
	bool ShouldDiscard(IMediaSample* pSample);
	HRESULT CopySampleProps(IMediaSample* pIn, IMediaSample* pOut);

private:
    Mpeg4Mux* m_pMux;
    int m_index;
    //TrackWriter* m_pTrack;

	CCritSec m_csSample;  // Liaokz, 2013-5
	CCritSec m_csStreamControl;
	AM_STREAM_INFO m_StreamInfo;

	ContigBuffer m_CopyBuffer;
	Suballocator* m_pCopyAlloc;
};


// output pin, writes multiplexed data downstream
// using IMemOutputPin while running, and then writes 
// metadata using IStream::Write when stopping.
class MuxOutput 
: /*public CBaseOutputPin,*/
  public AtomWriter
{
public:
    MuxOutput(Mpeg4Mux* pFilter, LPCTSTR lpszFile/*, CCritSec* pLock*/, HRESULT* phr);

	// CBaseOutputPin is no longer useful since we directly write file. --By Liaokz, 2013-5-4
    // CBaseOutputPin overrides
    /*HRESULT CheckMediaType(const CMediaType* pmt);
    HRESULT GetMediaType(int iPosition, CMediaType* pmt);
    HRESULT DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pprop);
    HRESULT CompleteConnect(IPin *pReceivePin);
	HRESULT BreakConnect();*/

    // called from filter
    void Reset();
    //void UseIStream();
    void FillSpace();

    // AtomWriter methods
    LONGLONG Length();
    LONGLONG Position();
    HRESULT Replace(LONGLONG pos, const BYTE* pBuffer, long cBytes);
    HRESULT Append(const BYTE* pBuffer, long cBytes);

	// Add by Liaokz
	void SetMovieWriter(MovieWriter * pMovie);

private:
    Mpeg4Mux* m_pMux;
    CCritSec m_csWrite;
    //bool m_bUseIStream;
    LONGLONG m_llBytes;
	//IStreamPtr m_pIStream;

	// Add by liaokz, 2013-5-4
	HANDLE   m_hFile;               // Handle to file for dumping
	TCHAR    m_szFileName[MAX_PATH];     // The filename where we dump
	FileWriter m_fileWriter;

	MovieWriter * m_pMovie;

	static TCHAR s_szLastFileName[MAX_PATH];     // The last file name, used to reduce the create file failure log
};

// To pass seeking calls upstream we must try all connected input pins.
// Where two input pins lead to the same splitter, only one will be
// allowed to SetTimeFormat at once, so we must call this on all
// pins and store them, then call SetTimeFormat(NULL) once the
// operation is complete.
// 
// This class manages that list of seekable pins. 
// It is also used for seeking calls that do not need to set 
// the time format.
class SeekingAggregator
{
public:
    SeekingAggregator(CBaseFilter* pFilter, bool bSetTimeFormat = false);
    ~SeekingAggregator();

    typedef list<IMediaSeeking*>::iterator iterator;
    iterator Begin()
    {
        return m_Pins.begin();
    }
    iterator End()
    {
        return m_Pins.end();
    }

private:
    bool m_bSetTimeFormat;
    list<IMediaSeeking*> m_Pins;

};


// For remember the tracks type. --By Liaokz, 2013-5-4
struct TrackStatus_t 
{
	const CMediaType *pmt;
	TrackWriter* pTrack;
};

typedef std::map<int, TrackStatus_t> TrackStatusMap;



class DECLSPEC_UUID("5FD85181-E542-4e52-8D9D-5D613C30131B")
Mpeg4Mux 
: public CBaseFilter,
  public IMediaSeeking,
  public ISCUTMpeg4Mux
{
public:
    // constructor method used by class factory
    static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT* phr);

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID iid, void** ppv);

    // filter registration tables
    static const AMOVIESETUP_MEDIATYPE m_sudType[];
    static const AMOVIESETUP_PIN m_sudPin[];
    static const AMOVIESETUP_FILTER m_sudFilter;

    // CBaseFilter methods
    int GetPinCount();
    CBasePin *GetPin(int n);
	STDMETHODIMP Run(REFERENCE_TIME tStart);
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

    void CreateInput();

    // called from input pin
    void OnDisconnect(int index);
    void OnConnect(int index);
    bool CanReceive(const CMediaType* pmt);
    bool MakeTrack(int index, const CMediaType* pmt);
	TrackWriter * GetTrack(int index);
	bool OnReceiveSample(int index, IMediaSample* pSample);
    void OnEOS();
	REFERENCE_TIME Start() { return m_tStart;}

    // we implement IMediaSeeking to allow encoding
    // of specific portions of an input clip, and
    // to report progress via the current position.
    // Calls (apart from current position) are
    // passed upstream to any pins that support seeking
// IMediaSeeking
public:
    STDMETHODIMP GetCapabilities(DWORD * pCapabilities );
    STDMETHODIMP CheckCapabilities(DWORD * pCapabilities );
    STDMETHODIMP IsFormatSupported(const GUID * pFormat);
    STDMETHODIMP QueryPreferredFormat(GUID * pFormat);
    STDMETHODIMP GetTimeFormat(GUID *pFormat);
    STDMETHODIMP IsUsingTimeFormat(const GUID * pFormat);
    STDMETHODIMP SetTimeFormat(const GUID * pFormat);
    STDMETHODIMP GetDuration(LONGLONG *pDuration);
    STDMETHODIMP GetStopPosition(LONGLONG *pStop);
    STDMETHODIMP GetCurrentPosition(LONGLONG *pCurrent);
    STDMETHODIMP ConvertTimeFormat(LONGLONG * pTarget, const GUID * pTargetFormat,
                              LONGLONG    Source, const GUID * pSourceFormat );
    STDMETHODIMP SetPositions(LONGLONG * pCurrent, DWORD dwCurrentFlags
			, LONGLONG * pStop, DWORD dwStopFlags );
    STDMETHODIMP GetPositions(LONGLONG * pCurrent,
                              LONGLONG * pStop );
    STDMETHODIMP GetAvailable(LONGLONG * pEarliest, LONGLONG * pLatest );
    STDMETHODIMP SetRate(double dRate);
    STDMETHODIMP GetRate(double * pdRate);
    STDMETHODIMP GetPreroll(LONGLONG * pllPreroll);

	
	// ISCUTMpeg4Mux
	HRESULT SetCallback(__in void * obj, __in RecordStatCallback func);
    HRESULT SetInitialJobId(__in long nJobId) { m_nNextJobId = nJobId; return S_OK; }
	HRESULT SetFileName( __in LPCTSTR szName );
	HRESULT GetFileName( __out_opt LPTSTR szName, __inout UINT &nLength );
	HRESULT CutMovieFile(__in long nJobId, __in_opt LPCTSTR szFileName);

    // Other methods
    void NotifyFileSizeChange(TCHAR *szFile, LONGLONG llSize);
    void NotifySampleTime(REFERENCE_TIME rtSample);


private:

    struct EpisodeNode
    {
        //当前分段的组件，当前分段的与上面的m_pOutput等一致
        MuxOutput* pOutput;
        MovieWriter* pMovie;
        TrackStatusMap pTracks;
        //其他信息
        int nEpisode; //分集号
        long nJobId;
        TCHAR szFileNames[MAX_PATH]; //正在写入的临时文件名
        long long llStartTime; //分集开始的绝对时间，in 100ns unit
        REFERENCE_TIME tDuration;
        HANDLE hCloseProc;

        //加个构造函数来初始化
        EpisodeNode();
    };
    typedef std::shared_ptr<EpisodeNode> EpisodeNodePointer;
    typedef std::vector<EpisodeNodePointer> EpisodeList;


private:
    // construct only via class factory
    Mpeg4Mux(LPUNKNOWN pUnk, HRESULT* phr);
    ~Mpeg4Mux();

	bool CreateMovie();
	bool CloseMovie(EpisodeNodePointer episode);
	bool ShouldNextEpisodeStart(REFERENCE_TIME rtStart) const;

	struct CloseMovieParams
	{
		Mpeg4Mux *pMux;
		EpisodeNode *pEpisode;
	};
	static unsigned __stdcall _CloseMovieProc(void *);

    inline TrackWriter *GetTrack(int EpiIdx, int TrckIdx);

private:
    CCritSec m_csFilter;
	CCritSec m_csMovie;
	vector<MuxInput*> m_pInputs;

    MuxOutput* m_pOutput;
    MovieWriter* m_pMovie;

    // for GetCurrentPosition
    REFERENCE_TIME m_tWritten;

	// 以下是录制分段添加的成员
	int m_nEpisode;
	TCHAR m_szFileTemplateName[MAX_PATH]; // Inputed file name, maybe a template string like video%03d.mp4
    EpisodeList m_Episodes;

    long m_nNextJobId; // 用于给上层区分报告所述分集的标识符
	REFERENCE_TIME m_rtEpisodeStart; // 当前分段的起始流媒体参考时间
    REFERENCE_TIME m_rtLastNotifiedTimestamp; // 最后向上层报告的流媒体参考时间
    LONGLONG m_llLastNotifiedSize; // 上次通知上层时的文件大小
	bool m_bEpisodeTransition; // 两集视频的过渡状态，此时部分Track已经开始进入新的一集
	bool m_bCutByUser;  // 由外部发起了分级
	bool m_bStartMux; // 是否开始接收数据 - 启动后必须在视频收到关键帧时才启动其他Track的接收，以保证同步

};

