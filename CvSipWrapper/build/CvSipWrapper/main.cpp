#include <QtCore/QCoreApplication>
#include "CvSipWrapperService.h"

#include <Windows.h>
#include <psapi.h>
#include <log/log.h>


// 修正VS2012中Release编译时std::basic_string::npos找不到定义的问题. --Liaokz, 2013-9-22
#ifdef NDEBUG
template std::string::size_type  std::string::npos;
template std::wstring::size_type std::wstring::npos;

#if _MSC_VER >= 1600 

const std::basic_string<char>::size_type std::basic_string<char>::npos = (std::basic_string<char>::size_type) -1; 

#endif //_MSC_VER
#endif //NDEBUG


int main(int argc, char *argv[])
{
	SetErrorMode(SEM_FAILCRITICALERRORS |SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);

	QCoreApplication a(argc, argv);

	strcpy( Log::_logFileName , "CvSipWrapper" );
	::SetPriorityClass( ::GetCurrentProcess() , ABOVE_NORMAL_PRIORITY_CLASS );
	CvSipWrapperService::getInstance();

	return a.exec();
}
