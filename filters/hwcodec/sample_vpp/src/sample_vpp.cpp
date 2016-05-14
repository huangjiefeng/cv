//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2008 - 2011 Intel Corporation. All Rights Reserved.
//

#include "sample_vpp_utils.h"

/* default params */
const sOwnFrameInfo  defaultOwnFrameInfo = {352, 288, 0, 0, 352, 288, MFX_FOURCC_NV12, MFX_PICSTRUCT_PROGRESSIVE, 30.0};
const sProcAmpParam  defaultProcAmpParam = { 0.0, 1.0, 1.0, 0.0, VPP_FILTER_DISABLED};
const sDetailParam   defaultDetailParam  = { 1,  VPP_FILTER_DISABLED};
const sDenoiseParam  defaultDenoiseParam = { 1,  VPP_FILTER_DISABLED};
const sVideoAnalysisParam defaultVAParam = { VPP_FILTER_DISABLED};

static
void vppDefaultInitParams( sInputParams* pParams )
{
  pParams->frameInfo[VPP_IN]  = defaultOwnFrameInfo;
  pParams->frameInfo[VPP_OUT] = defaultOwnFrameInfo;

  // Video Enhancement Algorithms
  pParams->denoiseParam = defaultDenoiseParam;
  pParams->detailParam  = defaultDetailParam;
  pParams->procampParam = defaultProcAmpParam;
  pParams->vaParam      = defaultVAParam; 

  pParams->bd3dAlloc    = false;//using system memory

  pParams->requestedFramesCount = MFX_MAX_32U;

  pParams->impLib = MFX_IMPL_SOFTWARE;

  return;

} // void vppDefaultInitParams( sInputParams* pParams )

static
void vppSafeRealInfo( sOwnFrameInfo* in, mfxFrameInfo* out )
{
  out->Width  = in->nWidth;
  out->Height = in->nHeight;
  out->CropX  = 0;
  out->CropY  = 0;
  out->CropW  = out->Width;
  out->CropH  = out->Height;
  out->FourCC = in->FourCC;

  return;

} // void vppSafeRealInfo( sOwnFrameInfo* in, mfxFrameInfo* out )

int _tmain(int argc, TCHAR *argv[])
{
  mfxStatus           sts = MFX_ERR_NONE;
  mfxU32              nFrames = 0;

  CRawVideoReader     yuvReader;
  CRawVideoWriter     yuvWriter;

  sFrameProcessor     FrameProcessor;
  sMemoryAllocator    Allocator;

  sInputParams        Params;
  mfxVideoParam       mfxParamsVideo;
  sAppResources       Resources;

  // to prevent incorrect read/write of image in case of CropW/H != width/height 
  mfxFrameInfo        frameInfo[2];

  mfxFrameSurface1*   pSurf[2];// 0 - in, 1 - out
  
  mfxSyncPoint        syncPoint;

  mfxExtVppAuxData    extVPPAuxData;

  mfxU32              readFramesCount = 0;
   
  //reset pointers to the all internal resources
  MSDK_ZERO_MEMORY(Resources);
  MSDK_ZERO_MEMORY(mfxParamsVideo);
  MSDK_ZERO_MEMORY(Params);
  MSDK_ZERO_MEMORY(Allocator);
  MSDK_ZERO_MEMORY(FrameProcessor);
  MSDK_ZERO_MEMORY(frameInfo[VPP_IN]);
  MSDK_ZERO_MEMORY(frameInfo[VPP_OUT]);

  Resources.pSrcFileReader    = &yuvReader;
  Resources.pDstFileWriter    = &yuvWriter;
  Resources.pProcessor        = &FrameProcessor;
  Resources.pAllocator        = &Allocator;
  Resources.pVppParams        = &mfxParamsVideo;

  vppDefaultInitParams( &Params );

  //parse input string
  sts = vppParseInputString(argv, (mfxU8)argc, &Params);
  MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, 1);

  // to prevent incorrect read/write of image in case of CropW/H != width/height 
  // application save real image size
  vppSafeRealInfo( &(Params.frameInfo[VPP_IN]),  &frameInfo[VPP_IN]  );
  vppSafeRealInfo( &(Params.frameInfo[VPP_OUT]), &frameInfo[VPP_OUT] );
  
  //prepare file reader (YUV/RGB file)  
  sts = yuvReader.Init(Params.strSrcFile);
  MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, 1);     

  //prepare file writer (YUV file)  
  sts = yuvWriter.Init(Params.strDstFile);                                                
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));                    

  //prepare mfxParams
  sts = InitParamsVPP(&mfxParamsVideo, &Params);
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));

  sts = ConfigVideoEnhancementFilters(&Params, &Resources);
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));

  sts = InitResources(&Resources, &mfxParamsVideo, Params.impLib);
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));

  // print parameters to console
  PrintInfo(&Params, &mfxParamsVideo, &Resources.pProcessor->mfxSession);

  sts = MFX_ERR_NONE;
  nFrames = 1;

  _tprintf(_T("VPP started\n"));
  
  bool isMultipleOut = false;
  while (MFX_ERR_NONE <= sts || MFX_ERR_MORE_DATA == sts || isMultipleOut )
  {
    if( !isMultipleOut )
    {    
        sts = GetFreeSurface(Allocator.pSurfaces[VPP_IN], 
                             Allocator.response[VPP_IN].NumFrameActual, 
                             &pSurf[VPP_IN]);
        MSDK_BREAK_ON_ERROR(sts);
        if (readFramesCount++ == Params.requestedFramesCount)
        {
            sts = MFX_ERR_MORE_DATA;
            break;
        }
          
        // if we share allocator with mediasdk we need to call Lock to access surface data and after we're done call Unlock
        if (Allocator.bUsedAsExternalAllocator) 
        {
          // get YUV pointers
          sts = Allocator.pMfxAllocator->Lock(Allocator.pMfxAllocator->pthis, pSurf[VPP_IN]->Data.MemId, &(pSurf[VPP_IN]->Data));
          MSDK_BREAK_ON_ERROR(sts);

          sts = yuvReader.LoadNextFrame( &(pSurf[VPP_IN]->Data), &(frameInfo[VPP_IN]));
          MSDK_BREAK_ON_ERROR(sts);

          sts = Allocator.pMfxAllocator->Unlock(Allocator.pMfxAllocator->pthis, pSurf[VPP_IN]->Data.MemId, &(pSurf[VPP_IN]->Data));
          MSDK_BREAK_ON_ERROR(sts);
        }
        else 
        {
          sts = yuvReader.LoadNextFrame( &(pSurf[VPP_IN]->Data), &(frameInfo[VPP_IN]));
          MSDK_BREAK_ON_ERROR(sts);
        }
    }

    sts = GetFreeSurface(Allocator.pSurfaces[VPP_OUT], 
                         Allocator.response[VPP_OUT].NumFrameActual, 
                         &pSurf[VPP_OUT]);
    MSDK_BREAK_ON_ERROR(sts);

    // VPP processing    
    isMultipleOut = false;
    sts = FrameProcessor.pmfxVPP->RunFrameVPPAsync( pSurf[VPP_IN], pSurf[VPP_OUT],
                                                    (VPP_FILTER_DISABLED != Params.vaParam.mode)? &extVPPAuxData : NULL, 
                                                     &syncPoint );

    if (MFX_ERR_MORE_DATA == sts)
    {
      continue;
    }

    //MFX_ERR_MORE_SURFACE means output is ready but need more surface
    //because VPP produce multiple out. example: Frame Rate Conversion 30->60
    if (MFX_ERR_MORE_SURFACE == sts)
    {
        isMultipleOut = true;
        sts = MFX_ERR_NONE;
    }

    MSDK_BREAK_ON_ERROR(sts);

    sts = Resources.pProcessor->mfxSession.SyncOperation(syncPoint, MSDK_VPP_WAIT_INTERVAL);
    MSDK_BREAK_ON_ERROR(sts);

    // if we share allocator with mediasdk we need to call Lock to access surface data and after we're done call Unlock
    if (Allocator.bUsedAsExternalAllocator) 
    {
      // get YUV pointers
      sts = Allocator.pMfxAllocator->Lock(Allocator.pMfxAllocator->pthis, pSurf[VPP_OUT]->Data.MemId, &(pSurf[VPP_OUT]->Data));
      MSDK_BREAK_ON_ERROR(sts);

      sts = yuvWriter.WriteFrame( &(pSurf[VPP_OUT]->Data), &(frameInfo[VPP_OUT]) );
      MSDK_BREAK_ON_ERROR(sts);

      sts = Allocator.pMfxAllocator->Unlock(Allocator.pMfxAllocator->pthis, pSurf[VPP_OUT]->Data.MemId, &(pSurf[VPP_OUT]->Data));
      MSDK_BREAK_ON_ERROR(sts);
    }
    else 
    {
      sts = yuvWriter.WriteFrame( &(pSurf[VPP_OUT]->Data), &(frameInfo[VPP_OUT]) );
      MSDK_BREAK_ON_ERROR(sts);
    }

    //VPP progress
    if( VPP_FILTER_DISABLED == Params.vaParam.mode )
    {
      _tprintf(_T("Frame number: %u\r"), nFrames++);
    } 
    else 
    {
      _tprintf(_T("Frame number: %u, spatial: %u, temporal: %u, scd: %u \n"), nFrames++, 
                                                            extVPPAuxData.SpatialComplexity,
                                                            extVPPAuxData.TemporalComplexity,
                                                            extVPPAuxData.SceneChangeRate);
    }
  }

  // means that file has ended, need to go to buffering loop
  MSDK_IGNORE_MFX_STS(sts, MFX_ERR_MORE_DATA);
  // exit in case of other errors
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));

  // loop to get buffered frames from VPP
  while (MFX_ERR_NONE <= sts)
  { 
    sts = GetFreeSurface(Allocator.pSurfaces[VPP_OUT], 
                         Allocator.response[VPP_OUT].NumFrameActual, 
                         &pSurf[VPP_OUT]);
    MSDK_BREAK_ON_ERROR(sts);

    sts = FrameProcessor.pmfxVPP->RunFrameVPPAsync( NULL, pSurf[VPP_OUT],
                                                    (VPP_FILTER_DISABLED != Params.vaParam.mode)? &extVPPAuxData : NULL, 
                                                    &syncPoint );
    MSDK_IGNORE_MFX_STS(sts, MFX_ERR_MORE_SURFACE);
    MSDK_BREAK_ON_ERROR(sts);

    sts = Resources.pProcessor->mfxSession.SyncOperation(syncPoint, MSDK_VPP_WAIT_INTERVAL);
    MSDK_BREAK_ON_ERROR(sts);

    // if we share allocator with mediasdk we need to call Lock to access surface data and after we're done call Unlock
    if (Allocator.bUsedAsExternalAllocator) 
    {
      // get YUV pointers
      sts = Allocator.pMfxAllocator->Lock(Allocator.pMfxAllocator->pthis, pSurf[VPP_OUT]->Data.MemId, &(pSurf[VPP_OUT]->Data));
      MSDK_BREAK_ON_ERROR(sts);

      sts = yuvWriter.WriteFrame( &(pSurf[VPP_OUT]->Data), &(frameInfo[VPP_OUT]) );
      MSDK_BREAK_ON_ERROR(sts);

      sts = Allocator.pMfxAllocator->Unlock(Allocator.pMfxAllocator->pthis, pSurf[VPP_OUT]->Data.MemId, &(pSurf[VPP_OUT]->Data));
      MSDK_BREAK_ON_ERROR(sts);
    }
    else 
    {
      sts = yuvWriter.WriteFrame( &(pSurf[VPP_OUT]->Data), &(frameInfo[VPP_OUT]) );
      MSDK_BREAK_ON_ERROR(sts);
    }

    //VPP progress
    if( VPP_FILTER_DISABLED == Params.vaParam.mode )
    {
      _tprintf(_T("Frame number: %u\r"), nFrames++);
    } 
    else 
    {
      _tprintf(_T("Frame number: %u, spatial: %u, temporal: %u, scd: %u \n"), nFrames++,
        extVPPAuxData.SpatialComplexity,
        extVPPAuxData.TemporalComplexity,
        extVPPAuxData.SceneChangeRate);
    }
  }

  MSDK_IGNORE_MFX_STS(sts, MFX_ERR_MORE_DATA);

  // report any errors that occurred 
  MSDK_CHECK_RESULT_SAFE(sts, MFX_ERR_NONE, 1, WipeResources(&Resources));

  _tprintf(_T("\nVPP finished\n"));

  WipeResources(&Resources);

  return 0; /* OK */

} // int _tmain(int argc, TCHAR *argv[])
/* EOF */
