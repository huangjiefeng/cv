
#include "stdafx.h"
#include "util.h"
#include "RecordStat.h"

#include <windows.h>
#include <IPHlpApi.h>
#include <winsock.h>
#include <io.h> // for _access

int getApplicationDirPath(char * applicationDirPath, size_t len) 
{
	char szAppPath[MAX_PATH]      = "";
	//char szAppDirectory[MAX_PATH] = "";

	if(!applicationDirPath) 
		return -1;

	if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
	{
		printf("failed to get module file name!\n");
		return -1;
	}

	// Extract directory
	int nPathLen = strrchr(szAppPath, '\\') - szAppPath;
	strncpy_s(applicationDirPath, len, szAppPath, nPathLen);
	applicationDirPath[nPathLen] = '\0';

	return 0;
}

int CreateParentDirectory( LPCTSTR lpszPath )
{
	int pos = 0;
	const int len = _tcslen(lpszPath);
	TCHAR szSubPath[256] = {0};

	while (pos < len)
	{
		szSubPath[pos] = lpszPath[pos];

		if (lpszPath[pos] == _T('\\') && _access(szSubPath, 0) == -1)
		{
			BOOL rs = CreateDirectory(szSubPath, NULL);
			if (!rs)
			{
				return -1;
			}
		}

		++pos;
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
  
    tv->tv_sec = (long)((microseconds + microdiff) / 1000000ui64);
    tv->tv_usec = (long)((microseconds + microdiff) % 1000000ui64);

    tz; //消除未引用的变量警告
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

void PrintFileName(LPCTSTR fmt, LPTSTR buf, const int epi, 
                   const TypeHandler * type, long long ts)
{
    time_t timer;
    const int BUF_SIZE = 32;
    char buffer[BUF_SIZE];
    tm tm_info;

    time(&timer);
    localtime_s(&tm_info, &timer);

    std::string src = fmt;
    std::string suffix;
    int dot_pos = src.find_last_of('.');
    if (dot_pos != std::string::npos) {
        suffix = src.substr(dot_pos); //带.的后缀名
        src = src.substr(0, dot_pos);
    }

    unsigned int index = src.find(TX_FNFMT_DATE);
    if (index != std::string::npos)
    {
        strftime(buffer, BUF_SIZE, "%Y-%m-%d", &tm_info);
        src.replace(index, strlen(TX_FNFMT_DATE), buffer);
    }

    index = src.find(TX_FNFMT_DATETIME);
    if (index != std::string::npos)
    {
        strftime(buffer, BUF_SIZE, "%y%m%d-%H%M%S", &tm_info);
        src.replace(index, strlen(TX_FNFMT_DATETIME), buffer);
    }

    index = src.find(TX_FNFMT_EPISODE);
    if (index != std::string::npos)
    {
        sprintf_s(buffer, BUF_SIZE, "%05d", epi);
        src.replace(index, strlen(TX_FNFMT_EPISODE), buffer);
    }

    index = src.find(TX_FNFMT_VINFO);
    if (index != std::string::npos)
    {
        if (type)
        {
            sprintf_s(buffer, BUF_SIZE, "%dp%I64d"
                , const_cast<TypeHandler *>(type)->Height()
                , UNITS / const_cast<TypeHandler *>(type)->FrameDuration());
            src.replace(index, strlen(TX_FNFMT_VINFO), buffer);
        }
    }

    if (ts > 0) 
    {
      sprintf_s(buffer, BUF_SIZE, "_ts%I64d", ts / 10000); // 100ns scale to ms scale
      src.append(buffer);
    }
    src.append(suffix);

    strcpy_s(buf, MAX_PATH, src.c_str());
}
