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

#include "tchar.h"

#include "mfxvideo.h"
#include "mfxvideo++.h"

#include <time.h>
#include <dshow.h>
#include <dvdmedia.h>

#pragma warning(disable : 4996)
#pragma warning(disable : 4995)

#define HEADER2(pVideoInfo) (&(((VIDEOINFOHEADER2*) (pVideoInfo))->bmiHeader))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_x) (sizeof(_x)/sizeof(_x[0]))
#endif

#define CHECK_RESULT_P_RET(P, X) {if ((X) != (P)) {return P;}}

const GUID* CodecId2GUID(const mfxU32 fmt, mfxU16 nCodecProfile, int iPosition);    

const GUID* ColorFormat2GUID(const mfxU32 fmt);

WORD ColorFormat2BiBitCount(const mfxU32 fmt);
DWORD ColorFormat2BiCompression(const mfxU32 fmt);

static const mfxU32 MFX_TIME_STAMP_FREQUENCY = 90000; 
static const mfxU64 MFX_TIME_STAMP_INVALID = (mfxU64)-1; 

//mfxU64                  ConvertReferenceTime2MFXTime(REFERENCE_TIME rtTime);
REFERENCE_TIME          ConvertMFXTime2ReferenceTime(mfxU64 nTime);

#define REGISTRY_PATH_PREFIX "Software\\Intel\\Media SDK Sample Filters\\"

TCHAR * GetCodecRegistryPath();

// stores interface parameter in registry:
template<class T>
__inline void SetParamToReg( const TCHAR * pParamName
                           , T           & refParam)
{
    SetRegistryValue(pParamName, (DWORD)refParam, GetCodecRegistryPath());
}

// get interface parameter from registry: 
//if the registry key doesn't exist create such with current value
template<class T>
__inline void GetParamFromReg( const TCHAR * pParamName
                             , T           & refParam) 
{ 
    DWORD tmp_value; 
    if (MFX_ERR_NONE == GetRegistryValue(pParamName, &tmp_value, GetCodecRegistryPath())) 
        refParam = (T)tmp_value; 
    else 
        SetParamToReg(pParamName, refParam); 
}

// get auxiliary parameter from registry: if the registry key doesn't exist do nothing
template<class T>
__inline void GetAuxParamFromReg( const TCHAR * pParamName
                                , T           & refParam) 
{ 
    DWORD tmp_value; 
    if (MFX_ERR_NONE == GetRegistryValue(pParamName, &tmp_value, GetCodecRegistryPath())) 
        refParam = (T)tmp_value; 
}

mfxStatus  SetRegistryValue(const TCHAR *pName, DWORD value, TCHAR *pPath); 
mfxStatus  GetRegistryValue(const TCHAR *pName, DWORD *pValue, TCHAR *pPath);
     
#define WSTR(x) L##x

#define FULL_FILTER_NAME_A(PREFIX, NAME, SUFFIX) \
    PREFIX NAME SUFFIX

#define FULL_FILTER_NAME_W(PREFIX, NAME, SUFFIX) \
    WSTR(PREFIX) WSTR(NAME) WSTR(SUFFIX)

#define FULL_FILTER_NAME    FULL_FILTER_NAME_W
#if defined (UNICODE) || defined(_UNICODE)
#define FULL_FILTER_NAME_T  FULL_FILTER_NAME_W
#else
#define FULL_FILTER_NAME_T  FULL_FILTER_NAME_A
#endif

DEFINE_GUID(MEDIASUBTYPE_avc1,
0x31637661,0x0000,0x0010,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71);

DEFINE_GUID(MEDIASUBTYPE_AVC1,
0x31435641, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71);
