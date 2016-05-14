/****************************************************************************
** Meta object code from reading C++ file 'scheduler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scheduler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scheduler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Scheduler_t {
    QByteArrayData data[11];
    char stringdata[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Scheduler_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Scheduler_t qt_meta_stringdata_Scheduler = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Scheduler"
QT_MOC_LITERAL(1, 10, 27), // "NotifyNetworkScheduleSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "NetworkInfo"
QT_MOC_LITERAL(4, 51, 4), // "info"
QT_MOC_LITERAL(5, 56, 18), // "weblib_speed_limit"
QT_MOC_LITERAL(6, 75, 26), // "RequestResetWeblibUploader"
QT_MOC_LITERAL(7, 102, 17), // "WeblibSettingInfo"
QT_MOC_LITERAL(8, 120, 7), // "setting"
QT_MOC_LITERAL(9, 128, 26), // "RequestStartWeblibUploader"
QT_MOC_LITERAL(10, 155, 25) // "RequestStopWeblibUploader"

    },
    "Scheduler\0NotifyNetworkScheduleSignal\0"
    "\0NetworkInfo\0info\0weblib_speed_limit\0"
    "RequestResetWeblibUploader\0WeblibSettingInfo\0"
    "setting\0RequestStartWeblibUploader\0"
    "RequestStopWeblibUploader"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Scheduler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    1,   39,    2, 0x06 /* Public */,
       9,    0,   42,    2, 0x06 /* Public */,
      10,    0,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Scheduler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Scheduler *_t = static_cast<Scheduler *>(_o);
        switch (_id) {
        case 0: _t->NotifyNetworkScheduleSignal((*reinterpret_cast< const NetworkInfo(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 1: _t->RequestResetWeblibUploader((*reinterpret_cast< const WeblibSettingInfo(*)>(_a[1]))); break;
        case 2: _t->RequestStartWeblibUploader(); break;
        case 3: _t->RequestStopWeblibUploader(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Scheduler::*_t)(const NetworkInfo & , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::NotifyNetworkScheduleSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (Scheduler::*_t)(const WeblibSettingInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::RequestResetWeblibUploader)) {
                *result = 1;
            }
        }
        {
            typedef void (Scheduler::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::RequestStartWeblibUploader)) {
                *result = 2;
            }
        }
        {
            typedef void (Scheduler::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::RequestStopWeblibUploader)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Scheduler::staticMetaObject = {
    { &BaseLoopJob::staticMetaObject, qt_meta_stringdata_Scheduler.data,
      qt_meta_data_Scheduler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Scheduler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scheduler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Scheduler.stringdata))
        return static_cast<void*>(const_cast< Scheduler*>(this));
    return BaseLoopJob::qt_metacast(_clname);
}

int Scheduler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseLoopJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Scheduler::NotifyNetworkScheduleSignal(const NetworkInfo & _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Scheduler::RequestResetWeblibUploader(const WeblibSettingInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Scheduler::RequestStartWeblibUploader()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Scheduler::RequestStopWeblibUploader()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
