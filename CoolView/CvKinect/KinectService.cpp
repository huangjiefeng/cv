#include "KinectService.h"
 
#include <dbus/kinect/common/KinectServiceCommon.h>
#include <dbus/kinect/service/kinectAdaptor.h>
#include "Kinect.h"
KinectService::KinectService()
{
	new KinectAdaptor( this );
    
	QDBusConnection connection = QDBusConnection::sessionBus();    
	bool ret = connection.registerService(KINECT_SERVICE_NAME);    
	ret = connection.registerObject( KINECT_OBJECT_PATH, this );
   
	kinect  = new CKinect;
    kinect->Nui_Zero();
	kinect->Nui_Init();   
}

KinectService::~KinectService()
{
	if( kinect )
	{
		kinect->Nui_UnInit( ); 
		delete kinect;
		kinect = NULL;
	}
}

void KinectService::EmitGestureSignal( const QString&to , const KinectGesture type )
{ 
	GestureOccur( to , type );
}

void KinectService::EmitKinectMessage( const KinectMessageType type , const QString&message )
{ 
	KinectMessage( type , message );
}