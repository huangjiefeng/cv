#include "media_sync.h"

REFERENCE_TIME MediaSync::delivered_stream_ts_ = 0;
long long MediaSync::delivered_stream_ts_wallclock_ = 0;
CCritSec MediaSync::lock_;

void MediaSync::Reset()
{
  Lock();
  delivered_stream_ts_ = 0;
  delivered_stream_ts_wallclock_ = 0;
  Unlock();
}
