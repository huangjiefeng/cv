/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except mtIn accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "mfx_video_dec_filter.h"
#include "mfx_filter_defs.h"

#pragma warning( disable : 4748 )

CDecVideoFilter::CDecVideoFilter(TCHAR *tszName,LPUNKNOWN punk, const GUID VideoCodecGUID, HRESULT *phr, 
                                 mfxU16 APIVerMinor, mfxU16 APIVerMajor) :
CTransformFilter(tszName, punk, VideoCodecGUID),
m_bstrFilterName(0),
m_bStop(FALSE),
m_bFlushing(FALSE),
m_nTempBufferLen(0),
m_nPitch(0),
m_pFrameConstructor(0),
m_pManager(0),
m_pFrameAllocator(NULL),
m_bNeedToDeleteAllocator(FALSE),
m_pDecoderOutput(NULL),
m_nPARW(0), m_nPARH(0),
m_bJoinSession(TRUE)
{
    mfxStatus sts = MFX_ERR_NONE;

    // save version to be able to re-create BaseDecoder when needed
    m_nAPIVerMinor = APIVerMinor;
    m_nAPIVerMajor = APIVerMajor;

    m_pDecoder = new CBaseDecoder(APIVerMinor, APIVerMajor, &sts);

    if (phr)
        *phr = (MFX_ERR_NONE == sts) ? S_OK : E_FAIL;

    memset(&m_mfxParamsVideo, 0, sizeof(m_mfxParamsVideo));

    m_pTimeManager = new CTimeManager;

    GetParamFromReg(_T("JoinSession"), m_bJoinSession);

    m_bLowLatencyMode = TRUE;
}

CDecVideoFilter::~CDecVideoFilter(void)
{
    Deinitialize();
    MSDK_SAFE_DELETE(m_pDecoder);
    MSDK_SAFE_DELETE(m_pTimeManager);

    MSDK_SAFE_DELETE(m_pFrameConstructor);
}

void CDecVideoFilter::Deinitialize(void)
{
    m_bFlushing                  = FALSE;
    m_bStop                      = FALSE;

    return;
}

HRESULT CDecVideoFilter::RenewSampleParams(mfxFrameSurface1* pSurface, IMediaSample* pSample)
{
    HRESULT                     hr = S_OK;
    CMediaType                  mt;
    VIDEOINFOHEADER2*           pVIH2 = NULL;    
    CComQIPtr<IMediaSample2>    pSample2 = pSample;   
    AM_SAMPLE2_PROPERTIES       props;

    BOOL                        bMediaTypeChanged = FALSE;
    RECT                        rcCrop;
    
    mfxIMPL                     impl;
    
    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pSurface, MFX_ERR_NULL_PTR);      
  
    MSDK_CHECK_NOT_EQUAL(m_State, State_Running, S_OK);    

    ZeroMemory(&props, sizeof(AM_SAMPLE2_PROPERTIES));

    hr = m_pOutput->ConnectionMediaType(&mt);
    CHECK_RESULT_P_RET(hr, S_OK);
    
    pVIH2 = reinterpret_cast<VIDEOINFOHEADER2*>(mt.pbFormat); 
    MSDK_CHECK_POINTER_SAFE(pVIH2, E_UNEXPECTED, FreeMediaType(mt));

    m_pDecoder->QueryIMPL(&impl);

    rcCrop.left   = (MFX_IMPL_HARDWARE == impl) ? pSurface->Info.CropX : 0;
    rcCrop.top    = (MFX_IMPL_HARDWARE == impl) ? pSurface->Info.CropY : 0;
    rcCrop.right  = rcCrop.left + pSurface->Info.CropW;
    rcCrop.bottom = rcCrop.top + pSurface->Info.CropH;

    // set media type if current rcSource differs from crop values    
    if ((rcCrop.left != pVIH2->rcSource.left || rcCrop.right != pVIH2->rcSource.right ||
        rcCrop.top != pVIH2->rcSource.top || rcCrop.bottom != pVIH2->rcSource.bottom) &&
        rcCrop.right <= pVIH2->rcSource.right && rcCrop.bottom <= pVIH2->rcSource.bottom) // if this condition is not met reconnection is required (render has smaller buffer)
    {
        pVIH2->rcSource = rcCrop;

        bMediaTypeChanged = TRUE;
    } 

    if (bMediaTypeChanged)
    {
        // update sample's media type
        hr = pSample->SetMediaType(&mt); 
        MSDK_CHECK_RESULT_SAFE(hr, S_OK, hr, FreeMediaType(mt));

        // update output pin's media type
        hr = m_pOutput->SetMediaType(&mt);
        MSDK_CHECK_RESULT_SAFE(hr, S_OK, hr, FreeMediaType(mt));
    }

    FreeMediaType(mt); 

    //update interlace properties of sample according to surface PicStruct
    hr = pSample2->GetProperties(sizeof(AM_SAMPLE2_PROPERTIES), (BYTE*)&props);
    CHECK_RESULT_P_RET(hr, S_OK);

    if (MFX_PICSTRUCT_FIELD_TFF == pSurface->Info.PicStruct)
    {
        props.dwTypeSpecificFlags = AM_VIDEO_FLAG_FIELD1FIRST;
    }    
    else if (MFX_PICSTRUCT_PROGRESSIVE == pSurface->Info.PicStruct)
    {
        props.dwTypeSpecificFlags = AM_VIDEO_FLAG_WEAVE;
    }

    hr = pSample2->SetProperties(sizeof(AM_SAMPLE2_PROPERTIES), (BYTE*)&props);
    CHECK_RESULT_P_RET(hr, S_OK); 

    return hr;
}

HRESULT CDecVideoFilter::DeliverSurface(mfxFrameSurface1* pSurface)
{
    HRESULT                 hr = S_OK;
    CComPtr<IMediaSample>   pOutSample;
    REFERENCE_TIME          rtStart = 0;

    MSDK_CHECK_POINTER(pSurface, E_POINTER);

    mfxIMPL impl;
    m_pDecoder->QueryIMPL(&impl); 

    // In case of sysmem surfaces we provide plane pointers, not MemIds,    
    if (!m_pDecoder->m_bd3dAlloc)
    {
        mfxStatus sts = m_pFrameAllocator->Lock(m_pFrameAllocator->pthis, pSurface->Data.MemId, &pSurface->Data);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);            
    }  

    if (m_pDecoderOutput->m_bOwnDSAllocatorAgreed)
    {
        CMFXSample *pmfxSample = (CMFXSample*)pSurface->Data.MemId;       

        // To pass surface pointer downstream
        pmfxSample->SetMfxSurfacePointer(pSurface);

        pOutSample = (IMediaSample*)pSurface->Data.MemId; // assignment to CComPtr does an AddRef                       
    }
    else 
    {
        // GetBuffer and copy surface data        
        mfxU32 nTry = 0;
        for (;;)
        {
            hr = m_pOutput->GetDeliveryBuffer(&pOutSample, NULL, NULL, 0);
            if (FAILED(hr) || !pOutSample)
            {
                if ((m_bFlushing || m_State != State_Running) && nTry > 25)
                {
                    return E_FAIL;
                }
                Sleep(1);
                nTry++;
                continue;
            }
            break;
        }

        MSDK_CHECK_POINTER(pOutSample, E_POINTER);

        mfxU8* pBuffer = NULL;

        hr = pOutSample->GetPointer(&pBuffer);
        CHECK_RESULT_P_RET(hr, S_OK);
        
        // copy the surface_out data to sample buffer

        // find out the frame height at connection 
        // to copy decoded data correctly in the case when we are connected with H and cropH != H     
        VIDEOINFOHEADER2 *pVIH2(NULL);
        pVIH2 = reinterpret_cast<VIDEOINFOHEADER2* >(m_pOutput->CurrentMediaType().Format());
        MSDK_CHECK_POINTER(pVIH2, E_UNEXPECTED);

        LONG lConnectionHeight = abs(pVIH2->bmiHeader.biHeight);     

        mfxFrameData *pData = &pSurface->Data;

        switch (m_mfxParamsVideo.mfx.FrameInfo.FourCC)
        {
        case MFX_FOURCC_NV12:
            memcpy(pBuffer,
                pSurface->Data.Y + pSurface->Info.CropX + pSurface->Info.CropY * pData->Pitch,
                pData->Pitch * m_mfxParamsVideo.mfx.FrameInfo.CropH);
            memcpy(pBuffer + pData->Pitch * lConnectionHeight,
                pSurface->Data.UV + pSurface->Info.CropX + pSurface->Info.CropY * pData->Pitch / 2,
                pData->Pitch * m_mfxParamsVideo.mfx.FrameInfo.CropH / 2);
            break;

        default:
            return E_UNEXPECTED;        
        }

        hr = pOutSample->SetActualDataLength(pData->Pitch * lConnectionHeight * 3 / 2);
        CHECK_RESULT_P_RET(hr, S_OK);
    } 
    
    MSDK_CHECK_POINTER(pOutSample, E_POINTER);
    rtStart = ConvertMFXTime2ReferenceTime(pSurface->Data.TimeStamp);    
    
    if (1 == pSurface->Info.FrameId.ViewId)
    {
        //in case of we have 2 views, only first sample comes with duration, 
        //for second duration is equal to 0
        REFERENCE_TIME rtEnd = rtStart;
        hr = pOutSample->SetTime(&rtStart, &rtEnd);
    }
    else
    {
        hr = pOutSample->SetTime(&rtStart, NULL);
    }
 
    CHECK_RESULT_P_RET(hr, S_OK);

    // notify cropping and interlace flags to downstream filter via output sample's media type and sample properties
    hr = RenewSampleParams(pSurface, pOutSample);
    CHECK_RESULT_P_RET(hr, S_OK);

    hr = m_pOutput->Deliver(pOutSample);
    CHECK_RESULT_P_RET(hr, S_OK);

    m_pTimeManager->SetTime(rtStart);

    return S_OK;
};

HRESULT CDecVideoFilter::ProcessNewResolution()
{
    HRESULT                hr = S_OK;
    CMediaType            mt;
    VIDEOINFOHEADER2*    pInfo;    
    mfxStatus            sts = MFX_ERR_NONE;   

    // let downstream filter process all samples with old mediatype
    BeginFlush();   
    EndFlush();
    
    if (m_pOutput->IsConnected()) 
    {
        CopyMediaType(&mt, &(m_pOutput->CurrentMediaType()));            
    } 
    else 
    {  
        return VFW_E_NOT_CONNECTED;
    }                    
      
    pInfo = reinterpret_cast<VIDEOINFOHEADER2*>(mt.pbFormat);
    if (!pInfo)
    {
        hr = E_UNEXPECTED;
    }   

    // update mediatype according to new m_mfxParamsVideo
    if (SUCCEEDED(hr))
    {
        // update pitch, EVR will update it if needed in NotifyFormatChange
        m_nPitch = m_mfxParamsVideo.mfx.FrameInfo.Width;

        // update resolution
        pInfo->rcSource.bottom = pInfo->rcTarget.bottom = pInfo->bmiHeader.biHeight 
            = m_mfxParamsVideo.mfx.FrameInfo.CropH;

        pInfo->rcSource.right = pInfo->rcTarget.right = pInfo->bmiHeader.biWidth 
            = m_mfxParamsVideo.mfx.FrameInfo.CropW;

        // update aspect ratio
        mfxU16 arw = m_mfxParamsVideo.mfx.FrameInfo.AspectRatioW;
        mfxU16 arh = m_mfxParamsVideo.mfx.FrameInfo.AspectRatioH;        

        // AR is not always contained in the header 
        if (arw * arh == 0)
        {
            arw = arh = 1;
        }
        
        pInfo->dwPictAspectRatioX = arw * pInfo->bmiHeader.biWidth;
        pInfo->dwPictAspectRatioY = arh * pInfo->bmiHeader.biHeight;        
        
        if (m_mfxParamsVideo.mfx.FrameInfo.PicStruct == MFX_PICSTRUCT_FIELD_TFF)
        {
            pInfo->dwInterlaceFlags = AMINTERLACE_IsInterlaced | AMINTERLACE_Field1First | AMINTERLACE_DisplayModeBobOrWeave;
        }
        else if (m_mfxParamsVideo.mfx.FrameInfo.PicStruct == MFX_PICSTRUCT_FIELD_BFF)
        {
            pInfo->dwInterlaceFlags = AMINTERLACE_IsInterlaced | AMINTERLACE_DisplayModeBobOrWeave;
        }        


        CDecoderOutputPin *pDecPin;
        pDecPin = dynamic_cast<CDecoderOutputPin*>(m_pOutput);
        if (pDecPin)
        {
            hr = pDecPin->NotifyFormatChange(&mt);
        }                                
    }  

    if (SUCCEEDED(hr))
    {          
        sts = m_pDecoder->InternalReset(&m_mfxParamsVideo, m_nPitch, true);
        if (MFX_ERR_NONE != sts)
        {
            hr = E_FAIL;
        }                 
    }         
   
    FreeMediaType(mt);

    return hr;
};

HRESULT CDecVideoFilter::Receive(IMediaSample* pSample)
{
    HRESULT         hr = S_OK;
    mfxStatus       sts = MFX_ERR_NONE;
    mfxBitstream    mfxBS; 
    mfxVideoParam VideoParams;
    mfxFrameSurface1 *pSurfaceOut = NULL;

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

    m_pFrameConstructor->ConstructFrame(pSample, &mfxBS);

    //decode mfxBitstream until all data is taken by decoder
    while (mfxBS.DataLength > 0 && !m_bStop)
    {
        if (!m_pDecoder->m_bHeaderDecoded)
        {            
            memset(&VideoParams, 0, sizeof(mfxVideoParam));
            VideoParams.mfx.CodecId = m_mfxParamsVideo.mfx.CodecId;
            VideoParams.AsyncDepth = (mfxU16)m_bLowLatencyMode;
//			VideoParams.AsyncDepth = 1;
            sts = m_pDecoder->DecodeHeader(&mfxBS, &VideoParams); 
            if (MFX_ERR_MORE_DATA == sts)
            {
                break;
            }            
                   
            mfxIMPL impl;
            m_pDecoder->QueryIMPL(&impl);
            VideoParams.IOPattern = (mfxU16)((m_pDecoder->m_bd3dAlloc) ? MFX_IOPATTERN_OUT_VIDEO_MEMORY : 
                MFX_IOPATTERN_OUT_SYSTEM_MEMORY);

            mfxFrameInfo* pNew = &VideoParams.mfx.FrameInfo;
            mfxFrameInfo* pOld = &m_mfxParamsVideo.mfx.FrameInfo;

            if ((MFX_ERR_NONE == sts)&&
                (pNew->CropH != pOld->CropH || pNew->CropW  != pOld->CropW ||
                pNew->Width != pOld->Width || pNew->Height != pOld->Height ||
                pNew->PicStruct != pOld->PicStruct))
            {
                if (VideoParams.mfx.FrameInfo.CropX && 
                    ((mfxU32)(VideoParams.mfx.FrameInfo.Width - VideoParams.mfx.FrameInfo.CropX) > m_nPitch))
                {
                    m_nPitch = VideoParams.mfx.FrameInfo.Width;            
                }

                m_pDecoder->Close();

                memcpy(&m_mfxParamsVideo.mfx.FrameInfo, &VideoParams.mfx.FrameInfo, sizeof(mfxFrameInfo)); 
                ProcessNewResolution();                       
            }   

            if (MFX_ERR_NONE == sts)
            {
                memcpy(&m_mfxParamsVideo, &VideoParams, sizeof(mfxVideoParam)); 
            } 
            else 
            {
                hr = E_FAIL;
                break;            
            }
        }      

        mfxBS.DataFlag = (mfxU16)m_bLowLatencyMode;                        
        sts = m_pDecoder->RunDecode(&mfxBS, pSurfaceOut);                

        if (MFX_ERR_NONE == sts)
        {
            hr = DeliverSurface(pSurfaceOut);            
            if (SUCCEEDED(hr)) 
            {
                continue;
            }
            else 
            {
                break;
            }                                               
        }
        else if (MFX_ERR_MORE_DATA == sts)
        {
            break; // need to receive more data from upstream filter
        }
        else if (MFX_WRN_VIDEO_PARAM_CHANGED == sts)
        {
            continue; // just continue processing
        }
        else if (MFX_ERR_MORE_SURFACE == sts)
        {
            continue; // just continue, new surface will be found in RunDecode
        }
        else if (MFX_ERR_INCOMPATIBLE_VIDEO_PARAM == sts)
        {           
            // retrieve new parameters
            ZeroMemory(&VideoParams, sizeof(mfxVideoParam)); 
            VideoParams.mfx.CodecId = m_pDecoder->m_nCodecId;
            sts = m_pDecoder->DecodeHeader(&mfxBS, &VideoParams); 
            if (MFX_ERR_MORE_DATA == sts)
            {
                break;
            }
            // save IOPattern and update parameters
            VideoParams.IOPattern = m_mfxParamsVideo.IOPattern; 
            memcpy(&m_mfxParamsVideo, &VideoParams, sizeof(mfxVideoParam));

            hr = ProcessNewResolution();
            if (SUCCEEDED(hr))
            {                
                continue;
            }
        }        

        // notify about an error from either RunDecode or ProcessNewResolution
        if (MFX_ERR_NONE != sts || FAILED(hr))
        {
            CComPtr<IMediaEventSink> pSink;
            hr = m_pGraph->QueryInterface(IID_IMediaEventSink, (void**)&pSink);

            if (SUCCEEDED(hr))
            {
                pSink->Notify(EC_ERRORABORT, 0, 0);
            }

            hr = E_FAIL;

            break;
        }
    }

    m_pFrameConstructor->SaveResidialData(&mfxBS);

    MSDK_SAFE_DELETE_ARRAY(mfxBS.Data);

    return hr;
}

void CDecVideoFilter::WriteMfxImplToRegistry()
{
    mfxIMPL impl;
    m_pDecoder->QueryIMPL(&impl);
    bool bIsHWLib = (MFX_IMPL_HARDWARE == impl) ? true : false;
    SetParamToReg(_T("IsHWMfxLib"), bIsHWLib);
    SetParamToReg(_T("IsDecodePartiallyAccelerated"), m_pDecoder->m_bDecPartialAcceleration);
}

HRESULT CDecVideoFilter::StartStreaming(void)
{
    mfxStatus sts = MFX_ERR_NONE;    

    CAutoLock cObjectLock(&m_csLock);

    // Deinitialize filter
    Deinitialize();

    //let's start
    m_bStop = FALSE;   

    // Commit the allocator to allow memory allocation in Init
    m_pDecoderOutput->Active();

    // if base decoder's frame allocator wasn't yet set
    if (!m_pDecoder->m_pMfxFrameAllocator)        
    {
        // Own DS allocator can act as frame allocator, 
        // alien DS allocator cannot so we create simple sys mem frame allocator in this case
        if (!m_pDecoderOutput->m_bOwnDSAllocatorAgreed)
        {
            m_pFrameAllocator = new SysMemFrameAllocator;
            MSDK_CHECK_POINTER(m_pFrameAllocator, E_POINTER);

            m_bNeedToDeleteAllocator = TRUE;

            sts = m_pFrameAllocator->Init(NULL);
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);
        }      
                
        // provide frame allocator to BaseDecoder,
        // BaseDecoder doesn't own the allocator
        sts = m_pDecoder->SetFrameAllocator(m_pFrameAllocator);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);
    }    

    sts = m_pDecoder->Init(&m_mfxParamsVideo, m_nPitch);
    WriteMfxImplToRegistry();
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

    // call method from parent
    return CTransformFilter::StartStreaming();
}

HRESULT CDecVideoFilter::StopStreaming(void)
{  
    CAutoLock cObjectLock(&m_csLock);

    m_bStop = TRUE;

    mfxStatus sts = MFX_ERR_NONE;   

    sts = m_pDecoder->Close();
    WriteMfxImplToRegistry();
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

    return CTransformFilter::StopStreaming();
}

HRESULT CDecVideoFilter::EndOfStream(void)
{
    HRESULT         hr = S_OK;
    mfxStatus       sts = MFX_ERR_NONE;

    CAutoLock cObjectLock(&m_csLock);

    // check error(s)
    if (m_bFlushing)
    {
        return S_OK;
    }

    //decode mfxBitstream until all data is taken by decoder
    while ((MFX_ERR_NONE == sts || MFX_ERR_MORE_SURFACE == sts) && !m_bStop)
    {
        mfxFrameSurface1* pSurf = NULL;

        sts = m_pDecoder->RunDecode(NULL, pSurf);
        if (MFX_ERR_NONE == sts)
        {
            hr = DeliverSurface(pSurf);
            CHECK_RESULT_P_RET(hr, S_OK);
        }
    }

    return CTransformFilter::EndOfStream();
}

HRESULT CDecVideoFilter::NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    mfxStatus sts = MFX_ERR_NONE;
    CAutoLock cObjectLock(&m_csLock);

    if (m_pDecoder && m_pFrameConstructor)
    {
        sts = m_pDecoder->Reset(&m_mfxParamsVideo, m_nPitch);
        WriteMfxImplToRegistry();
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);
        m_pFrameConstructor->Reset();
    }   

    m_nTempBufferLen = 0;
    return CTransformFilter::NewSegment(tStart,tStop,dRate);
}

STDMETHODIMP CDecVideoFilter::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IConfigureVideoDecoder)
    {
        return GetInterface(static_cast<IConfigureVideoDecoder*>(this), ppv);
    }

    if (riid == IID_IAboutPropertyPage)
    {
        return GetInterface(static_cast<IAboutProperty*>(this), ppv);
    }

    if (riid == IID_ISpecifyPropertyPages)
    {
        return GetInterface(static_cast<ISpecifyPropertyPages*>(this), ppv);
    }

    return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT CDecVideoFilter::DecideBufferSize(IMemAllocator* pAlloc,ALLOCATOR_PROPERTIES* pProperties)
{
    HRESULT                 hr;
    ALLOCATOR_PROPERTIES    Actual;
    mfxStatus               sts = MFX_ERR_NONE;
    mfxU32                  nBuffersNum = 0;

    MSDK_CHECK_POINTER(m_pDecoder, E_POINTER);

    //check whether we should use our own allocator
    if (m_pDecoderOutput->m_bRequireOwnDSAllocator)
    {        
        if (pAlloc != m_pDecoderOutput->m_pOwnAllocator)
        {
            return E_FAIL;
        }        
    }
    
    AM_MEDIA_TYPE mt;
    if (m_pOutput->IsConnected()) 
    {
        CopyMediaType(&mt, &(m_pOutput->CurrentMediaType()));            
    } 
    else 
    {   
        return VFW_E_NOT_CONNECTED;
    }    

    BITMAPINFOHEADER *pbmi = NULL;

    if(mt.formattype == FORMAT_VideoInfo)
    {
        pbmi = HEADER(mt.pbFormat);
    }
    else if(mt.formattype == FORMAT_VideoInfo2)
    {
        pbmi = HEADER2(mt.pbFormat);
    }

    // on failure to get BITMAPINFOHEADER
    if (!pbmi)
    {
        FreeMediaType(mt);
        return E_FAIL;
    }

    pProperties->cbBuffer = max((long)(DIBSIZE(*pbmi)), pProperties->cbBuffer);
    pProperties->cbBuffer = max(((m_mfxParamsVideo.mfx.FrameInfo.Height+31)&~31) * m_mfxParamsVideo.mfx.FrameInfo.Width * 3 / 2, pProperties->cbBuffer);

    if (pProperties->cbAlign == 0)
    {
        pProperties->cbAlign = 1;
    }

    if (pProperties->cBuffers == 0)
    {
        pProperties->cBuffers = 1;
    }

    if (MFX_ERR_NONE == sts)
    {
        nBuffersNum = m_pDecoder->GetIOSurfNum(&m_mfxParamsVideo);
    } 

    m_pDecoder->SetAuxFramesNum((mfxU16)pProperties->cBuffers); 

    pProperties->cBuffers += nBuffersNum;

    hr = pAlloc->SetProperties(pProperties, &Actual);

    MSDK_CHECK_RESULT_SAFE(hr, S_OK, hr, FreeMediaType(mt));

    CComPtr<IPin> pDownstreamPin = NULL;
    CComPtr<IExposeMfxMemTypeFlags> pMFXPin = NULL;
    DWORD flags = 0;
    m_pOutput->ConnectedTo(&pDownstreamPin);

    if (pDownstreamPin)
    {
        hr = pDownstreamPin->QueryInterface(IID_IExposeMfxMemTypeFlags, reinterpret_cast<void**>(&pMFXPin));

        if (SUCCEEDED(hr))
        {
            pMFXPin->GetMfxMemTypeFlags(&flags);
            m_pDecoder->SetAdditionalMemTypeFlags((mfxU16)flags);
        }
    }
    

    if (pProperties->cBuffers > Actual.cBuffers || pProperties->cbBuffer > Actual.cbBuffer)
    {
        return E_FAIL;
    }

    // Release the format block.
    FreeMediaType(mt);

    return S_OK;
}

HRESULT CDecVideoFilter::BreakConnect(PIN_DIRECTION dir)
{   
    if (PINDIR_OUTPUT == dir)
    {
        if (m_pDecoder && m_pDecoder->m_bSessionHasParent)
        {
            mfxStatus sts = MFX_WRN_IN_EXECUTION;

            while (MFX_WRN_IN_EXECUTION == sts)
            {
                sts = m_pDecoder->m_mfxVideoSession.DisjoinSession();
                Sleep(1);
            }

            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);
            m_pDecoder->m_bSessionHasParent = false;            
        }        

        m_nPitch = m_mfxParamsVideo.mfx.FrameInfo.Width;
        
        // Frame allocator is defined at connection.
        // BaseDecoder must be re-created if another frame allocator is used.
        // Therefore delete and free all these:        
        MSDK_SAFE_DELETE(m_pDecoder); 
        m_pDecoder = new CBaseDecoder(m_nAPIVerMinor, m_nAPIVerMajor);
        MSDK_CHECK_POINTER(m_pDecoder, E_FAIL);

        m_pDecoderOutput->SetAllocator(NULL); 

        if (m_bNeedToDeleteAllocator)
        {
            MSDK_SAFE_DELETE(m_pFrameAllocator); // normal object was created so need to delete
        }        
        
        m_pFrameAllocator = NULL;        
    }

    return CTransformFilter::BreakConnect(dir);
}

HRESULT CDecVideoFilter::BeginFlush(void)
{
    m_bFlushing = true;

    if (m_pOutput && m_pOutput->IsConnected())
    {
        m_pOutput->DeliverBeginFlush();
    }

    return S_OK;
}

HRESULT CDecVideoFilter::EndFlush(void)
{
    m_bFlushing = false;

    if (m_pOutput && m_pOutput->IsConnected())
    {
        m_pOutput->DeliverEndFlush();
    }

    return S_OK;
}

HRESULT CDecVideoFilter::GetMediaType(int iPosition, CMediaType* pmt)
{
    GUID               guidFormat;
    VIDEOINFOHEADER    *pSrcVIH = NULL;
    VIDEOINFOHEADER2   *pSrcVIH2(NULL), *pDstVIH(NULL);    

    MSDK_CHECK_POINTER(pmt, E_POINTER);
    MSDK_CHECK_POINTER(m_pInput,   E_POINTER);
    MSDK_CHECK_ERROR(m_pInput->IsConnected(), FALSE, E_UNEXPECTED);

    if (iPosition < 0)
    {
        return E_INVALIDARG;
    }

    if (iPosition > 1)
    {
        return VFW_S_NO_MORE_ITEMS;
    }    

    pmt->cbFormat = sizeof(VIDEOINFOHEADER2);
    pmt->pbFormat = (unsigned char*)CoTaskMemAlloc(pmt->cbFormat);
    MSDK_CHECK_POINTER(pmt->pbFormat, E_UNEXPECTED);
    memset(pmt->pbFormat, 0, pmt->cbFormat);

    pDstVIH = reinterpret_cast<VIDEOINFOHEADER2*> (pmt->pbFormat);
    MSDK_CHECK_POINTER(pDstVIH, E_UNEXPECTED);

    guidFormat = *m_pInput->CurrentMediaType().FormatType();

    if (guidFormat == FORMAT_VideoInfo2 || guidFormat == FORMAT_MPEG2_VIDEO)
    {
        pSrcVIH2 = reinterpret_cast<VIDEOINFOHEADER2*>(m_pInput->CurrentMediaType().Format());
        MSDK_CHECK_POINTER(pSrcVIH2, E_UNEXPECTED);

        memcpy(pDstVIH, pSrcVIH2, sizeof(VIDEOINFOHEADER2));        
    }
    else if (guidFormat == FORMAT_VideoInfo)
    {
        pSrcVIH = reinterpret_cast<VIDEOINFOHEADER*>(m_pInput->CurrentMediaType().Format());
        MSDK_CHECK_POINTER(pSrcVIH, E_UNEXPECTED);

        pDstVIH->rcSource         = pSrcVIH->rcSource;
        pDstVIH->rcTarget         = pSrcVIH->rcTarget;
        pDstVIH->dwBitRate        = pSrcVIH->dwBitRate;
        pDstVIH->dwBitErrorRate   = pSrcVIH->dwBitErrorRate;
        pDstVIH->AvgTimePerFrame  = pSrcVIH->AvgTimePerFrame;
        pDstVIH->bmiHeader        = pSrcVIH->bmiHeader;

        pDstVIH->dwPictAspectRatioX = pDstVIH->bmiHeader.biWidth;
        pDstVIH->dwPictAspectRatioY = pDstVIH->bmiHeader.biHeight;        
    }
    else
    {        
        return E_UNEXPECTED;
    }  
   
    pmt->SetType(&MEDIATYPE_Video);
    pmt->SetSampleSize(DIBSIZE(pDstVIH->bmiHeader));
    pmt->SetTemporalCompression(FALSE);

    pmt->SetSubtype(ColorFormat2GUID(m_mfxParamsVideo.mfx.FrameInfo.FourCC));
    pmt->SetFormatType(&FORMAT_VideoInfo2);

    // to be well compatible with EVR
    if (pDstVIH->dwInterlaceFlags & AMINTERLACE_IsInterlaced)
    {
        pDstVIH->dwInterlaceFlags = AMINTERLACE_IsInterlaced & AMINTERLACE_DisplayModeBobOrWeave;
    }

    if (m_mfxParamsVideo.mfx.FrameInfo.FrameRateExtD && m_mfxParamsVideo.mfx.FrameInfo.FrameRateExtN && m_pDecoder->HeaderDecoded())
    {
        pDstVIH->AvgTimePerFrame = (REFERENCE_TIME)(1e7/((DOUBLE)m_mfxParamsVideo.mfx.FrameInfo.FrameRateExtN / m_mfxParamsVideo.mfx.FrameInfo.FrameRateExtD));
    }

    pDstVIH->bmiHeader.biHeight = abs(pDstVIH->bmiHeader.biHeight);

    pDstVIH->rcSource.bottom =
        pDstVIH->rcTarget.bottom = pDstVIH->bmiHeader.biHeight;

    pDstVIH->rcSource.right =
        pDstVIH->rcTarget.right = pDstVIH->bmiHeader.biWidth;

    // consider alignment which is required by MSDK
    pDstVIH->bmiHeader.biWidth  = (pDstVIH->bmiHeader.biWidth + 15) &~ 15;
    pDstVIH->bmiHeader.biHeight = (pDstVIH->bmiHeader.biHeight + 31) &~ 31;

    pDstVIH->bmiHeader.biCompression = ColorFormat2BiCompression(m_mfxParamsVideo.mfx.FrameInfo.FourCC);
    pDstVIH->bmiHeader.biBitCount    = ColorFormat2BiBitCount(m_mfxParamsVideo.mfx.FrameInfo.FourCC);
    pDstVIH->bmiHeader.biPlanes      = 1;
    pDstVIH->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);    

    return S_OK;
}

HRESULT CDecVideoFilter::CheckInputType(const CMediaType *mtIn)
{
    GUID                guidFormat;
    VIDEOINFOHEADER2    vih2;
    ULONG               nVideoInfoSize  = 0;
    mfxStatus           sts             = MFX_ERR_NONE;
    mfxInfoMFX*         pmfx            = &m_mfxParamsVideo.mfx;

    MSDK_CHECK_POINTER(mtIn, E_POINTER);
    MSDK_CHECK_NOT_EQUAL(*mtIn->Type(), MEDIATYPE_Video, VFW_E_INVALIDMEDIATYPE);

    guidFormat = *mtIn->FormatType();

    if (FORMAT_VideoInfo2 == guidFormat || FORMAT_MPEG2_VIDEO == guidFormat)
    {
        VIDEOINFOHEADER2* pvi2 = reinterpret_cast<VIDEOINFOHEADER2*>(mtIn->pbFormat);
        MSDK_CHECK_POINTER(pvi2, E_UNEXPECTED);

        memcpy(&vih2, pvi2, sizeof(VIDEOINFOHEADER2));
        nVideoInfoSize = (FORMAT_VideoInfo2 == guidFormat) ? 
            sizeof(VIDEOINFOHEADER2) : sizeof(MPEG2VIDEOINFO);
    } 
    else if (FORMAT_VideoInfo == guidFormat)
    {
        VIDEOINFOHEADER* pvi = (VIDEOINFOHEADER*)(mtIn->pbFormat);
        MSDK_CHECK_POINTER(pvi, E_UNEXPECTED);

        ZeroMemory(&vih2, sizeof(VIDEOINFOHEADER2));

        vih2.rcSource           = pvi->rcSource;
        vih2.rcTarget           = pvi->rcTarget;
        vih2.dwBitRate          = pvi->dwBitRate;
        vih2.dwBitErrorRate     = pvi->dwBitErrorRate;
        vih2.bmiHeader          = pvi->bmiHeader;
        vih2.AvgTimePerFrame    = pvi->AvgTimePerFrame;

        vih2.dwPictAspectRatioX = pvi->bmiHeader.biWidth;
        vih2.dwPictAspectRatioY = pvi->bmiHeader.biHeight;

        nVideoInfoSize          = sizeof(VIDEOINFOHEADER);
    }
    else
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    if (!vih2.bmiHeader.biWidth || !vih2.bmiHeader.biHeight)
    {
        return VFW_E_INVALIDMEDIATYPE;
    }

    if (vih2.AvgTimePerFrame)
    {
        MSDK_CHECK_POINTER(m_pTimeManager, E_OUTOFMEMORY);
        m_pTimeManager->Init(1e7 / vih2.AvgTimePerFrame);
    }

    //additional class for frames constructing purposes
    if (!m_pFrameConstructor)
    {
        m_pFrameConstructor = new CFrameConstructor;
    }

    //splitter adds decoding headers to the end of the mtIn->pbFormat
    m_pDecoder->HeaderDecoded(false);
    if (mtIn->cbFormat > nVideoInfoSize)
    {
        mfxBitstream  bs;
        ZeroMemory(&bs, sizeof(mfxBitstream));

        mfxVideoParam params;
        ZeroMemory(&params, sizeof(mfxVideoParam));

        params.mfx.CodecId = m_mfxParamsVideo.mfx.CodecId;

        if (MAKEFOURCC('W', 'V', 'C', '1') == vih2.bmiHeader.biCompression)
        {
            nVideoInfoSize++;
        }
        //For AVC1 support
         
        //AVC Format MAY send bitstream SPS/PPS to the decoder via MPEG2VIDEOINFO (out of band) 
        //Use the FrameConstructor Class to parse the data, then use for MediaSDK's DecodeHeader() 
        if((MAKEFOURCC('a','v','c','1') == vih2.bmiHeader.biCompression) &&  (FORMAT_MPEG2_VIDEO == guidFormat))   
        {        
            MPEG2VIDEOINFO *mp2 = reinterpret_cast<MPEG2VIDEOINFO *> (mtIn->pbFormat); 
            MSDK_CHECK_POINTER(mp2, E_UNEXPECTED); 

            if (mp2->cbSequenceHeader > 0 ) //if > 0 then the SPS and/or PPS Data will be present 
            {
                mfxBitstream avcSPS_PPS; 
                ZeroMemory(&avcSPS_PPS, sizeof(mfxBitstream)); 

                CAVCFrameConstructor* pAVCConstructor = dynamic_cast<CAVCFrameConstructor*>(m_pFrameConstructor);

                // AVCFrameConstructor will allocate and fill avcSPS_PPS.Data buffer 
                // and also save headers internally for future use
                sts = pAVCConstructor->ReadAVCHeader(mp2, &avcSPS_PPS);    

                if(MFX_ERR_NONE == sts) 
                {
                    sts = m_pDecoder->DecodeHeader(&avcSPS_PPS, &params); 
                } 

                // Free allocated buffer as it is no longer needed
                MSDK_SAFE_DELETE_ARRAY(avcSPS_PPS.Data);
            }
        }
        
        else if (MAKEFOURCC('W', 'M', 'V', '3') != vih2.bmiHeader.biCompression)
        {
            bs.DataLength = bs.MaxLength 
                = mtIn->cbFormat - nVideoInfoSize;
            bs.Data = (mfxU8*)(mtIn->pbFormat + nVideoInfoSize);
            
            AttachCustomCodecParams(&params);

            sts = m_pDecoder->DecodeHeader(&bs, &params);
        }
        else
        {
            //header should be additionally constructed for main&simple profile in asf data type
            CVC1FrameConstructor* pVC1Constructor = dynamic_cast<CVC1FrameConstructor*>(m_pFrameConstructor);

            pmfx->FrameInfo.CropH = (mfxU16)vih2.bmiHeader.biHeight;
            pmfx->FrameInfo.CropW = (mfxU16)vih2.bmiHeader.biWidth;

            if (pVC1Constructor)
            {
                bs.MaxLength = bs.DataLength = mtIn->cbFormat - nVideoInfoSize + 20;

                bs.Data = new mfxU8[bs.MaxLength];

                sts = pVC1Constructor->ConstructHeaderSM(bs.Data, bs.DataLength, (mfxU8*)(mtIn->pbFormat + nVideoInfoSize), mtIn->cbFormat - nVideoInfoSize);

                if (MFX_ERR_NONE == sts)
                {
                    sts = m_pDecoder->DecodeHeader(&bs, &params);
                }

                MSDK_SAFE_DELETE_ARRAY(bs.Data);
            }
            else
            {

                sts = MFX_ERR_NULL_PTR;
            }
        }
        
        if (MFX_ERR_NONE == sts)
        {
            memcpy(&m_mfxParamsVideo, &params, sizeof(mfxVideoParam));            
        }
    }

    if (nVideoInfoSize == mtIn->cbFormat || MFX_ERR_NONE != sts)
    {
        pmfx->FrameInfo.CropH           = (mfxU16)vih2.bmiHeader.biHeight;
        pmfx->FrameInfo.CropW           = (mfxU16)vih2.bmiHeader.biWidth;

        pmfx->FrameInfo.Width           = (pmfx->FrameInfo.CropW + 15) &~ 15;
        pmfx->FrameInfo.Height          = (pmfx->FrameInfo.CropH + 31) &~ 31;

        pmfx->FrameInfo.FourCC          = MFX_FOURCC_NV12;
        pmfx->FrameInfo.ChromaFormat    = MFX_CHROMAFORMAT_YUV420;        

        ConvertFrameRate(m_pTimeManager->GetFrameRate(), 
                         &pmfx->FrameInfo.FrameRateExtN, 
                         &pmfx->FrameInfo.FrameRateExtD);
        
    }

    // AR is not always contained in the header (it's optional for MFX decoder initialization though)
    if (pmfx->FrameInfo.AspectRatioW * pmfx->FrameInfo.AspectRatioH == 0)
    {
        //convert display aspect ratio (is in VIDEOINFOHEADER2) to pixel aspect ratio (is accepted by MediaSDK components)
        sts = DARtoPAR(vih2.dwPictAspectRatioX, vih2.dwPictAspectRatioY, 
            pmfx->FrameInfo.CropW, pmfx->FrameInfo.CropH,
            &pmfx->FrameInfo.AspectRatioW, &pmfx->FrameInfo.AspectRatioH);
        MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

        m_nPARW = pmfx->FrameInfo.AspectRatioW;
        m_nPARH = pmfx->FrameInfo.AspectRatioH;        
    }    

    // set IOPattern based on allocator type (actual or default)
    m_mfxParamsVideo.IOPattern = (mfxU16)((m_pDecoder->m_bd3dAlloc) ? 
        MFX_IOPATTERN_OUT_VIDEO_MEMORY : MFX_IOPATTERN_OUT_SYSTEM_MEMORY); 
    
    //update pitch
    if (State_Stopped == m_State)
    {
        m_nPitch = pmfx->FrameInfo.Width;
    }    

    return S_OK;
};

// Fills a counted array of GUID values where each GUID specifies the CLSID of each property page that can be displayed
HRESULT CDecVideoFilter::GetPages(CAUUID *pPages)
{
    MSDK_CHECK_POINTER(pPages, E_POINTER);

    pPages->cElems = 2;
    pPages->pElems = (GUID*)CoTaskMemAlloc(sizeof(GUID) * pPages->cElems);

    MSDK_CHECK_POINTER(pPages->pElems, E_OUTOFMEMORY);

    pPages->pElems[0] = CLSID_VideoPropertyPage;
    pPages->pElems[1] = CLSID_AboutPropertyPage;

    return S_OK;
};

HRESULT CDecVideoFilter::GetRunTimeStatistics(Statistics *statistics)
{
    MSDK_CHECK_POINTER(statistics, E_POINTER);

    //set all values to 0
    memset(statistics, 0, sizeof(Statistics));

    //check if encoder was already created
    if (m_pDecoder)
    {
        mfxVideoParam mfxParams = m_mfxParamsVideo;
        m_pDecoder->GetVideoParams(&mfxParams);

        mfxDecodeStat mfxStat;
        mfxStatus sts = m_pDecoder->GetDecodeStat(&mfxStat);

        if (MFX_ERR_NONE == sts)
        {
            //fill statistics
            statistics->frames_decoded = mfxStat.NumFrame + 1;
        }

        //fill statistics
        statistics->decode_time = (DWORD)(m_pTimeManager->GetTime() * 1e2);
        statistics->width = mfxParams.mfx.FrameInfo.CropW + mfxParams.mfx.FrameInfo.CropX;
        statistics->height = mfxParams.mfx.FrameInfo.CropH + mfxParams.mfx.FrameInfo.CropY;

        if (mfxParams.mfx.FrameInfo.AspectRatioH * mfxParams.mfx.FrameInfo.AspectRatioW != 0)
        {
            statistics->aspect_ratio.vertical = mfxParams.mfx.FrameInfo.AspectRatioH;
            statistics->aspect_ratio.horizontal = mfxParams.mfx.FrameInfo.AspectRatioW;
        }
        else
        {
            // if the elementary video stream doesn't contain AR values (e.g. VC1 simple and main profiles)
            // we take the values from VideoType saved at connection
            statistics->aspect_ratio.vertical = m_nPARH;
            statistics->aspect_ratio.horizontal = m_nPARW;
        }        

        statistics->frame_rate = (DWORD)(m_pTimeManager->GetFrameRate() * 100);
    }
    return S_OK;
};

CBasePin* CDecVideoFilter::GetPin(int n)
{
    HRESULT hr = S_OK;

    if (0 == n)
    {
        if (m_pInput == NULL)
        {
            m_pInput = new CTransformInputPin(NAME("InputPin"), this, &hr, L"In");
        }

        return m_pInput;
    }

    if (1 == n)
    {
        if (NULL == m_pOutput)
        {
            m_pOutput = m_pDecoderOutput = new CDecoderOutputPin(this, &hr); 
        }

        return m_pOutput;
    }

    return NULL;
};

HRESULT QueryPinVideoMemSupport(IPin *pPin)
{
    HRESULT hr = S_OK;
    CComPtr<IDirectXVideoMemoryConfiguration> pVideoMemConfig(NULL);
    
    // Get the IDirectXVideoMemoryConfiguration interface.
    hr = pPin->QueryInterface(__uuidof(IDirectXVideoMemoryConfiguration), (void**)&pVideoMemConfig);

    // Notify the EVR.
    if (SUCCEEDED(hr))
    {
        DXVA2_SurfaceType surfaceType;
        DXVA2_SurfaceType ourType = DXVA2_SurfaceType_DecoderRenderTarget;

        for (DWORD nTypeIndex = 0; ; nTypeIndex++)
        {
            hr = pVideoMemConfig->GetAvailableSurfaceTypeByIndex(nTypeIndex, &surfaceType);

            if (FAILED(hr))
            {
                break;
            }

            if (surfaceType == ourType)
            {
                hr = pVideoMemConfig->SetSurfaceType(ourType);
                break;
            }
        }
    }

    return hr;
}

HRESULT CDecVideoFilter::CompleteConnect(PIN_DIRECTION direction, IPin *pReceivePin)
{
    HRESULT   hr  = S_OK;
    mfxStatus sts = MFX_ERR_NONE;    

    // create and init frame/MediaSample allocator at output pin
    if (direction == PINDIR_OUTPUT && !m_pFrameAllocator)
    {            
        hr = QueryPinVideoMemSupport(pReceivePin);
        if (SUCCEEDED(hr))
        {
            m_pDecoder->m_bd3dAlloc = TRUE;
            m_pDecoderOutput->m_bRequireOwnDSAllocator = TRUE;
        }
        else
        {
            hr = S_OK;
        }

        // now we know pattern (allocator type)
        m_mfxParamsVideo.IOPattern = (mfxU16)((m_pDecoder->m_bd3dAlloc) ? 
            MFX_IOPATTERN_OUT_VIDEO_MEMORY : MFX_IOPATTERN_OUT_SYSTEM_MEMORY);              

        if (m_pDecoder->m_bd3dAlloc)
        {
            std::auto_ptr<DSAllocatorParams> pParam(NULL);

            pParam.reset(new DSAllocatorParams);
            MSDK_CHECK_POINTER(pParam.get(), MFX_ERR_NULL_PTR);
                                        
            pParam->pPin = pReceivePin;            
            
            MFXDSFrameAllocatorD3D *pAlloc = new MFXDSFrameAllocatorD3D(_T("d3d allocator"), NULL, &hr);             
           
            m_pFrameAllocator = pAlloc; 

            sts = m_pFrameAllocator->Init(pParam.get());     
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

            sts = m_pDecoder->SetHandle(pParam.get());            
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

            sts = m_pDecoder->SetFrameAllocator(m_pFrameAllocator);  
            MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, E_FAIL);

            m_pDecoderOutput->SetAllocator(pAlloc);
        }
        else
        {                 
            MFXDSFrameAllocatorSys *pAlloc = new MFXDSFrameAllocatorSys(_T("sys mem allocator"), NULL, &hr);              

            m_pDecoderOutput->SetAllocator(pAlloc);

            // Suspend setting m_pDecoder frame allocator until DS allocator is decided,
            // only save the pointer and Initialize
            m_pFrameAllocator = pAlloc;

            m_pFrameAllocator->Init(NULL);
        }                                                    
    }

    if (direction == PINDIR_INPUT)
    {
        m_bLowLatencyMode = TRUE;
        m_mfxParamsVideo.AsyncDepth = 1;

        PIN_INFO infoPin;
        memset(&infoPin, 0, sizeof(infoPin));
        hr = pReceivePin->QueryPinInfo(&infoPin);

        if (SUCCEEDED(hr))
        {
            CComPtr<IConfigureVideoEncoder> pInterface(NULL);
            
            if (SUCCEEDED(infoPin.pFilter->QueryInterface(IID_IConfigureVideoEncoder, reinterpret_cast<void**>(&pInterface)))
                && pInterface)
            {
                //encode -> decode chain is for low latency purposes
                m_bLowLatencyMode = TRUE;
                m_mfxParamsVideo.AsyncDepth = 1;
            }

            MSDK_SAFE_RELEASE(infoPin.pFilter);
        }        
    }

    if (SUCCEEDED(hr))
    {
        hr = CTransformFilter::CompleteConnect(direction, pReceivePin);        
    }

    // check if filter should try joining its session to a session from downstream filter
    if (SUCCEEDED(hr) && direction == PINDIR_OUTPUT && m_bJoinSession && m_pDecoder)
    {  
        CComPtr<IShareMfxSession> pMFXPin(NULL);
        MFXVideoSession *pSession = NULL;        
        
        // check if downstream filter can share an mfx session
        hr = pReceivePin->QueryInterface(IID_IShareMfxSession, reinterpret_cast<void**>(&pMFXPin));

        if (SUCCEEDED(hr))
        {
            // get session  
            pMFXPin->GetMfxSession(reinterpret_cast<void**>(&pSession));

            // try joining: downstream session - parent, decoder's session - child
            sts = MFXJoinSession(*pSession, m_pDecoder->m_mfxVideoSession);

            if (MFX_ERR_NONE == sts)
            {
                m_pDecoder->m_bSessionHasParent = true;
                pMFXPin->SygnalSessionHasChild();
                m_pDecoderOutput->m_bRequireOwnDSAllocator = TRUE;
            }               
        }
        else
        {
            hr = S_OK;        
        }
    }

    // reconnect the output pin if it is connected during input pin connection final stage
    if (PINDIR_INPUT == direction && m_pOutput && m_pOutput->IsConnected() && SUCCEEDED(hr))
    {        
        CComPtr<IFilterGraph>   pGraph;

        hr = m_pGraph->QueryInterface(IID_IFilterGraph, (void**)&pGraph); 
        MSDK_CHECK_RESULT(hr, S_OK, E_FAIL);
        MSDK_CHECK_POINTER(pGraph, E_FAIL);

        hr = pGraph->Reconnect(m_pOutput);                 
    }

    return hr;
};

CDecoderOutputPin::CDecoderOutputPin(CTransformFilter* pTransformFilter, HRESULT* phr) :
CTransformOutputPin(NAME("OutputPin"), pTransformFilter, phr, L"Out")
{
    m_pOwnAllocator = NULL;
    m_bRequireOwnDSAllocator = FALSE;
    m_bOwnDSAllocatorAgreed = FALSE;
};

HRESULT CDecoderOutputPin::QueryAccept( const AM_MEDIA_TYPE *pmt )
{
    /* TODO: need to set new allocator properties. 
    Although we currently don't have scenarios where it might be needed */

    ((CDecVideoFilter*)m_pFilter)->SetNewStride(((VIDEOINFOHEADER2*)(pmt->pbFormat))->bmiHeader.biWidth);
    
    return CTransformOutputPin::QueryAccept(pmt);

};

HRESULT CDecoderOutputPin::InitAllocator(IMemAllocator **ppAlloc)
{
    HRESULT hr = S_OK;  

    MSDK_CHECK_POINTER(ppAlloc, E_POINTER);
    MSDK_CHECK_POINTER(m_pOwnAllocator, E_POINTER);

    *ppAlloc = m_pOwnAllocator;
    m_pOwnAllocator->AddRef();   
    
    return hr;
}

CDecoderOutputPin::~CDecoderOutputPin()
{
    MSDK_SAFE_RELEASE(m_pOwnAllocator);
}

HRESULT CDecoderOutputPin::SetAllocator(IMemAllocator *pAlloc)
{
    MSDK_SAFE_RELEASE(m_pOwnAllocator);

    if (NULL != pAlloc)
    {
        pAlloc->AddRef();
    }

    m_pOwnAllocator = pAlloc;

    MSDK_SAFE_RELEASE(CBaseOutputPin::m_pAllocator);
    CBaseOutputPin::m_pAllocator = pAlloc;

    return S_OK;
};

HRESULT CDecoderOutputPin::NotifyFormatChange(const CMediaType* pmt)
{    
    CComPtr<IPin> pPin;
    HRESULT hr;
    ALLOCATOR_PROPERTIES prop;

    pPin = GetConnected();
    if (!pPin)
    {
        return E_POINTER;
    } 

    hr = pPin->ReceiveConnection(this, pmt);    

    if (SUCCEEDED(hr) || hr == VFW_E_ALREADY_CONNECTED)
    {
        SetMediaType(pmt);
        
        m_pAllocator->Decommit();        
        
        hr = m_pInputPin->GetAllocatorRequirements(&prop);

        if (FAILED(hr))
        {
            memset(&prop, 0, sizeof(ALLOCATOR_PROPERTIES));
        }
        
        hr = DecideBufferSize(m_pAllocator, &prop);            
        
        if (SUCCEEDED(hr))
        {
            m_pInputPin->NotifyAllocator(m_pAllocator, false);
        }

        m_pAllocator->Commit();
    }

    return hr;
}

HRESULT CDecoderOutputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = m_pTransformFilter->CompleteConnect(PINDIR_OUTPUT,pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    
    hr = CBaseOutputPin::CompleteConnect(pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }

    return hr;    
}


/* Override DecideAllocator to try own allocator first. 
If downstream pin refuses (it may require its own allocator - e.g. like VMR), 
then agree to use their allocator for MediaSample buffers, but still use own memory for MFX frames. 
A copy from mfxFrameSurface to IMediaSample will be done on Deliver. 
This does not apply to D3D memory allocator, with D3D we require own allocator to be used. */
HRESULT CDecoderOutputPin::DecideAllocator(IMemInputPin *pPin, __deref_out IMemAllocator **ppAlloc)
{
    HRESULT hr = NOERROR;
    *ppAlloc = NULL;

    // get downstream prop request    
    ALLOCATOR_PROPERTIES prop;
    ZeroMemory(&prop, sizeof(prop));

    // whatever he returns, we assume prop is either all zeros
    // or he has filled it out.
    pPin->GetAllocatorRequirements(&prop);

    // if he doesn't care about alignment, then set it to 1
    if (prop.cbAlign == 0) {
        prop.cbAlign = 1;
    }

    /* Try the output pin's allocator */

    hr = InitAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {
        
        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                m_bOwnDSAllocatorAgreed = TRUE;
                return NOERROR;
            }
        }
    }

    /* We may not have an interface to release */

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }

    /* Try the allocator provided by the input pin by the same method */

    hr = pPin->GetAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {

        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                m_bOwnDSAllocatorAgreed = FALSE;
                return NOERROR;
            }
        }
    }  

    /* Likewise we may not have an interface to release */

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }

    return hr;
}

