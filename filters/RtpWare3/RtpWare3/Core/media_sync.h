/************************************************************************/
/* 本文件定义用于音视频同步的数据区                                     */
/* 同一DLL中，全局量共享，所以可以用于音视频filter通信                  */
/************************************************************************/

#ifndef MEDIA_SYNC_H
#define MEDIA_SYNC_H

#include "streams.h"

class MediaSync
{
public:
  //务必保证尽快释放，不要长时间持有锁
  static void Lock() { lock_.Lock(); }
  static void Unlock() { lock_.Unlock(); }
  static void Reset();

public:
  //用于同步
  static REFERENCE_TIME delivered_stream_ts_; //参考流已经推送的样本时间戳
  static long long delivered_stream_ts_wallclock_; //delivered_stream_ts_推送时对应的绝对时间

private:
  static CCritSec lock_;
};

#endif
