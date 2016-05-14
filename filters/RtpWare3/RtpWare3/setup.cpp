
#pragma warning(disable:4995)

#include <streams.h>
#include <initguid.h>

#include "guids.h"
#include "rtp_sender_filter.h"
#include "rtp_receiver_filter.h"
#include "Log/Log.h"
#include "ortp_logger.h"

#include "ortp/ortp.h"


// Setup data

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_NULL,            // Major type
	&MEDIASUBTYPE_NULL          // Minor type
};

const AMOVIESETUP_PIN sudRendererPins =
{
	L"Input",                   // Pin string name. Obsolete, not used.
	FALSE,                      // Is it rendered
	FALSE,                      // Is it an output
	FALSE,                      // Allowed none
	FALSE,                      // Likewise many
	&CLSID_NULL,                // Connects to filter. Obsolete.
	L"Output",                  // Connects to pin. Obsolete.
	1,                          // Number of types
	&sudPinTypes                // Pin information
};

const AMOVIESETUP_FILTER sudRenderer =
{
	&CLSID_SCUTRtpSender,  // Filter CLSID
	g_wszSCUTRtpSenderFilter,	// String name
	MERIT_DO_NOT_USE,				// Filter merit
	1,								// Number pins
	&sudRendererPins				// Pin details
};

const AMOVIESETUP_PIN sudSourcePin = 
{
	L"Output",      // Obsolete, not used.
	FALSE,          // Is this pin rendered?
	TRUE,           // Is it an output pin?
	FALSE,          // Can the filter create zero instances?
	FALSE,          // Does the filter create multiple instances?
	&CLSID_NULL,    // Obsolete.
	NULL,           // Obsolete.
	1,              // Number of media types.
	&sudPinTypes    // Pointer to media types.
};

const AMOVIESETUP_FILTER sudPushSource =
{
	&CLSID_SCUTRtpReceiver,	// Filter CLSID
	g_wszSCUTRtpReceiverFilter,	// String name
	MERIT_DO_NOT_USE,				// Filter merit
	1,								// Number pins
	&sudSourcePin					// Pin details
};


//
//  Object creation stuff
//
CFactoryTemplate g_Templates[]= {
	{ 
		g_wszSCUTRtpSenderFilter,
		&CLSID_SCUTRtpSender, 
		CRtpSenderFilter::CreateInstance, 
		NULL, 
		&sudRenderer 
	},
	{ 
		g_wszSCUTRtpReceiverFilter,		  // Name
		&CLSID_SCUTRtpReceiver,        // CLSID
		CRtpReceiverFilter::CreateInstance, // Method to create an instance of MyComponent
		NULL,								  // Initialization function
		&sudPushSource						  // Set-up information (for filters)
	}
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterSever
//
// Handle the registration of this filter
//
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  dwReason, 
					  LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}


//OrtpInitializeHelper帮助初始化和释放ortp库
//ortp库全局只能初始化一次，且在DLL卸载时应该销毁。
//由于音视频要创建两个filter，其创建和销毁的先后顺序也是不定的，
//所以不能在任何一个filter的构造函数和析构函数中做这项工作。
//我们创建一个不在堆上的全局变量对象，在程序退出卸载DLL时会销毁他，
//在其析构函数中销毁ortp即可确保安全。

class GlobalInitializeHelper
{
public:
  GlobalInitializeHelper()
  {
    //logger
    SetLogComponent("rtp");

    //ortp
    ortp_init();
    ortp_set_log_level_mask(/*ORTP_WARNING|*/ORTP_ERROR|ORTP_FATAL);
    ortp_set_log_handler(OrtpLogHandler);
    //ortp_scheduler_init(); //没用，我们不用调度器。如果用了，记得要调用ortp_exit()，不然程序退出时会阻塞
    LOG_PRINTF("ortp init");
  }
  ~GlobalInitializeHelper()
  {
    ortp_exit();
    LOG_PRINTF("ortp exit");
  }
} global_init_helper;

