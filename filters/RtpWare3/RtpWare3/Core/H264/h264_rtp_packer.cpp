#include "h264_rtp_packer.h"

#include "mediastreamer2/mscommon.h"

#include "assert.h"
#include "Mmsystem.h"


void GetMSQueue(H264NalList &nals, MSQueue &queue) {
  mblk_t *m = nullptr;
  for (auto i = nals.begin(); i != nals.end(); ++i) {
    H264Nal &nal = *i;
    m = allocb(nal.data_size_+10, 0);
    memcpy(m->b_wptr, nal.data_ptr_, nal.data_size_);
    m->b_wptr += nal.data_size_;
    ms_queue_put(&queue, m);
  }
}


H264RtpPacker::H264RtpPacker(void)
{
  packer=rfc3984_new();
  rfc3984_set_mode(packer,1);
  rfc3984_enable_stap_a(packer,true);
}


H264RtpPacker::~H264RtpPacker(void)
{
}

int H264RtpPacker::Pack( H264NalList &nal_list, MSQueue &rtps )
{
  MSQueue nalus;
  ms_queue_init(&nalus);
  GetMSQueue(nal_list, nalus);

  rfc3984_pack(packer,&nalus,&rtps,0);

  return 0;
}

int H264RtpPacker::Unpack( mblk_t *m, MSQueue &queue )
{
  rfc3984_unpack(packer, m, &queue);
  return 0;
}

