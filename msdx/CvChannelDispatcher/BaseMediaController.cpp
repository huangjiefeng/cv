/************************************************************************/
/* 从实现可以看出，这其实是一个状态机，但是代码量不大，懒得做的漂亮了， */
/* 直接switch好了。                                                     */
/************************************************************************/

#include <assert.h>
#include <windows.h>

#include <QCoreApplication>
#include <QFile>
#include <QDataStream>

#include "BaseMediaController.h"
#include "util/ProcessManager.h"
#include "util/Backupmanager/BackupManager.h"
#include "log/Log.h"


#define PROC_TERMINATE_TIMEOUT 5 //进程结束超时检查,(s)
#define PROC_CHECK_INTERVAL 500 //进程检查间隔,(ms)


//Impl of BaseMediaController methods

BaseMediaController::BaseMediaController(const QString &mediaID, const QString &type)
    : QObject(NULL)
    , media_id_(mediaID)
    , media_type_(type)
    , state_(Stopped)
    , pid_(0)
    , debug_mode_(false)
    , checker_timer_(this) //设置父对象以使其在相同线程中执行
    , proc_handle_(INVALID_HANDLE_VALUE)
    , recent_resume_times_(0)
{
#ifdef _DEBUG
    base_class_initialized = false;
#endif
}

BaseMediaController::~BaseMediaController()
{
    //thread_.quit();
    //thread_.wait();
}

void BaseMediaController::Initialize()
{
    LOG_PRINTF("Create controller for %s.", media_id_.toLocal8Bit().data());
#ifdef _DEBUG
    base_class_initialized = true;
#endif
    debug_mode_ = false;
    checker_timer_.setInterval(PROC_CHECK_INTERVAL);

    //connect signals
    connect(this, &BaseMediaController::CreateSignal,
        this, &BaseMediaController::HandleCreate);
    connect(this, &BaseMediaController::ModifySignal,
        this, &BaseMediaController::HandleModify);
    connect(this, &BaseMediaController::ReleaseSignal,
        this, &BaseMediaController::HandleRelease);
    connect(this, &BaseMediaController::StateChangedSignal,
        this, &BaseMediaController::HandleStateChanged);
    connect(this, &BaseMediaController::DestroySignal,
        this, &BaseMediaController::HandleDestroy);
    connect(&checker_timer_, &QTimer::timeout, 
        this, &BaseMediaController::HandleCheckerTimeout);

    //try to restore data if backup exist
    RestoreData();

    //set my slots to execute in a separate thread
    //moveToThread(&thread_);
    //thread_.start();
}

void BaseMediaController::SetDebugMode(bool debug)
{
    debug_mode_ = debug;
}

void BaseMediaController::Destroy()
{
    emit DestroySignal();
}

void BaseMediaController::HandleCreate( const QByteArray &param )
{
    assert(base_class_initialized);

    start_param_ = param;

    switch (state_)
    {
    case BaseMediaController::Stopped:
        SetState(StartPending);
        //尝试创建进程
        if (DoCreate())
        {
            SetState(Started);
        } 
        else
        {
            SetState(Stopped);
        }
        break;

    case BaseMediaController::StartPending:
    case BaseMediaController::Started:
    case BaseMediaController::StopPending:
        //这些状态下参数都尚未发送,只更新参数即可
        LOG_PRINTF("Create process %s scheduled, update parameters.",
            media_id_.toLocal8Bit().data());
        SetState(StartPending);
        break;

    case BaseMediaController::Running:
        LOG_WARN("Create process %s while running, release the previous one.",
            media_id_.toLocal8Bit().data());
        DoRelease();
        SetState(StartPending);
        break;
    default:
        break;
    }
}

void BaseMediaController::HandleModify( const int type, const QByteArray &param )
{
    assert(base_class_initialized);
    
    switch (state_)
    {
    case BaseMediaController::Stopped:
    case BaseMediaController::StartPending:
    case BaseMediaController::Started:
    case BaseMediaController::StopPending:
        LOG_WARN("Try to modify %s before it is running.", media_id_.toLocal8Bit().data());
        break;
    case BaseMediaController::Running:
        LOG_PRINTF("Modify process %s", media_id_.toLocal8Bit().data());
        SpecModify(type, param);
        break;
    default:
        break;
    }
}

void BaseMediaController::HandleRelease()
{
    assert(base_class_initialized);

    switch (state_)
    {
    case BaseMediaController::StopPending:
    case BaseMediaController::Stopped:
        //已经发送停止指令或停止,不执行
        break;
    case BaseMediaController::StartPending:
        //在启动挂起时就要求停止,则切换为等待停止以取消挂起的启动操作
        LOG_PRINTF("Release process %s in start pending, cancel scheduled start.", media_id_.toLocal8Bit().data());
        SetState(StopPending);
        break;
    case BaseMediaController::Started:
    case BaseMediaController::Running:
        //尝试正常结束之
        SetState(StopPending);
        DoRelease();
        break;
    default:
        break;
    }
}

void BaseMediaController::HandleStateChanged( int state )
{
    assert(base_class_initialized);
    
    switch (state_)
    {
    case BaseMediaController::Stopped:
        //未正常运行,忽略状态变更通知
        LOG_WARN("Receive state change notify of %s, but it is not running.", 
            media_id_.toLocal8Bit().data());
        break;
    case BaseMediaController::StopPending:
    case BaseMediaController::StartPending:
    case BaseMediaController::Started:
    case BaseMediaController::Running:
        LOG_PRINTF("Process %s state change to %d.", 
            media_id_.toLocal8Bit().data(), state);
        SpecStateChanged(state);
        break;
    default:
        break;
    }
}

void BaseMediaController::HandleCheckerTimeout()
{
    if (INVALID_HANDLE_VALUE == proc_handle_)
    {
        //意外情况,只能是进程未创建或已经停止
        LOG_ERROR("Check %s error. Invalid handle.", media_id_.toLocal8Bit().data());
        SetState(Stopped);
        checker_timer_.stop();
        return;
    }
    if (!ProcessManager::isRunning(proc_handle_))
    {
        //已停止
        OnExited();
    } 
    else if ((StopPending == state_ || StartPending == state_) && 
        PROC_TERMINATE_TIMEOUT <= stop_request_time_.secsTo(QTime::currentTime()))
    {
        //停止操作已超时（Debug时不强制关闭，便于调试）
#ifndef _DEBUG
        LOG_WARN("Process %s(PID=%d) exit timeout. Try to terminate.", media_id_.toLocal8Bit().data(), pid_);
        ::TerminateProcess(proc_handle_, -1);
#endif // !_DEBUG
        stop_request_time_ = QTime::currentTime(); //重置超时检查时间
    }
    else if (Running == state_) {
        SpecRunningCheck();
    }
}

void BaseMediaController::OnExited()
{
    checker_timer_.stop();
    if (INVALID_HANDLE_VALUE != proc_handle_)
    {
        DWORD exitCode = 0;
        GetExitCodeProcess(proc_handle_, &exitCode);
        LOG_PRINTF("Process %s(PID=%d) exit. Return code: %d", 
            media_id_.toLocal8Bit().data(), pid_, exitCode);
        CloseHandle(proc_handle_);
        proc_handle_ = INVALID_HANDLE_VALUE;
    }
    else
    {
        LOG_ERROR("Process %s(PID=%d) exit, handle invalid.", 
            media_id_.toLocal8Bit().data(), pid_);
    }
    pid_ = 0;

    switch (state_)
    {
    case BaseMediaController::Stopped:
    case BaseMediaController::StopPending:
        SetState(Stopped);
        break;

    case BaseMediaController::Started:
    case BaseMediaController::Running:
        LOG_ERROR("Process %s exit unexpectedly. Try to resume.", 
            media_id_.toLocal8Bit().data());
        //Do the same work as in StartPending state

    case BaseMediaController::StartPending:
        if (DoCreate())
        {
            SetState(Started);
        } 
        else
        {
            SetState(Stopped);
        }

        break;
    default:
        break;
    }
}

void BaseMediaController::HandleDestroy()
{
    HandleRelease();
    //由于controller在其内部线程中运行,故删除前要将其移动到主线程
    //否则将不会有消息循环处理controller的deleteLater请求
    //moveToThread(QCoreApplication::instance()->thread());
    deleteLater();
}

void BaseMediaController::Create( const QByteArray &param )
{
    //controller在自己的线程中运行,故来自主线程的调用需要使用信号/槽关联.
    //之所以采用这种看上去麻烦的方式,是为了确保主线程不被阻塞,以更快的响应
    //来自DBus的其它调用请求.否则,一旦主线程在某个controller中阻塞,对其他
    //controller的操作也将无法立即完成,直至当前的阻塞解除.
    emit CreateSignal(param);
}

void BaseMediaController::Modify( const int type, const QByteArray &param )
{
    emit ModifySignal(type, param);
}

void BaseMediaController::Release()
{
    emit ReleaseSignal();
}

void BaseMediaController::StateChanged(const int state)
{
    emit StateChangedSignal(state);
}

//此方法不是线程安全的.不加锁是防止CvChannelDispatcherService::Destroy中死锁
HANDLE BaseMediaController::GetHandle()
{
    return proc_handle_;
}

//此方法不是线程安全的.
QString BaseMediaController::GetMediaID()
{
    return media_id_;
}

void BaseMediaController::SetState(MediaProcState state)
{
    BackupData();
    state_ = state;
}

void BaseMediaController::BackupData()
{
    BackupManagerPtr backupMgr = BackupManager::Instance();
    if (NULL == backupMgr)
    {
        return;
    }
    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream << state_ << start_param_ << (quint32)pid_; //TODO: 注意64bit类型
    backupMgr->Backup(media_id_, media_type_, 0, data); //阻塞式调用
}

void BaseMediaController::RestoreData()
{
    BackupManagerPtr backupMgr = BackupManager::Instance();
    if (NULL == backupMgr)
    {
        return;
    }
    //还原备份数据
    QByteArray data;
    if (0 != backupMgr->FetchData(media_id_, 0, data)) //阻塞式调用
    {
        if (isProxyValid())
        {
            LOG_WARN("Detect %s out of control.", 
                media_id_.toLocal8Bit().data());
        }
        return;
    }

    if (data.size())
    {
        QDataStream stream(data);
        quint32 temp_pid;
        stream >> (qint32&)state_ >> start_param_ >> temp_pid; //TODO: 注意64bit类型
        pid_ = temp_pid;
        proc_handle_ = (pid_ ? ProcessManager::getHandleByPID(pid_) : INVALID_HANDLE_VALUE);
        LOG_INFO("Restore %s succeed: \n\tState:%d \n\tParam:%dBytes \n\tPID  :%d", 
            media_id_.toLocal8Bit().data(),
            state_,
            start_param_.size(),
            pid_);
    }

    //restore data according to state
    switch (state_)
    {
    case BaseMediaController::Stopped:
        break;
    case BaseMediaController::StartPending:
        if (isProxyValid()) //进程存在,尝试结束它
        {
            DoRelease();
        }
        //break;
    case BaseMediaController::StopPending:
        stop_request_time_ = QTime::currentTime(); //假设当前时间是停止指令发出时间
        //break;
    case BaseMediaController::Started:
    case BaseMediaController::Running:
        checker_timer_.start();
        break;
    default:
        break;
    }
}

bool BaseMediaController::DoCreate()
{
    if (0 > SpecCreate(start_param_))
    {
        //创建失败
        LOG_ERROR("Create process %s failed.", media_id_.toLocal8Bit().data());
        return false;
    }
    pid_ = ProcessManager::getPIDByHandle(proc_handle_);
    last_start_time_ = QTime::currentTime();
    checker_timer_.start();
    LOG_PRINTF("Create process %s(PID=%d) succeed. Start parameter data size is %d bytes.", 
        media_id_.toLocal8Bit().data(), pid_, start_param_.size());
    return true;
}

bool BaseMediaController::DoRelease()
{
    if (0 > SpecRelease())
    {
        //失败,尝试强制结束
        if (INVALID_HANDLE_VALUE != proc_handle_)
        {
            LOG_WARN("Release process %s failed. Try to terminate.", media_id_.toLocal8Bit().data());
            ::TerminateProcess(proc_handle_, -1);
        }
    }
    else
    {
        LOG_PRINTF("Release process %s scheduled.", media_id_.toLocal8Bit().data());
    }
    stop_request_time_ = QTime::currentTime(); //重置超时检查时间
    return true;
}
