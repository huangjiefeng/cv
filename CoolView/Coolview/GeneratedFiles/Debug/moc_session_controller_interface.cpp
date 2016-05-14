/****************************************************************************
** Meta object code from reading C++ file 'session_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../session_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'session_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ISessionController_t {
    QByteArrayData data[12];
    char stringdata[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISessionController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISessionController_t qt_meta_stringdata_ISessionController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ISessionController"
QT_MOC_LITERAL(1, 19, 22), // "NotifyLoginErrorSignal"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 30), // "ISessionController::LoginError"
QT_MOC_LITERAL(4, 74, 5), // "error"
QT_MOC_LITERAL(5, 80, 31), // "NotifySessionStateChangedSignal"
QT_MOC_LITERAL(6, 112, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(7, 145, 5), // "state"
QT_MOC_LITERAL(8, 151, 20), // "NotifyRecvSaviSignal"
QT_MOC_LITERAL(9, 172, 3), // "res"
QT_MOC_LITERAL(10, 176, 22), // "HandleLoginRequestSlot"
QT_MOC_LITERAL(11, 199, 23) // "HandleLogoutRequestSlot"

    },
    "ISessionController\0NotifyLoginErrorSignal\0"
    "\0ISessionController::LoginError\0error\0"
    "NotifySessionStateChangedSignal\0"
    "ISessionController::SessionState\0state\0"
    "NotifyRecvSaviSignal\0res\0"
    "HandleLoginRequestSlot\0HandleLogoutRequestSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISessionController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       8,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   48,    2, 0x0a /* Public */,
      11,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Bool,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ISessionController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ISessionController *_t = static_cast<ISessionController *>(_o);
        switch (_id) {
        case 0: _t->NotifyLoginErrorSignal((*reinterpret_cast< ISessionController::LoginError(*)>(_a[1]))); break;
        case 1: _t->NotifySessionStateChangedSignal((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 2: _t->NotifyRecvSaviSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->HandleLoginRequestSlot(); break;
        case 4: _t->HandleLogoutRequestSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ISessionController::*_t)(ISessionController::LoginError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISessionController::NotifyLoginErrorSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ISessionController::*_t)(ISessionController::SessionState );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISessionController::NotifySessionStateChangedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ISessionController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISessionController::NotifyRecvSaviSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ISessionController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ISessionController.data,
      qt_meta_data_ISessionController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ISessionController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISessionController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ISessionController.stringdata))
        return static_cast<void*>(const_cast< ISessionController*>(this));
    return QObject::qt_metacast(_clname);
}

int ISessionController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ISessionController::NotifyLoginErrorSignal(ISessionController::LoginError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ISessionController::NotifySessionStateChangedSignal(ISessionController::SessionState _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ISessionController::NotifyRecvSaviSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
