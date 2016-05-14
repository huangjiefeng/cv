#ifndef H264_RTP_PACKAGER_H
#define H264_RTP_PACKAGER_H

#include "h264_nal.h"
#include "mediastreamer2/msqueue.h"
#include "mediastreamer2/rfc3984.h"


struct Rfc3984Context;

class H264RtpPacker
{
public:
  H264RtpPacker(void);
  ~H264RtpPacker(void);

  int Package(H264NalList &nals, MSQueue &queue);

private:

  Rfc3984Context *packer;

};


#endif
