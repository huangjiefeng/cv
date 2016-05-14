//------------------------------------------------------------------------------
// File: rtp_sender_filter.cpp
//
// Desc: Implementation of a renderer that send the samples it receives via
//       rtp. Developed base on microsoft directshow dump filter sample.
//
// By  : Liaokz, 2014-05. liaokz@gmail.com
//
// Copyright (c) SCUT.  All rights reserved.
//------------------------------------------------------------------------------


#pragma warning(disable:4995)


#include <windows.h>
#include <initguid.h>

#include "guids.h"
#include "Core/core_defs.h"
#include "rtp_sender_filter.h"
#include "Core/h264_frame_splitter.h"
#include "Core/aac_frame_splitter.h"
#include "Core/audio_frame_splitter.h"
#include "Core/ortp_sender.h"


// Util

std::string MakeConnParamKey(const char *ip, unsigned short port)
{
  char buf[128] = {0};
  sprintf_s(buf, sizeof(buf), "%s:%d", ip, port);
  return buf;
}


// Constructor

CRtpSenderFilter::CRtpSenderFilter(LPUNKNOWN pUnk, HRESULT *phr)
  : CBaseRenderer(CLSID_SCUTRtpSender, g_szSCUTRtpSenderFilter, pUnk, phr) 
  , input_pin_(nullptr)
  , payload_type_(kPayloadUnknown)
  , allow_all_send_(true)
{
  logger_ = CreateLogger();
}

CRtpSenderFilter::~CRtpSenderFilter()
{
  ResetComponents();
}


CUnknown * CRtpSenderFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	ASSERT(phr);

	CRtpSenderFilter *pNewObject = new CRtpSenderFilter(punk, phr);
	if (pNewObject == nullptr) {
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;
}


HRESULT CRtpSenderFilter::CheckMediaType( const CMediaType * pMediaType )
{
  const GUID *type = pMediaType->Type();
  if (*type == MEDIATYPE_Video || *type == MEDIATYPE_Audio) {
    return S_OK;
  }
  return E_FAIL;
}

HRESULT CRtpSenderFilter::DoRenderSample( IMediaSample *pMediaSample )
{
  if (kPayloadUnknown == payload_type_) {
    return S_OK; //返回成功以便下次尝试，否则Graph会启动失败
  }
  
  BYTE *data = nullptr;
  long data_length = 0;
  REFERENCE_TIME start_time, end_time;
  pMediaSample->GetPointer(&data);
  data_length = pMediaSample->GetActualDataLength();
  pMediaSample->GetTime(&start_time, &end_time);

  IFrameUnitsPtr frame_units;
  if (splitter_) {
    frame_units = splitter_->Split(data, data_length, start_time);
  }

  if (frame_units && allow_all_send_) {
    CAutoLock lock(&senders_lock_); // lock first
    for (auto sender_iter : senders_) {
      if (sender_iter.second.sender && sender_iter.second.allow_send) 
        sender_iter.second.sender->Send(frame_units);
    }
  }

  return S_OK;
}

HRESULT CRtpSenderFilter::InitializeComponent()
{
  ResetComponents();
  if (!m_pInputPin || !m_pInputPin->IsConnected()) {
    return E_FAIL;
  }

  CLSID sub_type;
  CMediaType mt;
  if (FAILED(m_pInputPin->ConnectionMediaType(&mt))) {
    return E_FAIL;
  }
  sub_type = mt.subtype;
  //FreeMediaType(mt);

  if (sub_type == MEDIASUBTYPE_H264) {
    payload_type_ = kPayloadH264;
    logger_->SetModuleName("rtp-avc");
    splitter_.reset(new H264FrameSplitter(logger_));

  } else if (sub_type == MEDIASUBTYPE_AAC) {
    payload_type_ = kPayloadAAC;
    logger_->SetModuleName("rtp-aac");
    splitter_.reset(new AACFrameSplitter(mt, logger_));

  } else if (sub_type == MEDIASUBTYPE_SPEEX) {
    payload_type_ = kPayloadSpeex;
    logger_->SetModuleName("rtp-spx");
    splitter_.reset(new AudioFrameSplitter(mt, logger_));

  } else {
    payload_type_ = kPayloadUnknown;
    //filter必须连接了才能添加发送目标
    LOG_ERROR_EX(logger_, "Init failed, filter not connected or format not supported");
    return E_FAIL;
  }

  return S_OK;
}

void CRtpSenderFilter::ResetComponents()
{
  splitter_.reset();
  senders_.clear();
  payload_type_ = kPayloadUnknown;
}

//
// Stop
//
// Overriden to close the dump file
//
STDMETHODIMP CRtpSenderFilter::Stop()
{
  CAutoLock cObjectLock(m_pLock);
  HRESULT hr = CBaseRenderer::Stop();
  if (FAILED(hr)) return hr;

  ResetComponents();
    
  return NOERROR;
}

//
// Run
//
STDMETHODIMP CRtpSenderFilter::Run(REFERENCE_TIME tStart)
{
  CAutoLock cObjectLock(m_pLock);
  if (payload_type_ == kPayloadUnknown) {
    HRESULT hr = InitializeComponent();
    if (FAILED(hr)) return hr;
  }

  return CBaseRenderer::Run(tStart);
}


STDMETHODIMP CRtpSenderFilter::NonDelegatingQueryInterface( REFIID riid, void **ppv )
{
	CheckPointer( ppv , E_POINTER );

	if (riid == IID_ISCUTRtpSender)
	{
		return GetInterface((ISCUTRtpSender *)(this), ppv);
	}

	return CBaseRenderer::NonDelegatingQueryInterface(riid, ppv);

}

HRESULT CRtpSenderFilter::AddConnection( const ConnectionParam &param )
{
  if (param.port == 0) {
    return S_OK;
  }
  if (payload_type_ == kPayloadUnknown) {
    HRESULT hr = InitializeComponent();
    if (FAILED(hr)) {
      return hr;
    }
  }
  {
    CAutoLock lock(&senders_lock_);
    std::string key = MakeConnParamKey(param.ip_addr, param.port);
    senders_[key].ip = param.ip_addr;
    senders_[key].port = param.port;
    senders_[key].allow_send = true;
    senders_[key].sender.reset(new ORtpSender);
    senders_[key].sender->InitSession(param.ip_addr, param.port, payload_type_);
  }
  LOG_INFO_EX(logger_, "Connection [%s]:%d added", param.ip_addr, param.port);

  return S_OK;
}

HRESULT CRtpSenderFilter::RemoveConnection( const char *ip, unsigned short port )
{
  CAutoLock lock(&senders_lock_);
  std::string key = MakeConnParamKey(ip, port);
  senders_.erase(key);
  LOG_INFO_EX(logger_, "Connection [%s]:%d removed", ip, port);
  return S_OK;
}

HRESULT CRtpSenderFilter::RemoveAllConnection()
{
  CAutoLock lock(&senders_lock_);
  senders_.clear();
  LOG_INFO_EX(logger_, "All connection removed");
  return S_OK;
}

unsigned long CRtpSenderFilter::ConnectionCount()
{
  return senders_.size();
}

HRESULT CRtpSenderFilter::ControlSending( const char *ip, unsigned short port, bool send )
{
  CAutoLock lock(&senders_lock_);
  std::string key = MakeConnParamKey(ip, port);
  if (senders_.find(key) == senders_.end()) {
    LOG_ERROR_EX(logger_, "Control sending failed, target [%s]:%d not exist",
      ip, port);
    return E_INVALIDARG;
  }
  senders_[key].allow_send = send;
  return S_OK;
}

HRESULT CRtpSenderFilter::ControlAllSending( bool send )
{
  allow_all_send_ = send;
  return S_OK;
}
