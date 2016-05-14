#include "StreamMediaController.h"

#include <psapi.h>
#include <time.h>

#include "util\ProcessManager.h"
#include "log/Log.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"

StreamMediaController::StreamMediaController(const QString &mediaID)
    : BaseMediaController(mediaID, STREAMED_MEDIA_SERVICE_NAME)
{

}

StreamMediaController::~StreamMediaController()
{

}

void StreamMediaController::Initialize()
{
    //创建StreamMedia客户端接口并连接到DBus服务.
    //无论对应媒体流进程是否存在接口方法都可调用;可通过isValid判断对应媒体流进程是否存在
    QString serviceName = getStreamedMediaServiceName(media_id_);
    QString objectPath = getStreamedMediaObjectPath(media_id_);
    CvStreamedMediaIf * ptr = new CvStreamedMediaIf(serviceName, 
        objectPath, 
        QDBusConnection::sessionBus(),
        this); //设置父对象以在相同线程中运行
    media_if_.reset(ptr);

    //连接媒体流进程状态变更的信号与槽,不推荐此方式!
    //理由:建议直接调用ChannelDispatcher的ChannelStateChanged方法而不是发送该信号,是因为
    //目前配置ChannelDispatcher为DBus服务(见dbus\services下配置文件),当ChannelDispatcher
    //崩溃后,如果发送信号,ChannelDispatcher不会被重启,而调用其方法DBus会自动重启服务
    //Liaokz, 2013-11
    /*connect(ptr, &CvStreamedMediaIf::RunningStateChanged,
            this, &StreamMediaController::HandleStateChanged);*/

    //最后调用父类初始化方法,以确保新创建的children能在独立线程中执行
    BaseMediaController::Initialize();
}

int StreamMediaController::SpecCreate( const QByteArray &param )
{
    QString procName = QString(STREAMED_MEDIA_PROC_NAME) + QString(".exe");
    QString procArgs = media_id_;
    proc_handle_ = ProcessManager::startProcess(procName.toLocal8Bit().data(), 
        procArgs.toLocal8Bit().data(), debug_mode_);
    if (INVALID_HANDLE_VALUE == proc_handle_)
    {
        return -1;
    }

    init_memory_usage_ = 0;
    return 0;
}

int StreamMediaController::SpecModify( const int type, const QByteArray &param )
{
    if (!isProxyValid())
    {
        return -1;
    }
    media_if_->SetMediaInfo(type, param);
    return 0;
}

int StreamMediaController::SpecRelease()
{
    if (!isProxyValid())
    {
        return -1;
    }
    media_if_->Destroy();
    return 0;
}

int StreamMediaController::SpecStateChanged( const int state )
{
    if (!isProxyValid())
    {
        return -1;
    }

    StreamedMediaState currentState = static_cast<StreamedMediaState>(state);

    switch (currentState)
    {
    case StreamedMedia_NoInitial:
        if(isSendMediaID(media_id_)) // send
        {
            LOG_PRINTF("Action_InitialSendGraph %s", media_id_.toLocal8Bit().data());
            media_if_->SetMediaInfo(Action_InitialSendGraph , start_param_);
        }
        else if(isSmallVideoMediaID(media_id_)) // small
        {
            LOG_PRINTF("Action_InitialSmallVideoGraph %s", media_id_.toLocal8Bit().data());
            media_if_->SetMediaInfo(Action_InitialSmallVideoGraph, start_param_);
        }
        else if (isFilePlayMediaID(media_id_)) // file play
        {
          LOG_PRINTF("Action_InitialFilePlayGraph %s", media_id_.toLocal8Bit().data());
          media_if_->SetMediaInfo(Action_InitialFilePlayGraph, start_param_);
        }
        else // recv
        {
            LOG_PRINTF("Action_InitialRecvGraph %s", media_id_.toLocal8Bit().data());
            media_if_->SetMediaInfo(Action_InitialRecvGraph , start_param_);
        }
        SetState(Running);
        init_time_ = time(nullptr);
        break;
    case StreamedMedia_Running:
        break;
    case StreamedMedia_Pause:
        break;
    case StreamedMedia_Stop:
        break;
    case StreamedMedia_Destroyed:
        break;
    default:
        break;
    }

    return 0;
}

bool StreamMediaController::isProxyValid()
{
    //QT只会返回创建时的状态，故每次都要新建一个再检查才准确
    QString serviceName = getStreamedMediaServiceName(media_id_);
    QString objectPath = getStreamedMediaObjectPath(media_id_);
    CvStreamedMediaIf * ptr = new CvStreamedMediaIf(serviceName, 
        objectPath, 
        QDBusConnection::sessionBus(),
        this);
    return ptr->isValid();
}

QString StreamMediaController::GetType()
{
    return STREAMED_MEDIA_SERVICE_NAME;
}

int StreamMediaController::SpecRunningCheck()
{
    if (Running != state_) { //确保在running状态下
        return 0;
    }
    if (time(nullptr) - init_time_ < 60) { //运行一分钟后内存占用数值更准确
        return 0;
    }

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(proc_handle_, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        if (0 == init_memory_usage_) {
            init_memory_usage_ = pmc.PrivateUsage;
            LOG_INFO("Process %s initial memory usage: %d MB", 
                media_id_.toLocal8Bit().data(), init_memory_usage_/(1<<20));
        } 
        else if (pmc.PrivateUsage > init_memory_usage_ && //注意数值是unsigned long，直接减可能会变成很大的数从而误杀
                 pmc.PrivateUsage - init_memory_usage_ > kMemoryIncreaseLimit) {
            //当内存增长超过一定量时，认为存在内存泄露，杀掉进程，
            //重启见BaseStreamController实现，状态恢复又CvStreamMedia进程自己完成
            ::TerminateProcess(proc_handle_, -1);
            LOG_WARN("Process %s memory usage: %d MB, init: %d MB, kill it", 
                media_id_.toLocal8Bit().data(), pmc.PrivateUsage/(1<<20), init_memory_usage_/(1<<20));
        }
    }
    return 0;
}
