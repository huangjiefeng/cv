

#pragma warning(disable:4995)

#include <windows.h>
#include <shlwapi.h>
#include "Util.h"
#include "util/ini/TxConfig.h"

/************************************************************************/
/* Functions for string conversion                                      */
/************************************************************************/

std::wstring str2wstr(const std::string& str)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	const int nSize = str.size() + 1;
	size_t nConverted = 0;
	wchar_t *buff = new wchar_t[nSize];

	int nRet = mbstowcs_s(&nConverted, buff, nSize, str.c_str(), str.size());
	if (nRet !=0 || nConverted != str.size() + 1)
	{
		delete buff;
		setlocale(LC_ALL, curLocale.c_str());
		return L"";
	}

	std::wstring wstr(buff);
	delete buff;

	setlocale(LC_ALL, curLocale.c_str());
	return wstr;
}

std::string wstr2str(const std::wstring& wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	const int nSize = wstr.size() + 1;
	size_t nConverted = 0;
	char *buff = new char[nSize];

	int nRet = wcstombs_s(&nConverted, buff, nSize, wstr.c_str(), wstr.size());
	if (nRet !=0 || nConverted != wstr.size() + 1)
	{
		delete buff;
		setlocale(LC_ALL, curLocale.c_str());
		return "";
	}

	std::string str(buff);
	delete buff;

	setlocale(LC_ALL, curLocale.c_str());
	return str;
}

/************************************************************************/
/* Funtion for time                                                     */
/************************************************************************/

time_t SystemTimeToTimet( const SYSTEMTIME &st )
{
	FILETIME ft;
	SystemTimeToFileTime( &st, &ft );

	LONGLONG ll;

	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;

	time_t t = (DWORD)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);

	return t;

}


/************************************************************************/
/* Functions for path handle                                            */
/************************************************************************/

BOOL PathExist(std::string strRawPath)
{
	// App Dir
	if (strRawPath.empty() || strRawPath == "\\")
	{
		return TRUE;
	}

	std::string strPath = strRawPath;

	// Relative path
	if (strRawPath.size() < 2 || strRawPath[1] != ':')
	{
		strPath = GetAbsolutePath(strRawPath);
	}

	return PathFileExists(strPath.c_str());

}

BOOL ParentDirExist( const std::string& strPath )
{
	std::string strAbsolutePath;
	strAbsolutePath = GetAbsolutePath(strPath);
	int nPosDir = strAbsolutePath.find_last_of('\\');
	std::string strDir = strAbsolutePath.substr(0, nPosDir);
	return PathFileExists(strDir.c_str());

}

BOOL MakeDirRecursive( std::string strPath )
{
	int pos = 1;
	const int len = strPath.length();
	std::string szSubPath;

	while (pos <= len)
	{
		szSubPath = strPath.substr(0, pos);

		if ((strPath[pos] == '\\' || pos == len)  && !PathFileExists(szSubPath.c_str()))
		{
			BOOL rs = CreateDirectory(szSubPath.c_str(), NULL);
			if (!rs)
			{
				return FALSE;
			}
		}

		++pos;
	}

	return TRUE;

}

BOOL MakeParentDir( const std::string& strFilePath )
{
	int nPosDir = strFilePath.find_last_of('\\') + 1;
	std::string strDir = strFilePath.substr(0, nPosDir);

	return MakeDirRecursive(strDir);

}

BOOL GetApplicationDirPath(char * applicationDirPath /*= NULL*/, char * applicationName /*= NULL*/) 
{
	char szAppPath[MAX_PATH]      = {0};

	if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
	{
		printf("failed to get module file name!\n");
		return -1;
	}

	// Extract directory
	if(applicationDirPath) 
	{
		strncpy(applicationDirPath, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
		applicationDirPath[strlen(applicationDirPath)] = '\0';
	}

	// Extract application name
	if(applicationName) 
	{
		strncpy(applicationName, strrchr(szAppPath, '\\') + 1, strlen(strrchr(szAppPath, '\\')) - 5);	// ignore '\\' and .exe
		applicationName[strlen(applicationName)] = '\0';
	}

	return 0;
}

std::string GetAbsolutePath( const std::string& strRelative )
{
	char szAppPath[200] = {0};
	GetApplicationDirPath(szAppPath, NULL);
	std::string strDest;

	if (strRelative.empty())
	{
		strDest = szAppPath;
	}
	// Absolute path input
	else if (strRelative.size() > 2 && strRelative[1] == ':')
	{
		strDest = strRelative;
	}
	// Relative path
	else
	{
		if (strRelative[0] == '\\')
		{
			strDest = std::string(szAppPath) + strRelative;
		}
		else
		{
			strDest = std::string(szAppPath) + "\\" + strRelative;
		}
	}

	return strDest;

}

QString GetAbsolutePath(const QString &strRelative)
{
  char szAppPath[200] = {0};
  GetApplicationDirPath(szAppPath, NULL);
  QString strDest;

  if (strRelative.isEmpty())
  {
    strDest = szAppPath;
  }
  // Absolute path input
  else if (strRelative.size() > 2 && strRelative[1] == ':')
  {
    strDest = strRelative;
  }
  // Relative path
  else
  {
    if (strRelative[0] == '\\')
    {
      strDest = QString::fromLocal8Bit(szAppPath) + strRelative;
    }
    else
    {
      strDest = QString::fromLocal8Bit(szAppPath) + "\\" + strRelative;
    }
  }

  return strDest;
}

std::string GetTempFilePath( const std::string& strPath )
{
	std::string strTempPath;
	int nDot = strPath.find_last_of('.');
	strTempPath = strPath.substr(0, nDot) + "_tmp" + strPath.substr(nDot, strPath.size() - nDot);

	strTempPath = GetAbsolutePath(strTempPath);

	return strTempPath;

}

QString GetRelativePath(const QString &prefix, QString path)
{
  path.replace("/", "\\"); //将qt路径修正为windows路径格式
  int pos = path.indexOf(prefix);
  if (pos != -1) {
    return path.mid(pos + prefix.length());
  }
  return path;
}

QString GetDBFilePath( const QString &path )
{
  static QString library_path = CTxConfig::getInstance().GetUploadPath();
  return GetRelativePath(library_path, path);
}
