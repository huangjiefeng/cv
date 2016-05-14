//------------------------------------------------------------------------------
// File: rtp_receiver_filter.cpp
//
// Desc: Definition for RtpReceiver Filter. 
//       Base on DirectShow sample code - In-memory push mode source filter
//
// By  : Liaokz, 2014-05. liaokz@gmail.com
//
// Copyright (c) SCUT.  All rights reserved.
//------------------------------------------------------------------------------

#pragma warning(disable:4995)

#include <assert.h>

#include "rtp_receiver_filter.h"
#include "guids.h"
#include "Core/receive_controller.h"
#include "Core/ortp_receiver.h"
#include "Core/generic_frame_reassembler.h"
#include "Core/h264_frame_reassembler.h"
#include "Core/aac_frame_reassembler.h"
#include "Core/video_deliver_strategy.h"
#include "Core/audio_deliver_strategy.h"
#include "Util/classic_ptr_helper.h"
#include "Util/util.h"
#include "Core/media_sync.h"
#include "Log/Log.h"

/**********************************************
 *
 *  CPushPinDesktop Class
 *
 **********************************************/

CRtpReceiverFilterPushPin::CRtpReceiverFilterPushPin(
    HRESULT *phr, CRtpReceiverFilter *pFilter, LPCWSTR pName)
  : CBaseOutputPin(NAME(g_szSCUTRtpReceiverFilter), pFilter, &pFilter->filter_lock_, phr, pName)
  , filter_(pFilter)
{
}

CRtpReceiverFilterPushPin::~CRtpReceiverFilterPushPin()
{
}


//
// GetMediaType
//
//
HRESULT CRtpReceiverFilterPushPin::GetMediaType(int iPosition, CMediaType *pmt)
{
  CheckPointer(pmt,E_POINTER);
  CAutoLock cAutoLock(&filter_->filter_lock_);

  if(iPosition < 0)
      return E_INVALIDARG;

  // Have we run off the end of types?
  if(iPosition > 0)
      return VFW_S_NO_MORE_ITEMS;

  //pin中继承的m_mt成员为连接后的格式，而不是支持的格式，这里应返回支持的格式
  filter_->GetMediaType(*pmt);

  return NOERROR;

} // GetMediaType


//
// CheckMediaType
//
//
HRESULT CRtpReceiverFilterPushPin::CheckMediaType(const CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);

	CMediaType rtp_type;
  filter_->GetMediaType(rtp_type);
  if (!rtp_type.IsValid()) return E_INVALIDARG;

  if (*pMediaType != rtp_type) return E_INVALIDARG;

  return S_OK;  // This format is acceptable.

} // CheckMediaType


//
// DecideBufferSize
//
// This will always be called after the format has been sucessfully
// negotiated. So we have a look at m_mt to see what size image we agreed.
// Then we can ask for buffers of the correct size to contain them.
//
HRESULT CRtpReceiverFilterPushPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
  CheckPointer(pAlloc,E_POINTER);
  CheckPointer(pProperties,E_POINTER);

  CAutoLock cAutoLock(&filter_->filter_lock_);
  HRESULT hr = NOERROR;

	const int nBufCount = 5;
  pProperties->cBuffers = nBufCount;
	if (m_mt.formattype == FORMAT_WaveFormatEx)
	{
		WAVEFORMATEX * pFmt = (WAVEFORMATEX *)m_mt.pbFormat;
		pProperties->cbBuffer = pFmt->nAvgBytesPerSec;
	}
	else
	{
		pProperties->cbBuffer = m_mt.lSampleSize;
	}
	if (pProperties->cbBuffer < 100) // 如果无法计算得到缓存块准确大小，设置一个可能的最大值
	{
		pProperties->cbBuffer = 1920*1080*3; // big enough for RGB24 1080p pic
	}

  ASSERT(pProperties->cbBuffer);

  // Ask the allocator to reserve us some sample memory. NOTE: the function
  // can succeed (return NOERROR) but still not have allocated the
  // memory that we requested, so we must check we got whatever we wanted.
  ALLOCATOR_PROPERTIES Actual;
  hr = pAlloc->SetProperties(pProperties,&Actual);
  if(FAILED(hr))
  {
      return hr;
  }

  // Is this allocator unsuitable?
  if(Actual.cbBuffer < pProperties->cbBuffer)
  {
      return E_FAIL;
  }

  ASSERT(Actual.cBuffers >= nBufCount);
  return NOERROR;

} // DecideBufferSize


//
// SetMediaType
//
// Called when a media type is agreed between filters
//
//再次提示：该方法应该在连接后由graph调用，设置当前连接格式
HRESULT CRtpReceiverFilterPushPin::SetMediaType(const CMediaType *pMediaType)
{
	CAutoLock cAutoLock(&filter_->filter_lock_);

	if (FAILED(CheckMediaType(pMediaType))) return E_FAIL;

  // Pass the call up to my base class
  HRESULT hr = CBaseOutputPin::SetMediaType(pMediaType);

  return hr;

} // SetMediaType




/**********************************************
 *
 *  CPushSourceDesktop Class
 *
 **********************************************/

CRtpReceiverFilter::CRtpReceiverFilter(IUnknown *pUnk, HRESULT *phr)
  : CBaseFilter(NAME(g_szSCUTRtpReceiverFilter), pUnk, &filter_lock_, CLSID_SCUTRtpReceiver, phr)
  , output_pin_(nullptr)
  , logger_(nullptr)
  , receive_controller_(nullptr)
  , receiver_(nullptr)
  , reassembler_(nullptr)
  , strategy_(nullptr)
  , source_port_(0)
  , sample_to_deliver_(nullptr)
{
  // The pin magically adds itself to our pin array.
  output_pin_ = new CRtpReceiverFilterPushPin(phr, this, L"push");

	if (phr) {
		if (output_pin_ == nullptr)
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}

  //init members
  logger_ = CreateLogger();
  logger_->SetModuleName("rtp");

  rtp_callback_.obj = nullptr;
  rtp_callback_.func = nullptr;
}


CRtpReceiverFilter::~CRtpReceiverFilter()
{
  Stop();

	if (output_pin_) {
		delete output_pin_;
		output_pin_ = nullptr;
  }
}


CUnknown * WINAPI CRtpReceiverFilter::CreateInstance(IUnknown *pUnk, HRESULT *phr)
{
  CRtpReceiverFilter *pNewFilter = new CRtpReceiverFilter(pUnk, phr );

	if (phr)
	{
		if (pNewFilter == nullptr) 
			*phr = E_OUTOFMEMORY;
		else
			*phr = S_OK;
	}
  return pNewFilter;

}


STDMETHODIMP CRtpReceiverFilter::NonDelegatingQueryInterface( REFIID riid, void **ppv )
{
	CheckPointer( ppv , E_POINTER );

  if(riid == IID_ISCUTRtpReceiver)
  {
    return GetInterface((ISCUTRtpReceiver *) this, ppv);
  }

	return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CRtpReceiverFilter::Run( REFERENCE_TIME tStart )
{
  CAutoLock lock(&filter_lock_);
  filter_start_wallclock_ = GetDShowTimeFromUnixTime(nullptr);
  if (InitializeComponents() < 0) {
    return E_FAIL;
  }

  HRESULT hr = CBaseFilter::Run(tStart);
  if (FAILED(hr)) {
    return hr;
  }

  if (receive_controller_) {
    receive_controller_->Start();
  }

  return S_OK;
}

STDMETHODIMP CRtpReceiverFilter::Stop()
{
  CAutoLock lock(&filter_lock_);
  if (receive_controller_) {
    receive_controller_->Stop();
  }

  HRESULT hr = CBaseFilter::Stop();
  if (FAILED(hr))
  {
    return hr;
  }

  ResetComponents();
  //重置时间同步数据
  //不要在reset中做，因为filter运行期间可能会应上层要求暂停接收数据，
  //也会调用ResetComponents，这会导致同步数据重置
  MediaSync::Reset();

  return NOERROR;
}

STDMETHODIMP CRtpReceiverFilter::GetState(DWORD dwMSecs, __out FILTER_STATE *state)
{
  CheckPointer(state, E_POINTER);
  *state = m_State;
  if(m_State == State_Paused) {
    return VFW_S_CANT_CUE; //表示启动时不能阻塞在Pause状态，必须Run，否则在没有收到数据时Graph会卡死
  } 
  return S_OK;
}

int CRtpReceiverFilter::GetPinCount()
{
  return 1;
}

CBasePin * CRtpReceiverFilter::GetPin( int n )
{
  if (n == 0 && output_pin_) {
    return output_pin_;
  }
  return nullptr;
}

HRESULT CRtpReceiverFilter::ControlRecv( bool recv )
{
  CAutoLock lock(&filter_lock_);
  if (recv) {
    if (receive_controller_) {
       return S_OK;
    }
    if (InitializeComponents() < 0) {
      return E_FAIL;
    }
    receive_controller_->Start();
  } else {
    if (!receive_controller_) {
      return S_OK;
    }
    receive_controller_->Stop();
    ResetComponents();
  }
  return S_OK;
}

HRESULT CRtpReceiverFilter::SetId( const char * id )
{
  if (id) {
    id_ = id;
  } else {
    return E_INVALIDARG;
  }
  return S_OK;
}

const char * CRtpReceiverFilter::GetId()
{
  return id_.c_str();
}

HRESULT CRtpReceiverFilter::SetLocalAddr(const char *ip, unsigned short port)
{
  source_ip_ = ip;
  source_port_ = port;
  return S_OK;
}

unsigned int CRtpReceiverFilter::GetSourcePort()
{
  return source_port_;
}

HRESULT CRtpReceiverFilter::SetCallback( void *callback_obj, RtpStatCallback callback_func )
{
  rtp_callback_.obj = callback_obj;
  rtp_callback_.func = callback_func;
  return S_OK;
}

//设置媒体格式
//注意：这里设置的是RTP输出的格式，pin中继承的成员m_mt是连接后确定的格式，
//故不能直接通过pin的SetMediaType接口设置m_mt
HRESULT CRtpReceiverFilter::SetMediaType( const CMediaType &type )
{
  if (!type.IsValid() || (type.majortype != MEDIATYPE_Video && 
      type.majortype != MEDIATYPE_Audio)) {
    return E_INVALIDARG;
  }
  current_media_type_ = type;
  return S_OK;
}

HRESULT CRtpReceiverFilter::GetMediaType( CMediaType &type )
{
  type = current_media_type_;
  return S_OK;
}

int CRtpReceiverFilter::AddFrame( IFrame *frame )
{
  CAutoLock lock(&frame_list_lock_);
  assert(max_frame_list_size_ > 0);
  if (frame_list_.size() == max_frame_list_size_) {
    IFrame *frame_drop = frame_list_.front();
    LOG_WARN_EX(logger_, "frame buffer is full, discard frame rtp_ts=%u", 
      frame_drop->GetTimestamp());

    if (frame_drop) delete frame_drop;
    frame_list_.pop_front();

    //设置不连续，不确定队列中是否有下一帧，故要在strategy中记录不连续性
    strategy_->SetDiscontinuity();
  }

  frame_list_.push_back(frame);

  return 0;
}

int CRtpReceiverFilter::DeliverFrame()
{
  //看看有不有待推送的sample
  if (!sample_to_deliver_) {
    //没有就获取一个
    IFrame *frame = nullptr;
    ClassicPtrHelper<IFrame*> frame_helper(&frame);

    {
      //避免长时间持有锁，否则如果推送过程阻塞，接收线程也可能阻塞
      CAutoLock lock(&frame_list_lock_);
      while (!frame_list_.empty()) {
        frame = frame_list_.front();
        frame_list_.pop_front();

        if (strategy_->ShouldFrameDiscard(*frame)) {
          if (frame_discarded_ == 0) {
            //仅记录丢帧开始，避免日志过多
            LOG_WARN_EX(logger_, "frame discarded, rtp_ts=%u", frame->GetTimestamp());
          }
          delete frame;
          frame = nullptr;
          //因为不确定队列中是否有下一帧，故要在strategy中记录不连续性
          strategy_->SetDiscontinuity();
          ++frame_discarded_;
        } else {
          break;
        }
      }
    }
    //如果没有帧，则返回
    if (!frame) return 0;

    //不要无限尝试获取sample，尝试大约0.5秒
    int try_times = 500;
    do {
      output_pin_->GetDeliveryBuffer(&sample_to_deliver_, NULL, NULL, AM_GBF_NOWAIT);
      if (!sample_to_deliver_) Sleep(1);
    } while (!sample_to_deliver_ && --try_times > 0);

    if (!sample_to_deliver_) {
      LOG_ERROR_EX(logger_, "get deliver buffer failed, discard frame rtp_ts=%u", 
        frame->GetTimestamp());
      strategy_->SetDiscontinuity();
      ++frame_discarded_;
      return -1;
    }

    //将frame转换成IMediaSample
    if (strategy_->ConvertFrameToSample(*frame, *sample_to_deliver_) < 0) {
      LOG_ERROR_EX(logger_, "convert to dshow sample failed, discard frame rtp_ts=%u", 
        frame->GetTimestamp());
      strategy_->SetDiscontinuity();
      ++frame_discarded_;
      return -1;
    }
  }

  //终于从故障中恢复了，记录一下损失
  if (frame_discarded_ != 0) {
    LOG_WARN_EX(logger_, "%u frame discarded", frame_discarded_);
    frame_discarded_ = 0;
  }
  
  //前面保证了sample_to_deliver_不为空
  assert(sample_to_deliver_);
  if (strategy_->ShouldSampleDeliver(*sample_to_deliver_)) {
    output_pin_->Deliver(sample_to_deliver_);
    sample_to_deliver_->Release();
    sample_to_deliver_ = nullptr;
  }

  return 0;
}

int CRtpReceiverFilter::InitializeComponents()
{
  int ret = 0;
  ResetComponents();

  receiver_ = new ORtpReceiver(logger_);

  // receiver & reassembler & strategy
  if (current_media_type_.subtype == MEDIASUBTYPE_H264) {
    logger_->SetModuleName("rtp-avc");

    ret = receiver_->InitSession(source_ip_, source_port_, kPayloadH264);
    if (ret < 0) {
      LOG_ERROR_EX(logger_, "init rtp session failed, port: %d", source_port_);
      return -1;
    }

    reassembler_ = new H264FrameReassembler(logger_); //H264
    strategy_ = new VideoDeliverStrategy(this, current_media_type_, logger_);
    strategy_->SetDiscontinuity(); //初始时不要推送，直到收到第一个关键帧
  }
  else if (current_media_type_.subtype == MEDIASUBTYPE_AAC) {
    logger_->SetModuleName("rtp-aac");

    ret = receiver_->InitSession(source_ip_, source_port_, kPayloadAAC);
    if (ret < 0) {
      LOG_ERROR_EX(logger_, "init rtp session failed, port: %d", source_port_);
      return -1;
    }

    reassembler_ = new AACFrameReassembler(logger_); //AAC
    strategy_ = new AudioDeliverStrategy(this, current_media_type_, logger_);
  }
  else {
    if (current_media_type_.subtype == MEDIASUBTYPE_SPEEX) logger_->SetModuleName("rtp-spx");

    ret = receiver_->InitSession(source_ip_, source_port_, kPayloadSpeex);
    if (ret < 0) {
      LOG_ERROR_EX(logger_, "init rtp session failed, port: %d", source_port_);
      return -1;
    }

    reassembler_ = new GenericFrameReassembler(logger_); //Speex and Unknown
    strategy_ = new AudioDeliverStrategy(this, current_media_type_, logger_);
  }

  // init recv controller
  receive_controller_ = new ReceiveController(logger_);
  ret = receive_controller_->Initialize(receiver_, reassembler_, this);
  if (ret < 0) {
    LOG_ERROR_EX(logger_, "init receive controller failed");
    return -1;
  }
  receive_controller_->SetRtpCallback(rtp_callback_);

  //set stream time base
  if (strategy_) {
    strategy_->SetStreamTimeBase(filter_start_wallclock_);
  }

  // 帧缓存队列，视频可以多一点，而音频的尽量少，以减小延迟
  if (current_media_type_.majortype == MEDIATYPE_Video) {
    max_frame_list_size_ = 5;
  } else {
    max_frame_list_size_ = 3; //AAC解码需要至少3个，Speex2个就可以
  }

  frame_discarded_ = 0;

  return 0;
}

void CRtpReceiverFilter::ResetComponents()
{
  //注意先释放receive_controller_，再释放其他
  if (receive_controller_) {
    delete receive_controller_;
    receive_controller_ = nullptr;
  }
  if (receiver_) {
    delete receiver_;
    receiver_ = nullptr;
  }
  if (reassembler_) {
    delete reassembler_;
    reassembler_ = nullptr;
  }
  if (strategy_) {
    delete strategy_;
    strategy_ = nullptr;
  }

  //release frame
  while (!frame_list_.empty()) {
    delete frame_list_.front();
    frame_list_.pop_front();
  }
  if (sample_to_deliver_) {
    sample_to_deliver_->Release();
    sample_to_deliver_ = nullptr;
  }
}

void CRtpReceiverFilter::SSRCChanged()
{
  strategy_->ResetNTPReferenceTimestamp();
  frame_list_.clear();
  frame_discarded_ = 0;
}


