// AudioDevice.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "SystemDeviceManager.h"
#include <string>
#include <tchar.h>
#include <dshow.h>
#include <atlbase.h>
#include <qedit.h>
#include <comutil.h> //for _bstr_t

using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//工具函数----------------------------------------------------zhenHua.sun 2010-07-19
std::wstring str2wstr(const std::string& str)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	wchar_t *buff = (wchar_t*) malloc(sizeof(wchar_t) * (str.size() + 1));
	mbstowcs(buff, str.c_str(), str.size() + 1);
	std::wstring wstr(buff);
	free(buff);

	setlocale(LC_ALL, curLocale.c_str());
	return wstr;
}

std::string wstr2str( const std::wstring& wstr )
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	char *buff = (char*) malloc( sizeof(char)*(wstr.size()+1) );
	wcstombs( buff , wstr.c_str() , wstr.size()+1 );
	std::string str( buff );
	free( buff );

	setlocale(LC_ALL, curLocale.c_str());
	return str;
}


/**
 * @brief 枚举指定ClassID下的设备友好名称和设备名称
 * @param clsidDeviceClass 要枚举的设备ClassID
 * @param pWStringArray 存储设备名称的数组
 * @NumOfDevices 输出参数，对于每个设备存在着两个名称，因此大小是设备数量乘以2
 */
HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass,std::wstring * pWStringArray,int & NumOfDevices)
{
	//设备枚举器Interface
	ICreateDevEnum *pDevEnum = NULL; 
	
	//名称枚举Interface
	IEnumMoniker *pEnum = NULL;

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum)); //创建设备枚举COM对象
	if( FAILED( hr ) )
	{
		return E_FAIL;
	}
	
	// Create an enumerator for the video capture category.
	hr = pDevEnum->CreateClassEnumerator(
			clsidDeviceClass,	//CLSID_VideoInputDeviceCategory or CLSID_AudioInputDeviceCategory
			&pEnum, 0); //创建视频采集设备枚举COM对象
	if( pEnum==NULL )
	{
		pDevEnum->Release();
		return E_FAIL;
	}
	
	NumOfDevices=0;
	IMoniker *pMoniker = NULL;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK) //依次枚举，直至为空
	{
		IPropertyBag *pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue; // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &varName, 0); //设备友好名称
		}
		if (SUCCEEDED(hr))
		{
			// Add it to the application's list box.
		//	char displayName[1024];
		//	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);
			
			pWStringArray[NumOfDevices++]=varName.bstrVal;//varName.bstrVal;
			VariantClear(&varName); 

			WCHAR * wszDiaplayName=NULL;
			pMoniker->GetDisplayName(NULL,NULL,&wszDiaplayName);
			pWStringArray[NumOfDevices++]=wszDiaplayName;
			CoTaskMemFree(wszDiaplayName);
		//	SysFreeString(varName.bstrVal); 
		}

		pPropBag->Release();
		pMoniker->Release();
	}

	pEnum->Release();
	pDevEnum->Release();
	return S_OK;
}

void SystemDeviceManager::getAudioInputDeviceList( vector<string>* deviceList )
{
	int numOfDevice = 0;
	std::wstring deviceCaptureList[20];
	HRESULT result = ListCaptureDevice( CLSID_AudioInputDeviceCategory, deviceCaptureList , numOfDevice );
	if( FAILED( result ) )
		return;

	for( int deviceCount=0 ; deviceCount<numOfDevice ; deviceCount+=2 )
	{
		string friendlyName = wstr2str( deviceCaptureList[deviceCount] );
		deviceList->push_back( friendlyName );//friendly name
		//deviceCaptureList[deviceCount+1] is device name
	}
}

void SystemDeviceManager::getAudioOuputDeviceList( vector<string>* deviceList )
{
	int numOfDevice = 0;
	std::wstring deviceCaptureList[20];
	HRESULT result = ListCaptureDevice( CLSID_AudioRendererCategory, deviceCaptureList , numOfDevice );
	if( FAILED( result ) )
		return ;

	for( int deviceCount=0 ; deviceCount<numOfDevice ; deviceCount+=2 )
	{
		string friendlyName = wstr2str( deviceCaptureList[deviceCount] );
		deviceList->push_back( friendlyName );//friendly name
		//deviceCaptureList[deviceCount+1] is device name
	}
}