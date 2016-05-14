#include "ScreenMediaController.h"
#include "util\ProcessManager.h"
#include "log/Log.h"
#include "dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h"
#include "dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h"

ScreenMediaController::ScreenMediaController(const QString &mediaID)
    : BaseMediaController(mediaID, SCREEN_MEDIA_SERVICE_NAME)
{

}

ScreenMediaController::~ScreenMediaController()
{

}

void ScreenMediaController::Initialize()
{
    BaseMediaController::Initialize();

    QString serviceName = getScreenMediaServiceName(media_id_);
    QString objectPath = getScreenMediaObjectPath(media_id_);
    CvScreenMediaIf * ptr = new CvScreenMediaIf(serviceName, 
        objectPath, 
        QDBusConnection::sessionBus(),
        this);
    media_if_.reset(ptr);

    //已废弃,理由见StreamMediaController
    /*connect(ptr, SIGNAL(RunningStateChanged(const QString &, int)),
        this, SLOT(HandleStateChanged(const QString &, int)));*/
}

int ScreenMediaController::SpecCreate( const QByteArray &param )
{
    QString procName;
    if( media_id_ == SEND_SCREEN_MEDIA_ID )
    {
        procName = SEND_SCREEN_MEDIA_PROC_NAME + QString(".exe");
    }
    else
    {
        procName = RECV_SCREEN_MEDIA_PROC_NAME + QString(".exe");
    }

    QString procArgs = media_id_;
    proc_handle_ = ProcessManager::startProcess(procName.toLocal8Bit().data(), procArgs.toLocal8Bit().data(), debug_mode_);
    if (INVALID_HANDLE_VALUE == proc_handle_)
    {
        return -1;
    }

    return 0;
}

int ScreenMediaController::SpecModify( const int type, const QByteArray &param )
{
    if (!isProxyValid())
    {
        return -1;
    }
    //No operation
    return 0;
}

int ScreenMediaController::SpecRelease()
{
    if (!isProxyValid())
    {
        return -1;
    }
    media_if_->Destroy();
    return 0;
}

int ScreenMediaController::SpecStateChanged( const int state )
{
    if (!isProxyValid())
    {
        return -1;
    }

    MediaState currentState = static_cast<MediaState>(state);

    switch (currentState)
    {
    case Media_NoInitial:
        if(media_id_ == SEND_SCREEN_MEDIA_ID)
        {
            media_if_->SetMediaInfo(ScreenMediaAction_InitialSendMedia, start_param_);
        }
        else
        {
            media_if_->SetMediaInfo(ScreenMediaAction_InitialRecvMedia, start_param_);
        }
        SetState(Running);
        break;
    case Media_Running:
        break;
    case Media_Pause:
        break;
    case Media_Stop:
        break;
    case Media_Destroyed:
        break;
    default:
        break;
    }

    return 0;
}

bool ScreenMediaController::isProxyValid()
{
    if (NULL == media_if_ || !media_if_->isValid())
    {
        return false;
    }
    return true;
}

QString ScreenMediaController::GetType()
{
    return SCREEN_MEDIA_SERVICE_NAME;
}
