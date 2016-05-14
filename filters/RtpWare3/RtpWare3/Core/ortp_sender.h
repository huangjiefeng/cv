#ifndef RTP_SENDER_H
#define RTP_SENDER_H

#include "windows.h"
#include "send_interfaces.h"
#include "streams.h"
#include "log/log.h"
#include <ortp/ortp.h>

class ORtpSender : public IRtpSender
{
public:
  ORtpSender(void);
  ~ORtpSender(void);

  int InitSession(std::string ip, unsigned short port, RtpPayloadType payload_type);
  int DestroySession();

  int Send(IFrameUnitsPtr p);

private:
  static unsigned int __stdcall SendProc(void *param);
  void DoSend();

private:

  std::string ip_;
  uint16_t port_;

  RtpSession *rtp_session_;

  CCritSec queue_lock_;
  IFrameUnitsQueue frame_units_queue_;
  HANDLE send_thread_;
  bool stop_thread_exec_;

  ILogger *logger_;
};


#endif

