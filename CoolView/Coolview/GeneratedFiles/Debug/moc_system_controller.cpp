/****************************************************************************
** Meta object code from reading C++ file 'system_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../system_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'system_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SystemController_t {
    QByteArrayData data[15];
    char stringdata[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SystemController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SystemController_t qt_meta_stringdata_SystemController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SystemController"
QT_MOC_LITERAL(1, 17, 19), // "LogoutRequestSignal"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 30), // "RequestGetTerminalConfigSignal"
QT_MOC_LITERAL(4, 69, 26), // "ReportTerminalConfigSignal"
QT_MOC_LITERAL(5, 96, 10), // "ConfigDict"
QT_MOC_LITERAL(6, 107, 6), // "config"
QT_MOC_LITERAL(7, 114, 30), // "HandleRestartSystemRequestSlot"
QT_MOC_LITERAL(8, 145, 31), // "HandleShutDownSystemRequestSlot"
QT_MOC_LITERAL(9, 177, 27), // "HandleQuitSystemRequestSlot"
QT_MOC_LITERAL(10, 205, 31), // "HandleSessionStateChangedNotify"
QT_MOC_LITERAL(11, 237, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(12, 270, 5), // "state"
QT_MOC_LITERAL(13, 276, 19), // "ExecutionActionSlot"
QT_MOC_LITERAL(14, 296, 28) // "HandleRecvTerminalConfigSlot"

    },
    "SystemController\0LogoutRequestSignal\0"
    "\0RequestGetTerminalConfigSignal\0"
    "ReportTerminalConfigSignal\0ConfigDict\0"
    "config\0HandleRestartSystemRequestSlot\0"
    "HandleShutDownSystemRequestSlot\0"
    "HandleQuitSystemRequestSlot\0"
    "HandleSessionStateChangedNotify\0"
    "ISessionController::SessionState\0state\0"
    "ExecutionActionSlot\0HandleRecvTerminalConfigSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SystemController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   64,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    0,   66,    2, 0x0a /* Public */,
      10,    1,   67,    2, 0x08 /* Private */,
      13,    0,   70,    2, 0x08 /* Private */,
      14,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void SystemController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SystemController *_t = static_cast<SystemController *>(_o);
        switch (_id) {
        case 0: _t->LogoutRequestSignal(); break;
        case 1: _t->RequestGetTerminalConfigSignal(); break;
        case 2: _t->ReportTerminalConfigSignal((*reinterpret_cast< const ConfigDict(*)>(_a[1]))); break;
        case 3: _t->HandleRestartSystemRequestSlot(); break;
        case 4: _t->HandleShutDownSystemRequestSlot(); break;
        case 5: _t->HandleQuitSystemRequestSlot(); break;
        case 6: _t->HandleSessionStateChangedNotify((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 7: _t->ExecutionActionSlot(); break;
        case 8: _t->HandleRecvTerminalConfigSlot((*reinterpret_cast< const ConfigDict(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SystemController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemController::LogoutRequestSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SystemController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemController::RequestGetTerminalConfigSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (SystemController::*_t)(const ConfigDict & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemController::ReportTerminalConfigSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SystemController::staticMetaObject = {
    { &ISystemController::staticMetaObject, qt_meta_stringdata_SystemController.data,
      qt_meta_data_SystemController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SystemController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SystemController.stringdata))
        return static_cast<void*>(const_cast< SystemController*>(this));
    return ISystemController::qt_metacast(_clname);
}

int SystemController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ISystemController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SystemController::LogoutRequestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void SystemController::RequestGetTerminalConfigSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SystemController::ReportTerminalConfigSignal(const ConfigDict & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
