//* ////////////////////////////////////////////////////////////////////////////// */
//*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
//
//
//*/

#pragma once

#include "mfxvideo.h"
#include "mfxvideo++.h"
#include "memory.h"

#include "strmif.h"

#include "sample_utils.h"

class CTimeManager
{
public:

      CTimeManager() : m_dDecodeTime(0), m_dFrameRate(24) {};

      void Init(DOUBLE dFrameRate)
      {
          m_dFrameRate = dFrameRate;
      };

      DOUBLE GetTime()
      {
          return m_dDecodeTime;
      }

      DOUBLE GetFrameRate()
      {
          return m_dFrameRate;
      };

      void SetTime(REFERENCE_TIME rtStart)
      {
          m_dDecodeTime = (DOUBLE)rtStart / 1e7;
      }

private:
    DOUBLE      m_dDecodeTime;
    DOUBLE      m_dFrameRate;
};
