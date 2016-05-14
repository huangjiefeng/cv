#include "frame_units.h"


FrameUnits::FrameUnits()
  : timestamp_(0)
{
}

FrameUnits::FrameUnits( const FrameUnits &u )
{
  timestamp_ = u.timestamp_;
  queue_ = u.queue_;
}

FrameUnits::~FrameUnits()
{
  while (!queue_.empty()) {
    queue_.pop();
  }
}

unsigned long FrameUnits::GetTimestamp()
{
  return timestamp_;
}

void FrameUnits::SetTimestamp(unsigned long t)
{
  timestamp_ = t;
}

void FrameUnits::PushUnit(ISendUnitPtr p)
{
  queue_.push(p);
}

ISendUnitPtr FrameUnits::PopUnit()
{
  if (queue_.empty()) return nullptr;
  ISendUnitPtr p = queue_.front();
  queue_.pop();
  return p;
}

bool FrameUnits::Empty()
{
  return queue_.empty();
}

IFrameUnits * FrameUnits::Clone()
{
  IFrameUnits *obj = new FrameUnits(*this);
  return obj;
}
