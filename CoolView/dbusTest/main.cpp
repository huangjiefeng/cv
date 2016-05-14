
#include <QtCore/QCoreApplication>
#include "serviceTest.h"
#include "D:\vs2012\trunk\CoolView\include\dbus\dbusTest\serviceTestAdaptor.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QDBusConnection bus =QDBusConnection::sessionBus();
	serviceTest st;
	serviceTestAdaptor myAdaptor(&st);
	if (!bus.registerService("com.dcampus.coolview.dbustest"))
	{
		qDebug() << bus.lastError().message();
		exit(1);
	}
	bus.registerObject("/com/dcampus/coolview/dbustest",&st);


	return a.exec();
}

//#include <QtCore/QCoreApplication>
//
//#include  "test_adaptor.h"
//#include  "test.h"
//
//int main(int argc,char *argv[])
//{
//	QCoreApplication app(argc,argv);
//
//	Test *test = new Test();
//
//	new TestInterfaceAdaptor(test);
//	QDBusConnection conn = QDBusConnection::sessionBus();
//	conn.registerObject("/",test);
//	conn.registerService("com.asianux.btagent");
//
//	return app.exec();
//}
