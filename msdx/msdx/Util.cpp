#include "StdAfx.h"
#include "DxDefinitions.h"
#include "Util.h"
#include "log/Log.h"
#include "intel\mfxvideo.h"
#include <errors.h>

using namespace msdx;

Util::Util(void)
{
}

Util::~Util(void)
{
}

std::wstring Util::str2wstr(const std::string & str)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	int size = sizeof(wchar_t) * (str.size() + 1);
	wchar_t *buff = (wchar_t*) malloc(size);
	memset(buff, NULL, size);
	mbstowcs(buff, str.c_str(), str.size() + 1);
	std::wstring wstr(buff);
	free(buff);

	setlocale(LC_ALL, curLocale.c_str());
	return wstr;
}

std::string Util::wstr2str(const std::wstring & wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	int size = 2 * sizeof(char) * (wstr.size() + 1);
	char *buff = (char*) malloc(size);
	memset(buff, NULL, size);
	wcstombs(buff, wstr.c_str(), size - 1);
	std::string str(buff);
	free(buff);

	setlocale(LC_ALL, curLocale.c_str());
	return str;
}

// std::string CUtil::mfcstr2str(const CString &strMFC)
// {
// #ifdef UNICODE
// 	char * szText = new char[2 * strMFC.GetLength() + 1];
// 	memset(szText, NULL, 2 * strMFC.GetLength() + 1);
// 
// 	wchar_t * wszText = new wchar_t[strMFC.GetLength() + 1];
// 	for (UINT i = 0; i < strMFC.GetLength(); ++i)
// 	{
// 		wszText[i] = strMFC[i];
// 		wszText[i+1] = NULL;
// 	}
// 	WideCharToMultiByte(CP_ACP, 0, wszText, strMFC.GetLength(), szText, 2 * strMFC.GetLength(), NULL, NULL);
// #else
// 	char * szText = strMFC.GetBuffer();
// #endif
// 	std::string str = szText;
// 	delete szText;
// 	delete wszText;
// 
// 	return str;
// }

// CString CUtil::str2mfcstr(const std::string & str)
// {
// #ifdef UNICODE
// 	wchar_t * wszText = new wchar_t[str.length() + 1];
// 	memset(wszText, NULL, (str.length() + 1) * sizeof(wchar_t));
// 
// 	const char * szText = str.c_str();
// 
// 	MultiByteToWideChar(CP_ACP, 0, szText, str.length(), wszText, str.length());
// 
// 	CString strMFC = wszText;
// 	delete wszText;
// 	return strMFC;
// #else
// 	char * szText = str.c_str();
// 	return CString(szText);
// #endif
//}

// CString CUtil::wstr2mfcstr(const std::wstring & wstr)
// {
// 	std::string str = wstr2str(wstr);
// 	return str2mfcstr(str);
// }
// 
// std::wstring CUtil::mfcstr2wstr(const CString & wstr)
// {
// 	std::string str = mfcstr2str(wstr);
// 	return str2wstr(str);
// }

// CString CUtil::hr2mfcstr( CString strMsg, HRESULT hr )
// {
// 	TCHAR tcsHr[128] = {0};
// 	AMGetErrorText(hr, tcsHr, sizeof(tcsHr)/sizeof(TCHAR));
// 
// 	CString str;
// 	str.Format(_T(" (0x%08x: %s)"), hr, tcsHr);
// 	return strMsg + str;
// }

BOOL Util::wstrCmpWithUnknownChar( const std::wstring &str1, const std::wstring &str2 )
{
	if (str1.size() != str2.size())
	{
		return FALSE;
	}

	for (unsigned int i = 0; i < str1.size(); ++i)
	{
		if (str1[i] == '?' || str2[i] == '?') continue;
		if (str1[i] != str2[i]) return FALSE;
	}

	return TRUE;
}

bool Util::CheckIntelHwCodecSupport()
{
    static bool checked = false;
    static mfxStatus checkResult = MFX_ERR_NONE;

    //Ruan 2012/09/28 :首次调用本函数时进行硬编码支持测试，此后均返回首次测试的结果
    if(checked == false)
    {
        mfxSession session = (mfxSession) 0;
        mfxIMPL impl = MFX_IMPL_HARDWARE_ANY;
        mfxU16 APIVerMinor = 1, APIVerMajor = 1;
        mfxVersion version = {APIVerMinor, APIVerMajor};
        checkResult = MFXInit(impl, &version, &session);	//使用硬编码格式初始化环境,如果不支持硬编码则初始化失败
        MFXClose(session);
        session = (mfxSession) 0;
        checked = true;
    }

    if(checkResult == MFX_ERR_NONE)
    {
        LOG_PRINTF("HWCheck: The result is true");
        return true;
    }
    LOG_PRINTF("HWCheck: The result is false");
    return false;
}

//参考自http://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
const char * Util::GetCrossbarPhysicalPinName( long lType )
{
    switch (lType) 
    {
    case PhysConn_Auto:                   return "Auto";
    case PhysConn_Video_Tuner:            return "Video Tuner";
    case PhysConn_Video_Composite:        return "Video Composite";
    case PhysConn_Video_SVideo:           return "S-Video";
    case PhysConn_Video_RGB:              return "Video RGB";
    case PhysConn_Video_YRYBY:            return "Video YRYBY";
    case PhysConn_Video_SerialDigital:    return "Video Serial Digital";
    case PhysConn_Video_ParallelDigital:  return "Video Parallel Digital"; 
    case PhysConn_Video_SCSI:             return "Video SCSI";
    case PhysConn_Video_AUX:              return "Video AUX";
    case PhysConn_Video_1394:             return "Video 1394";
    case PhysConn_Video_USB:              return "Video USB";
    case PhysConn_Video_VideoDecoder:     return "Video Decoder";
    case PhysConn_Video_VideoEncoder:     return "Video Encoder";

    case PhysConn_Audio_Tuner:            return "Audio Tuner";
    case PhysConn_Audio_Line:             return "Audio Line";
    case PhysConn_Audio_Mic:              return "Audio Microphone";
    case PhysConn_Audio_AESDigital:       return "Audio AES/EBU Digital";
    case PhysConn_Audio_SPDIFDigital:     return "Audio S/PDIF";
    case PhysConn_Audio_SCSI:             return "Audio SCSI";
    case PhysConn_Audio_AUX:              return "Audio AUX";
    case PhysConn_Audio_1394:             return "Audio 1394";
    case PhysConn_Audio_USB:              return "Audio USB";
    case PhysConn_Audio_AudioDecoder:     return "Audio Decoder";

    default:                              return "Unknown Type";
    }    
}

