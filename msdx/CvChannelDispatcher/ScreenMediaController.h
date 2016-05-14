#ifndef SCREENMEDIACONTROLLER_H
#define SCREENMEDIACONTROLLER_H

#include "BaseMediaController.h"
#include "dbus/channel/type/screenMedia/client/CvScreenMediaIf.h"

class ScreenMediaController : public BaseMediaController
{
    Q_OBJECT

public:
    ScreenMediaController(const QString &mediaID);
    ~ScreenMediaController();

    virtual void Initialize() override;

    virtual int SpecCreate( const QByteArray &param ) override;
    virtual int SpecModify( const int type, const QByteArray &param ) override;
    virtual int SpecRelease() override;
    virtual int SpecStateChanged( const int state ) override;
    virtual int SpecRunningCheck() { return 0; }

    virtual inline bool isProxyValid() override;

    virtual QString GetType() override;

private:
    QScopedPointer<CvScreenMediaIf> media_if_; //QScopedPointer限定其只能在内部使用,且不可作为参数传递
};

#endif // SCREENMEDIACONTROLLER_H
