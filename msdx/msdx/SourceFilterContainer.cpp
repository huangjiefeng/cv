//此文件包含音视频输入有关的filter实现

#include <atlbase.h>
#include <assert.h>
#include <dshow.h> //need by dvdmedia.h
#include <dvdmedia.h> //VIDEOINFOHEADER2
#include <stdio.h> //sprintf_s

#include "InterfaceDefination.h" //AEC
#include "FilterContainer.h"
#include "GraphUtil.h"
#include "LogUtil.h"
#include "Util.h"

using namespace msdx;


//crossbar container

CrossbarContainer::CrossbarContainer()
    : physical_type_(PhysConn_Auto)
{
}

CrossbarContainer::~CrossbarContainer()
{
    if (graph_builder_ && video_crossbar_filter_)
    {
        graph_builder_->RemoveFilter(video_crossbar_filter_);
    }
}

dxStatus CrossbarContainer::AddFilter( IGraphBuilder *owner_graph, std::string display_name )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add crossbar to null graph.");
    graph_builder_ = owner_graph;

    // Free previous added filter
    if (NULL != video_crossbar_filter_)
    {
        graph_builder_->RemoveFilter(video_crossbar_filter_);
        video_crossbar_filter_ = NULL; //release COM ptr
    }

    LOG_INFO("Add crossbar: %s", display_name.c_str());
    // Get filter
    HRESULT hr;
    hr = GraphUtil::GetFilterByDisplayName(
        Util::str2wstr(display_name).c_str(), 
        &video_crossbar_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_BAD_DEVICE, "Get corssbar failed. DisplayName=%s", 
        display_name.c_str());

    // Add filter to graph
    hr = graph_builder_->AddFilter(video_crossbar_filter_, L"CrossBar");
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Add corssbar to graph failed");

    return DX_ERR_NONE;
}

dxStatus CrossbarContainer::AddFilterAuto(
    ICaptureGraphBuilder2 *capture, IBaseFilter *source_filter)
{
    CHK_FAIL_MSG_RETURN(!source_filter, DX_ERR_NULL_PTR, "Cannot find crossbar by null source filter.");
    CHK_FAIL_MSG_RETURN(!capture, DX_ERR_NULL_PTR, "Add crossbar using null capture graph.");
    graph_builder_ = nullptr;
    capture->GetFiltergraph(&graph_builder_);
    CHK_FAIL_MSG_RETURN(!graph_builder_, DX_ERR_NULL_PTR, "Get null graph from capture graph.");

    LOG_INFO("Auto finding crossbar device.");

    //使用ICaptureGraphBuilder2可以不需要设备名自动寻找采集卡，但有多个相似采集卡时会悲剧
    //自动查找采集卡后就自动连好了，不要再调用下面的连接方法哦
    CComPtr<IAMCrossbar> pXBar1;
    HRESULT hr = capture->FindInterface(&LOOK_UPSTREAM_ONLY, NULL,
        source_filter, IID_IAMCrossbar, (void**)&pXBar1);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Find crossbar failed.");

    pXBar1->QueryInterface(IID_IBaseFilter, (void**)&video_crossbar_filter_);
    CHK_FAIL_MSG_RETURN(!video_crossbar_filter_, DX_ERR_SYS, "Get crossbar filter from IAMCrossbar failed.");

    physical_type_ = PhysConn_Auto;

    return DX_ERR_NONE;
}

dxStatus CrossbarContainer::SetPhysicalType(long type)
{
    physical_type_ = type;
    LOG_INFO("Set crossbar physical type: %s", Util::GetCrossbarPhysicalPinName(type));

    // Get type
    CComQIPtr<IAMCrossbar> pXBar = video_crossbar_filter_;
    CHK_FAIL_MSG_RETURN(!pXBar, DX_ERR_BAD_INTERFACE, "Get IAMCrossbar failed.");

    //不指定类型时不处理
    if (physical_type_ == PhysConn_Auto) return DX_ERR_NONE;

    HRESULT hr;
    long cOutput = -1, cInput = -1;
    hr = pXBar->get_PinCounts(&cOutput, &cInput);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get crossbar pin count failed.");

    CHK_FAIL_MSG_RETURN(physical_type_ >= cInput, DX_ERR_BAD_PARAM, 
        "Invalid crossbar input index %d.", physical_type_);

    for (long in_index = 0; in_index < cInput; ++in_index)
    {
        for (long out_index = 0; out_index < cOutput; ++out_index)
        {
            if(S_OK != pXBar->CanRoute(out_index, in_index)) continue;

            long cur_type = 0;
            long related_index = 0;
            pXBar->get_CrossbarPinInfo(TRUE, in_index, &related_index, &cur_type);
            if (cur_type == physical_type_ && S_OK == pXBar->Route(out_index, in_index))
            {
                LOG_INFO("Route crossbar successful: input %d to output %d", in_index, out_index);
                break;
            }
        }
    }

    LOG_ERROR("Route crossbar failed.");
    return DX_ERR_ABORT;
}

dxStatus CrossbarContainer::ConnectToVideoSource( IBaseFilter *filter )
{
    CHK_FAIL_MSG_RETURN(!filter, DX_ERR_NULL_PTR, "Try to connect crossbar to null filter.");
    HRESULT hr = GraphUtil::ConnectFilters(graph_builder_, video_crossbar_filter_, filter);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Connect crossbar to camera failed.");

    return DX_ERR_NONE;
}


//video source container

VideoSourceContainer::VideoSourceContainer()
{
}

VideoSourceContainer::~VideoSourceContainer()
{
    if (graph_builder_ && video_source_filter_)
    {
        graph_builder_->RemoveFilter(video_source_filter_);
    }
    //将COM指针设为NULL以减少计数
    video_pin_out_ = nullptr; //确保先释放pin再释放filter
    video_source_filter_ = nullptr;
}

dxStatus VideoSourceContainer::AddToGraph( IGraphBuilder *owner_graph, std::string device_path )
{
	HRESULT hr;
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video source to null graph.");
    graph_builder_ = owner_graph;

    // Free previous added filter
    if (video_source_filter_)
    {
        graph_builder_->RemoveFilter(video_source_filter_);
        video_source_filter_ = NULL;
    }

    LOG_INFO("Add video source: %s", device_path.c_str());

    // Get filter
	if (device_path=="@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}")
	{
		 hr = GraphUtil::GetFilterByDisplayName(Util::str2wstr(device_path).c_str(),&video_source_filter_);//added by hjf;
	}
	//get by Friendlyname
	//HRESULT hr = GraphUtil::GetFilterByFriendlyName(CLSID_VideoInputDeviceCategory,NULL,&video_source_filter_);

	//HRESULT hr = GraphUtil::getFilterByVariousName(CLSID_VideoInputDeviceCategory,3,L"@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}",&video_source_filter_);
	else
	{
		//origin
		 hr = GraphUtil::GetFilterByDevicePath(CLSID_VideoInputDeviceCategory,
			Util::str2wstr(device_path).c_str(), &video_source_filter_);
	}


    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_BAD_DEVICE, "Get camera failed. DevicePath=%s", 
        device_path.c_str());

    // Add filter to graph
    hr = graph_builder_->AddFilter(video_source_filter_, L"VideoSource");
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Add camera to graph failed");

	GraphUtil::GetUnconnectedPin(video_source_filter_, PINDIR_OUTPUT, &video_pin_out_);//原本代码是必须调用SetFormat顺便设置outputpin，这里改为在添加filter的时候直接获取--hjf 2015.11.19
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get camera output pin failed.");
    //保证不调用SetFormat也能用

    LOG_PRINTF("Default video format is:");
    SetFormatKinect(0, 0, 0);

    return DX_ERR_NONE;
}

dxStatus VideoSourceContainer::AddToGraph( IGraphBuilder *owner_graph, std::string device_path,std::string displayName )
{
	CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add video source to null graph.");
	graph_builder_ = owner_graph;

	// Free previous added filter
	if (video_source_filter_)
	{
		graph_builder_->RemoveFilter(video_source_filter_);
		video_source_filter_ = NULL;
	}

	LOG_INFO("Add video source: %s", device_path.c_str());
	// Get filter
	//因为Kinect获取不到devicepath，所以判断如果devicepath为空就采用displayname获取filter的方式（暂时。。得改）--HJF
	HRESULT hr;
	if (device_path=="")
	{
		if (displayName!="")
		{
			hr = GraphUtil::GetFilterByDisplayName(L"@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}",&video_source_filter_);//added by hjf;
		}
	}
	else
	{
		//origin
		hr = GraphUtil::GetFilterByDevicePath(CLSID_VideoInputDeviceCategory,
			Util::str2wstr(device_path).c_str(), &video_source_filter_);
	}



	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_BAD_DEVICE, "Get camera failed. DevicePath=%s", 
		device_path.c_str());

	// Add filter to graph
	hr = graph_builder_->AddFilter(video_source_filter_, L"VideoSource");
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Add camera to graph failed");


	//保证不调用SetFormat也能用
	LOG_PRINTF("Default video format is:");
	SetFormat(0, 0, 0);

	return DX_ERR_NONE;
}

dxStatus VideoSourceContainer::SetFormatKinect( unsigned int width, unsigned int height, unsigned int fps )
{
	CHK_FAIL_MSG_RETURN(video_source_filter_ == NULL, DX_ERR_NULL_PTR, "Set format to null camera.");

	video_pin_out_ = NULL;
	HRESULT hr = GraphUtil::GetUnconnectedPin(video_source_filter_, PINDIR_OUTPUT, &video_pin_out_);
	CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get camera output pin failed.");

	CComQIPtr<IAMStreamConfig> pCfg = video_pin_out_;
	CHK_FAIL_MSG_RETURN(pCfg==NULL, DX_ERR_SYS, "Get IAMStreamConfig of video pin failed");

	LOG_INFO("Attempt video format: %dx%d@%d", width, height, fps);

	unsigned int retreived_fps = 0;
	int try_times = 3; //有些设备有延迟，要多试几次
	candidate_type_.InitMediaType(); //reset
	while (try_times-- > 0)
	{
		SeekRealFormat(video_pin_out_);

		int iCount = 0, iSize = 0;
		hr = pCfg->GetNumberOfCapabilities(&iCount, &iSize);

		// Check the size to make sure we pass in the correct structure
		if(iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{
			// Use the video capabilities structure.
			for(int iFormat = 0; iFormat < iCount; iFormat++)
			{
				CMediaType mt;
				{
					VIDEO_STREAM_CONFIG_CAPS scc;
					AM_MEDIA_TYPE* pmtConfig = NULL;
					hr = pCfg->GetStreamCaps(iFormat, &pmtConfig, (BYTE *)&scc);
					if (FAILED(hr)) continue;
					if (pmtConfig->majortype != MEDIATYPE_Video) {
						DeleteMediaType(pmtConfig);
						continue;
					}
					mt = *pmtConfig;
					DeleteMediaType(pmtConfig);
				}

				//无效格式
				if (mt.pbFormat == NULL) continue;

				//检查分辨率
				if((mt.formattype == FORMAT_VideoInfo) &&
					(mt.cbFormat >= sizeof(VIDEOINFOHEADER))) 
				{
					VIDEOINFOHEADER* info = (VIDEOINFOHEADER*)mt.pbFormat;
					if (width == 0 || height == 0)  //0表示未指定，此时以第一个获取到的分辨率为准
					{
						width = info->bmiHeader.biWidth;
						height = info->bmiHeader.biHeight;
					}

					if(info->bmiHeader.biWidth != width || info->bmiHeader.biHeight != height)
						continue;
				} 
				else if((mt.formattype == FORMAT_VideoInfo2) &&
					(mt.cbFormat >= sizeof(VIDEOINFOHEADER2))) 
				{
					VIDEOINFOHEADER2* info = (VIDEOINFOHEADER2*)mt.pbFormat;
					if (width == 0 || height == 0)  //0表示未指定，此时以第一个获取到的分辨率为准
					{
						width = info->bmiHeader.biWidth;
						height = info->bmiHeader.biHeight;
					}
					if(info->bmiHeader.biWidth != width || info->bmiHeader.biHeight != height)
						continue;
				} 
				else
					continue;

				//检查图片格式
				if(mt.subtype != MEDIASUBTYPE_YUY2 && mt.subtype != MEDIASUBTYPE_RGB24)
				{
					continue;
				}

				candidate_type_ = mt;

				
				// Set the new formattype for the output pin
				hr = pCfg->SetFormat((AM_MEDIA_TYPE*)&mt);
				CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Set camera video format failed");

				//设置成功的话，跳出
				if (hr==S_OK) break;//2015.11.18 by HJF

				//优先使用YUY2，如果已找到，则跳出
				if (candidate_type_.subtype == MEDIASUBTYPE_YUY2) {
					break;
				}
			}
		}
		else
		{
			LOG_ERROR("Unknown video format data.");
			return DX_ERR_ABORT;
		}

		if (candidate_type_.IsValid()) break;
		Sleep(1000); //等待1s重试
	}

	if (!candidate_type_.IsValid())
	{
		LOG_ERROR("Invalid video format %dx%d@%d fps", width, height, fps);
		return DX_ERR_BAD_PARAM;
	}
	else
	{
		LOG_INFO("Device video format: %dx%d@%d, %s", width, height, retreived_fps,
			candidate_type_.subtype == MEDIASUBTYPE_YUY2 ? "YUY2" : "RGB24");
		//上面的检查过程确定了只能是这两种格式二选一
	}

	return DX_ERR_NONE;
}

dxStatus VideoSourceContainer::SetFormat( unsigned int width, unsigned int height, unsigned int fps )
{
    CHK_FAIL_MSG_RETURN(video_source_filter_ == NULL, DX_ERR_NULL_PTR, "Set format to null camera.");

    video_pin_out_ = NULL;
    HRESULT hr = GraphUtil::GetUnconnectedPin(video_source_filter_, PINDIR_OUTPUT, &video_pin_out_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get camera output pin failed.");

    CComQIPtr<IAMStreamConfig> pCfg = video_pin_out_;
    CHK_FAIL_MSG_RETURN(pCfg==NULL, DX_ERR_SYS, "Get IAMStreamConfig of video pin failed");

    LOG_INFO("Attempt video format: %dx%d@%d", width, height, fps);

    unsigned int retreived_fps = 0;
    int try_times = 3; //有些设备有延迟，要多试几次
    candidate_type_.InitMediaType(); //reset

    while (try_times-- > 0)
    {
        SeekRealFormat(video_pin_out_);

        int iCount = 0, iSize = 0;
        hr = pCfg->GetNumberOfCapabilities(&iCount, &iSize);

        // Check the size to make sure we pass in the correct structure
        if(iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
        {
            // Use the video capabilities structure.
            for(int iFormat = 0; iFormat < iCount; iFormat++)
            {
                CMediaType mt;
                {
                    VIDEO_STREAM_CONFIG_CAPS scc;
                    AM_MEDIA_TYPE* pmtConfig = NULL;
                    hr = pCfg->GetStreamCaps(iFormat, &pmtConfig, (BYTE *)&scc);
                    if (FAILED(hr)) continue;
                    if (pmtConfig->majortype != MEDIATYPE_Video) {
                        DeleteMediaType(pmtConfig);
                        continue;
                    }
                    mt = *pmtConfig;
                    DeleteMediaType(pmtConfig);
                }

                //无效格式
                if (mt.pbFormat == NULL) continue;

                //检查分辨率
                if((mt.formattype == FORMAT_VideoInfo) &&
                    (mt.cbFormat >= sizeof(VIDEOINFOHEADER))) 
                {
                    VIDEOINFOHEADER* info = (VIDEOINFOHEADER*)mt.pbFormat;
                    if (width == 0 || height == 0)  //0表示未指定，此时以第一个获取到的分辨率为准
                    {
                        width = info->bmiHeader.biWidth;
                        height = info->bmiHeader.biHeight;
                    }
                    if(info->bmiHeader.biWidth != width || info->bmiHeader.biHeight != height)
                        continue;
                    retreived_fps = info->AvgTimePerFrame == 0 ? 0 : (unsigned int)(1e7 / info->AvgTimePerFrame);
                    if (retreived_fps > fps + 1 || retreived_fps < fps - 1) { //允许一帧误差
                        info->AvgTimePerFrame = (REFERENCE_TIME)(1e7 / fps);
                    }
                } 
                else if((mt.formattype == FORMAT_VideoInfo2) &&
                    (mt.cbFormat >= sizeof(VIDEOINFOHEADER2))) 
                {
                    VIDEOINFOHEADER2* info = (VIDEOINFOHEADER2*)mt.pbFormat;
                    if (width == 0 || height == 0)  //0表示未指定，此时以第一个获取到的分辨率为准
                    {
                        width = info->bmiHeader.biWidth;
                        height = info->bmiHeader.biHeight;
                    }
                    if(info->bmiHeader.biWidth != width || info->bmiHeader.biHeight != height)
                        continue;
                    retreived_fps = info->AvgTimePerFrame == 0 ? 0 : (unsigned int)(1e7 / info->AvgTimePerFrame);
                    if (retreived_fps > fps + 1 || retreived_fps < fps - 1) { //允许一帧误差
                        info->AvgTimePerFrame = (REFERENCE_TIME)(1e7 / fps);
                    }
                } 
                else
                    continue;

                //检查图片格式
                if(mt.subtype != MEDIASUBTYPE_YUY2 && mt.subtype != MEDIASUBTYPE_RGB24)
                {
                    continue;
                }

                candidate_type_ = mt;

                // Set the new formattype for the output pin
                hr = pCfg->SetFormat((AM_MEDIA_TYPE*)&mt);
                CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Set camera video format failed");

				//设置成功的话，跳出
				if (hr==S_OK) break;//2015.11.18 by HJF

                //优先使用YUY2，如果已找到，则跳出
                if (candidate_type_.subtype == MEDIASUBTYPE_YUY2) {
                    break;
                }
            }
        }
        else
        {
            LOG_ERROR("Unknown video format data.");
            return DX_ERR_ABORT;
        }

        if (candidate_type_.IsValid()) break;
        Sleep(1000); //等待1s重试
    }

    if (!candidate_type_.IsValid())
    {
        LOG_ERROR("Invalid video format %dx%d@%d fps", width, height, fps);
        return DX_ERR_BAD_PARAM;
    }
    else
    {
        LOG_INFO("Device video format: %dx%d@%d, %s", width, height, retreived_fps,
            candidate_type_.subtype == MEDIASUBTYPE_YUY2 ? "YUY2" : "RGB24");
        //上面的检查过程确定了只能是这两种格式二选一
    }

    return DX_ERR_NONE;
}

//有些摄像头需要此伪连接操作才能获得真实视频格式
void VideoSourceContainer::SeekRealFormat( IPin * pPin )
{
    if (NULL == pPin)
    {
        return;
    }

    CComPtr<IBaseFilter> pNullRender;
    HRESULT hr = pNullRender.CoCreateInstance(GraphUtil::clsid_null_renderer); //ref count 1
    if (FAILED(hr)) return;
    hr = graph_builder_->AddFilter(pNullRender, L"NullRenderer"); //ref count 2
    if (FAILED(hr)) return;
    hr = GraphUtil::ConnectFilters(graph_builder_, pPin, pNullRender);
    if (FAILED(hr)) return;
    pPin->Disconnect();
    graph_builder_->RemoveFilter(pNullRender); //ref count 1
}



//MicrophoneSourceContainer

MicrophoneSourceContainer::MicrophoneSourceContainer()
{
}

MicrophoneSourceContainer::~MicrophoneSourceContainer()
{
    if (graph_builder_ && audio_source_filter_)
    {
        graph_builder_->RemoveFilter(audio_source_filter_);
    }
}

dxStatus MicrophoneSourceContainer::AddFilter( 
    IGraphBuilder *owner_graph, 
    std::string input_device, 
    std::string output_device )
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add audio source to null graph.");
    graph_builder_ = owner_graph;

    LOG_INFO("Adding audio source: %s", input_device.c_str());

    HRESULT hr;
    hr = GraphUtil::GetFilterByDisplayName(
        Util::str2wstr(input_device).c_str(), &audio_source_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_BAD_DEVICE, "Get audio source failed. DisplayName=%s", 
        input_device.c_str());

    // Add filter to graph
    hr = graph_builder_->AddFilter(audio_source_filter_, L"AudioSource");
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Add audio source to graph failed");

    //保证不调用SetFormat也能用
    LOG_PRINTF("Default audio format is:");
    SetFormat(0, 0, 0, 40);

    return DX_ERR_NONE;
}

dxStatus MicrophoneSourceContainer::SetFormat( 
    unsigned int sample_rate, 
    unsigned int channel_count, 
    unsigned int bits_per_sample,
    unsigned int latency)
{
    audio_pin_out_ = NULL;
    HRESULT hr = GraphUtil::GetUnconnectedPin(audio_source_filter_, PINDIR_OUTPUT, &audio_pin_out_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get audio source output pin failed.");

    //设备采集的缓冲区，减少采集时延
    CComQIPtr<IAMBufferNegotiation> pNeg = audio_pin_out_;
    CHK_FAIL_MSG_RETURN(!pNeg, DX_ERR_BAD_INTERFACE, 
        "Query IAMBufferNegotiation of audio source failed.");

    //延迟设置只对AAC有效，Speex的延迟需要到Speex编码器中修改
    if (latency <= 20) latency = 40; //默认40ms
    LOG_INFO("Audio source delay: %dms", latency);

    ALLOCATOR_PROPERTIES prop = {0};
    prop.cbBuffer = sample_rate * channel_count * (bits_per_sample / 8) * latency / 1000;
    prop.cBuffers = 1000 / latency; // enough for 1s
    prop.cbAlign = (bits_per_sample / 8) * channel_count;
    prop.cbPrefix = 0;
    hr = pNeg->SuggestAllocatorProperties(&prop);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Set audio source buffer failed: 0x%08x", hr);

    //设置音频格式
    CComQIPtr<IAMStreamConfig> pCfg = audio_pin_out_;

    AM_MEDIA_TYPE *pmt = NULL;
    hr = pCfg->GetFormat(&pmt);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get original audio format failed: 0x%08x", hr);

    WAVEFORMATEX * pWF = (WAVEFORMATEX *)pmt->pbFormat;
    if (sample_rate >= 8000 && channel_count > 0 && bits_per_sample == 16)
    {
        // Fill in values for the new format
        pWF->nChannels = (WORD)channel_count;
        pWF->nSamplesPerSec = sample_rate;
        pWF->wBitsPerSample = (WORD)bits_per_sample;
        pWF->nBlockAlign = (WORD)((pWF->wBitsPerSample / 8) * pWF->nChannels);
        pWF->nAvgBytesPerSec = pWF->nBlockAlign * pWF->nSamplesPerSec;

        // Set the new formattype for the output pin
        hr = pCfg->SetFormat(pmt);
        CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Set audio format failed: 0x%08x", hr);
    }
    else if (sample_rate != 0 || channel_count != 0 || bits_per_sample != 0)
    {
        LOG_WARN("Invalid audio format: %dHz, %dchn, %dbits. Default format will be used.", 
            sample_rate, channel_count, bits_per_sample);
    }
    LOG_INFO("Audio format:  %dHz, %dchn, %dbits",
        pWF->nSamplesPerSec, pWF->nChannels, pWF->wBitsPerSample);

    candidate_type_ = *pmt;
    DeleteMediaType(pmt);

    return DX_ERR_NONE;
}


//AEC

AECSourceContainer::AECSourceContainer()
{

}

AECSourceContainer::~AECSourceContainer()
{
    if (graph_builder_ && audio_source_filter_)
    {
        graph_builder_->RemoveFilter(audio_source_filter_);
    }
}

dxStatus AECSourceContainer::AddFilter( 
    IGraphBuilder *owner_graph, 
    std::string input_device, 
    std::string output_device)
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add aec filter to null graph.");
    graph_builder_ = owner_graph;

    HRESULT hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"AEC", 
        GraphUtil::clsid_scut_aec_source, &audio_source_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Add AEC filter to graph failed: 0x%08x", hr);

    hr = GraphUtil::GetUnconnectedPin(audio_source_filter_, PINDIR_OUTPUT, &audio_pin_out_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_SYS, "Get AEC output pin failed: 0x%08x", hr);

    CComQIPtr<IAECProperty> aec_config = audio_pin_out_;
    CHK_FAIL_MSG_RETURN(!aec_config, DX_ERR_SYS, "Query IAECProperty of AEC failed.");

    LOG_INFO("Set AEC devices:\n\tInput: %s \n\tOutput%s", input_device.c_str(), output_device.c_str());

    std::wstring input_device_wstr = Util::str2wstr(input_device);
    std::wstring output_device_wstr = Util::str2wstr(output_device);
    hr = aec_config->put_CapDevFriendName((wchar_t *)input_device_wstr.c_str(), input_device_wstr.size(), 
        (wchar_t*)output_device_wstr.c_str(), output_device_wstr.size());
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_BAD_DEVICE, "Invalid AEC devices");

    //不知道这个是干什么用的，师兄留下的了
    HWND win_handle = ::FindWindow(0, "CoolView");
    aec_config->put_PreviewHandle(win_handle);

    //保证不调用SetFormat也能用
    LOG_PRINTF("Default audio format is:");
    SetFormat(0, 0, 0, 40);

    return DX_ERR_NONE;
}


//File Source

FileSourceContainer::~FileSourceContainer()
{
    if (graph_builder_ && file_source_filter_)
    {
        graph_builder_->RemoveFilter(file_source_filter_);
    }
}

dxStatus FileSourceContainer::AddFilter(IGraphBuilder *owner_graph, std::string file)
{
    CHK_FAIL_MSG_RETURN(!owner_graph, DX_ERR_NULL_PTR, "Add file source to null graph.");
    CHK_FAIL_MSG_RETURN(file.empty(), DX_ERR_NULL_PTR, "Set file source to null path.");
    graph_builder_ = owner_graph;
    file_source_filter_ = nullptr;
    pin_out_ = nullptr;
    HRESULT hr;

    hr = GraphUtil::AddFilterByCLSID(graph_builder_, L"File Source", 
        GraphUtil::clsid_file_source, &file_source_filter_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Add file source failed.(hr=0x%08x)", hr);

    CComQIPtr<IFileSourceFilter> pFileSource = file_source_filter_;
    CHK_FAIL_MSG_RETURN(!pFileSource, DX_ERR_BAD_INTERFACE, 
        "Query IFileSourceFilter failed.");

    hr = pFileSource->Load(
        Util::str2wstr(file).c_str(), NULL);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Load file failed.(hr=0x%08x)", hr);

    hr = GraphUtil::GetUnconnectedPin(file_source_filter_, PINDIR_OUTPUT, &pin_out_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Get file source pin failed.(hr=0x%08x)", hr);

    return DX_ERR_NONE;
}
