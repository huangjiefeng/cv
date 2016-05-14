#ifndef H264_FRAME_SPLITTER_H
#define H264_FRAME_SPLITTER_H

#include "send_interfaces.h"
#include "Log/Log.h"

#include "h264/h264_stream_parser.h"
#include "H264/h264_rtp_packer.h"

class H264FrameSplitter : public IFrameSplitter
{
public:
  H264FrameSplitter(ILogger *logger);
  ~H264FrameSplitter();

  virtual IFrameUnitsPtr Split(const unsigned char *buf, 
    unsigned long size, long long timestamp);

private:
  H264StreamParser parser_;
  H264RtpPacker packer_;
  ILogger *logger_;
};

#endif
