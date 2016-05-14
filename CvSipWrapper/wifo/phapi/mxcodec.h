#ifndef MXCODEC_H
#define MXCODEC_H
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************************
 *										Codec MIME constants							   *
 *******************************************************************************************/

#ifdef USE_FLV1_OVER_H261
#define CODEC_FLV1_MIME_STRING "H261"
#else
#define CODEC_FLV1_MIME_STRING "H263-FLV1"
#endif

#define CODEC_MPG4_MIME_STRING "XVID"
#define CODEC_H264_MIME_STRING "H264"
#define CODEC_H263_MIME_STRING "H263"
#define CODEC_H263P_MIME_STRING "H263-1998"

#define CODEC_MPG2_MIME_STRING "MPEG2"

#define CODEC_AACLC_MIME_STRING "AAC_LC" // 注意这是非标准化的MIME
#define CODEC_SPEEX_MIME_STRING "SPEEX"

#ifdef __cplusplus
}
#endif

#endif