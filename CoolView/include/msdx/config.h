#ifndef CONFIG_H
#define CONFIG_H

//通过测试发现在存在视频分流器的机器上不支持320的格式，只能支持384，
//在根据1280x720的宽高比得出216的高...zhenhua.sun 2012/02/29
//可能是因为解码器直接连接Render时需要做Surface Stride对齐
//详见下文中Calculating Surface Stride：
//http://msdn.microsoft.com/en-us/library/windows/desktop/dd318229(v=vs.85).aspx
//Liaokz补充
#define MSDX_CONF_SMALL_VIDEO_WIDTH     384
#define MSDX_CONF_SMALL_VIDEO_HEIGHT    216

//音视频可选设置更改后，请同时修改CoolView中RunningConfig和设备设置页面处理逻辑

#define MSDX_CONF_AUDIO_CODEC_SPEEX     "SPEEX"
#define MSDX_CONF_AUDIO_CODEC_AAC       "AAC"
#define MSDX_CONF_AUDIO_SAMPLERATE_DEFAULT    16000
#define MSDX_CONF_AUDIO_CHANNEL_DEFAULT           1
#define MSDX_CONF_AUDIO_BITSPERSAMPLE_DEFAULT    16

//定义声音输入设备类型
#define MSDX_CONF_AUDIO_INPUT_NOAEC 0
#define MSDX_CONF_AUDIO_INPUT_WINAEC 1
#define MSDX_CONF_AUDIO_INPUT_WEBRTC 2

#define MSDX_CONF_VIDEO_CODEC_H264      "H264"

//定义自动选择采集卡的标示
#define MSDX_CONF_CROSSBAR_AUTO "Auto"

//定义本地媒体流端口基数
#define LOCALPRE_VIDEO_PORT_BASE 8000
#define LOCALPRE_SMALL_PORT_BASE 8100
#define LOCALREC_AUDIO_PORT_BASE 9000
#define LOCALREC_VIDEO_PORT_BASE 9100

#endif