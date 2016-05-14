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

#include <list>
#include "mfxvideo++.h"
#include "sample_utils.h"

struct sOutputBitstream
{
    mfxBitstream*       pmfxBS;
    mfxSyncPoint*       psyncP;
};

struct sBitstreamPoolEntry
{
    sOutputBitstream *pBitstream;
    int Locked;

    sBitstreamPoolEntry(sOutputBitstream *bitstream = NULL, int locked = 0) 
        : pBitstream(bitstream)
        , Locked(locked)
    {
    };
};

class BitstreamPool
{
private:
    std::list<sBitstreamPoolEntry> m_Bitstreams;
    unsigned m_Capacity;

    sBitstreamPoolEntry *findBitstream(int maxLength);
public:
    BitstreamPool(unsigned capacity = 15);
    virtual ~BitstreamPool() { FreeResources(); }

    mfxStatus newBitstream(sOutputBitstream **bitstream, int maxLength);
    mfxStatus deleteBitstream(sOutputBitstream *bitstream);

    mfxStatus setCapacity(unsigned capacity);
    unsigned getCapacity();

    mfxStatus FreeResources();
private:
    DISALLOW_COPY_AND_ASSIGN(BitstreamPool);
};