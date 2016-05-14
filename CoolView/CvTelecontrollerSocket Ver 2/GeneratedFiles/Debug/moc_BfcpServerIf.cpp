/****************************************************************************
** Meta object code from reading C++ file 'BfcpServerIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpServer/client/BfcpServerIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BfcpServerIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpServerIf_t {
    QByteArrayData data[35];
    char stringdata[468];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpServerIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpServerIf_t qt_meta_stringdata_BfcpServerIf = {
    {
QT_MOC_LITERAL(0, 0, 12), // "BfcpServerIf"
QT_MOC_LITERAL(1, 13, 18), // "ChairControlSignal"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "control_error"
QT_MOC_LITERAL(4, 47, 13), // "conference_id"
QT_MOC_LITERAL(5, 61, 8), // "floor_id"
QT_MOC_LITERAL(6, 70, 7), // "user_id"
QT_MOC_LITERAL(7, 78, 23), // "ConferenceControlSignal"
QT_MOC_LITERAL(8, 102, 19), // "ConferenceIDsSignal"
QT_MOC_LITERAL(9, 122, 14), // "conference_ids"
QT_MOC_LITERAL(10, 137, 20), // "ConferenceInfoSignal"
QT_MOC_LITERAL(11, 158, 15), // "conference_info"
QT_MOC_LITERAL(12, 174, 18), // "FloorControlSignal"
QT_MOC_LITERAL(13, 193, 19), // "ServerControlSignal"
QT_MOC_LITERAL(14, 213, 17), // "UserControlSignal"
QT_MOC_LITERAL(15, 231, 13), // "AddConference"
QT_MOC_LITERAL(16, 245, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(17, 265, 14), // "conference_arg"
QT_MOC_LITERAL(18, 280, 8), // "AddFloor"
QT_MOC_LITERAL(19, 289, 9), // "floor_arg"
QT_MOC_LITERAL(20, 299, 7), // "AddUser"
QT_MOC_LITERAL(21, 307, 8), // "user_arg"
QT_MOC_LITERAL(22, 316, 16), // "GetConferenceIDs"
QT_MOC_LITERAL(23, 333, 17), // "GetConferenceInfo"
QT_MOC_LITERAL(24, 351, 16), // "ModifyConference"
QT_MOC_LITERAL(25, 368, 11), // "ModifyFloor"
QT_MOC_LITERAL(26, 380, 4), // "Quit"
QT_MOC_LITERAL(27, 385, 11), // "RemoveChair"
QT_MOC_LITERAL(28, 397, 16), // "RemoveConference"
QT_MOC_LITERAL(29, 414, 11), // "RemoveFloor"
QT_MOC_LITERAL(30, 426, 10), // "RemoveUser"
QT_MOC_LITERAL(31, 437, 8), // "SetChair"
QT_MOC_LITERAL(32, 446, 5), // "Start"
QT_MOC_LITERAL(33, 452, 10), // "server_arg"
QT_MOC_LITERAL(34, 463, 4) // "Stop"

    },
    "BfcpServerIf\0ChairControlSignal\0\0"
    "control_error\0conference_id\0floor_id\0"
    "user_id\0ConferenceControlSignal\0"
    "ConferenceIDsSignal\0conference_ids\0"
    "ConferenceInfoSignal\0conference_info\0"
    "FloorControlSignal\0ServerControlSignal\0"
    "UserControlSignal\0AddConference\0"
    "QDBusPendingReply<>\0conference_arg\0"
    "AddFloor\0floor_arg\0AddUser\0user_arg\0"
    "GetConferenceIDs\0GetConferenceInfo\0"
    "ModifyConference\0ModifyFloor\0Quit\0"
    "RemoveChair\0RemoveConference\0RemoveFloor\0"
    "RemoveUser\0SetChair\0Start\0server_arg\0"
    "Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpServerIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  124,    2, 0x06 /* Public */,
       7,    2,  133,    2, 0x06 /* Public */,
       8,    2,  138,    2, 0x06 /* Public */,
      10,    3,  143,    2, 0x06 /* Public */,
      12,    3,  150,    2, 0x06 /* Public */,
      13,    1,  157,    2, 0x06 /* Public */,
      14,    3,  160,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    2,  167,    2, 0x0a /* Public */,
      18,    3,  172,    2, 0x0a /* Public */,
      20,    3,  179,    2, 0x0a /* Public */,
      22,    0,  186,    2, 0x0a /* Public */,
      23,    1,  187,    2, 0x0a /* Public */,
      24,    2,  190,    2, 0x0a /* Public */,
      25,    3,  195,    2, 0x0a /* Public */,
      26,    0,  202,    2, 0x0a /* Public */,
      27,    2,  203,    2, 0x0a /* Public */,
      28,    1,  208,    2, 0x0a /* Public */,
      29,    2,  211,    2, 0x0a /* Public */,
      30,    2,  216,    2, 0x0a /* Public */,
      31,    3,  221,    2, 0x0a /* Public */,
      32,    1,  228,    2, 0x0a /* Public */,
      34,    0,  231,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort, QMetaType::UShort,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    3,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::QString,    3,    4,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,    3,    4,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,    3,    4,    6,

 // slots: parameters
    0x80000000 | 16, QMetaType::UInt, QMetaType::QByteArray,    4,   17,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    4,    5,   19,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    4,    6,   21,
    0x80000000 | 16,
    0x80000000 | 16, QMetaType::UInt,    4,
    0x80000000 | 16, QMetaType::UInt, QMetaType::QByteArray,    4,   17,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    4,    5,   19,
    0x80000000 | 16,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort,    4,    5,
    0x80000000 | 16, QMetaType::UInt,    4,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort,    4,    5,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort,    4,    6,
    0x80000000 | 16, QMetaType::UInt, QMetaType::UShort, QMetaType::UShort,    4,    5,    6,
    0x80000000 | 16, QMetaType::QByteArray,   33,
    0x80000000 | 16,

       0        // eod
};

void BfcpServerIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpServerIf *_t = static_cast<BfcpServerIf *>(_o);
        switch (_id) {
        case 0: _t->ChairControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 1: _t->ConferenceControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 2: _t->ConferenceIDsSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->ConferenceInfoSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->FloorControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 5: _t->ServerControlSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->UserControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 7: { QDBusPendingReply<> _r = _t->AddConference((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->AddFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->AddUser((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 10: { QDBusPendingReply<> _r = _t->GetConferenceIDs();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 11: { QDBusPendingReply<> _r = _t->GetConferenceInfo((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 12: { QDBusPendingReply<> _r = _t->ModifyConference((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 13: { QDBusPendingReply<> _r = _t->ModifyFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 14: { QDBusPendingReply<> _r = _t->Quit();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 15: { QDBusPendingReply<> _r = _t->RemoveChair((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 16: { QDBusPendingReply<> _r = _t->RemoveConference((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 17: { QDBusPendingReply<> _r = _t->RemoveFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 18: { QDBusPendingReply<> _r = _t->RemoveUser((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 19: { QDBusPendingReply<> _r = _t->SetChair((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 20: { QDBusPendingReply<> _r = _t->Start((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 21: { QDBusPendingReply<> _r = _t->Stop();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpServerIf::*_t)(int , uint , ushort , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::ChairControlSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int , uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::ConferenceControlSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::ConferenceIDsSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int , uint , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::ConferenceInfoSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int , uint , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::FloorControlSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::ServerControlSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (BfcpServerIf::*_t)(int , uint , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerIf::UserControlSignal)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject BfcpServerIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_BfcpServerIf.data,
      qt_meta_data_BfcpServerIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpServerIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpServerIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpServerIf.stringdata))
        return static_cast<void*>(const_cast< BfcpServerIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int BfcpServerIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void BfcpServerIf::ChairControlSignal(int _t1, uint _t2, ushort _t3, ushort _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpServerIf::ConferenceControlSignal(int _t1, uint _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpServerIf::ConferenceIDsSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpServerIf::ConferenceInfoSignal(int _t1, uint _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpServerIf::FloorControlSignal(int _t1, uint _t2, ushort _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpServerIf::ServerControlSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BfcpServerIf::UserControlSignal(int _t1, uint _t2, ushort _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
