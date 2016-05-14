/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "bitstream_pool.h"
#include "mfx_filter_defs.h"

using namespace std;

sBitstreamPoolEntry *BitstreamPool::findBitstream(int maxLength)
{
    for (list<sBitstreamPoolEntry>::iterator it = m_Bitstreams.begin(); it != m_Bitstreams.end(); ++it)
    {
        if ( (it->Locked == 0) && (it->pBitstream->pmfxBS->MaxLength == maxLength))
        {            
            return &(*it);
        }
    }
    return NULL;
}

mfxStatus BitstreamPool::newBitstream(sOutputBitstream **bitstream, int maxLength)
{
    mfxStatus sts = MFX_ERR_NONE;

    sBitstreamPoolEntry *bs_entry = findBitstream(maxLength);

    if (!bs_entry)
    {
        if (m_Bitstreams.size() < m_Capacity)
        {
            sOutputBitstream *bs = new sOutputBitstream;
            MSDK_CHECK_POINTER(bs, MFX_ERR_MEMORY_ALLOC);

            bs->pmfxBS = new mfxBitstream;
            MSDK_CHECK_POINTER_SAFE(bs->pmfxBS, MFX_ERR_MEMORY_ALLOC, {delete bs;})        

            memset(bs->pmfxBS, 0, sizeof(mfxBitstream));
            bs->pmfxBS->MaxLength = maxLength;
            bs->pmfxBS->Data = new mfxU8[bs->pmfxBS->MaxLength];
            MSDK_CHECK_POINTER_SAFE(bs->pmfxBS->Data, MFX_ERR_MEMORY_ALLOC, {delete bs->pmfxBS; delete bs;});        

            bs->psyncP = new mfxSyncPoint;
            MSDK_CHECK_POINTER_SAFE(bs->psyncP, MFX_ERR_MEMORY_ALLOC, {delete [] bs->pmfxBS->Data; delete bs->pmfxBS; delete bs; }); 

            sBitstreamPoolEntry bs_entry(bs, 1);
            m_Bitstreams.push_back(bs_entry);   

            *bitstream = bs;
        }
        else
        {
            *bitstream = NULL;
        }
    }    
    else
    {
        bs_entry->Locked = 1;
        *bitstream = bs_entry->pBitstream;
    }
    
    return sts;
}

mfxStatus BitstreamPool::deleteBitstream(sOutputBitstream *bitstream)
{    
    for (list<sBitstreamPoolEntry>::iterator it = m_Bitstreams.begin(); it != m_Bitstreams.end(); ++it)
    {
        if (it->pBitstream == bitstream)
        {
            it->Locked = 0;
            return MFX_ERR_NONE;
        }
    }
    return MFX_ERR_UNDEFINED_BEHAVIOR;
}

mfxStatus BitstreamPool::FreeResources()
{
    for (list<sBitstreamPoolEntry>::iterator it = m_Bitstreams.begin(); it != m_Bitstreams.end(); ++it)
    {
        MSDK_SAFE_DELETE(it->pBitstream->psyncP);
        MSDK_SAFE_DELETE_ARRAY(it->pBitstream->pmfxBS->Data);
        MSDK_SAFE_DELETE(it->pBitstream->pmfxBS);
        MSDK_SAFE_DELETE(it->pBitstream);
    }
    m_Bitstreams.clear();
    return MFX_ERR_NONE;
}

BitstreamPool::BitstreamPool(unsigned capacity) : m_Capacity(capacity)
{
}

mfxStatus BitstreamPool::setCapacity(unsigned capacity)
{
    if (capacity < m_Bitstreams.size())
        return MFX_ERR_UNDEFINED_BEHAVIOR;

    m_Capacity = capacity;
    return MFX_ERR_NONE;
}

unsigned BitstreamPool::getCapacity()
{
    return m_Capacity;
}