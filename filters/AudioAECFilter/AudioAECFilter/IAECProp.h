#ifndef __IAECProp__
#define __IAECProp__

//#ifdef __cplusplus
//extern "C" {
//#endif
	

	// {3EE64499-F3C2-4074-8E41-FEE91C8BE36F}
	DEFINE_GUID(IID_IAECProperty, 
	0x3ee64499, 0xf3c2, 0x4074, 0x8e, 0x41, 0xfe, 0xe9, 0x1c, 0x8b, 0xe3, 0x6f);


	
	DECLARE_INTERFACE_(IAECProperty, IUnknown)
	{
		
		STDMETHOD(put_CapDevFriendName) (THIS_
			wchar_t* capDevFriendName,
			int capNameSize,
			wchar_t* rendDevFriendName,
			int rendNameSize
			) PURE;
		STDMETHOD(put_PreviewHandle) (THIS_
			HWND hPreviewHandle
			) PURE;
	};
	
//#ifdef __cplusplus
//}
//#endif

#endif
