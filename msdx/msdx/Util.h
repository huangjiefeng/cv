/************************************************************************/
/* 在此定义一些通用的工具方法                                           */
/************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <string>

class Util
{
public:
	Util(void);
	~Util(void);

    //字符串工具
	static std::wstring str2wstr(const std::string& str);
	static std::string wstr2str(const std::wstring& wstr);
	//static std::string mfcstr2str(const CString &strMFC);
	//static CString str2mfcstr(const std::string & str);
	//static CString wstr2mfcstr(const std::wstring & wstr);
	//static std::wstring mfcstr2wstr(const CString & wstr);

	//static CString hr2mfcstr(CString strMsg, HRESULT hr);
	static BOOL wstrCmpWithUnknownChar(const std::wstring &str1, const std::wstring &str2);

    //检查Intel硬编码支持
    static bool CheckIntelHwCodecSupport();

    //Crossbar类型名称
    static const char * GetCrossbarPhysicalPinName(long lType);
};

#endif
