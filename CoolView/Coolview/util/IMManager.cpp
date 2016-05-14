#include "immanager.h"
#include <dbus/im/jabber/client/accountproxy.h>

IMManager::IMManager()
{
	_messagerProxy = new CvMsgAccountProxy( JABBER_SERVICE_NAME , JABBER_OBJECT_PATH , QDBusConnection::sessionBus() );
	connect( _messagerProxy , SIGNAL(accountStateChanged(const QString &, int )), this , SLOT(AccountStateChangedSlot(const QString& , int)));
	connect( _messagerProxy, SIGNAL(messageReceived(const QString &, const QString &, const QString &, const QString &, const QString &)),
		this , SLOT(messageReceivedSlot(const QString &, const QString &, const QString &, const QString &, const QString &) ));
	_imState = IM_State_NoInitial;
}
IMManager::~IMManager()
{

}

void IMManager::Login( const QString& userAccount , const QString& password , const int state/*=1 */ )
{
	_imState = IM_State_Processing;
	_messagerProxy->initAccount( userAccount , password );
	_messagerProxy->login( state );
}

void IMManager::Logout( const QString& userAccount )
{
	_imState = IM_State_Logout;
	_messagerProxy->logout();

}

void IMManager::AccountStateChangedSlot( const QString&from , int state )
{
	switch ( state )
	{
	case StateClosed:
		{
		};break;
	case StateOk:
		{
			_imState = IM_State_Login;
		};break;
	case StateProgress:
		{
		};break;
	case StateAuthenticationError:
		{
		};break;
	case StateServerError:
		{
		};break;
	case StateTimeout:
		{
		};break;
	case StateUnknown:
		{
		};break;
	}
}

int IMManager::SendMessage( const QString &to, const QString &subject, const QString &body )
{
	if( _imState!=IM_State_Login )
		return -1;

	_messagerProxy->sendMessage( to , subject , body );

	return 0;
}


void IMManager::messageReceivedSlot( const QString &from, const QString &to, const QString &subject, const QString &body, const QString &time )
{

	jabberMessageReceivedSignal( from , to , subject , body , time );
}

void IMManager::setMainWindow( MainWindow* mainWindow )
{
	_mainWindow = mainWindow;
}