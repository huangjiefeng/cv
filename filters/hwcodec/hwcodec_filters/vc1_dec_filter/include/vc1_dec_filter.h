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
#include "mfx_video_dec_filter.h"

class CVC1DecVideoFilter :  public CDecVideoFilter
{
public:

    DECLARE_IUNKNOWN;
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    CVC1DecVideoFilter(TCHAR *tszName,LPUNKNOWN punk,HRESULT *phr);
    ~CVC1DecVideoFilter();

    HRESULT CheckInputType(const CMediaType *mtIn);

    // Receive new media potion
    HRESULT Receive(IMediaSample *pSample);

    HRESULT NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);

protected:
    BYTE *m_pVC1SeqHeader;
    mfxU32 m_nVC1SeqHeaderSize;
    BOOL m_bVC1SeqHeaderDeliver;

private:
    DISALLOW_COPY_AND_ASSIGN(CVC1DecVideoFilter);
};

