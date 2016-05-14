#ifndef RTP_RECEIVER_H
#define RTP_RECEIVER_H

#include <ortp/ortp.h>

#include "mediastreamer2/msqueue.h"


class RtpReceiver
{
public:
  RtpReceiver(void);
  ~RtpReceiver(void);

  int InitSession(uint16_t port);
  int DestroySession();

  int Receive(MSQueue &rtps, uint32_t &timestamp);

private:

  RtpSession *rtp_session_;  
  uint32_t last_recv_ts;

};

#endif
