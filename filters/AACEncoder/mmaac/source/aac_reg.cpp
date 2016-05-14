//-----------------------------------------------------------------------------
//
//	Monogram AAC Encoder
//
//	Base on GNU LGPL libfaac
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Registry Information
//
//-----------------------------------------------------------------------------

const AMOVIESETUP_MEDIATYPE sudPinTypes[] =   
		{ 
			{
				&MEDIATYPE_Audio, 
				&MEDIASUBTYPE_PCM
			},
			{
				&MEDIATYPE_Audio,
				&MEDIASUBTYPE_AAC
			}
		};

const AMOVIESETUP_PIN psudPins[] = 
		{ 
			{ 
				L"Input",
				FALSE,              // bRendered
                FALSE,              // bOutput
                FALSE,              // bZero
                FALSE,              // bMany
                &CLSID_NULL,        // clsConnectsToFilter
                NULL,               // strConnectsToPin
                1,                  // nTypes
                &sudPinTypes[0]		// lpTypes
			}      
             , 
			{ 
				L"Output",          // strName
                FALSE,              // bRendered
                TRUE,               // bOutput
                FALSE,              // bZero
                FALSE,              // bMany
                &CLSID_NULL,        // clsConnectsToFilter
                NULL,	            // strConnectsToPin
                1,                  // nTypes
                &sudPinTypes[1]		// lpTypes
			} 
		};   


const AMOVIESETUP_FILTER sudAACEncoder = 
		{ 
			&CLSID_MonogramAACEncoder,		// clsID
            L"MONOGRAM AAC Encoder",		// strName
            MERIT_NORMAL,					// dwMerit
            2,								// nPins
            psudPins						// lpPin 
		};                     

CFactoryTemplate g_Templates[]=
		{
			{
				L"MONOGRAM AAC Encoder",
				&CLSID_MonogramAACEncoder,
				CAACEncoder::CreateInstance, 
				NULL,
				&sudAACEncoder 
			},
			{
				L"MONOGRAM AAC Encoder",
				&CLSID_MonogramAACEncoderPropertypage,
				CAACPropertyPage::CreateInstance
			}
		};

int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


//-----------------------------------------------------------------------------
//
//	DLL Entry Points
//
//-----------------------------------------------------------------------------

STDAPI DllRegisterServer() 
{
	return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
}

