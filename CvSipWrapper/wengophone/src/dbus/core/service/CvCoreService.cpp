#include "stdafx.h"
#include "CvCoreService.h"
#include <dbus/core/service/CvCoreAdaptor.h>
#include <presentation/qt/QtWengoPhone.h>
#include <log.h>
CvCoreService::CvCoreService()
{
	new CvCoreAdaptor( this );
	QDBusConnection connection = QDBusConnection::sessionBus();
	bool res = connection.registerService( CVCORE_SERVICE_NAME );
	res = connection.registerObject( CVCORE_SERVICE_OBJECT_PATH, this );
	if( !res )
		LOG_ERROR( "×¢²áCV_CORE_SERVICEÊ§°Ü");

	_qtWengoPhone = NULL;
}

void CvCoreService::TeleCommand( const QString &command )
{
	if( _qtWengoPhone )
		_qtWengoPhone->commandProcess( command.toUtf8().data() );
}

CvCoreService::~CvCoreService()
{

}