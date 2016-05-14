#ifndef STREAMMEDIACONTROLLER_H
#define STREAMMEDIACONTROLLER_H

#include "BaseMediaController.h"
#include "dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h"

class StreamMediaController : public BaseMediaController
{
    Q_OBJECT

public:
    StreamMediaController(const QString &mediaID);
    ~StreamMediaController();

    //OVERRIDE
    virtual void Initialize() override;

    //IMPL ISpecMediaControl
    virtual int SpecCreate( const QByteArray &param ) override;
    virtual int SpecModify( const int type, const QByteArray &param ) override;
    virtual int SpecRelease() override;
    virtual int SpecStateChanged( const int state ) override;
    virtual int SpecRunningCheck();

    virtual inline bool isProxyValid() override;

    virtual QString GetType() override;

private:
    QScopedPointer<CvStreamedMediaIf> media_if_; //QScopedPointer限定其只能在内部使用,且不可作为参数传递

    //进程内存占用检查相关
    static const unsigned long kMemoryIncreaseLimit = 300 * (1 << 20); //300M
    time_t init_time_;
    unsigned long init_memory_usage_;
};

#endif // STREAMMEDIACONTROLLER_H
