/****************************************************************************
** Meta object code from reading C++ file 'TelecontrollerIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/telecontroller/client/TelecontrollerIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TelecontrollerIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TelecontrollerIf_t {
    QByteArrayData data[7];
    char stringdata[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TelecontrollerIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TelecontrollerIf_t qt_meta_stringdata_TelecontrollerIf = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TelecontrollerIf"
QT_MOC_LITERAL(1, 17, 8), // "TeleInfo"
QT_MOC_LITERAL(2, 26, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(3, 46, 0), // ""
QT_MOC_LITERAL(4, 47, 10), // "info_index"
QT_MOC_LITERAL(5, 58, 12), // "subscribe_id"
QT_MOC_LITERAL(6, 71, 11) // "input_array"

    },
    "TelecontrollerIf\0TeleInfo\0QDBusPendingReply<>\0"
    "\0info_index\0subscribe_id\0input_array"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TelecontrollerIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    4,    5,    6,

       0        // eod
};

void TelecontrollerIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TelecontrollerIf *_t = static_cast<TelecontrollerIf *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<> _r = _t->TeleInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject TelecontrollerIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_TelecontrollerIf.data,
      qt_meta_data_TelecontrollerIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TelecontrollerIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TelecontrollerIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TelecontrollerIf.stringdata))
        return static_cast<void*>(const_cast< TelecontrollerIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int TelecontrollerIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
