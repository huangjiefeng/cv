#include "stdafx.h"

#include <string>

#include "Exportutil.h"
#include "IGraphController.h"
#include "intel\mfxvideo.h" //intel sdk, for hw check
#include "intel\mfxvideo++.h"
#include "util.h"
#include "LogUtil.h"

using namespace std;


extern msdx::SendGraphParam send_graph_param; //这个定义在ExportUtil_SendGraph.cpp中
extern msdx::RecvGraphParam recv_graph_param; //这个定义在ExportUtil_RecvGraph.cpp中

wstring  AudioDeviceList[20];
int NumberOfAudioDevice = 0;

wstring AudioRenderDeviceList[50];
int NumberOfAudioRenderDevice = 0;

wstring  VideoDeviceList[20];
int NumberOfVideoDevice = 0;

//for 视频采集卡crossbar add by chenjunlin  2010-7-8
wstring Crossbar[20];
int NumberOfCrossbar = 0;


HRESULT ListDevicesByCategory(const CLSID & clsidDeviceClass, wstring * pWStringArray, int & NumOfDevices)
{
    //设备枚举器Interface
    ICreateDevEnum *pDevEnum = NULL;

    //名称枚举Interface
    IEnumMoniker *pEnum = NULL;

    // Create the System Device Enumerator.
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
        CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
        reinterpret_cast<void**>(&pDevEnum)); //创建设备枚举COM对象

    if (SUCCEEDED(hr))
    {
        // Create an enumerator for the video capture category.
        hr = pDevEnum->CreateClassEnumerator(
            clsidDeviceClass,	//CLSID_VideoInputDeviceCategory or CLSID_AudioInputDeviceCategory
            &pEnum, 0); //创建视频采集设备枚举COM对象
    }
    else if (pDevEnum==NULL)
    {
        //创建失败
        return E_FAIL; //如果没有设备，返回
    }

    NumOfDevices = 0;

    IMoniker *pMoniker = NULL;
    if(pEnum == NULL )
    {
        pDevEnum->Release();
        return E_FAIL; //如果没有设备，返回
    }

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

            pWStringArray[NumOfDevices++] = varName.bstrVal; //varName.bstrVal;
            VariantClear(&varName);

            WCHAR * wszDiaplayName = NULL;
            pMoniker->GetDisplayName(NULL, NULL, &wszDiaplayName);
            pWStringArray[NumOfDevices++] = wszDiaplayName;
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

HRESULT  ListAudioCaptureDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_AudioInputDeviceCategory, pWStringArray, NumOfDevices);
}

HRESULT  ListAudioRenderDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_AudioRendererCategory, pWStringArray, NumOfDevices);
}

HRESULT   ListVideoCaptureDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_VideoInputDeviceCategory, pWStringArray, NumOfDevices);
}

HRESULT  ListVideoCrossbar(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(AM_KSCATEGORY_CROSSBAR, pWStringArray, NumOfDevices);
}


//这函数尼玛居然是发送和接收共用的，太恶心了，也只能将就下了
void MSDXAPI msdx_config_set_audio_device(
    const char* input_device , //声音输入设备
    const char* output_device) //声音输出设备，回声对消也用
{
    if (input_device) send_graph_param.audio.input_device = input_device;
    if (output_device) send_graph_param.audio.output_device = output_device;
    if (output_device) recv_graph_param.audio.output_device = output_device;
}

//判断视频编解码是否支持硬件加速  2012.10.11
bool MSDXAPI msdx_check_hardware_acceleration_support()
{
    static bool checked = false;
    static mfxStatus checkResult = MFX_ERR_NONE;

    //Ruan 2012/09/28 :首次调用本函数时进行硬编码支持测试，此后均返回首次测试的结果
    if(checked == false)
    {
        mfxSession session = (mfxSession) 0;
        mfxIMPL impl = MFX_IMPL_HARDWARE;
        mfxU16 APIVerMinor = 1, APIVerMajor = 1;
        mfxVersion version = {APIVerMinor, APIVerMajor};
        checkResult = MFXInit(impl, &version, &session);	//使用硬编码格式初始化环境,如果不支持硬编码则初始化失败
        MFXClose(session);
        session = (mfxSession) 0;
        checked = true;
    }

    if(checkResult == MFX_ERR_NONE)
    {
        LOG_PRINTF("HWCheck: The result is true");
        return true;
    }
    else
    {
        LOG_PRINTF("HWCheck: The result is false");
        return false;
    }
}

int GetAudioSourceDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name)
{
    HRESULT hr = ListAudioCaptureDevice(AudioDeviceList, NumberOfAudioDevice);
    CHK_FAIL_MSG_RETURN(FAILED(hr), -1, "Enum audio capture devices failed.");

    //先找到设定的设备的DisplayName
    std::wstring display_name_w = AudioDeviceList[1]; //找不到可以用默认设备
    for (int i = 0; i < NumberOfAudioDevice; i += 2) 
    {
        if (AudioDeviceList[i] == Util::str2wstr(friendly_name))
        {
            display_name_w = AudioDeviceList[i+1]; //偶数为FriendlyName，奇数索引为对应DisplayName
            break;
        }
    }
    display_name = Util::wstr2str(display_name_w);
    //是不是被上面宽字节转换来转换去弄晕了？别怪我，以前师兄用的宽字节，不方便……

    LOG_PRINTF("Audio source: %s \n\tDevice displayName: %s\n", 
        friendly_name.c_str(), display_name.c_str());

    return 0;
}

int GetAudioRendererDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name)
{
    CHK_FAIL_MSG_RETURN(friendly_name.empty(), -1, "Null audio renderer device friendlyName");

    HRESULT hr = ListAudioRenderDevice(AudioRenderDeviceList, NumberOfAudioRenderDevice);
    CHK_FAIL_MSG_RETURN(FAILED(hr), -1, "Get audio renderer devices failed: 0x%08x", hr);
    CHK_FAIL_MSG_RETURN(NumberOfAudioRenderDevice == 0, -1, "System has no audio renderer devices.");

    //取得波形设备对应的DirectSound设备名，根据MSDN，只要加上"DirectSound: "(注意冒号后有空格)前缀即可
    const wchar_t *directsound_prefix = L"DirectSound: ";
    std::wstring audio_renderer_friendly_name = Util::str2wstr(friendly_name);
    std::wstring audio_renderer_display_name;
    bool fuzzy_match = false; //是否模糊匹配

    if (audio_renderer_friendly_name.find(directsound_prefix) != 0) {
        //不以"DirectSound: "开头的不是DirectSound设备，尝试构建对应的DirectSound设备
        //下面名称匹配时有个问题，波形设备名不能超过31字节，可能是不完整的，例如：
        //  波形设备名：Headphone (VIA HD Audio(Compati
        //  对应DirectSound设备名：DirectSound: Headphone (VIA HD Audio(Compatible))
        //因此上面构建的名称可能不完整，只做前半部分匹配，将fuzzy_match设为true
        audio_renderer_friendly_name = std::wstring(directsound_prefix) + audio_renderer_friendly_name;
        fuzzy_match = true;
    }

    for (int i = 0; i < NumberOfAudioRenderDevice; i += 2) 
    {
        if ((fuzzy_match && AudioRenderDeviceList[i].find(audio_renderer_friendly_name) == 0) ||
            AudioRenderDeviceList[i] == audio_renderer_friendly_name)
        {
            audio_renderer_display_name = AudioRenderDeviceList[i+1]; //奇数索引为DisplayName，偶数为FriendlyName
            break;
        }
    }

    CHK_FAIL_MSG_RETURN(audio_renderer_display_name.empty(), -1, 
      L"Audio renderer device not exist: %s", audio_renderer_display_name.c_str());

    LOG_PRINTF(L"Audio renderer: %s \n\tCorresponding DirectSound device: %s \n\tDirectSound device displayName: %s\n", 
        Util::str2wstr(friendly_name).c_str(), 
        audio_renderer_friendly_name.c_str(), audio_renderer_display_name.c_str());

    display_name = Util::wstr2str(audio_renderer_display_name);
    return 0;
}

