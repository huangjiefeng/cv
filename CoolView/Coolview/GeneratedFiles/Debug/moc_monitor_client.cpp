/****************************************************************************
** Meta object code from reading C++ file 'monitor_client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MonitorClient_t {
    QByteArrayData data[8];
    char stringdata[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorClient_t qt_meta_stringdata_MonitorClient = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MonitorClient"
QT_MOC_LITERAL(1, 14, 24), // "NotifyHardwareInfoSignal"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 12), // "HardwareInfo"
QT_MOC_LITERAL(4, 53, 4), // "info"
QT_MOC_LITERAL(5, 58, 29), // "HandleRequestHardwareInfoSlot"
QT_MOC_LITERAL(6, 88, 30), // "HandleRequestBeepForAWhileSlot"
QT_MOC_LITERAL(7, 119, 2) // "ms"

    },
    "MonitorClient\0NotifyHardwareInfoSignal\0"
    "\0HardwareInfo\0info\0HandleRequestHardwareInfoSlot\0"
    "HandleRequestBeepForAWhileSlot\0ms"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   32,    2, 0x0a /* Public */,
       6,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void MonitorClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MonitorClient *_t = static_cast<MonitorClient *>(_o);
        switch (_id) {
        case 0: _t->NotifyHardwareInfoSignal((*reinterpret_cast< const HardwareInfo(*)>(_a[1]))); break;
        case 1: _t->HandleRequestHardwareInfoSlot(); break;
        case 2: _t->HandleRequestBeepForAWhileSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MonitorClient::*_t)(const HardwareInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MonitorClient::NotifyHardwareInfoSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MonitorClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MonitorClient.data,
      qt_meta_data_MonitorClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MonitorClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorClient.stringdata))
        return static_cast<void*>(const_cast< MonitorClient*>(this));
    return QObject::qt_metacast(_clname);
}

int MonitorClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MonitorClient::NotifyHardwareInfoSignal(const HardwareInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
