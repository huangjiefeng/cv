/****************************************************************************
** Meta object code from reading C++ file 'CvPerfMonitorAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/performance/monitor/service/CvPerfMonitorAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvPerfMonitorAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvPerfMonitorAdaptor_t {
    QByteArrayData data[20];
    char stringdata[1981];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvPerfMonitorAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvPerfMonitorAdaptor_t qt_meta_stringdata_CvPerfMonitorAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CvPerfMonitorAdaptor"
QT_MOC_LITERAL(1, 21, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 37, 40), // "com.dcampus.coolview.performa..."
QT_MOC_LITERAL(3, 78, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 98, 1655), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 1631, 32), // "cvMonitorCofMessageReceivedEvent"
QT_MOC_LITERAL(6, 1664, 0), // ""
QT_MOC_LITERAL(7, 1665, 14), // "MonitorCofInfo"
QT_MOC_LITERAL(8, 1680, 15), // "CreateTestMedia"
QT_MOC_LITERAL(9, 1696, 8), // "media_id"
QT_MOC_LITERAL(10, 1705, 12), // "input_garray"
QT_MOC_LITERAL(11, 1718, 9), // "HelloInfo"
QT_MOC_LITERAL(12, 1728, 9), // "processID"
QT_MOC_LITERAL(13, 1738, 12), // "processImage"
QT_MOC_LITERAL(14, 1751, 20), // "RegisterMediaProcess"
QT_MOC_LITERAL(15, 1772, 11), // "processInfo"
QT_MOC_LITERAL(16, 1784, 15), // "RegisterProcess"
QT_MOC_LITERAL(17, 1800, 16), // "ReleaseTestMedia"
QT_MOC_LITERAL(18, 1817, 22), // "UnregisterMediaProcess"
QT_MOC_LITERAL(19, 1840, 17) // "UnregisterProcess"

    },
    "CvPerfMonitorAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.performance.monitor\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.performance.monitor\">\n    <"
    "signal name=\"cvMonitorCofMessageReceivedEvent\">\n      <arg directio"
    "n=\"out\" type=\"ay\" name=\"MonitorCofInfo\"/>\n    </signal>&#xd;\n	"
    "&#xd;\n	//向监控进程注册被监控信息&#xd;\n	<method name=\"RegisterProce"
    "ss\">\n      <arg direction=\"in\" type=\"ay\" name=\"processInfo\"/>	"
    "	//MonitoredProcess&#xd;\n	</method>&#xd;\n	&#xd;\n	//向监控进程注册媒"
    "体进程监控信息&#xd;\n	<method name=\"RegisterMediaProcess\">\n      <a"
    "rg direction=\"in\" type=\"ay\" name=\"processInfo\"/>		//MonitoredMed"
    "iaProcess&#xd;\n	</method>&#xd;\n	&#xd;\n   //取消监控进程对指定进程的"
    "监控&#xd;\n	<method name=\"UnregisterProcess\">\n      <arg direction="
    "\"in\" type=\"i\" name=\"processID\"/>\n    </method>&#xd;\n &#xd;\n  "
    "  //取消监控进程对指定进程的监控&#xd;\n	<method name=\"UnregisterMedia"
    "Process\">\n      <arg direction=\"in\" type=\"i\" name=\"processID\"/"
    ">\n    </method>&#xd;\n&#xd;\n	//Hello报文，包含了进程的运行状态的二进"
    "制信息&#xd;\n	<method name=\"HelloInfo\">\n      <arg direction=\"in\""
    " type=\"i\" name=\"processID\"/>\n      <arg direction=\"in\" type=\"a"
    "y\" name=\"processImage\"/>	//进程二进制信息，用于恢复进程&#xd;\n	</me"
    "thod>&#xd;\n	&#xd;\n	//创建测试媒体&#xd;\n	<method name=\"CreateTestMe"
    "dia\">\n      <arg direction=\"in\" type=\"s\" name=\"media_id\"/>\n  "
    "    <!--channel_id -->\n      <arg direction=\"in\" type=\"ay\" name=\""
    "input_garray\"/>\n      <!--params for channel-->\n    </method>&#xd;\n"
    "	&#xd;\n	//销毁测试媒体&#xd;\n	<method name=\"ReleaseTestMedia\">\n   "
    "   <arg direction=\"in\" type=\"s\" name=\"media_id\"/>\n      <!--cha"
    "nnel_id-->\n    </method>\n  </interface>\n\0"
    "cvMonitorCofMessageReceivedEvent\0\0"
    "MonitorCofInfo\0CreateTestMedia\0media_id\0"
    "input_garray\0HelloInfo\0processID\0"
    "processImage\0RegisterMediaProcess\0"
    "processInfo\0RegisterProcess\0"
    "ReleaseTestMedia\0UnregisterMediaProcess\0"
    "UnregisterProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvPerfMonitorAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       8,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    1,   58,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   61,    6, 0x0a /* Public */,
      11,    2,   66,    6, 0x0a /* Public */,
      14,    1,   71,    6, 0x0a /* Public */,
      16,    1,   74,    6, 0x0a /* Public */,
      17,    1,   77,    6, 0x0a /* Public */,
      18,    1,   80,    6, 0x0a /* Public */,
      19,    1,   83,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   12,   13,
    QMetaType::Void, QMetaType::QByteArray,   15,
    QMetaType::Void, QMetaType::QByteArray,   15,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void CvPerfMonitorAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvPerfMonitorAdaptor *_t = static_cast<CvPerfMonitorAdaptor *>(_o);
        switch (_id) {
        case 0: _t->cvMonitorCofMessageReceivedEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->CreateTestMedia((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->HelloInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->RegisterMediaProcess((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->RegisterProcess((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->ReleaseTestMedia((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->UnregisterMediaProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->UnregisterProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvPerfMonitorAdaptor::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvPerfMonitorAdaptor::cvMonitorCofMessageReceivedEvent)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvPerfMonitorAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_CvPerfMonitorAdaptor.data,
      qt_meta_data_CvPerfMonitorAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvPerfMonitorAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvPerfMonitorAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvPerfMonitorAdaptor.stringdata))
        return static_cast<void*>(const_cast< CvPerfMonitorAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CvPerfMonitorAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CvPerfMonitorAdaptor::cvMonitorCofMessageReceivedEvent(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
