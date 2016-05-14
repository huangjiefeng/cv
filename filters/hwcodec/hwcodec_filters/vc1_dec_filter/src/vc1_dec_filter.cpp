/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "streams.h"
#include <initguid.h>
#include <tchar.h>
#include <stdio.h>
#include <dvdmedia.h>
#include "wmsdkidl.h"

#include "vc1_dec_filter.h"
#include "filter_defs.h"
#include "mfx_filter_register.h"

CVC1DecVideoFilter::CVC1DecVideoFilter(TCHAR *tszName,LPUNKNOWN punk,HRESULT *phr) :
CDecVideoFilter(tszName,punk, FILTER_GUID,phr)
{
    m_bstrFilterName = FILTER_NAME;
    m_mfxParamsVideo.mfx.CodecId = MFX_CODEC_VC1;

    m_mfxParamsVideo.mfx.FrameInfo.FourCC = MFX_FOURCC_NV12;

    m_pVC1SeqHeader = NULL;
    m_nVC1SeqHeaderSize = 0;

    m_bVC1SeqHeaderDeliver = FALSE;
}

CVC1DecVideoFilter::~CVC1DecVideoFilter()
{
    MSDK_SAFE_DELETE_ARRAY(m_pVC1SeqHeader);
}

CUnknown * WINAPI CVC1DecVideoFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    CVC1DecVideoFilter *pNewObject = new CVC1DecVideoFilter(FILTER_NAME, punk, phr);

    if (NULL == pNewObject)
    {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
}

HRESULT CVC1DecVideoFilter::CheckInputType(const CMediaType *mtIn)
{
    mfxStatus         sts            = MFX_ERR_NONE;
    BITMAPINFOHEADER* pBmpHdr        = NULL;
    INT               nSeqHeaderSize = 0;
    INT               nVideoInfoSize = 0;

    MSDK_CHECK_POINTER(mtIn, E_POINTER);

    if (WMMEDIASUBTYPE_WMV3 == *mtIn->Subtype())
    {
        m_mfxParamsVideo.mfx.CodecProfile = MFX_PROFILE_VC1_SIMPLE;        
    }   
    else if (WMMEDIASUBTYPE_WVC1 == *mtIn->Subtype())
    {
        m_mfxParamsVideo.mfx.CodecProfile = MFX_PROFILE_VC1_ADVANCED;
    }
    else
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    if (FORMAT_VideoInfo2 == *mtIn->FormatType()) 
    {
        VIDEOINFOHEADER2* pvi2 = (VIDEOINFOHEADER2*)mtIn->pbFormat;
        MSDK_CHECK_POINTER(pvi2, E_POINTER);

        nVideoInfoSize = sizeof(VIDEOINFOHEADER2);

        pBmpHdr = &(pvi2->bmiHeader);
    }

    if (FORMAT_VideoInfo == *mtIn->FormatType()) 
    {
        VIDEOINFOHEADER* pvi = (VIDEOINFOHEADER*)mtIn->pbFormat;
        MSDK_CHECK_POINTER(pvi, E_POINTER);

        nVideoInfoSize = sizeof(VIDEOINFOHEADER);

        pBmpHdr = &(pvi->bmiHeader);
    }

    m_nVC1SeqHeaderSize = 0;

    //Microsoft adds decoding headers to the end of the mtIn->pbFormat
    if (nVideoInfoSize != mtIn->cbFormat)
    {
        MSDK_CHECK_POINTER(pBmpHdr, E_POINTER);

        if (MAKEFOURCC('W', 'V', 'C', '1') == pBmpHdr->biCompression)
        {
            nVideoInfoSize++;
        }

        nSeqHeaderSize = mtIn->cbFormat - nVideoInfoSize;

        MSDK_SAFE_DELETE_ARRAY(m_pVC1SeqHeader);
        m_pVC1SeqHeader = new BYTE [nSeqHeaderSize];
        MSDK_CHECK_POINTER(m_pVC1SeqHeader, MFX_ERR_MEMORY_ALLOC);
        
        memcpy(m_pVC1SeqHeader, mtIn->pbFormat + nVideoInfoSize, nSeqHeaderSize);

        m_nVC1SeqHeaderSize = nSeqHeaderSize;
        m_bVC1SeqHeaderDeliver = TRUE;

        MSDK_SAFE_DELETE(m_pFrameConstructor);
        m_pFrameConstructor = new CVC1FrameConstructor(&m_mfxParamsVideo);        
    }

    return CDecVideoFilter::CheckInputType(mtIn);
};

HRESULT CVC1DecVideoFilter::Receive(IMediaSample *pSample)
{
    mfxStatus sts = MFX_ERR_NONE;    

    if (m_bVC1SeqHeaderDeliver && m_nVC1SeqHeaderSize && !m_bStop)
    {
        mfxU8*              pDataBuffer = NULL;
        mfxFrameSurface1*   pSurf       = NULL;
        mfxBitstream        mfxBS;
        mfxU32              nSaveBufferSize = 0;
        mfxU8*              pSaveDataBuffer = NULL;

        CAutoLock cObjectLock(&m_csLock);

        // check error(s)
        if (NULL == pSample || m_bFlushing)
        {
            return S_OK;
        }

        if (0 == pSample->GetActualDataLength())
        {
            return S_FALSE;
        }    
        
        memset(&mfxBS, 0, sizeof(mfxBitstream));
       
        pSample->GetPointer(&pDataBuffer);
        MSDK_CHECK_POINTER(pDataBuffer, E_POINTER);

        nSaveBufferSize = pSample->GetActualDataLength();

        pSaveDataBuffer = new mfxU8[nSaveBufferSize];
        MSDK_CHECK_POINTER(pSaveDataBuffer, E_POINTER);

        memcpy(pSaveDataBuffer, pDataBuffer, nSaveBufferSize);

        memcpy(pDataBuffer, m_pVC1SeqHeader, m_nVC1SeqHeaderSize);
        
        pSample->SetActualDataLength(m_nVC1SeqHeaderSize);
        
        //input sample is updated if required
        sts = m_pFrameConstructor->ConstructFrame(pSample, 0);

        if (MFX_ERR_NONE == sts)
        {
            mfxBS.Data = pDataBuffer;
            mfxBS.DataLength = mfxBS.MaxLength = pSample->GetActualDataLength();

            sts = m_pDecoder->RunDecode(&mfxBS, pSurf);

            if (MFX_ERR_MORE_DATA == sts)
            {
                m_pFrameConstructor->SaveResidialData(&mfxBS);
            }
            else if (MFX_ERR_NONE != sts) // notify about error and return
            {
                CComPtr<IMediaEventSink> pSink;
                HRESULT hr = m_pGraph->QueryInterface(IID_IMediaEventSink, (void**)&pSink);

                if (SUCCEEDED(hr))
                {
                    pSink->Notify(EC_ERRORABORT, 0, 0);
                }

                MSDK_SAFE_DELETE_ARRAY(pSaveDataBuffer);

                return E_FAIL;                                         
            }
        }

        memcpy(pDataBuffer, pSaveDataBuffer, nSaveBufferSize);
        pSample->SetActualDataLength(nSaveBufferSize);

        MSDK_SAFE_DELETE_ARRAY(pSaveDataBuffer);

        m_bVC1SeqHeaderDeliver = FALSE;
    }    
    
    return CDecVideoFilter::Receive(pSample);        
};

HRESULT CVC1DecVideoFilter::NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    m_bVC1SeqHeaderDeliver = TRUE;

    return CDecVideoFilter::NewSegment(tStart,tStop,dRate);
};

