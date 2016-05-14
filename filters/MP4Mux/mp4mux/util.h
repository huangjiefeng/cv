#ifndef UTIL_H
#define UTIL_H

#include "TypeHandler.h"

struct timeval;
int getApplicationDirPath(char * applicationDirPath, size_t len);
int CreateParentDirectory(LPCTSTR lpszPath);
void PrintFileName(LPCTSTR fmt, LPTSTR buf, const int epi, 
                   const TypeHandler * type, long long ts);

//returns time(in 100ns units) elapsed since 1970-1-1 00:00:00 UTC
long long GetDShowTimeFromUnixTime(struct timeval *tv);

#endif
