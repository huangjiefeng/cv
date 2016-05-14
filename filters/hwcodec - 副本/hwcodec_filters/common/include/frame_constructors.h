/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except mtIn accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "windows.h"
#include "mfxvideo.h"
#include "mfxvideo++.h"

#include <time.h>
#include <iostream>
#include <dshow.h>
#include <dvdmedia.h>

class CFrameConstructor
{
public:
    CFrameConstructor();
    virtual ~CFrameConstructor();

    virtual mfxStatus ConstructFrame(IMediaSample* pSample, mfxBitstream* pBS);
    virtual void      Reset() {};
    void              SaveResidialData(mfxBitstream* pBS);

protected:

    mfxBitstream      m_mfxResidualBS;
};

class CVC1FrameConstructor : public CFrameConstructor
{
public:    
    CVC1FrameConstructor(mfxVideoParam* pVideoParam);
    
    mfxStatus       ConstructFrame              (IMediaSample* pSample, mfxBitstream* pBS);

    mfxStatus       ConstructHeaderSM           (mfxU8* pHeaderSM, mfxU32 nHeaderSize, mfxU8* pDataBuffer, mfxU32 nDataSize);

protected:
    
    mfxStatus       ConstructSequenceHeaderSM   (IMediaSample* pSample);
    bool            StartCodeExist              (mfxU8* pStart);
    
protected:
    
    mfxVideoParam*  m_pVideoParam;
};

//For AVC1 support

typedef enum
{
    NALU_TYPE_SLICE    = 1,
    NALU_TYPE_DPA      = 2,
    NALU_TYPE_DPB      = 3,
    NALU_TYPE_DPC      = 4,
    NALU_TYPE_IDR      = 5,
    NALU_TYPE_SEI      = 6,
    NALU_TYPE_SPS      = 7,
    NALU_TYPE_PPS      = 8,
    NALU_TYPE_AUD      = 9,
    NALU_TYPE_EOSEQ    = 10,
    NALU_TYPE_EOSTREAM = 11,
    NALU_TYPE_FILL     = 12
} NALU_TYPE;


class CAVCFrameConstructor : public CFrameConstructor
{
public:
    CAVCFrameConstructor();
    ~CAVCFrameConstructor();

    virtual mfxStatus ConstructFrame(IMediaSample* pSample, mfxBitstream *pBS);
    
    mfxStatus ReadAVCHeader( MPEG2VIDEOINFO *pMPEG2VidInfo,  mfxBitstream  *pBS ); 

    virtual void Reset() { m_bInsertHeaders = true; };

private:
    bool         m_bInsertHeaders;
    mfxU32       m_NalSize; 
    mfxU32       m_HeaderNalSize; 

    mfxBitstream m_Headers; 
    mfxBitstream m_StartCodeBS;
};

class StartCodeIteratorMP4
{
public:

    StartCodeIteratorMP4() :m_nCurPos(0), m_nNALStartPos(0), m_nNALDataPos(0), m_nNextRTP(0){};
    

    void Init(BYTE * pBuffer, int nSize, int nNALSize)
    {    
         m_pBuffer = pBuffer; 
         m_nSize = nSize; 
         m_nNALSize = nNALSize; 
        
    }

    NALU_TYPE    GetType()        const { return m_nal_type; };
    int          GetDataLength()    const { return m_nCurPos - m_nNALDataPos; };
    BYTE*        GetDataBuffer() { return m_pBuffer + m_nNALDataPos; };

    bool ReadNext()
    {
        int nTemp;
        if (m_nCurPos >= m_nSize) return false;
        if ((m_nNALSize != 0) && (m_nCurPos == m_nNextRTP))
        {
            
            m_nNALStartPos   = m_nCurPos;
            m_nNALDataPos    = m_nCurPos + m_nNALSize;
            nTemp            = 0;
            for (mfxU32 i=0; i<m_nNALSize; i++)
            {
                nTemp = (nTemp << 8) + m_pBuffer[m_nCurPos++];
            }
            m_nNextRTP += nTemp + m_nNALSize;
            MoveToNextStartcode();
        }
        else
        {

            while (m_pBuffer[m_nCurPos]==0x00 && ((*((DWORD*)(m_pBuffer+m_nCurPos)) & 0x00FFFFFF) != 0x00010000))
                m_nCurPos++;

            m_nNALStartPos    = m_nCurPos;
            m_nCurPos       += 3;
            m_nNALDataPos    = m_nCurPos;
            MoveToNextStartcode();
        }        
        m_nal_type    = (NALU_TYPE) (m_pBuffer[m_nNALDataPos] & 0x1f);

        return true;
    }

private:
    mfxU32        m_nNALStartPos; 
    mfxU32        m_nNALDataPos; 
    mfxU32        m_nDatalen; 
    mfxU32        m_nCurPos;
    mfxU32        m_nNextRTP; 
    mfxU32        m_nSize; 
    mfxU32        m_nNALSize; 
    NALU_TYPE    m_nal_type; 
    BYTE *        m_pBuffer; 

    bool MoveToNextStartcode()
    {
        int        nBuffEnd = (m_nNextRTP > 0) ? min (m_nNextRTP, m_nSize-4) : m_nSize-4;

        for (int i=m_nCurPos; i<nBuffEnd; i++)
        {
            if ((*((DWORD*)(m_pBuffer+i)) & 0x00FFFFFF) == 0x00010000)
            {
            // Find next AnnexB Nal
            m_nCurPos = i;
            return true;
            }
        }

        if ((m_nNALSize != 0) && (m_nNextRTP < m_nSize))
        {
            m_nCurPos = m_nNextRTP;
            return true;
        }

        m_nCurPos = m_nSize;
        return false;
    }
};
