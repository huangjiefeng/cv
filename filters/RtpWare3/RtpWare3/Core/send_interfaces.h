/************************************************************************/
/* 此处定义发送器各个模块的接口                                         */
/************************************************************************/

#ifndef SEND_INTERFACES_H
#define SEND_INTERFACES_H

#include "core_defs.h"
#include "RtpStat.h"
#include <string>
#include <memory>
#include <queue>

//
// rtp负载单元
//
class ISendUnit
{
public:
  virtual ~ISendUnit() {}

  virtual unsigned char * GetData() = 0;
  virtual unsigned long GetSize() = 0;
};

typedef std::shared_ptr<ISendUnit> ISendUnitPtr;
typedef std::queue<ISendUnitPtr> ISendUnitQueue;

//
// 组成一帧的所有单元
//
class IFrameUnits
{
public:
  virtual ~IFrameUnits() {}

  virtual unsigned long GetTimestamp() = 0;
  virtual void SetTimestamp(unsigned long) = 0;
  virtual void PushUnit(ISendUnitPtr p) = 0;
  virtual ISendUnitPtr PopUnit() = 0;
  virtual bool Empty() = 0; //是否为空
  virtual IFrameUnits *Clone() = 0; //复制一份自身
};

typedef std::shared_ptr<IFrameUnits> IFrameUnitsPtr;
typedef std::queue<IFrameUnitsPtr> IFrameUnitsQueue;


//
//数据包发送器
//
class IRtpSender
{
public:
  virtual ~IRtpSender() {}

  virtual int InitSession(std::string ip, 
    unsigned short port, 
    RtpPayloadType payload_type) = 0;
  virtual int DestroySession() = 0;

  virtual int Send(IFrameUnitsPtr p) = 0;
};


//
//帧拆分器，将一帧数据拆分为多个可发送的数据包单元
//

class IFrameSplitter
{
public:
  virtual ~IFrameSplitter() {}

  virtual IFrameUnitsPtr Split(const unsigned char *buf, 
    unsigned long size,
    long long timestamp) = 0;
};

#endif
