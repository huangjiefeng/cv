#include "h264_nal.h"

#include <memory.h>
#include <assert.h>

H264Nal::H264Nal()
  : rbsp_header_(nullptr)
  , data_ptr_(nullptr)
  , data_size_(0)
{
  memset(&nal_header_, 0, sizeof(nal_header_));
}

H264Nal::H264Nal(const H264Nal &other)
  : rbsp_header_(nullptr)
  , data_ptr_(nullptr)
  , data_size_(0)
{
  //TODO
  other;
  assert(false);
}

H264Nal::H264Nal(H264Nal &&other)
  : nal_header_(other.nal_header_)
  , rbsp_header_(other.rbsp_header_)
  , data_ptr_(other.data_ptr_)
  , data_size_(other.data_size_)
{
  memset(&other.nal_header_, 0, sizeof(nal_header_));
  other.rbsp_header_ = nullptr;
  other.data_ptr_ = nullptr;
  other.data_size_ = 0;
}

H264Nal::~H264Nal() {
  if (rbsp_header_) {
    delete rbsp_header_;
    rbsp_header_ = nullptr;
  }
}

H264Nal &H264Nal::operator=(const H264Nal &other) {
  if (&other == this) {
    return *this;
  }
  assert(false);
  return *this;
}

H264Nal &H264Nal::operator=(H264Nal &&other) {
  if (&other == this) {
    return *this;
  }

  nal_header_ = other.nal_header_;
  rbsp_header_ = other.rbsp_header_;
  data_ptr_ = other.data_ptr_;
  data_size_ = other.data_size_;

  memset(&other.nal_header_, 0, sizeof(nal_header_));
  other.rbsp_header_ = nullptr;
  other.data_ptr_ = nullptr;
  other.data_size_ = 0;

  return *this;
}

bool H264Nal::IsSvcPrefix() const {
  return nal_header_.nal_unit_type == NALType_SVCPrefix;
}

bool H264Nal::IsIdrSlice() const
{
  return nal_header_.nal_unit_type == NALType_IDR;
}

unsigned char H264Nal::GetSvcTid() const {
  if (rbsp_header_ && nal_header_.nal_unit_type == 14) {
    //Svc prefix
    const SVCPrefixHeader *prefix = static_cast<const SVCPrefixHeader *>(rbsp_header_);
    return prefix->temporal_id;
  }
  return 0;
}

void H264Nal::Print() const {
    unsigned char nal_type = nal_header_.nal_unit_type;
    if ((nal_type == 1 || nal_type == 5) && rbsp_header_) {
      unsigned char slice_type = static_cast<H264Nal::SliceHeader*>(rbsp_header_)->slice_type;
      _tprintf(_T("(%u,%s)"), nal_header_.nal_ref_idc, SliceName(slice_type));
    } else {
      _tprintf(_T("(%u,%s)"), nal_header_.nal_ref_idc, NaluName(nal_type));
    }

    //Futher
    if (nal_header_.nal_unit_type == 14 && rbsp_header_) {
      //SVC Prefix
      const SVCPrefixHeader *prefix = static_cast<const SVCPrefixHeader *>(rbsp_header_);
      _tprintf(_T("{prid=%u, tid=%u}"), prefix->priority_id, prefix->temporal_id);
    }
}


const TCHAR *NaluNameTable[] = {
  _T("Undef"), _T("nonIDR"), _T("A"), _T("B"), _T("C"),
  _T("IDR"), _T("SEI"), _T("SPS"), _T("PPS"), _T("Delim"),
  _T("EOSeq"), _T("EOS"), _T("Filler")
};

const TCHAR * H264Nal::NaluName( unsigned char type )
{
  if (type < 13) {
    return NaluNameTable[type];
  } else {
    switch (type)
    {
    case NALType_SVCPrefix:
      return _T("SVCPre");
      break;
    case NALType_SVCSPS:
      return _T("SVCSPS");
      break;
    case NALType_SVCExt:
      return _T("SVCExt");
      break;
    case NALType_PACSI:
      return _T("PACSI");
      break;
    case NALType_SVCSpe:
      return _T("SVCSpe");
      break;
    default:
      break;
    }
  }

  return _T("Rsvd");
}

const TCHAR *SliceNameTable[] = {
  _T("P"), _T("B"), _T("I"), _T("SP"), _T("SI")
};

const TCHAR * H264Nal::SliceName( unsigned char type ) 
{
  if (type > 9) {
    return _T("NaN");
  }
  type %= 5;
  return SliceNameTable[type];
}

