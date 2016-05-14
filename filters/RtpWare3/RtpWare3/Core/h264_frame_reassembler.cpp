#include "h264_frame_reassembler.h"
#include "H264/h264_rtp_packer.h"
#include "H264/h264_nal.h"
#include "ortp_packet.h"
#include "ortp/ortp.h"
#include "Log/Log.h"


//
// Util
//

mblk_t *RtpPkt2Msgb(IRtpPacket *pkt) {
  mblk_t *m = nullptr;
  const unsigned int size = pkt->GetPlayloadSize();
  m = allocb(size+10, 0);
  if (m) {
    memcpy(m->b_wptr, pkt->GetPayload(), size);
    m->b_wptr += size;
  }
  return m;
}

//
// H264FrameReassembler
//

H264FrameReassembler::H264FrameReassembler(ILogger *logger)
  : GenericFrameReassembler(logger)
  , packer_(nullptr)
  , is_key_frame_(false)
{
  packer_ = new H264RtpPacker;
}


H264FrameReassembler::~H264FrameReassembler(void)
{
  if (packer_) {
    delete packer_;
    packer_ = nullptr;
  }
}

unsigned long H264FrameReassembler::ReassembleFrameData( 
  unsigned short begin_pos, unsigned short end_pos )
{
  if (!packer_) return 0;

  unsigned char *buf_tail = reassemble_buffer_;
  reassembled_data_size_ = 0;
  is_key_frame_ = false;
  MSQueue nals;
  ms_queue_init(&nals);

  for (unsigned short i = begin_pos; i <= end_pos; ++i) {
    IRtpPacket *pkt = rtp_packet_buffer_[i];
    if (pkt) {
      /* 这里说明一下：无论是发送接收用到的ortp，还是H264的标准化打包拆包代码，都是linphone
         开源项目的一部分，其API接受的数据结构都是mblt_t，所以这里要做一个转换处理 */
      mblk_t *rtpm = nullptr;
      ORtpPacket *ortp_pkt = dynamic_cast<ORtpPacket*>(pkt);
      if (ortp_pkt) {
        rtpm = ortp_pkt->GetMsgb(); //这里复制了一份，但不用free它，Unpack中会释放
      } else {
        //dynamic_cast失败，不是ORtpPacket对象，不能直接取得mblt_t，得建一个mblt_t
        rtpm = RtpPkt2Msgb(pkt);
      }

      if (rtpm) {
        if (i == end_pos) {
          //Unpack要检查mark标记，很郁闷，不知道为什么发送端设了没用
          mblk_set_marker_info(rtpm, 1);
        }

        unsigned char *payload = nullptr;
        rtp_get_payload(rtpm, &payload); //获得RTP头后的实际数据载荷
        rtpm->b_rptr = payload; //将数据读指针移动到数据域，以便调用解包函数

        packer_->Unpack(rtpm, nals);

        while (!ms_queue_empty(&nals)) {
          mblk_t *m = ms_queue_get(&nals);
          msgpullup(m, -1); //mblk_t内部数据可能是链表结果，该方法将其整合成一个节点，方便复制数据

          //复制H264 nal起始码
          unsigned long len = sizeof(start_code);
          memcpy_s(buf_tail, reassemble_buffer_size_ - reassembled_data_size_,
            start_code, len);
          buf_tail += len;
          reassembled_data_size_ += len;
          //复制数据
          len = m->b_wptr - m->b_rptr;
          const unsigned long buf_size_remain = reassemble_buffer_size_ - reassembled_data_size_;
          if (buf_size_remain < len) {
            LOG_WARN_EX(logger_, "reassemble buffer insufficient, need=%uB, remain=%uB",
              len, buf_size_remain);
            len = buf_size_remain;
          }
          memcpy_s(buf_tail, reassemble_buffer_size_ - reassembled_data_size_,
            m->b_rptr, len);
          buf_tail += len;
          reassembled_data_size_ += len;

          //检查是否为关键帧，这里的数据块本来就是按照nal划分的了，直接用H264Nal中有关数据结构
          //I帧中，会包含一个I-Slice，其nal类型编号为5，找到及确认这是一个I帧
          H264Nal::NALHeader *nal_header = reinterpret_cast<H264Nal::NALHeader*>(m->b_rptr);
          if (nal_header->nal_unit_type == H264Nal::NALType_IDR) {
            is_key_frame_ = true; 
            //LOG_PRINTF_EX(logger_, "I-Frame received"); // debug
          }

          freemsg(m);
        }
        //ms_queue_flush(&nals); //到这里队列肯定是空的了，不用清空
        //freemsg(rtpm); //unpack()中会释放，不要再次释放
      }

    } else {
      //理论上这里不该有null包，如果出现要记录异常
      LOG_WARN_EX(logger_, "null pkt found when reassembling frame");
    }
  }

  return reassembled_data_size_;
}

bool H264FrameReassembler::CheckSyncPoint()
{
  return is_key_frame_;
}
