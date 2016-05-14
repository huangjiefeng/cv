/* ////////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2005-2011 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __SAMPLE_UTILS_H__
#define __SAMPLE_UTILS_H__

#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "mfxstructures.h"
#include "mfxjpeg.h"

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    void operator=(const TypeName&)   

//TODO: consider switch to base class no_copy
//! Base class for types that should not be assigned.
class no_assign {
    // Deny assignment
    void operator=( const no_assign& );
public:
#if __GNUC__
    //! Explicitly define default construction, because otherwise gcc issues gratuitous warning.
    no_assign() {}
#endif /* __GNUC__ */
};

//! Base class for types that should not be copied or assigned.
class no_copy: no_assign {
    //! Deny copy construction
    no_copy( const no_copy& );
public:
    //! Allow default construction
    no_copy() {}
};


class CSmplYUVReader
{
public :

    CSmplYUVReader();
    virtual ~CSmplYUVReader();

    virtual void Close();
    virtual mfxStatus Init(const TCHAR *strFileName, const mfxU32 ColorFormat, const mfxU32 numViews, std::vector<TCHAR*> srcFileBuff);
    virtual mfxStatus LoadNextFrame(mfxFrameSurface1* pSurface);
    mfxU32 m_ColorFormat; // color format of input YUV data, YUV420 or NV12

    void SetMultiView() { m_bIsMultiView = true; }

protected:
    FILE* m_fSource, **m_fSourceMVC;
    bool m_bInited, m_bIsMultiView;  
    mfxU32 m_numLoadedFiles;
};

class CSmplBitstreamWriter
{
public :

    CSmplBitstreamWriter();
    virtual ~CSmplBitstreamWriter();

    virtual mfxStatus Init(const TCHAR *strFileName);
    virtual mfxStatus WriteNextFrame(mfxBitstream *pMfxBitstream, bool isPrint = true);
    virtual void Close();
    mfxU32 m_nProcessedFramesNum;
    
protected:
    FILE*       m_fSource;
    bool        m_bInited;    
};

class CSmplYUVWriter
{
public :

    CSmplYUVWriter();
    virtual ~CSmplYUVWriter();

    virtual void      Close();
    virtual mfxStatus Init(const TCHAR *strFileName, const mfxU32 numViews);
    virtual mfxStatus WriteNextFrame(mfxFrameSurface1 *pSurface);

    void SetMultiView() { m_bIsMultiView = true; }

protected:
    FILE         *m_fDest, **m_fDestMVC;
    bool         m_bInited, m_bIsMultiView;
    mfxU32       m_numCreatedFiles;
};

class CSmplBitstreamReader
{
public :

    CSmplBitstreamReader();
    virtual ~CSmplBitstreamReader();

    //resets position to file begin
    virtual void      Reset();
    virtual void      Close();
    virtual mfxStatus Init(const TCHAR *strFileName);
    virtual mfxStatus ReadNextFrame(mfxBitstream *pBS);

protected:
    FILE*     m_fSource;
    bool      m_bInited;
};

//provides output bistream with at least 1 slice, reports about error
class CH264FrameReader : public CSmplBitstreamReader
{
public:
    CH264FrameReader();
    virtual mfxStatus ReadNextFrame(mfxBitstream *pBS);
protected:
    //1 - means slice start indicator present
    //2 - means slice start and backend startcode present
    int FindSlice(mfxBitstream *pBS, int & pos2ndnalu);
    

    mfxBitstream m_lastBs;
    std::vector<mfxU8> m_bsBuffer;
};

//provides output bistream with at least 1 frame, reports about error
class CJPEGFrameReader : public CSmplBitstreamReader
{
public:
    virtual mfxStatus ReadNextFrame(mfxBitstream *pBS);
protected:
    //1 - means SOI marker present
    //2 - means 2 SOI markers present
    int FindSOImarkers(mfxBitstream *pBS);
};

//timeinterval calculation helper
#ifdef WIN32
    #include "windows.h"
#endif

template <int tag = 0>
class CTimeInterval : private no_copy
{
    static double g_Freq;
    double       &m_start;
    double        m_own;//reference to this if external counter not required
    //since QPC functions are quite slow it make sense to optionally enable them
    bool         m_bEnable;
#ifdef  WIN32
    LARGE_INTEGER m_liStart;
#endif

public:
    CTimeInterval(double &dRef , bool bEnable = true)
        : m_start(dRef)
        , m_bEnable(bEnable)
    {
        if (!m_bEnable)
            return;
        Initialize();
    }
    CTimeInterval(bool bEnable = true)
        : m_start(m_own)
        , m_bEnable(bEnable)
        , m_own()
    {
        if (!m_bEnable)
            return;
        Initialize();
    }

    //updates external value with current time
    double Commit()
    {
        if (!m_bEnable)
            return 0.0;

        if (0.0 != g_Freq)
        {
#ifdef  WIN32
            LARGE_INTEGER liEnd;
            QueryPerformanceCounter(&liEnd);
            m_start = ((double)liEnd.QuadPart - (double)m_liStart.QuadPart)  / g_Freq;
#endif
        }
        return m_start;
    }
    //lastcomitted value
    double Last()
    {
        return m_start;
    }
    ~CTimeInterval()
    {
        Commit();
    }
private:
    void Initialize()
    {
#ifdef  WIN32
        if (0.0 == g_Freq)
        {
            QueryPerformanceFrequency(&m_liStart);
            g_Freq = (double)m_liStart.QuadPart;
        }
        QueryPerformanceCounter(&m_liStart);
#endif
    }
};

template <int tag>double CTimeInterval<tag>::g_Freq = 0.0f;


mfxStatus ConvertFrameRate(mfxF64 dFrameRate, mfxU32* pnFrameRateExtN, mfxU32* pnFrameRateExtD);
mfxF64 CalculateFrameRate(mfxU32 nFrameRateExtN, mfxU32 nFrameRateExtD);
mfxU16 GetFreeSurfaceIndex(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize);
mfxU16 GetFreeSurfaceIndex(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize, mfxU16 step);
mfxU16 GetFreeSurface(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize);
mfxStatus InitMfxBitstream(mfxBitstream* pBitstream, mfxU32 nSize);
//performs copy to end if possible also move data to buffer begin if necessary
//shifts offset pointer in source bitstream in success case
mfxStatus MoveMfxBitstream(mfxBitstream *pTarget, mfxBitstream *pSrc, mfxU32 nBytesToCopy);
mfxStatus ExtendMfxBitstream(mfxBitstream* pBitstream, mfxU32 nSize);
void WipeMfxBitstream(mfxBitstream* pBitstream);
TCHAR* CodecIdToStr(mfxU32 nFourCC);
mfxU16 CalculateDefaultBitrate(mfxU32 nCodecId, mfxU32 nTargetUsage, mfxU32 nWidth, mfxU32 nHeight, mfxF64 dFrameRate);
mfxU16 StrToTargetUsage(TCHAR* strInput);
TCHAR* TargetUsageToStr(mfxU16 tu);
TCHAR* ColorFormatToStr(mfxU32 format);
TCHAR* MfxStatusToStr(mfxStatus sts);

// sets bitstream->PicStruct parsing first APP0 marker in bitstream
mfxStatus MJPEG_AVI_ParsePicStruct(mfxBitstream *bitstream);

// For MVC encoding/decoding purposes
std::basic_string<TCHAR> FormMVCFileName(const TCHAR *strFileName, const mfxU32 numView);

//piecewise linear function for bitrate approximation
class PartiallyLinearFNC
{
    mfxF64 *m_pX;
    mfxF64 *m_pY;
    mfxU32  m_nPoints;
    mfxU32  m_nAllocated;

public:
    PartiallyLinearFNC();
    ~PartiallyLinearFNC();

    void AddPair(mfxF64 x, mfxF64 y);
    mfxF64 at(mfxF64);
private:
    DISALLOW_COPY_AND_ASSIGN(PartiallyLinearFNC);
};

// function for conversion of display aspect ratio to pixel aspect ratio
mfxStatus DARtoPAR(mfxU32 darw, mfxU32 darh, mfxU32 w, mfxU32 h, mfxU16 *pparw, mfxU16 *pparh);

// function for getting a pointer to a specific external buffer from the array
mfxExtBuffer* GetExtBuffer(mfxExtBuffer** ebuffers, mfxU32 nbuffers, mfxU32 BufferId);

//declare used extended buffers
template<class T>
struct mfx_ext_buffer_id{
};
template<>struct mfx_ext_buffer_id<mfxExtCodingOption>{
    enum {id = MFX_EXTBUFF_CODING_OPTION};
};
template<>struct mfx_ext_buffer_id<mfxExtAvcTemporalLayers>{
    enum {id = MFX_EXTBUFF_AVC_TEMPORAL_LAYERS};
};
template<>struct mfx_ext_buffer_id<mfxExtAVCRefListCtrl>{
    enum {id = MFX_EXTBUFF_AVC_REFLIST_CTRL};
};

//helper function to initialize mfx ext buffer structure
template <class T>
void init_ext_buffer(T & ext_buffer)
{
    memset(&ext_buffer, 0, sizeof(ext_buffer));
    reinterpret_cast<mfxExtBuffer*>(&ext_buffer)->BufferId = mfx_ext_buffer_id<T>::id;
    reinterpret_cast<mfxExtBuffer*>(&ext_buffer)->BufferSz = sizeof(ext_buffer);
}
 
// returns false if buf length is insufficient, otherwise 
// skips step bytes in buf with specified length and returns true
template <typename Buf_t, typename Length_t>
bool skip(const Buf_t *&buf, Length_t &length, Length_t step)
{
    if (length < step)
        return false;

    buf    += step;
    length -= step;

    return true;
}

#endif //__SAMPLE_UTILS_H__