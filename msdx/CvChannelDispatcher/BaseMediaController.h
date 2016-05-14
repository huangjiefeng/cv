#ifndef BASEMEDIACONTROLLER_H
#define BASEMEDIACONTROLLER_H

#include <windows.h>

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QThread>

#include "IMediaControl.h"
#include "ISpecMediaControl.h"

class BaseMediaController 
    : public QObject
    , public IMediaControl
    , public ISpecMediaControl
{
    Q_OBJECT

protected: // TYPES
    enum MediaProcState
    {
        Stopped = 0, //已停止或从未运行
        StartPending,//等待启动,因为之前的进程尚未结束
        Started,     //已经启动,等待初始化
        Running,     //已初始化,正常运行中
        StopPending  //已发送停止指令,等待停止
    };

public: // METHODS
    BaseMediaController(const QString &mediaID, const QString &type);
    virtual ~BaseMediaController();

    virtual void SetDebugMode(bool debug);

    //初始化.注意应该在派生类中调用基类的此方法已完成默认的初始化.
    virtual void Initialize();
    //删除对象时调用此方法,而非直接delete或调用QObject::deleteLater
    virtual void Destroy();

    //IMPL IMediaControl
    virtual void Create(const QByteArray &param) override;
    virtual void Modify(const int type, const QByteArray &param) override;
    virtual void Release() override;
    virtual void StateChanged(const int state) override;
    virtual HANDLE GetHandle() override;
    virtual QString GetMediaID() override;

Q_SIGNALS:
    void CreateSignal(const QByteArray &param);
    void ModifySignal(const int type, const QByteArray &param);
    void ReleaseSignal();
    void StateChangedSignal(const int state);
    void DestroySignal();

protected:
    void OnExited();
    void SetState(MediaProcState state);
    void BackupData();
    void RestoreData();
    inline bool DoCreate();
    inline bool DoRelease();

protected slots:
    //定义媒体流操作的模板方法,已完成加锁以及状态转移等默认动作
    void HandleCreate(const QByteArray &param);
    void HandleModify(const int type, const QByteArray &param);
    void HandleRelease();
    void HandleDestroy();
    void HandleStateChanged(const int state);
    void HandleCheckerTimeout();

protected: // PROPERTIES

    const QString media_id_; //媒体流进程ID
    const QString media_type_; //类型
    HANDLE proc_handle_; //进程句柄,没有使用QProcess,因为进程恢复后无法恢复QProcess信息

    //进程恢复时需要恢复的数据对象
    MediaProcState state_; //媒体流进程状态,确保只通过SetState方法修改
    QByteArray start_param_; //启动参数
    unsigned long pid_; //进程PID
    //End

    bool debug_mode_; //是否调试模式,用于判断是否显示控制台


private: // PRIVATE PROPERTY

#ifdef _DEBUG
    bool base_class_initialized; //用于检查派生类是否调用了基类的Init方法
#endif

    //QThread thread_;
    QTimer  checker_timer_; //用于检查进程状态

    QTime stop_request_time_; //最后尝试停止的时间,用于停止超时检查
    QTime last_start_time_; //最后一次启动进程的时间,与下一个变量一起防止无限制尝试恢复
    int recent_resume_times_; //最近尝试重启进程的次数
};

#endif // BASEMEDIACONTROLLER_H
