/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "..\..\..\sample_common\include\current_date.h"

#define FILTER_NAME_STRING "Intel Media SDK H.264 Encoder"
#define DLL_NAME           "h264_enc_filter.dll"
#define FILTER_NAME        _T(FILTER_NAME_STRING)
#define FILTER_INPUT       { &MEDIATYPE_Video, &MEDIASUBTYPE_NV12 }
#define FILTER_OUTPUT      { &MEDIATYPE_Video, &MEDIASUBTYPE_H264 }
#define FILTER_GUID        CLSID_H264EncFilter
#define FILTER_CREATE      CH264EncVideoFilter::CreateInstance

#define FILTER_PROPERTIES1 {L"Configure", &CLSID_H246EncPropertyPage, CH264EncPropPage::CreateInstance, NULL, NULL}
#define FILTER_PROPERTIES2 { L"Video Encoder Properties", &CLSID_VideoPropertyPage, CVideoEncPropPage::CreateInstance, NULL, NULL }
#define FILTER_PROPERTIES3 { L"About", &CLSID_AboutPropertyPage, CAboutPropPage::CreateInstance, NULL, NULL }

TCHAR * GetCodecRegistryPath()    {return _T(REGISTRY_PATH_PREFIX) _T("H264 Encoder");}
