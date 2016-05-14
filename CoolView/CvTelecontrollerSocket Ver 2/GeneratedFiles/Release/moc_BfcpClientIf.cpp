/****************************************************************************
** Meta object code from reading C++ file 'BfcpClientIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpClient/client/BfcpClientIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BfcpClientIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpClientIf_t {
    QByteArrayData data[29];
    char stringdata[448];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpClientIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpClientIf_t qt_meta_stringdata_BfcpClientIf = {
    {
QT_MOC_LITERAL(0, 0, 12), // "BfcpClientIf"
QT_MOC_LITERAL(1, 13, 19), // "ClientControlSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "control_error"
QT_MOC_LITERAL(4, 48, 9), // "client_id"
QT_MOC_LITERAL(5, 58, 10), // "client_arg"
QT_MOC_LITERAL(6, 69, 19), // "ErrorReceivedSignal"
QT_MOC_LITERAL(7, 89, 4), // "data"
QT_MOC_LITERAL(8, 94, 30), // "FloorRequestInfoReceivedSignal"
QT_MOC_LITERAL(9, 125, 25), // "FloorStatusReceivedSignal"
QT_MOC_LITERAL(10, 151, 18), // "StateChangedSignal"
QT_MOC_LITERAL(11, 170, 5), // "state"
QT_MOC_LITERAL(12, 176, 24), // "UserStatusReceivedSignal"
QT_MOC_LITERAL(13, 201, 12), // "CreateClient"
QT_MOC_LITERAL(14, 214, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(15, 234, 13), // "DestroyClient"
QT_MOC_LITERAL(16, 248, 15), // "SendChairAction"
QT_MOC_LITERAL(17, 264, 16), // "chair_action_arg"
QT_MOC_LITERAL(18, 281, 14), // "SendFloorQuery"
QT_MOC_LITERAL(19, 296, 9), // "floor_ids"
QT_MOC_LITERAL(20, 306, 16), // "SendFloorRelease"
QT_MOC_LITERAL(21, 323, 16), // "floor_request_id"
QT_MOC_LITERAL(22, 340, 16), // "SendFloorRequest"
QT_MOC_LITERAL(23, 357, 17), // "floor_request_arg"
QT_MOC_LITERAL(24, 375, 21), // "SendFloorRequestQuery"
QT_MOC_LITERAL(25, 397, 11), // "SendGoodBye"
QT_MOC_LITERAL(26, 409, 9), // "SendHello"
QT_MOC_LITERAL(27, 419, 13), // "SendUserQuery"
QT_MOC_LITERAL(28, 433, 14) // "user_query_arg"

    },
    "BfcpClientIf\0ClientControlSignal\0\0"
    "control_error\0client_id\0client_arg\0"
    "ErrorReceivedSignal\0data\0"
    "FloorRequestInfoReceivedSignal\0"
    "FloorStatusReceivedSignal\0StateChangedSignal\0"
    "state\0UserStatusReceivedSignal\0"
    "CreateClient\0QDBusPendingReply<>\0"
    "DestroyClient\0SendChairAction\0"
    "chair_action_arg\0SendFloorQuery\0"
    "floor_ids\0SendFloorRelease\0floor_request_id\0"
    "SendFloorRequest\0floor_request_arg\0"
    "SendFloorRequestQuery\0SendGoodBye\0"
    "SendHello\0SendUserQuery\0user_query_arg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpClientIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   94,    2, 0x06 /* Public */,
       6,    2,  101,    2, 0x06 /* Public */,
       8,    2,  106,    2, 0x06 /* Public */,
       9,    2,  111,    2, 0x06 /* Public */,
      10,    2,  116,    2, 0x06 /* Public */,
      12,    2,  121,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,  126,    2, 0x0a /* Public */,
      15,    1,  129,    2, 0x0a /* Public */,
      16,    2,  132,    2, 0x0a /* Public */,
      18,    2,  137,    2, 0x0a /* Public */,
      20,    2,  142,    2, 0x0a /* Public */,
      22,    2,  147,    2, 0x0a /* Public */,
      24,    2,  152,    2, 0x0a /* Public */,
      25,    1,  157,    2, 0x0a /* Public */,
      26,    1,  160,    2, 0x0a /* Public */,
      27,    2,  163,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    4,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    4,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    4,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    4,    7,

 // slots: parameters
    0x80000000 | 14, QMetaType::QByteArray,    5,
    0x80000000 | 14, QMetaType::Int,    4,
    0x80000000 | 14, QMetaType::Int, QMetaType::QByteArray,    4,   17,
    0x80000000 | 14, QMetaType::Int, QMetaType::QByteArray,    4,   19,
    0x80000000 | 14, QMetaType::Int, QMetaType::UShort,    4,   21,
    0x80000000 | 14, QMetaType::Int, QMetaType::QByteArray,    4,   23,
    0x80000000 | 14, QMetaType::Int, QMetaType::UShort,    4,   21,
    0x80000000 | 14, QMetaType::Int,    4,
    0x80000000 | 14, QMetaType::Int,    4,
    0x80000000 | 14, QMetaType::Int, QMetaType::QByteArray,    4,   28,

       0        // eod
};

void BfcpClientIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpClientIf *_t = static_cast<BfcpClientIf *>(_o);
        switch (_id) {
        case 0: _t->ClientControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 1: _t->ErrorReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->FloorRequestInfoReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->FloorStatusReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 4: _t->StateChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->UserStatusReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 6: { QDBusPendingReply<> _r = _t->CreateClient((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->DestroyClient((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->SendChairAction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->SendFloorQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 10: { QDBusPendingReply<> _r = _t->SendFloorRelease((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 11: { QDBusPendingReply<> _r = _t->SendFloorRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 12: { QDBusPendingReply<> _r = _t->SendFloorRequestQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 13: { QDBusPendingReply<> _r = _t->SendGoodBye((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 14: { QDBusPendingReply<> _r = _t->SendHello((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 15: { QDBusPendingReply<> _r = _t->SendUserQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpClientIf::*_t)(int , int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::ClientControlSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpClientIf::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::ErrorReceivedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpClientIf::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::FloorRequestInfoReceivedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpClientIf::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::FloorStatusReceivedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpClientIf::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::StateChangedSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpClientIf::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientIf::UserStatusReceivedSignal)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject BfcpClientIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_BfcpClientIf.data,
      qt_meta_data_BfcpClientIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpClientIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpClientIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpClientIf.stringdata))
        return static_cast<void*>(const_cast< BfcpClientIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int BfcpClientIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void BfcpClientIf::ClientControlSignal(int _t1, int _t2, const QByteArray & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpClientIf::ErrorReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpClientIf::FloorRequestInfoReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpClientIf::FloorStatusReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpClientIf::StateChangedSignal(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpClientIf::UserStatusReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
