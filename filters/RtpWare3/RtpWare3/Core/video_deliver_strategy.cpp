#include "video_deliver_strategy.h"

#include "media_sync.h"
#include "../Util/util.h"
#include "Log/Log.h"

VideoDeliverStrategy::VideoDeliverStrategy(
  IBaseFilter *filter, CMediaType &type, ILogger *logger)
  : filter_(filter)
  , logger_(logger)
  , sample_count_(0)
  , discontinuity_(true)
  , base_rtp_timestamp_(0)
  , base_wallclock_time_(0)
  , recv_start_wallclock_time_(0)
  , last_sample_timestamp_(0)
  , got_time_base_from_rtcp_(false)
  , time_base_reset_(false)
  , sr_count_(0)
  , last_sr_ts_(0)
{
}


VideoDeliverStrategy::~VideoDeliverStrategy(void)
{
}

bool VideoDeliverStrategy::ShouldFrameDiscard( IFrame &frame )
{
#ifdef USE_SR_NTP
  if (!got_time_base_from_rtcp_) {
    return true;
  }
#endif
  discontinuity_ = discontinuity_ || frame.IsDiscontinuity();
  if (discontinuity_ && !frame.IsSyncPoint()) {
    return true;
  }
  return false;
}

int VideoDeliverStrategy::ConvertFrameToSample( IFrame &frame, IMediaSample &sample )
{
  //data
  BYTE *data_buf = nullptr;
  sample.GetPointer(&data_buf);
  long buf_size = sample.GetSize();

  if (!data_buf || 0 >= buf_size) {
    LOG_ERROR_EX(logger_, "invalid dshow media sample");
    return -1;
  }
  if (buf_size < (long)frame.GetDataSize()) {
    LOG_ERROR_EX(logger_, "dshow media sample insufficient, buf_size=%d, need=%u",
      buf_size, frame.GetDataSize());
    return -1;
  }
  memcpy_s(data_buf, buf_size, frame.GetData(), frame.GetDataSize());
  sample.SetActualDataLength(frame.GetDataSize());

  //timestamp
  //计算方式同audio
  long long timestamp = 0;
  {
    //增加作用域，减少持有锁的时间
    CAutoLock lock(&timestamp_lock_);

#ifdef USE_SR_NTP

    if (!got_time_base_from_rtcp_) return -1;

    //step1:从rtp时间戳差值计算出样本与参考时间戳的绝对时间差值
    //不用考虑时间戳循环后减出负数的问题，无符号数溢出后刚好是正确结果
    timestamp = (long long)((frame.GetTimestamp() - base_rtp_timestamp_) * 1e7 / sample_rate_);

    //step2:从差值推算样本产生时对应的发送端绝对时间，以此作为时间戳，便于音视频同步
    timestamp += base_wallclock_time_;

    //记录一下日志，便于分析估计值与实际值的误差
    if (time_base_reset_) {
      LOG_INFO_EX(logger_, "rtp_ts=%u, ts=%I64dms, increament=%I64dms", 
        frame.GetTimestamp(), Ts2Ms(timestamp), Ts2Ms(timestamp-last_sample_timestamp_));
      time_base_reset_ = false;
    }

#else // USE_SR_NTP

    if (base_rtp_timestamp_ == 0) {
      base_rtp_timestamp_ = frame.GetTimestamp();
      base_wallclock_time_ = GetDShowTimeFromUnixTime(nullptr) - recv_start_wallclock_time_;
    }

    //step1:从rtp时间戳差值计算出样本时间戳与rtp基准时间戳的ntp时间差值
    timestamp = (long long)((frame.GetTimestamp() - base_rtp_timestamp_) * 1e7 / sample_rate_);

    //step2:从差值计算样本对应的流时间
    timestamp += base_wallclock_time_;
    //LOG_PRINTF_EX(logger_, "ts=%I64d", Ts2Ms(timestamp)); //debug

#endif // USE_SR_NTP

    //step3:防止时间戳非单调递增
    if (last_sample_timestamp_ == 0) { 
      //即使timestamp为负数或恰为0的情况，这样处理也没问题
      last_sample_timestamp_ = timestamp - 1;
    }
    if (last_sample_timestamp_ >= timestamp) {
      LOG_WARN_EX(logger_, "ts=%I64d less than last_ts=%I64d", timestamp, last_sample_timestamp_);
      //时间戳不大于上一帧的，则设为上一帧后10ms
      timestamp = last_sample_timestamp_ + 100000;
    }

    last_sample_timestamp_ = timestamp;
  }

  REFERENCE_TIME start_time, end_time;
  start_time = timestamp;
  end_time = start_time + 1; //end_time没人看的，微软都允许这样子的
  sample.SetTime(&start_time, &end_time);

  //other
  {
    REFERENCE_TIME start_media_time, end_media_time;
    start_media_time = sample_count_;
    end_media_time = ++sample_count_;
    sample.SetMediaTime(&start_media_time, &end_media_time);
  }

  sample.SetDiscontinuity(discontinuity_ || frame.IsDiscontinuity());
  sample.SetSyncPoint(frame.IsSyncPoint());

  //reset discontinuity
  if (frame.IsSyncPoint()) {
    discontinuity_ = false; //标记可以不用丢弃帧了
  }

  return 0;
}

bool VideoDeliverStrategy::ShouldSampleDeliver( IMediaSample &sample )
{
  bool ret = false;
  REFERENCE_TIME start_time, end_time;
  sample.GetTime(&start_time, &end_time);

  //保证音视频同步，只有理论上对应时间的音频已经推送了，才推送视频
  MediaSync::Lock();
  //但不能像下面这样简单的判断，如果发送方暂停发送音频，视频会卡住
  //if (start_time <= MediaSync::delivered_stream_ts_) ret = true;
  //这么做才是正确的
  if (MediaSync::delivered_stream_ts_wallclock_ > 0) {
    long long now = GetDShowTimeFromUnixTime(nullptr);
    long long audio_stream_time = now - MediaSync::delivered_stream_ts_wallclock_ + 
      MediaSync::delivered_stream_ts_;
    if (start_time <= audio_stream_time) ret = true;
  } else {
    //没有音频参考，视频尽可能快的推送
    ret = true;
    //LOG_DEBUG_EX(logger_, "%I64d %I64d", start_time, video_stream_time);
  }
  MediaSync::Unlock();

  //printf_s("%s", ret ? ">" : "-"); //debug
  return ret;
}

void VideoDeliverStrategy::SetNTPReferenceTimestamp( 
  unsigned long long ntp_ts, unsigned long ts )
{
  //基本同AudioDeliverStrategy::SetNTPReferenceTimestamp
  long long sender_wallclock = GetDShowTimeFromNTPTime(ntp_ts);
  if (sender_wallclock <= 0) return;

  //除了AudioDeliverStrategy中的理由，视频还有一个理由要忽略第一个SR：
  //Intel硬编码器在编码第一帧时会有200ms左右延迟，而ortp在发送第一帧时会发送SR报告，
  //并以发送第一帧的时刻作为ntp_ts值。那就是说，第一个视频SR报告中的ntp_ts值延迟200ms左右！
  //如果以此作为参考，将导致视频延迟，故需要忽略第一个SR报告。
  if (ts == last_sr_ts_) return;
  else { ++sr_count_; last_sr_ts_ = ts; }
  if (1 == sr_count_) return;

#ifdef USE_SR_NTP
  if (!got_time_base_from_rtcp_ || 
      sender_wallclock - base_wallclock_time_ >= 3600*10000000LL) {
    CAutoLock lock(&timestamp_lock_);
    //都提前1秒。理由同音频
    base_rtp_timestamp_ = ts - sample_rate_;
    base_wallclock_time_ = sender_wallclock - 10000000LL;
    time_base_reset_ = true;
#else
  if (!got_time_base_from_rtcp_) {
#endif
    got_time_base_from_rtcp_ = true;
    LOG_INFO_EX(logger_, "by SR report, rtp_ts=%u correspond to sender_wallclock=%I64dms", 
      ts, Ts2Ms(sender_wallclock));
  }
}

void VideoDeliverStrategy::ResetNTPReferenceTimestamp()
{
  LOG_INFO_EX(logger_, "ntp reference reset");
  CAutoLock lock(&timestamp_lock_);
  got_time_base_from_rtcp_ = false;
  last_sr_ts_ = 0;
  sr_count_ = 0;
}

void VideoDeliverStrategy::SetStreamTimeBase( long long t )
{
  recv_start_wallclock_time_ = t;
  base_rtp_timestamp_ = 0;
  base_wallclock_time_ = 0;
#ifdef USE_SR_NTP
  LOG_INFO_EX(logger_, "video waiting for SR report");
#else // USE_SR_NTP
  LOG_INFO_EX(logger_, "video start receiver_wallclock=%I64dms", Ts2Ms(recv_start_wallclock_time_));
#endif // USE_SR_NTP
}
