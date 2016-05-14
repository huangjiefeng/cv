#ifndef H264_NAL_H
#define H264_NAL_H

#include <tchar.h>
#include <vector>

//在编译器配置中设置预处理器
//#define ENDIAN_LITTLE

class H264Nal
{
public:
  //constant
  //NAL类型及Slice类型，参见ISO14496-12
  enum NALType
  {
    NALType_Undef = 0, //未指定
    NALType_NonIDR,
    NALType_SliceA,
    NALType_SliceB,
    NALType_SliceC,
    NALType_IDR,
    NALType_SEI,
    NALType_SPS,
    NALType_PPS,
    NALType_Delim, //分隔符
    NALType_EndOfSeq, //序列结束
    NALType_EndOfStream, //流结束
    NALType_Filler, //填充数据
    //SVC Extend
    NALType_SVCPrefix = 14,
    NALType_SVCSPS,
    NALType_SVCExt = 20,
    NALType_PACSI = 30,
    NALType_SVCSpe = 31
  };

  enum SliceType
  {
    SliceType_P, SliceType_B, SliceType_I, SliceType_SP, SliceType_SI
  };

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
  bool IsIdrSlice() const;
  unsigned char GetSvcTid() const;

  void Print() const;

  static const TCHAR * NaluName(unsigned char type);
  static const TCHAR * SliceName(unsigned char type);
};

typedef std::vector<H264Nal> H264NalList;

#endif
