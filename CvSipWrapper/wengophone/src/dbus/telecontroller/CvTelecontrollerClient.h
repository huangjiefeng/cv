#pragma  once

#ifdef CVTELECONTROLLERCLIENT_EXPORTS
#define	TELECONTROLLER_API   _declspec(dllexport)
#else
#define TELECONTROLLER_API   _declspec(dllimport)
#endif

#include <QtCore/Qtcore>
#include <dbus/telecontroller/common/TelecontrollerServiceCommon.h>
class TelecontrollerIf;
class TELECONTROLLER_API  CvTelecontrollerClient
{
public:
	static CvTelecontrollerClient*  getInstance()
	{
		static CvTelecontrollerClient client;
		return &client;
	}

	void TeleInfo( QString msg );

private:
	CvTelecontrollerClient();
	~CvTelecontrollerClient();

	TelecontrollerIf*  _teleconIf;
};