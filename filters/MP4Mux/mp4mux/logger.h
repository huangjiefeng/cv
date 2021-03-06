// Copyright (c) GDCL 2011-2013. All Rights Reserved. 
// You are free to re-use this as the basis for your own development,
// provided you retain this copyright notice in the source.
// http://www.gdcl.co.uk

// writes log output to a text file in home directory if present

#include <strsafe.h>
#include <ShlObj.h>
#include "util.h"

#define LOG_PRINT(fmt, ...) printf_s("[MP4Mux] "fmt"\n", __VA_ARGS__)

class Logger
{
public:
	Logger(const TCHAR* pFile)
	: m_hFile(NULL)
	, m_bInit(FALSE)
	{
		// to turn this on, create the file c:\GMFBridge.txt

		TCHAR szPath[MAX_PATH];
		/*if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, szPath)))
		{
			StringCbCat(szPath, sizeof(szPath), TEXT("\\"));
			StringCbCat(szPath, sizeof(szPath), pFile);
			if (GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES)
			{		
				m_hFile = CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
				if (m_hFile == INVALID_HANDLE_VALUE)
				{
					m_hFile = NULL;
				}
			}
		}*/

		if (m_hFile == NULL)
		{
			getApplicationDirPath(szPath, sizeof(szPath));
			StringCbCat(szPath, sizeof(szPath), TEXT("\\log\\"));
			StringCbCat(szPath, sizeof(szPath), pFile);
			CreateParentDirectory(szPath);
			LOG_PRINT("Create log file %s", szPath);
			/*if (GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES)
			{*/
				m_hFile = CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
			//}
		}
		if (m_hFile == INVALID_HANDLE_VALUE || m_hFile == NULL)
		{
			LOG_PRINT("Create log file %s failed", szPath);
		}
		if (m_hFile != NULL)
		{
			SetFilePointer(m_hFile, 0, NULL, FILE_END);
			m_msBase = timeGetTime();

			SYSTEMTIME st;
			GetLocalTime(&st);
			Log(TEXT("\r\n\r\nStarted %04d-%02d-%02d %02d:%02d:%02d\r\n"
				     "========================================="),
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond);
		}
		m_bInit = TRUE;
	}

	~Logger()
	{
		if (m_hFile != NULL)
		{
			CloseHandle(m_hFile);
		}
	}

	void Log(const TCHAR* pFormat, ...)
	{
		va_list va;
		va_start(va, pFormat);
		TCHAR  ach[4096];
		TCHAR* pEndTime = ach;
		size_t cb = sizeof(ach);
		TCHAR* pEnd = pEndTime;
		StringCbPrintfEx(ach, sizeof(ach), &pEndTime, &cb, 0, TEXT("%d:\t"), timeGetTime() - m_msBase);
		StringCbVPrintfEx(pEndTime, cb, &pEnd, NULL, 0, pFormat, va);
		va_end(va);

		if (m_bInit)
		{
			_tprintf_s(_T("[MP4Mux] %s\n"), pEndTime);
		}


		if (m_hFile != NULL)
		{
			// debug output without newline and without time (added by existing debug code)
			_bstr_t str = pEndTime;
			DbgLog((LOG_TRACE, 0, "%s", (char*)str));

			// add time at start and newline at end for file output
			if ((pEnd+2) < (ach + (sizeof(ach) / sizeof(ach[0]))))
			{
				*pEnd++ = TEXT('\r');
				*pEnd++ = TEXT('\n');
			}

			CAutoLock lock(&m_csLog);
			DWORD cActual;
			WriteFile(m_hFile, ach, DWORD((pEnd - ach) * sizeof(TCHAR)), &cActual, NULL);
		}
	}

private:
	CCritSec m_csLog;
	DWORD m_msBase;
	HANDLE m_hFile;
	BOOL m_bInit;
};
extern Logger theLogger;
#define LOG(x)	theLogger.Log x
