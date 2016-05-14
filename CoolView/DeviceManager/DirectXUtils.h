#ifndef DIRECTX_UTILS_H
#define DIRECTX_UTILS_H

#include <tchar.h>
#include <strsafe.h>
#include <dshow.h>
#include <atlbase.h>
#define __IDxtCompositor_INTERFACE_DEFINED__ 
#define __IDxtAlphaSetter_INTERFACE_DEFINED__ 
#define __IDxtJpeg_INTERFACE_DEFINED__ 
#define __IDxtKey_INTERFACE_DEFINED__ 
#include "qedit.h"
#include <comutil.h> //for _bstr_t

#include <string>
#include <vector>

struct DeviceInfo {
  std::wstring friendly_name;
  std::wstring device_path;

  std::wstring display_name;//add by hjf
};

HRESULT FindMyCaptureDevice(IBaseFilter ** pF, BSTR bstrName);

HRESULT FindDevice(CLSID classid , IBaseFilter * * pF, BSTR bstrName);
/**
 * Takes a base filter and try to return a pointer to its IAMStreamConfig.
 *
 * @return a NULL pointer in the case it fails
 */
IAMStreamConfig * GetIAMStreamConfig(IBaseFilter * pFilter);

std::wstring str2wstr(const std::string& str);

std::string wstr2str( const std::wstring& wstr );
/**
 * @brief 枚举指定ClassID下的设备友好名称和设备名称
 * @param clsidDeviceClass 要枚举的设备ClassID
 * @param pWStringArray 存储设备名称的数组
 * @NumOfDevices 输出参数，对于每个设备存在着两个名称，因此大小是设备数量乘以2
 */
HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass, std::vector<std::wstring> & wstringArray,int & nNumOfDevices);

HRESULT EnumerateDevices(REFGUID category, std::vector<DeviceInfo> &devices);

/**
 * @brief 将指定的IPin连接到Null Renderer
 */
HRESULT TrailConnect(IPin * pPin);

///**
// * @brief 枚举指定ClassID下的设备友好名称和设备名称
// * @param clsidDeviceClass 要枚举的设备ClassID
// * @param pWStringArray 存储设备名称的数组
// * @NumOfDevices 输出参数，对于每个设备存在着两个名称，因此大小是设备数量乘以2
// */
//HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass,std::wstring * pWStringArray,int & NumOfDevices);

#endif	//DIRECTX_UTILS_H