#ifndef ORTP_UNIT_H
#define ORTP_UNIT_H

#include "send_interfaces.h"

typedef struct msgb mblk_t;

class OrtpUnit : public ISendUnit
{
public:
  OrtpUnit(mblk_t *m);
  OrtpUnit(const OrtpUnit &u);
  OrtpUnit(ISendUnit *u);
  ~OrtpUnit();

  virtual unsigned char * GetData();
  virtual unsigned long GetSize();

  //class specific member
  mblk_t *GetMsgb(); //返回内部ortp数据对象

private:
  mblk_t *m_;
};

#endif
