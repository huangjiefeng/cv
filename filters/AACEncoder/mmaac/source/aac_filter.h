//-----------------------------------------------------------------------------
//
//	Monogram AAC Encoder
//
//	Base on GNU LGPL libfaac
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------
#pragma once

#include <queue>

struct SampleTime_t 
{
	__int64 sampleNo;
	REFERENCE_TIME rtStart;
};
typedef std::queue<SampleTime_t> SampleTimeQueue;

//-----------------------------------------------------------------------------
//
//	CAACEncoder
//
//-----------------------------------------------------------------------------

class CAACEncoder : 
	public CTransformFilter,
	public ISpecifyPropertyPages,
	public IMonogramAACEncoder

{
protected:

	// nastavenia + property pagesa
	CCritSec							lock_info;
	IMonogramAACEncoderPropertyPage		*property_page;
	AACConfig							config;
	AACInfo								info;

	// enkoder
	faacEncHandle						encoder;
	BYTE								*extradata;
	int									extradata_size;
    unsigned long                       max_output_size;

	// timing
	bool								got_time;
	REFERENCE_TIME						start;
	short								*buffer;
	int									samples;
	SampleTimeQueue                     sample_time;
	__int64                             base_sample;
	__int64								sample_received;

    // outbuf
    const int outsize;
    BYTE	 *outbuf;

	void CloseEncoder();
	int OpenEncoder();

public:

	// constructor & destructor
	CAACEncoder(LPUNKNOWN pUnk, HRESULT *phr);
	virtual ~CAACEncoder();
	static CUnknown *WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);

	DECLARE_IUNKNOWN;
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    // ISpecifyPropertyPages interface
    STDMETHODIMP GetPages(CAUUID *pPages);

	// Pure virtual methods
	virtual HRESULT CheckInputType(const CMediaType *mtIn);
	virtual HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp);
	virtual HRESULT GetMediaType(int iPosition, CMediaType *pmt);
	virtual HRESULT SetMediaType(PIN_DIRECTION direction, const CMediaType *pmt);
	virtual HRESULT BreakConnect(PIN_DIRECTION dir);

	// iny proces transformovania samplov
	virtual HRESULT Receive(IMediaSample *pSample);
	virtual HRESULT GetDeliveryBuffer(IMediaSample **sample);

	// IMonogramAACEncoder
	STDMETHODIMP GetConfig(AACConfig *config);
	STDMETHODIMP SetConfig(AACConfig *config);
	STDMETHODIMP GetInfo(AACInfo *info);
	STDMETHODIMP SetPropertyPage(IUnknown *pUnk);

	void UpdatePage(int info_only);

	// filter states
    virtual HRESULT StartStreaming();
	virtual HRESULT EndFlush();
};

