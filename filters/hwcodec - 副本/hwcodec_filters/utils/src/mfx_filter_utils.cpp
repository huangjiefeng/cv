/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/
  
#include <atlbase.h>
#include <Windows.h>
#include "guiddef.h"
#include <uuids.h>
#include <mmreg.h>
#include <math.h>

#include "wmsdkidl.h"

#include "mfx_filter_guid.h"

static const struct
{
    mfxU32          color_format;
    GUID            guid;
    mfxU32          bpp;
    mfxU32          fourcc;
}
color_formats[] =
{
    { MFX_FOURCC_YV12,       MEDIASUBTYPE_YV12,    12,  MAKEFOURCC('Y','V','1','2') },
    { MFX_FOURCC_NV12,       MEDIASUBTYPE_NV12,    12,  MAKEFOURCC('N','V','1','2') },
    { MFX_FOURCC_RGB3,       MEDIASUBTYPE_RGB32,   32,  BI_RGB                      },
};

static const struct
{
    mfxU32          codec_type;
    mfxI8           profile;
    GUID            guid; 
    int             iPosition;
}
codec_types[] =
{
    { MFX_CODEC_AVC,   -1, MEDIASUBTYPE_H264,        0},
    { MFX_CODEC_MPEG2, -1, MEDIASUBTYPE_MPEG2_VIDEO, 0},
};

const GUID* CodecId2GUID(const mfxU32 fmt, mfxU16 nCodecProfile, int iPosition)
{
    for (int i = 0; i < ARRAY_SIZE(codec_types); i++)
    {
        if (codec_types[i].codec_type == fmt &&  codec_types[i].iPosition == iPosition)
        {
            if (-1 != codec_types[i].profile && codec_types[i].profile != nCodecProfile)
            {
                continue;
            }
            return &codec_types[i].guid;
        }               
    }
    return NULL;
}

const GUID* ColorFormat2GUID(const mfxU32 fmt)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(color_formats); i++)
    {
        if (color_formats[i].color_format == fmt) return &color_formats[i].guid;
    }
    return NULL;
}

DWORD ColorFormat2BiCompression(const mfxU32 fmt)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(color_formats); i++)
    {
        if (color_formats[i].color_format == fmt) return color_formats[i].fourcc;
    }
    return 0;
}

WORD ColorFormat2BiBitCount(const mfxU32 fmt)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(color_formats); i++)
    {
        if (color_formats[i].color_format == fmt) return (WORD)color_formats[i].bpp;
    }
    return 0;
}

#define REGISTRY_ROOT                 HKEY_CURRENT_USER

mfxStatus SetRegistryValue(const TCHAR *pName, DWORD value, TCHAR *pPath)
{
    HKEY key = NULL;
    LONG res;

    if (!pPath || !pName)
    {
        return MFX_ERR_NULL_PTR;
    }    

    res = RegCreateKeyEx(REGISTRY_ROOT, pPath, 0, 0, 0, KEY_SET_VALUE, NULL, &key, NULL);
    if (res == ERROR_SUCCESS)
    {
        res = RegSetValueEx(key, pName, 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegCloseKey(key);
    }

    return (res == ERROR_SUCCESS) ? MFX_ERR_NONE : MFX_ERR_UNKNOWN;    
}

mfxStatus  GetRegistryValue(const TCHAR *pName, DWORD *pValue, TCHAR *pPath)
{
    HKEY key = NULL;
    LONG res;
    DWORD type;

    if (!pPath || !pName || !pValue)
    {
        return MFX_ERR_NULL_PTR;
    }

    DWORD cbData = sizeof(*pValue);             
        
    res = RegOpenKeyEx(REGISTRY_ROOT, pPath, 0, KEY_QUERY_VALUE, &key);
    if (res == ERROR_SUCCESS)
    {
        res = RegQueryValueEx(key, pName, NULL, &type, (LPBYTE)pValue, &cbData);
        RegCloseKey(key);
    }   
    
    return (res == ERROR_SUCCESS) ? MFX_ERR_NONE : MFX_ERR_UNKNOWN;    
}

REFERENCE_TIME ConvertMFXTime2ReferenceTime(mfxU64 nTime)
{
    if (MFX_TIME_STAMP_INVALID != nTime)
    {
        return (REFERENCE_TIME)((nTime / (DOUBLE)MFX_TIME_STAMP_FREQUENCY) * 1e7);
    }
    else
    {
        return (REFERENCE_TIME)-1e7;
    }    
};