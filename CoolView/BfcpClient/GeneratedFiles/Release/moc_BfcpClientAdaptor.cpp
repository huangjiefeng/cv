/****************************************************************************
** Meta object code from reading C++ file 'BfcpClientAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpClient/service/BfcpClientAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BfcpClientAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpClientAdaptor_t {
    QByteArrayData data[32];
    char stringdata[3281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpClientAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpClientAdaptor_t qt_meta_stringdata_BfcpClientAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 17), // "BfcpClientAdaptor"
QT_MOC_LITERAL(1, 18, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 34, 31), // "com.dcampus.coolview.BfcpClient"
QT_MOC_LITERAL(3, 66, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 86, 2779), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 2595, 19), // "ClientControlSignal"
QT_MOC_LITERAL(6, 2615, 0), // ""
QT_MOC_LITERAL(7, 2616, 13), // "control_error"
QT_MOC_LITERAL(8, 2630, 9), // "client_id"
QT_MOC_LITERAL(9, 2640, 10), // "client_arg"
QT_MOC_LITERAL(10, 2651, 19), // "ErrorReceivedSignal"
QT_MOC_LITERAL(11, 2671, 4), // "data"
QT_MOC_LITERAL(12, 2676, 30), // "FloorRequestInfoReceivedSignal"
QT_MOC_LITERAL(13, 2707, 25), // "FloorStatusReceivedSignal"
QT_MOC_LITERAL(14, 2733, 18), // "StateChangedSignal"
QT_MOC_LITERAL(15, 2752, 5), // "state"
QT_MOC_LITERAL(16, 2758, 24), // "UserStatusReceivedSignal"
QT_MOC_LITERAL(17, 2783, 12), // "CreateClient"
QT_MOC_LITERAL(18, 2796, 13), // "DestroyClient"
QT_MOC_LITERAL(19, 2810, 15), // "SendChairAction"
QT_MOC_LITERAL(20, 2826, 16), // "chair_action_arg"
QT_MOC_LITERAL(21, 2843, 14), // "SendFloorQuery"
QT_MOC_LITERAL(22, 2858, 9), // "floor_ids"
QT_MOC_LITERAL(23, 2868, 16), // "SendFloorRelease"
QT_MOC_LITERAL(24, 2885, 16), // "floor_request_id"
QT_MOC_LITERAL(25, 2902, 16), // "SendFloorRequest"
QT_MOC_LITERAL(26, 2919, 17), // "floor_request_arg"
QT_MOC_LITERAL(27, 2937, 21), // "SendFloorRequestQuery"
QT_MOC_LITERAL(28, 2959, 11), // "SendGoodBye"
QT_MOC_LITERAL(29, 2971, 9), // "SendHello"
QT_MOC_LITERAL(30, 2981, 13), // "SendUserQuery"
QT_MOC_LITERAL(31, 2995, 14) // "user_query_arg"

    },
    "BfcpClientAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.BfcpClient\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.BfcpClient\">\n    <signal na"
    "me=\"ClientControlSignal\">\n      <arg direction=\"out\" type=\"i\" n"
    "ame=\"control_error\"/>\n      <arg direction=\"out\" type=\"i\" name="
    "\"client_id\"/>\n      <arg direction=\"out\" type=\"ay\" name=\"clien"
    "t_arg\"/>\n    </signal>\n    <signal name=\"StateChangedSignal\">\n  "
    "    <arg direction=\"out\" type=\"i\" name=\"client_id\"/>\n      <arg"
    " direction=\"out\" type=\"i\" name=\"state\"/>\n    </signal>\n    <si"
    "gnal name=\"FloorRequestInfoReceivedSignal\">\n      <arg direction=\""
    "out\" type=\"i\" name=\"client_id\"/>\n      <arg direction=\"out\" ty"
    "pe=\"ay\" name=\"data\"/>\n    </signal>\n    <signal name=\"UserStatu"
    "sReceivedSignal\">\n      <arg direction=\"out\" type=\"i\" name=\"cli"
    "ent_id\"/>\n      <arg direction=\"out\" type=\"ay\" name=\"data\"/>\n"
    "    </signal>\n    <signal name=\"FloorStatusReceivedSignal\">\n      "
    "<arg direction=\"out\" type=\"i\" name=\"client_id\"/>\n      <arg dir"
    "ection=\"out\" type=\"ay\" name=\"data\"/>\n    </signal>\n    <signal"
    " name=\"ErrorReceivedSignal\">\n      <arg direction=\"out\" type=\"i\""
    " name=\"client_id\"/>\n      <arg direction=\"out\" type=\"ay\" name=\""
    "data\"/>\n    </signal>\n    <method name=\"CreateClient\">\n      <ar"
    "g direction=\"in\" type=\"ay\" name=\"client_arg\"/>\n    </method>\n "
    "   <method name=\"DestroyClient\">\n      <arg direction=\"in\" type=\""
    "i\" name=\"client_id\"/>\n    </method>\n    <method name=\"SendFloorR"
    "equest\">\n      <arg direction=\"in\" type=\"i\" name=\"client_id\"/>"
    "\n      <arg direction=\"in\" type=\"ay\" name=\"floor_request_arg\"/>"
    "\n    </method>\n    <method name=\"SendFloorRelease\">\n      <arg di"
    "rection=\"in\" type=\"i\" name=\"client_id\"/>\n      <arg direction=\""
    "in\" type=\"q\" name=\"floor_request_id\"/>\n    </method>\n    <metho"
    "d name=\"SendFloorRequestQuery\">\n      <arg direction=\"in\" type=\""
    "i\" name=\"client_id\"/>\n      <arg direction=\"in\" type=\"q\" name="
    "\"floor_request_id\"/>\n    </method>\n    <method name=\"SendUserQuer"
    "y\">\n      <arg direction=\"in\" type=\"i\" name=\"client_id\"/>\n   "
    "   <arg direction=\"in\" type=\"ay\" name=\"user_query_arg\"/>\n    </"
    "method>\n    <method name=\"SendFloorQuery\">\n      <arg direction=\""
    "in\" type=\"i\" name=\"client_id\"/>\n      <arg direction=\"in\" type"
    "=\"ay\" name=\"floor_ids\"/>\n    </method>\n    <method name=\"SendCh"
    "airAction\">\n      <arg direction=\"in\" type=\"i\" name=\"client_id\""
    "/>\n      <arg direction=\"in\" type=\"ay\" name=\"chair_action_arg\"/"
    ">\n    </method>\n    <method name=\"SendHello\">\n      <arg directio"
    "n=\"in\" type=\"i\" name=\"client_id\"/>\n    </method>\n    <method n"
    "ame=\"SendGoodBye\">\n      <arg direction=\"in\" type=\"i\" name=\"cl"
    "ient_id\"/>\n    </method>\n  </interface>\n\0"
    "ClientControlSignal\0\0control_error\0"
    "client_id\0client_arg\0ErrorReceivedSignal\0"
    "data\0FloorRequestInfoReceivedSignal\0"
    "FloorStatusReceivedSignal\0StateChangedSignal\0"
    "state\0UserStatusReceivedSignal\0"
    "CreateClient\0DestroyClient\0SendChairAction\0"
    "chair_action_arg\0SendFloorQuery\0"
    "floor_ids\0SendFloorRelease\0floor_request_id\0"
    "SendFloorRequest\0floor_request_arg\0"
    "SendFloorRequestQuery\0SendGoodBye\0"
    "SendHello\0SendUserQuery\0user_query_arg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpClientAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
      16,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    3,   98,    6, 0x06 /* Public */,
      10,    2,  105,    6, 0x06 /* Public */,
      12,    2,  110,    6, 0x06 /* Public */,
      13,    2,  115,    6, 0x06 /* Public */,
      14,    2,  120,    6, 0x06 /* Public */,
      16,    2,  125,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  130,    6, 0x0a /* Public */,
      18,    1,  133,    6, 0x0a /* Public */,
      19,    2,  136,    6, 0x0a /* Public */,
      21,    2,  141,    6, 0x0a /* Public */,
      23,    2,  146,    6, 0x0a /* Public */,
      25,    2,  151,    6, 0x0a /* Public */,
      27,    2,  156,    6, 0x0a /* Public */,
      28,    1,  161,    6, 0x0a /* Public */,
      29,    1,  164,    6, 0x0a /* Public */,
      30,    2,  167,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    7,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   20,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::UShort,    8,   24,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   26,
    QMetaType::Void, QMetaType::Int, QMetaType::UShort,    8,   24,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,   31,

       0        // eod
};

void BfcpClientAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpClientAdaptor *_t = static_cast<BfcpClientAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ClientControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 1: _t->ErrorReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->FloorRequestInfoReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->FloorStatusReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 4: _t->StateChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->UserStatusReceivedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 6: _t->CreateClient((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->DestroyClient((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->SendChairAction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 9: _t->SendFloorQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 10: _t->SendFloorRelease((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 11: _t->SendFloorRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 12: _t->SendFloorRequestQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 13: _t->SendGoodBye((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->SendHello((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->SendUserQuery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpClientAdaptor::*_t)(int , int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::ClientControlSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpClientAdaptor::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::ErrorReceivedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpClientAdaptor::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::FloorRequestInfoReceivedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpClientAdaptor::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::FloorStatusReceivedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpClientAdaptor::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::StateChangedSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpClientAdaptor::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpClientAdaptor::UserStatusReceivedSignal)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject BfcpClientAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_BfcpClientAdaptor.data,
      qt_meta_data_BfcpClientAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpClientAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpClientAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpClientAdaptor.stringdata))
        return static_cast<void*>(const_cast< BfcpClientAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int BfcpClientAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void BfcpClientAdaptor::ClientControlSignal(int _t1, int _t2, const QByteArray & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpClientAdaptor::ErrorReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpClientAdaptor::FloorRequestInfoReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpClientAdaptor::FloorStatusReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpClientAdaptor::StateChangedSignal(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpClientAdaptor::UserStatusReceivedSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
