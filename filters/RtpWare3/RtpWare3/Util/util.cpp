#include "util.h"

#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdlib.h>

StreamMediaType GetRtpStatMedaiTypeFromPayload( RtpPayloadType type )
{
  switch (type)
  {
  case kPayloadSpeex:
  case kPayloadAAC:
    return eMT_Audio;
    break;
  case kPayloadH264:
    return eMT_Video;
    break;
  default:
    return eMT_Other;
    break;
  }
  return eMT_Other;
}

long long GetDShowTimeFromNTPTime(unsigned long long ntp_ts)
{
  long long dshow_ts = 0;
  //NTP时间格式为：64位定点数，高32位为整数部分，低32位为小数部分，以1900-1-1以来的秒为单位计时
  //DShow时间格式为：64位整数，以1970-1-1以来的100纳秒数为单位计时
  const unsigned long long ntp_t_from_1900_to_1970 = 0x83AA7E80LL << 32; /* 0x83AA7E80 is the number of seconds from 1900 to 1970 */
  if (ntp_ts >= ntp_t_from_1900_to_1970) 
    dshow_ts = (long long)((ntp_ts - ntp_t_from_1900_to_1970)/(double)(1LL << 32) * 1.0e7); /* 100ns精度，1s含有10e7个单位 */
  return dshow_ts;
}

unsigned long GetIPv4Data( addrinfo *addr )
{
  return (addr ? (reinterpret_cast<sockaddr_in*>(addr->ai_addr))->sin_addr.S_un.S_addr : 0);
}

in6_addr *GetIPv6Data( addrinfo *addr )
{
  return (addr ? &((reinterpret_cast<sockaddr_in6*>(addr->ai_addr))->sin6_addr) : nullptr);
}

bool IsLoopbackAddr( addrinfo *addr )
{
  //ipv4地址用unsigned long表示，win是小端编码，故127.0.0.0/8类地址应判断高八位是否等于127
  return ((IsIPv4Addr(addr) && ((GetIPv4Data(addr) & 0xFF) == 127u)) || 
    (IsIPv6Addr(addr) && IN6_IS_ADDR_LOOPBACK(GetIPv6Data(addr))));
}

bool IsMulticastAddr( addrinfo *addr )
{
  return ((IsIPv4Addr(addr) && (IN_MULTICAST(htonl(GetIPv4Data(addr)))) ) ||
    (IsIPv6Addr(addr) && IN6_IS_ADDR_MULTICAST(GetIPv6Data(addr))));
}

int AddrStrToAddrInfo( const char *ip, addrinfo **addr )
{
  if (!addr) return -1;
  *addr = NULL;

  //地址解析
  if (GetAddrInfo(ip, NULL, NULL, addr) != 0) {
    return -1 ;
  }
  //FreeAddrInfoA(aiList);
  return 0;
}

//以下两个获取本机IP的函数也是某不留名师兄写的了，感谢

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 获取本机IPv4地址
/// @param [in,out]	pHostIP	IPv4地址输出字符串，长度至少为16
/// @return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetLocalIPv4Addr(char* pHostIP, unsigned short len)
{
  PIP_ADAPTER_INFO pAdapterInfo = NULL;
  DWORD dwRetVal = 0;
  ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
  if(pAdapterInfo == NULL)
  {
    return false;
  }
  // Make an initial call to GetAdaptersInfo to get
  // the necessary size into the ulOutBufLen variable
  if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
  {
    free(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
    if(pAdapterInfo == NULL)
    {
      //  printf("Error allocating memory needed to call GetAdaptersinfo\n");
      return false;
    }
  }
  bool succeed = false;
  if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
  {
    //读取成功
    PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
    while(pAdapter)
    {
      //提取第一个地址
      //strncpy(pHostIP, pAdapter->GatewayList.IpAddress.String, strlen(pAdapter->GatewayList.IpAddress.String));
      sprintf_s(pHostIP, len, "%s", pAdapter->IpAddressList.IpAddress.String);
      succeed = true;
      break;
    }
  }
  else
  {
    //  printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
  }
  if(pAdapterInfo) free(pAdapterInfo);
  return succeed;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	 获取本机IPv6地址，优先使用临时IPv6地址
/// @param [in,out]	pHostIP	IPv6地址输出字符串，长度至少为40
/// @return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetLocalIPv6Addr(char* pHostIP, unsigned short len)
{
  /* Declare and initialize variables */
  DWORD dwSize = 0;
  DWORD dwRetVal = 0;

  unsigned int i = 0;

  // Set the flags to pass to GetAdaptersAddresses
  ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS ;//GAA_FLAG_INCLUDE_PREFIX;

  // default to ipv6 address family
  ULONG family = AF_INET6;

  LPVOID lpMsgBuf = NULL;

  PIP_ADAPTER_ADDRESSES pAddresses = NULL;
  ULONG outBufLen = 0;
  ULONG Iterations = 0;

  PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
  PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
  PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
  PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
  IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
  IP_ADAPTER_PREFIX *pPrefix = NULL;
  // Allocate a 15 KB buffer to start with.
  outBufLen = 15000;

  do
  {
    pAddresses = (IP_ADAPTER_ADDRESSES *) malloc(outBufLen);
    if (pAddresses == NULL)
    {
      // printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
      return false;
    }
    dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
    if (dwRetVal == ERROR_BUFFER_OVERFLOW)
    {
      free(pAddresses);
      pAddresses = NULL;
    }
    else
    {
      break;
    }
    Iterations++; //重试次数

  }
  while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3)); //重试3次

  bool succeed = false;
  char ipBuffer[MAX_IP_STRING_LENGTH] = {}; //longer than or equal
  DWORD bufflen = MAX_IP_STRING_LENGTH;
  PSOCKADDR_IN6 addr;   //Important! It switches the normal sockaddr into the IPv6 type.
  if (dwRetVal == NO_ERROR)
  {
    // If successful, output some information from the data we received
    pCurrAddresses = pAddresses;
    while (pCurrAddresses)
    {
      if (pCurrAddresses->FirstUnicastAddress == NULL)
      {
        pCurrAddresses = pCurrAddresses->Next;
        continue;
      }
      PIP_ADAPTER_UNICAST_ADDRESS unicastAddress;
      //如果第二个IPv6地址有效，则使用第二个(临时IPv6地址)
      unicastAddress = pCurrAddresses->FirstUnicastAddress->Next != NULL ? pCurrAddresses->FirstUnicastAddress->Next :
        pCurrAddresses->FirstUnicastAddress;
      //获取主机地址（取第一个地址）
      addr = (struct sockaddr_in6 *)unicastAddress->Address.lpSockaddr; // Switch ing it!

      //解析成字符串格式
      long length = MAX_IP_STRING_LENGTH;
      WSAAddressToStringA((sockaddr*)(addr), sizeof(sockaddr_in6), NULL, ipBuffer, reinterpret_cast<LPDWORD>(&length));

      strcpy_s(pHostIP, len, ipBuffer);
      //跳出循环
      succeed = true;
      break;
    }
  }
  else
  {
    //printf("Call to GetAdaptersAddresses failed with error: %d\n", dwRetVal);
  }
  if (pAddresses)
  {
    free(pAddresses);
  }

  return succeed;
}

bool IsIPv6Addr( addrinfo *addr )
{
  return (addr && addr->ai_family == PF_INET6);
}

bool IsIPv4Addr( addrinfo *addr )
{
  return (addr && addr->ai_family == PF_INET);
}

int GetInfoFromSockAddrStorage(const sockaddr_storage *sock_addr_storage, 
                               unsigned short sock_addr_len, 
                               char *ip_buf, int ip_buf_len, unsigned short *port)
{
  if (!sock_addr_storage) return -1;

  if (getnameinfo((struct sockaddr*)sock_addr_storage, sock_addr_len,
    ip_buf, ip_buf_len, 0, 0, NI_NUMERICHOST) != 0) {
      return -1;
  }

  switch(sock_addr_storage->ss_family)
  {
  case AF_INET:
    {
      if (port) {
        *port = ntohs(((sockaddr_in *)sock_addr_storage)->sin_port);
      }
    }
    break;
  case AF_INET6:
    {
      if (port) {
        *port = ntohs(((sockaddr_in6 *)sock_addr_storage)->sin6_port);
      }
    }
    break;
  default:
    return -1;
  }
  return 0;
}

static unsigned __int64 CalculateMicroseconds(unsigned __int64 performancecount,unsigned __int64 performancefrequency)
{
  unsigned __int64 f = performancefrequency;
  unsigned __int64 a = performancecount;
  unsigned __int64 b = a/f;
  unsigned __int64 c = a%f; // a = b*f+c => (a*1000000)/f = b*1000000+(c*1000000)/f

  return b*1000000ui64+(c*1000000ui64)/f;
}

/**   
 *  Returns an timeval struct representing the current wallclock time. This is expressed 
 *  as a number of seconds since 00:00:00 UTC, January 1, 1970.
 */
int gettimeofday(struct timeval *tv, void* tz)
{
	static int inited = 0;
	static unsigned __int64 microseconds, initmicroseconds;
	static LARGE_INTEGER performancefrequency;

	unsigned __int64 emulate_microseconds, microdiff;
	SYSTEMTIME systemtime;
	FILETIME filetime;

	LARGE_INTEGER performancecount;

	QueryPerformanceCounter(&performancecount);
    
	if(!inited){
		inited = 1;
		QueryPerformanceFrequency(&performancefrequency);
		GetSystemTime(&systemtime);
		SystemTimeToFileTime(&systemtime,&filetime);
		microseconds = ( ((unsigned __int64)(filetime.dwHighDateTime) << 32) + (unsigned __int64)(filetime.dwLowDateTime) ) / 10ui64;
		microseconds-= 11644473600000000ui64; // EPOCH
		initmicroseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart);
	}
    
	emulate_microseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart);

	microdiff = emulate_microseconds - initmicroseconds;
  
    tv->tv_sec =(uint32_t)((microseconds + microdiff) / 1000000ui64);
    tv->tv_usec = (uint32_t)((microseconds + microdiff) % 1000000ui64);
    return (0);
}

long long GetDShowTimeFromUnixTime( struct timeval *tv )
{
  if (!tv) {
    struct timeval now;
    gettimeofday(&now, NULL);
    tv = &now;
  }
  return (long long)(tv->tv_sec * 10000000LL + tv->tv_usec * 10);
}
