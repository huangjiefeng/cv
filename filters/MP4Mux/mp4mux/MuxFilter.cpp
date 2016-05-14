//
// MuxFilter.cpp
//
// Implementation of classes for DirectShow MPEG-4 Multiplexor filter
//
// Geraint Davies, May 2004
//
// Copyright (c) GDCL 2004-6. All Rights Reserved. 
// You are free to re-use this as the basis for your own filter development,
// provided you retain this copyright notice in the source.
// http://www.gdcl.co.uk
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INITGUID.H"
#include "MuxFilter.h"

#include <sstream>
#include <time.h>
#include <string>

#include <process.h>

#include "util.h"
#include "logger.h"
#include "Notifier.h"

#define MP4_TMP_SUFFIX TEXT(".tmp")
//#define EpisodeToIndex(epi) ((epi) % FILE_INFO_BUF)
//#define CurEpisode(epi_count) ((epi_count)-1)
//#define CurIndex(epi_count)  (((epi_count)-1) % FILE_INFO_BUF)
//#define PrevEpisode(epi_count) ((epi_count)-2)
//#define PrevIndex(epi_count)  (((epi_count)-2) % FILE_INFO_BUF)

#define CurIdx 0
#define LastIdx 1
#define BeforeLastIdx 2
#define HasLastEpisode (m_Episodes.size()>=2)

Logger theLogger(TEXT("MP4Mux.log"));


// --- registration tables ----------------

// filter registration -- these are the types that our
// pins accept and produce
const AMOVIESETUP_MEDIATYPE 
Mpeg4Mux::m_sudType[] = 
{
    {
        &MEDIATYPE_Video,
        &MEDIASUBTYPE_NULL      // wild card
    },
    {
        &MEDIATYPE_Audio,
        &MEDIASUBTYPE_NULL
    },
    {
        &MEDIATYPE_Stream,
        &MEDIASUBTYPE_NULL,
    },
};

// registration of our pins for auto connect and render operations
const AMOVIESETUP_PIN 
Mpeg4Mux::m_sudPin[] = 
{
    {
        L"Video",           // pin name
        FALSE,              // is rendered?    
        FALSE,              // is output?
        FALSE,              // zero instances allowed?
        FALSE,              // many instances allowed?
        &CLSID_NULL,        // connects to filter (for bridge pins)
        NULL,               // connects to pin (for bridge pins)
        1,                  // count of registered media types
        &m_sudType[0]       // list of registered media types    
    },
    {
        L"Audio",           // pin name
        FALSE,              // is rendered?    
        FALSE,              // is output?
        FALSE,              // zero instances allowed?
        FALSE,              // many instances allowed?
        &CLSID_NULL,        // connects to filter (for bridge pins)
        NULL,               // connects to pin (for bridge pins)
        1,                  // count of registered media types
        &m_sudType[1]       // list of registered media types    
    },
    {
        L"Output",          // pin name
        FALSE,              // is rendered?    
        TRUE,               // is output?
        FALSE,              // zero instances allowed?
        FALSE,              // many instances allowed?
        &CLSID_NULL,        // connects to filter (for bridge pins)
        NULL,               // connects to pin (for bridge pins)
        1,                  // count of registered media types
        &m_sudType[2]       // list of registered media types    
    },
};

// filter registration information. 
const AMOVIESETUP_FILTER 
Mpeg4Mux::m_sudFilter = 
{
    &__uuidof(Mpeg4Mux),    // filter clsid
    L"SCUT Mpeg-4 Multiplexor Writer",        // filter name - Change from "GDCL Mpeg-4 Multiplexor". By Liaokz, 2013-5-4
    MERIT_DO_NOT_USE,       // ie explicit insertion only
    3,                      // count of registered pins
    m_sudPin                // list of pins to register
};

// ---- construction/destruction and COM support -------------

// the class factory calls this to create the filter
//static 
CUnknown* WINAPI 
Mpeg4Mux::CreateInstance(LPUNKNOWN pUnk, HRESULT* phr)
{
    return new Mpeg4Mux(pUnk, phr);
}


Mpeg4Mux::Mpeg4Mux(LPUNKNOWN pUnk, HRESULT* phr)
: CBaseFilter(NAME("Mpeg4Mux"), pUnk, &m_csFilter, *m_sudFilter.clsID),
  m_pOutput(NULL),
  m_pMovie(NULL),
  m_tWritten(0),
  m_nEpisode(0),
  m_nNextJobId(0),
  m_rtEpisodeStart(-1),
  m_rtLastNotifiedTimestamp(-1),
  m_llLastNotifiedSize(0),
  m_bEpisodeTransition(false),
  m_bCutByUser(false)
{
    // create output pin and one free input
    //m_pOutput = new MuxOutput(this, , &m_csFilter, phr);
	UNREFERENCED_PARAMETER(phr);
    CreateInput();
	SetFileName(_T("rec\\TestVideo.mp4"));
}

Mpeg4Mux::~Mpeg4Mux()
{
    delete m_pOutput;
    for (UINT i = 0; i < m_pInputs.size(); i++)
    {
        m_pInputs[i]->Release();
    }
}


STDMETHODIMP 
Mpeg4Mux::NonDelegatingQueryInterface(REFIID iid, void** ppv)
{
    if (iid == IID_IMediaSeeking)
    {
        return GetInterface((IMediaSeeking*) this, ppv);
    }
	if (iid == IID_ISCUTMpeg4Mux)
	{
		return GetInterface((ISCUTMpeg4Mux*) this, ppv);
	}

    return CBaseFilter::NonDelegatingQueryInterface(iid, ppv);
}

int 
Mpeg4Mux::GetPinCount()
{
    // one output, plus variable set of inputs
    return (int) (m_pInputs.size() /*+ 1*/);  // Modified to hide output pin. --By Liaokz, 2013-5-4
}

CBasePin *
Mpeg4Mux::GetPin(int n)
{
    // list all inputs first, then one output

	// Modified to hide output pin. --By Liaokz, 2013-5-4
   /* if (n == (int)m_pInputs.size())
    {
        return m_pOutput;
    } else */
	if (n < (int)m_pInputs.size())
    {
        return m_pInputs[n];
    }
    return NULL;
}

void
Mpeg4Mux::CreateInput()
{
	ostringstream strm;
	strm << "Input " << m_pInputs.size() + 1;
	_bstr_t str = strm.str().c_str();

	HRESULT hr = S_OK;
    MuxInput* pPin = new MuxInput(this, &m_csFilter, &hr, str, (int) (m_pInputs.size()));
    pPin->AddRef();
    m_pInputs.push_back(pPin);

    // ensure enumerator is refreshed
    IncrementPinVersion();
}

void 
Mpeg4Mux::OnDisconnect(int index)
{
    // if index is the last but one, and
    // the last one is still unconnected, then
    // remove the last one
    if ((index == (int)(m_pInputs.size() - 2)) &&
        (!m_pInputs[m_pInputs.size()-1]->IsConnected()))
    {
        m_pInputs[m_pInputs.size()-1]->Release();
        m_pInputs.pop_back();

        // refresh enumerator
        IncrementPinVersion();
    }
}

void 
Mpeg4Mux::OnConnect(int index)
{
    // if this is the last one, make a new one
    if (index == (int)(m_pInputs.size()-1))
    {
        CreateInput();
    }
}

bool 
Mpeg4Mux::CanReceive(const CMediaType* pmt)
{
    return TypeHandler::CanSupport(pmt);
}

bool 
Mpeg4Mux::MakeTrack(int index, const CMediaType* pmt)
{
    CAutoLock lock(&m_csMovie);

    TrackStatus_t trackStatus = {0};
    trackStatus.pmt = pmt; //先记录媒体类型,以便分段过程中m_pMovie为NULL时可重建Track
	if (NULL == m_pMovie)
	{
		return false;
	}
	TrackWriter* pTrack = m_pMovie->MakeTrack(pmt);
    trackStatus.pTrack = pTrack;

	m_Episodes[CurIdx]->pTracks[index] = trackStatus;

	return pTrack != NULL;
}

TrackWriter *
Mpeg4Mux::GetTrack(int index)
{
	CAutoLock lock(&m_csMovie);
	if (m_bEpisodeTransition && HasLastEpisode)
    {
        TrackWriter *OldTrack = GetTrack(LastIdx, index);
        if (OldTrack && !OldTrack->IsAtEOS()) {
            //上一集的该track还应该写入数据
            return  OldTrack;
        }
	}
	return GetTrack(CurIdx, index);
}

TrackWriter * 
Mpeg4Mux::GetTrack(int EpiIdx, int TrckIdx)
{
    CAutoLock lock(&m_csMovie);
    if ((int)m_Episodes.size() > EpiIdx && 
        m_Episodes[EpiIdx]->pTracks.find(TrckIdx) != m_Episodes[EpiIdx]->pTracks.end()) {
        return m_Episodes[EpiIdx]->pTracks[TrckIdx].pTrack;
    }
    return NULL;
}

/*
 在正式处理Sample前作预处理，以确定是否接受Sample或是否分割视频。
 @return 返回true表示应该接受sample，否则不应处理sample。
*/
bool 
Mpeg4Mux::OnReceiveSample(int index, IMediaSample* pSample)
{
	// ensure one thread access this function in a time
	// also in case GetTrack access during movie switching
	CAutoLock lock(&m_csMovie);

	REFERENCE_TIME rtStart, rtEnd;

	if (NULL == m_pOutput)
	{
		// retry
		if (!CreateMovie())
		{
			return false;
		}
		LOG(("Retry to create MovieWriter succeed"));
	}

	if (S_OK == pSample->GetTime(&rtStart, &rtEnd))
	{
		// in a status that some tracks should write to new episode while others not
        TrackWriter *pOldTrack = GetTrack(LastIdx, index);
		if (m_bEpisodeTransition && pOldTrack && !pOldTrack->IsAtEOS())
		{
			if (rtStart >= m_rtEpisodeStart)
			{
				// Mark Track[index] is EOS in last episode
				if (pOldTrack->OnEOS())
				{
                    EpisodeNodePointer last = m_Episodes[LastIdx];
					last->pMovie->Stop();
					CloseMovie(last);
					m_bEpisodeTransition = false;
				}
			}
			return true;
		}
		// DO NOT use else, so old movie can be force closed when it not be closed properly.
		// if video, check to see whether new should episode starts
        ASSERT(m_Episodes[CurIdx]->pTracks.find(index) != m_Episodes[CurIdx]->pTracks.end());
		if (m_Episodes[CurIdx]->pTracks[index].pmt->majortype == MEDIATYPE_Video && 
            S_OK == pSample->IsSyncPoint())
		{
			// For 1st episode only. Ensure audio and video sync.
			if (!m_bStartMux)
			{
				m_bStartMux = true;
				m_rtEpisodeStart = rtStart;
                m_bCutByUser = false; // 清除待分集标记
                m_Episodes[CurIdx]->llStartTime = GetDShowTimeFromUnixTime(NULL); //以收到第一帧的时间为准更精确
                //接下来考虑一种情况：如果开始录制时对方停止发送该视角的媒体流，当其他视角开始录制第n集了，
                //该视角才开始接收媒体流，此时该视角的第一集也应该按照第n集处理，故还要设置一下两个成员
                m_Episodes[CurIdx]->nJobId = m_nNextJobId; // 将job_id设为最新
				LOG_PRINT("Start muxing 1st episode, job=%d", m_nNextJobId);
                Notifier::getInstance()->FileStartWrite(m_nNextJobId, m_Episodes[CurIdx]->szFileNames);
			}
			if (ShouldNextEpisodeStart(rtStart))
			{
				if (!CreateMovie())
				{
					LOG(("Create new MovieWriter failed! Data will be writen to old file."));
					return false;
                }
                m_rtEpisodeStart = rtStart;
		        m_bCutByUser = false; // clear mark, if set

                EpisodeNodePointer cur = m_Episodes[CurIdx];
                EpisodeNodePointer last = m_Episodes[LastIdx];
				if (last->pTracks[index].pTrack->OnEOS()) // Mark the video track EOS
				{
					last->pMovie->Stop();
					CloseMovie(last);
					return true;
				}
                cur->llStartTime = GetDShowTimeFromUnixTime(NULL);
                m_bEpisodeTransition = true;  // Mark new episode starts status, wait when all tracks EOS, movie can be closed.
                LOG_PRINT("Start muxing episode %d, job=%d", cur->nEpisode, cur->nJobId);
                Notifier::getInstance()->FileStartWrite(cur->nJobId, cur->szFileNames);
			}
		}
		else if (!m_bStartMux || rtStart < m_rtEpisodeStart) // For 1st episode av sync
		{
            // drop audio samples received before the 1st video I-Frame
			return false;
		}
		return true;
	}

	return false;
}

bool Mpeg4Mux::ShouldNextEpisodeStart(REFERENCE_TIME rtStart) const
{
    UNREFERENCED_PARAMETER(rtStart);
	if (m_bCutByUser)
	{
		return true;
	}
	return false;
}

void 
Mpeg4Mux::OnEOS()
{
    // all tracks are now written
    //m_pOutput->DeliverEndOfStream(); // --By Liaokz, 2013-5-4
}

STDMETHODIMP
Mpeg4Mux::Run(REFERENCE_TIME tStart)
{
	Notifier::getInstance()->MuxRun();
	return CBaseFilter::Run(tStart);
}

STDMETHODIMP 
Mpeg4Mux::Stop()
{
    HRESULT hr = S_OK;
    if (m_State != State_Stopped)
    {
        //“从最老的一集开始”关闭所有未关闭的分段
        //pMovie不为NULL，且hCloseProc==NULL（关闭线程没开启）,则表示分段未关闭
        for (auto it = m_Episodes.rbegin(); it != m_Episodes.rend(); ++it) {
            EpisodeNodePointer episode = (*it);
            // ensure that queue-writing is stopped
            if (episode->pMovie && !episode->hCloseProc)
            {
                episode->pMovie->Stop();

                // switch to IStream for post-run fixup.
                //m_pOutput->UseIStream();
            }
        }

        // stop all input pins
		hr = CBaseFilter::Stop();

        for (auto it = m_Episodes.rbegin(); it != m_Episodes.rend(); ++it) {
            EpisodeNodePointer episode = (*it);
            if (episode->pMovie && !episode->hCloseProc)
            {
                CloseMovie(episode);
            }
        }

        //等待所有的关闭文件线程结束
        DWORD nEpiCount = m_Episodes.size();
		DWORD nThread = 0;
        HANDLE *hTreads = new HANDLE[nEpiCount];
        for (DWORD i = 0; i < nEpiCount; ++i) {
            if (m_Episodes[i]->hCloseProc) 
                hTreads[nThread++] = m_Episodes[i]->hCloseProc;
        }
		if (nThread > 0)
		{
			WaitForMultipleObjects(nThread, hTreads, TRUE, INFINITE);
			for (DWORD i = 0; i < nThread; ++i)
			{
				CloseHandle(hTreads[i]);
			}
			LOG_PRINT("All CloseMovie threads exit.");
		}
        delete [] hTreads;

        //清理
        m_Episodes.clear();
        m_pMovie = NULL;
        m_pOutput = NULL;
        m_bStartMux = false;
        m_tWritten = 0;

        Notifier::getInstance()->MuxStop();
    }
    return hr;
}
    

STDMETHODIMP 
Mpeg4Mux::Pause()
{
    if (m_State == State_Stopped)
    {
		LOG_PRINT("Init MovieWriter.");
		m_bStartMux = false;
		if (!CreateMovie())
		{
			LOG(("Init MovieWriter failed! No data will be writen to file."));
		}
    }
    return CBaseFilter::Pause();
}
// ------- input pin -------------------------------------------------------

MuxInput::MuxInput(Mpeg4Mux* pFilter, CCritSec* pLock, HRESULT* phr, LPCWSTR pName, int index)
: m_pMux(pFilter),
  m_index(index),
  //m_pTrack(NULL),
  m_pCopyAlloc(NULL),
  CBaseInputPin(NAME("MuxInput"), pFilter, pLock, phr, pName)
{
	ZeroMemory(&m_StreamInfo, sizeof(m_StreamInfo));
}

MuxInput::~MuxInput()
{
	if (m_pCopyAlloc)
	{
		m_pCopyAlloc->Release();
	}
}

HRESULT 
MuxInput::CheckMediaType(const CMediaType* pmt)
{
    if (m_pMux->CanReceive(pmt))
    {
        return S_OK;
    }
    return VFW_E_TYPE_NOT_ACCEPTED;
}

HRESULT 
MuxInput::GetMediaType(int iPosition, CMediaType* pmt)
{
    UNREFERENCED_PARAMETER(iPosition);
    UNREFERENCED_PARAMETER(pmt);
    return VFW_S_NO_MORE_ITEMS;
}

STDMETHODIMP 
MuxInput::Receive(IMediaSample* pSample)
{
	CAutoLock lock(&m_csSample);

    HRESULT hr = CBaseInputPin::Receive(pSample);
    if (hr != S_OK)
    {
        return hr;
    }

	if (!m_pMux->OnReceiveSample(m_index, pSample))
	{
		return S_OK;
	}

	TrackWriter * pTrack = m_pMux->GetTrack(m_index);
    if (!pTrack)
    {
        return E_FAIL;
    }

	if (ShouldDiscard(pSample))
	{
		return S_OK;
	}
    {
        REFERENCE_TIME rtStart, rtEnd;
        pSample->GetTime(&rtStart, &rtEnd);
        m_pMux->NotifySampleTime(rtStart);
    }
	if (m_pCopyAlloc)
	{
		BYTE* pSrc;
		pSample->GetPointer(&pSrc);
		IMediaSamplePtr pOurs;
		hr = m_pCopyAlloc->AppendAndWrap(pSrc, pSample->GetActualDataLength(), &pOurs);
		if (SUCCEEDED(hr))
		{
			hr = CopySampleProps(pSample, pOurs);
		}
		if (SUCCEEDED(hr))
		{
			hr = pTrack->Add(pOurs);
		}
		return hr;
	}

    return pTrack->Add(pSample);
}

// copy the input buffer to the output
HRESULT
MuxInput::CopySampleProps(IMediaSample* pIn, IMediaSample* pOut)
{
    REFERENCE_TIME tStart, tEnd;
    if (SUCCEEDED(pIn->GetTime(&tStart, &tEnd)))
	{
		ASSERT(tStart <= tEnd);
        pOut->SetTime(&tStart, &tEnd);
    }

	if (SUCCEEDED(pIn->GetMediaTime(&tStart, &tEnd)))
	{
		pOut->SetMediaTime(&tStart, &tEnd);
	}

    if (pIn->IsSyncPoint() == S_OK)
    {
        pOut->SetSyncPoint(true);
    }
    if (pIn->IsDiscontinuity() == S_OK)
    {
        pOut->SetDiscontinuity(true);
    }
    if (pIn->IsPreroll() == S_OK)
    {
        pOut->SetPreroll(true);
    }
	return S_OK;
}

STDMETHODIMP 
MuxInput::EndOfStream()
{
	TrackWriter * pTrack = m_pMux->GetTrack(m_index);
    if ((pTrack != NULL) && (pTrack->OnEOS()))
    {
        // we are the last -- can forward now
        m_pMux->OnEOS();
    }
    return S_OK;
}

STDMETHODIMP 
MuxInput::BeginFlush()
{
    // ensure no more data accepted, and queued
    // data is discarded, so no threads are blocking
	TrackWriter * pTrack = m_pMux->GetTrack(m_index);
    if (pTrack)
    {
        pTrack->Stop(true);
    }
    return S_OK;
}

STDMETHODIMP 
MuxInput::EndFlush()
{
    // we don't re-enable writing -- we support only
    // one contiguous sequence in a file.
    return S_OK;
}

HRESULT 
MuxInput::Active()
{
    HRESULT hr = CBaseInputPin::Active();
    if (SUCCEEDED(hr))
    {
		if (m_pCopyAlloc)
		{
			m_CopyBuffer.ResetAbort();
		}

        m_pMux->MakeTrack(m_index, &m_mt);
    }
    return hr;
}

HRESULT 
MuxInput::Inactive()
{
    // ensure that there are no more writes and no blocking threads
	TrackWriter * pTrack = m_pMux->GetTrack(m_index);
	if (pTrack)
	{
		pTrack->Stop(false);
		//m_pTrack = NULL;
	}
    HRESULT hr = CBaseInputPin::Inactive();
	if (m_pCopyAlloc)
	{
		m_CopyBuffer.Abort();
	}
	return hr;
}

HRESULT 
MuxInput::BreakConnect()
{
    HRESULT hr = CBaseInputPin::BreakConnect();
    m_pMux->OnDisconnect(m_index);
    return hr;
}


HRESULT 
MuxInput::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = CBaseInputPin::CompleteConnect(pReceivePin);
    if (SUCCEEDED(hr))
    {
        m_pMux->OnConnect(m_index);
    }
    return hr;
}

STDMETHODIMP 
MuxInput::GetAllocator(IMemAllocator** ppAllocator)
{
    CAutoLock lock(m_pLock);

    HRESULT hr = S_OK;

	// we supply our own allocator whose only purpose is to increase the requested number of
	// buffers. if possible, we supply a sanity-check max buffer size
	long cMaxBuffer = 0;
	TypeHandler* ph = TypeHandler::Make(&m_mt);
	if (ph)
	{
		if (ph->IsVideo())
		{
			cMaxBuffer = ph->Height() * ph->Width() * 4;
		}
		delete ph;
	}

	MuxAllocator* pAlloc = new MuxAllocator(NULL, &hr, cMaxBuffer);
    if (!pAlloc)
    {
        return E_OUTOFMEMORY;
    }
    pAlloc->QueryInterface(IID_IMemAllocator, (void**)&m_pAllocator);
    return pAlloc->QueryInterface(IID_IMemAllocator, (void**)ppAllocator);
}

	
STDMETHODIMP 
MuxInput::NotifyAllocator(IMemAllocator* pAlloc, BOOL bReadOnly)
{
	ALLOCATOR_PROPERTIES propAlloc;
	pAlloc->GetProperties(&propAlloc);
	LOG_PRINT("NotifyAllocator %d",propAlloc.cBuffers);

	if (propAlloc.cBuffers < 100)
	{
		// too few buffers -- we need to copy
		// -- base the buffer size on 100 x buffers, but
		// restrict to 200MB max
		int cSpace = propAlloc.cbBuffer * 100;
		//cSpace = min((200 * 1024 * 1024), cSpace); --By Liaokz, 2013-8
		cSpace = min((4 * MAX_INTERLEAVE_SIZE), cSpace); // 1个full，1个正在写入的，外加一倍冗余 --By Liaokz
		HRESULT hr = m_CopyBuffer.Allocate(cSpace);
		if (SUCCEEDED(hr))
		{
			m_pCopyAlloc = new Suballocator(&m_CopyBuffer, &hr);
			m_pCopyAlloc->AddRef();
		}
	}
	else
	{
		if (m_pCopyAlloc)
		{
			m_pCopyAlloc->Release();
			m_pCopyAlloc = NULL;
		}
	}
	return __super::NotifyAllocator(pAlloc, bReadOnly);
}
	
STDMETHODIMP
MuxInput::StartAt(const REFERENCE_TIME* ptStart, DWORD dwCookie)
{
	if (ptStart == NULL)
	{
		m_StreamInfo.dwFlags &= ~AM_STREAM_INFO_DISCARDING;
	}
	else if (*ptStart == MAXLONGLONG) 
	{
		// cancels a start request (but does not stop)
		m_StreamInfo.dwFlags &= ~(AM_STREAM_INFO_START_DEFINED);

		// if running, and stop pending, then by some wierd overloading of the spec, this means start now
		if (m_pMux->IsActive() && (m_StreamInfo.dwFlags & AM_STREAM_INFO_STOP_DEFINED))
		{
			m_StreamInfo.dwFlags &= ~AM_STREAM_INFO_DISCARDING;
		}
	}
	else
	{
		m_StreamInfo.dwFlags |= (AM_STREAM_INFO_START_DEFINED | AM_STREAM_INFO_DISCARDING);
		m_StreamInfo.dwStartCookie = dwCookie;
		m_StreamInfo.tStart = *ptStart;
		m_pMux->GetTrack(m_index)->SetStartAt(m_StreamInfo.tStart);
		DbgLog((LOG_TRACE, 0, "Mux StartAt %d ms", long(m_StreamInfo.tStart/10000)));
	}
	return S_OK;
	
}

STDMETHODIMP
MuxInput::StopAt(const REFERENCE_TIME* ptStop, BOOL bSendExtra, DWORD dwCookie)
{
	CAutoLock lock(&m_csStreamControl);

	if (ptStop == NULL)
	{
		m_StreamInfo.dwFlags |= AM_STREAM_INFO_DISCARDING;
	}
	else if (*ptStop == MAXLONGLONG) 
	{
		m_StreamInfo.dwFlags &= ~(AM_STREAM_INFO_STOP_DEFINED);
	}
	else
	{
		m_StreamInfo.dwFlags |= AM_STREAM_INFO_STOP_DEFINED | (bSendExtra ? AM_STREAM_INFO_STOP_SEND_EXTRA : 0);
		m_StreamInfo.dwStopCookie = dwCookie;
		m_StreamInfo.tStop = *ptStop;
		DbgLog((LOG_TRACE, 0, "Mux StopAt %d ms", long(m_StreamInfo.tStop/10000)));
	}
	return S_OK;
}

STDMETHODIMP
MuxInput::GetInfo(AM_STREAM_INFO* pInfo)
{
	CAutoLock lock(&m_csStreamControl);

	*pInfo = m_StreamInfo;
	return S_OK;
}

	
bool 
MuxInput::ShouldDiscard(IMediaSample* pSample)
{
	CAutoLock lock(&m_csStreamControl);
	if (m_StreamInfo.dwFlags & AM_STREAM_INFO_DISCARDING)
	{
		if (m_StreamInfo.dwFlags & AM_STREAM_INFO_START_DEFINED)
		{
			REFERENCE_TIME tStart, tStop;
			if ((SUCCEEDED(pSample->GetTime(&tStart, &tStop))) &&
				(tStop > m_StreamInfo.tStart))
			{
				m_StreamInfo.dwFlags &= ~(AM_STREAM_INFO_DISCARDING | AM_STREAM_INFO_START_DEFINED);
				if (m_StreamInfo.dwStartCookie)
				{
					m_pMux->NotifyEvent(EC_STREAM_CONTROL_STARTED, (LONG_PTR) this, m_StreamInfo.dwStartCookie);
					m_StreamInfo.dwStartCookie = 0;
				}
				TrackWriter * pTrack = m_pMux->GetTrack(m_index);
				if ((tStart < m_StreamInfo.tStart) && pTrack->Handler()->CanTruncate())
				{
					pTrack->Handler()->Truncate(pSample, m_StreamInfo.tStart);
				}
			}
		}
	}
	else
	{
		if (m_StreamInfo.dwFlags & AM_STREAM_INFO_STOP_DEFINED)
		{
			REFERENCE_TIME tStart, tStop;
			if (SUCCEEDED(pSample->GetTime(&tStart, &tStop)))
			{
				DbgLog((LOG_TRACE, 0, "Pending stop %d ms, sample %d", long(m_StreamInfo.tStop/10000), long(tStart/10000)));

				if (tStart >= m_StreamInfo.tStop)
				{
					m_StreamInfo.dwFlags |= AM_STREAM_INFO_DISCARDING;
					m_StreamInfo.dwFlags &= ~(AM_STREAM_INFO_STOP_DEFINED);
					if (m_StreamInfo.dwStopCookie)
					{
						m_pMux->NotifyEvent(EC_STREAM_CONTROL_STOPPED, (LONG_PTR) this, m_StreamInfo.dwStopCookie);
						m_StreamInfo.dwStopCookie = 0;
					}
				}
			}
		}
	}
	return (m_StreamInfo.dwFlags & AM_STREAM_INFO_DISCARDING) ? true : false;
}

// ----------------------


MuxAllocator::MuxAllocator(LPUNKNOWN pUnk, HRESULT* phr, long cMaxBuffer)
: CMemAllocator(NAME("MuxAllocator"), pUnk, phr),
  m_cMaxBuffer(cMaxBuffer)
{
}

// we override this just to increase the requested buffer count
STDMETHODIMP 
MuxAllocator::SetProperties(
        ALLOCATOR_PROPERTIES* pRequest,
        ALLOCATOR_PROPERTIES* pActual)
{
    // !! base buffer count on media type size?

    ALLOCATOR_PROPERTIES prop = *pRequest;

	// some encoders request excessively large output buffers and if we
	// ask for 100 of those, we will use up the system memory
	if ((m_cMaxBuffer == 0) || (prop.cbBuffer <= m_cMaxBuffer))
	{
		// comment by Liaokz, 2013-8
		/*if (prop.cBuffers < 100)
		{
			prop.cBuffers = 100;
		}*/
		// end comment
		// Add by Liaokz, 2013-8
		// 只需要2个buffer，以强迫使用内部的Suballocator，以便内存管理
		if (prop.cBuffers <= 0)
		{
			prop.cBuffers = 2;
		}
		// end Add
	}
    return CMemAllocator::SetProperties(&prop, pActual);
}

// --- output --------------------------------------------------------

TCHAR MuxOutput::s_szLastFileName[MAX_PATH] = {0};

MuxOutput::MuxOutput(Mpeg4Mux* pFilter, LPCTSTR lpszFile/*, CCritSec* pLock*/, HRESULT* phr)
: m_pMux(pFilter),
  m_llBytes(0),
  //m_bUseIStream(true),		// use IStream always
  m_hFile(INVALID_HANDLE_VALUE)
  //CBaseOutputPin(NAME("MuxOutput"), pFilter, pLock, phr, L"Output") // By Liaokz, 2013-5-4
{
	_tcscpy_s(m_szFileName, sizeof(m_szFileName)/sizeof(TCHAR), lpszFile);

	CreateParentDirectory(m_szFileName);

	m_hFile = CreateFile((LPCTSTR) m_szFileName,   // The filename
		GENERIC_WRITE,         // File access
		FILE_SHARE_READ,       // Share access
		NULL,                  // Security
		CREATE_ALWAYS,         // Open flags
		(DWORD) 0,             // More flags
		NULL);

	//ASSERT(INVALID_HANDLE_VALUE != m_hFile);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		if (_tcscmp(s_szLastFileName, m_szFileName) != 0)
		{
			LOG((TEXT("Create file %s failed."), m_szFileName));
		}
		*phr = E_FAIL;
	}

	_tcscpy_s(s_szLastFileName, sizeof(s_szLastFileName)/sizeof(TCHAR), m_szFileName);

	m_fileWriter.SetFile(m_hFile);
}

// CBaseOutputPin overrides
//HRESULT 
//MuxOutput::CheckMediaType(const CMediaType* pmt)
//{
//    if (*pmt->Type() == MEDIATYPE_Stream)
//    {
//        return S_OK;
//    }
//    return VFW_E_TYPE_NOT_ACCEPTED;
//}
//
//HRESULT 
//MuxOutput::GetMediaType(int iPosition, CMediaType* pmt)
//{
//    if (iPosition != 0)
//    {
//        return VFW_S_NO_MORE_ITEMS;
//    }
//    pmt->InitMediaType();
//    pmt->SetType(&MEDIATYPE_Stream);
//    pmt->SetSubtype(&MEDIASUBTYPE_NULL);
//    return S_OK;
//}

//HRESULT 
//MuxOutput::DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pprop)
//{
//    // we can break up large write into multiple buffers, so we are 
//    // better off using a few small buffers
//    pprop->cbBuffer = 4 * 1024;
//    pprop->cBuffers = 20;
//    pprop->cbAlign = 1;
//    ALLOCATOR_PROPERTIES propActual;
//    return pAlloc->SetProperties(pprop, &propActual);
//}
//
//HRESULT 
//MuxOutput::CompleteConnect(IPin *pReceivePin)
//{
//    // make sure that this is the file writer, supporting
//    // IStream, or we will not be able to write out the metadata
//    // at stop time
//    m_pIStream = pReceivePin;
//    if (m_pIStream == NULL)
//    {
//        return E_NOINTERFACE;
//    }
//    return CBaseOutputPin::CompleteConnect(pReceivePin);
//}
	
//HRESULT 
//MuxOutput::BreakConnect()
//{
//	m_pIStream = NULL;
//	return __super::BreakConnect();
//}

void 
MuxOutput::Reset()
{
    CAutoLock lock(&m_csWrite);
    m_llBytes = 0;
    //m_bUseIStream = true;		// always use IStream, so we don't fail when downstream filter is stopped first
}

//void
//MuxOutput::UseIStream()
//{
//    CAutoLock lock(&m_csWrite);
//    //m_bUseIStream = true;
//}

LONGLONG 
MuxOutput::Position()
{
    // start of this container in absolute byte position
    return 0;
}

LONGLONG
MuxOutput::Length()
{
    // length of this atom container (ie location of next atom)
    return m_llBytes;
}

HRESULT 
MuxOutput::Append(const BYTE* pBuffer, long cBytes)
{
    HRESULT hr = Replace(m_llBytes, pBuffer, cBytes);
    m_llBytes += cBytes;
    m_pMux->NotifyFileSizeChange(m_szFileName, m_llBytes);
    return hr;
}

// This function is rewrite to write files directly. --By Liaokz, 2013-5-4
HRESULT 
MuxOutput::Replace(LONGLONG pos, const BYTE* pBuffer, long cBytes)
{
    // all media content is written when the graph is running,
    // using IMemInputPin. On stop (during our stop, but after the
    // file writer has stopped), we switch to IStream for the metadata.
    // The in-memory index is updated after a successful call to this function, so
    // any data not written on completion of Stop will not be in the index.
    CAutoLock lock(&m_csWrite);

	// comment begin -- By Liaokz, 2013-5-4
    //HRESULT hr = S_OK;
    //if (m_bUseIStream)
    //{
    //    IStreamPtr pStream = GetConnected();
    //    if (m_pIStream == NULL)
    //    {
    //        hr = E_NOINTERFACE;
    //    } else {
    //        LARGE_INTEGER liTo;
    //        liTo.QuadPart = pos;
    //        ULARGE_INTEGER uliUnused;
    //        hr = m_pIStream->Seek(liTo, STREAM_SEEK_SET, &uliUnused);
    //        if (SUCCEEDED(hr))
    //        {
    //            ULONG cActual;
    //            hr = m_pIStream->Write(pBuffer, cBytes, &cActual);
    //            if (SUCCEEDED(hr) && ((long)cActual != cBytes))
    //            {
    //                hr = E_FAIL;
    //            }
    //        }
    //    }
    //} else {
    //    // where the buffer boundaries lie is not important in this 
    //    // case, so break writes up into the buffers.
    //    while (cBytes && (hr == S_OK))
    //    {
    //        IMediaSamplePtr pSample;
    //        hr = GetDeliveryBuffer(&pSample, NULL, NULL, 0);
    //        if (SUCCEEDED(hr))
    //        {
    //            long cThis = min(pSample->GetSize(), cBytes);
    //            BYTE* pDest;
    //            pSample->GetPointer(&pDest);
    //            CopyMemory(pDest, pBuffer,  cThis);
    //            pSample->SetActualDataLength(cThis);
    //
    //            // time stamps indicate file position in bytes
    //            LONGLONG tStart = pos;
    //            LONGLONG tEnd = pos + cThis;
    //            pSample->SetTime(&tStart, &tEnd);
    //            hr = Deliver(pSample);
    //            if (SUCCEEDED(hr))
    //            {
    //                pBuffer += cThis;
    //                cBytes -= cThis;
    //                pos += cThis;
    //            }
    //        }
    //    }
    //}
    //return hr;
	// comment end

	if (NULL == m_hFile || INVALID_HANDLE_VALUE == m_hFile)
	{
		return E_FAIL;
	}

	if (m_fileWriter.IsStop())
	{
		m_fileWriter.Start();
	}

	m_fileWriter.Add(pos, pBuffer, cBytes);

	/*if (m_hFile == INVALID_HANDLE_VALUE) {
		return E_FAIL;
	}

	LARGE_INTEGER liTo;
	LARGE_INTEGER liUnused;
	liTo.QuadPart = pos;

	if (SetFilePointerEx(m_hFile, liTo, &liUnused, FILE_BEGIN))
	{
		DWORD dwWritten;

		if (!WriteFile(m_hFile, (PVOID)pBuffer, (DWORD)cBytes,
			&dwWritten, NULL)) 
		{
			return E_FAIL;
		}
	}*/

	//if (m_llBytes != pos || _cbuf + cBytes > BUF_SIZE)
	//{
	//	if (0 < _cbuf)
	//	{
	//		printf("[MP4Mux] Write file %I64d + %d = %I64d\n", _llWriten, _cbuf, _llFile);
	//		// TODO: write buf
	//		liTo.QuadPart = _llWriten;

	//		if (SetFilePointerEx(m_hFile, liTo, &liUnused, FILE_BEGIN))
	//		{
	//			DWORD dwWritten;

	//			if (!WriteFile(m_hFile, (PVOID)_buf, (DWORD)_cbuf,
	//				&dwWritten, NULL)) 
	//			{
	//				ASSERT(FALSE);
	//				return E_FAIL;
	//			}
	//			_llWriten += _cbuf;
	//			_cbuf = 0;
	//			ASSERT(_llWriten == m_llBytes);
	//		}
	//	}
	//}

	//if (m_llBytes != pos) // replace
	//{
	//	ASSERT(_llFile == m_llBytes);
	//	ASSERT(_llWriten == m_llBytes);
	//	ASSERT(pos + cBytes <= m_llBytes);
	//	printf("[MP4Mux] Replace %I64d of %I64d\n", pos, _llFile);
	//	liTo.QuadPart = pos;

	//	if (SetFilePointerEx(m_hFile, liTo, &liUnused, FILE_BEGIN))
	//	{
	//		DWORD dwWritten;

	//		if (!WriteFile(m_hFile, (PVOID)pBuffer, (DWORD)cBytes,
	//			&dwWritten, NULL)) 
	//		{
	//			return E_FAIL;
	//		}
	//	}
	//}
	//else
	//{
	//	//printf("[MP4Mux] Write buf %d + %d\n", _cbuf, cBytes);
	//	ASSERT(_llFile == m_llBytes);
	//	memcpy(_buf + _cbuf, pBuffer, cBytes);
	//	_cbuf += cBytes;
	//	_llFile += cBytes;
	//}

	return S_OK;
}
    
// This function is rewrite to write files directly. --By Liaokz, 2013-5-4
void 
MuxOutput::FillSpace()
{
	if (NULL == m_hFile || INVALID_HANDLE_VALUE == m_hFile) {
		return;
	}

	m_fileWriter.Stop(); // stop and flush writer

	// comment by Liaokz, 2013-5
    /*IStreamPtr pStream = GetConnected();
    if (pStream != NULL)
    {*/
    LARGE_INTEGER li0;
    li0.QuadPart = 0;
    LARGE_INTEGER liEnd;
	// comment by Liaokz, 2013-5
    //HRESULT hr = pStream->Seek(li0, STREAM_SEEK_END, &uliEnd);
    //if (SUCCEEDED(hr))
	if (SetFilePointerEx(m_hFile, li0, &liEnd, FILE_END))
    {
        if (liEnd.QuadPart > (LONGLONG) m_llBytes)
        {
            LONGLONG free = liEnd.QuadPart - m_llBytes;
            if ((free < 0x7fffffff) && (free >= 8))
            {
                // create a free chunk
                BYTE b[8];
                WriteLong(long(free), b);
                WriteLong(DWORD('free'), b+4);
				Append(b, 8);

				m_fileWriter.Flush(); // add by Liaokz, 2013-8
			}
        }
    }
	//}


	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE; // Invalidate the file 
}


void 
MuxOutput::SetMovieWriter(MovieWriter * pMovie)
{
	m_pMovie = pMovie;
}

// ---- seeking support ------------------------------------------------

SeekingAggregator::SeekingAggregator(CBaseFilter* pFilter, bool bSetTimeFormat)
: m_bSetTimeFormat(bSetTimeFormat)
{
    // collect all pins that support seeking.
    // if bSet is true, collect only those that
    // allow SetTimeFormat -- this will be one per splitter
    for (int i = 0; i < pFilter->GetPinCount(); i++)
    {
        CBasePin* pPin = pFilter->GetPin(i);
        PIN_DIRECTION pindir;
        pPin->QueryDirection(&pindir);
        if (pindir == PINDIR_INPUT)
        {
            IMediaSeekingPtr pSeek = pPin->GetConnected();
            if (pSeek != NULL)
            {
                HRESULT hr = S_OK;
                if (m_bSetTimeFormat)
                {
                    hr = pSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
                }
                if (SUCCEEDED(hr))
                {
                    m_Pins.push_back(pSeek.Detach());
                }
            }
        }
    }
}

SeekingAggregator::~SeekingAggregator()
{
    for (iterator i = Begin(); i != End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        // undo the SetTimeFormat call
        if (m_bSetTimeFormat)
        {
            pSeek->SetTimeFormat(&TIME_FORMAT_NONE);
        }
        pSeek->Release();
    }
}

STDMETHODIMP 
Mpeg4Mux::GetCurrentPosition(LONGLONG *pCurrent)
{
    if (m_pMovie == NULL)
    {
        // return previous total (after Stop)
        *pCurrent = m_tWritten;
    } else {
        // this is not passed upstream -- we report the 
        // position of the mux. Report the earliest write
        // time of any pin
        REFERENCE_TIME tCur = m_pMovie->CurrentPosition();
        *pCurrent = tCur;
    }

    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::GetCapabilities(DWORD * pCapabilities )
{
    // OR together all the pins' capabilities, together with our own
    DWORD caps = AM_SEEKING_CanGetCurrentPos;
    SeekingAggregator pins(this);
    for (SeekingAggregator::iterator i = pins.Begin(); i != pins.End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        DWORD dwThis;
        HRESULT hr = pSeek->GetCapabilities(&dwThis);
        if (SUCCEEDED(hr))
        {
            caps |= dwThis;
        }
    }
    *pCapabilities = caps;
    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::CheckCapabilities(DWORD * pCapabilities )
{
    DWORD dwActual;
    GetCapabilities(&dwActual);
    if (*pCapabilities & (~dwActual)) {
        return S_FALSE;
    }
    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::IsFormatSupported(const GUID * pFormat)
{
    if (*pFormat == TIME_FORMAT_MEDIA_TIME)
    {
        return S_OK;
    }
    return S_FALSE;
}

STDMETHODIMP 
Mpeg4Mux::QueryPreferredFormat(GUID * pFormat)
{
    *pFormat = TIME_FORMAT_MEDIA_TIME;
    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::GetTimeFormat(GUID *pFormat)
{
    *pFormat = TIME_FORMAT_MEDIA_TIME;
    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::IsUsingTimeFormat(const GUID * pFormat)
{
    if (*pFormat == TIME_FORMAT_MEDIA_TIME)
    {
        return S_OK;
    }
    return S_FALSE;
}

STDMETHODIMP 
Mpeg4Mux::SetTimeFormat(const GUID * pFormat)
{
    if ((*pFormat == TIME_FORMAT_MEDIA_TIME) ||
        (*pFormat == TIME_FORMAT_NONE))
    {
        return S_OK;
    }
    return VFW_E_NO_TIME_FORMAT;
}

STDMETHODIMP 
Mpeg4Mux::GetDuration(LONGLONG *pDuration)
{
    // find the longest of all input durations
    SeekingAggregator pins(this);
    REFERENCE_TIME tMax = 0;
    for (SeekingAggregator::iterator i = pins.Begin(); i != pins.End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        REFERENCE_TIME t;
        pSeek->GetDuration(&t);
        if (t > tMax)
        {
            tMax = t;
        }
    }
    *pDuration = tMax;
    return S_OK;
}

STDMETHODIMP 
Mpeg4Mux::GetStopPosition(LONGLONG *pStop)
{
    // return the first valid entry
    SeekingAggregator pins(this);
    if (pins.Begin() == pins.End())
    {
        return E_NOINTERFACE;
    }
    IMediaSeekingPtr pSeek = *pins.Begin();
    return pSeek->GetStopPosition(pStop);
}

STDMETHODIMP 
Mpeg4Mux::ConvertTimeFormat(LONGLONG * pTarget, const GUID * pTargetFormat,
                          LONGLONG    Source, const GUID * pSourceFormat )
{
    if (((pTargetFormat == 0) || (*pTargetFormat == TIME_FORMAT_MEDIA_TIME)) &&
        ((pSourceFormat == 0) || (*pSourceFormat == TIME_FORMAT_MEDIA_TIME)))
    {
        *pTarget = Source;
        return S_OK;
    }
    return VFW_E_NO_TIME_FORMAT;
}

STDMETHODIMP 
Mpeg4Mux::SetPositions(LONGLONG * pCurrent, DWORD dwCurrentFlags
        , LONGLONG * pStop, DWORD dwStopFlags )
{
    // must be passed to all valid inputs -- all must succeed
    SeekingAggregator pins(this, true);
    HRESULT hr = S_OK;
    for (SeekingAggregator::iterator i = pins.Begin(); i != pins.End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        HRESULT hrThis = pSeek->SetPositions(pCurrent, dwCurrentFlags, pStop, dwStopFlags);
        if (FAILED(hrThis) && SUCCEEDED(hr))
        {
            hr = hrThis;
        }
    }
    return hr;
}

STDMETHODIMP 
Mpeg4Mux::GetPositions(LONGLONG * pCurrent,
                          LONGLONG * pStop )
{
    // return first valid input
    SeekingAggregator pins(this);
    if (pins.Begin() == pins.End())
    {
        return E_NOINTERFACE;
    }
    IMediaSeekingPtr pSeek = *pins.Begin();
    return pSeek->GetPositions(pCurrent, pStop);
}

STDMETHODIMP 
Mpeg4Mux::GetAvailable(LONGLONG * pEarliest, LONGLONG * pLatest )
{
    // the available section is the area for which any
    // data is available -- and here it is not very important whether
    // it is actually available
    *pEarliest = 0;
    return GetDuration(pLatest);
}

STDMETHODIMP 
Mpeg4Mux::SetRate(double dRate)
{
    // must be passed to all valid inputs -- all must succeed
    SeekingAggregator pins(this, true);
    HRESULT hr = S_OK;
    for (SeekingAggregator::iterator i = pins.Begin(); i != pins.End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        HRESULT hrThis = pSeek->SetRate(dRate);
        if (FAILED(hrThis) && SUCCEEDED(hr))
        {
            hr = hrThis;
        }
    }
    return hr;
}

STDMETHODIMP 
Mpeg4Mux::GetRate(double * pdRate)
{
    // return first valid input
    SeekingAggregator pins(this);
    if (pins.Begin() == pins.End())
    {
        return E_NOINTERFACE;
    }
    IMediaSeekingPtr pSeek = *pins.Begin();
    return pSeek->GetRate(pdRate);
}

STDMETHODIMP 
Mpeg4Mux::GetPreroll(LONGLONG * pllPreroll)
{
    // preroll time needed is the longest of any
    SeekingAggregator pins(this);
    REFERENCE_TIME tMax = 0;
    for (SeekingAggregator::iterator i = pins.Begin(); i != pins.End(); i++)
    {
        IMediaSeekingPtr pSeek = *i;
        REFERENCE_TIME t;
        HRESULT hr = pSeek->GetPreroll(&t);

        if ((hr == S_OK) && (t > tMax))
        {
            tMax = t;
        }
    }
    *pllPreroll = tMax;
    return S_OK;
}

bool Mpeg4Mux::CreateMovie()
{
	CAutoLock lock(&m_csMovie);

	// We create new movie by first creating MuxOutput in case CreateFile failed.
	// If so, we return false and do not infect normal running.
	// --Liaokz, 2013-5

    EpisodeNodePointer episode(new EpisodeNode);
    episode->nEpisode = m_nEpisode;
    //episode->llStartTime = GetDShowTimeFromUnixTime(NULL); //在OnReceiveSample中设置
    episode->nJobId = m_nNextJobId;
    if (m_Episodes.size() && m_Episodes[0]->nJobId == m_nNextJobId) {
        LOG((TEXT("warning: Recording episode using same job_id=%d."), m_nNextJobId));
    }

    //file name
	PrintFileName(m_szFileTemplateName, episode->szFileNames, m_nEpisode, NULL, 0);
	_tcscat_s(episode->szFileNames, MP4_TMP_SUFFIX);

	HRESULT hr = S_OK;
	MuxOutput * pOutput = new MuxOutput(this, episode->szFileNames, &hr);
	if (FAILED(hr))
	{
		delete pOutput;
		return false;
	}

    ASSERT(m_Episodes.size() <= 3);
	while (m_Episodes.size() >= 2)
	{
        EpisodeNodePointer last = m_Episodes.back();
        if (last->pMovie)
        {
            LOG((TEXT("Last movie %s not closed yet."), last->szFileNames));
            last->pMovie->Stop();
            CloseMovie(last);
            break; //每次只关闭最后一个未关闭的分段，删除则等成功关闭后下次到这里时执行else处理
        } 
        else
        { //pMovie==NULL，说明成功停止了
            CloseHandle(last->hCloseProc);
            last->hCloseProc = NULL;
            m_Episodes.pop_back(); //DO NOT access 'last' after
        }
	}

	episode->pOutput = m_pOutput = pOutput;
	m_pOutput->Reset();
	episode->pMovie = m_pMovie = new MovieWriter(m_pOutput);
    m_pOutput->SetMovieWriter(m_pMovie);

    //要在下面调用MakeTrack前插入哦
    m_Episodes.insert(m_Episodes.begin(), episode);

	// Recover Tracks
	// 注意：如果是首次启动，则Episode记录为空，因为MuxInput::Active尚未调用。故以下语句不会执行，不影响原程序逻辑。
	// 在后来的分段视频中，Map中也仅记录了有效的muxInput信息。
	// By Liaokz, 2013-5-4
    if (m_Episodes.size() > 1) {
        TrackStatusMap &tracksMap = m_Episodes[LastIdx]->pTracks;
        for (unsigned int i = 0; i < m_pInputs.size(); ++i)
        {
            int InputIndex = m_pInputs[i]->GetIndex();
            if (tracksMap.find(InputIndex) != tracksMap.end())
            {
                MakeTrack(InputIndex, tracksMap[InputIndex].pmt);
            }
        }
    }

	Notifier::getInstance()->FileCreate(episode->nJobId, episode->szFileNames);

	++m_nEpisode; //更新下一集计数
    m_llLastNotifiedSize = 0; //重置当前文件大小

	return true;
}

// Must called after calling the MovieWriter::Stop to ensure the MovieWriter::CheckQueue stopped.
// --By Liaokz, 2013-5-4
bool Mpeg4Mux::CloseMovie(EpisodeNodePointer pEpisode)
{
    CAutoLock lock(&m_csMovie); //一次只能一个线程访问

	if (pEpisode && pEpisode->pMovie && !pEpisode->hCloseProc)
	{ //movie未关闭且关闭线程没有启动

		CloseMovieParams * params = new CloseMovieParams;
		params->pEpisode = pEpisode.get();
		params->pMux = this;

		ASSERT(!pEpisode->hCloseProc);

		// Movie结束时，涉及统计和磁盘写入，IO导致线程阻塞，为不影响接收数据流，此处另开线程 --Liaokz, 2013-5
		pEpisode->hCloseProc = (HANDLE)_beginthreadex(NULL, 0, _CloseMovieProc, (void *)params, 0, NULL);
	}

	return true;
}

unsigned Mpeg4Mux::_CloseMovieProc(void * params)
{
	CloseMovieParams * p = (CloseMovieParams *)params;
    EpisodeNode *episode = p->pEpisode;

	LPCTSTR szFile = episode->szFileNames;
	LOG((TEXT("Closing movie file %s"), szFile));

    // step1: stop movie writer and close the file
	// write all queued data
	episode->pMovie->WriteOnStop();

	// write all metadata, return S_OK means video received data, S_FALSE means the video is empty
    REFERENCE_TIME tDur = 0;
	HRESULT hr = episode->pMovie->Close(&tDur);
    episode->tDuration = tDur;
    p->pMux->m_tWritten += tDur;

	// fill remaining file space
	episode->pOutput->FillSpace();

	// step2: we need to gather media format info for report
    TypeHandler *pVideoHandler = NULL;
    const CMediaType *pAudioType = NULL;
	for(auto track : episode->pTracks)
	{
		if (track.second.pmt == NULL) continue;

		if (track.second.pTrack->IsVideo()) {
            pVideoHandler = track.second.pTrack->Handler();
        }
        else if (track.second.pTrack->IsAudio()) {
            pAudioType = track.second.pmt;
        }
	}

    // prepare new file name from .tmp to .mp4
	TCHAR szFileFinal[MAX_PATH] = {0};
  _tcscpy_s(szFileFinal, szFile);
	szFileFinal[_tcslen(szFileFinal) - _tcslen(MP4_TMP_SUFFIX)] = '\0';  // delete .tmp suffix
	PrintFileName(szFileFinal, szFileFinal, 0, pVideoHandler, episode->llStartTime);

    // step3: report recording result and other work
	// Notify the graph, the 1st param used to identify the filter when graph contains more than one mux
	if (hr == S_OK)
	{
	    DeleteFile(szFileFinal); // Delete file if exist
        _trename(szFile, szFileFinal); // rename the file
        LOG((TEXT("File complete: %s"), szFileFinal));

        Notifier::MediaFormat fmt = {0};
        fmt.start_time = episode->llStartTime / 10000; // dshow ts to ms
        fmt.duration = tDur / 10000; // dshow ts to ms
        if (pVideoHandler) {
            fmt.video_width = pVideoHandler->Width();
            fmt.video_height = pVideoHandler->Height();
            fmt.video_fps = (int)(UNITS / pVideoHandler->FrameDuration());
        }
        if (pAudioType) {
            WAVEFORMATEX* pwfx = (WAVEFORMATEX*)pAudioType->Format();
            fmt.audio_sample_rate = pwfx->nSamplesPerSec;
            fmt.audio_channel = pwfx->nChannels;
            fmt.audio_bits_count = pwfx->wBitsPerSample;
        }
	    Notifier::getInstance()->FileClose(episode->nJobId, szFileFinal, &fmt);
	}
	else // empty video
	{
        DeleteFile(szFile);
        LOG((TEXT("File empty, deleted: %s"), szFile));
		Notifier::getInstance()->FileAbandon(episode->nJobId, szFile);
	}

    //回收资源，标记所有对象可安全删除了
    pVideoHandler = NULL;
    pAudioType = NULL;
	delete episode->pMovie;
    episode->pMovie = NULL;
    delete episode->pOutput;
    episode->pOutput = NULL;
    delete p;

	return 0;
}

HRESULT Mpeg4Mux::SetFileName( __in LPCTSTR szName )
{
	CAutoLock lock(&m_csMovie);
	_tcscpy_s(m_szFileTemplateName, sizeof(m_szFileTemplateName)/sizeof(TCHAR), szName);
	return S_OK;
}

HRESULT Mpeg4Mux::GetFileName( __out_opt LPTSTR szName, __inout UINT &nLength )
{
	if (NULL == szName)
	{
		nLength = _tcslen(m_szFileTemplateName);
	}
	else
	{
		_tcscpy_s(szName, nLength, m_szFileTemplateName);
	}
	return S_OK;
}

HRESULT Mpeg4Mux::CutMovieFile(__in long nJobId, __in_opt LPCTSTR szFileName)
{
	CAutoLock lock(&m_csMovie);
	if (szFileName && _tcslen(szFileName) > 0)
	{
		_tcscpy_s(m_szFileTemplateName, sizeof(m_szFileTemplateName)/sizeof(TCHAR), szFileName);
	}
    m_nNextJobId = nJobId;
	m_bCutByUser = true;

	Notifier::getInstance()->FileCutRequest(m_Episodes[CurIdx]->szFileNames, szFileName);
	return S_OK;
}

HRESULT Mpeg4Mux::SetCallback( __in void * obj, __in RecordStatCallback func )
{
    Notifier::getInstance()->m_pCallbackCaller = obj;
    Notifier::getInstance()->m_pCallbackFunc = func;
    return S_OK;
}

void Mpeg4Mux::NotifyFileSizeChange( TCHAR *szFile, LONGLONG llSize )
{
    //仅报告当前录制的文件，前一个文件的大小忽略
    if (_tcscmp(szFile, m_Episodes[CurIdx]->szFileNames) == 0) {
        m_llLastNotifiedSize = llSize;
    } 
    /*else {
        LOG_PRINT("Ignore previous file size change");
    }*/
}

void Mpeg4Mux::NotifySampleTime( REFERENCE_TIME rtSample )
{
    if (m_rtLastNotifiedTimestamp >= rtSample) {
        return;
    }
    REFERENCE_TIME rtDiff = rtSample - m_rtLastNotifiedTimestamp;
    if (m_rtEpisodeStart == -1 || rtDiff < 10000000) {
        //每隔至少1秒才报告一次
        return;
    }
    m_rtLastNotifiedTimestamp = rtSample;
    LONGLONG llDur = m_rtLastNotifiedTimestamp - m_rtEpisodeStart;
    //这里用printf，因为LOG_PRINTF末尾有换行
    printf("[MP4Mux] Recording: %I64ds, %.1fMB\r", 
      llDur / 10000000, m_llLastNotifiedSize / 1000000.0);
    //此处将100ns精度转为ms精度
    Notifier::getInstance()->FileWriting(m_Episodes[CurIdx]->nJobId,
        m_Episodes[CurIdx]->szFileNames,
        llDur / 10000, m_llLastNotifiedSize);
}

Mpeg4Mux::EpisodeNode::EpisodeNode()
{
    pOutput = NULL;
    pMovie = NULL;
    nEpisode = -1;
    nJobId = 0;
    memset(szFileNames, 0, sizeof(szFileNames));
    llStartTime = 0;
    tDuration = 0;
    hCloseProc = NULL;
}
