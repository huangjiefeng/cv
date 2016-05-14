
#include <streams.h>
#include <initguid.h>

#include "guid.h"
#include "FilePlayCtrlFilter.h"
#include "log/Log.h"


const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_NULL,       // Major type
	&MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN psudPins[] =
{
	{
		L"Input",           // String pin name
			FALSE,              // Is it rendered
			FALSE,              // Is it an output
			FALSE,              // Allowed none
			FALSE,              // Allowed many
			&CLSID_NULL,        // Connects to filter
			L"Output",          // Connects to pin
			1,                  // Number of types
			&sudPinTypes
	},     // The pin details
	{
		L"Output",          // String pin name
			FALSE,              // Is it rendered
			TRUE,               // Is it an output
			FALSE,              // Allowed none
			FALSE,              // Allowed many
			&CLSID_NULL,        // Connects to filter
			L"Input",           // Connects to pin
			1,                  // Number of types
			&sudPinTypes        // The pin details
		}
};

const AMOVIESETUP_FILTER sudContrast =
{
	&CLSID_SCUTFilePlayCtrl,  // Filter CLSID
	L"SCUT File Play Controller",    // Filter name
	MERIT_DO_NOT_USE,       // Its merit
	2,                      // Number of pins
	psudPins                // Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = 
{
	{ 
		L"SCUT File Play Controller",
		&CLSID_SCUTFilePlayCtrl, 
		FilePlayCtrlFilter::CreateInstance, 
		NULL, 
		&sudContrast 
	},
	/*{
		L"Video Mixer Property Page",
		&CLSID_VideoMixerProp,
		CVideoMixerProp::CreateInstance,
		NULL,
		NULL
	}*/

};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);



//
// DllRegisterServer
//
// Handle registration of this filter
//
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2(TRUE);

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);

} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  dwReason, 
					  LPVOID lpReserved)
{
    SetLogComponent("play");
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}


