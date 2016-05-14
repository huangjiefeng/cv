#ifndef AAC_FRAME_SPLITTER_H
#define AAC_FRAME_SPLITTER_H

#include "audio_frame_splitter.h"

class AACFrameSplitter : public AudioFrameSplitter
{
public:
  AACFrameSplitter(const CMediaType &type, ILogger *logger);
  ~AACFrameSplitter();

  virtual IFrameUnitsPtr Split( const unsigned char *buf, 
    unsigned long size, long long timestamp );

private:

};

#endif
