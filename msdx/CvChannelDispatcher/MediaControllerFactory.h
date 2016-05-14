#pragma once
#include <QString>

#include "IMediaControl.h"

class MediaControllerFactory
{
public:
    static IMediaControlPtr GetController(const QString &type, const QString &id);

private:
    MediaControllerFactory(void);
};

