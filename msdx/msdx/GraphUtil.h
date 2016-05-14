/************************************************************************/
/* 此处定义graph相关的工具方法                                          */
/************************************************************************/

#ifndef GRAPH_UTIL_H
#define GRAPH_UTIL_H

#include <atlbase.h>
#include <DShow.h>

#include "DxDefinitions.h"


#define SAFE_DELETE_PTR(ptr) \
    if (ptr) { delete (ptr); (ptr) = nullptr; }
    


namespace msdx
{

//graph工具类
class GraphUtil
{
private:
    GraphUtil(void);
    ~GraphUtil(void);

public:
    static HRESULT GetFilterByCLSID(const CLSID &clsid, IBaseFilter **ppF);
	//将三种名字查找filter的方法合为一个--HJF
	static HRESULT getFilterByVariousName(const CLSID & clsidDeviceClass, const int type,LPCWSTR wszName, IBaseFilter **ppF);
    //Set wszFrendlyName to be NULL to get the Default Device
    static HRESULT GetFilterByFriendlyName(const CLSID &clsidDeviceClass, LPCWSTR wszFrendlyName, IBaseFilter **ppF);
    static HRESULT GetFilterByDisplayName(LPCWSTR wszDisplayName, IBaseFilter **ppF);
    //Device path is just a part of display name
    static HRESULT GetFilterByDevicePath(const CLSID &clsidDeviceClass, LPCWSTR wszDevicePath, IBaseFilter **ppF);
    static HRESULT AddFilterByCLSID(IGraphBuilder *graph, LPCWSTR wszName, const CLSID &clsid, IBaseFilter **ppF);
    static HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
    static HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSource, IBaseFilter *pDest);
    static HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pPinOut, IBaseFilter *pDest);

#ifdef _DEBUG
    //Debug工具，添加到graph editor跟踪
    static HRESULT AddToRot(LPCWSTR wszDescription, IUnknown *pUnkGraph, DWORD *pdwRegister);
    static void RemoveFromRot(DWORD pdwRegister);
#endif // DEBUG

public:

    //GUID
    //video
    static const CLSID clsid_intel_h264_enc;
    static const CLSID clsid_intel_h264_dec;
    static const CLSID clsid_scut_rgb2yuv;
    static const CLSID clsid_scut_h264_enc;
    static const CLSID clsid_scut_h264_dec;
    //audio
    static const CLSID clsid_scut_aec_source;
    static const CLSID clsid_scut_speex_enc;
    static const CLSID clsid_scut_speex_dec;
    static const CLSID clsid_monogram_aac_enc;
    static const CLSID clsid_monogram_aac_dec;
    //new rtp
    static const CLSID clsid_scut_rtp_sender;
    static const CLSID clsid_scut_rtp_receiver;
    //old rtp
    static const CLSID clsid_scut_rtp_out_v;
    static const CLSID clsid_scut_rtp_out_a;
    static const CLSID clsid_scut_rtp_in_v;
    static const CLSID clsid_scut_rtp_in_a;
    //record
    static const CLSID clsid_gdcl_mp4_muxer;
    //file play
    static const CLSID clsid_gdcl_mp4_demuxer;
    static const CLSID clsid_scut_play_ctrl;
    //other
    static const CLSID clsid_null_renderer;
    static const CLSID clsid_inf_pin_tee;
    static const CLSID clsid_scut_multi_smart_tee;
    static const CLSID clsid_file_source;
	//ffdshow
	static const CLSID clsid_ffdshow_encoder;
	static const CLSID clsid_ffdshow_decoder;

	//lead
	static const CLSID clsid_lead_h264_encoder;
	static const CLSID clsid_lead_CSconverter;
	//render
	static const CLSID clsid_render;

};

}


#endif
