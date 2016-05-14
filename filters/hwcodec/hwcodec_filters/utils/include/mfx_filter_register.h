/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#pragma once

#include "mfx_filter_guid.h"
#include "current_date.h"
#include "mfx_filter_externals_main.h"

#ifdef FILTER_NAME

#ifdef FILTER_INPUT
static const AMOVIESETUP_MEDIATYPE sudIn1PinTypes[] = { FILTER_INPUT };
#endif
#ifdef FILTER_INPUT2
static const AMOVIESETUP_MEDIATYPE sudIn2PinTypes[] = { FILTER_INPUT2 };
#endif
#ifdef FILTER_OUTPUT
static const AMOVIESETUP_MEDIATYPE sudOut1PinTypes[] = { FILTER_OUTPUT };
#endif
#ifdef FILTER_OUTPUT2
static const AMOVIESETUP_MEDIATYPE sudOut2PinTypes[] = { FILTER_OUTPUT2 };
#endif

static const AMOVIESETUP_PIN psudPins[] =
{
#ifdef FILTER_INPUT
    {
        L"Input",           // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output",          // Connects to pin
        ARRAY_SIZE(sudIn1PinTypes), // Number of types
        sudIn1PinTypes       // The pin details
    },
#endif
#ifdef FILTER_INPUT2
    {
        L"Input2",           // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output",          // Connects to pin
        ARRAY_SIZE(sudIn2PinTypes), // Number of types
        sudIn2PinTypes       // The pin details
    },
#endif
#ifdef FILTER_OUTPUT
    {
        L"Output",          // String pin name
        FALSE,              // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input",           // Connects to pin
        ARRAY_SIZE(sudOut1PinTypes), // Number of types
        sudOut1PinTypes      // The pin details
    },
#endif
#ifdef FILTER_OUTPUT2
    {
        L"Output2",         // String pin name
        FALSE,              // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input",           // Connects to pin
        ARRAY_SIZE(sudOut2PinTypes), // Number of types
        sudOut2PinTypes     // The pin details
    }
#endif
};

#ifndef FILTER_MERIT
#define FILTER_MERIT MERIT_PREFERRED
#endif

static const AMOVIESETUP_FILTER sudFilter =
{
    &FILTER_GUID,
    FILTER_NAME,
    FILTER_MERIT,
    ARRAY_SIZE(psudPins),   // Number of pins
    psudPins                    // Pin details
};

// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] =
{
    {
        FILTER_NAME,
        &FILTER_GUID,
        FILTER_CREATE,
        NULL,
        &sudFilter
    }
#ifdef FILTER_PROPERTIES1
    , FILTER_PROPERTIES1
#endif
#ifdef FILTER_PROPERTIES2
    , FILTER_PROPERTIES2
#endif
#ifdef FILTER_PROPERTIES3
    , FILTER_PROPERTIES3
#endif
};

// Number of objects listed in g_Templates
int g_cTemplates = ARRAY_SIZE(g_Templates);

#endif //ifdef FILTER_NAME

/******************************************************************************/

STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2(FALSE);
}

//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
