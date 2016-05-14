/****************************************************************************
** Meta object code from reading C++ file 'ConferenceRoomIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/conferenceRoom/client/ConferenceRoomIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConferenceRoomIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConferenceRoomIf_t {
    QByteArrayData data[23];
    char stringdata[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceRoomIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceRoomIf_t qt_meta_stringdata_ConferenceRoomIf = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ConferenceRoomIf"
QT_MOC_LITERAL(1, 17, 14), // "AddMediaWindow"
QT_MOC_LITERAL(2, 32, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(3, 52, 0), // ""
QT_MOC_LITERAL(4, 53, 4), // "send"
QT_MOC_LITERAL(5, 58, 12), // "input_garray"
QT_MOC_LITERAL(6, 71, 11), // "screenIndex"
QT_MOC_LITERAL(7, 83, 4), // "seet"
QT_MOC_LITERAL(8, 88, 12), // "ChangeLayout"
QT_MOC_LITERAL(9, 101, 12), // "displayModel"
QT_MOC_LITERAL(10, 114, 16), // "CloseMediaWindow"
QT_MOC_LITERAL(11, 131, 7), // "user_id"
QT_MOC_LITERAL(12, 139, 11), // "CloseWindow"
QT_MOC_LITERAL(13, 151, 14), // "ExitConference"
QT_MOC_LITERAL(14, 166, 17), // "ModifyMediaWindow"
QT_MOC_LITERAL(15, 184, 10), // "actionIdex"
QT_MOC_LITERAL(16, 195, 9), // "arguments"
QT_MOC_LITERAL(17, 205, 19), // "OpenScreenShareFile"
QT_MOC_LITERAL(18, 225, 8), // "filePath"
QT_MOC_LITERAL(19, 234, 24), // "RecoveryRecvMediaProcess"
QT_MOC_LITERAL(20, 259, 11), // "ShowRtcpMsg"
QT_MOC_LITERAL(21, 271, 4), // "show"
QT_MOC_LITERAL(22, 276, 13) // "UpdateRtcpMsg"

    },
    "ConferenceRoomIf\0AddMediaWindow\0"
    "QDBusPendingReply<>\0\0send\0input_garray\0"
    "screenIndex\0seet\0ChangeLayout\0"
    "displayModel\0CloseMediaWindow\0user_id\0"
    "CloseWindow\0ExitConference\0ModifyMediaWindow\0"
    "actionIdex\0arguments\0OpenScreenShareFile\0"
    "filePath\0RecoveryRecvMediaProcess\0"
    "ShowRtcpMsg\0show\0UpdateRtcpMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceRoomIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   64,    3, 0x0a /* Public */,
       8,    2,   73,    3, 0x0a /* Public */,
      10,    1,   78,    3, 0x0a /* Public */,
      12,    0,   81,    3, 0x0a /* Public */,
      13,    0,   82,    3, 0x0a /* Public */,
      14,    3,   83,    3, 0x0a /* Public */,
      17,    2,   90,    3, 0x0a /* Public */,
      19,    3,   95,    3, 0x0a /* Public */,
      20,    2,  102,    3, 0x0a /* Public */,
      22,    1,  107,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, QMetaType::Bool, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int,    4,    5,    6,    7,
    0x80000000 | 2, QMetaType::Int, QMetaType::Int,    6,    9,
    0x80000000 | 2, QMetaType::QString,   11,
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 2, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   11,   15,   16,
    0x80000000 | 2, QMetaType::QString, QMetaType::Int,   18,    6,
    0x80000000 | 2, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int,    5,    6,    7,
    0x80000000 | 2, QMetaType::Int, QMetaType::Bool,    6,   21,
    0x80000000 | 2, QMetaType::QByteArray,    5,

       0        // eod
};

void ConferenceRoomIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConferenceRoomIf *_t = static_cast<ConferenceRoomIf *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<> _r = _t->AddMediaWindow((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 1: { QDBusPendingReply<> _r = _t->ChangeLayout((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<> _r = _t->CloseMediaWindow((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->CloseWindow();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->ExitConference();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->ModifyMediaWindow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->OpenScreenShareFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->RecoveryRecvMediaProcess((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->ShowRtcpMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->UpdateRtcpMsg((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject ConferenceRoomIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_ConferenceRoomIf.data,
      qt_meta_data_ConferenceRoomIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConferenceRoomIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceRoomIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceRoomIf.stringdata))
        return static_cast<void*>(const_cast< ConferenceRoomIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int ConferenceRoomIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
