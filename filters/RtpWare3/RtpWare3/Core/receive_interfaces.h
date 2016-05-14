/************************************************************************/
/* 此处定义接收器各个模块的接口                                         */
/************************************************************************/

#ifndef RECEIVE_INTERFACES_H
#define RECEIVE_INTERFACES_H

#include "core_defs.h"
#include "RtpStat.h"
#include <string>

#define USE_SR_NTP

class IRtpPacket
{
public:
  virtual ~IRtpPacket() {}

  virtual bool GetMarkbit() = 0;
  virtual unsigned int GetSSRC() = 0;
  virtual unsigned int GetTimestamp() = 0;
  virtual unsigned short GetSeqNumber() = 0;
  virtual unsigned short GetPayloadType() = 0;
  virtual unsigned short GetPlayloadSize() = 0;
  virtual unsigned char *GetPayload() = 0;
};


class IFrame
{
public:
  virtual ~IFrame() {}

  virtual bool IsSyncPoint() = 0; //是否同步点(关键帧)
  virtual void SetSyncPoint(bool) = 0;
  virtual bool IsDiscontinuity() = 0;
  virtual void SetDiscontinuity(bool) = 0;
  virtual unsigned long GetTimestamp() = 0;
  virtual void SetTimestamp(unsigned long) = 0;
  virtual unsigned long GetDataSize() = 0;
  virtual unsigned char *GetData() = 0;
  virtual void SetData(unsigned char *, unsigned long) = 0;
};


//
//rtp接收器，目前只有ortp实现
//
class IRtpReceiver
{
public:
  virtual ~IRtpReceiver() {}

  virtual int InitSession(std::string ip, unsigned short port, RtpPayloadType payload_type) = 0;
  virtual int DestroySession() = 0;
  virtual void ResetSession() = 0;

  virtual int Receive(IRtpPacket *&packet) = 0;
  virtual int GetStatistics(RtpStatItem &) = 0;
  virtual void GetNTPReferenceTimestamp(unsigned long long &ntp_ts, unsigned long &ts) = 0;

};


//
//组帧器，根据不同媒体类型，可能需要不同的组帧策略
//
class IFrameReassembler
{
public:
  virtual ~IFrameReassembler() {}

  //添加一个rtp包到队列，返回值>0应调用PopFrontFrame检查是否有完整帧，<0表示出错
  virtual int PushRtpPacket(IRtpPacket *packet) = 0;
  //返回最靠前的帧，不存在完整帧时返回nullptr
  virtual IFrame *PopFrontFrame() = 0;

public:
  static const int PUSH_ERR = -1;
  static const int PUSH_CHK_FRAME = 1;
  static const int PUSH_SSRC_CHANGE = 2;
  static const int PUSH_PACKET_DROP = 4;
};


//
//推送策略器，组合于IFrameDeliverer
//
struct IMediaSample;

class IDeliverStrategy
{
public:
  virtual ~IDeliverStrategy() {}

  virtual bool ShouldFrameDiscard(IFrame &frame) = 0;
  virtual int ConvertFrameToSample(IFrame &frame, IMediaSample &) = 0;
  virtual bool ShouldSampleDeliver(IMediaSample &) = 0;
  virtual void SetDiscontinuity() = 0;
  //设置RTP时间戳与NTP时间对应关系
  virtual void SetNTPReferenceTimestamp(unsigned long long ntp_ts, unsigned long ts) = 0;
  virtual void ResetNTPReferenceTimestamp() = 0;
  //设置流时间其实时刻对应的绝对时间
  virtual void SetStreamTimeBase(long long t) = 0;
};


//
//Sample推送器，一般由filter实现该接口，因为涉及DirectShow具体内容
//
class IFrameDeliverer
{
public:
  virtual ~IFrameDeliverer() {}

  //将Frame放入推送队列
  virtual int AddFrame(IFrame *frame) = 0;
  //尝试推送Sample
  virtual int DeliverFrame() = 0;
  //获取内部IDeliverStrategy
  virtual IDeliverStrategy *GetStrategy() = 0;
  //通知发送端改变了
  virtual void SSRCChanged() = 0;
};


#endif
