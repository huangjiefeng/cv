#include "h264_stream_parser.h"

#include "bit_stream.h"

#include <assert.h>


H264StreamParser::H264StreamParser(void)
  : bit_stream_(nullptr)
{
}


H264StreamParser::~H264StreamParser(void)
{
  if (bit_stream_) {
    delete bit_stream_;
    bit_stream_ = nullptr;
  }
}

int H264StreamParser::Parse( const unsigned char *bs, unsigned int size, H264NalList &nals )
{
  if (bit_stream_) {
    delete bit_stream_;
  }
  bit_stream_ = new BitStream((const char *)bs, size * 8);

  while (bit_stream_->MoveToNextNaluStart() != bit_stream_->EndPtr()) {
    H264Nal nal;

    //Ptr
    nal.data_ptr_ = bit_stream_->CurPtr();

    //Parse
    if (ParseNAL(&nal.nal_header_) == 0) {
      unsigned char type = nal.nal_header_.nal_unit_type;
      if (type == 1 || type == 5) {
        //nonIDR or IDR
        H264Nal::SliceHeader *slice = new H264Nal::SliceHeader;
        ParseSlice(slice);
        nal.rbsp_header_ = slice;
      }
      else if (type == 14) {
        //SVC Prefix
        H264Nal::SVCPrefixHeader *prefix = new H264Nal::SVCPrefixHeader;
        ParseSvcPrefix(prefix);
        nal.rbsp_header_ = prefix;
      }

      //¼ÆËãNALU³¤¶È
      bit_stream_->MoveToNaluEnd();
      nal.data_size_ = bit_stream_->CurPtr() - nal.data_ptr_;

      nals.push_back(std::move(nal));
    }
  }
  return 0;
}

int H264StreamParser::ParseNAL(H264Nal::NALHeader *header_out) {
  if (nullptr == bit_stream_ || nullptr == header_out) {
    return -1;
  }
  assert(1 == sizeof(H264Nal::NALHeader));
  if (!bit_stream_->ReadNextBytes(header_out, sizeof(H264Nal::NALHeader))) {
    return -2;
  }
  return 0;
}

int H264StreamParser::ParseSlice(H264Nal::SliceHeader *slice) {
  if (nullptr == bit_stream_ || nullptr == slice) {
    return -1;
  }

  memset(slice, 0, sizeof(H264Nal::SliceHeader));
  try {
    //first_mb_in_slice
    slice->first_mb_in_slice = (unsigned short)bit_stream_->ReadGolombCode();
    //slice_type
    slice->slice_type = (unsigned char)bit_stream_->ReadGolombCode();
  }
  catch(unsigned int) {
    return -2;
  }
  
  return 0;
}

int H264StreamParser::ParseSvcPrefix(H264Nal::SVCPrefixHeader *prefix) {
  if (nullptr == bit_stream_ || nullptr == prefix) {
    return -1;
  }

  assert(3 == sizeof(H264Nal::SVCPrefixHeader));
  if (!bit_stream_->ReadNextBytes(prefix, sizeof(H264Nal::SVCPrefixHeader))) {
    return -2;
  }
  return 0;
}

