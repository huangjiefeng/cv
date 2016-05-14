#ifndef UTIL_H
#define UTIL_H

#include "../Core/core_defs.h"
#include "RtpStat.h"

#include <WinSock2.h>
#include <ws2ipdef.h>

//从filter定义的RtpPayloadType获取RTP统计报告的StreamMediaType
StreamMediaType GetRtpStatMedaiTypeFromPayload(RtpPayloadType type);

//从NTP时间转换成DShow的绝对时间（即以100ns为精度的1970-1-1以来的时间）
long long GetDShowTimeFromNTPTime(unsigned long long ntp_ts);

//Unix标准的获取1970-1-1以来的时间的API
int gettimeofday(struct timeval *tv, void* tz);

//从Unix时间转换为DShow时间，tv==nullptr为获取当前时间
long long GetDShowTimeFromUnixTime(struct timeval *tv);

//将DShow时间戳转成毫秒精度
#define Ts2Ms(_ts_) ((_ts_)/10000LL)

//addrinfo相关
bool IsIPv6Addr(addrinfo *addr);
bool IsIPv4Addr(addrinfo *addr);

unsigned long GetIPv4Data(addrinfo *addr);
in6_addr *GetIPv6Data(addrinfo *addr);

bool IsLoopbackAddr(addrinfo *addr);
bool IsMulticastAddr(addrinfo *addr);

//从字符串地址获取Windows内部的addrinfo结构
int AddrStrToAddrInfo(const char *ip, addrinfo **addr);

bool GetLocalIPv4Addr(char *ip_buf, unsigned short len);
bool GetLocalIPv6Addr(char *ip_buf, unsigned short len);

int GetInfoFromSockAddrStorage(const sockaddr_storage *sock_addr_storage, 
                               unsigned short sock_addr_len, 
                               char *ip_buf, int ip_buf_len, unsigned short *port);

#endif
