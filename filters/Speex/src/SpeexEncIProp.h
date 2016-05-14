#ifndef __SpeexEncIProp__
#define __SpeexEncIProp__

#ifdef __cplusplus
extern "C" {
#endif
	
	DEFINE_GUID(IID_SpeexEncIProp,
		0x08af6540, 0x4f21, 0x11cf, 0xaa, 0xcb, 0x00, 0x20, 0xaf, 0x0b, 0x19, 0x92);

	
	DECLARE_INTERFACE_(SpeexEncIProp, IUnknown)
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

		//add by dsh 10-08-05
		STDMETHOD(put_State) (THIS_
			UINT32 state
			) PURE;
		
		STDMETHOD(get_State) (THIS_
			UINT32* pState
			) PURE;
		//end by dsh 
	};
	
#ifdef __cplusplus
}
#endif

#endif
