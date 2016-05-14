#include "MediaControllerFactory.h"
#include "BaseMediaController.h"
#include "StreamMediaController.h"
#include "ScreenMediaController.h"
#include "TestMediaController.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"
#include "dbus/channel/type/screenMedia/common/ScreenMediaServiceCommon.h"
#include "dbus/channel/type/testMedia/common/TestMediaServiceCommon.h"


static void DeleteControllerLater(IMediaControl *obj)
{
    BaseMediaController * base = static_cast<BaseMediaController *>(obj);
    if (base)
    {
        base->Destroy();
    }
}

MediaControllerFactory::MediaControllerFactory(void)
{
}

IMediaControlPtr MediaControllerFactory::GetController( const QString &type, const QString &id )
{
    BaseMediaController * controller = NULL;

    if(type == QString(STREAMED_MEDIA_SERVICE_NAME))
    {
        controller = new StreamMediaController(id);
    }
    else if(type == QString(SCREEN_MEDIA_SERVICE_NAME))
    {
        controller = new ScreenMediaController(id);
    }
    else if(type == QString(TEST_MEDIA_SERVICE_NAME))
    {
        controller = new TestMediaController(id);
    }

    if (controller)
    {
        controller->Initialize();
    }
    return IMediaControlPtr(controller, DeleteControllerLater);
}


