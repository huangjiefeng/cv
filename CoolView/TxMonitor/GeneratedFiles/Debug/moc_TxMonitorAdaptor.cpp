/****************************************************************************
** Meta object code from reading C++ file 'TxMonitorAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/txMonitor/service/TxMonitorAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TxMonitorAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TxMonitorAdaptor_t {
    QByteArrayData data[9];
    char stringdata[370];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TxMonitorAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TxMonitorAdaptor_t qt_meta_stringdata_TxMonitorAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TxMonitorAdaptor"
QT_MOC_LITERAL(1, 17, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 33, 30), // "com.dcampus.coolview.txMonitor"
QT_MOC_LITERAL(3, 64, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 84, 248), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 311, 13), // "ReportRecStat"
QT_MOC_LITERAL(6, 325, 0), // ""
QT_MOC_LITERAL(7, 326, 9), // "stat_type"
QT_MOC_LITERAL(8, 336, 11) // "data_garray"

    },
    "TxMonitorAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.txMonitor\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.txMonitor\">\n    <method nam"
    "e=\"ReportRecStat\">\n      <arg direction=\"in\" type=\"i\" name=\"st"
    "at_type\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"data_garra"
    "y\"/>\n    </method>\n  </interface>\n\0"
    "ReportRecStat\0\0stat_type\0data_garray"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TxMonitorAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       1,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // slots: name, argc, parameters, tag, flags
       5,    2,   23,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    7,    8,

       0        // eod
};

void TxMonitorAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TxMonitorAdaptor *_t = static_cast<TxMonitorAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ReportRecStat((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject TxMonitorAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TxMonitorAdaptor.data,
      qt_meta_data_TxMonitorAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TxMonitorAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TxMonitorAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TxMonitorAdaptor.stringdata))
        return static_cast<void*>(const_cast< TxMonitorAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TxMonitorAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
