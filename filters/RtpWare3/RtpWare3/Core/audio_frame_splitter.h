#ifndef AUDIO_FRAME_SPLITTER_H
#define AUDIO_FRAME_SPLITTER_H

#include "send_interfaces.h"
#include "Log/Log.h"

#include "streams.h"

class AudioFrameSplitter : public IFrameSplitter
{
public:
  AudioFrameSplitter(const CMediaType &type, ILogger *logger);
  ~AudioFrameSplitter();

  virtual IFrameUnitsPtr Split(const unsigned char *buf, 
    unsigned long size, long long timestamp);

protected:
  double ratio_;
  ILogger *logger_;
};

#endif
