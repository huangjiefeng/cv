/****************************************************************************
** Meta object code from reading C++ file 'TelecontrollerAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/telecontroller/service/TelecontrollerAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TelecontrollerAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TelecontrollerAdaptor_t {
    QByteArrayData data[10];
    char stringdata[537];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TelecontrollerAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TelecontrollerAdaptor_t qt_meta_stringdata_TelecontrollerAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 21), // "TelecontrollerAdaptor"
QT_MOC_LITERAL(1, 22, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 38, 46), // "com.dcampus.coolview.controll..."
QT_MOC_LITERAL(3, 85, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 105, 385), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 462, 8), // "TeleInfo"
QT_MOC_LITERAL(6, 471, 0), // ""
QT_MOC_LITERAL(7, 472, 10), // "info_index"
QT_MOC_LITERAL(8, 483, 12), // "subscribe_id"
QT_MOC_LITERAL(9, 496, 11) // "input_array"

    },
    "TelecontrollerAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.controller.telecontroller\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.controller.telecontroller\">\n"
    "    <method name=\"TeleInfo\">\n      <arg direction=\"in\" type=\"i\""
    " name=\"info_index\"/>  //tele info index&#xd;\n	   <arg direction=\"i"
    "n\" type=\"i\" name=\"subscribe_id\"/>		//Tele info to specific contro"
    "ller&#xd;\n	   <arg direction=\"in\" type=\"ay\" name=\"input_array\"/"
    ">\n    </method>\n  </interface>\n\0"
    "TeleInfo\0\0info_index\0subscribe_id\0"
    "input_array"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TelecontrollerAdaptor[] = {

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
       5,    3,   23,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    7,    8,    9,

       0        // eod
};

void TelecontrollerAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TelecontrollerAdaptor *_t = static_cast<TelecontrollerAdaptor *>(_o);
        switch (_id) {
        case 0: _t->TeleInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject TelecontrollerAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TelecontrollerAdaptor.data,
      qt_meta_data_TelecontrollerAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TelecontrollerAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TelecontrollerAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TelecontrollerAdaptor.stringdata))
        return static_cast<void*>(const_cast< TelecontrollerAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TelecontrollerAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
