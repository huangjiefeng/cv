#ifndef H264_NAL_H
#define H264_NAL_H

#include <tchar.h>
#include <vector>

class H264Nal
{
public:
  //type
  struct NALHeader
  {
#ifdef ENDIAN_LITTLE
    unsigned char nal_unit_type:5;
    unsigned char nal_ref_idc:2;
    unsigned char forbidden_zero_bit:1;
#else
    unsigned char forbidden_zero_bit:1;
    unsigned char nal_ref_idc:2;
    unsigned char nal_unit_type:5;
#endif
  };

  struct SliceHeader 
  {
    unsigned short first_mb_in_slice;
    unsigned char slice_type;
  };

  struct SVCPrefixHeader
  {
#ifdef ENDIAN_LITTLE
    unsigned char priority_id:6; //Byte1
    unsigned char idr_flag:1;
    unsigned char :1;
    unsigned char quality_id:4; //Byte2
    unsigned char dependency_id:3;
    unsigned char no_inter_layer_pred_flag:1;
    unsigned char :2; //Byte3
    unsigned char output_flag:1;
    unsigned char discardable_flag:1;
    unsigned char use_ref_base_pic_flag:1;
    unsigned char temporal_id:3;
#else
    unsigned char :1;
    unsigned char idr_flag:1;
    unsigned char priority_id:6;
    unsigned char no_inter_layer_pred_flag:1;
    unsigned char dependency_id:3;
    unsigned char quality_id:4;
    unsigned char temporal_id:3;
    unsigned char use_ref_base_pic_flag:1;
    unsigned char discardable_flag:1;
    unsigned char output_flag:1;
    unsigned char :2;
#endif
  };


  //member
public:
  NALHeader nal_header_;
  void *rbsp_header_;
  const char *data_ptr_;
  unsigned int data_size_;


  //method
public:
  H264Nal();
  H264Nal(const H264Nal &);
  H264Nal(H264Nal &&);
  ~H264Nal();

  H264Nal &operator=(const H264Nal &);
  H264Nal &operator=(H264Nal &&);

  bool IsSvcPrefix() const;
  unsigned char GetSvcTid() const;

  void Print() const;

  static const TCHAR * NaluName(unsigned char type);
  static const TCHAR * SliceName(unsigned char type);
};

typedef std::vector<H264Nal> H264NalList;

#endif
