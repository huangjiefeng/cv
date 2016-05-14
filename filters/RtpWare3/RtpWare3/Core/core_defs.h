#ifndef CORE_DEFS_H
#define CORE_DEFS_H


enum RtpPayloadType
{
  kPayloadUnknown = 0,
  kPayloadSpeex = 50,
  kPayloadH264 = 96,
  kPayloadAAC = 97,
};

//最大rtp负载长度，1500减去IPv6（40B）、udp（8B）、rtp（12B）得到
const unsigned long max_rtp_payload_size = 1440;

#endif