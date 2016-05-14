/* ////////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2008 - 2011 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __SAMPLE_VPP_UTILS_H
#define __SAMPLE_VPP_UTILS_H

/* ************************************************************************* */

#define D3D_SURFACES_SUPPORT

#ifdef D3D_SURFACES_SUPPORT
#pragma warning(disable : 4201)
#include <d3d9.h>
#include <dxva2api.h>
#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "mfxvideo.h"
#include "mfxvideo++.h"

#include "sample_defs.h"
#include "sample_utils.h"
#include "base_allocator.h"

#include "sample_vpp_config.h"
/* ************************************************************************* */

#define VPP_IN       (0)
#define VPP_OUT      (1)

#define MFX_MAX_32U   ( 0xFFFFFFFF )

// we introduce new macros without error message (returned status only)
// it allows to remove final error message due to EOF
#define IOSTREAM_CHECK_NOT_EQUAL(P, X, ERR)          {if ((X) != (P)) {return ERR;}}

typedef struct _ownFrameInfo
{
  mfxU16  nWidth;
  mfxU16  nHeight;
  // ROI
  mfxU16  CropX;
  mfxU16  CropY;
  mfxU16  CropW;
  mfxU16  CropH;

  mfxU32 FourCC;
  mfxU8  PicStruct;
  mfxF64 dFrameRate;

} sOwnFrameInfo;

struct sInputParams
{
  /* smart filters defined by mismatch btw src & dst */
  sOwnFrameInfo frameInfo[2];// [0] - in, [1] - out

  /* Video Enhancement Algorithms */
  sDenoiseParam denoiseParam;
  sDetailParam  detailParam;
  sProcAmpParam procampParam;
  sVideoAnalysisParam vaParam;

  // flag describes type of memory
  // true  - frames in video memory (d3d surfaces),  
  // false - in system memory
  bool   bd3dAlloc;

  mfxU32   requestedFramesCount;
  /* ********************** */
  /* input\output streams   */
  /* ********************** */
  TCHAR  strSrcFile[MSDK_MAX_FILENAME_LEN];
  TCHAR  strDstFile[MSDK_MAX_FILENAME_LEN];

  // required implementation of MediaSDK library
  mfxIMPL  impLib;
};

struct sFrameProcessor
{
  MFXVideoSession     mfxSession;
  MFXVideoVPP*        pmfxVPP;

  sFrameProcessor( void ){ pmfxVPP = NULL; return;};
};

struct sMemoryAllocator
{
  MFXFrameAllocator*  pMfxAllocator;
  mfxAllocatorParams* pAllocatorParams;
  bool                bd3dAlloc;
  bool                bUsedAsExternalAllocator;

  mfxFrameSurface1*     pSurfaces[2];
  mfxFrameAllocResponse response[2];

#ifdef D3D_SURFACES_SUPPORT
  IDirect3DDeviceManager9* pd3dDeviceManager;  
#endif
};

class CRawVideoReader
{
public :

  CRawVideoReader();
  ~CRawVideoReader();

  void    Close();
  mfxStatus  Init(const TCHAR *strFileName);
  mfxStatus  LoadNextFrame(mfxFrameData* pData, mfxFrameInfo* pInfo);

private:
  FILE*       m_fSrc;
};

class CRawVideoWriter
{
public :

  CRawVideoWriter();
  ~CRawVideoWriter();

  void       Close();
  mfxStatus  Init(const TCHAR *strFileName);
  mfxStatus  WriteFrame(mfxFrameData* pData, mfxFrameInfo* pInfo);

private:
  FILE*       m_fDst;
};

struct sAppResources
{
  CRawVideoReader*    pSrcFileReader;
  CRawVideoWriter*    pDstFileWriter;

  sFrameProcessor*    pProcessor;
  mfxVideoParam*      pVppParams;
  sMemoryAllocator*   pAllocator;

  /* VPP extension */
  mfxExtVPPDoUse      extDoUse;
  mfxU32              tabDoUseAlg[ENH_FILTERS_COUNT];    
  mfxExtBuffer*       pExtBuf[1+ENH_FILTERS_COUNT];
  mfxExtVppAuxData    extVPPAuxData;

  /* config video enhancement algorithms */
  mfxExtVPPProcAmp    procampConfig;
  mfxExtVPPDetail     detailConfig;
  mfxExtVPPDenoise    denoiseConfig;
};

/* ******************************************************************* */
/*                        service functions                            */
/* ******************************************************************* */

mfxStatus vppParseInputString(TCHAR* strInput[], mfxU8 nArgNum, sInputParams* pParams);

void PrintInfo(sInputParams* pParams, mfxVideoParam* pMfxParams, MFXVideoSession *pMfxSession);

mfxStatus InitParamsVPP(mfxVideoParam* pMFXParams, sInputParams* pInParams);

mfxStatus InitResources(sAppResources* pResources, mfxVideoParam* pParams, mfxIMPL impl);

void WipeResources(sAppResources* pResources);

mfxStatus GetFreeSurface(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize, mfxFrameSurface1** ppSurface);

mfxStatus ConfigVideoEnhancementFilters( sInputParams* pParams, sAppResources* pResources );

#endif /* __SAMPLE_VPP_UTILS_H */
