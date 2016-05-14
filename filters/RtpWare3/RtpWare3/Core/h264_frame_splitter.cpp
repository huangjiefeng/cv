#include "h264_frame_splitter.h"
#include "frame_units.h"
#include "ortp_unit.h"

H264FrameSplitter::H264FrameSplitter(ILogger *logger)
  : logger_(logger)
{
}

H264FrameSplitter::~H264FrameSplitter()
{
}

IFrameUnitsPtr H264FrameSplitter::Split(
  const unsigned char *buf, 
  unsigned long size,
  long long timestamp)
{
  //step1:将h264码流解析成分离的语法单元
  H264NalList nals;
  parser_.Parse(buf, size, nals);

  //step2:按照rfc将语法单元封装成rtp负载单元
  MSQueue rtps;
  ms_queue_init(&rtps);
  packer_.Pack(nals, rtps);

  //for debug
  //printf_s(" ts:%I64d, ", timestamp);
  //for (const H264Nal &nal : nals) {
  //  nal.Print(); // nalu header info
  //  printf_s(",");
  //}
  //printf_s("\n");

  //转成我们支持的格式
  IFrameUnitsPtr frame_units(new FrameUnits);
  mblk_t *m = nullptr;
  while((m=ms_queue_get(&rtps))!=NULL){
    unsigned int size=msgdsize(m);
    //uint8_t type=((*(m->b_rptr)) & ((1<<5)-1));
    //printf(("Rtp packet: %4d B, type=%2d\n"), size, type);
    msgpullup(m, (size_t)-1); //Combine mblk link list

    ISendUnitPtr send_unit(new OrtpUnit(m));
    frame_units->PushUnit(send_unit);

    freemsg(m);
  }

  //将DirectShow时间戳转换成rtp时间戳
  frame_units->SetTimestamp((unsigned long)(timestamp*(90000/1e7))); 
  //LOG_PRINTF("%I64d %u", timestamp, frame_units->GetTimestamp()); //Debug
  return frame_units;
}
