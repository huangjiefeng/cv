#ifndef SCHEDULER_TYPES_H
#define SCHEDULER_TYPES_H

#include <QString>

struct NetworkInfo 
{
  QString adapter_description;
  unsigned long adapter_index;
  quint64 transmit_bandwidth; // 发送带宽,bit
  quint64 receive_bandwidth; // 接受带宽,bit
  quint64 out_bytes; // 总计发送字节数
  quint64 out_speed_in_bytes; // 发送速度
  quint64 in_bytes;
  quint64 in_speed_in_bytes;

  NetworkInfo() :
    adapter_index(-1),
    transmit_bandwidth(0),
    receive_bandwidth(0),
    out_bytes(0),
    out_speed_in_bytes(0),
    in_bytes(0),
    in_speed_in_bytes(0)
  {}
};

#endif