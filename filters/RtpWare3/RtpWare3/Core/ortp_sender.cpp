#include "ortp_sender.h"
#include "log/log.h"

#include <process.h>

ORtpSender::ORtpSender(void)
{
  logger_ = CreateLogger(); //使用每个sender自己的logger，以便区分哪个发送目标出错

  rtp_session_ = nullptr;
  send_thread_ = INVALID_HANDLE_VALUE;
  stop_thread_exec_ = true;
}


ORtpSender::~ORtpSender(void)
{
  DestroySession();
}

int ORtpSender::InitSession(std::string ip, unsigned short port, RtpPayloadType payload_type)
{
  DestroySession();
  ip_ = ip;
  port_ = port;

  //init rtp session
  //不要忘记在DllMain中调用ortp_init();以初始化ortp库！

  rtp_session_ = rtp_session_new(RTP_SESSION_SENDONLY);  

  rtp_session_set_scheduling_mode(rtp_session_, 0);  
  //rtp_session_set_blocking_mode(rtp_session_, 0); //当rtp_session_set_scheduling_mode启用时有效
  /** use_connect为true时，将与第一个接收到的RTP/RTCP包建立socket连接，排除其他来源的RTP包；
   ** 设置为false，将接收所有发到本地端口的数据包	*/
  rtp_session_set_connected_mode(rtp_session_, false);
  /** symmetric_rtp为true时，将把接收到的第一个RTP/RTCP包的发送地址当作后面所有包的来源地址。该值必须设置为true*/
  rtp_session_set_symmetric_rtp(rtp_session_, true); 
  rtp_session_set_profile(rtp_session_, &av_profile);
  rtp_session_set_rtp_socket_send_buffer_size(rtp_session_, 1024 * 64); //Socket的缓冲区最大大小，0表示用系统默认值，这个对UDP有用？
  rtp_session_enable_rtcp(rtp_session_, true);
  rtp_session_enable_adaptive_jitter_compensation(rtp_session_, false); //不能设置自适应抖动，否则会导致视频很卡
  rtp_session_enable_jitter_buffer(rtp_session_, false); //不使用抖动控制
  rtp_session_set_send_payload_type(rtp_session_, payload_type); // 96 is for H.264
  rtp_session_set_remote_addr(rtp_session_, ip.c_str(), port); 

  //支持组播回环
  rtp_session_set_multicast_loopback(rtp_session_, 1);

  {
    //使用此版本的oRTP可以直接设置socket属性；如果升级到最新版的oRTP不行，得另想办法
    //使用阻塞模式，并设置一个超时，这样可以在socket暂时不可写时避免手动重试，也防止filter卡死
    unsigned long nonblock = 0;
    int timeout = 20; //最多20ms，一般等待3ms socket就可写了，20ms为50帧时的推送间隔，不能再大了，不然发不过来了
    ioctlsocket(rtp_session_->rtp.socket, FIONBIO, &nonblock);
    setsockopt(rtp_session_->rtp.socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
    nonblock = 1;
    ioctlsocket(rtp_session_->rtcp.socket, FIONBIO, &nonblock); //rtcp不要阻塞，收不到就略过
  }

  rtp_session_reset(rtp_session_); //重置rtp session统计

  /*m_SSRC  = getenv("SSRC");  
  if (m_SSRC != NULL)   
  {  
    rtp_session_set_ssrc(rtp_session_mgr.rtp_session, atoi(m_SSRC));  
  } */  

  //start thread
  stop_thread_exec_ = false;
  send_thread_ = (HANDLE)_beginthreadex(nullptr, 0, SendProc, static_cast<void*>(this), 0, nullptr);

  //根据this指针设置logger标识，这样就可以区分是哪个sender打的日志了
  char logger_name[32] = {0};
  sprintf_s(logger_name, sizeof(logger_name), "rtp%d", (int)this);
  logger_->SetModuleName(logger_name);
  LOG_INFO_EX(logger_, "sender thread start, ready send payload %d to [%s]:%d",
    payload_type, ip.c_str(), port); //记录一下线程对应的发送目标信息

  return 0;
}

int ORtpSender::DestroySession()
{
  //stop thread
  stop_thread_exec_ = true;
  DWORD wait_result = 0;
  if (send_thread_ != INVALID_HANDLE_VALUE) {
    wait_result = WaitForSingleObject(send_thread_, INFINITE);
    if (WAIT_OBJECT_0 != wait_result) {
      LOG_ERROR_EX(logger_, "stop sender thread failed");
    } else {
      LOG_INFO_EX(logger_, "sender thread stopped");
    }
    send_thread_ = INVALID_HANDLE_VALUE;
  }

  //destroy session
  if (rtp_session_) {
    rtp_session_destroy(rtp_session_);
    rtp_session_ = nullptr;
  }

  //clear queued data
  while (!frame_units_queue_.empty()) {
    frame_units_queue_.pop();
  }
  return 0;
}

int ORtpSender::Send(IFrameUnitsPtr p)
{
  //备注：原来是采用多线程发送的，后来发现没有必要，发送效率很高，一个线程足够了，
  //所以也不用加锁了，这里用到了队列，其实也没意义了 ——Liaokz，2015.4
  CAutoLock lock(&queue_lock_);
  if (frame_units_queue_.size() > 3) {
    //万一发送端口来不及发送了，不要囤积过多包
    LOG_WARN_EX(logger_, "too many packet queued, ignore new packet");
    return 0;
  }

  //每个Sender发送时会改变p内部的数据块队列，故要复制一份
  //注意：复制操作仅复制了p内部的数据块的智能指针队列，而不是数据块本身哦
  IFrameUnitsPtr new_p(p->Clone());
  frame_units_queue_.push(new_p);

  return 0;
}

unsigned int __stdcall ORtpSender::SendProc( void *param )
{
  ORtpSender *obj = static_cast<ORtpSender*>(param);
  while (!obj->stop_thread_exec_) {
    obj->DoSend();
  }
  return 0;
}

void ORtpSender::DoSend()
{
  IFrameUnitsPtr frame_units;
  {
    //仅作用域内持有锁
    CAutoLock lock(&queue_lock_);
    if (!frame_units_queue_.empty()) {
      frame_units = frame_units_queue_.front();
      frame_units_queue_.pop();
    }
  }
  if (!frame_units) {
    Sleep(5); //没有数据则等待一段时间
    return;
  }

  unsigned long ts = frame_units->GetTimestamp();
  while (!frame_units->Empty()) {
    ISendUnitPtr unit = frame_units->PopUnit();
    if (!unit) continue;
    if (rtp_session_) {
      //create packet
      mblk_t* m = rtp_session_create_packet(rtp_session_, 
        RTP_FIXED_HEADER_SIZE, 
        (uint8_t *)unit->GetData(), 
        unit->GetSize());
      //set markbit
      if (frame_units->Empty()) rtp_set_markbit(m, 1);
      else rtp_set_markbit(m, 0);
      //send
      //在非阻塞模式下，可能返回-1，rtp.send_errno==WSAEWOULDBLOCK，表示资源暂时不可用，因此尝试发送若干次
      //我们在初始化时设置了阻塞，并设置了延迟，所以不重试，失败就真的失败了
      int packet_size = msgdsize(m);
      //从源码可以看出rtp_session_sendm_with_ts无论失败与否都会释放m，所以不用调用 freemsg(m);
      int sent_size = rtp_session_sendm_with_ts(rtp_session_, m, ts);
      if (sent_size < packet_size) {
        LOG_WARN_EX(logger_, "send data error, data_size=%u, sent_size=%d, socket_err=%d", 
          packet_size, sent_size, rtp_session_->rtp.send_errno);
      }
    }
  }
}
