/************************************************************************/
/* 本文件定义graph的控制类和控制类工厂类的对外导出接口                  */
/* 使用接口的目的是，隐藏具体控制类的实现，以减少暴漏不必要的成员。     */
/************************************************************************/

#ifndef I_GRAPH_CONTROLLER_H
#define I_GRAPH_CONTROLLER_H

#include "DxDefinitions.h"
#include "util\report\RtpStat.h" //导入RTP回调报告定义
#include "util\report\RecordStat.h" //导入录像回调报告定义
#include "util\report\FilePlayStat.h"
//本来这导出文件不该引入别的文件，但是真的要用到，没办法

#include <string>
#include <vector>


#ifdef MSDX_EXPORTS
#define MSDXAPI __declspec(dllexport)
#else
#define MSDXAPI __declspec(dllimport)
#endif


namespace msdx //注意所有定义都在msdx命名空间中
{


//发送Graph控制器接口
//创建发送Graph的参数结构
typedef struct
{
    struct Video
    {
        std::string crossbar; //采集卡设备DisplayName，其实采集卡现在应该也不用手动配置了，但还是保留
        long crossbar_physical_type; //采集卡输入索引
		std::string displayName;//多加一个displayName作为选用设备标识--hjf
        std::string device; //摄像头DevicePath
        dxVideoCodec codec;
        unsigned int frame_width;
        unsigned int frame_height;
        unsigned int fps;
        bool enable_hwcodec; //是否使用硬件加速编码

        //预览窗口句柄，调试用
        VideoWindow preview;
    } video;

    struct Audio
    {
        std::string input_device;
        std::string output_device;
        dxAudioCodec codec;
        unsigned int sample_rate;
        unsigned int channels;
        unsigned int bitcount;
        unsigned int latency;
        bool enable_aec; //使用回声对消audio echo cancellation
    } audio;

} SendGraphParam;

class MSDXAPI ISendGraphController {
public:
    virtual ~ISendGraphController() {};
    //获取graph运行状态
    virtual dxGraphStatus GetStatus() = 0;

    //创建和启停graph
    virtual dxStatus Build(SendGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;

    //添加和删除发送端口
    virtual dxStatus AddVideoDestination(const NetAddr &) = 0;
    virtual dxStatus DeleteVideoDestination(const NetAddr &) = 0;
    virtual dxStatus AddAudioDestination(const NetAddr &) = 0;
    virtual dxStatus DeleteAudioDestination(const NetAddr &) = 0;

    //用于停止发送音视频功能
    virtual dxStatus StopSendingVideo(bool stop) = 0;
    virtual dxStatus StopSendingAudio(bool stop) = 0;

    //RTP统计回调，没有放到SendGraphParam中，以便于动态更改
    virtual dxStatus SetVideoRtpStatCallBack(void *obj, RtpStatCallback func) = 0;
    virtual dxStatus SetAudioRtpStatCallBack(void *obj, RtpStatCallback func) = 0;
};



//小流发送Graph控制器接口
typedef struct
{
    struct Video
    {
        NetAddr addr;
        dxVideoCodec codec;
        unsigned int frame_width;
        unsigned int frame_height;
        unsigned int fps;
        bool enable_hwcodec; //是否使用硬件加速编码
    } video;

} SmallGraphParam;

class MSDXAPI ISmallGraphController {
public:
    virtual ~ISmallGraphController() {}
    //获取graph运行状态
    virtual dxGraphStatus GetStatus() = 0;

    //创建和启停graph
    virtual dxStatus Build(SmallGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;

    //添加和删除发送端口
    virtual dxStatus AddVideoDestination(const NetAddr &) = 0;
    virtual dxStatus DeleteVideoDestination(const NetAddr &) = 0;

    //用于停止发送音视频功能
    virtual dxStatus StopSendingVideo(bool stop) = 0;
};



//接收Graph控制器接口
typedef struct
{
    std::string graph_name; //一个区分graph的标识，先用上层的user_id
    bool used_as_record; //是否是录像用途

    struct Video
    {
        NetAddr addr;
        dxVideoCodec codec;
        unsigned int frame_width;
        unsigned int frame_height;
        unsigned int fps;

        //显示窗口句柄
        VideoWindow preview;
    } video;

    struct Audio
    {
        NetAddr addr;
        std::string output_device;
        dxAudioCodec codec;
        unsigned int sample_rate;
        unsigned int channels;
        unsigned int bitcount;
    } audio;

    struct Record
    {
        long job_id;
        std::string file_path_name;
    } record;

} RecvGraphParam;

class MSDXAPI IRecvGraphController {
public:
    virtual ~IRecvGraphController() {}
    //获取graph运行状态
    virtual dxGraphStatus GetStatus() = 0;

    virtual dxStatus Build(RecvGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;

    //用于停止接收音视频功能
    virtual dxStatus StopReceivingVideo(bool stop) = 0;
    virtual dxStatus StopReceivingAudio(bool stop) = 0;

    //变更视频窗口
    virtual dxStatus ChangeVideoWindow(VideoWindow window) = 0;

    //录像控制
    virtual dxStatus StartRecordingNewFile(long job_id, std::string new_file_name) = 0;
    virtual dxStatus SetRecordStatCallBack(void *obj, RecordStatCallback func) = 0;

    //RTP统计回调，没有放到SendGraphParam中，以便于动态更改
    virtual dxStatus SetVideoRtpStatCallBack(void *obj, RtpStatCallback func) = 0;
    virtual dxStatus SetAudioRtpStatCallBack(void *obj, RtpStatCallback func) = 0;
};



//视频文件播放Graph控制器接口
typedef struct
{
    std::string graph_name; //一个区分graph的标识
    std::string sync_id; //用于多路音视频同步的同步组标识
    std::vector<std::string> file_list; //要播放的文件列表
    long long initial_delay; //开始播放时的延迟，用于多路视频同步

    struct Video
    {
        NetAddr addr;
    } video;

    struct Audio
    {
        NetAddr addr;
    } audio;

} FilePlayGraphParam;

class MSDXAPI IFilePlayGraphController {
public:
    virtual ~IFilePlayGraphController() {}
    //获取graph运行状态
    virtual dxGraphStatus GetStatus() = 0;

    virtual dxStatus Build(FilePlayGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;

    //播放操作，op_id应从1开始
    virtual dxStatus Pause(unsigned long op_id) = 0;
    virtual dxStatus Resume(unsigned long op_id) = 0;
    //随机访问，t精度为1/10000000秒
    virtual dxStatus SeekTo(unsigned long op_id, long long t) = 0;

    virtual dxStatus SetFilePlayStatCallBack(void *obj, FilePlayStatCallback func) = 0;
    //通过回调报告音视频格式
    virtual void ReportMediaFormat() = 0;
};



//设备测试Graph控制器接口
typedef struct
{
    //video/audio只能二选一
    struct Video
    {
        std::string crossbar; //采集卡设备DisplayName
        long crossbar_physical_type; //采集卡输入索引
        std::string device; //摄像头display_name
        unsigned int frame_width;
        unsigned int frame_height;
        VideoWindow preview;
    } video;

    struct Audio
    {
        //音频测试仅支持默认采样率和声道
        std::string input_device; //设备display_name,优先
        std::string input_file; //文件名。同时指定了输入设备时，此项无效
        std::string output_device;
    } audio;
} TestGraphParam;

class MSDXAPI ITestGraphController {
public:
    virtual ~ITestGraphController() {}

    virtual dxStatus Build(TestGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;
};



//用于创建graph对象的工厂类
class MSDXAPI GraphControllerFactory
{
private:
    GraphControllerFactory(void);
    ~GraphControllerFactory(void);

public:
    static GraphControllerFactory &GetInstance();

    ISendGraphController *CreateSendGraphController();
    ISmallGraphController *CreateSmallGraphController();
    IRecvGraphController *CreateRecvGraphController();
    ITestGraphController *CreateTestGraphController();
    IFilePlayGraphController *CreateFilePlayGraphController();

	ISendGraphController *CreateTestSendGraphController();
};


} // end namespace msdx

#endif
