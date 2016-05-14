/* ////////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2010 - 2011 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __SAMPLE_VPP_CONFIG_H
#define __SAMPLE_VPP_CONFIG_H

#include "mfxvideo.h"
#include "mfxvideo++.h"

enum
{
    NOT_INIT_VALUE      =   0xFFF7
};

// number of video enhancement filters (denoise, procamp, detail, video_analysis)
#define ENH_FILTERS_COUNT                4

#define VPP_PROCAMP_BRIGHTNESS_DEFAULT    0.0
#define VPP_PROCAMP_CONTRAST_DEFAULT      1.0
#define VPP_PROCAMP_HUE_DEFAULT           0.0
#define VPP_PROCAMP_SATURATION_DEFAULT    1.0

#define VPP_DENOISE_FACTOR_DEFAULT      NOT_INIT_VALUE
#define VPP_FILTER_FACTOR_DEFAULT       NOT_INIT_VALUE

typedef enum 
{
    VPP_FILTER_DISABLED           =0,
    VPP_FILTER_ENABLED_DEFAULT    =1,
    VPP_FILTER_ENABLED_CONFIGURED =7    

} FilterConfig;

typedef struct
{    
    FilterConfig mode;

} sVideoAnalysisParam;

typedef struct
{    
    mfxF64   brightness;           
    mfxF64   contrast;                                  
    mfxF64   saturation;
    mfxF64   hue; 

    FilterConfig mode;

} sProcAmpParam;

typedef struct
{    
    mfxU16  factor;

    FilterConfig mode;

} sDenoiseParam;

typedef struct
{    
    mfxU16  factor;

    FilterConfig mode;

} sDetailParam;

#endif /* __SAMPLE_VPP_CONFIG_H */
/* EOF */
