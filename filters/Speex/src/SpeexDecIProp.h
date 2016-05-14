#ifndef __SpeexEncIProp__
#define __SpeexEncIProp__

#ifdef __cplusplus
extern "C" {
#endif
	

	// {782416A6-EB09-4c14-A836-55D4D4E44AA4}
	DEFINE_GUID(IID_SpeexDecIProp, 
	0x782416a6, 0xeb09, 0x4c14, 0xa8, 0x36, 0x55, 0xd4, 0xd4, 0xe4, 0x4a, 0xa4);


	
	DECLARE_INTERFACE_(SpeexDecIProp, IUnknown)
	{
		
		STDMETHOD(put_SampleRate) (THIS_
			UINT32 sampleRate
			) PURE;
		
		STDMETHOD(get_SampleRate) (THIS_
			UINT32* pSampleRate
			) PURE;
		
		STDMETHOD(put_Quality) (THIS_
			UINT32 quality
			) PURE;
		
		STDMETHOD(get_Quality) (THIS_
			UINT32* pQuality
			) PURE;
	};
	
#ifdef __cplusplus
}
#endif

#endif
