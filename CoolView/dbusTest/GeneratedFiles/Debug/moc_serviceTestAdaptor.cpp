/****************************************************************************
** Meta object code from reading C++ file 'serviceTestAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/dbusTest/serviceTestAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serviceTestAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_serviceTestAdaptor_t {
    QByteArrayData data[7];
    char stringdata[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_serviceTestAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_serviceTestAdaptor_t qt_meta_stringdata_serviceTestAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 18), // "serviceTestAdaptor"
QT_MOC_LITERAL(1, 19, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 35, 29), // "com.dcampus.coolview.dbustest"
QT_MOC_LITERAL(3, 65, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 85, 160), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 233, 8), // "DbusFunc"
QT_MOC_LITERAL(6, 242, 0) // ""

    },
    "serviceTestAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.dbustest\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.dbustest\">\n    <method name"
    "=\"DbusFunc\">\n      <arg direction=\"out\" type=\"i\"/>\n    </metho"
    "d>\n  </interface>\n\0"
    "DbusFunc\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_serviceTestAdaptor[] = {

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
       5,    0,   23,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int,

       0        // eod
};

void serviceTestAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        serviceTestAdaptor *_t = static_cast<serviceTestAdaptor *>(_o);
        switch (_id) {
        case 0: { int _r = _t->DbusFunc();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject serviceTestAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_serviceTestAdaptor.data,
      qt_meta_data_serviceTestAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *serviceTestAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *serviceTestAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_serviceTestAdaptor.stringdata))
        return static_cast<void*>(const_cast< serviceTestAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int serviceTestAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
