/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <webcam/DirectXWebcamDriver.h>

#include <webcam/DirectXWebcamUtils.h>

#include <pixertool/directx-pixertool.h>

#include <util/Logger.h>
#include <mtype.h>

using namespace std;

#define SAFE_RELEASE_POINTER(x) { if (x) x->Release(); x = NULL; }
#define SAFE_RELEASE(x) { if (x) x.Release(); x = NULL; }

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
//-----------------------------------------------------------------------------------


DirectXWebcamDriver::DirectXWebcamDriver(WebcamDriver * driver, int flags)
	: IWebcamDriver(flags) {

	// no more needed since apiInitialize() was introduced.
	//FIXME does not work because of Qt4.1.2
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);
	////
	//CoInitialize(NULL);
	////

	_webcamDriver = driver;
	_pGrabberF = NULL;
	_pGrabber = NULL;
	_pNull = NULL;
	_iam = NULL;
	_pGraph = NULL;
	_pControl = NULL;
	_pCap = NULL;
	_pBuild = NULL;
}

DirectXWebcamDriver::~DirectXWebcamDriver() {
	stopCapture();
	// no more needed since apiUninitialize() was introduced.
	//CoUninitialize();
	////
}

void DirectXWebcamDriver::cleanup() {
	LOG_DEBUG("Cleaning up the Direct X webcam driver");
	_isOpen = false;
	_cachedWidth = 0;
	_cachedHeight = 0;
	_cachedFPS = 15;
	_cachedPalette = PIX_OSI_UNSUPPORTED;
	SAFE_RELEASE_POINTER(_pGrabberF);
	SAFE_RELEASE_POINTER(_pGrabber);
	SAFE_RELEASE_POINTER(_pNull);
	SAFE_RELEASE_POINTER(_iam);
	SAFE_RELEASE(_pControl);
	SAFE_RELEASE(_pGraph);
	SAFE_RELEASE(_pCap);
	SAFE_RELEASE(_pBuild);
}

StringList DirectXWebcamDriver::getDeviceList() {
	StringList deviceList;

	LOG_DEBUG("Direct X Get Device List");

	/************************************************************************/
	/* 添加一个虚拟摄像头                                                   */
	//deviceList +="虚拟摄像头";
	/************************************************************************/
	//create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pCreateDevEnum) {
		return deviceList;
	}

	//enumerate video capture devices
	CComPtr< IEnumMoniker > pEnumMoniker;
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
	if (!pEnumMoniker) {
		return deviceList;
	}

	pEnumMoniker->Reset();
	//go through and find all video capture device(s)
	while (true) {
		CComPtr< IMoniker > pMoniker;
		HRESULT hr = pEnumMoniker->Next(1, &pMoniker, 0);
		if (hr != S_OK) {
			break;
		}

		//get the property bag for this moniker
		CComPtr< IPropertyBag > pPropertyBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**) &pPropertyBag);
		if (hr != S_OK) {
			continue;
		}

		//ask for the english-readable name
		CComVariant FriendlyName;
		CComVariant DevicePath;
		hr = pPropertyBag->Read(L"FriendlyName", &FriendlyName, NULL);
		hr = pPropertyBag->Read(L"DevicePath", &DevicePath, NULL);
		if (hr != S_OK) {
			continue;
		}

		std::string deviceName;
//		if (((string) _bstr_t(DevicePath)).find("pci") == string::npos) {//Modify by zhuang 09-04-20 不加判断
			deviceName = (string) _bstr_t(FriendlyName);
			deviceList += deviceName;
//		}

		/* TODO: do we still use this variable? see lib video in classic.
		else if (pci_device) {
			deviceName = (string) _bstr_t(FriendlyName);
			deviceList += deviceName;
		}
		*/
	}


	return deviceList;
}

std::string DirectXWebcamDriver::getDefaultDevice() {
	std::string defaultDevice;

	LOG_DEBUG("Direct X Get Default Device");

	//create an enumerator
	CComPtr< ICreateDevEnum > pCreateDevEnum;
	pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);

	//_ASSERTE(pCreateDevEnum);
	if (!pCreateDevEnum) {
		return WEBCAM_NULL;
	}

	//enumerate video capture devices
	CComPtr< IEnumMoniker > pEm;
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);

	//_ASSERTE(pEm);
	if (!pEm) {
		return WEBCAM_NULL;
	}

	pEm->Reset();

	//go through and find first video capture device
	while (true) {
		ULONG ulFetched = 0;
		CComPtr< IMoniker > pM;

		HRESULT hr = pEm->Next(1, &pM, &ulFetched);
		if (hr != S_OK) {
			break;
		}

		//get the property bag
		CComPtr< IPropertyBag > pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void**) &pBag);
		if (hr != S_OK) {
			continue;
		}

		//ask for the english-readable name
		CComVariant var;
		var.vt = VT_BSTR;
		hr = pBag->Read(L"FriendlyName", &var, NULL);
		if (hr != S_OK) {
			continue;
		}

		defaultDevice = (const char *) _bstr_t(var);

		//ask for the actual filter
		CComPtr< IBaseFilter > ppCap;
		hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**) &ppCap);
		if (ppCap) {
			break;
		}
	}

	return defaultDevice;
}

WebcamErrorCode DirectXWebcamDriver::setDevice(const std::string & deviceName) {
	//TODO: test if a webcam is already open

	//TODO: refactor the COM initialization phase to avoid
	//multiple initalisations and better handle unitialization
	//cf trac ticket #1008

	// We really need to refactor that point
	// I leave this line here just because the phapi thread
	// must call this function (one time). We must move this elsewhere ...
	CoInitialize(NULL);
	////

	_pGraph.CoCreateInstance(CLSID_FilterGraph);
	if (!_pGraph) {
		LOG_ERROR("failed to create Graph builder");
		return WEBCAM_NOK;
	}

	_pBuild.CoCreateInstance(CLSID_CaptureGraphBuilder2);
	if (!_pBuild) {
		LOG_ERROR("failed to create Capture Graph builder");
		return WEBCAM_NOK;
	}

	_pBuild->SetFiltergraph(_pGraph);
	//Create the Sample Grabber
	HRESULT hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&(_pGrabberF));
	if (hr != S_OK) {
		LOG_ERROR("failed to create COM instance");
		return WEBCAM_NOK;
	}

	//Add the filter to the graph
	hr = (_pGraph)->AddFilter(_pGrabberF, L"Sample Grabber");
	if (hr != S_OK) {
		LOG_ERROR("failed to add filter");
		return WEBCAM_NOK;
	}

	//Query the Sample Grabber for the ISampleGrabber interface.
	_pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&_pGrabber);
	hr = _pGrabber->SetBufferSamples(FALSE);
	hr = _pGrabber->SetOneShot(FALSE);

	//Set the Sample Grabber callback
	//0: SampleCB (the buffer is the original buffer, not a copy)
	//1: BufferCB (the buffer is a copy of the original buffer)
	if (_pGrabber->SetCallback(this, 0) != S_OK) {
		LOG_ERROR("failed to assign callback");
		return WEBCAM_NOK;
	}

	CComBSTR bstrName(deviceName.c_str());
	hr = FindMyCaptureDevice(&_pCap, bstrName);
	if ((hr != S_OK) || !_pCap) {
		return WEBCAM_NOK;
	}

	//initialize IAMStreamConfig
	_iam = GetIAMStreamConfig(_pCap);
	if (!_iam) {
		return WEBCAM_NOK;
	}

	//add the capture filter to the graph
	hr = (_pGraph)->AddFilter(_pCap, L"");
	if (hr != S_OK) {
		LOG_ERROR("failed to add filter");
		return WEBCAM_NOK;
	}

	//Add a null renderer filter
	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&_pNull);
	hr = (_pGraph)->AddFilter(_pNull, L"NullRender");

	_pBuild->RenderStream(&PIN_CATEGORY_CAPTURE, NULL, _pCap, NULL, _pGrabberF);

	//try to assign some palette until the webcam supports it
	if (setCaps(PIX_OSI_YUV420P, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_YUV420P, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_I420, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_I420, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB32, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB32, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB24, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB24, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_YUV422, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_YUV422, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB565, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB565, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB555, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_RGB555, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_YUY2, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_YUY2, _cachedFPS, 160, 120) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_UYVY, _cachedFPS, 176, 144) != WEBCAM_OK) {
	if (setCaps(PIX_OSI_UYVY, _cachedFPS, 160, 120) != WEBCAM_OK) {
	}}}}}}}}}}}}}}}}}}

	readCaps();

	if (getPalette() == PIX_OSI_UNSUPPORTED) {
		return WEBCAM_NOK;
	}

	_isOpen = true;

	return WEBCAM_OK;
}

bool DirectXWebcamDriver::isOpen() const {
	return _isOpen;
}

void DirectXWebcamDriver::startCapture() {
	HRESULT hr;
	hr = _pGraph->QueryInterface(IID_IMediaControl, (void **)&_pControl);
	if (hr != S_OK) {
		LOG_ERROR("Could not get _pControl MediaControl");
	}

	hr = _pControl->Run();
	if (hr != S_OK) {
		LOG_ERROR("Could not run graph");
		return;
	}

	SAFE_RELEASE(_pControl);
}

void DirectXWebcamDriver::pauseCapture() {
}

void DirectXWebcamDriver::stopCapture() {
	HRESULT hr;

	if (!_pGraph) {
		LOG_WARN("_pGraph is NULL");
		return;
	}

	hr = _pGraph->QueryInterface(IID_IMediaControl, (void **)&_pControl);
	if (hr != S_OK) {
		LOG_ERROR("Could not get _pControl MediaControl");
	}

	hr = _pControl->StopWhenReady();
	if (hr != S_OK) {
		LOG_ERROR("Could not stop capture");
	}

	SAFE_RELEASE(_pControl);
}

WebcamErrorCode DirectXWebcamDriver::setPalette(pixosi palette) {
	setCaps(palette, getFPS(), getWidth(), getHeight());
	if (_cachedPalette != palette) {
		return WEBCAM_NOK;
	} else {
		return WEBCAM_OK;
	}
}

pixosi DirectXWebcamDriver::getPalette() const {
	return _cachedPalette;
}

WebcamErrorCode DirectXWebcamDriver::setFPS(unsigned fps) {
	setCaps(getPalette(), fps, getWidth(), getHeight());

	//TODO: check if fps is correctly set
	return WEBCAM_NOK;
}

unsigned DirectXWebcamDriver::getFPS() const {
	return _cachedFPS;
}

WebcamErrorCode DirectXWebcamDriver::setResolution(unsigned width, unsigned height) {
	setCaps(getPalette(), getFPS(), width, height);
	if ((_cachedWidth != width) || (_cachedHeight != height)) {
		return WEBCAM_NOK;
	} else {
		return WEBCAM_OK;
	}
}


unsigned DirectXWebcamDriver::getWidth() const {
	return _cachedWidth;
}

unsigned DirectXWebcamDriver::getHeight() const {
	return _cachedHeight;
}

void DirectXWebcamDriver::setBrightness(int brightness) {
}

int DirectXWebcamDriver::getBrightness() const {
	return 0;
}

void DirectXWebcamDriver::setContrast(int contrast) {
}

int DirectXWebcamDriver::getContrast() const {
	return 0;
}

void DirectXWebcamDriver::flipHorizontally(bool flip) {
	//TODO: add horizontal flip support
}

WebcamErrorCode DirectXWebcamDriver::setCaps(pixosi palette, unsigned fps, unsigned resolutionWidth, unsigned resolutionHeight) {
	_cachedFPS = fps;

	if (!isOpen()) {
		return WEBCAM_NOK;
	}

	if (!_pCap) {
		LOG_FATAL("webcam not initialized");
	}
	if (!_iam) {
		LOG_FATAL("webcam not initialized");
	}

	int iCount, iSize;
	HRESULT hr = _iam->GetNumberOfCapabilities(&iCount, &iSize);

	VIDEO_STREAM_CONFIG_CAPS scc;
	if (sizeof(scc) != iSize) {
		LOG_ERROR("wrong config structure");
		return WEBCAM_NOK;
	}

	for (int i = 0; i < iCount; i++) {
		AM_MEDIA_TYPE * pmt = NULL;
		hr = _iam->GetStreamCaps(i, &pmt, reinterpret_cast<BYTE *>(&scc));
		if (hr == S_OK) {
			pixosi wc_palette = pix_directx_to_pix_osi(pmt->subtype);
			if (wc_palette != palette) {
				hr = E_FAIL;
				continue;
			}
			VIDEOINFOHEADER * pvi = (VIDEOINFOHEADER *) pmt->pbFormat;
			pvi->bmiHeader.biWidth = resolutionWidth;
			pvi->bmiHeader.biHeight = resolutionHeight;
			pvi->AvgTimePerFrame = (LONGLONG) (10000000. / (double)fps);
			hr = _iam->SetFormat(pmt);
			if (hr != S_OK) {
				hr = E_FAIL;
				continue;
			} else {
				LOG_DEBUG("assigned caps : ("
					+ String::fromNumber(palette)
					+ "," + String::fromNumber(fps)
					+ "," + String::fromNumber(resolutionWidth)
					+ "," + String::fromNumber(resolutionHeight)
					+ ")");
				break;
			}
		}
	}

	readCaps();

	if (hr == S_OK) {
		return WEBCAM_OK;
	}

	LOG_ERROR("failed caps request: ("
		+ String::fromNumber(palette)
		+ "," + String::fromNumber(fps)
		+ "," + String::fromNumber(resolutionWidth)
		+ "," + String::fromNumber(resolutionHeight)
		+ ")");

	return WEBCAM_NOK;
}

void DirectXWebcamDriver::readCaps() {
	VIDEOINFOHEADER * pvi;
	pixosi palette;

	AM_MEDIA_TYPE * pmt = NULL;
	HRESULT hr = _iam->GetFormat(&pmt);

	if (pmt->formattype == FORMAT_VideoInfo) {
		pvi = (VIDEOINFOHEADER *) pmt->pbFormat;
		palette = pix_directx_to_pix_osi(pmt->subtype);
	}

	_cachedPalette = palette;
	_cachedWidth = pvi->bmiHeader.biWidth;
	_cachedHeight = pvi->bmiHeader.biHeight;
}

STDMETHODIMP DirectXWebcamDriver::QueryInterface(REFIID riid, void ** ppv) {
	LOG_DEBUG("CSampleGrabberCB::QueryInterface");

	if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) {
		*ppv = (void *) static_cast<ISampleGrabberCB*> (this);
		return NOERROR;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP DirectXWebcamDriver::BufferCB(double dblSampleTime, BYTE * pBuffer, long lBufferSize) {
	//This method is not used but must be implemented
	LOG_ERROR("this method should not be called");
	return 0;
}

STDMETHODIMP DirectXWebcamDriver::SampleCB(double SampleTime, IMediaSample * pSample) {
	if (!pSample) {
		return E_POINTER;
	} else {
		BYTE * pBuffer;
		pSample->GetPointer(&pBuffer);
		long lBufferSize = pSample->GetSize();

		if (!pBuffer) {
			return E_POINTER;
		}

		_capturedImage.data = (uint8_t *) pBuffer;
		_capturedImage.width = _cachedWidth;
		_capturedImage.height = _cachedHeight;
		_capturedImage.palette = _cachedPalette;

		_webcamDriver->frameBufferAvailable(&_capturedImage);
	}
	return 0;
}

/************************************************************************///Add by zhuang 08-12-23
/*获得视频帧的width，height，bitrate                                    */
void DirectXWebcamDriver::getCodecParameter(const std::string & deviceName,int &width,int &height,int &bitrate,std::string & codecSerial)
{
	CComPtr< IGraphBuilder > _pFilterGraph;
	CComPtr< IBaseFilter > _pCaptureFilter;
	HRESULT hr;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	
	_pFilterGraph.CoCreateInstance(CLSID_FilterGraph);
	if (!_pFilterGraph) {
		LOG_ERROR("failed to create Graph builder");
		return;
	}
	if (deviceName=="虚拟摄像头")
	{
		//高清MPEG2
		codecSerial = "MPEG2/XVID";
		return;
	}

	CComBSTR bstrName(deviceName.c_str());
	FindMyCaptureDevice(&_pCaptureFilter, bstrName);
	if (!_pCaptureFilter) {
		LOG_ERROR("failed to create Capture Filter");
		return;
	}
	_pCaptureFilter->EnumPins(&pEnum);

	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PINDIR_OUTPUT)
		{
			break;
		}
		pPin->Release();
	}
	pEnum->Release();

	if(pPin)
	{
		IAMStreamConfig *pCfg=0;
		pPin->QueryInterface(IID_IAMStreamConfig, (void **)&pCfg);

		AM_MEDIA_TYPE *pmt={0};
		pCfg->GetFormat(&pmt);

		VIDEOINFOHEADER * pvi = (VIDEOINFOHEADER *) pmt->pbFormat;
		width = (int)pvi->bmiHeader.biWidth;
		height = (int)pvi->bmiHeader.biHeight;
		bitrate = (int)pvi->dwBitRate;

		/*判断媒体类型，Mpeg4或者Mpeg2*/
		if(pmt->majortype==MEDIATYPE_Video) 
		{
			if (pmt->subtype==MEDIASUBTYPE_MPEG2_VIDEO)//修改于2009-2-23，用于发送ts后缀的文件
			{
				//高清MPEG2
				codecSerial = "MPEG2/XVID";
			}
			else
			{
				//MPEG4
				codecSerial = "XVID/MPEG2";
			}
		}
		if(pmt->majortype==MEDIATYPE_Stream) 
		{
			//高清MPEG2
			codecSerial = "MPEG2/XVID";
		}

		pPin->Release();
	}
	SAFE_RELEASE(_pCaptureFilter);
	SAFE_RELEASE(_pFilterGraph);
}
/************************************************************************/

//*************************************************************************add by zhenHua.sun 2010-07-15
/**
* @brief 用于获取Crossbar设备列表。通常高清才需要用到
*/
StringList DirectXWebcamDriver::getCrossbarDeviceList( )
{
	StringList deviceList;
	deviceList += "";		//加入一个空值，表示不选择任何crossbar

	int numOfDevice = 0;
	std::wstring deviceCaptureList[20];
	HRESULT result = ListCaptureDevice( AM_KSCATEGORY_CROSSBAR , deviceCaptureList , numOfDevice );
	if( FAILED( result ) )
		return deviceList;

	for( int deviceCount=0 ; deviceCount<numOfDevice ; deviceCount+=2 )
	{
		string friendlyName = wstr2str( deviceCaptureList[deviceCount] );
		deviceList += friendlyName;		//friendly name
		//deviceCaptureList[deviceCount+1] is device name
	}

	return deviceList;

}

/**
* @brief 用于获取指定crossbar的输入类型
* @return crossbar支持的媒体类型
*/
StringList DirectXWebcamDriver::getCrossbarInputType( const std::string& crossbarName )
{
	StringList inputTypeList;
	
	//转换为宽字符串
	std::wstring w_crossbarName = str2wstr( crossbarName );
	int numOfDevice = 0;
	std::wstring deviceCaptureList[20];
	HRESULT result = ListCaptureDevice( AM_KSCATEGORY_CROSSBAR , deviceCaptureList , numOfDevice );
	if( FAILED( result ) )
		return inputTypeList;

	for( int deviceCount=0 ; deviceCount<numOfDevice ; deviceCount+=2 )
	{
		if( wcscmp( w_crossbarName.c_str() , deviceCaptureList[deviceCount].c_str() )==0 )
		{
			IMoniker *pMoniker=NULL;
			IBaseFilter *m_pCrossbarFilter=0;
			IBindCtx *pBindCtx;
			HRESULT hr=CreateBindCtx(0,&pBindCtx);
			ULONG chEaten=0;
			hr= MkParseDisplayName(pBindCtx,deviceCaptureList[deviceCount+1].c_str(),&chEaten,&pMoniker);
			pBindCtx->Release();
			if(FAILED(hr))
			{
				return  inputTypeList;
			}
			pMoniker->AddRef();
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pCrossbarFilter);
			if (m_pCrossbarFilter == NULL) 
			{
				pMoniker->Release();
				return  inputTypeList;
			}

			//获取crossbar filter的输入针脚
			IAMCrossbar *m_pCrossBar;
			hr=m_pCrossbarFilter->QueryInterface(IID_IAMCrossbar,(void   **)&m_pCrossBar);
			if( FAILED(hr) )
			{
				pMoniker->Release();
				m_pCrossbarFilter->Release();
				return inputTypeList;
			}

			long outCount = -1; //输出针脚计数
			long inCount = -1; //输入针脚计数
			m_pCrossBar->get_PinCounts(&outCount,&inCount);

			for(int i=0;i<inCount;i++)
			{
				long index = 0;
				//pinType保存的是输入pin的物理类型,如Svideo等
				long pinType;		

				m_pCrossBar->get_CrossbarPinInfo(TRUE,i,&index,&pinType );
				switch( pinType )
				{
				case PhysConn_Video_Composite:
					inputTypeList += "Composite";
					break;
				case PhysConn_Video_SVideo:
					inputTypeList += "SVideo";
					break;
				case PhysConn_Video_Tuner:
					inputTypeList += "Tuner";
					break;
				}
			}

			//释放资源
			pMoniker->Release();
			m_pCrossbarFilter->Release();
			m_pCrossBar->Release();
			break;
		}
	}

	return inputTypeList;
}

StringList DirectXWebcamDriver::getVideoDeviceMediaType( const std::string &deviceName )
{
	StringList mediaTypeList;
	char media_type_info[56];
	HRESULT hr;
	
	IBaseFilter*  pCaptureDevice = NULL;
	CComBSTR bstrName(deviceName.c_str());
	hr = FindMyCaptureDevice(&pCaptureDevice, bstrName);
	if ((hr != S_OK) || !pCaptureDevice) {
		return mediaTypeList;
	}

	//initialize IAMStreamConfig
	_iam = GetIAMStreamConfig(pCaptureDevice);
	if (!_iam) {
		return mediaTypeList;
	}

	int iCount, iSize;
	hr = _iam->GetNumberOfCapabilities(&iCount, &iSize);

	VIDEO_STREAM_CONFIG_CAPS scc;
	if (sizeof(scc) != iSize) {
		LOG_ERROR("wrong config structure");
		return mediaTypeList;
	}

	for (int i = 0; i < iCount; i++) {
		AM_MEDIA_TYPE * pmt = NULL;
		hr = _iam->GetStreamCaps(i, &pmt, reinterpret_cast<BYTE *>(&scc));
		if (hr == S_OK) {

			VIDEOINFOHEADER * pvi = (VIDEOINFOHEADER *) pmt->pbFormat;

			if( pmt->subtype == MEDIASUBTYPE_YUY2 || pmt->subtype==MEDIASUBTYPE_RGB24 )
			{
				if( pvi->bmiHeader.biWidth<300 || pvi->bmiHeader.biHeight<200 )
					continue;
				
				sprintf( media_type_info , "%dX%d", pvi->bmiHeader.biWidth , pvi->bmiHeader.biHeight );
				std::string mediaTypeInfo = media_type_info;

				if( !mediaTypeList.contains( mediaTypeInfo ) )
					mediaTypeList += mediaTypeInfo;
			}
		}
	}

	pCaptureDevice->Release();

	return mediaTypeList;
}
//*************************************