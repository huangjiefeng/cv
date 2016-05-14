//------------------------------------------------------------------------------
// File: rtp_sender_filter.h
//
// Desc: Definitions for RtpSender Filter.
//
// By  : Liaokz, 2014-05. liaokz@gmail.com
//
// Copyright (c) SCUT.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef __RTP_SENDER_FILTER_H
#define __RTP_SENDER_FILTER_H

#include "streams.h"

#include "rtp_sender_config_interface.h"
#include "Core/send_interfaces.h"
#include "Core/core_defs.h"
#include "log/log.h"

#include <map>

// Filter name strings
#define g_wszSCUTRtpSenderFilter L"SCUT Rtp Sender"
#define g_szSCUTRtpSenderFilter "SCUT Rtp Sender"


class CRtpSenderFilterInputPin;
class CRtpSenderFilter;


// Main filter object

class CRtpSenderFilter : public CBaseRenderer, public ISCUTRtpSender
{

	friend class CRtpSenderFilterInputPin;

public:

  // Constructor
  CRtpSenderFilter(LPUNKNOWN pUnk, HRESULT *phr);
	~CRtpSenderFilter();

	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	DECLARE_IUNKNOWN;
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

  //MUST override these function
  HRESULT DoRenderSample(IMediaSample *pMediaSample);
  HRESULT CheckMediaType(const CMediaType * pMediaType);

  // Open and close the file as necessary
  STDMETHODIMP Run(REFERENCE_TIME tStart);
  // STDMETHODIMP Pause();
  STDMETHODIMP Stop();

  // Impl of ISCUTRtpSender
  virtual HRESULT AddConnection( const ConnectionParam &param );
  virtual HRESULT RemoveConnection( const char *ip, unsigned short port );
  virtual HRESULT RemoveAllConnection();
  virtual unsigned long ConnectionCount();
  virtual HRESULT ControlSending( const char *ip, unsigned short port, bool send );
  virtual HRESULT ControlAllSending(bool send);


private:
  HRESULT InitializeComponent();
  void ResetComponents();

private:

  CRtpSenderFilterInputPin *input_pin_;
  RtpPayloadType payload_type_;

  typedef std::shared_ptr<IFrameSplitter> IFrameSplitterPtr;
  IFrameSplitterPtr splitter_;

  typedef std::shared_ptr<IRtpSender> IRtpSenderPtr;
  struct SenderInfo
  {
    std::string ip;
    unsigned short port;
    bool allow_send;
    IRtpSenderPtr sender;
  };
  typedef std::map<std::string, SenderInfo> SenderTable;
  SenderTable senders_;
  CCritSec senders_lock_;
  bool allow_all_send_; //全局发送开关

  ILogger *logger_;
};


#endif
