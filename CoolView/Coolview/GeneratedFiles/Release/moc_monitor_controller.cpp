/****************************************************************************
** Meta object code from reading C++ file 'monitor_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MonitorController_t {
    QByteArrayData data[12];
    char stringdata[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorController_t qt_meta_stringdata_MonitorController = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MonitorController"
QT_MOC_LITERAL(1, 18, 26), // "RequestBeepForAWhileSignal"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 2), // "ms"
QT_MOC_LITERAL(4, 49, 20), // "RequestRestartSignal"
QT_MOC_LITERAL(5, 70, 21), // "RequestShutDownSignal"
QT_MOC_LITERAL(6, 92, 29), // "HandleRequestHardwareInfoSlot"
QT_MOC_LITERAL(7, 122, 28), // "HandleHardwareInfoNotifySlot"
QT_MOC_LITERAL(8, 151, 12), // "HardwareInfo"
QT_MOC_LITERAL(9, 164, 4), // "info"
QT_MOC_LITERAL(10, 169, 16), // "ResetCPUBeepSlot"
QT_MOC_LITERAL(11, 186, 22) // "ResetMainboardBeepSlot"

    },
    "MonitorController\0RequestBeepForAWhileSignal\0"
    "\0ms\0RequestRestartSignal\0RequestShutDownSignal\0"
    "HandleRequestHardwareInfoSlot\0"
    "HandleHardwareInfoNotifySlot\0HardwareInfo\0"
    "info\0ResetCPUBeepSlot\0ResetMainboardBeepSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    0,   52,    2, 0x06 /* Public */,
       5,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   54,    2, 0x0a /* Public */,
       7,    1,   55,    2, 0x08 /* Private */,
      10,    0,   58,    2, 0x08 /* Private */,
      11,    0,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MonitorController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MonitorController *_t = static_cast<MonitorController *>(_o);
        switch (_id) {
        case 0: _t->RequestBeepForAWhileSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->RequestRestartSignal(); break;
        case 2: _t->RequestShutDownSignal(); break;
        case 3: _t->HandleRequestHardwareInfoSlot(); break;
        case 4: _t->HandleHardwareInfoNotifySlot((*reinterpret_cast< const HardwareInfo(*)>(_a[1]))); break;
        case 5: _t->ResetCPUBeepSlot(); break;
        case 6: _t->ResetMainboardBeepSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MonitorController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MonitorController::RequestBeepForAWhileSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (MonitorController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MonitorController::RequestRestartSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (MonitorController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MonitorController::RequestShutDownSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject MonitorController::staticMetaObject = {
    { &IMonitorController::staticMetaObject, qt_meta_stringdata_MonitorController.data,
      qt_meta_data_MonitorController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MonitorController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorController.stringdata))
        return static_cast<void*>(const_cast< MonitorController*>(this));
    return IMonitorController::qt_metacast(_clname);
}

int MonitorController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IMonitorController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MonitorController::RequestBeepForAWhileSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MonitorController::RequestRestartSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MonitorController::RequestShutDownSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
