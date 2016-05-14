/****************************************************************************
** Meta object code from reading C++ file 'BfcpServerAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/BfcpServer/service/BfcpServerAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BfcpServerAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BfcpServerAdaptor_t {
    QByteArrayData data[38];
    char stringdata[4445];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BfcpServerAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BfcpServerAdaptor_t qt_meta_stringdata_BfcpServerAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 17), // "BfcpServerAdaptor"
QT_MOC_LITERAL(1, 18, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 34, 31), // "com.dcampus.coolview.BfcpServer"
QT_MOC_LITERAL(3, 66, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 86, 3923), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 3620, 18), // "ChairControlSignal"
QT_MOC_LITERAL(6, 3639, 0), // ""
QT_MOC_LITERAL(7, 3640, 13), // "control_error"
QT_MOC_LITERAL(8, 3654, 13), // "conference_id"
QT_MOC_LITERAL(9, 3668, 8), // "floor_id"
QT_MOC_LITERAL(10, 3677, 7), // "user_id"
QT_MOC_LITERAL(11, 3685, 23), // "ConferenceControlSignal"
QT_MOC_LITERAL(12, 3709, 19), // "ConferenceIDsSignal"
QT_MOC_LITERAL(13, 3729, 14), // "conference_ids"
QT_MOC_LITERAL(14, 3744, 20), // "ConferenceInfoSignal"
QT_MOC_LITERAL(15, 3765, 15), // "conference_info"
QT_MOC_LITERAL(16, 3781, 18), // "FloorControlSignal"
QT_MOC_LITERAL(17, 3800, 19), // "ServerControlSignal"
QT_MOC_LITERAL(18, 3820, 17), // "UserControlSignal"
QT_MOC_LITERAL(19, 3838, 13), // "AddConference"
QT_MOC_LITERAL(20, 3852, 14), // "conference_arg"
QT_MOC_LITERAL(21, 3867, 8), // "AddFloor"
QT_MOC_LITERAL(22, 3876, 9), // "floor_arg"
QT_MOC_LITERAL(23, 3886, 7), // "AddUser"
QT_MOC_LITERAL(24, 3894, 8), // "user_arg"
QT_MOC_LITERAL(25, 3903, 16), // "GetConferenceIDs"
QT_MOC_LITERAL(26, 3920, 17), // "GetConferenceInfo"
QT_MOC_LITERAL(27, 3938, 16), // "ModifyConference"
QT_MOC_LITERAL(28, 3955, 11), // "ModifyFloor"
QT_MOC_LITERAL(29, 3967, 4), // "Quit"
QT_MOC_LITERAL(30, 3972, 11), // "RemoveChair"
QT_MOC_LITERAL(31, 3984, 16), // "RemoveConference"
QT_MOC_LITERAL(32, 4001, 11), // "RemoveFloor"
QT_MOC_LITERAL(33, 4013, 10), // "RemoveUser"
QT_MOC_LITERAL(34, 4024, 8), // "SetChair"
QT_MOC_LITERAL(35, 4033, 5), // "Start"
QT_MOC_LITERAL(36, 4039, 10), // "server_arg"
QT_MOC_LITERAL(37, 4050, 4) // "Stop"

    },
    "BfcpServerAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.BfcpServer\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.BfcpServer\">\n    <signal na"
    "me=\"ServerControlSignal\">\n      <arg direction=\"out\" type=\"i\" n"
    "ame=\"control_error\"/>\n    </signal>\n    <signal name=\"ConferenceC"
    "ontrolSignal\">\n      <arg direction=\"out\" type=\"i\" name=\"contro"
    "l_error\"/>\n      <arg direction=\"out\" type=\"u\" name=\"conference"
    "_id\"/>\n    </signal>\n    <signal name=\"FloorControlSignal\">\n    "
    "  <arg direction=\"out\" type=\"i\" name=\"control_error\"/>\n      <a"
    "rg direction=\"out\" type=\"u\" name=\"conference_id\"/>\n      <arg d"
    "irection=\"out\" type=\"q\" name=\"floor_id\"/>\n    </signal>\n    <s"
    "ignal name=\"UserControlSignal\">\n      <arg direction=\"out\" type=\""
    "i\" name=\"control_error\"/>\n      <arg direction=\"out\" type=\"u\" "
    "name=\"conference_id\"/>\n      <arg direction=\"out\" type=\"q\" name"
    "=\"user_id\"/>\n    </signal>\n    <signal name=\"ChairControlSignal\""
    ">\n      <arg direction=\"out\" type=\"i\" name=\"control_error\"/>\n "
    "     <arg direction=\"out\" type=\"u\" name=\"conference_id\"/>\n     "
    " <arg direction=\"out\" type=\"q\" name=\"floor_id\"/>\n      <arg dir"
    "ection=\"out\" type=\"q\" name=\"user_id\"/>\n    </signal>\n    <sign"
    "al name=\"ConferenceIDsSignal\">\n      <arg direction=\"out\" type=\""
    "i\" name=\"control_error\"/>\n      <arg direction=\"out\" type=\"ay\""
    " name=\"conference_ids\"/>\n    </signal>\n    <signal name=\"Conferen"
    "ceInfoSignal\">\n      <arg direction=\"out\" type=\"i\" name=\"contro"
    "l_error\"/>\n      <arg direction=\"out\" type=\"u\" name=\"conference"
    "_id\"/>\n      <arg direction=\"out\" type=\"s\" name=\"conference_inf"
    "o\"/>\n    </signal>\n    <method name=\"Start\">\n      <arg directio"
    "n=\"in\" type=\"ay\" name=\"server_arg\"/>\n    </method>\n    <method"
    " name=\"Stop\"/>\n    <method name=\"Quit\"/>\n    <method name=\"AddC"
    "onference\">\n      <arg direction=\"in\" type=\"u\" name=\"conference"
    "_id\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"conference_arg"
    "\"/>\n    </method>\n    <method name=\"RemoveConference\">\n      <ar"
    "g direction=\"in\" type=\"u\" name=\"conference_id\"/>\n    </method>\n"
    "    <method name=\"ModifyConference\">\n      <arg direction=\"in\" ty"
    "pe=\"u\" name=\"conference_id\"/>\n      <arg direction=\"in\" type=\""
    "ay\" name=\"conference_arg\"/>\n    </method>\n    <method name=\"AddF"
    "loor\">\n      <arg direction=\"in\" type=\"u\" name=\"conference_id\""
    "/>\n      <arg direction=\"in\" type=\"q\" name=\"floor_id\"/>\n      "
    "<arg direction=\"in\" type=\"ay\" name=\"floor_arg\"/>\n    </method>\n"
    "    <method name=\"RemoveFloor\">\n      <arg direction=\"in\" type=\""
    "u\" name=\"conference_id\"/>\n      <arg direction=\"in\" type=\"q\" n"
    "ame=\"floor_id\"/>\n    </method>\n    <method name=\"ModifyFloor\">\n"
    "      <arg direction=\"in\" type=\"u\" name=\"conference_id\"/>\n     "
    " <arg direction=\"in\" type=\"q\" name=\"floor_id\"/>\n      <arg dire"
    "ction=\"in\" type=\"ay\" name=\"floor_arg\"/>\n    </method>\n    <met"
    "hod name=\"AddUser\">\n      <arg direction=\"in\" type=\"u\" name=\"c"
    "onference_id\"/>\n      <arg direction=\"in\" type=\"q\" name=\"user_i"
    "d\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"user_arg\"/>\n  "
    "  </method>\n    <method name=\"RemoveUser\">\n      <arg direction=\""
    "in\" type=\"u\" name=\"conference_id\"/>\n      <arg direction=\"in\" "
    "type=\"q\" name=\"user_id\"/>\n    </method>\n    <method name=\"SetCh"
    "air\">\n      <arg direction=\"in\" type=\"u\" name=\"conference_id\"/"
    ">\n      <arg direction=\"in\" type=\"q\" name=\"floor_id\"/>\n      <"
    "arg direction=\"in\" type=\"q\" name=\"user_id\"/>\n    </method>\n   "
    " <method name=\"RemoveChair\">\n      <arg direction=\"in\" type=\"u\""
    " name=\"conference_id\"/>\n      <arg direction=\"in\" type=\"q\" name"
    "=\"floor_id\"/>\n    </method>\n    <method name=\"GetConferenceIDs\"/"
    ">\n    <method name=\"GetConferenceInfo\">\n      <arg direction=\"in\""
    " type=\"u\" name=\"conference_id\"/>\n    </method>\n  </interface>\n\0"
    "ChairControlSignal\0\0control_error\0"
    "conference_id\0floor_id\0user_id\0"
    "ConferenceControlSignal\0ConferenceIDsSignal\0"
    "conference_ids\0ConferenceInfoSignal\0"
    "conference_info\0FloorControlSignal\0"
    "ServerControlSignal\0UserControlSignal\0"
    "AddConference\0conference_arg\0AddFloor\0"
    "floor_arg\0AddUser\0user_arg\0GetConferenceIDs\0"
    "GetConferenceInfo\0ModifyConference\0"
    "ModifyFloor\0Quit\0RemoveChair\0"
    "RemoveConference\0RemoveFloor\0RemoveUser\0"
    "SetChair\0Start\0server_arg\0Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BfcpServerAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
      22,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    4,  128,    6, 0x06 /* Public */,
      11,    2,  137,    6, 0x06 /* Public */,
      12,    2,  142,    6, 0x06 /* Public */,
      14,    3,  147,    6, 0x06 /* Public */,
      16,    3,  154,    6, 0x06 /* Public */,
      17,    1,  161,    6, 0x06 /* Public */,
      18,    3,  164,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    2,  171,    6, 0x0a /* Public */,
      21,    3,  176,    6, 0x0a /* Public */,
      23,    3,  183,    6, 0x0a /* Public */,
      25,    0,  190,    6, 0x0a /* Public */,
      26,    1,  191,    6, 0x0a /* Public */,
      27,    2,  194,    6, 0x0a /* Public */,
      28,    3,  199,    6, 0x0a /* Public */,
      29,    0,  206,    6, 0x0a /* Public */,
      30,    2,  207,    6, 0x0a /* Public */,
      31,    1,  212,    6, 0x0a /* Public */,
      32,    2,  215,    6, 0x0a /* Public */,
      33,    2,  220,    6, 0x0a /* Public */,
      34,    3,  225,    6, 0x0a /* Public */,
      35,    1,  232,    6, 0x0a /* Public */,
      37,    0,  235,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort, QMetaType::UShort,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    7,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::QString,    7,    8,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,    7,    8,    9,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::UShort,    7,    8,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::QByteArray,    8,   20,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    8,    9,   22,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    8,   10,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    8,
    QMetaType::Void, QMetaType::UInt, QMetaType::QByteArray,    8,   20,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort, QMetaType::QByteArray,    8,    9,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,    8,    9,
    QMetaType::Void, QMetaType::UInt,    8,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,    8,    9,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort,    8,   10,
    QMetaType::Void, QMetaType::UInt, QMetaType::UShort, QMetaType::UShort,    8,    9,   10,
    QMetaType::Void, QMetaType::QByteArray,   36,
    QMetaType::Void,

       0        // eod
};

void BfcpServerAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BfcpServerAdaptor *_t = static_cast<BfcpServerAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ChairControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 1: _t->ConferenceControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 2: _t->ConferenceIDsSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->ConferenceInfoSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->FloorControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 5: _t->ServerControlSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->UserControlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 7: _t->AddConference((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 8: _t->AddFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 9: _t->AddUser((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 10: _t->GetConferenceIDs(); break;
        case 11: _t->GetConferenceInfo((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 12: _t->ModifyConference((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 13: _t->ModifyFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 14: _t->Quit(); break;
        case 15: _t->RemoveChair((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 16: _t->RemoveConference((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 17: _t->RemoveFloor((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 18: _t->RemoveUser((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2]))); break;
        case 19: _t->SetChair((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 20: _t->Start((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 21: _t->Stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BfcpServerAdaptor::*_t)(int , uint , ushort , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::ChairControlSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int , uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::ConferenceControlSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::ConferenceIDsSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int , uint , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::ConferenceInfoSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int , uint , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::FloorControlSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::ServerControlSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (BfcpServerAdaptor::*_t)(int , uint , ushort );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BfcpServerAdaptor::UserControlSignal)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject BfcpServerAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_BfcpServerAdaptor.data,
      qt_meta_data_BfcpServerAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BfcpServerAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BfcpServerAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BfcpServerAdaptor.stringdata))
        return static_cast<void*>(const_cast< BfcpServerAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int BfcpServerAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void BfcpServerAdaptor::ChairControlSignal(int _t1, uint _t2, ushort _t3, ushort _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BfcpServerAdaptor::ConferenceControlSignal(int _t1, uint _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BfcpServerAdaptor::ConferenceIDsSignal(int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BfcpServerAdaptor::ConferenceInfoSignal(int _t1, uint _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BfcpServerAdaptor::FloorControlSignal(int _t1, uint _t2, ushort _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BfcpServerAdaptor::ServerControlSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BfcpServerAdaptor::UserControlSignal(int _t1, uint _t2, ushort _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
