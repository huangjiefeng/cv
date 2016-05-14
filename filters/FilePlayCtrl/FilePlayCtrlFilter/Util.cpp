#include "Util.h"
#include <windows.h>
#include <IPHlpApi.h>

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
  
    tv->tv_sec = (long)((microseconds + microdiff) / 1000000ui64);
    tv->tv_usec = (long)((microseconds + microdiff) % 1000000ui64);
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

void GenerateRandomSyncId( char *buf, unsigned long len )
{
    const char ch_set[] = "0123456789abcdef";
    const unsigned long ch_set_len = strlen(ch_set);
    const unsigned long rand_len = 5;
    char rand_suffix[rand_len+1] = {0}; // ensure space for tail '\0'
    srand((unsigned int)time(NULL));
    for (unsigned long i = 0; i < rand_len; ++i) {
        rand_suffix[i] = ch_set[rand() % ch_set_len];
    }
    strcpy_s(buf, len, "FilePlayCtrl_");
    strcat_s(buf, len, rand_suffix);
}
