/****************************************************************************
** Meta object code from reading C++ file 'bfcp_server_proxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpServer/client/bfcp_server_proxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bfcp_server_proxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpServerProxy_t {
    QByteArrayData data[47];
    char stringdata[832];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpServerProxy_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpServerProxy_t qt_meta_stringdata_BfcpServerProxy = {
    {
QT_MOC_LITERAL(0, 0, 15), // "BfcpServerProxy"
QT_MOC_LITERAL(1, 16, 31), // "NotifyServerControlResultSignal"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 16), // "bfcps::ErrorCode"
QT_MOC_LITERAL(4, 66, 10), // "error_code"
QT_MOC_LITERAL(5, 77, 35), // "NotifyConferenceControlResult..."
QT_MOC_LITERAL(6, 113, 8), // "uint32_t"
QT_MOC_LITERAL(7, 122, 13), // "conference_id"
QT_MOC_LITERAL(8, 136, 30), // "NotifyFloorControlResultSignal"
QT_MOC_LITERAL(9, 167, 8), // "uint16_t"
QT_MOC_LITERAL(10, 176, 8), // "floor_id"
QT_MOC_LITERAL(11, 185, 29), // "NotifyUserControlResultSignal"
QT_MOC_LITERAL(12, 215, 7), // "user_id"
QT_MOC_LITERAL(13, 223, 30), // "NotifyChairControlResultSignal"
QT_MOC_LITERAL(14, 254, 25), // "NotifyConferenceIDsSignal"
QT_MOC_LITERAL(15, 280, 23), // "bfcps::ConferenceIDList"
QT_MOC_LITERAL(16, 304, 14), // "conference_ids"
QT_MOC_LITERAL(17, 319, 26), // "NotifyConferenceInfoSignal"
QT_MOC_LITERAL(18, 346, 15), // "conference_info"
QT_MOC_LITERAL(19, 362, 5), // "Start"
QT_MOC_LITERAL(20, 368, 16), // "bfcps::ServerArg"
QT_MOC_LITERAL(21, 385, 3), // "arg"
QT_MOC_LITERAL(22, 389, 4), // "Stop"
QT_MOC_LITERAL(23, 394, 4), // "Quit"
QT_MOC_LITERAL(24, 399, 13), // "AddConference"
QT_MOC_LITERAL(25, 413, 20), // "bfcps::ConferenceArg"
QT_MOC_LITERAL(26, 434, 16), // "RemoveConference"
QT_MOC_LITERAL(27, 451, 16), // "ModifyConference"
QT_MOC_LITERAL(28, 468, 16), // "GetConferenceIDs"
QT_MOC_LITERAL(29, 485, 17), // "GetConferenceInfo"
QT_MOC_LITERAL(30, 503, 8), // "AddFloor"
QT_MOC_LITERAL(31, 512, 15), // "bfcps::FloorArg"
QT_MOC_LITERAL(32, 528, 11), // "RemoveFloor"
QT_MOC_LITERAL(33, 540, 11), // "ModifyFloor"
QT_MOC_LITERAL(34, 552, 7), // "AddUser"
QT_MOC_LITERAL(35, 560, 14), // "bfcps::UserArg"
QT_MOC_LITERAL(36, 575, 10), // "RemoveUser"
QT_MOC_LITERAL(37, 586, 8), // "SetChair"
QT_MOC_LITERAL(38, 595, 11), // "RemoveChair"
QT_MOC_LITERAL(39, 607, 29), // "HandleServerControlResultSlot"
QT_MOC_LITERAL(40, 637, 13), // "control_error"
QT_MOC_LITERAL(41, 651, 33), // "HandleConferenceControlResult..."
QT_MOC_LITERAL(42, 685, 28), // "HandleFloorControlResultSlot"
QT_MOC_LITERAL(43, 714, 27), // "HandleUserControlResultSlot"
QT_MOC_LITERAL(44, 742, 28), // "HandleChairControlResultSlot"
QT_MOC_LITERAL(45, 771, 29), // "HandleConferenceIDsResultSlot"
QT_MOC_LITERAL(46, 801, 30) // "HandleConferenceInfoResultSlot"

    },
    "BfcpServerProxy\0NotifyServerControlResultSignal\0"
    "\0bfcps::ErrorCode\0error_code\0"
    "NotifyConferenceControlResultSignal\0"
    "uint32_t\0conference_id\0"
    "NotifyFloorControlResultSignal\0uint16_t\0"
    "floor_id\0NotifyUserControlResultSignal\0"
    "user_id\0NotifyChairControlResultSignal\0"
    "NotifyConferenceIDsSignal\0"
    "bfcps::ConferenceIDList\0conference_ids\0"
    "NotifyConferenceInfoSignal\0conference_info\0"
    "Start\0bfcps::ServerArg\0arg\0Stop\0Quit\0"
    "AddConference\0bfcps::ConferenceArg\0"
    "RemoveConference\0ModifyConference\0"
    "GetConferenceIDs\0GetConferenceInfo\0"
    "AddFloor\0bfcps::FloorArg\0RemoveFloor\0"
    "ModifyFloor\0AddUser\0bfcps::UserArg\0"
    "RemoveUser\0SetChair\0RemoveChair\0"
    "HandleServerControlResultSlot\0"
    "control_error\0HandleConferenceControlResultSlot\0"
    "HandleFloorControlResultSlot\0"
    "HandleUserControlResultSlot\0"
    "HandleChairControlResultSlot\0"
    "HandleConferenceIDsResultSlot\0"
    "HandleConferenceInfoResultSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpServerProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  159,    2, 0x06 /* Public */,
       5,    2,  162,    2, 0x06 /* Public */,
       8,    3,  167,    2, 0x06 /* Public */,
      11,    3,  174,    2, 0x06 /* Public */,
      13,    4,  181,    2, 0x06 /* Public */,
      14,    2,  190,    2, 0x06 /* Public */,
      17,    3,  195,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    1,  202,    2, 0x0a /* Public */,
      22,    0,  205,    2, 0x0a /* Public */,
      23,    0,  206,    2, 0x0a /* Public */,
      24,    2,  207,    2, 0x0a /* Public */,
      26,    1,  212,    2, 0x0a /* Public */,
      27,    2,  215,    2, 0x0a /* Public */,
      28,    0,  220,    2, 0x0a /* Public */,
      29,    1,  221,    2, 0x0a /* Public */,
      30,    3,  224,    2, 0x0a /* Public */,
      32,    2,  231,    2, 0x0a /* Public */,
      33,    3,  236,    2, 0x0a /* Public */,
      34,    3,  243,    2, 0x0a /* Public */,
      36,    2,  250,    2, 0x0a /* Public */,
      37,    3,  255,    2, 0x0a /* Public */,
      38,    2,  262,    2, 0x0a /* Public */,
      39,    1,  267,    2, 0x08 /* Private */,
      41,    2,  270,    2, 0x08 /* Private */,
      42,    3,  275,    2, 0x08 /* Private */,
      43,    3,  282,    2, 0x08 /* Private */,
      44,    4,  289,    2, 0x08 /* Private */,
      45,    2,  298,    2, 0x08 /* Private */,
      46,    3,  303,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6,    4,    7,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6, 0x80000000 | 9,    4,    7,   10,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6, 0x80000000 | 9,    4,    7,   12,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 9,    4,    7,   10,   12,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 15,    4,   16,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6, QMetaType::QString,    4,    7,   18,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 25,    7,   21,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 25,    7,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 31,    7,   10,   21,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9,    7,   10,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 31,    7,   10,   21,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 35,    7,   12,   21,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9,    7,   12,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 9,    7,   10,   12,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9,    7,   10,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt,   40,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,   40,    7,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,   40,    7,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort, QMetaType::UShort,   40,    7,   10,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   40,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::QString,   40,    7,   18,

       0        // eod
};

void BfcpServerProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpServerProxy *_t = static_cast<BfcpServerProxy *>(_o);
        switch (_id) {
        case 0: _t->NotifyServerControlResultSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1]))); break;
        case 1: _t->NotifyConferenceControlResultSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2]))); break;
        case 2: _t->NotifyFloorControlResultSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3]))); break;
        case 3: _t->NotifyUserControlResultSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3]))); break;
        case 4: _t->NotifyChairControlResultSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< uint16_t(*)>(_a[4]))); break;
        case 5: _t->NotifyConferenceIDsSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< const bfcps::ConferenceIDList(*)>(_a[2]))); break;
        case 6: _t->NotifyConferenceInfoSignal((*reinterpret_cast< bfcps::ErrorCode(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->Start((*reinterpret_cast< const bfcps::ServerArg(*)>(_a[1]))); break;
        case 8: _t->Stop(); break;
        case 9: _t->Quit(); break;
        case 10: _t->AddConference((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< const bfcps::ConferenceArg(*)>(_a[2]))); break;
        case 11: _t->RemoveConference((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 12: _t->ModifyConference((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< const bfcps::ConferenceArg(*)>(_a[2]))); break;
        case 13: _t->GetConferenceIDs(); break;
        case 14: _t->GetConferenceInfo((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 15: _t->AddFloor((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< const bfcps::FloorArg(*)>(_a[3]))); break;
        case 16: _t->RemoveFloor((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 17: _t->ModifyFloor((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< const bfcps::FloorArg(*)>(_a[3]))); break;
        case 18: _t->AddUser((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< const bfcps::UserArg(*)>(_a[3]))); break;
        case 19: _t->RemoveUser((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 20: _t->SetChair((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3]))); break;
        case 21: _t->RemoveChair((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2]))); break;
        case 22: _t->HandleServerControlResultSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->HandleConferenceControlResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 24: _t->HandleFloorControlResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 25: _t->HandleUserControlResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 26: _t->HandleChairControlResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 27: _t->HandleConferenceIDsResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 28: _t->HandleConferenceInfoResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyServerControlResultSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , uint32_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyConferenceControlResultSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , uint32_t , uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyFloorControlResultSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , uint32_t , uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyUserControlResultSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , uint32_t , uint16_t , uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyChairControlResultSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , const bfcps::ConferenceIDList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyConferenceIDsSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (BfcpServerProxy::*_t)(bfcps::ErrorCode , uint32_t , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerProxy::NotifyConferenceInfoSignal)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject BfcpServerProxy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BfcpServerProxy.data,
      qt_meta_data_BfcpServerProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpServerProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpServerProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpServerProxy.stringdata))
        return static_cast<void*>(const_cast< BfcpServerProxy*>(this));
    return QObject::qt_metacast(_clname);
}

int BfcpServerProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void BfcpServerProxy::NotifyServerControlResultSignal(bfcps::ErrorCode _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpServerProxy::NotifyConferenceControlResultSignal(bfcps::ErrorCode _t1, uint32_t _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpServerProxy::NotifyFloorControlResultSignal(bfcps::ErrorCode _t1, uint32_t _t2, uint16_t _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpServerProxy::NotifyUserControlResultSignal(bfcps::ErrorCode _t1, uint32_t _t2, uint16_t _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpServerProxy::NotifyChairControlResultSignal(bfcps::ErrorCode _t1, uint32_t _t2, uint16_t _t3, uint16_t _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpServerProxy::NotifyConferenceIDsSignal(bfcps::ErrorCode _t1, const bfcps::ConferenceIDList & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BfcpServerProxy::NotifyConferenceInfoSignal(bfcps::ErrorCode _t1, uint32_t _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
