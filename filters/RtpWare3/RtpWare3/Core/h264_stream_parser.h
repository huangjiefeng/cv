#pragma once

#include "h264_nal.h"

class BitStream;

class H264StreamParser
{
public:
  H264StreamParser(void);
  ~H264StreamParser(void);

  int Parse(const unsigned char *bs, unsigned int size, H264NalList &nals);

private:
  int ParseNAL(H264Nal::NALHeader *nal);
  int ParseSlice(H264Nal::SliceHeader *slice);
  int ParseSvcPrefix(H264Nal::SVCPrefixHeader *prefix);

private:
  BitStream *bit_stream_;

};

