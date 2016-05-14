#ifndef ORTP_FRAME_UNIT_H
#define ORTP_FRAME_UNIT_H

#include "send_interfaces.h"


class FrameUnits : public IFrameUnits
{
public:
  FrameUnits();
  FrameUnits(const FrameUnits &u);
  ~FrameUnits();

  virtual unsigned long GetTimestamp();
  virtual void SetTimestamp( unsigned long );
  virtual void PushUnit( ISendUnitPtr p );
  virtual ISendUnitPtr PopUnit();
  virtual bool Empty();
  virtual IFrameUnits *Clone();

private:
  unsigned long timestamp_;
  ISendUnitQueue queue_;
};

#endif
