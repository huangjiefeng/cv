#include <atlbase.h>
#include <assert.h>
#include <streams.h>

#include "GraphUtil.h"
#include "LogUtil.h"
#include "Util.h"
#include "DxDefinitions.h"

using namespace msdx;

//other
const CLSID GraphUtil::clsid_null_renderer = {0xC1F400A4, 0x3F08, 0x11D3, {0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};
const CLSID GraphUtil::clsid_inf_pin_tee = {0xf8388a40, 0xd5bb, 0x11d0, {0xbe, 0x5a, 0x00, 0x80, 0xc7, 0x06, 0x56, 0x8e}};
const CLSID GraphUtil::clsid_scut_multi_smart_tee = {0xAFACE3E1, 0xD5FD, 0x4717,{0xAF, 0x4B, 0x41, 0xD8, 0x6E, 0xBD, 0x4A, 0xD1}};
const CLSID GraphUtil::clsid_file_source = {0xE436EBB6, 0x524F, 0x11CE, {0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
//video
const CLSID GraphUtil::clsid_intel_h264_enc = {0x71183C45,0xF4FA,0x4B10,{0x9E,0x04,0xF9,0x04,0x0C,0xB1,0x91,0x39}};
const CLSID GraphUtil::clsid_intel_h264_dec = {0xCCCE52FD,0x02CB,0x482C,{0xAC,0x81,0x1E,0x55,0xEF,0x1D,0x61,0xEE}};
const CLSID GraphUtil::clsid_scut_rgb2yuv = {0x828ED7A4,0x0769,0x4751,{0x80,0x16,0xD9,0xE4,0x2A,0x65,0xFA,0x7D}};
const CLSID GraphUtil::clsid_scut_h264_enc = {0xe2c7d3a5, 0x819a, 0x4881,{0x87, 0xba, 0xf1, 0xc9, 0x33, 0x19, 0xd1, 0x8a}};
const CLSID GraphUtil::clsid_scut_h264_dec = {0x7398baae, 0xbb0, 0x4565, {0x99, 0xa6, 0x44, 0x80, 0xdf, 0xd4, 0xa9, 0x73}};
//audio
const CLSID GraphUtil::clsid_scut_aec_source = {0x31c942ce, 0xe32c, 0x4aef, {0x95, 0xb4, 0x86, 0xf9, 0xe6, 0x97, 0xaf, 0xd8}};
const CLSID GraphUtil::clsid_scut_speex_enc = {0x08af6540, 0x4f21, 0x11cf, {0xaa, 0xcb, 0x00, 0x20, 0xaf, 0x0b, 0x99, 0xFA}};
const CLSID GraphUtil::clsid_scut_speex_dec = {0x08af6540, 0x4f21, 0x11cf, {0xaa, 0xcb, 0x00, 0x20, 0xaf, 0x0b, 0x99, 0xFB}};
const CLSID GraphUtil::clsid_monogram_aac_enc = {0x88F36DB6, 0xD898, 0x40B5, {0xB4, 0x09, 0x46, 0x6A, 0x0E, 0xEC, 0xC2, 0x6A}};
const CLSID GraphUtil::clsid_monogram_aac_dec = {0x3FC3DBBF, 0x9D37, 0x4CE0, {0x86, 0x89, 0x65, 0x3F, 0xE8, 0xBA, 0xB9, 0xB3}};
//新RTP
const CLSID GraphUtil::clsid_scut_rtp_sender = {0x31158a57, 0x8952, 0x4ebe, {0x85, 0xf0, 0x90, 0x9d, 0xff, 0x6b, 0x4b, 0x24}};
const CLSID GraphUtil::clsid_scut_rtp_receiver = {0x92a734e7, 0x0a0d, 0x493b, {0x91, 0x96, 0xdf, 0xb8, 0x41, 0xec, 0x68, 0xb4}};
//旧RTP
const CLSID GraphUtil::clsid_scut_rtp_out_a = {0x8a773710, 0x9b5c, 0x41c0,{0xb6, 0x5f, 0x34, 0xa0, 0xdf, 0x21, 0xac, 0xcc}};
const CLSID GraphUtil::clsid_scut_rtp_out_v = {0x9bc900af, 0xac7e, 0x4d25,{0xb9, 0x4a, 0x5a, 0xe8, 0xea, 0x06, 0xc9, 0x6b}};
const CLSID GraphUtil::clsid_scut_rtp_in_v = {0x436cd349, 0x1c80, 0x455d,{0x81, 0x3d, 0x3, 0xe9, 0xb3, 0x68, 0x68, 0x64}};
const CLSID GraphUtil::clsid_scut_rtp_in_a = {0x2A46575E, 0x0FA8, 0x4F3A,{0xa2, 0x92, 0x38, 0x98, 0xD1, 0xCB, 0x67, 0x10}};
//record
const CLSID GraphUtil::clsid_gdcl_mp4_muxer = {0x5FD85181, 0xE542, 0x4e52, {0x8D, 0x9D, 0x5D, 0x61, 0x3C, 0x30, 0x13, 0x1B}};
//file play
const CLSID GraphUtil::clsid_gdcl_mp4_demuxer = {0x025BE2E4, 0x1787, 0x4DA4, {0xA5, 0x85, 0xC5, 0xB2, 0xB9, 0xEE, 0xB5, 0x7C}};
const CLSID GraphUtil::clsid_scut_play_ctrl = {0xafd076d, 0x3ef3, 0x4178, {0x93, 0x9f, 0x9, 0x97, 0x2e, 0x91, 0x62, 0x41}};
//ffdshow
const CLSID GraphUtil::clsid_ffdshow_encoder = {0x4DB2B5D9,0x4556,0x4340,{0xB1,0x89,0xAD,0x20,0x11,0x0D,0x95,0x3F}};
//{04FE9017-F873-410E-871E-AB91661A4EF7}
const CLSID GraphUtil::clsid_ffdshow_decoder = {0x04FE9017,0xF873,0x410E,{0x87,0x1E,0xAB,0x91,0x66,0x1A,0x4E,0xF7}};

//Lead
const CLSID GraphUtil::clsid_lead_h264_encoder = {0xE2B7DF52,0x38C5,0x11D5,{0x91,0xF6,0x00,0x10,0x4B,0xDB,0x8F,0xF9}};
//leadconverter
const CLSID GraphUtil::clsid_lead_CSconverter = {0xE2B7DAF2,0x38C5,0x11D5,{0x91,0xF6,0x00,0x10,0x4B,0xDB,0x8F,0xF9}};
//render
const CLSID GraphUtil::clsid_render = {0x70E102B0,0x5556,0x11CE,{0x97,0xC0,0x00,0xAA,0x00,0x55,0x59,0x5A}};


GraphUtil::GraphUtil(void)
{
}

GraphUtil::~GraphUtil(void)
{
}

HRESULT GraphUtil::GetFilterByCLSID(const CLSID& clsid, IBaseFilter **ppF)
{
	if (!ppF)
	{
		return E_POINTER;
	}

	*ppF = 0;
	IBaseFilter *pF = 0;
	HRESULT hr = CoCreateInstance(clsid,0,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)(&pF));
	if (FAILED(hr))
	{
		return hr;
	}
	*ppF = pF;
	return S_OK;
} 

//之前写的根据三种名字的getFilter方法综合为一个，用type指出方法名
HRESULT GraphUtil::getFilterByVariousName(const CLSID & clsidDeviceClass, const int type,LPCWSTR wszName, IBaseFilter **ppF)
{
	CComPtr<ICreateDevEnum> pDevEnum;
	CComPtr<IEnumMoniker> pEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));


	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(clsidDeviceClass,&pEnum,0);
		if(pEnum == NULL)
		{
			return VFW_E_NOT_FOUND;
		}
	}

	CComPtr<IMoniker> pMoniker;
	while (pEnum->Next(1,&pMoniker,NULL) == S_OK)
	{
		CComPtr<IPropertyBag> pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0,0,IID_PPV_ARGS(&pPropBag));
		if(FAILED(hr))
		{
			continue;
		}

		VARIANT devicepath;
		VariantInit(&devicepath);
		VARIANT friendlyname;
		VariantInit(&friendlyname);
		hr = pPropBag->Read(L"DevicePath",&devicepath,0);
		hr = pPropBag->Read(L"FriendlyName",&friendlyname,0);
		WCHAR *displayname=NULL;
		pMoniker->GetDisplayName(NULL,NULL,&displayname);
		switch (type)
		{
		case 1://FriendlyName
			if(wcscmp(wszName,friendlyname.bstrVal) == 0)
			{
				IBaseFilter *pF = NULL;
				hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pF);
				if (pF) *ppF = pF;
				return hr;
			}
			break;
		case 2://DevicePath
			if(wcscmp(wszName,devicepath.bstrVal) == 0)
			{
				IBaseFilter *pF = NULL;
				hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pF);
				if (pF) *ppF = pF;
				return hr;
			}
			break;
		case 3://DisplayName
			if(wcscmp(wszName,displayname) == 0)
			{
				IBaseFilter *pF = NULL;
				hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pF);
				if (pF) *ppF = pF;
				return hr;
			}

		default:
			break;
		}


		pMoniker = NULL; //注意：COM智能指针必须为空才能继续使用
		VariantClear(&devicepath);
		VariantClear(&friendlyname);
	}

	return E_FAIL;
}


// Set wszFrendlyName to be NULL to get the Default Device
HRESULT GraphUtil::GetFilterByFriendlyName(const CLSID & clsidDeviceClass, LPCWSTR wszFrendlyName, IBaseFilter **ppF)
{
	//设备枚举器Interface
	CComPtr<ICreateDevEnum> pDevEnum;

	//名称枚举Interface
	CComPtr<IEnumMoniker> pEnum;

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
		reinterpret_cast<void**>(&pDevEnum));

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			clsidDeviceClass,	//CLSID_VideoInputDeviceCategory or CLSID_AudioInputDeviceCategory
			&pEnum, 0);
	}
	else if (pDevEnum==NULL)
	{
		//创建失败
		return E_FAIL; //如果没有设备，返回
	}

	if(pEnum == NULL )
	{
		return E_FAIL; //如果没有设备，返回
	}

	CComPtr<IMoniker> pMoniker;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK) //依次枚举，直至为空
	{
		CComPtr<IPropertyBag> pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)(&pPropBag));
		if (FAILED(hr))
		{
			continue; // Skip this one, maybe the next one will work.
		}

		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		//hr = pPropBag->Read(L"Description", &varName, 0);
		//if (FAILED(hr))
		//{
		hr = pPropBag->Read(L"FriendlyName", &varName, 0); //设备友好名称
		//}
		if (SUCCEEDED(hr))
		{

			// Add it to the application's list box.
			//	char displayName[1024];
			//	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);

			//pWStringArray[NumOfDevices++] = varName.bstrVal; //varName.bstrVal;
			WCHAR * wszDeviceFriendlyName = NULL;
			wszDeviceFriendlyName = varName.bstrVal;
			VariantClear(&varName);
			if (wszFrendlyName == NULL || wcscmp(wszDeviceFriendlyName, wszFrendlyName) == 0)
			{
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)ppF);
				if (SUCCEEDED(hr))
				{
					return S_OK;
				} 
				else
				{
					return hr;
				}
			}
		}
		pMoniker = NULL; //注意：COM智能指针必须为空才能继续使用
	}

	return E_FAIL;
}

HRESULT GraphUtil::GetFilterByDisplayName(LPCWSTR wszDisplayName, IBaseFilter **ppF)
{
	HRESULT hr;
	CComPtr<IMoniker> pMoniker;
	CComPtr<IBindCtx> pBindCtx;
	ULONG chEaten = 0;

	hr = CreateBindCtx(0, &pBindCtx);
	if(FAILED(hr))
	{
		return  hr;
	}
	hr = MkParseDisplayName(pBindCtx, wszDisplayName, &chEaten, &pMoniker);
	if(FAILED(hr))
	{
		return  hr;
	}
	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)ppF);
	if (ppF == NULL) 
	{
		return  E_UNEXPECTED;
	}

	return S_OK;
}

HRESULT GraphUtil::GetFilterByDevicePath(const CLSID &clsidDeviceClass, LPCWSTR wszDevicePath, IBaseFilter **ppF)
{
	//用DisplayName的方法
	//HRESULT hr;
	//WCHAR szMon[]=L"@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}";
	//IBindCtx *pBindCtx;
	//hr=CreateBindCtx(0,&pBindCtx);
	//ULONG chEaten=0;
	//IMoniker *pMoniker=0;
	//hr=MkParseDisplayName(pBindCtx,szMon,&chEaten,&pMoniker);
	//pBindCtx->Release();
	//IBaseFilter *pF = NULL;
	//hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pF);
	//if (pF) *ppF = pF;
	//return hr;
	//if(SUCCEEDED(hr))
	//{
	//	pMoniker = NULL;
	//}

	//	ICreateDevEnum *pCreateDevEnum=0;
	//	HRESULT hr;
	//	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
	//		IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	//	if(hr != NOERROR)
	//	{
	////		ErrMsg(TEXT("Error Creating Device Enumerator"));
	//		//return;
	//	}
	//
	//	IEnumMoniker *pEm=0;
	//	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	//	pEm->Reset();
	//	ULONG cFetched;
	//	IMoniker *pM;
	//
	//	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	//	{
	//		IPropertyBag *pBag=0;
	//
	//		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
	//		if(SUCCEEDED(hr))
	//		{
	//			VARIANT var;
	//			VARIANT varDisplayName;
	//			VARIANT varDevicePath;
	//			var.vt = VT_BSTR;
	//			varDevicePath.vt = VT_BSTR;
	//			varDisplayName.vt=VT_BSTR;
	//			hr = pBag->Read(L"FriendlyName", &var, NULL);
	//			hr = pBag->Read(L"DisplayName", &varDisplayName, NULL);
	//						hr = pBag->Read(L"DevicePath", &varDisplayName, NULL);
	//			if(hr == NOERROR)
	//			{
	//				pM->AddRef();
	//			}
	//			pBag->Release();
	//		}
	//
	//		pM->Release();
	//	}
	//	pEm->Release();
	CComPtr<ICreateDevEnum> pDevEnum;
	CComPtr<IEnumMoniker> pEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));


	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(clsidDeviceClass,&pEnum,0);
		if(pEnum == NULL)
		{
			return VFW_E_NOT_FOUND;
		}
	}

	CComPtr<IMoniker> pMoniker;
	while (pEnum->Next(1,&pMoniker,NULL) == S_OK)
	{
		CComPtr<IPropertyBag> pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0,0,IID_PPV_ARGS(&pPropBag));
		if(FAILED(hr))
		{
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		VARIANT var2;
		VariantInit(&var2);
		hr = pPropBag->Read(L"DevicePath",&var,0);
		hr = pPropBag ->Read(L"DisplayName", &var2, 0);

		if(wcscmp(wszDevicePath,var.bstrVal) == 0)
		{
			IBaseFilter *pF = NULL;
			hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pF);
			if (pF) *ppF = pF;
			return hr;
		}
		pMoniker = NULL; //注意：COM智能指针必须为空才能继续使用
		VariantClear(&var);
	}

	return E_FAIL;
}

HRESULT GraphUtil::AddFilterByCLSID(
	IGraphBuilder *pGraph, LPCWSTR wszName, const CLSID &clsid, IBaseFilter **ppF)
{
	HRESULT hr;
	IBaseFilter *pFilter = NULL;

	LOG_PRINTF(L"Adding filter: %s.", wszName);

	hr = GetFilterByCLSID(clsid, &pFilter);
	if (FAILED(hr) || pFilter == NULL) return hr;

	if (pGraph)
	{
		hr = pGraph->AddFilter(pFilter, wszName);
		if (FAILED(hr)) return hr;
	}

	if (ppF)
	{
		(*ppF) = pFilter;
	}
	else
	{
		pFilter->Release(); //减少一个引用计数
	}

	return S_OK;
}

HRESULT GraphUtil::GetUnconnectedPin(
	IBaseFilter *pFilter, // Pointer to the filter.
	PIN_DIRECTION PinDir, // Direction of the pin to find.
	IPin **ppPin) // Receives a pointer to the pin.
{
	*ppPin = 0;
	CComPtr<IEnumPins> pEnum;
	IPin *pPin = nullptr;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr)) // Already connected, not the pin we want.
			{
				pTmp->Release();
			}
			else // Unconnected, 这就是我们想要的pin，空闲的pin
			{
				//pEnum->Next()返回pin前已增加引用计数，这里不要再加了
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	// Did not find a matching pin.
	return E_FAIL;
}

HRESULT GraphUtil::ConnectFilters(
	IGraphBuilder *pGraph, // Filter Graph Manager.
	IBaseFilter *pSource, // Upstream filter.
	IBaseFilter *pDest) // Downstream filter.
{
	if ((pGraph == NULL) || (pSource == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}

	//找一个空闲的输入pin
	IPin *pOut = NULL;
	IPin *pIn = NULL;

	HRESULT hr = GetUnconnectedPin(pSource, PINDIR_OUTPUT, &pOut);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr))
	{
		return hr;
	}
	// Try to connect them.
	hr = pGraph->Connect(pOut, pIn);
	pOut->Release();
	pIn->Release();
	return hr;
}

HRESULT GraphUtil::ConnectFilters(IGraphBuilder *pGraph, IPin *pPinOut, IBaseFilter *pDest)
{
	if ((pGraph == NULL) || (pPinOut == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
	
	//找一个空闲的输入pin
	IPin *pIn = NULL;

	HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr))
	{
		return hr;
	}
	// Try to connect them.
	hr = pGraph->Connect(pPinOut, pIn);
	pIn->Release(); //Release
	return hr;
}


#ifdef _DEBUG
//之前版本在本机（Win10 64bit）上不能观察到graph，故修改--HJF 2015.11.27
HRESULT GraphUtil::AddToRot( LPCWSTR wszDescription, IUnknown *pUnkGraph, DWORD *pdwRegister )//
{
		IMoniker * pMoniker;
		IRunningObjectTable *pROT;
		if (FAILED(GetRunningObjectTable(0, &pROT))) {
			return E_FAIL;
		}
		WCHAR wsz[256];
		wsprintfW(wsz, L"FilterGraph %08x pid %08x", wszDescription, GetCurrentProcessId());
		HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
		if (SUCCEEDED(hr)) {
			hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
				pMoniker, pdwRegister);
			pMoniker->Release();
		}
		pROT->Release();
		return hr;

}

void GraphUtil::RemoveFromRot( DWORD pdwRegister )
{
	//CComPtr<IRunningObjectTable> pROT;
	//if(SUCCEEDED(GetRunningObjectTable(0,&pROT)))
	//{
	//	pROT->Revoke(pdwRegister);
	//}
	IRunningObjectTable *pROT;
	if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
		pROT->Revoke(pdwRegister);
		pROT->Release();
	}
}

#endif // DEBUG

