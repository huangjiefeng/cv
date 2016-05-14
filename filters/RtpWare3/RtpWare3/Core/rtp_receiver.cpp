#include "rtp_receiver.h"


RtpReceiver::RtpReceiver(void)
{
}


RtpReceiver::~RtpReceiver(void)
{
}

int RtpReceiver::InitSession( uint16_t port )
{
  DestroySession();

  ortp_init();  
  ortp_scheduler_init();  

  rtp_session_ = rtp_session_new(RTP_SESSION_RECVONLY);  
  rtp_session_set_profile(rtp_session_, &av_profile);

  /** use_connect为true时，将与第一个接收到的RTP/RTCP包建立socket连接，排除其他来源的RTP包；
   ** 设置为false，将接收所有发到本地端口的数据包	*/
  rtp_session_set_connected_mode(rtp_session_, true);
  /** symmetric_rtp为true时，将把接收到的第一个RTP/RTCP包的发送地址当作后面所有包的来源地址。
   ** 该值必须设置为true，否则不能正常发送Rr包*/
  rtp_session_set_symmetric_rtp(rtp_session_, true);

  rtp_session_set_scheduling_mode(rtp_session_, false);  
  rtp_session_set_blocking_mode(rtp_session_, false);  
  rtp_session_set_local_addr(rtp_session_, "127.0.0.1", port, port+1);  

  rtp_session_enable_adaptive_jitter_compensation(rtp_session_, false);  
  //rtp_session_set_jitter_compensation(rtp_session_, 40);
  rtp_session_enable_jitter_buffer(rtp_session_, false);
  rtp_session_->rtp.max_rq_size = 1000; /* maximum number of packet allowed to be queued */

  rtp_session_set_payload_type(rtp_session_, 96);  
  rtp_session_set_rtp_socket_recv_buffer_size(rtp_session_, 1 << 20); 

  /*unsigned long nonblock = 0;
  int timeout = 100;
  ioctlsocket(rtp_session_->rtp.socket, FIONBIO, &nonblock);
  setsockopt(rtp_session_->rtp.socket, SOL_SOCKET,SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); */

  last_recv_ts = 0;

  return 0;
}

int RtpReceiver::DestroySession()
{
  if (rtp_session_) {
    rtp_session_destroy(rtp_session_);
    rtp_session_ = nullptr;
  }
  return 0;
}

int RtpReceiver::Receive( MSQueue &rtps, uint32_t &timestamp )
{
  bool got_m = false;
  while (true)  
  {  
    mblk_t *m = rtp_session_recvm_with_ts(rtp_session_, last_recv_ts);
    if (nullptr == m) {
      //printf("No data ts=%d\n", last_recv_ts);
      break;
    }
    got_m = true;
    ms_queue_put(&rtps, m);
    timestamp = rtp_get_timestamp(m);
    int seq = rtp_get_seqnumber(m);
    int mark = rtp_get_markbit(m);
    printf("size=%4d, ts=%9d, seq=%5d, mark=%d\n", msgdsize(m),timestamp, seq, mark);
    last_recv_ts = timestamp + 10000;
  }
  if (got_m) {
    //last_recv_ts += 50000;
    //printf("Next ts=%d\n", last_recv_ts);
  }
  last_recv_ts += 10000;

  return 0;
}
