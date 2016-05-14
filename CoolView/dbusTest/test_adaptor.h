#ifndef  TEST_ADAPTOR_H
#define  TEST_ADAPTOR_H

#include  <QtCore/QObject>
#include  <QtDBus/QtDBus>
/****
 **dbus-send --session --print-reply --dest=com.asianux.btagent /  com.asianux.btagent.adaptor.test
 **dbus-monitor --session \ "type='signal',interface='com.asianux.btagent.adaptor',member='stateChange'"
*****/
class TestInterfaceAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.asianux.btagent.adaptor")

    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.asianux.btagent.adaptor\">\n"
                "    <method name=\"test\">\n"
                "      <arg name=\"state\" type=\"s\" direction=\"out\"/>\n"
                "	 </method> \n"
                "	 <signal name=\"stateChange\"> \n"
                "		<arg type=\"s\" direction=\"out\"/>\n"
                "	 </signal> \n"
                "  </interface>\n"
                "")

public:
	TestInterfaceAdaptor(QObject *parent);
	virtual ~TestInterfaceAdaptor();

public:
public slots:
	QString test();

signals:
	void stateChange(QString str);
};

#endif  /*TEST_ADAPTOR_H*/
