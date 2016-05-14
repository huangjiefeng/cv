/****************************************************************************
** Meta object code from reading C++ file 'CvCoreIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/dbus/core/client/CvCoreIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvCoreIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvCoreIf_t {
    QByteArrayData data[8];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvCoreIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvCoreIf_t qt_meta_stringdata_CvCoreIf = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CvCoreIf"
QT_MOC_LITERAL(1, 9, 21), // "CloseCoolViewSlotDbus"
QT_MOC_LITERAL(2, 31, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(3, 51, 0), // ""
QT_MOC_LITERAL(4, 52, 20), // "MoveToMainThreadSlot"
QT_MOC_LITERAL(5, 73, 11), // "TeleCommand"
QT_MOC_LITERAL(6, 85, 13), // "command_index"
QT_MOC_LITERAL(7, 99, 12) // "intput_array"

    },
    "CvCoreIf\0CloseCoolViewSlotDbus\0"
    "QDBusPendingReply<>\0\0MoveToMainThreadSlot\0"
    "TeleCommand\0command_index\0intput_array"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvCoreIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    3, 0x0a /* Public */,
       4,    0,   30,    3, 0x0a /* Public */,
       5,    2,   31,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 2, QMetaType::Int, QMetaType::QByteArray,    6,    7,

       0        // eod
};

void CvCoreIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvCoreIf *_t = static_cast<CvCoreIf *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<> _r = _t->CloseCoolViewSlotDbus();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 1: { QDBusPendingReply<> _r = _t->MoveToMainThreadSlot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<> _r = _t->TeleCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CvCoreIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvCoreIf.data,
      qt_meta_data_CvCoreIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvCoreIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvCoreIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvCoreIf.stringdata))
        return static_cast<void*>(const_cast< CvCoreIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvCoreIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
