//------------------------------------------------------------------------------
// File: rtp_receiver_filter.h
//
// Desc: Definition for RtpReceiver Filter. 
//       Base on DirectShow sample code - In-memory push mode source filter
//
// By  : Liaokz, 2014-05. liaokz@gmail.com
//
// Copyright (c) SCUT.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef __RTP_RECEIVER_FILTER_H
#define __RTP_RECEIVER_FILTER_H

#include <string>
#include <list>
#include "streams.h"

#include "rtp_receiver_config_interface.h"
#include "Core/receive_interfaces.h"
#include "Core/receive_controller.h"
#include "Log/Log.h"

// Filter name strings
#define g_wszSCUTRtpReceiverFilter L"SCUT Rtp Receiver"
#define g_szSCUTRtpReceiverFilter "SCUT Rtp Receiver"

class CRtpReceiverFilter;
class ReceiveController;

/**********************************************
 *
 *  Class declarations
 *
 **********************************************/

class CRtpReceiverFilterPushPin : public CBaseOutputPin
{

	friend class CRtpReceiverFilter;

public:

  CRtpReceiverFilterPushPin(HRESULT *phr, CRtpReceiverFilter *pFilter, LPCWSTR pName);
  ~CRtpReceiverFilterPushPin();

	DECLARE_IUNKNOWN;

  // Override the version that offers exactly one media type
  HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest);
    
  // Set the agreed media type and set up the necessary parameters
  HRESULT SetMediaType(const CMediaType *pMediaType);
  // Support display formats
  HRESULT CheckMediaType(const CMediaType *pMediaType);
  HRESULT GetMediaType(int iPosition, CMediaType *pmt);

  // 质量控制,暂时不需要
  STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q)
  {
    return E_FAIL;
  }

  //Impl of IFrameDeliverer
  int DeliverFrame(char *data, uint32_t timestamp);

private:
  CRtpReceiverFilter *filter_;

};


class CRtpReceiverFilter : public CBaseFilter
  , public ISCUTRtpReceiver
  , public IFrameDeliverer
{

	friend class CRtpReceiverFilterPushPin;

public:
  static CUnknown * WINAPI CreateInstance(IUnknown *pUnk, HRESULT *phr);

  DECLARE_IUNKNOWN;

  STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

  STDMETHODIMP Run(REFERENCE_TIME tStart);
  STDMETHODIMP Stop();

  STDMETHODIMP GetState(DWORD dwMSecs, FILTER_STATE *State);
  
  virtual int GetPinCount();
  virtual CBasePin *GetPin(int n);

  //Impl of ISCUTRtpReceiver
  //Set id
  virtual HRESULT SetId(const char * id);
  virtual const char *GetId();
  //Set receiving port
  virtual HRESULT SetLocalAddr(const char *ip, unsigned short port);
  virtual unsigned int GetSourcePort();
  //Set callback, use null to clear callback
  virtual HRESULT SetCallback(void *callback_obj, RtpStatCallback callback_func);
  //Set media type
  virtual HRESULT SetMediaType(const CMediaType &type);
  virtual HRESULT GetMediaType(CMediaType &type);
  //Start or Stop receive
  virtual HRESULT ControlRecv(bool recv);

  //Impl of IFrameDeliverer
  virtual int AddFrame(IFrame *frame);
  virtual int DeliverFrame();
  virtual IDeliverStrategy *GetStrategy() { return strategy_; }
  virtual void SSRCChanged();

private:
  // Constructor is private because you have to use CreateInstance
  CRtpReceiverFilter(IUnknown *pUnk, HRESULT *phr);
  ~CRtpReceiverFilter();

  int InitializeComponents();
  void ResetComponents();

private:
  CRtpReceiverFilterPushPin *output_pin_;
  CCritSec filter_lock_;

  //logger
  ILogger *logger_;

  //相关设置
  std::string id_; //rtp标识
  std::string source_ip_; //本地监听地址，用于区分是IPv4、IPv6、组播
  unsigned int source_port_; //本地监听端口
  RtpCallbackInfo rtp_callback_;
  CMediaType current_media_type_; //设置的音频或视频格式

  //组件
  ReceiveController *receive_controller_;
  IRtpReceiver *receiver_;
  IFrameReassembler *reassembler_;
  IDeliverStrategy *strategy_;

  typedef std::list<IFrame *> FrameList;
  FrameList frame_list_;
  CCritSec frame_list_lock_;
  unsigned short max_frame_list_size_;
  unsigned int frame_discarded_;
  IMediaSample *sample_to_deliver_;
  long long filter_start_wallclock_;

};


#endif
