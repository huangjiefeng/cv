#ifndef TESTMEDIACONTROLLER_H
#define TESTMEDIACONTROLLER_H

#include "BaseMediaController.h"
#include "dbus/channel/type/testMedia/client/CvTestMediaIf.h"

class TestMediaController : public BaseMediaController
{
    Q_OBJECT

public:
    TestMediaController(const QString &mediaID);
    ~TestMediaController();

    //OVERRIDE
    virtual void Initialize() override;

    //IMPL ISpecMediaControl
    virtual int SpecCreate( const QByteArray &param ) override;
    virtual int SpecModify( const int type, const QByteArray &param ) override;
    virtual int SpecRelease() override;
    virtual int SpecStateChanged( const int state ) override;
    virtual int SpecRunningCheck() { return 0; }

    virtual inline bool isProxyValid() override;

    virtual QString GetType() override;

private:
    QScopedPointer<CvTestMediaIf> media_if_;
};

#endif // TESTMEDIACONTROLLER_H
