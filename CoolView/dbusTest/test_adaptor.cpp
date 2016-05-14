#include	"test_adaptor.h"
#include	<QtCore>

TestInterfaceAdaptor::TestInterfaceAdaptor(QObject *parent)
	:QDBusAbstractAdaptor(parent)
{
	qDebug() << "***************TestInterfaceAdaptor::TestInterfaceAdaptor**************";
	setAutoRelaySignals(true);  //connection of the signals on the parent
}

TestInterfaceAdaptor::~TestInterfaceAdaptor()
{
}

QString TestInterfaceAdaptor::test()
{
	QString out;
	QMetaObject::invokeMethod(parent(), "testStart",Q_RETURN_ARG(QString, out));
	qDebug() << "==========" << out;
	return out;
}
