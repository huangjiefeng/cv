#ifndef CV_CORE_SERVICE_H
#define CV_CORE_SERVICE_H
#include <QtCore/QtCore>
#include <dbus/core/common/CvCoreServiceCommon.h>
class QtWengoPhone;
class CvCoreService : public QObject
{
	Q_OBJECT
public:
	static CvCoreService* getInstance()
	{
		static CvCoreService service;
		return &service;
	}

	void setQtWengoPhone( QtWengoPhone*  qtWengoPhone )
	{
		_qtWengoPhone = qtWengoPhone;
	}

private:
	CvCoreService();
	~CvCoreService();

	QtWengoPhone*  _qtWengoPhone;
public: // PROPERTIES
public Q_SLOTS: // METHODS
	void TeleCommand(const QString &command);
Q_SIGNALS: // SIGNALS
};
#endif