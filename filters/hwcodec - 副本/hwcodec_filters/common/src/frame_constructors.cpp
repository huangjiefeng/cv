/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except mtIn accordance with the terms of that agreement.
//          Copyright(c) 2003-2011 Intel Corporation. All Rights Reserved.
//
*/

#include "sample_defs.h"
#include "frame_constructors.h"
#include <vector>

#define CHECK_POINTER_SET_STS(P, STS) {if (!(P)) {STS = MFX_ERR_NULL_PTR;}}

static inline mfxU32 GetValue16(mfxU8* pBuf)
{
    MSDK_CHECK_POINTER(pBuf, 0);
    return ((*pBuf) << 8) | (*(pBuf + 1));
};

static inline mfxU32 GetValue24(mfxU8* pBuf)
{
    MSDK_CHECK_POINTER(pBuf, 0);
    return ((*pBuf) << 16) | (*(pBuf + 1) << 8) | (*(pBuf + 2));
};

static inline mfxU32 GetValue32(mfxU8* pBuf)
{
    MSDK_CHECK_POINTER(pBuf, 0);
    return ((*pBuf) << 24) | (*(pBuf + 1) << 16) | (*(pBuf + 2) << 8) | (*(pBuf + 3));
};

static inline mfxU32 GetLength(mfxU32 nBytesCount, mfxU8* pBuf)
{    
    mfxU32 nLenght = 0;
    MSDK_CHECK_POINTER(pBuf, 0);

    switch (nBytesCount)
    {
        case 1: nLenght = *pBuf;            break;
        case 2: nLenght = GetValue16(pBuf); break;
        case 3: nLenght = GetValue24(pBuf); break;
        case 4: nLenght = GetValue32(pBuf); break;
    }

    return nLenght;
}

static inline void SetValue(mfxU32 nValue, mfxU8* pBuf)
{
    MSDK_CHECK_POINTER_NO_RET(pBuf);

    for (mfxU32 i = 0; i < 4; i++)
    {
        *pBuf = (mfxU8)(nValue >> 8 * i);
        pBuf++;
    }

    return;
}

mfxU32 MFX_TIME_STAMP_FREQUENCY = 90000; 
mfxU64 MFX_TIME_STAMP_INVALID = (mfxU64)-1; 

mfxU64 ConvertReferenceTime2MFXTime(REFERENCE_TIME rtTime)
{
    mfxU64 nTime = 0;

    if (-1e7 == rtTime)
    {
        nTime = MFX_TIME_STAMP_INVALID;
    }
    else
    {
        nTime = (mfxU64)((rtTime / 1e7) * MFX_TIME_STAMP_FREQUENCY);
    }

    return nTime;
};

CFrameConstructor::CFrameConstructor() 
{
    memset(&m_mfxResidualBS, 0, sizeof(mfxBitstream)); 

    m_mfxResidualBS.Data        = new mfxU8[100];
    m_mfxResidualBS.MaxLength   = 100;    
};

CFrameConstructor::~CFrameConstructor() 
{
    delete [] m_mfxResidualBS.Data;
};

void CFrameConstructor::SaveResidialData(mfxBitstream* pBS)
{
    MSDK_CHECK_POINTER_NO_RET(pBS);

    if (pBS > 0 && pBS->DataLength <= m_mfxResidualBS.MaxLength)
    {
        memcpy(m_mfxResidualBS.Data, pBS->Data + pBS->DataOffset, pBS->DataLength);

        m_mfxResidualBS.DataLength = pBS->DataLength;
    }
};

mfxStatus CFrameConstructor::ConstructFrame(IMediaSample* pSample, mfxBitstream *pBS)
{
    mfxStatus sts = MFX_ERR_NONE;
    mfxU8*          pDataBuffer = NULL;
    mfxU32          nDataSize   = 0;
    REFERENCE_TIME  rtStart(0), rtEnd(0);

    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR);

    nDataSize = pSample->GetActualDataLength();

    if (0 == nDataSize)
    {
        sts = MFX_ERR_MORE_DATA;
    }

    if (MFX_ERR_NONE == sts)
    {
        pSample->GetPointer(&pDataBuffer);
        CHECK_POINTER_SET_STS(pDataBuffer, sts);
    }

    if (MFX_ERR_NONE == sts)
    {
        MSDK_SAFE_DELETE_ARRAY(pBS->Data);

        pBS->MaxLength = 
            pBS->DataLength = nDataSize + m_mfxResidualBS.DataLength;

        pBS->Data = new mfxU8[pBS->DataLength];

        CHECK_POINTER_SET_STS(pBS->Data, sts);
    }

    if (MFX_ERR_NONE == sts)
    {
        memcpy(pBS->Data, m_mfxResidualBS.Data, m_mfxResidualBS.DataLength);
        memcpy(pBS->Data + m_mfxResidualBS.DataLength, pDataBuffer, nDataSize);

        m_mfxResidualBS.DataLength = 0;
    }

    if (MFX_ERR_NONE == sts)
    {            
        if (SUCCEEDED(pSample->GetTime(&rtStart, &rtEnd)))
        {
            if (-1e7 != rtStart || -1e7 != rtEnd)
            {
                rtStart = (rtStart < 0) ? 0 : rtStart;
                rtEnd   = (rtEnd < 0)   ? 0 : rtEnd;
            }

            pBS->TimeStamp = ConvertReferenceTime2MFXTime(rtStart);
        }
        else
        {
            pBS->TimeStamp = MFX_TIME_STAMP_INVALID;         
        }        
    }

    return sts;
}

CVC1FrameConstructor::CVC1FrameConstructor(mfxVideoParam*  pVideoParam) : CFrameConstructor(),
m_pVideoParam(pVideoParam)
{
};

mfxStatus CVC1FrameConstructor::ConstructFrame(IMediaSample* pSample, mfxBitstream *pBS)
{
    mfxStatus       sts         = MFX_ERR_NONE;
    mfxU8*          pDataBuffer = NULL;
    mfxU32          nDataSize   = 0;
    REFERENCE_TIME  rtStart(0), rtEnd(0);

    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR);

    //if need to init SM profiles only
    if (NULL == pBS)
    {
        if (MFX_PROFILE_VC1_ADVANCED != m_pVideoParam->mfx.CodecProfile)
        {
            sts = ConstructSequenceHeaderSM(pSample);
        }
    }
    else
    {
        nDataSize = pSample->GetActualDataLength();
        
        if (0 == nDataSize)
        {
            sts = MFX_ERR_MORE_DATA;
        }
        
        if (MFX_ERR_NONE == sts)
        {
            pSample->GetPointer(&pDataBuffer);
            CHECK_POINTER_SET_STS(pDataBuffer, sts);
        }
        
        if (MFX_ERR_NONE == sts)
        {
            MSDK_SAFE_DELETE_ARRAY(pBS->Data);

            pBS->MaxLength = 
                pBS->DataLength = nDataSize + 4 + m_mfxResidualBS.DataLength;

            if (MFX_PROFILE_VC1_ADVANCED != m_pVideoParam->mfx.CodecProfile)
            {
                pBS->MaxLength = 
                    pBS->DataLength = pBS->DataLength + 4;
            }

            pBS->Data = new mfxU8[pBS->DataLength];
            CHECK_POINTER_SET_STS(pBS->Data, sts);
        }

        if (MFX_ERR_NONE == sts)
        {            
            pSample->GetTime(&rtStart, &rtEnd);    
            
            pBS->TimeStamp = ConvertReferenceTime2MFXTime(rtStart);
        }

        if (MFX_ERR_NONE == sts)
        {
            if (m_mfxResidualBS.DataLength)
            {
                memcpy(pBS->Data, m_mfxResidualBS.Data, m_mfxResidualBS.DataLength);
            }

            if (MFX_PROFILE_VC1_ADVANCED != m_pVideoParam->mfx.CodecProfile)
            {
                SetValue(nDataSize, pBS->Data + m_mfxResidualBS.DataLength);     
                SetValue(0, pBS->Data + 4 + m_mfxResidualBS.DataLength);
                memcpy(pBS->Data + 8 + m_mfxResidualBS.DataLength, pDataBuffer, nDataSize);                
            }
            else 
            {
                if (false == StartCodeExist(pDataBuffer))
                {
                    // set start code to first 4 bytes
                    SetValue(0x0D010000, pBS->Data + m_mfxResidualBS.DataLength);

                    memcpy(pBS->Data + 4 + m_mfxResidualBS.DataLength, pDataBuffer, nDataSize);
                }
                else
                {
                    memcpy(pBS->Data + m_mfxResidualBS.DataLength, pDataBuffer, nDataSize);
                }
            }

            m_mfxResidualBS.DataLength = 0;
        }
    }    

    return sts;
};
mfxStatus CVC1FrameConstructor::ConstructSequenceHeaderSM(IMediaSample *pSample)
{
    mfxStatus   sts         = MFX_ERR_NONE;
    mfxU8*      pTempBuffer = NULL;
    
    mfxU8*      pDataBuffer = NULL;
    mfxU32      nDataSize   = 0;

    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR);

    nDataSize = pSample->GetActualDataLength();

    if (0 == nDataSize)
    {
        sts = MFX_ERR_MORE_DATA;
    }

    if (pSample->GetActualDataLength() + 20 > pSample->GetSize())
    {
        sts = MFX_ERR_MORE_DATA;
    }

    if (MFX_ERR_NONE == sts)
    {
        pSample->GetPointer(&pDataBuffer);
        CHECK_POINTER_SET_STS(pDataBuffer, sts);
    }

    if (MFX_ERR_NONE == sts)
    {
        pTempBuffer = new mfxU8[nDataSize];
        CHECK_POINTER_SET_STS(pTempBuffer, sts);
    }

    if (MFX_ERR_NONE == sts)
    {
        // save input data
        memcpy(pTempBuffer, pDataBuffer, nDataSize);

        ConstructHeaderSM(pDataBuffer, nDataSize + 20, pTempBuffer, nDataSize);

        // set resulted size
        pSample->SetActualDataLength(nDataSize + 20);
    }

    MSDK_SAFE_DELETE_ARRAY(pTempBuffer);

    return sts;
}

mfxStatus CVC1FrameConstructor::ConstructHeaderSM(mfxU8* pHeaderSM, mfxU32 nHeaderSize, mfxU8* pDataBuffer, mfxU32 nDataSize)
{
    MSDK_CHECK_POINTER(pHeaderSM, MFX_ERR_NULL_PTR);
    MSDK_CHECK_POINTER(pDataBuffer, MFX_ERR_NULL_PTR);
    if (nHeaderSize < nDataSize + 20)
    {
        return MFX_ERR_NOT_ENOUGH_BUFFER;
    }

    // set start code
    SetValue(0xC5000000, pHeaderSM);

    // set size of sequence header is 4 bytes
    SetValue(nDataSize, pHeaderSM + 4);

    // copy saved data back
    memcpy(pHeaderSM + 8, pDataBuffer, nDataSize);

    // set sizes to the end of data
    SetValue(m_pVideoParam->mfx.FrameInfo.CropH, pHeaderSM + 8  + nDataSize);
    SetValue(m_pVideoParam->mfx.FrameInfo.CropW,  pHeaderSM + 12 + nDataSize);

    //set 0 to the last 4 bytes
    SetValue(0,  pHeaderSM + 16 + nDataSize);

    return MFX_ERR_NONE;
};

bool CVC1FrameConstructor::StartCodeExist(mfxU8* pStart)
{
    MSDK_CHECK_POINTER(pStart, false);

    //check first 4 bytes to be start code
    switch (GetLength(4, pStart))
    {
        case 0x010D:
        case 0x010F:
        case 0x010A:
        case 0x010E:
        case 0x010C:
        case 0x011B:
        case 0x011C:
        case 0x011D:
        case 0x011E:
        case 0x011F:
            // start code found
            return true;
        default:
            // start code not found
            return false; 
    }
}

CAVCFrameConstructor::CAVCFrameConstructor() : CFrameConstructor(),
m_bInsertHeaders(true)
{
    m_HeaderNalSize  = 2;  //MSDN - MPEG2VideoInfo->dwSequenceHeader delimited by 2 byte length fields
    
    memset(&m_StartCodeBS, 0, sizeof(mfxBitstream));

    m_StartCodeBS.Data = new mfxU8[4];
    
    m_StartCodeBS.DataLength =
        m_StartCodeBS.MaxLength = 4;

    SetValue(0x01000000, m_StartCodeBS.Data);

    memset(&m_Headers, 0, sizeof(mfxBitstream));
}

CAVCFrameConstructor::~CAVCFrameConstructor()
{
    MSDK_SAFE_DELETE_ARRAY(m_StartCodeBS.Data);
    MSDK_SAFE_DELETE_ARRAY(m_Headers.Data);
}

mfxStatus CAVCFrameConstructor::ReadAVCHeader( MPEG2VIDEOINFO *pMPEG2VidInfo,   mfxBitstream  *pBS )
{    
    mfxStatus sts = MFX_ERR_NONE;    

    std::vector<mfxU8>      tempBuffer;
    mfxU32 nNalDataLen;          
    mfxU8* pNalDataBuff; 
    StartCodeIteratorMP4    m_pStartCodeIter;

    ZeroMemory(&m_Headers, sizeof(mfxBitstream));                     
            
    m_NalSize = pMPEG2VidInfo->dwFlags;     
        
    m_pStartCodeIter.Init((BYTE *)pMPEG2VidInfo->dwSequenceHeader, pMPEG2VidInfo->cbSequenceHeader, m_HeaderNalSize ); //Nal size = 2 
    while (m_pStartCodeIter.ReadNext())
    {    
        nNalDataLen = m_pStartCodeIter.GetDataLength(); 
        pNalDataBuff = m_pStartCodeIter.GetDataBuffer();

        switch(m_pStartCodeIter.GetType())
        {
            case NALU_TYPE_SPS:
            case NALU_TYPE_PPS:
                tempBuffer.insert(tempBuffer.end(), m_StartCodeBS.Data, m_StartCodeBS.Data + 4);
                tempBuffer.insert(tempBuffer.end(), pNalDataBuff, pNalDataBuff+nNalDataLen);
                break; 
            default: 
                sts = MFX_ERR_MORE_DATA; 
                break;
        }
    }
    if (tempBuffer.size())
    {
        // allocate output bitstream and copy data
        pBS->Data = new mfxU8[tempBuffer.size()];
        pBS->DataLength = pBS->MaxLength = (mfxU32)tempBuffer.size();
        memcpy(pBS->Data, &tempBuffer.front(), tempBuffer.size());

        //Keep a copy of the SPS/PPS to place put into the  decode stream. 
        MSDK_SAFE_DELETE_ARRAY(m_Headers.Data);
        m_Headers.Data = new mfxU8[tempBuffer.size()];
        m_Headers.DataLength = m_Headers.MaxLength = (mfxU32)tempBuffer.size(); 
        memcpy(m_Headers.Data, &tempBuffer.front(), tempBuffer.size()); 

        tempBuffer.clear();
    }

    return sts; 
}

mfxStatus CAVCFrameConstructor::ConstructFrame(IMediaSample *pSample, mfxBitstream *pBS)
{
    mfxStatus sts = MFX_ERR_NONE; 
    mfxU32 nDataSize = 0; 
    mfxU8*                  pDataBuffer = NULL;    
    std::vector<mfxU8>      tempBuffer;
    REFERENCE_TIME          rtStart(0), rtEnd(0);
    StartCodeIteratorMP4    m_pStartCodeIter;
    mfxU32 nNalDataLen; 
    mfxU8* pNalDataBuff; 
    
    MSDK_CHECK_POINTER(pSample, MFX_ERR_NULL_PTR); 
    MSDK_CHECK_POINTER(pBS, MFX_ERR_NULL_PTR); 

    nDataSize = pSample->GetActualDataLength(); 
    if( 0 == nDataSize)
    {
        sts = MFX_ERR_MORE_DATA; 
    }
    
    if (MFX_ERR_NONE == sts)
    {
        pSample->GetPointer(&pDataBuffer);
        CHECK_POINTER_SET_STS(pDataBuffer, sts);
    }

    if (MFX_ERR_NONE == sts)
    {             
        m_pStartCodeIter.Init(pDataBuffer, nDataSize, m_NalSize); //Nal size = 4 
        while (m_pStartCodeIter.ReadNext())
        {    
            switch(m_pStartCodeIter.GetType())
            {
            case     NALU_TYPE_AUD:
                break;
            default:
                nNalDataLen =  m_pStartCodeIter.GetDataLength(); 
                pNalDataBuff = m_pStartCodeIter.GetDataBuffer();
                tempBuffer.insert(tempBuffer.end(), m_StartCodeBS.Data, m_StartCodeBS.Data + 4);
                tempBuffer.insert(tempBuffer.end(), pNalDataBuff, pNalDataBuff + nNalDataLen);
                break;
            }
        }

        if (tempBuffer.size())
        {
            if(m_bInsertHeaders) 
            {
                pBS->Data = new mfxU8[tempBuffer.size() + m_mfxResidualBS.DataLength + m_Headers.DataLength];
                pBS->DataLength = pBS->MaxLength = (mfxU32)tempBuffer.size() + m_mfxResidualBS.DataLength + m_Headers.DataLength;

                // copy saved headers
                memcpy(pBS->Data, m_Headers.Data, m_Headers.DataLength); 
                // copy any residual data
                if (m_mfxResidualBS.DataLength)
                {
                    memcpy(pBS->Data + m_Headers.DataLength, m_mfxResidualBS.Data, m_mfxResidualBS.DataLength);
                }                
                // copy current data portion
                memcpy(pBS->Data + m_Headers.DataLength + m_mfxResidualBS.DataLength, 
                    &tempBuffer.front(), tempBuffer.size());                
            }
            else 
            {
                pBS->Data = new mfxU8[tempBuffer.size() + m_mfxResidualBS.DataLength];
                pBS->DataLength = pBS->MaxLength = (mfxU32)tempBuffer.size() + m_mfxResidualBS.DataLength;
                
                // copy any residual data
                if (m_mfxResidualBS.DataLength)
                {
                    memcpy(pBS->Data, m_mfxResidualBS.Data, m_mfxResidualBS.DataLength);
                }     
                // copy current data portion
                memcpy(pBS->Data + m_mfxResidualBS.DataLength, &tempBuffer.front(), tempBuffer.size());
            }                  
                        
            tempBuffer.clear();            
        }

        if (MFX_ERR_NONE == sts)
        {            
            pSample->GetTime(&rtStart, &rtEnd);      
            if(rtStart < 0 )
            {
                rtStart = 0; 

            }
            pBS->TimeStamp = ConvertReferenceTime2MFXTime(rtStart);        
        }

    }
    return sts; 
}
