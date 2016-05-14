#ifndef ISPECMEDIACONTROL_H
#define ISPECMEDIACONTROL_H

//实现特定媒体流相关的操作(Specific Media Control)
//与IMediaControl的区别是,外部对象调用其方法完成一些通用动作(如加锁)后,
//根据不同媒体流类型调用本接口相应方法完成具体动作.
class ISpecMediaControl
{
public:
    virtual ~ISpecMediaControl() {}

    virtual int SpecCreate(const QByteArray &param) = 0;
    virtual int SpecModify(const int type, const QByteArray &param) = 0;
    virtual int SpecRelease() = 0;
    virtual int SpecStateChanged(const int state) = 0;
    virtual int SpecRunningCheck() = 0;

    virtual bool isProxyValid() = 0;
};

#endif