/****************************************************************************
** Meta object code from reading C++ file 'vconf_sip_msg_sender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vconf_sip_msg_sender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vconf_sip_msg_sender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VConfSipMsgSender_t {
    QByteArrayData data[16];
    char stringdata[320];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VConfSipMsgSender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VConfSipMsgSender_t qt_meta_stringdata_VConfSipMsgSender = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VConfSipMsgSender"
QT_MOC_LITERAL(1, 18, 34), // "NotifyVRequestConferenceListS..."
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 35), // "NotifyVSendConferenceJoinInfo..."
QT_MOC_LITERAL(4, 90, 28), // "NotifyVRequestStartMediaSlot"
QT_MOC_LITERAL(5, 119, 15), // "MediaRelayParam"
QT_MOC_LITERAL(6, 135, 5), // "param"
QT_MOC_LITERAL(7, 141, 25), // "RequestConferenceListSlot"
QT_MOC_LITERAL(8, 167, 21), // "RequestStartMediaSlot"
QT_MOC_LITERAL(9, 189, 23), // "RequestTerminalListSlot"
QT_MOC_LITERAL(10, 213, 14), // "conference_uri"
QT_MOC_LITERAL(11, 228, 26), // "SendConferenceJoinInfoSlot"
QT_MOC_LITERAL(12, 255, 19), // "ConferenceJoinParam"
QT_MOC_LITERAL(13, 275, 7), // "SDPInfo"
QT_MOC_LITERAL(14, 283, 8), // "sdp_info"
QT_MOC_LITERAL(15, 292, 27) // "SendConferenceLeaveInfoSlot"

    },
    "VConfSipMsgSender\0NotifyVRequestConferenceListSignal\0"
    "\0NotifyVSendConferenceJoinInfoSignal\0"
    "NotifyVRequestStartMediaSlot\0"
    "MediaRelayParam\0param\0RequestConferenceListSlot\0"
    "RequestStartMediaSlot\0RequestTerminalListSlot\0"
    "conference_uri\0SendConferenceJoinInfoSlot\0"
    "ConferenceJoinParam\0SDPInfo\0sdp_info\0"
    "SendConferenceLeaveInfoSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VConfSipMsgSender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   59,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
       9,    1,   63,    2, 0x0a /* Public */,
      11,    2,   66,    2, 0x0a /* Public */,
      15,    1,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 13,    6,   14,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void VConfSipMsgSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VConfSipMsgSender *_t = static_cast<VConfSipMsgSender *>(_o);
        switch (_id) {
        case 0: _t->NotifyVRequestConferenceListSignal(); break;
        case 1: _t->NotifyVSendConferenceJoinInfoSignal(); break;
        case 2: _t->NotifyVRequestStartMediaSlot((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 3: _t->RequestConferenceListSlot(); break;
        case 4: _t->RequestStartMediaSlot((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 5: _t->RequestTerminalListSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->SendConferenceJoinInfoSlot((*reinterpret_cast< const ConferenceJoinParam(*)>(_a[1])),(*reinterpret_cast< const SDPInfo(*)>(_a[2]))); break;
        case 7: _t->SendConferenceLeaveInfoSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VConfSipMsgSender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VConfSipMsgSender::NotifyVRequestConferenceListSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (VConfSipMsgSender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VConfSipMsgSender::NotifyVSendConferenceJoinInfoSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (VConfSipMsgSender::*_t)(const MediaRelayParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VConfSipMsgSender::NotifyVRequestStartMediaSlot)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject VConfSipMsgSender::staticMetaObject = {
    { &SipMsgSender::staticMetaObject, qt_meta_stringdata_VConfSipMsgSender.data,
      qt_meta_data_VConfSipMsgSender,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VConfSipMsgSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VConfSipMsgSender::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VConfSipMsgSender.stringdata))
        return static_cast<void*>(const_cast< VConfSipMsgSender*>(this));
    return SipMsgSender::qt_metacast(_clname);
}

int VConfSipMsgSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SipMsgSender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void VConfSipMsgSender::NotifyVRequestConferenceListSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void VConfSipMsgSender::NotifyVSendConferenceJoinInfoSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void VConfSipMsgSender::NotifyVRequestStartMediaSlot(const MediaRelayParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
