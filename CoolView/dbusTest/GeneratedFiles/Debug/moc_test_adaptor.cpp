/****************************************************************************
** Meta object code from reading C++ file 'test_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../test_adaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TestInterfaceAdaptor_t {
    QByteArrayData data[9];
    char stringdata[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestInterfaceAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestInterfaceAdaptor_t qt_meta_stringdata_TestInterfaceAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 20), // "TestInterfaceAdaptor"
QT_MOC_LITERAL(1, 21, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 37, 27), // "com.asianux.btagent.adaptor"
QT_MOC_LITERAL(3, 65, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 85, 255), // "  <interface name=\"com.asian..."
QT_MOC_LITERAL(5, 317, 11), // "stateChange"
QT_MOC_LITERAL(6, 329, 0), // ""
QT_MOC_LITERAL(7, 330, 3), // "str"
QT_MOC_LITERAL(8, 334, 4) // "test"

    },
    "TestInterfaceAdaptor\0D-Bus Interface\0"
    "com.asianux.btagent.adaptor\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.asianux.btagent.adaptor\">\n    <method name=\""
    "test\">\n      <arg name=\"state\" type=\"s\" direction=\"out\"/>\n	 <"
    "/method> \n	 <signal name=\"stateChange\"> \n		<arg type=\"s\" directi"
    "on=\"out\"/>\n	 </signal> \n  </interface>\n\0"
    "stateChange\0\0str\0test"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestInterfaceAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       2,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    1,   28,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   31,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::QString,

       0        // eod
};

void TestInterfaceAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TestInterfaceAdaptor *_t = static_cast<TestInterfaceAdaptor *>(_o);
        switch (_id) {
        case 0: _t->stateChange((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: { QString _r = _t->test();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TestInterfaceAdaptor::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TestInterfaceAdaptor::stateChange)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject TestInterfaceAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TestInterfaceAdaptor.data,
      qt_meta_data_TestInterfaceAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TestInterfaceAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestInterfaceAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TestInterfaceAdaptor.stringdata))
        return static_cast<void*>(const_cast< TestInterfaceAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TestInterfaceAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TestInterfaceAdaptor::stateChange(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
