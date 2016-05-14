#ifndef RTP_RECEIVER_H
#define RTP_RECEIVER_H

#include "receive_interfaces.h"
#include "Log/Log.h"

//减少导入ortp头文件对其他模块的影响
typedef struct _RtpSession RtpSession;

class ORtpReceiver : public IRtpReceiver
{
public:
  ORtpReceiver(ILogger *logger);
  ~ORtpReceiver(void);

  int InitSession(std::string ip, unsigned short port, RtpPayloadType payload_type);
  int DestroySession();
  void ResetSession();

  int Receive(IRtpPacket *&packet);
  int GetStatistics(RtpStatItem &stat);
  void GetNTPReferenceTimestamp(unsigned long long &ntp_ts, unsigned long &ts);

private:
  void InitRtpStat();

private:
  RtpSession *rtp_session_;
  unsigned int last_recv_ts_;

  std::string ip_;
  unsigned short port_;
  RtpPayloadType payload_type_;
  RtpStatItem rtp_stat_;

  ILogger *logger_;

};

#endif
