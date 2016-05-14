#include "ortp_receiver.h"

#include <assert.h>

#include "ortp/ortp.h"
#include "../util/util.h"
#include "ortp_packet.h"


ORtpReceiver::ORtpReceiver(ILogger *logger)
  : logger_(logger)
{
  memset(&rtp_stat_, 0, sizeof(rtp_stat_));
}

ORtpReceiver::~ORtpReceiver(void)
{
  DestroySession();
}

int ORtpReceiver::InitSession(std::string ip, unsigned short port, RtpPayloadType payload_type)
{
  DestroySession();

  LOG_INFO_EX(logger_, "ortp receiver init, ip=%s, port=%u, payload_type=%d", 
    ip.c_str(), port, payload_type);

  //ortp init
  //不要忘记在DllMain中调用ortp_init();以初始化ortp库！

  rtp_session_ = rtp_session_new(RTP_SESSION_RECVONLY);  
  rtp_session_set_profile(rtp_session_, &av_profile);

  /** use_connect为true时，将使用UDP的connect模式，只接收第一个数据包源的包，排除其他来源的RTP包；
   ** 设置为false，将接收所有发到本地端口的数据包	*/
  rtp_session_set_connected_mode(rtp_session_, true);
  /** symmetric_rtp为true时，将把接收到的第一个RTP/RTCP包的发送地址当作后面所有包的来源地址。
   ** 该值必须设置为true，否则不能正常发送RR包*/
  rtp_session_set_symmetric_rtp(rtp_session_, true);
  rtp_session_enable_rtcp(rtp_session_, true);
  rtp_session_->rtp.max_rq_size = 1000; /* maximum number of packet allowed to be queued */

  rtp_session_set_scheduling_mode(rtp_session_, false); //不使用调度器
  rtp_session_set_blocking_mode(rtp_session_, false); //非阻塞模式
  //rtp_session_set_dscp(rtp_session_, 0); //QoS参数使用ortp默认值
  rtp_session_set_local_addr(rtp_session_, ip.c_str(), port); //虽然是本地地址，还是要指定，区分IPv4/6和组播
  rtp_session_set_multicast_loopback(rtp_session_, true); //默认支持组播回环

  rtp_session_enable_adaptive_jitter_compensation(rtp_session_, false);
  rtp_session_enable_jitter_buffer(rtp_session_, false);

  rtp_session_set_payload_type(rtp_session_, payload_type);  
  /** 1M Byte的Socket缓冲区，40Mbits/s的视频流也够存200ms了 */
  rtp_session_set_rtp_socket_recv_buffer_size(rtp_session_, 1 << 20); 
  rtp_session_->rtp.max_rq_size = 1000; /* maximum number of packet allowed to be queued */

  {
    //使用此版本的oRTP可以直接设置socket属性；如果升级到最新版的oRTP不行，得另想办法
    //个人认为师兄设置这些属性是很好的，当没有收到数据包时，接收阻塞，防止持续循环占用CPU
    //同时，阻塞最多20ms，这样不会影响线程循环检查终止标记和正常退出
    unsigned long nonblock = 0;
    int timeout = 20;
    ioctlsocket(rtp_session_->rtp.socket, FIONBIO, &nonblock);
    setsockopt(rtp_session_->rtp.socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
  }

  //other members
  last_recv_ts_ = 0;
  ip_ = ip;
  port_ = port;
  payload_type_ = payload_type;
  InitRtpStat();

  return 0;
}

int ORtpReceiver::DestroySession()
{
  if (rtp_session_) {
    rtp_session_destroy(rtp_session_);
    rtp_session_ = nullptr;
  }
  memset(&rtp_stat_, 0, sizeof(rtp_stat_));

  return 0;
}

int ORtpReceiver::Receive( IRtpPacket *&packet )
{
  packet = nullptr;

  if (rtp_session_ == nullptr) {
    LOG_ERROR_EX(logger_, "try to receive rtp packet before init session");
    return -1;
  }

  mblk_t *m = rtp_session_recvm_with_ts(rtp_session_, last_recv_ts_);
  if (nullptr == m) {
    /*
      为什么加3000呢？根据RTP规范，视频的时钟频率为90000，则30帧的视频每帧+3000；
      音频时钟频率为其采样率，如16k的音频就是16000. 对于rtp_session_recvm_with_ts，
      其只返回时间戳比user_ts小的rtp包，故每次加3000已经能保证数据的接收了。即使
      加过头也没事，收到数据包后last_recv_ts_会被置为最后一个数据包的时间戳+3000
    */
    last_recv_ts_ += 3000;
    return 1;
  }
  
  packet = new ORtpPacket(m);
  last_recv_ts_ = packet->GetTimestamp() + 3000; //重置时间戳

  return 0;
}

int ORtpReceiver::GetStatistics( RtpStatItem &stat )
{
  if (rtp_session_ == nullptr) return -1;

  //需要实时获取的session基本信息
  rtp_stat_.ssrc = rtp_session_->rcv.ssrc;
  rtp_stat_.payload_type = rtp_session_get_recv_payload_type(rtp_session_);
  rtp_stat_.media_type = GetRtpStatMedaiTypeFromPayload(
    (RtpPayloadType)rtp_session_get_recv_payload_type(rtp_session_));

  //统计信息
  const rtp_stats_t* session_stat = rtp_session_get_stats(rtp_session_);
  if(session_stat) {
    rtp_stat_.packet_sent = session_stat->packet_sent;
    rtp_stat_.sent = session_stat->sent ;
    rtp_stat_.packet_recv = session_stat->packet_recv ;
    rtp_stat_.hw_recv = session_stat->hw_recv;
    rtp_stat_.recv = session_stat->recv ;
    rtp_stat_.outoftime = session_stat->outoftime;
    rtp_stat_.cum_packet_loss = session_stat->cum_packet_loss ;
    rtp_stat_.discarded = session_stat->discarded;
  }

  //估计带宽
  rtp_stat_.recv_bandwidth = rtp_session_compute_recv_bandwidth(rtp_session_) / 1000; //网络中为1000进制，不同于存储

  RtpStream *stream = &rtp_session_->rtp;

  //获得对方地址，只用获取一次，因为开启了UDP connect模式，因此只能接收一个发送端的数据
  //TODO:师兄定义的长度不够，要改涉及CoolView啊之类的很多模块
  //assert(sizeof(rtp_stat_.rem_addr) >= INET6_ADDRSTRLEN);
  if (rtp_stat_.rem_addr_len == 0) {
    unsigned short port = 0;
    if (GetInfoFromSockAddrStorage(&stream->rem_addr, stream->rem_addrlen,
        rtp_stat_.rem_addr, sizeof(rtp_stat_.rem_addr), &port) == 0) {
      rtp_stat_.rem_addr_len = strlen(rtp_stat_.rem_addr);
      rtp_stat_.rem_port = port;
    }
  }

  //获取前一次收到RR包时计算得出的网络往返延迟(ms)
  if (rtp_stat_.recv_bandwidth > 0) {
    rtp_stat_.delay = stream->delay;
    rtp_stat_.lost = stream->lost;
    rtp_stat_.jitter = stream->jitter;
  } else {
    rtp_stat_.delay = 0;
    rtp_stat_.lost = 0;
    rtp_stat_.jitter = 0;
  }

  stat = rtp_stat_;

  return 0;
}

void ORtpReceiver::InitRtpStat()
{
  //这两项这里初始化了，还是要试试获取，确保无措
  rtp_stat_.payload_type = rtp_session_get_recv_payload_type(rtp_session_);
  rtp_stat_.media_type = GetRtpStatMedaiTypeFromPayload(
    (RtpPayloadType)rtp_session_get_recv_payload_type(rtp_session_));

  rtp_stat_.rtp_endpoint_type = eET_Receiver;
  rtp_stat_.seqnumber = 0;
  rtp_stat_.interval = 5000; //TODO:这个值貌似现在没用了，暂时写死，注意保持跟RecvController中的间隔一致
  rtp_stat_.local_port = port_;

  //获取本机IP的规则啊真是麻烦
  addrinfo *rtp_ip_addr = nullptr;
  if (AddrStrToAddrInfo(ip_.c_str(), &rtp_ip_addr) == 0) {
    char local_addr[INET6_ADDRSTRLEN]; //足够长的缓冲区

    //根据传入的地址类型获取相应的本机地址
    if (IsIPv6Addr(rtp_ip_addr)) {
      GetLocalIPv6Addr(local_addr, sizeof(local_addr));
    } else {
      GetLocalIPv4Addr(local_addr, sizeof(local_addr));
    }

    if (IsMulticastAddr(rtp_ip_addr)) {
      //是组播地址的话，local要填真实本机地址，并设置组播地址
      strcpy_s(rtp_stat_.local_addr, sizeof(rtp_stat_.local_addr), local_addr);
      rtp_stat_.local_addr_len = strlen(rtp_stat_.local_addr);
      rtp_stat_.isMulticast = true;
      strcpy_s(rtp_stat_.mcast_addr, sizeof(rtp_stat_.mcast_addr), ip_.c_str());
      rtp_stat_.mcast_addr_len = strlen(rtp_stat_.mcast_addr);
    }
    else if (IsLoopbackAddr(rtp_ip_addr)) {
      //是回环地址的话，local要填真实本机地址
      strcpy_s(rtp_stat_.local_addr, sizeof(rtp_stat_.local_addr), local_addr);
      rtp_stat_.local_addr_len = strlen(rtp_stat_.local_addr);
    } else {
      strcpy_s(rtp_stat_.local_addr, sizeof(rtp_stat_.local_addr), ip_.c_str());
      rtp_stat_.local_addr_len = strlen(rtp_stat_.local_addr);
    }

    FreeAddrInfo(rtp_ip_addr);
  }
}

void ORtpReceiver::GetNTPReferenceTimestamp( unsigned long long &ntp_ts, unsigned long &ts )
{
  if (rtp_session_) {
    ntp_ts = rtp_session_->rtp.last_rcv_SR_ntp_ts;
    ts = rtp_session_->rtp.last_rcv_SR_rtp_ts;
  } else {
    ntp_ts = ts = 0;
  }
}

void ORtpReceiver::ResetSession()
{
  InitSession(ip_, port_, payload_type_);
}
