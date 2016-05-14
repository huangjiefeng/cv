#ifndef IMEDIACONTROL_H
#define IMEDIACONTROL_H

#include <windows.h>

#include <QSharedPointer>

//实现接口以实现控制媒体流进程的基本操作
class IMediaControl
{
public:
    virtual ~IMediaControl() {}

    virtual void SetDebugMode(bool debug) = 0;
    
    //媒体流进程基本操作
    virtual void Create(const QByteArray &param) = 0;
    virtual void Modify(const int type, const QByteArray &param) = 0;
    virtual void Release() = 0;
    virtual void StateChanged(const int state) = 0;

    virtual HANDLE GetHandle() = 0;
    virtual QString GetMediaID() = 0;
    virtual QString GetType() = 0;
};

typedef QSharedPointer<IMediaControl> IMediaControlPtr;

#endif