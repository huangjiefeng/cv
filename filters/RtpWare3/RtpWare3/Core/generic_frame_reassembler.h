/************************************************************************/
/* 通用的rtp组帧器                                                      */
/* 其特点是：1、排序rtp包；                                             */
/*           2、收到mark为1的包时检查帧是否完整；                       */
/*           3、每次调用PopFrontFrame时返回最前面的完整帧，若该帧前还有 */
/*              不完整的帧，则丢弃                                      */
/************************************************************************/

#ifndef GENERIC_FRAME_REASSEMBLER_H
#define GENERIC_FRAME_REASSEMBLER_H

#include "receive_interfaces.h"
#include "../Util/circular_buffer.h"
#include "log/Log.h"


class RtpPacketBuffer : public CCircularBuffer<IRtpPacket*>
{
public:
  RtpPacketBuffer(ILogger *logger);
  ~RtpPacketBuffer();

  //放入一个包，如果不在缓冲区范围内则返回负值，否则返回索引位置
  int Put(IRtpPacket* pkt);
  //返回数据的结束位置
  unsigned int GetEndPos() { return end_pos_; }
  //获取第一个完整帧的索引区间[begin, end]
  void GetFirstValidFrameInterval(int &begin, int &end);
  //释放前n个位置的数据包
  void Free(unsigned int n);
  void Clear();
  bool Full();

private:
  unsigned int end_pos_; //当前最后一个有数据位置的下一个位置

  ILogger *logger_;

};


class GenericFrameReassembler :
  public IFrameReassembler
{
public:
  GenericFrameReassembler(ILogger *logger);
  ~GenericFrameReassembler(void);

  virtual int PushRtpPacket( IRtpPacket *packet );
  virtual IFrame * PopFrontFrame();

protected:
  //组帧算法。输入为rtp缓存起始和结束位置，返回帧长度
  virtual unsigned long ReassembleFrameData(
    unsigned short begin_pos, unsigned short end_pos);
  //检查组帧缓存中的数据是否为同步点
  virtual bool CheckSyncPoint();

protected:
  ILogger *logger_;

  static const unsigned short rtp_buffer_size_;
  RtpPacketBuffer rtp_packet_buffer_; //rtp包排序缓存

  static const unsigned long reassemble_buffer_size_;
  unsigned char *reassemble_buffer_; //组包缓存
  unsigned long reassembled_data_size_;

  unsigned int ssrc_;
  bool packet_dropped_;
  bool first_mark_bit_received_;

};


#endif