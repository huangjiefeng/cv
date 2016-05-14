
#ifndef __ISCUT_RTP_SENDER_CFG_H
#define __ISCUT_RTP_SENDER_CFG_H

#include "../include/RtpStat.h"

//GUID must by the same as the definition in the guid.h
MIDL_INTERFACE("A0904DEC-71DF-4AF4-8B7D-3FDB2067FDB6")
ISCUTRtpSender : public IUnknown
{
public:

  struct ConnectionParam {
    const char * ip_addr;
    unsigned short port;
    //callback
    void *callback_obj;
    RtpStatCallback callback_func;
    //QoS(reserved)
    unsigned int dscp;
  };

public:
  //Add connection.
  virtual HRESULT AddConnection(const ConnectionParam &param) = 0;

  //Remove the connection specified. Return S_FALSE if not exist.
  //Specially, if the ip is NULL, all connection will be removed.
  virtual HRESULT RemoveConnection(const char *ip, unsigned short port) = 0;

  virtual HRESULT RemoveAllConnection() = 0;

  //Get current connection count. If not return S_OK, the count is indeterminate.
  virtual unsigned long ConnectionCount() = 0;

  //Start or stop sending to the specified ip and port. Returns E_FAIL if connection 
  //not exist. Specially, it controls all connection if ip is NULL.
  virtual HRESULT ControlSending(const char *ip, unsigned short port, bool send) = 0;

  virtual HRESULT ControlAllSending(bool send) = 0;

};


#endif