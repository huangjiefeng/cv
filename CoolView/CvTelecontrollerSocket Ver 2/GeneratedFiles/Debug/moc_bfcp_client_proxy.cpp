/****************************************************************************
** Meta object code from reading C++ file 'bfcp_client_proxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpClient/client/bfcp_client_proxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bfcp_client_proxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpClientProxy_t {
    QByteArrayData data[46];
    char stringdata[849];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpClientProxy_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpClientProxy_t qt_meta_stringdata_BfcpClientProxy = {
    {
QT_MOC_LITERAL(0, 0, 15), // "BfcpClientProxy"
QT_MOC_LITERAL(1, 16, 34), // "NotifyClientControlSuccessedS..."
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 9), // "client_id"
QT_MOC_LITERAL(4, 62, 17), // "bfcpc::ClientArg&"
QT_MOC_LITERAL(5, 80, 3), // "arg"
QT_MOC_LITERAL(6, 84, 30), // "NotifyClientAlreadyExistSignal"
QT_MOC_LITERAL(7, 115, 26), // "NotifyClientNotExistSignal"
QT_MOC_LITERAL(8, 142, 23), // "NotifyStateChangeSignal"
QT_MOC_LITERAL(9, 166, 18), // "bfcpc::ClientState"
QT_MOC_LITERAL(10, 185, 5), // "state"
QT_MOC_LITERAL(11, 191, 25), // "NotifyErrorReceivedSignal"
QT_MOC_LITERAL(12, 217, 20), // "bfcpc::ResponseError"
QT_MOC_LITERAL(13, 238, 5), // "error"
QT_MOC_LITERAL(14, 244, 28), // "NotifyFloorRequestInfoSignal"
QT_MOC_LITERAL(15, 273, 23), // "bfcpc::FloorRequestInfo"
QT_MOC_LITERAL(16, 297, 4), // "info"
QT_MOC_LITERAL(17, 302, 22), // "NotifyUserStatusSignal"
QT_MOC_LITERAL(18, 325, 17), // "bfcpc::UserStatus"
QT_MOC_LITERAL(19, 343, 6), // "status"
QT_MOC_LITERAL(20, 350, 23), // "NotifyFloorStatusSignal"
QT_MOC_LITERAL(21, 374, 18), // "bfcpc::FloorStatus"
QT_MOC_LITERAL(22, 393, 12), // "CreateClient"
QT_MOC_LITERAL(23, 406, 16), // "bfcpc::ClientArg"
QT_MOC_LITERAL(24, 423, 13), // "DestroyClient"
QT_MOC_LITERAL(25, 437, 16), // "SendFloorRequest"
QT_MOC_LITERAL(26, 454, 22), // "bfcpc::FloorRequestArg"
QT_MOC_LITERAL(27, 477, 16), // "SendFloorRelease"
QT_MOC_LITERAL(28, 494, 16), // "floor_request_id"
QT_MOC_LITERAL(29, 511, 21), // "SendFloorRequestQuery"
QT_MOC_LITERAL(30, 533, 13), // "SendUserQuery"
QT_MOC_LITERAL(31, 547, 19), // "bfcpc::UserQueryArg"
QT_MOC_LITERAL(32, 567, 14), // "SendFloorQuery"
QT_MOC_LITERAL(33, 582, 18), // "bfcpc::FloorIDList"
QT_MOC_LITERAL(34, 601, 15), // "SendChairAction"
QT_MOC_LITERAL(35, 617, 21), // "bfcpc::ChairActionArg"
QT_MOC_LITERAL(36, 639, 9), // "SendHello"
QT_MOC_LITERAL(37, 649, 11), // "SendGoodbye"
QT_MOC_LITERAL(38, 661, 29), // "HandleClientControlResultSlot"
QT_MOC_LITERAL(39, 691, 13), // "control_error"
QT_MOC_LITERAL(40, 705, 4), // "data"
QT_MOC_LITERAL(41, 710, 22), // "HandleStateChangedSlot"
QT_MOC_LITERAL(42, 733, 29), // "HandleErrorReceivedResultSlot"
QT_MOC_LITERAL(43, 763, 26), // "HandleFloorRequestInfoSlot"
QT_MOC_LITERAL(44, 790, 28), // "HandleUserStatusReceivedSlot"
QT_MOC_LITERAL(45, 819, 29) // "HandleFloorStatusReceivedSlot"

    },
    "BfcpClientProxy\0NotifyClientControlSuccessedSignal\0"
    "\0client_id\0bfcpc::ClientArg&\0arg\0"
    "NotifyClientAlreadyExistSignal\0"
    "NotifyClientNotExistSignal\0"
    "NotifyStateChangeSignal\0bfcpc::ClientState\0"
    "state\0NotifyErrorReceivedSignal\0"
    "bfcpc::ResponseError\0error\0"
    "NotifyFloorRequestInfoSignal\0"
    "bfcpc::FloorRequestInfo\0info\0"
    "NotifyUserStatusSignal\0bfcpc::UserStatus\0"
    "status\0NotifyFloorStatusSignal\0"
    "bfcpc::FloorStatus\0CreateClient\0"
    "bfcpc::ClientArg\0DestroyClient\0"
    "SendFloorRequest\0bfcpc::FloorRequestArg\0"
    "SendFloorRelease\0floor_request_id\0"
    "SendFloorRequestQuery\0SendUserQuery\0"
    "bfcpc::UserQueryArg\0SendFloorQuery\0"
    "bfcpc::FloorIDList\0SendChairAction\0"
    "bfcpc::ChairActionArg\0SendHello\0"
    "SendGoodbye\0HandleClientControlResultSlot\0"
    "control_error\0data\0HandleStateChangedSlot\0"
    "HandleErrorReceivedResultSlot\0"
    "HandleFloorRequestInfoSlot\0"
    "HandleUserStatusReceivedSlot\0"
    "HandleFloorStatusReceivedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpClientProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  134,    2, 0x06 /* Public */,
       6,    2,  139,    2, 0x06 /* Public */,
       7,    1,  144,    2, 0x06 /* Public */,
       8,    2,  147,    2, 0x06 /* Public */,
      11,    2,  152,    2, 0x06 /* Public */,
      14,    2,  157,    2, 0x06 /* Public */,
      17,    2,  162,    2, 0x06 /* Public */,
      20,    2,  167,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    1,  172,    2, 0x0a /* Public */,
      24,    1,  175,    2, 0x0a /* Public */,
      25,    2,  178,    2, 0x0a /* Public */,
      27,    2,  183,    2, 0x0a /* Public */,
      29,    2,  188,    2, 0x0a /* Public */,
      30,    2,  193,    2, 0x0a /* Public */,
      32,    2,  198,    2, 0x0a /* Public */,
      34,    2,  203,    2, 0x0a /* Public */,
      36,    1,  208,    2, 0x0a /* Public */,
      37,    1,  211,    2, 0x0a /* Public */,
      38,    3,  214,    2, 0x08 /* Private */,
      41,    2,  221,    2, 0x08 /* Private */,
      42,    2,  226,    2, 0x08 /* Private */,
      43,    2,  231,    2, 0x08 /* Private */,
      44,    2,  236,    2, 0x08 /* Private */,
      45,    2,  241,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 9,    3,   10,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,    3,   13,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,    3,   16,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 18,    3,   19,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 21,    3,   19,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 23,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 26,    3,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::UShort,    3,   28,
    QMetaType::Void, QMetaType::Int, QMetaType::UShort,    3,   28,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 31,    3,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 33,    3,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 35,    3,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,   39,    3,   40,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,   40,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,   40,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,   40,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,   40,

       0        // eod
};

void BfcpClientProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpClientProxy *_t = static_cast<BfcpClientProxy *>(_o);
        switch (_id) {
        case 0: _t->NotifyClientControlSuccessedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bfcpc::ClientArg(*)>(_a[2]))); break;
        case 1: _t->NotifyClientAlreadyExistSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bfcpc::ClientArg(*)>(_a[2]))); break;
        case 2: _t->NotifyClientNotExistSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->NotifyStateChangeSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bfcpc::ClientState(*)>(_a[2]))); break;
        case 4: _t->NotifyErrorReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::ResponseError(*)>(_a[2]))); break;
        case 5: _t->NotifyFloorRequestInfoSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::FloorRequestInfo(*)>(_a[2]))); break;
        case 6: _t->NotifyUserStatusSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::UserStatus(*)>(_a[2]))); break;
        case 7: _t->NotifyFloorStatusSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::FloorStatus(*)>(_a[2]))); break;
        case 8: _t->CreateClient((*reinterpret_cast< const bfcpc::ClientArg(*)>(_a[1]))); break;
        case 9: _t->DestroyClient((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->SendFloorRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::FloorRequestArg(*)>(_a[2]))); break;
        case 11: _t->SendFloorRelease((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 12: _t->SendFloorRequestQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 13: _t->SendUserQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::UserQueryArg(*)>(_a[2]))); break;
        case 14: _t->SendFloorQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::FloorIDList(*)>(_a[2]))); break;
        case 15: _t->SendChairAction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const bfcpc::ChairActionArg(*)>(_a[2]))); break;
        case 16: _t->SendHello((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->SendGoodbye((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->HandleClientControlResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 19: _t->HandleStateChangedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->HandleErrorReceivedResultSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 21: _t->HandleFloorRequestInfoSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 22: _t->HandleUserStatusReceivedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 23: _t->HandleFloorStatusReceivedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpClientProxy::*_t)(int , bfcpc::ClientArg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyClientControlSuccessedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , bfcpc::ClientArg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyClientAlreadyExistSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyClientNotExistSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , bfcpc::ClientState );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyStateChangeSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , const bfcpc::ResponseError & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyErrorReceivedSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , const bfcpc::FloorRequestInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyFloorRequestInfoSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , const bfcpc::UserStatus & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyUserStatusSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (BfcpClientProxy::*_t)(int , const bfcpc::FloorStatus & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientProxy::NotifyFloorStatusSignal)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject BfcpClientProxy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BfcpClientProxy.data,
      qt_meta_data_BfcpClientProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpClientProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpClientProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpClientProxy.stringdata))
        return static_cast<void*>(const_cast< BfcpClientProxy*>(this));
    return QObject::qt_metacast(_clname);
}

int BfcpClientProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void BfcpClientProxy::NotifyClientControlSuccessedSignal(int _t1, bfcpc::ClientArg & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpClientProxy::NotifyClientAlreadyExistSignal(int _t1, bfcpc::ClientArg & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpClientProxy::NotifyClientNotExistSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpClientProxy::NotifyStateChangeSignal(int _t1, bfcpc::ClientState _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpClientProxy::NotifyErrorReceivedSignal(int _t1, const bfcpc::ResponseError & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpClientProxy::NotifyFloorRequestInfoSignal(int _t1, const bfcpc::FloorRequestInfo & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BfcpClientProxy::NotifyUserStatusSignal(int _t1, const bfcpc::UserStatus & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void BfcpClientProxy::NotifyFloorStatusSignal(int _t1, const bfcpc::FloorStatus & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
