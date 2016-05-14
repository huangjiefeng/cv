/****************************************************************************
** Meta object code from reading C++ file 'HttpWrapper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../HttpWrapper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HttpWrapper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HttpWrapper_t {
    QByteArrayData data[12];
    char stringdata[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HttpWrapper_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HttpWrapper_t qt_meta_stringdata_HttpWrapper = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HttpWrapper"
QT_MOC_LITERAL(1, 12, 22), // "RecvTerminalListSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "TerminalVector"
QT_MOC_LITERAL(4, 51, 32), // "NotifyTerminalListReceivedSignal"
QT_MOC_LITERAL(5, 84, 12), // "TerminalList"
QT_MOC_LITERAL(6, 97, 9), // "terminals"
QT_MOC_LITERAL(7, 107, 23), // "RequestTerminalListSlot"
QT_MOC_LITERAL(8, 131, 3), // "cid"
QT_MOC_LITERAL(9, 135, 17), // "LoginFinishedSlot"
QT_MOC_LITERAL(10, 153, 27), // "GetTerminalListFinishedSlot"
QT_MOC_LITERAL(11, 181, 27) // "HandleTerminalListReplySlot"

    },
    "HttpWrapper\0RecvTerminalListSignal\0\0"
    "TerminalVector\0NotifyTerminalListReceivedSignal\0"
    "TerminalList\0terminals\0RequestTerminalListSlot\0"
    "cid\0LoginFinishedSlot\0GetTerminalListFinishedSlot\0"
    "HandleTerminalListReplySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HttpWrapper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   50,    2, 0x0a /* Public */,
       9,    0,   53,    2, 0x08 /* Private */,
      10,    0,   54,    2, 0x08 /* Private */,
      11,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HttpWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HttpWrapper *_t = static_cast<HttpWrapper *>(_o);
        switch (_id) {
        case 0: _t->RecvTerminalListSignal((*reinterpret_cast< TerminalVector(*)>(_a[1]))); break;
        case 1: _t->NotifyTerminalListReceivedSignal((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 2: _t->RequestTerminalListSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->LoginFinishedSlot(); break;
        case 4: _t->GetTerminalListFinishedSlot(); break;
        case 5: _t->HandleTerminalListReplySlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HttpWrapper::*_t)(TerminalVector );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpWrapper::RecvTerminalListSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (HttpWrapper::*_t)(const TerminalList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpWrapper::NotifyTerminalListReceivedSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject HttpWrapper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HttpWrapper.data,
      qt_meta_data_HttpWrapper,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HttpWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HttpWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HttpWrapper.stringdata))
        return static_cast<void*>(const_cast< HttpWrapper*>(this));
    return QObject::qt_metacast(_clname);
}

int HttpWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void HttpWrapper::RecvTerminalListSignal(TerminalVector _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HttpWrapper::NotifyTerminalListReceivedSignal(const TerminalList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
