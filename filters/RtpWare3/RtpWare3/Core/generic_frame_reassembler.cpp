#include "generic_frame_reassembler.h"
#include "media_frame.h"
#include "Log/Log.h"

//
// RtpPacketBuffer
//

RtpPacketBuffer::RtpPacketBuffer(ILogger *logger)
  : CCircularBuffer()
  , end_pos_(0)
  , logger_(logger)
{
}

RtpPacketBuffer::~RtpPacketBuffer()
{
  Destroy();
}

int RtpPacketBuffer::Put( IRtpPacket* packet )
{
  unsigned short seq = packet->GetSeqNumber();

  //计算位置
  unsigned short pos = 0; //初始为第一个位置，类型必须为unsigned short，与seq一致，原因见下
  if ((*this)[0]) {
    unsigned short first_seq = (*this)[0]->GetSeqNumber();
    pos = seq - first_seq;
    //注意咯：rtp序列号会循环，最大65535，但是这里却没有看到处理循环的情况？
    //写个程序，看看unsigned short的0-65535结果是啥？
  }

  //检查位置有效性
  if (pos >= BufSize()) {
    LOG_ERROR_EX(logger_,"rtp buf overflow(packet arrived too late?), seq=%u, pos=%u", 
      seq, pos);
    return -1;
  }

  //放置数据
  if ((*this)[pos]) {
    //已有数据，难道序列号相等？这是不该发生的，太可怕了，用新的替换
    LOG_WARN_EX(logger_, "rtp seq repetition, seq=%u", seq);
  }
  (*this)[pos] = packet;
  if (pos >= end_pos_) end_pos_ = pos + 1;
  
  return pos;
}

void RtpPacketBuffer::GetFirstValidFrameInterval( int &begin, int &end )
{
  begin = end = -1;
  bool lost = false;
  unsigned int cur_begin = 0;
  for (unsigned int i = 0; i < end_pos_; ++i) {
    if ((*this)[i] == nullptr) {
      lost = true; //标记丢包
    }
    else if ((*this)[i]->GetMarkbit()) {
      if (!lost) {
        //没有丢包，帧完整
        begin = cur_begin;
        end = i;
        return;
      } else {
        //帧不完整，则更新开始标记为下一帧开始，继续查找
        cur_begin = i + 1;
        lost = false; //重置丢包标记
      }
    }
  }
}

void RtpPacketBuffer::Free( unsigned int n )
{
  if (n > end_pos_) return;
  end_pos_ -= n; //先减再做循环，看循环条件，n会变
  while (n-- > 0) {
    delete (*this)[0];
    Pop();
  }
}

void RtpPacketBuffer::Clear()
{
  for (unsigned int i = 0; i < end_pos_; ++i) {
    delete (*this)[i];
  }
  end_pos_ = 0;
  CCircularBuffer::Clear();
}

bool RtpPacketBuffer::Full()
{
  if (BufSize() == end_pos_) {
    return true;
  }
  return false;
}


//
// GenericFrameReassembler
//

//初始化缓存队列大小，单位：个
//1080p30fps视频流按照10Mbps计算，每秒1.25MB，每个MTU最大1500B
//则1000个大约可容纳1s该视频流数据。实际不是每个MTU都是1500B，
//但1000个也足够了，能缓存3帧就好，缓存太多帧会导致延迟。且正常
//情况下，一帧只要完整立即就取出了，只有不完整的才会被缓存
const unsigned short GenericFrameReassembler::rtp_buffer_size_ = 1000;

//MTU最大1500B，200个包足够1080p的一帧了，也只占用150k内存
//如果以后接收4k视频，需要改这里两个缓冲区大小
const unsigned long GenericFrameReassembler::reassemble_buffer_size_ = 1500 * 200;


GenericFrameReassembler::GenericFrameReassembler(ILogger *logger)
  : logger_(logger)
  , rtp_packet_buffer_(logger)
  , ssrc_(0)
  , packet_dropped_(true)
  , first_mark_bit_received_(false)
{
  rtp_packet_buffer_.Allocate(rtp_buffer_size_);
  reassemble_buffer_ = new unsigned char[reassemble_buffer_size_];
}


GenericFrameReassembler::~GenericFrameReassembler(void)
{
  if (reassemble_buffer_) {
    delete reassemble_buffer_;
    reassemble_buffer_ = nullptr;
  }
}

int GenericFrameReassembler::PushRtpPacket( IRtpPacket *packet )
{
  if (!packet) return PUSH_ERR;

  int result = 0;

  //未收到第一个markbit时，不确定第一帧是否完整，丢弃所有包
  if (!first_mark_bit_received_) {
    if (packet->GetMarkbit())  first_mark_bit_received_ = true;
    delete packet;
    return PUSH_PACKET_DROP;
  }

  //check ssrc
  if (0 == ssrc_) ssrc_ = packet->GetSSRC();
  else if (packet->GetSSRC() != ssrc_) {
      //ssrc改变，说明发送端可能重启或变更，重置有关变量
    LOG_WARN_EX(logger_,"rtp ssrc differs, expected=%u, received=%u", ssrc_, packet->GetSSRC());
    rtp_packet_buffer_.Clear();
    ssrc_ = packet->GetSSRC();
    packet_dropped_ = true;
    first_mark_bit_received_ = true; //这个主要用于防止第一帧不完整，但ssrc改变时一般是发送端重新开始发送，帧是完整的，不用设为false
    result |= PUSH_SSRC_CHANGE;
  }

  //检查缓冲区是否已满
  if (rtp_packet_buffer_.Full()) {
    unsigned int n = (rtp_packet_buffer_.BufSize() > 10 ? rtp_packet_buffer_.BufSize() / 10 : 1);
    LOG_WARN_EX(logger_, "rtp buffer is full, free %u packets", n);
    rtp_packet_buffer_.Free(n); //释放10%位置
    packet_dropped_ = true; //记录丢包
    result |= PUSH_PACKET_DROP;
  }

  //put packet to buffer
  int pos = rtp_packet_buffer_.Put(packet);
  if (pos < 0) {
    //缓冲区溢出，这是异常，重置缓存吧
    rtp_packet_buffer_.Clear();
    packet_dropped_ = true; //记录丢包
    first_mark_bit_received_ = false; //重新开始接收帧
    LOG_WARN_EX(logger_, "reset rtp packet buffer, maybe a lot of packets lost");
    return PUSH_ERR;
  }

  if (packet->GetMarkbit() || pos != rtp_packet_buffer_.GetEndPos() - 1) {
    //当收到带有帧结束标记的包，或收到一个乱序晚到的包时，应检查帧完整性
    result |= PUSH_CHK_FRAME;
  }
  return result;
}

IFrame * GenericFrameReassembler::PopFrontFrame()
{
  int begin_pos, end_pos;
  rtp_packet_buffer_.GetFirstValidFrameInterval(begin_pos, end_pos);
  if (begin_pos == -1) return nullptr;

  const unsigned long buf_size_need = (end_pos-begin_pos+1) * 1500;
  if (buf_size_need > reassemble_buffer_size_) {
    LOG_ERROR_EX(logger_,"reassemble buf insufficient, need=%uB, actual=%uB", 
      buf_size_need, reassemble_buffer_size_);
    return nullptr;
  }

  //组帧，基本策略，直接拼接
  unsigned long data_size = ReassembleFrameData(begin_pos, end_pos);

  //记录第1个包，一帧的每个包除了markbit其他都一样的
  IRtpPacket *first_pkt = rtp_packet_buffer_[begin_pos];
  IFrame *frame = nullptr;
  if (first_pkt) {
    frame = new MediaFrame;
    if (frame) {
      frame->SetData(reassemble_buffer_, data_size);
      frame->SetTimestamp(first_pkt->GetTimestamp());
      frame->SetSyncPoint(CheckSyncPoint());

      //begin_pos不为0时，意味着前面有不完整帧
      //这里的策略是，返回完整帧，并丢弃前面不完整的帧
      if (begin_pos == 0 && !packet_dropped_) frame->SetDiscontinuity(false);
      else {
        frame->SetDiscontinuity(true);
        LOG_WARN_EX(logger_, "some packets lost, ignore %d packets", begin_pos);
      }

      //重置丢包标记
      packet_dropped_ = false;
    } else {
      LOG_ERROR_EX(logger_,"alloc media frame failed, maybe memory leak?");
    }
  } else {
    LOG_ERROR_EX(logger_,"cannot retrieve timestamp, null rtp packet");
  }

  //释放rtp包
  rtp_packet_buffer_.Free(end_pos + 1); //end_pos是闭区间上界，记得+1

  return frame;
}

unsigned long GenericFrameReassembler::ReassembleFrameData( 
  unsigned short begin_pos, unsigned short end_pos )
{
  unsigned char *buf_tail = reassemble_buffer_;
  reassembled_data_size_ = 0;

  for (unsigned short i = begin_pos; i <= end_pos; ++i) {
    IRtpPacket *pkt = rtp_packet_buffer_[i];
    if (pkt) {
      memcpy_s(buf_tail, reassemble_buffer_size_ - reassembled_data_size_,
        pkt->GetPayload(), pkt->GetPlayloadSize());
      buf_tail += pkt->GetPlayloadSize();
      reassembled_data_size_ += pkt->GetPlayloadSize();
    } else {
      //理论上这里不该有null包，如果出现要记录异常
      LOG_WARN_EX(logger_, "null pkt found when reassembling frame");
    }
  }
  return reassembled_data_size_;
}

bool GenericFrameReassembler::CheckSyncPoint()
{
  //默认一律返回true
  return true;
}
