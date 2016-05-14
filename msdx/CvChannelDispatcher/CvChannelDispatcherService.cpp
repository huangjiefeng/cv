#include <windows.h>

#include "CvChannelDispatcherService.h"
#include "dbus/ChannelDispatcher/service/ChannelDispatcherAdaptor.h"
#include "dbus/channelDispatcher/common/ChannelDispatcherServiceCommon.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"
#include "dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h"
#include "dbus/channel/type/testMedia/common/TestMediaServiceCommon.h"

#include "log/Log.h"
#include "MediaControllerFactory.h"
#include "util/ProcessManager.h"
#include "util/BackupManager/BackupManager.h"


CvChannelDispatcherService::CvChannelDispatcherService()
{

}

CvChannelDispatcherService::~CvChannelDispatcherService()
{

}

int CvChannelDispatcherService::Initial()
{
    QReadLocker locker(&lock_);
    //Adapter
    new ChannelDispatcherAdaptor(this);

    //Register dbus service
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService(CHANNEL_DISPATCHER_SERVICE_NAME);
    if (!ret)
    {
        LOG_ERROR("Register dbus service failed. ServiceName=%s", CHANNEL_DISPATCHER_SERVICE_NAME);
        return 1;
    }
    ret = connection.registerObject(CHANNEL_DISPATCHER_OBJECT_PATH, this);
    if (!ret)
    {
        LOG_ERROR("Register dbus object failed. ObjectPath=%s", CHANNEL_DISPATCHER_OBJECT_PATH);
        return 1;
    }

    //Init data members
    destroy_ = FALSE;

    QString appDir = QCoreApplication::applicationDirPath();
    QString debugPath = appDir + "\\debug";
    if(QFile::exists(debugPath))
    {
        debug_mode_ = true;
    }
    else
    {
        debug_mode_ = false;
    }

    //Restore data
    RestoreControllers();

    LOG_PRINTF("Initialize channel dispatcher succeed.");
    return 0;
}

void CvChannelDispatcherService::RestoreControllers()
{
    BackupManagerPtr backupMgr = BackupManager::Instance();
    const int count = backupMgr->GetCount();
    if (0 == count)
    {
        LOG_PRINTF("No controller to restore.");
        return;
    }
    LOG_INFO("Restore %d controller data.", count);

    BackupManager::IDList ids = backupMgr->GetMediaIDList();
    for (const QString id : ids)
    {
        QString type;
        if (0 == backupMgr->FetchType(id, type))
        {
            IMediaControlPtr controller = MediaControllerFactory::GetController(type, id);
            media_controllers_[id] = controller;
        }
    }
}

void CvChannelDispatcherService::CreateChannel( const QString &mediaID, const QString &mediaType, const QByteArray &param )
{
    QReadLocker locker(&lock_);
    if (destroy_)
    {
        return;
    }

    IMediaControlPtr controller = media_controllers_[mediaID];//根据每一个mediaId创建controller?
    if (NULL == controller)
    {
        controller = MediaControllerFactory::GetController(mediaType, mediaID);
        if (NULL == controller)
        {
            LOG_ERROR("Create media controller for type %s failed.", mediaType.toLocal8Bit().data());
            return;
        }
        media_controllers_[mediaID] = controller;
    }

    controller->SetDebugMode(debug_mode_);
    controller->Create(param);
}

void CvChannelDispatcherService::ReleaseChannel( const QString &mediaID, const QString &mediaType )
{
    // 当前不需要mediaType参数
    QReadLocker locker(&lock_);
    if (destroy_)
    {
        return;
    }

    IMediaControlPtr controller = media_controllers_[mediaID];
    if (NULL == controller)
    {
        //Ignore
        LOG_PRINTF("Release ignored. Controller for %s does not exist.", mediaID.toLocal8Bit().data());
        return;
    }

    controller->Release();
}

// @param mediaID 媒体流进程的ID
// @param mediaType 媒体流类型
// @param type 为action类型。当指定了mediaID或mediaType，或两者同时制定，type应为具体媒体流类型的action，
//   即enum StreamedMediaActionID, enum ScreenMediaActionID, enum TestMediaActionID之一
//   当mediaID和mediaType均未制定时，应为enum MediaModifyType
// @remark 指定mediaID则无需指定mediaType，因为ID是唯一的；若不指定ID而指定type，则某类媒体流进程均被设置，
//   若二者均不指定，则根据操作，会对所有媒体流做设置，详见实现
void CvChannelDispatcherService::ModifyChannel( const QString &mediaID, const QString &mediaType, const int type, const QByteArray &param )
{
    // 仅mediaID为空时需要mediaType参数
    QReadLocker locker(&lock_);
    if (destroy_)
    {
        return;
    }

    if (mediaID.isEmpty())
    {
        // for all media process of type
        ModifyMultipleChannel(mediaType, type, param);
    } 
    else
    {
        // for one media process
        IMediaControlPtr controller = media_controllers_[mediaID];
        if (NULL == controller)
        {
            LOG_ERROR("Modify failed. Controller for %s does not exist.", mediaID.toLocal8Bit().data());
            return;
        }
        controller->Modify(type, param);
    }
}

void CvChannelDispatcherService::ChannelStateChanged( const QString &mediaID, 
                                                     const QString &user_id, 
                                                     const QString &mediaType, 
                                                     int state )
{
    QReadLocker locker(&lock_);
    if (destroy_)
    {
        return;
    }

    IMediaControlPtr controller = media_controllers_[mediaID];
    if (NULL == controller)
    {
        LOG_ERROR("Report state change failed. Controller for %s does not exist.", mediaID.toLocal8Bit().data());
        return;
    }

    controller->StateChanged(state);
    emit NofityChannelStateChanged(mediaID, user_id, mediaType, state);
}

void CvChannelDispatcherService::Destroy()
{
    {
        QWriteLocker locker(&lock_);
        destroy_ = TRUE;
    }

    LOG_PRINTF("Release all the processes...");
    const int count = media_controllers_.size();
    if (count > 0)
    {
        //Try to normally release
        for (IMediaControlPtr controller : media_controllers_)
        {
            if (NULL == controller || INVALID_HANDLE_VALUE == controller->GetHandle())
            {
                continue;
            }
            controller->Release();
        }
        //Wait for all process to exit
        //Wait for 5 secs, but check every second
        const int maxWaitSec = 5;
        int waitSec = 0;
        do 
        {
            QThread::sleep(1); //主线程中唯一的阻塞式调用
            ++waitSec;
            bool allExit = true;
            for (IMediaControlPtr controller : media_controllers_)
            {
                if (NULL == controller || INVALID_HANDLE_VALUE == controller->GetHandle())
                {
                    continue;
                }
                HANDLE handle = controller->GetHandle();
                if (ProcessManager::isRunning(handle))
                {
                    if (maxWaitSec == waitSec)
                    {
                        LOG_WARN("Try to force terminate process %s",
                            controller->GetMediaID().toLocal8Bit().data());
                        ::TerminateProcess(handle, -1);
                    }
                    else
                    {
                        allExit = false;
                    }
                }
            }
            if (allExit)
            {
                break;
            }
        } while (waitSec < 5);        

        media_controllers_.clear();
    }

    LOG_PRINTF("All media processes terminated. Channel dispatcher exit now.");
    QCoreApplication::instance()->quit();
}

void CvChannelDispatcherService::ModifyMultipleChannel( const QString &mediaType, const int type, const QByteArray &param )
{
    QDataStream out( param );
    out.setVersion(QDataStream::Qt_4_4 );

    // TODO:特殊行为，可以改用其它方式实现。目前只有一个，故只用一个if
    if (type == MMT_ShowConsoleWindow) {
        bool is_show = false;
        out >> is_show;
        ShowConsoleWindow(is_show);
    }

    for (auto controller : media_controllers_)
    {
        if (NULL == controller)
        {
            continue;
        }
        if (!mediaType.isEmpty() && mediaType != controller->GetType())
        {
            // mediaType不为空时处理指定类型媒体流
            continue;
        }

        // TODO:可以改用映射表执行动作
        switch (type)
        {
        case MMT_ShowConsoleWindow:
            {
                const int type_spec = ReinterpretActionType(controller->GetType(), type);
                controller->Modify(type_spec, param);
            }
            break;
        default:
            break;
        }
    }
}

void CvChannelDispatcherService::ShowConsoleWindow( const bool is_show )
{
    HWND console_window = GetConsoleWindow();
    ShowWindow(console_window, is_show ? SW_SHOW : SW_HIDE);
    debug_mode_ = is_show;
}

int CvChannelDispatcherService::ReinterpretActionType( const QString &mediaType, const int type )
{
    // TODO:可以采用映射表
    if (STREAMED_MEDIA_SERVICE_NAME == mediaType)
    {
        switch (type)
        {
        case MMT_ShowConsoleWindow:
            return Action_ShowConsoleWindow;
        default:
            return -1;
        }
    }
    if (SCREEN_MEDIA_SERVICE_NAME == mediaType)
    {
        switch (type)
        {
        case MMT_ShowConsoleWindow:
            return ScreenMediaAction_ShowConsoleWindow;
        default:
            return -1;
        }
    }
    if (TEST_MEDIA_SERVICE_NAME == mediaType)
    {
        switch (type)
        {
        case MMT_ShowConsoleWindow:
            return TestMediaAction_ShowConsoleWindow;
        default:
            return -1;
        }
    }
    return -1;
}
