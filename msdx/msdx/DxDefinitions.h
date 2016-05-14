/************************************************************************/
/* 该文件中定义了MSDX模块各部分共用的定义，以及对外暴漏的定义           */
/************************************************************************/

#ifndef DX_DEFINITIONS_H
#define DX_DEFINITIONS_H

#include <string>

namespace msdx
{

//返回码
enum dxStatus
{
    DX_ERR_NULL_PTR       = -1,
    DX_ERR_MEMORY_ALLOC   = -2,
    DX_ERR_SYS            = -3, //系统API返回错误
    DX_ERR_ABORT          = -4,
    DX_ERR_UNSUPPORT      = -5,
    DX_ERR_BAD_DEVICE     = -6, //无效设备
    DX_ERR_BAD_INTERFACE  = -7, //无效的接口
    DX_ERR_BAD_PARAM      = -8,

    DX_ERR_NONE           = 0,

    DX_WARN_NOT_IMPL      = 1,
    DX_WARN_OP_IGNORED    = 2,
};

#define STATUS_ERR(status) (status) < 0
#define STATUS_OK(status) (status) >= 0

//Graph状态
enum dxGraphStatus
{
    DX_GRAPH_STATUS_UNKNOWN = -1,
    DX_GRAPH_STATUS_INITIALIZED = 0,
    DX_GRAPH_STATUS_RUNNING,
    DX_GRAPH_STATUS_STOPPED,
    DX_GRAPH_STATUS_PAUSED
};

//音频编码方式
enum dxAudioCodec
{
    DX_AUDIO_CODEC_UNKNOWN,
    DX_AUDIO_CODEC_SPEEX,
    DX_AUDIO_CODEC_AAC
};

//视频编码方式
enum dxVideoCodec
{
    DX_VIDEO_CODEC_UNKNOWN,
    DX_VIDEO_CODEC_H264,
};

//定义采集卡连接类型未设置时的值
//注意不要与系统枚举tagPhysicalConnectorType中的任何值重复
//http://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
const int PhysConn_Auto = -1;


//网络发送或接受目标
typedef struct
{
    std::string ip; //ipv4 or ipv6
    unsigned short port;
} NetAddr;

//视频预览窗口
typedef struct
{
    unsigned long window_handle;
    long position_left; //图像位于窗口的位置
    long position_top;
    long width; //显示图像的大小
    long height;
} VideoWindow;


} // end namespace msdx

#endif