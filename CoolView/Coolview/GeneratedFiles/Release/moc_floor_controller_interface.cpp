/****************************************************************************
** Meta object code from reading C++ file 'floor_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../floor_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'floor_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IFloorController_t {
    QByteArrayData data[14];
    char stringdata[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IFloorController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IFloorController_t qt_meta_stringdata_IFloorController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "IFloorController"
QT_MOC_LITERAL(1, 17, 30), // "NotifyFloorRequestStatusSignal"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 8), // "uint16_t"
QT_MOC_LITERAL(4, 58, 7), // "user_id"
QT_MOC_LITERAL(5, 66, 20), // "bfcpc::RequestStatus"
QT_MOC_LITERAL(6, 87, 6), // "status"
QT_MOC_LITERAL(7, 94, 29), // "HandleCreateClientRequestSlot"
QT_MOC_LITERAL(8, 124, 23), // "FloorControlInfoPointer"
QT_MOC_LITERAL(9, 148, 4), // "info"
QT_MOC_LITERAL(10, 153, 30), // "HandleDestroyClientRequestSlot"
QT_MOC_LITERAL(11, 184, 26), // "HandleApplyFloorRequstSlot"
QT_MOC_LITERAL(12, 211, 28), // "HandleAcceptFloorRequestSlot"
QT_MOC_LITERAL(13, 240, 29) // "HandleReleaseFloorRequestSlot"

    },
    "IFloorController\0NotifyFloorRequestStatusSignal\0"
    "\0uint16_t\0user_id\0bfcpc::RequestStatus\0"
    "status\0HandleCreateClientRequestSlot\0"
    "FloorControlInfoPointer\0info\0"
    "HandleDestroyClientRequestSlot\0"
    "HandleApplyFloorRequstSlot\0"
    "HandleAcceptFloorRequestSlot\0"
    "HandleReleaseFloorRequestSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IFloorController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   49,    2, 0x0a /* Public */,
      10,    0,   54,    2, 0x0a /* Public */,
      11,    0,   55,    2, 0x0a /* Public */,
      12,    1,   56,    2, 0x0a /* Public */,
      13,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 8,    4,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void IFloorController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IFloorController *_t = static_cast<IFloorController *>(_o);
        switch (_id) {
        case 0: _t->NotifyFloorRequestStatusSignal((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< bfcpc::RequestStatus(*)>(_a[2]))); break;
        case 1: _t->HandleCreateClientRequestSlot((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< const FloorControlInfoPointer(*)>(_a[2]))); break;
        case 2: _t->HandleDestroyClientRequestSlot(); break;
        case 3: _t->HandleApplyFloorRequstSlot(); break;
        case 4: _t->HandleAcceptFloorRequestSlot((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 5: _t->HandleReleaseFloorRequestSlot((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IFloorController::*_t)(uint16_t , bfcpc::RequestStatus );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IFloorController::NotifyFloorRequestStatusSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject IFloorController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IFloorController.data,
      qt_meta_data_IFloorController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IFloorController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IFloorController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IFloorController.stringdata))
        return static_cast<void*>(const_cast< IFloorController*>(this));
    return QObject::qt_metacast(_clname);
}

int IFloorController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void IFloorController::NotifyFloorRequestStatusSignal(uint16_t _t1, bfcpc::RequestStatus _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
