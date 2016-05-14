#include "CvConfigService.h"
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
#include <dbus/telecontroller/client/TelecontrollerIf.h>


CvConfigService::CvConfigService()
{
	_teleProxy = new TelecontrollerIf( TELECONTROLLER_SERVICE_NAME,
		TELECONTROLLER_SERVICE_OBJECT_PATH, QDBusConnection::sessionBus() );
}

CvConfigService::~CvConfigService()
{
	if( _teleProxy!=NULL )
		delete _teleProxy;
}

void CvConfigService::QueryAllConfig( int subscribe_id )
{

}

void CvConfigService::QueryCurrentConfig( int subscribe_id )
{

}

void CvConfigService::RefreshConfig()
{

}

void CvConfigService::SetCamera( const QString &camera_name, const QString &crossbar_name, const QString &crossbar_type, int video_width, int video_height )
{

}

void CvConfigService::SetIP( const QString &ip )
{

}

void CvConfigService::SetMic( const QString &mic_name, int mic_volumn )
{

}

void CvConfigService::SetSpeaker( const QString &speaker_name, int speaker_volumn )
{

}