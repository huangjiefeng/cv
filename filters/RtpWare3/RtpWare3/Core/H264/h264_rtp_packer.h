#ifndef H264_RTP_PACKER_H
#define H264_RTP_PACKER_H

#include "h264_nal.h"
#include "mediastreamer2/msqueue.h"
#include "mediastreamer2/rfc3984.h"


const char start_code[] = {0x00, 0x00, 0x00, 0x01};

struct Rfc3984Context;

class H264RtpPacker
{
public:
  H264RtpPacker(void);
  ~H264RtpPacker(void);

  int Pack(H264NalList &nals, MSQueue &queue);
  int Unpack(mblk_t *m, MSQueue &queue);

private:

  Rfc3984Context *packer;

};


#endif
