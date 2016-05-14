#include "audio_deliver_strategy.h"

#include "media_sync.h"
#include "../Util/util.h"
#include "Log/Log.h"

AudioDeliverStrategy::AudioDeliverStrategy(
  IBaseFilter *filter, CMediaType &type, ILogger *logger)
  : filter_(filter)
  , logger_(logger)
  , sample_rate_(0)
  , sample_count_(0)
  , base_rtp_timestamp_(0)
  , base_wallclock_time_(0)
  , recv_start_wallclock_time_(0)
  , last_sample_timestamp_(0)
  , got_time_base_from_rtcp_(false)
  , time_base_reset_(false)
  , sr_count_(0)
  , last_sr_ts_(0)
{
  if (type.IsValid() && type.formattype == FORMAT_WaveFormatEx) {
    WAVEFORMATEX *wfx = reinterpret_cast<WAVEFORMATEX *>(type.pbFormat);
    sample_rate_ = wfx->nSamplesPerSec;
    if (8000 > sample_rate_) {
      LOG_ERROR_EX(logger_,"init audio deliver strategy failed: invalid audio sample rate: %.0f",
        sample_rate_);
    }
  } else {
    LOG_ERROR_EX(logger_,"init audio deliver strategy failed: invalid audio format");
  }
  LOG_PRINTF_EX(logger_, "audio_sample_rate=%.0f", sample_rate_);
}


AudioDeliverStrategy::~AudioDeliverStrategy(void)
{
}

bool AudioDeliverStrategy::ShouldFrameDiscard( IFrame &frame )
{
#ifdef USE_SR_NTP
  if (!got_time_base_from_rtcp_) {
    return true;
  }
#endif
  //音频不用考虑丢帧问题
  return false;
}

int AudioDeliverStrategy::ConvertFrameToSample( 
  IFrame &frame, IMediaSample &sample )
{
  //data
  BYTE *data_buf = nullptr;
  sample.GetPointer(&data_buf);
  long buf_size = sample.GetSize();

  if (!data_buf || 0 >= buf_size) {
    LOG_ERROR_EX(logger_,"invalid dshow media sample");
    return -1;
  }
  if (buf_size < (long)frame.GetDataSize()) {
    LOG_ERROR_EX(logger_,"dshow media sample insufficient, buf_size=%d, need=%u",
      buf_size, frame.GetDataSize());
    return -1;
  }
  memcpy_s(data_buf, buf_size, frame.GetData(), frame.GetDataSize());
  sample.SetActualDataLength(frame.GetDataSize());

  //timestamp
  long long timestamp = 0;
  {
    //增加作用域，减少持有锁的时间
    CAutoLock lock(&timestamp_lock_);

#ifdef USE_SR_NTP

    /* 音视频时间戳与发送端的绝对时间(wallclock time)存在相关性，因此要还原音视频时间戳的相关性，
       实现音视频同步，首先要计算出Sample产生时刻的发送端ntp时间。
       通过SetNTPReferenceTimestamp我们获得了rtp时间戳rtp_ts，以及其对应的发送端绝对时间ntp_ts。
       由sample的时间戳ts与rtp_ts的差值，结合sample_rate_，就可以算出ts与rtp_ts的ntp时间差，进而
       根据ntp_ts可以得到sample在发送端产生时对应的ntp时刻。
     */
    //尚未获取时间基准，不推送帧
    if (!got_time_base_from_rtcp_) return -1;

    //step1:从rtp时间戳差值计算出样本时间戳与rtp基准时间戳的ntp时间差值
    //不用考虑时间戳循环后减出负数的问题，无符号数溢出后刚好是正确结果
    timestamp = (long long)((frame.GetTimestamp() - base_rtp_timestamp_) * 1e7 / sample_rate_);

    //step2:从差值推算样本产生时对应的ntp时间，以此作为时间戳，便于音视频同步
    timestamp += base_wallclock_time_;

    //记录一下日志，便于分析重设时间基准后的误差
    if (time_base_reset_) {
      LOG_INFO_EX(logger_, "rtp_ts=%u, ts=%I64dms, increament=%I64dms", 
        frame.GetTimestamp(), Ts2Ms(timestamp), Ts2Ms(timestamp-last_sample_timestamp_));
      time_base_reset_ = false;
    }

#else // USE_SR_NTP

    //不使用SR报告的时间参考值，直接估算流时间，此方法不保证音视频同步
    //尚未设置时间基准，先设置
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

  //以下处理与视频不同
  sample.SetDiscontinuity(TRUE);
  sample.SetSyncPoint(TRUE);

  return 0;
}

bool AudioDeliverStrategy::ShouldSampleDeliver(IMediaSample &sample)
{
  //音频总是尽快推送，并更新同步数据
  REFERENCE_TIME start_time, end_time;
  sample.GetTime(&start_time, &end_time);

  //保证音视频同步，只有对应时间的音频已经推送了，才推送视频
  //但要注意，例如一段80ms的音频被推送后，则其起始时间后80ms内的视频应该都可以推送，
  //故要加上一个值
  const REFERENCE_TIME ts_offset = 750000LL;
  MediaSync::Lock();
  if (start_time + ts_offset > MediaSync::delivered_stream_ts_) {
    MediaSync::delivered_stream_ts_ = start_time + ts_offset;
    MediaSync::delivered_stream_ts_wallclock_ = GetDShowTimeFromUnixTime(nullptr);
  }
  MediaSync::Unlock();

  //LOG_PRINTF_EX(logger_, "ts=%I64dms, sync_ts=%I64dms", start_time/10000, MediaSync::delivered_stream_ts_/10000); //debug
  return true;
}

void AudioDeliverStrategy::SetNTPReferenceTimestamp( unsigned long long ntp_ts, unsigned long ts )
{
  /* （1）本来，每收到一次RTCP的SR报告（师兄设置是3s发一个），都可以更新时间戳的参考基准。但是，
     由于样本本身推送并不是严格等间隔的，因此，如果频繁更新参考基准，会造成接收端的样本时间
     戳抖动。这里设置为每小时更新一次。
     （2）另外要注意一个问题，如果32位的RTP时间戳循环后参考基准未更新，也是会出问题的。更新间隔：
     视频按照90000Hz的频率计时，32位时间戳也够用2^32/90000/3600=13.25h，视频会议中音频采样
     一般不高于44kHz，所以每小时更新一次足够了。
     （3）此外还有一个问题，实测中发现ortp发出的第一个SR报告时间戳不对，偏大90秒左右，要忽略它。
     一般第一个SR在一秒内发送，ts会小于90000（RFC规定的视频时间戳精度），但是不能以此作为判断
     第一个SR报告的标准。试想万一发送端并非一开始就立即编码视频而是过了一段时间才开始编码第一
     帧呢？所以用了下面这个麻烦的手段
   */
  long long sender_wallclock = GetDShowTimeFromNTPTime(ntp_ts);
  if (sender_wallclock <= 0) return;

  //忽略第一个SR
  if (ts == last_sr_ts_) return;
  else { ++sr_count_; last_sr_ts_ = ts; }
  if (1 == sr_count_) return;

#ifdef USE_SR_NTP
  if (!got_time_base_from_rtcp_ || 
      sender_wallclock - base_wallclock_time_ >= 3600*10000000LL) {
    CAutoLock lock(&timestamp_lock_);
    //都提前1秒。因为发送端的RTCP总是在发完某一帧后发出，假设是第n帧，有的时候接收端收到了SR报告，
    //却还没处理n-1帧，这时候就悲剧了。为了解决这个问题，我们把基准提前一秒
    base_rtp_timestamp_ = ts - (unsigned long)sample_rate_;
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

void AudioDeliverStrategy::ResetNTPReferenceTimestamp()
{
  LOG_INFO_EX(logger_, "ntp reference reset");
  CAutoLock lock(&timestamp_lock_);
  got_time_base_from_rtcp_ = false;
  last_sr_ts_ = 0;
  sr_count_ = 0;
}

void AudioDeliverStrategy::SetStreamTimeBase( long long t )
{
  recv_start_wallclock_time_ = t;
  base_rtp_timestamp_ = 0;
  base_wallclock_time_ = 0;
#ifdef USE_SR_NTP
  LOG_INFO_EX(logger_, "audio waiting for SR report");
#else // USE_SR_NTP
  LOG_INFO_EX(logger_, "audio start receiver_wallclock=%I64dms", Ts2Ms(recv_start_wallclock_time_));
#endif // USE_SR_NTP
}
