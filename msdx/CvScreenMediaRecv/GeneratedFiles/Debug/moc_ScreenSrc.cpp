/****************************************************************************
** Meta object code from reading C++ file 'ScreenSrc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ScreenSrc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScreenSrc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScreenSrc_t {
    QByteArrayData data[12];
    char stringdata[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScreenSrc_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScreenSrc_t qt_meta_stringdata_ScreenSrc = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ScreenSrc"
QT_MOC_LITERAL(1, 10, 9), // "dealError"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(4, 50, 5), // "error"
QT_MOC_LITERAL(5, 56, 20), // "dealSocketConnection"
QT_MOC_LITERAL(6, 77, 16), // "imageBufferReady"
QT_MOC_LITERAL(7, 94, 11), // "processData"
QT_MOC_LITERAL(8, 106, 3), // "key"
QT_MOC_LITERAL(9, 110, 11), // "const BYTE*"
QT_MOC_LITERAL(10, 122, 4), // "data"
QT_MOC_LITERAL(11, 127, 10) // "dataLength"

    },
    "ScreenSrc\0dealError\0\0QAbstractSocket::SocketError\0"
    "error\0dealSocketConnection\0imageBufferReady\0"
    "processData\0key\0const BYTE*\0data\0"
    "dataLength"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScreenSrc[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,
       6,    0,   38,    2, 0x08 /* Private */,
       7,    3,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, QMetaType::Int, 0x80000000 | 9, QMetaType::Int,    8,   10,   11,

       0        // eod
};

void ScreenSrc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScreenSrc *_t = static_cast<ScreenSrc *>(_o);
        switch (_id) {
        case 0: _t->dealError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: _t->dealSocketConnection(); break;
        case 2: _t->imageBufferReady(); break;
        case 3: { int _r = _t->processData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const BYTE*(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject ScreenSrc::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ScreenSrc.data,
      qt_meta_data_ScreenSrc,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ScreenSrc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenSrc::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ScreenSrc.stringdata))
        return static_cast<void*>(const_cast< ScreenSrc*>(this));
    return QObject::qt_metacast(_clname);
}

int ScreenSrc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
