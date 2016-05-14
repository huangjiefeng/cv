//此文件包含编解码有关的filter实现

#include <atlbase.h>
#include <assert.h>
#include <stdio.h> //sprintf_s

#include "InterfaceDefination.h"
#include "FilterContainer.h"
#include "GraphUtil.h"
#include "LogUtil.h"
#include "Util.h"
#include "3rd/mfx_filter_externals.h"
#include "AACUtil.h"
#include "3rd/faac/aac_types.h"

using namespace msdx;


//Intel H264 container

IntelH264EncoderContainer::IntelH264EncoderContainer()
{
}

IntelH264EncoderContainer::~IntelH264EncoderContainer()
{
    if (graph_builder_ && video_encoder_filter_)
    {
        graph_builder_->RemoveFilter(video_encoder_filter_);
    }
}

LeadH264EncoderContainer::~LeadH264EncoderContainer()
{
	if (graph_builder_ && video_encoder_filter_)
	{
		graph_builder_->RemoveFilter(video_encoder_filter_);
	}
}


msdx::dxStatus msdx::IntelH264EncoderContainer::AddToGraph( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt )
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect encoder to null camera.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video encoder to null graph.");
    graph_builder_ = owner_graph;

    //不要这么严格检查了，还有些Intel自己的格式也是支持的
    /*if (mt.subtype != MEDIASUBTYPE_YUY2 && mt.subtype != MEDIASUBTYPE_RGB24)
    {
        LOG_ERROR("Video format not supported by Intel H264 encoder.");
        return DX_ERR_UNSUPPORT;
    }*/

    HRESULT hr = S_OK;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Intel H.264 Enc", 
        GraphUtil::clsid_intel_h264_enc, &video_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add intel_h264_enc filter to graph failed: 0x%08x", hr);

    if (mt.subtype == MEDIASUBTYPE_RGB24)
    {
        LOG_PRINTF("Camera format is RGB24, rgb2yuv filter needed.");
        hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"rgb2yuv", 
            GraphUtil::clsid_scut_rgb2yuv, &video_converter_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
            "Add rgb2yuv filter to graph failed: 0x%08x", hr);

        hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, video_converter_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect camera to rgb2yuv failed.");

        hr = GraphUtil::ConnectFilters(graph_builder_, video_converter_filter_, video_encoder_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect rgb2yuv to intel_h264_enc failed.");
    }
    else
    {
        hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, video_encoder_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect camera to intel_h264_enc failed.");
    }

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::IntelH264EncoderContainer::Config( EncoderParam &param )
{
    if (!video_encoder_filter_)
    {
        LOG_ERROR("Try to config null intel_h264_enc.");
        return DX_ERR_NULL_PTR;
    }

    CComQIPtr<IConfigureVideoEncoder> encoder_config = video_encoder_filter_;
    if (encoder_config)
    {
        IConfigureVideoEncoder::Params params;
        HRESULT hr = encoder_config->GetParams(&params);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Get intel h264 video encoder default config failed.");

        params.profile_idc = IConfigureVideoEncoder::Params::PF_H264_BASELINE;
        params.level_idc = IConfigureVideoEncoder::Params::LL_5;
        params.ps_control.GopPicSize = 64;
        params.ps_control.GopRefDist = 1;
        params.frame_control.width  = param.width;
        params.frame_control.height = param.height;
        params.rc_control.rc_method = IConfigureVideoEncoder::Params::RCControl::RC_VBR;
        params.rc_control.bitrate = param.target_kbps;

        hr = encoder_config->SetParams(&params);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, 
            "Config intel h264 video encoder failed.");
    }
    else
    {
        LOG_ERROR("Video encoder has no IConfigureVideoEncoder.");
        return DX_ERR_BAD_INTERFACE;
    }
    return DX_ERR_NONE;
}

dxStatus IntelH264EncoderContainer::PauseEncoding(bool pause)
{
    CHK_FAIL_MSG_RETURN(!video_encoder_filter_, DX_ERR_NULL_PTR, 
        "Stop video encoder failed: null encoder.");

    CComQIPtr<HWVideoEncoder> encoder_config = video_encoder_filter_;
    if (encoder_config)
    {
        encoder_config->setEncoderState(!pause);
        LOG_INFO("%s intel_h264_encoder.", (pause ? "Pause" : "Resume"));
    }
    else
    {
        LOG_ERROR("Stop video encoder failed: cannot query HWVideoEncoder");
        return DX_ERR_BAD_INTERFACE;
    }

    return DX_ERR_NONE;
}


//ScutH264EncoderContainer

msdx::ScutH264EncoderContainer::~ScutH264EncoderContainer()
{
    if (graph_builder_ && video_encoder_filter_)
    {
        graph_builder_->RemoveFilter(video_encoder_filter_);
    }
}

msdx::dxStatus msdx::ScutH264EncoderContainer::AddToGraph( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt )
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect encoder to null camera.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video encoder to null graph.");
    graph_builder_ = owner_graph;

    HRESULT hr = S_OK;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"SCUT H.264 Enc", 
        GraphUtil::clsid_scut_h264_enc, &video_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add scut_h264_enc filter failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, video_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect camera to rgb2yuv failed.");

    return DX_ERR_NONE;
}

msdx::dxStatus msdx::ScutH264EncoderContainer::Config( EncoderParam &param )
{
    if (!video_encoder_filter_)
    {
        LOG_ERROR("Try to config null scut_h264_enc.");
        return DX_ERR_NULL_PTR;
    }

    CComQIPtr<ISCUTVideoEncoder> encoder_config = video_encoder_filter_;
    if (!encoder_config) 
    {
        LOG_ERROR("Video encoder has no ISCUTVideoEncoder.");
        return DX_ERR_BAD_INTERFACE;
    }

    if (param.width > 0 && param.height > 0) 
    {
        encoder_config->setDestSize(param.width, param.height);
    }

    return DX_ERR_NONE;
}

dxStatus ScutH264EncoderContainer::PauseEncoding(bool pause)
{
    CHK_FAIL_MSG_RETURN(!video_encoder_filter_, DX_ERR_NULL_PTR, 
        "Stop video encoder failed: null encoder.");

    CComQIPtr<ISCUTVideoEncoder> encoder_config = video_encoder_filter_;
    if (encoder_config)
    {
        encoder_config->setEncoderState(!pause);
        LOG_INFO("%s scut_h264_encoder.", (pause ? "Pause" : "Resume"));
    }
    else
    {
        LOG_ERROR("Stop video encoder failed: cannot query ISCUTVideoEncoder");
        return DX_ERR_BAD_INTERFACE;
    }
    return DX_ERR_NONE;
}

FfdshowH264EncoderContainer::~FfdshowH264EncoderContainer()
{
	
		if (graph_builder_ && video_ffdshow_encoder_filter_)
		{
			graph_builder_->RemoveFilter(video_ffdshow_encoder_filter_);
		}
		if (graph_builder_ && video_ffdshow_decoder_filter_)
		{
			graph_builder_->RemoveFilter(video_ffdshow_decoder_filter_);
		}
}

dxStatus FfdshowH264EncoderContainer::AddToGraph(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt)
{
	CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect encoder to null camera.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video encoder to null graph.");
    graph_builder_ = owner_graph;

    //不要这么严格检查了，还有些Intel自己的格式也是支持的
    /*if (mt.subtype != MEDIASUBTYPE_YUY2 && mt.subtype != MEDIASUBTYPE_RGB24)
    {
        LOG_ERROR("Video format not supported by Intel H264 encoder.");
        return DX_ERR_UNSUPPORT;
    }*/

    HRESULT hr = S_OK;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Intel H.264 Enc", 
        GraphUtil::clsid_intel_h264_enc, &video_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add intel_h264_enc filter to graph failed: 0x%08x", hr);


        hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"ffdshow encoder", 
            GraphUtil::clsid_ffdshow_encoder, &video_ffdshow_encoder_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
            "Add ffdshow encoder filter to graph failed: 0x%08x", hr);

		hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"ffdshow decoder", 
			GraphUtil::clsid_ffdshow_decoder, &video_ffdshow_decoder_filter_);
		CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
			"Add ffdshow encoder filter to graph failed: 0x%08x", hr);

        hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, video_ffdshow_encoder_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect camera to ffdshow failed.");

        hr = GraphUtil::ConnectFilters(graph_builder_, video_ffdshow_encoder_filter_, video_ffdshow_decoder_filter_);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect ffdshow dec to ffdshow enc failed.");
		hr = GraphUtil::ConnectFilters(graph_builder_, video_ffdshow_decoder_filter_,video_encoder_filter_);
		CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect ffdshow to intel_h264_enc failed.");
    


    return DX_ERR_NONE;
}

dxStatus LeadH264EncoderContainer::AddToGraph(IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt)
{
	CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect encoder to null camera.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video encoder to null graph.");
    graph_builder_ = owner_graph;

    //不要这么严格检查了，还有些Intel自己的格式也是支持的
    /*if (mt.subtype != MEDIASUBTYPE_YUY2 && mt.subtype != MEDIASUBTYPE_RGB24)
    {
        LOG_ERROR("Video format not supported by Intel H264 encoder.");
        return DX_ERR_UNSUPPORT;
    }*/



    HRESULT hr = S_OK;

	hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Lead ColorSpace Converter", 
		GraphUtil::clsid_lead_CSconverter, &video_CSconverter_filter_);
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
		"Add leadCSconverter filter to graph failed: 0x%08x", hr);
	hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, video_CSconverter_filter_);
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Connect camera to lead failed.");

    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Intel H.264 Enc", 
        GraphUtil::clsid_intel_h264_enc, &video_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add Intel_h264_enc filter to graph failed: 0x%08x", hr);

	hr = GraphUtil::ConnectFilters(graph_builder_,video_CSconverter_filter_,video_encoder_filter_);
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
		"connect converter to encoder failed: 0x%08x", hr);



    return DX_ERR_NONE;
}
//IntelH264DecoderContainer

IntelH264DecoderContainer::~IntelH264DecoderContainer()
{
    if (graph_builder_ && video_decoder_filter_)
    {
        graph_builder_->RemoveFilter(video_decoder_filter_);
    }
}

msdx::dxStatus msdx::IntelH264DecoderContainer::AddFilter( 
    IGraphBuilder *owner_graph, IPin *upstream_pin )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add intel_h264_dec to null graph.");
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect intel_h264_dec to null pin.");

    HRESULT hr;
    hr = GraphUtil::AddFilterByCLSID(owner_graph, L"Intel H.264 Dec", 
        GraphUtil::clsid_intel_h264_dec, &video_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add intel_h264_dec to graph failed.");

    hr = GraphUtil::ConnectFilters(owner_graph, upstream_pin, video_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect intel_h264_dec failed.");

    return DX_ERR_NONE;
}


//ScutH264DecoderContainer

msdx::ScutH264DecoderContainer::~ScutH264DecoderContainer()
{
    if (graph_builder_ && video_decoder_filter_)
    {
        graph_builder_->RemoveFilter(video_decoder_filter_);
    }
}

msdx::dxStatus msdx::ScutH264DecoderContainer::AddFilter( IGraphBuilder *owner_graph, IPin *upstream_pin )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add scut_h264_dec to null graph.");
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect scut_h264_dec to null pin.");

    HRESULT hr;
    hr = GraphUtil::AddFilterByCLSID(owner_graph, L"Intel H.264 Dec", 
        GraphUtil::clsid_scut_h264_dec, &video_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add scut_h264_dec to graph failed.");

    hr = GraphUtil::ConnectFilters(owner_graph, upstream_pin, video_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect scut_h264_dec failed.");

    return DX_ERR_NONE;
}


//AACEncoderContainer

msdx::AACEncoderContainer::~AACEncoderContainer()
{
    if (graph_builder_ && audio_encoder_filter_)
    {
        graph_builder_->RemoveFilter(audio_encoder_filter_);
    }
}

msdx::dxStatus msdx::AACEncoderContainer::AddFilter( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt )
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect AAC encoder to null audio source.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add AAC encoder to null graph.");
    graph_builder_ = owner_graph;

    //check format
    if (mt.subtype != MEDIASUBTYPE_PCM)
    {
        LOG_ERROR("Non-PCM format not supported by AAC encoder.");
        return DX_ERR_UNSUPPORT;
    }
    //AAC支持更多音频格式，不作具体检查了

    //add audio encoder
    HRESULT hr = S_OK;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"AAC Enc", 
        GraphUtil::clsid_monogram_aac_enc, &audio_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add AAC Encoder filter to graph failed: 0x%08x", hr);

    //connect
    hr = GraphUtil::ConnectFilters(owner_graph, upstream_pin, audio_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Connect audio source to AAC encoder failed: 0x%08x", hr);

    //config
    //如果不强制设置一次，不能编码16kHz PCM
    CComQIPtr<IMonogramAACEncoder> aac_encoder_config = audio_encoder_filter_;
    if (aac_encoder_config)
    {
        AACConfig cfg = AACUtil::GetAACEncConfig();
        hr = aac_encoder_config->SetConfig(&cfg);
    }
    else
    {
        LOG_WARN("Query IMonogramAACEncoder of AAC encoder failed.");
    }

    return DX_ERR_NONE;
}


//AACDecoderContainer

AACDecoderContainer::~AACDecoderContainer()
{
    if (graph_builder_ && audio_decoder_filter_)
    {
        graph_builder_->RemoveFilter(audio_decoder_filter_);
    }
};

dxStatus AACDecoderContainer::AddFilter(
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt)
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Try to connect AAC Dec to null pin.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Try to add AAC Dec to null graph.");
    graph_builder_ = owner_graph;

    HRESULT hr;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"AAC Dec", 
        GraphUtil::clsid_monogram_aac_dec, &audio_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add AAC Dec failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, audio_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Connect rtp receiver to AAC Dec failed: 0x%08x", hr);

    return DX_ERR_NONE;
}


//SpeexEncoderContainer

SpeexEncoderContainer::~SpeexEncoderContainer()
{
    if (graph_builder_ && audio_encoder_filter_)
    {
        graph_builder_->RemoveFilter(audio_encoder_filter_);
    }
}

msdx::dxStatus msdx::SpeexEncoderContainer::AddFilter( 
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt)
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Connect speex encoder to null audio source.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add speex encoder to null graph.");
    graph_builder_ = owner_graph;

    //check format
    if (mt.subtype != MEDIASUBTYPE_PCM)
    {
        LOG_ERROR("Non-PCM format not supported by speex encoder.");
        return DX_ERR_UNSUPPORT;
    }

    WAVEFORMATEX * pWF = (WAVEFORMATEX *)mt.pbFormat;
    if ((pWF->nSamplesPerSec > 22500 || pWF->nSamplesPerSec < 8000) ||
        pWF->nChannels != 1 || pWF->wBitsPerSample != 16)
    {
        LOG_ERROR("Audio format not supported by speex: %dHz, %dchn, %dbits",
            pWF->nSamplesPerSec, pWF->nChannels, pWF->wBitsPerSample);
        return DX_ERR_UNSUPPORT;
    }

    //add audio encoder
    HRESULT hr = S_OK;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Speex Enc", 
        GraphUtil::clsid_scut_speex_enc, &audio_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add Speex Encoder filter to graph failed: 0x%08x", hr);

    //config - 先配置再连接，不然你会听到很奇怪的声音……
    CComQIPtr<SpeexEncIProp> pSpeexEncProp = audio_encoder_filter_;
    CHK_FAIL_MSG_RETURN(!pSpeexEncProp, DX_ERR_NULL_PTR, 
        "Query SpeexEncIProp of speex encoder failed.");

    hr = pSpeexEncProp->put_SampleRate(pWF->nSamplesPerSec / 1000);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Config speex encoder failed: 0x%08x", hr);

    //connect
    hr = GraphUtil::ConnectFilters(owner_graph, upstream_pin, audio_encoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Connect audio source to speex encoder failed: 0x%08x", hr);

    return DX_ERR_NONE;
}


//SpeexDecoderContainer

SpeexDecoderContainer::~SpeexDecoderContainer()
{
    if (graph_builder_ || audio_decoder_filter_)
    {
        graph_builder_->RemoveFilter(audio_decoder_filter_);
    }
}

dxStatus SpeexDecoderContainer::AddFilter(
    IGraphBuilder *owner_graph, IPin *upstream_pin, CMediaType mt)
{
    CHK_FAIL_MSG_RETURN(!upstream_pin, DX_ERR_NULL_PTR, "Try to connect speex_dec to null pin.");
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Try to add speex_dec to null graph.");
    graph_builder_ = owner_graph;

    HRESULT hr;
    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"Speex Dec", 
        GraphUtil::clsid_scut_speex_dec, &audio_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Add speex dec failed: 0x%08x", hr);

    hr = GraphUtil::ConnectFilters(graph_builder_, upstream_pin, audio_decoder_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, 
        "Connect rtp receiver to Speex Dec failed: 0x%08x", hr);

    //config
    CComQIPtr<SpeexDecIProp> speex_config = audio_decoder_filter_;
    CHK_FAIL_MSG_RETURN(!speex_config, DX_ERR_BAD_INTERFACE, "Speex dec has no SpeexDecIProp");

    WAVEFORMATEX *info = (WAVEFORMATEX *)mt.pbFormat;
    speex_config->put_SampleRate(info->nSamplesPerSec);

    return DX_ERR_NONE;
}
