#ifndef __ISCUT_RTP_RECEIVER_CFG_H
#define __ISCUT_RTP_RECEIVER_CFG_H

#include "../include/RtpStat.h"

//GUID must by the same as the definition in the guid.h
MIDL_INTERFACE("BCFA2135-67FA-44AE-B490-BC5182C8AED0")
ISCUTRtpReceiver : public IUnknown
{
public:
  //Set id
  virtual HRESULT SetId(const char * id) = 0;
  virtual const char *GetId() = 0;

  //Set receiving addr, local ipv4 or ipv6 addr, and port
  virtual HRESULT SetLocalAddr(const char *ip, unsigned short port) = 0;
  virtual unsigned int GetSourcePort() = 0;

  //Set callback, use null to clear callback
  virtual HRESULT SetCallback(void *callback_obj, RtpStatCallback callback_func) = 0;

  //Set media type
  virtual HRESULT SetMediaType(const CMediaType &type) = 0;
  virtual HRESULT GetMediaType(CMediaType &type) = 0;

  //Start or Stop receive
  virtual HRESULT ControlRecv(bool recv) = 0;

};


#endif
