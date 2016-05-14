//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
//

#include "mfx_video_enc_filter_utils.h"
#include "mfx_filter_guid.h"
#include "mfx_filter_defs.h"

mfxStatus InitMfxFrameSurface( mfxFrameSurface1* pSurface, mfxFrameInfo* pInfo, int nSurfaceSize )
{
    //check input params
    MSDK_CHECK_POINTER(pSurface,   MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pInfo,      MFX_ERR_NULL_PTR);

    //prepare pSurface->Info
    memset(&pSurface->Info, 0, sizeof(mfxFrameInfo));                               // zero memory

    pSurface->Info.FourCC           = pInfo->FourCC;                                // FourCC type
    pSurface->Info.Width            = pInfo->Width;                                 // width, should be multiplier of 16
    pSurface->Info.Height           = pInfo->Height;                                // height, should be multiplier of 16 or 32 (interlaced frame)
    pSurface->Info.PicStruct        = pInfo->PicStruct;                             // picture structure
    pSurface->Info.ChromaFormat     = MFX_CHROMAFORMAT_YUV420;                      // picture chroma format
    pSurface->Info.CropH            = pInfo->CropH;                                 // height crop
    pSurface->Info.CropW            = pInfo->CropW;                                 // width crop

    //Set Pitch
    if(pInfo->FourCC == MFX_FOURCC_NV12)
    {
        pSurface->Data.Pitch            = (mfxU16)(nSurfaceSize / pSurface->Info.CropH / 3 * 2);  //factor = 1 for NV12 and YV12
    }
    else if(pInfo->FourCC == MFX_FOURCC_RGB4)
    {
        pSurface->Data.Pitch            = (mfxU16)(nSurfaceSize / pSurface->Info.CropH);
    }
    else if(pInfo->FourCC == MFX_FOURCC_YUY2)
    {
        pSurface->Data.Pitch            = (mfxU16)(nSurfaceSize / pSurface->Info.CropH);
    }
    
    return MFX_ERR_NONE;
}

static const struct
{
    mfxU32          nWidth;
    mfxU32          nMin;
    mfxU32          nMax;    
    mfxU32          nRecommended;
}
bitrate_ranges[] =
{
    { (176 + 352)/2,   60,      480,    200},
    { (352 + 640)/2,   150,     4000,   900},
    { (640 + 1280)/2,  500,     20000,  6000},
    { (1280 + 1920)/2, 1000,    30000,  9000},
    { 1920 * 2,        2000,    40000,  11500},
};

mfxStatus GetBitrateRanges(mfxU32 nWidth, mfxU32* nMin, mfxU32* nMax, mfxU32* nRecommended)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(bitrate_ranges); i++)
    {
        if (bitrate_ranges[i].nWidth > nWidth) 
        {
            *nMin            = bitrate_ranges[i].nMin;
            *nMax            = bitrate_ranges[i].nMax;
            *nRecommended    = bitrate_ranges[i].nRecommended;

            return MFX_ERR_NONE;
        } 
    }
    return MFX_ERR_UNSUPPORTED;
};

mfxStatus CopyMFXToEncoderParams(IConfigureVideoEncoder::Params* pParams, mfxVideoParam* pmfxParams)
{     
    //check input params
    MSDK_CHECK_POINTER(pParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pmfxParams, MFX_ERR_NULL_PTR);
    
    mfxInfoMFX* pMFXInfo  = &pmfxParams->mfx;

    pParams->level_idc                      = (IConfigureVideoEncoder::Params::Level)
                                              pMFXInfo->CodecLevel;
    pParams->profile_idc                    = (IConfigureVideoEncoder::Params::Profile)
                                              pMFXInfo->CodecProfile;    
    pParams->pc_control                     = (IConfigureVideoEncoder::Params::PCControl)
                                              pMFXInfo->FrameInfo.PicStruct;   
    pParams->rc_control.bitrate             = pMFXInfo->TargetKbps;
    
    pParams->rc_control.rc_method           = (IConfigureVideoEncoder::Params::RCControl::RCMethod)
                                              pMFXInfo->RateControlMethod;    
    pParams->ps_control.NumSlice            = pMFXInfo->NumSlice;    
    pParams->ps_control.GopRefDist          = pMFXInfo->GopRefDist;
    pParams->ps_control.GopPicSize          = pMFXInfo->GopPicSize;
    pParams->ps_control.BufferSizeInKB      = pMFXInfo->BufferSizeInKB;    
    pParams->target_usage                   = pMFXInfo->TargetUsage;   

    pParams->frame_control.width            = pMFXInfo->FrameInfo.CropW;
    pParams->frame_control.height           = pMFXInfo->FrameInfo.CropH;        

    return MFX_ERR_NONE;
}

mfxStatus CopyEncoderToMFXParams(IConfigureVideoEncoder::Params* pParams, mfxVideoParam* pmfxParams)
{   
    //check input params
    MSDK_CHECK_POINTER(pParams, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pmfxParams, MFX_ERR_NULL_PTR);


    //prepare mfxVideoParam
    mfxInfoMFX* pMFXInfo  = &pmfxParams->mfx;
    pMFXInfo->MaxKbps                   = (mfxU16)pParams->rc_control.bitrate;
    pMFXInfo->TargetKbps                = (mfxU16)pParams->rc_control.bitrate;

    pMFXInfo->FrameInfo.CropW           = (mfxU16)(0 == pParams->frame_control.width ? pMFXInfo->FrameInfo.CropW : pParams->frame_control.width);
    pMFXInfo->FrameInfo.CropH           = (mfxU16)(0 == pParams->frame_control.height ? pMFXInfo->FrameInfo.CropH : pParams->frame_control.height);

    mfxStatus sts = CodecPreset::VParamsFromPreset(*pmfxParams, pParams->preset);
    MSDK_CHECK_RESULT(sts, MFX_ERR_NONE, sts);

    pMFXInfo->CodecLevel                = (mfxU16)pParams->level_idc;
    pMFXInfo->CodecProfile              = (mfxU16)pParams->profile_idc;

    pMFXInfo->FrameInfo.PicStruct       = MFX_PICSTRUCT_PROGRESSIVE;
    pMFXInfo->RateControlMethod         = (mfxU16)pParams->rc_control.rc_method;        

    if (pParams->ps_control.GopRefDist) pMFXInfo->GopRefDist = (mfxU8)pParams->ps_control.GopRefDist;
    if (pParams->ps_control.GopPicSize) pMFXInfo->GopPicSize = (mfxU16)pParams->ps_control.GopPicSize;
    if (pParams->ps_control.BufferSizeInKB) pMFXInfo->BufferSizeInKB = (mfxU16)pParams->ps_control.BufferSizeInKB;
    if (pParams->ps_control.NumSlice) pMFXInfo->NumSlice = (mfxU16)pParams->ps_control.NumSlice;

    pMFXInfo->TargetUsage               = (mfxU8)pParams->target_usage;
    if (pParams->rc_control.bitrate) {
        pMFXInfo->MaxKbps                   = (mfxU16)pParams->rc_control.bitrate;
        pMFXInfo->TargetKbps                = (mfxU16)pParams->rc_control.bitrate;
    }
    
 //   CodecPreset::VParamsFromPreset(*pmfxParams, pParams->preset);
// 	pMFXInfo->QPI = 27;
//     pMFXInfo->QPP = 27;

    printf("[H264Enc] Frame size: %dx%d, Bitrate = %dKbps\n", 
        pMFXInfo->FrameInfo.CropW, pMFXInfo->FrameInfo.CropH, pMFXInfo->TargetKbps);

    return MFX_ERR_NONE;
}