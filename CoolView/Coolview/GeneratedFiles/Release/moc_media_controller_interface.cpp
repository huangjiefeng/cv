/****************************************************************************
** Meta object code from reading C++ file 'media_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../media_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'media_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IMediaController_t {
    QByteArrayData data[30];
    char stringdata[613];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IMediaController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IMediaController_t qt_meta_stringdata_IMediaController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "IMediaController"
QT_MOC_LITERAL(1, 17, 39), // "HandleSendRTPStatToQoSServerR..."
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 11), // "RtpStatItem"
QT_MOC_LITERAL(4, 70, 8), // "rtp_stat"
QT_MOC_LITERAL(5, 79, 36), // "HandleSendRecStatToServerRequ..."
QT_MOC_LITERAL(6, 116, 11), // "RecStatItem"
QT_MOC_LITERAL(7, 128, 7), // "recstat"
QT_MOC_LITERAL(8, 136, 35), // "HandleAddRecvStreamMediaReque..."
QT_MOC_LITERAL(9, 172, 11), // "remote_vuri"
QT_MOC_LITERAL(10, 184, 12), // "screen_index"
QT_MOC_LITERAL(11, 197, 4), // "seat"
QT_MOC_LITERAL(12, 202, 15), // "use_small_video"
QT_MOC_LITERAL(13, 218, 38), // "HandleRemoveRecvStreamMediaRe..."
QT_MOC_LITERAL(14, 257, 25), // "HandleChangeVideoSeatSlot"
QT_MOC_LITERAL(15, 283, 10), // "remote_uri"
QT_MOC_LITERAL(16, 294, 38), // "HandleCreateSendStreamMediaRe..."
QT_MOC_LITERAL(17, 333, 33), // "HandleControlSendMediaRequest..."
QT_MOC_LITERAL(18, 367, 16), // "MediaControlType"
QT_MOC_LITERAL(19, 384, 4), // "type"
QT_MOC_LITERAL(20, 389, 6), // "enable"
QT_MOC_LITERAL(21, 396, 38), // "HandleCreateSendScreenMediaRe..."
QT_MOC_LITERAL(22, 435, 3), // "wnd"
QT_MOC_LITERAL(23, 439, 38), // "HandleCreateRecvScreenMediaRe..."
QT_MOC_LITERAL(24, 478, 38), // "HandleRemoveRecvScreenMediaRe..."
QT_MOC_LITERAL(25, 517, 35), // "HandleScreenShareControlReque..."
QT_MOC_LITERAL(26, 553, 20), // "RecoveryMediaProcess"
QT_MOC_LITERAL(27, 574, 15), // "MediaStreamType"
QT_MOC_LITERAL(28, 590, 7), // "is_send"
QT_MOC_LITERAL(29, 598, 14) // "remove_user_id"

    },
    "IMediaController\0"
    "HandleSendRTPStatToQoSServerRequestSlot\0"
    "\0RtpStatItem\0rtp_stat\0"
    "HandleSendRecStatToServerRequestSlot\0"
    "RecStatItem\0recstat\0"
    "HandleAddRecvStreamMediaRequestSlot\0"
    "remote_vuri\0screen_index\0seat\0"
    "use_small_video\0HandleRemoveRecvStreamMediaRequestSlot\0"
    "HandleChangeVideoSeatSlot\0remote_uri\0"
    "HandleCreateSendStreamMediaRequestSlot\0"
    "HandleControlSendMediaRequestSlot\0"
    "MediaControlType\0type\0enable\0"
    "HandleCreateSendScreenMediaRequestSlot\0"
    "wnd\0HandleCreateRecvScreenMediaRequestSlot\0"
    "HandleRemoveRecvScreenMediaRequestSlot\0"
    "HandleScreenShareControlRequestSlot\0"
    "RecoveryMediaProcess\0MediaStreamType\0"
    "is_send\0remove_user_id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IMediaController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       5,    1,   77,    2, 0x0a /* Public */,
       8,    4,   80,    2, 0x0a /* Public */,
      13,    1,   89,    2, 0x0a /* Public */,
      14,    3,   92,    2, 0x0a /* Public */,
      16,    0,   99,    2, 0x0a /* Public */,
      17,    2,  100,    2, 0x0a /* Public */,
      21,    1,  105,    2, 0x0a /* Public */,
      23,    2,  108,    2, 0x0a /* Public */,
      24,    2,  113,    2, 0x0a /* Public */,
      25,    1,  118,    2, 0x0a /* Public */,
      26,    3,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   15,   10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18, QMetaType::Bool,   19,   20,
    QMetaType::Void, QMetaType::QRect,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   15,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   15,   10,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, 0x80000000 | 27, QMetaType::Bool, QMetaType::QString,   19,   28,   29,

       0        // eod
};

void IMediaController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IMediaController *_t = static_cast<IMediaController *>(_o);
        switch (_id) {
        case 0: _t->HandleSendRTPStatToQoSServerRequestSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 1: _t->HandleSendRecStatToServerRequestSlot((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 2: _t->HandleAddRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 3: _t->HandleRemoveRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->HandleChangeVideoSeatSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->HandleCreateSendStreamMediaRequestSlot(); break;
        case 6: _t->HandleControlSendMediaRequestSlot((*reinterpret_cast< MediaControlType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->HandleCreateSendScreenMediaRequestSlot((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 8: _t->HandleCreateRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->HandleRemoveRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->HandleScreenShareControlRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->RecoveryMediaProcess((*reinterpret_cast< MediaStreamType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject IMediaController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IMediaController.data,
      qt_meta_data_IMediaController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IMediaController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IMediaController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IMediaController.stringdata))
        return static_cast<void*>(const_cast< IMediaController*>(this));
    return QObject::qt_metacast(_clname);
}

int IMediaController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
