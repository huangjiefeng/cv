#include "stdafx.h"
#include "DirectXUtils.h"


#include <iostream>
using std::string;
using std::wstring;

/** 
 * @author zhenHua.sun
 * @date  2010-09-27
 * @brief 对特殊字符进行处理的字符创匹配函数，主要用于摄像头设备
 * @param incomeDeviceName 从终端传进来的摄像头名称
 * @param currentDeviceName 当前比较的摄像头名称
 * @return 如果匹配则返回0
 */

int wstrCmpSpecial( wstring currentDeviceName , wstring incomeDeviceName  )
{
	//对存在?的字符串进行特殊处理，即在?出现的位置两个输入参数都要做同等替换
	wstring specialSign = L"?";
	wstring newSign = L"_";

	size_t pos = 0;         
	while(true)   
	{   
		pos = incomeDeviceName.find( L"?" ,  pos);   
		if (pos == wstring::npos) 
		{
			//如果没找到?的话则不进行特殊处理
			break;
		}
		incomeDeviceName.replace(pos, specialSign.length() , newSign); 
		currentDeviceName.replace( pos , specialSign.length() , newSign);
		pos += newSign.length();  
	}   
	return wcscmp( incomeDeviceName.c_str() , currentDeviceName.c_str() );
}


HRESULT FindMyCaptureDevice(IBaseFilter **pF, BSTR bstrName) {
	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	// Enumerate the monikers.
	//CComPtr<IMoniker> pMoniker;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
		IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"FriendlyName", & varName, 0);
		if (SUCCEEDED(hr) && ( wstrCmpSpecial( varName.bstrVal ,bstrName )==0)) {
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void * *) & pFilter);
			break;
		}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	if (pFilter) {
		*pF = pFilter;
		(*pF)->AddRef(); // Add ref on the way out.
	}
	return hr;
}

HRESULT FindDevice( CLSID classid , IBaseFilter * * pF, BSTR bstrName) {
	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator( classid, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	// Enumerate the monikers.
	//CComPtr<IMoniker> pMoniker;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
			IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"FriendlyName", & varName, 0);
		if (SUCCEEDED(hr) && ( wstrCmpSpecial( varName.bstrVal ,bstrName )==0)) {
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void * *) & pFilter);
			break;
		}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	if (pFilter) {
		*pF = pFilter;
		(*pF)->AddRef(); // Add ref on the way out.
	}
	return hr;
}

IAMStreamConfig * GetIAMStreamConfig(IBaseFilter * pFilter) {
	IEnumPins * pEnum = NULL;

	HRESULT hr = pFilter->EnumPins(& pEnum);
	if (FAILED(hr)) {
		return NULL;
	}

	IPin * pPin = NULL;
	while (pEnum->Next(1, & pPin, NULL) == S_OK) {
        //有些视频设备需要先连接一次才能获得正确格式
        TrailConnect(pPin);
		IAMStreamConfig * pIAMS = NULL;
		hr = pPin->QueryInterface(IID_IAMStreamConfig, (void * *) & pIAMS);
		if (hr == S_OK) {
			return pIAMS;
		}
		pPin->Release();
	}
	pEnum->Release();
	return NULL;
}

//工具函数----------------------------------------------------zhenHua.sun 2010-07-19
std::wstring str2wstr(const std::string& str)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = str.c_str();
	size_t _Dsize = str.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring wstr = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return wstr;
}

std::string wstr2str( const std::wstring& wstr )
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = wstr.c_str();
	size_t _Dsize = 2 * wstr.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string str = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());

    return str;
}

/**
 * @brief 枚举指定ClassID下的设备友好名称和设备名称
 * @param clsidDeviceClass 要枚举的设备ClassID
 * @param pWStringArray 存储设备名称的数组
 * @NumOfDevices 输出参数，对于每个设备存在着两个名称，因此大小是设备数量乘以2
 */
HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass, std::vector<std::wstring> & wstringArray,int & nNumOfDevices)
{
	////设备枚举器Interface
	//ICreateDevEnum *pDevEnum = NULL; 
	//
	////名称枚举Interface
	//IEnumMoniker *pEnum = NULL;

	//// Create the System Device Enumerator.
	//HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
	//	CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
	//	reinterpret_cast<void**>(&pDevEnum)); //创建设备枚举COM对象
	//if( FAILED( hr ) )
	//{
	//	return E_FAIL;
	//}
	//
	//// Create an enumerator for the video capture category.
	//hr = pDevEnum->CreateClassEnumerator(
	//		clsidDeviceClass,	//CLSID_VideoInputDeviceCategory or CLSID_AudioInputDeviceCategory
	//		&pEnum, 0); //创建视频采集设备枚举COM对象
	//if( pEnum==NULL )
	//{
	//	pDevEnum->Release();
	//	return E_FAIL;
	//}
	//
	//NumOfDevices=0;
	//IMoniker *pMoniker = NULL;
	//while (pEnum->Next(1, &pMoniker, NULL) == S_OK) //依次枚举，直至为空
	//{
	//	IPropertyBag *pPropBag;
	//	hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
	//		(void**)(&pPropBag));
	//	if (FAILED(hr))
	//	{
	//		pMoniker->Release();
	//		continue; // Skip this one, maybe the next one will work.
	//	} 
	//	// Find the description or friendly name.
	//	VARIANT varName;
	//	VariantInit(&varName);
	//	
	//	hr = pPropBag->Read(L"Description", &varName, 0);
	//	if (FAILED(hr))
	//	{
	//		hr = pPropBag->Read(L"FriendlyName", &varName, 0); //设备友好名称
	//	}
	//	if (SUCCEEDED(hr))
	//	{
	//		// Add it to the application's list box.
	//	//	char displayName[1024];
	//	//	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);
	//		
	//		pWStringArray[NumOfDevices++]=varName.bstrVal;//varName.bstrVal;
	//		VariantClear(&varName); 

	//		WCHAR * wszDiaplayName=NULL;
	//		pMoniker->GetDisplayName(NULL,NULL,&wszDiaplayName);
	//		pWStringArray[NumOfDevices++]=wszDiaplayName;
	//		CoTaskMemFree(wszDiaplayName);
	//	//	SysFreeString(varName.bstrVal); 
	//	}

	//	pPropBag->Release();
	//	pMoniker->Release();
	//}

	//pEnum->Release();
	//pDevEnum->Release();
	//return S_OK;

	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator(clsidDeviceClass, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	// Enumerate the monikers.
	//CComPtr<IMoniker> pMoniker;

	nNumOfDevices = 0;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
			IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"Description", &varName, 0);
		if (FAILED(hr))
		{
			hr = pProp->Read(L"FriendlyName", &varName, 0); //设备友好名称
		}
		if (SUCCEEDED(hr))
		{
			// Add it to the application's list box.
			//	char displayName[1024];
			//	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);

			wstringArray.push_back(varName.bstrVal);//varName.bstrVal;
			++nNumOfDevices;
			VariantClear(&varName); 

			WCHAR * wszDiaplayName=NULL;
			pMoniker->GetDisplayName(NULL,NULL,&wszDiaplayName);
			wstringArray.push_back(wszDiaplayName);
			++nNumOfDevices;
			CoTaskMemFree(wszDiaplayName);
			//	SysFreeString(varName.bstrVal); 
		}
		//hr = pProp->Read(L"FriendlyName", & varName, 0);
		//if (SUCCEEDED(hr) && ( wstrCmpSpecial( varName.bstrVal ,bstrName )==0)) {
		//	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void * *) & pFilter);
		//	break;
		//}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	return hr;
}
//-----------------------------------------------------------------------------------

HRESULT EnumerateDevices( REFGUID category, std::vector<DeviceInfo> &devices) {
  devices.clear();
  CoInitialize(NULL); //因为可能会在不同线程调用此方法，每个线程都要初始化，所以在这里放了一个，重复调用无问题
  ICreateDevEnum *pDevEnum;
  IEnumMoniker *pEnum;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
                                NULL,
                                CLSCTX_INPROC_SERVER,
                                IID_PPV_ARGS(&pDevEnum));

  if (SUCCEEDED(hr)) {
	hr = pDevEnum->CreateClassEnumerator(category, &pEnum,0);
	pDevEnum->Release();
	if(hr == S_FALSE){
      hr = VFW_E_NOT_FOUND;
      return hr;
	}
  }

  IMoniker *pMoniker = NULL;
  while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
	IPropertyBag *pPropBag;
	HRESULT hr = pMoniker->BindToStorage(0,0,IID_PPV_ARGS(&pPropBag));
	if(FAILED(hr)) {
	  pMoniker->Release();
	  continue;
	}

	VARIANT var;
	VariantInit(&var);

	hr = pPropBag->Read(L"FriendlyName", &var,0);
    DeviceInfo info;
	if (SUCCEEDED(hr)) {
	  info.friendly_name = var.bstrVal;
	}
	hr = pPropBag->Read(L"DevicePath", &var,0);
	if(SUCCEEDED(hr)) {
	  info.device_path = var.bstrVal;
	}

	//读取Displayname
	LPOLESTR displayName = nullptr;
	hr = pMoniker->GetDisplayName(NULL,NULL,&displayName);
	if (SUCCEEDED(hr))
	{
		info.display_name = displayName;
	}
    devices.push_back(info);

	VariantClear(&var);
	pPropBag->Release();
	pMoniker->Release();
  }
  pEnum->Release();
  return hr;
}

HRESULT TrailConnect( IPin * pPin )
{
    HRESULT hr;
    if (!pPin) return E_INVALIDARG;
    //检查PIN类型
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir != PINDIR_OUTPUT) return E_INVALIDARG;
    }

    //添加NULL Renderer
    const CLSID nullRenderId = CLSID_NullRenderer; // {0xC1F400A4, 0x3F08, 0x11D3, {0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};
    CComPtr<IBaseFilter> pNullRenderer = 0;
    hr = CoCreateInstance(nullRenderId,0,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)(&pNullRenderer));
    if (FAILED(hr) || !pNullRenderer) return E_FAIL;

    //获得Null Renderer的输入PIN
    CComPtr<IPin> pPinRenderer;
    CComPtr<IEnumPins> pEnum;
    hr = pNullRenderer->EnumPins(&pEnum);
    if (FAILED(hr)) return E_FAIL;
    while (pEnum->Next(1, &pPinRenderer, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPinRenderer->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PINDIR_INPUT) break;
    }
    if (!pPinRenderer) return E_FAIL;

    //试连接
    hr = pPin->Connect(pPinRenderer, NULL);
    hr = pPin->Disconnect();
    return S_OK;
}
