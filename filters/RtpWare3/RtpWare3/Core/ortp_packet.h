#ifndef ORTP_PACKET_H
#define ORTP_PACKET_H

#include "receive_interfaces.h"

typedef struct msgb mblk_t;

class ORtpPacket : public IRtpPacket
{
public:
  ORtpPacket(mblk_t *m);
  ORtpPacket(const ORtpPacket &p);
  ~ORtpPacket();

  //interface
  virtual bool GetMarkbit();
  virtual unsigned int GetSSRC();
  virtual unsigned int GetTimestamp();
  virtual unsigned short GetSeqNumber();
  virtual unsigned short GetPayloadType();
  virtual unsigned short GetPlayloadSize();
  virtual unsigned char * GetPayload();

  //class specific member
  mblk_t *GetMsgb(); //返回内部ortp数据对象

private:
  mblk_t *m_;
};


#endif
