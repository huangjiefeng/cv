#include "audio_frame_splitter.h"
#include "frame_units.h"
#include "ortp_unit.h"
#include "core_defs.h"
#include <ortp/str_utils.h>


AudioFrameSplitter::AudioFrameSplitter(const CMediaType &type, ILogger *logger)
  : ratio_(1.0)
  , logger_(logger)
{
  if (type.formattype == FORMAT_WaveFormatEx) {
    WAVEFORMATEX *wfx = reinterpret_cast<WAVEFORMATEX *>(type.pbFormat);
    ratio_ = ((double)wfx->nSamplesPerSec) / 1e7;
    LOG_PRINTF_EX(logger_, "audio_sample_rate=%d", wfx->nSamplesPerSec);
  } else {
    LOG_ERROR_EX(logger_,"init audio frame splitter failed: invalid audio format");
  }
}

AudioFrameSplitter::~AudioFrameSplitter()
{

}

IFrameUnitsPtr AudioFrameSplitter::Split(
  const unsigned char *buf, 
  unsigned long size, 
  long long timestamp)
{
  //音频帧一般较小，不分割，如果超长了，记录警告，但继续尝试发送
  if (size > max_rtp_payload_size) {
    LOG_WARN_EX(logger_, "aac frame size %d larger than mtu size limit %d", 
      size, max_rtp_payload_size);
  }

  mblk_t *m = allocb(size, 0);
  memcpy(m->b_wptr, buf, size);
  m->b_wptr += size;

  ISendUnitPtr send_unit(new OrtpUnit(m));
  IFrameUnitsPtr frame_units(new FrameUnits);
  frame_units->PushUnit(send_unit);
  freemsg(m);

  frame_units->SetTimestamp((unsigned long)(timestamp * ratio_));
  return frame_units;
}
