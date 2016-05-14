#ifndef GENERIC_DELIVER_STRATEGY_H
#define GENERIC_DELIVER_STRATEGY_H

#include "receive_interfaces.h"
#include "Log/Log.h"
#include <streams.h>

class AudioDeliverStrategy :
  public IDeliverStrategy
{
public:
  AudioDeliverStrategy(IBaseFilter *filter, CMediaType &type, ILogger *logger);
  ~AudioDeliverStrategy(void);

  virtual bool ShouldFrameDiscard( IFrame &frame );
  virtual int ConvertFrameToSample( IFrame &frame, IMediaSample &sample );
  virtual bool ShouldSampleDeliver( IMediaSample &sample );
  virtual void SetDiscontinuity() {}
  virtual void SetNTPReferenceTimestamp( unsigned long long ntp_ts, unsigned long ts );
  virtual void ResetNTPReferenceTimestamp();
  virtual void SetStreamTimeBase( long long t );

protected:
  IBaseFilter *filter_;
  double sample_rate_; //用double方便计算，避免每次转换
  ILogger *logger_;

  //实时状态数据
  unsigned long long sample_count_; //对于96k的音频，无符号int只能用(2^32-1)/96000/86400=0.5天

  //时间戳基准
  unsigned long base_rtp_timestamp_; //rtp时间戳基准，务必与rtp报头的32位无符号一致，
    //时间戳循环后要利用其自动溢出特性计算时间差值
  long long base_wallclock_time_; //base_rtp_timestamp_对应的发送端绝对时间，单位100ns
  long long recv_start_wallclock_time_; //视频流起始时刻对应的接收端绝对时间，即filter启动时间
  long long last_sample_timestamp_;
  bool got_time_base_from_rtcp_; //用于区分时间基准是估计值还是从SR报告得到的可靠值
  bool time_base_reset_; //for logging 
  CCritSec timestamp_lock_;

  unsigned long sr_count_; //收到的SR报告数，用于解决一个无语的问题，作用见SetNTPReferenceTimestamp实现
  unsigned long last_sr_ts_; //用于辨别sr报告是否更新了，作用同上
};


#endif // !GENERIC_DELIVER_STRATEGY_H
