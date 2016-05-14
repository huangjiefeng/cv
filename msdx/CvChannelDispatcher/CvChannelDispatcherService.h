#ifndef CV_TUNNEL_DISPATCHER_SERVICE_H
#define CV_TUNNEL_DISPATCHER_SERVICE_H

#include <QtCore/QtCore>

#include "IMediaControl.h"

typedef QMap<QString, IMediaControlPtr> MediaControllerMap;


class CvChannelDispatcherService : public QObject
{
    Q_OBJECT
public:
    CvChannelDispatcherService();
    ~CvChannelDispatcherService();

    /**
     * @brief 初始化.不在构造函数中执行复杂初始化动作,是为防止出现问题时难于跟踪
     * @return 0为初始化成功，否则为其他数值
     */
    int Initial();

public Q_SLOTS: // METHODS
    void ChannelStateChanged(const QString &mediaID, const QString &user_id, const QString &mediaType, int state);
    void CreateChannel(const QString &mediaID, const QString &mediaType, const QByteArray &param);
    void Destroy();
    // ModifyChannel参数说明见cpp文件
    void ModifyChannel(const QString &mediaID, const QString &mediaType, const int type, const QByteArray &param);
    void ReleaseChannel(const QString &mediaID, const QString &mediaType);
Q_SIGNALS: // SIGNALS
    void NofityChannelStateChanged(const QString &channel_id, const QString &user_id, 
        const QString &channel_type, int channel_state);

private: // PRIVATE METHODS
    void RestoreControllers();
    void ModifyMultipleChannel(const QString &mediaType, const int type, const QByteArray &param);
    int  ReinterpretActionType(const QString &mediaType, const int type);
    void ShowConsoleWindow(const bool is_show);
    
private:
    MediaControllerMap media_controllers_;
    BOOL debug_mode_; // 是否开启控制台窗口
    BOOL destroy_; //标记量,在Destroy调用后需防止任何其它调用
    QReadWriteLock lock_; //此处用了一个读写锁,目的是保证Destroy调用时,可唯一的修改标记量,其他方法均不可调用.
                          //其他时候,任何其他方法都可以无限制的并行调用.
                          //另注:由于Qt DBus Adapter是在单线程中(此处为主消息循环)基于
                          //queued signal/slot connections方式运行的,故加锁其实无意义.
                          //Liaokz, 2013-11
    
};
#endif