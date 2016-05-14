#include "aac_frame_reassembler.h"
#include "Log/Log.h"


//2 bytes for AU-headers-length and 2 bytes for AU-header
const unsigned short au_header_size = 4;

AACFrameReassembler::AACFrameReassembler(ILogger *logger)
  : GenericFrameReassembler(logger)
{
}


AACFrameReassembler::~AACFrameReassembler(void)
{
}

//AAC音频组帧，除了第一个包要考虑去掉AU头，其他与GenericFrameReassemble一致
unsigned long AACFrameReassembler::ReassembleFrameData( 
  unsigned short begin_pos, unsigned short end_pos )
{
  unsigned char *buf_tail = reassemble_buffer_;
  reassembled_data_size_ = 0;

  for (unsigned short i = begin_pos; i <= end_pos; ++i) {
    IRtpPacket *pkt = rtp_packet_buffer_[i];
    if (pkt) {
      unsigned char *payload = pkt->GetPayload();
      unsigned short len = pkt->GetPlayloadSize();

      if (i == begin_pos && len > au_header_size) {
        payload += au_header_size;
        len -= au_header_size;
      }
      memcpy_s(buf_tail, reassemble_buffer_size_ - reassembled_data_size_,
        payload, len);
      buf_tail += len;
      reassembled_data_size_ += len;
    } else {
      //理论上这里不该有null包，如果出现要记录异常
      LOG_WARN_EX(logger_, "null pkt found when reassembling frame");
    }
  }
  return reassembled_data_size_;
}
