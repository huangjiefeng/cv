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

#include <afx.h>
#include "mfx_filter_guid.h"
#include "mfx_video_enc_filter_utils.h"

#include "base_encoder.h"

#include "mfx_video_enc_proppage.h"
#include "wmsdkidl.h"

#include "mfxvideo.h"
#include "mfxvideo++.h"

#include "bs_adapter.h"
#include "mfx_filter_externals.h"

#define FILTER_MERIT MERIT_DO_NOT_USE

class CEncoderOutputPin : public CTransformOutputPin
{
public:
    CEncoderOutputPin(CTransformFilter* pTransformFilter, HRESULT* phr):
    CTransformOutputPin(NAME("OutputPin"), pTransformFilter, phr, L"Out"){};

    DECLARE_IUNKNOWN;    
};

class CEncVideoFilter : public CTransformFilter, public ISpecifyPropertyPages, public IAboutProperty,
                        public IConfigureVideoEncoder, public IVideoEncoderProperty, INotifier,
                        public IExposeMfxMemTypeFlags, public IShareMfxSession,public IHWVideoEncoder //Patrick
{  
public:
    DECLARE_IUNKNOWN;

    // Default constructor
    CEncVideoFilter(TCHAR* tszName, LPUNKNOWN punk, const GUID VIDEOCodecGUID, HRESULT* phr,
        mfxU16 APIVerMinor = 1, mfxU16 APIVerMajor = 1);
    // Destructor
    ~CEncVideoFilter(void);

    // Overridden pure virtual functions from CTransformFilter base class

    // Notify that no more data passed through graph
    HRESULT             EndOfStream(void);
    // Receive new media portion
    HRESULT             Receive(IMediaSample* pSample);
    // Get new frame
    HRESULT             GetFrame(void);
    // Start data streaming through graph
    HRESULT             StartStreaming(void);
    // Stop data streaming
    HRESULT             StopStreaming(void);
    // Check compatibility of connected pin(s)
    HRESULT             CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut) {return S_OK;};
    // Decide buffer size
    HRESULT             DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pProperties);
    // Get connection media type
    HRESULT             GetMediaType(int iPosition, CMediaType* pMediaType);
    // These must be supplied in a derived class
    HRESULT             CheckInputType(const CMediaType* mtIn);
    // break connection
    HRESULT             BreakConnect(PIN_DIRECTION dir);
    // override this to say what interfaces we support where
    STDMETHODIMP        NonDelegatingQueryInterface(REFIID riid, void** ppv);

    HRESULT             NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);

    CBasePin*           GetPin(int n);

    //IAboutProperty
    STDMETHODIMP        GetFilterName(BSTR* pName) { *pName = m_bstrFilterName; return S_OK; };

    HRESULT             CompleteConnect(PIN_DIRECTION direction, IPin *pReceivePin);   

    HRESULT             GetRequiredFramesNum(mfxU16* nMinFrames, mfxU16* nRecommendedFrames);

    //IConfigureVideoEncoder custom interface
    STDMETHODIMP        GetRunTimeStatistics(Statistics *statistics);
    STDMETHODIMP        SetParams(Params *params);
    STDMETHODIMP        GetParams(Params *params);

    //IExposeMfxMemTypeFlags custom interface
    STDMETHODIMP        GetMfxMemTypeFlags(DWORD *pFlags);

    //IShareMfxSession custom interface
    STDMETHODIMP        GetMfxSession(void **pSession);
    STDMETHODIMP        SygnalSessionHasChild() { if (m_pEncoder) {m_pEncoder->m_bSessionHasChild = true;} return S_OK; };

			//停止发送视频流或者恢复 Patrick
	STDMETHODIMP        setEncoderState( bool work );


protected:

    virtual void            ReadFrameRateFromRegistry(mfxU32 &iFrameRate) = 0;
    virtual void            WriteParamsToRegistry() = 0;
    virtual void            WriteMfxImplToRegistry();
    virtual void            UpdateRegistry() 
    {
        mfxStatus sts;

        mfxVideoParam mfxParam;  
        mfxEncodeStat mfxStat;
        mfxU32        nBitrate;

        memset(&mfxParam, 0, sizeof(mfxVideoParam));
        memset(&mfxStat,  0, sizeof(mfxEncodeStat));

        sts = m_pEncoder->GetVideoParams(&mfxParam);

        if (MFX_ERR_NONE == sts)
        {
            SetParamToReg(_T("Profile"), mfxParam.mfx.CodecProfile);
            SetParamToReg(_T("Level"),   mfxParam.mfx.CodecLevel);
        }       

        sts = m_pEncoder->GetEncodeStat(&mfxStat);        
                
        if (MFX_ERR_NONE == sts && mfxStat.NumFrame)
        {
            nBitrate = (mfxU32)(mfxStat.NumBit / mfxStat.NumFrame * m_pTimeManager->GetFrameRate());

            SetParamToReg(_T("RealBitrate"),   nBitrate);
            SetParamToReg(_T("FramesEncoded"), mfxStat.NumFrame);
        }
    };

    bool UseCSCPlugin()
    {
        GUID guidFormat = *m_pInput->CurrentMediaType().FormatType();
        BITMAPINFOHEADER bmiHeader; 
        VIDEOINFOHEADER2 *pVIH2(NULL);
        VIDEOINFOHEADER *pVIH(NULL);

        bmiHeader.biHeight = 0;
        bmiHeader.biCompression = 0;

        if (FORMAT_VideoInfo2 == guidFormat)
        {
            pVIH2 = reinterpret_cast<VIDEOINFOHEADER2* >(m_pInput->CurrentMediaType().Format());            
            bmiHeader = pVIH2->bmiHeader;
        }
        else if (FORMAT_VideoInfo == guidFormat)
        {
            pVIH = reinterpret_cast<VIDEOINFOHEADER* >(m_pInput->CurrentMediaType().Format());            
            bmiHeader = pVIH->bmiHeader;
        } 

        return (bmiHeader.biHeight > 0) && (bmiHeader.biCompression == 0);
    }

    HRESULT                 AlignFrameSizes(mfxFrameInfo* pInfo, mfxU16 nWidth, mfxU16 nHeight);
    HRESULT                 DynamicFrameSizeChange(mfxFrameInfo* pInfo);
    HRESULT                 DeliverBitstream(mfxBitstream* pBS);
    
    mfxStatus               OnFrameReady(mfxBitstream* pBS);
    mfxStatus               StoreEncoderParams(Params *params);
    virtual bool            EncResetRequired(const mfxFrameInfo *pNewFrameInfo);

protected:

    CComBSTR                      m_bstrFilterName;      //filter name 
	//FILE*						  fp;					 //调试用 Patrick

    BOOL                          m_bStop;               // (BOOL) graph is stopped
    CCritSec                      m_csLock;

    HANDLE                        m_Thread;

    // minimum required API version
    mfxU16                        m_nAPIVerMinor; 
    mfxU16                        m_nAPIVerMajor;

    mfxVideoParam                 m_mfxParamsVideo;      // video params for ENC
    mfxVideoParam                 m_mfxParamsVPP;        // video params for VPP

    CTimeManager*                 m_pTimeManager;
    CBitstreamAdapter*            m_pAdapter;

    CBaseEncoder*                 m_pEncoder;

    BOOL                          m_bUseVPP;    

    BaseFrameAllocator*           m_pExternalFrameAllocator; // if color space converting plugin is used
                                                             // we must provide Media SDK an external frame allocator
    BOOL                          m_bUseRGB32toNV12Plugin;

    Params                        m_EncoderParams;

    DWORD                         m_FramesReceived;          // number of frames received for statistics

	BOOL						  m_isWorking;				//是否暂停视频发送  Patrick

    REFERENCE_TIME                m_rtLastKeyFrame; // 记录最后一个关键帧时间

private:
    DISALLOW_COPY_AND_ASSIGN(CEncVideoFilter);
};

/////////////////////////////////////////////////////////////////////////////////////////////

class CEncoderInputPin : public CTransformInputPin
{
public:
    CEncoderInputPin(CTransformFilter* pTransformFilter, HRESULT* phr);
    ~CEncoderInputPin();

    // CBaseInputPin
    STDMETHODIMP GetAllocatorRequirements(ALLOCATOR_PROPERTIES *pProps);

    // IUnknown
    DECLARE_IUNKNOWN;
    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);   
    
    virtual STDMETHODIMP NotifyAllocator(IMemAllocator * pAllocator, BOOL bReadOnly);

private:
    DISALLOW_COPY_AND_ASSIGN(CEncoderInputPin);
};

class CVideoMemEncoderInputPin :
    public CEncoderInputPin,
    public IMFGetService,
    public IDirectXVideoMemoryConfiguration
{
public:
    CVideoMemEncoderInputPin(CTransformFilter* pTransformFilter, HRESULT* phr);
    ~CVideoMemEncoderInputPin();

    // IUnknown
    DECLARE_IUNKNOWN;
    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    // IDirectXVideoMemoryConfiguration
    virtual STDMETHODIMP GetService(REFGUID guidService,
        REFIID riid,
        LPVOID *ppvObject);
    virtual STDMETHODIMP GetAvailableSurfaceTypeByIndex( 
        DWORD dwTypeIndex,
        DXVA2_SurfaceType *pdwType);
    virtual STDMETHODIMP SetSurfaceType(DXVA2_SurfaceType dwType);

protected:    
    IDirect3D9*              m_pd3d;
    IDirect3DDeviceManager9* m_pd3dDeviceManager; 
    IDirect3DDevice9*        m_pd3dDevice;

private:
    DISALLOW_COPY_AND_ASSIGN(CVideoMemEncoderInputPin);
};
