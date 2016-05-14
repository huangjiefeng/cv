#ifndef H264_FRAME_REASSEMBLER_H
#define H264_FRAME_REASSEMBLER_H

#include "generic_frame_reassembler.h"


class H264RtpPacker;


class H264FrameReassembler :
  public GenericFrameReassembler
{
public:
  H264FrameReassembler(ILogger *logger);
  ~H264FrameReassembler(void);

protected:
  virtual unsigned long ReassembleFrameData(
    unsigned short begin_pos, unsigned short end_pos);

  virtual bool CheckSyncPoint();

protected:
  H264RtpPacker *packer_;
  bool is_key_frame_; //当前帧是否为关键帧

};


#endif
