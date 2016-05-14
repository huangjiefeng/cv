/****************************************************************************
** Meta object code from reading C++ file 'sip_msg_sender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sip_msg_sender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sip_msg_sender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SipMsgSender_t {
    QByteArrayData data[48];
    char stringdata[967];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SipMsgSender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SipMsgSender_t qt_meta_stringdata_SipMsgSender = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SipMsgSender"
QT_MOC_LITERAL(1, 13, 25), // "SendTerminalLoginInfoSlot"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 18), // "TerminalLoginParam"
QT_MOC_LITERAL(4, 59, 5), // "param"
QT_MOC_LITERAL(5, 65, 27), // "SendTerminalInfoForSAVASlot"
QT_MOC_LITERAL(6, 93, 17), // "TerminalSAVAParam"
QT_MOC_LITERAL(7, 111, 36), // "SendRegisterHeartBeatToSipSer..."
QT_MOC_LITERAL(8, 148, 34), // "SendOnlineHeartBeatToKeepLogi..."
QT_MOC_LITERAL(9, 183, 18), // "SendOnlineTestSlot"
QT_MOC_LITERAL(10, 202, 26), // "SendTerminalLogoutInfoSlot"
QT_MOC_LITERAL(11, 229, 25), // "SendGetTerminalConfigSlot"
QT_MOC_LITERAL(12, 255, 28), // "SendReportTerminalConfigSlot"
QT_MOC_LITERAL(13, 284, 10), // "ConfigDict"
QT_MOC_LITERAL(14, 295, 25), // "RequestConferenceListSlot"
QT_MOC_LITERAL(15, 321, 27), // "RequestFloorControlInfoSlot"
QT_MOC_LITERAL(16, 349, 3), // "cid"
QT_MOC_LITERAL(17, 353, 26), // "SendConferenceJoinInfoSlot"
QT_MOC_LITERAL(18, 380, 19), // "ConferenceJoinParam"
QT_MOC_LITERAL(19, 400, 7), // "SDPInfo"
QT_MOC_LITERAL(20, 408, 8), // "sdp_info"
QT_MOC_LITERAL(21, 417, 41), // "SendOnlineHeartBeatToKeepInCo..."
QT_MOC_LITERAL(22, 459, 14), // "conference_uri"
QT_MOC_LITERAL(23, 474, 27), // "SendConferenceLeaveInfoSlot"
QT_MOC_LITERAL(24, 502, 23), // "RequestTerminalListSlot"
QT_MOC_LITERAL(25, 526, 20), // "QueryMainSpeakerSlot"
QT_MOC_LITERAL(26, 547, 17), // "QueryChairmanSlot"
QT_MOC_LITERAL(27, 565, 20), // "QueryShareScreenSlot"
QT_MOC_LITERAL(28, 586, 17), // "SendHandUpMsgSlot"
QT_MOC_LITERAL(29, 604, 9), // "local_uri"
QT_MOC_LITERAL(30, 614, 6), // "handup"
QT_MOC_LITERAL(31, 621, 21), // "SendAllowSpeakMsgSlot"
QT_MOC_LITERAL(32, 643, 12), // "terminal_uri"
QT_MOC_LITERAL(33, 656, 5), // "allow"
QT_MOC_LITERAL(34, 662, 21), // "RequestStartMediaSlot"
QT_MOC_LITERAL(35, 684, 15), // "MediaRelayParam"
QT_MOC_LITERAL(36, 700, 26), // "ReplyStartMediaRequestSlot"
QT_MOC_LITERAL(37, 727, 15), // "MediaReplyParam"
QT_MOC_LITERAL(38, 743, 20), // "RequestStopMediaSlot"
QT_MOC_LITERAL(39, 764, 25), // "ReplyStopMediaRequestSlot"
QT_MOC_LITERAL(40, 790, 26), // "SendScreenShareControlSlot"
QT_MOC_LITERAL(41, 817, 18), // "ScreenControlParam"
QT_MOC_LITERAL(42, 836, 24), // "SendMediaControlInfoSlot"
QT_MOC_LITERAL(43, 861, 17), // "MediaControlParam"
QT_MOC_LITERAL(44, 879, 21), // "SendRecordControlSlot"
QT_MOC_LITERAL(45, 901, 18), // "RecordControlParam"
QT_MOC_LITERAL(46, 920, 24), // "SendRecordControlAckSlot"
QT_MOC_LITERAL(47, 945, 21) // "RecordControlAckParam"

    },
    "SipMsgSender\0SendTerminalLoginInfoSlot\0"
    "\0TerminalLoginParam\0param\0"
    "SendTerminalInfoForSAVASlot\0"
    "TerminalSAVAParam\0"
    "SendRegisterHeartBeatToSipServerSlot\0"
    "SendOnlineHeartBeatToKeepLoginSlot\0"
    "SendOnlineTestSlot\0SendTerminalLogoutInfoSlot\0"
    "SendGetTerminalConfigSlot\0"
    "SendReportTerminalConfigSlot\0ConfigDict\0"
    "RequestConferenceListSlot\0"
    "RequestFloorControlInfoSlot\0cid\0"
    "SendConferenceJoinInfoSlot\0"
    "ConferenceJoinParam\0SDPInfo\0sdp_info\0"
    "SendOnlineHeartBeatToKeepInConferenceSlot\0"
    "conference_uri\0SendConferenceLeaveInfoSlot\0"
    "RequestTerminalListSlot\0QueryMainSpeakerSlot\0"
    "QueryChairmanSlot\0QueryShareScreenSlot\0"
    "SendHandUpMsgSlot\0local_uri\0handup\0"
    "SendAllowSpeakMsgSlot\0terminal_uri\0"
    "allow\0RequestStartMediaSlot\0MediaRelayParam\0"
    "ReplyStartMediaRequestSlot\0MediaReplyParam\0"
    "RequestStopMediaSlot\0ReplyStopMediaRequestSlot\0"
    "SendScreenShareControlSlot\0"
    "ScreenControlParam\0SendMediaControlInfoSlot\0"
    "MediaControlParam\0SendRecordControlSlot\0"
    "RecordControlParam\0SendRecordControlAckSlot\0"
    "RecordControlAckParam"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SipMsgSender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  149,    2, 0x0a /* Public */,
       5,    1,  152,    2, 0x0a /* Public */,
       7,    0,  155,    2, 0x0a /* Public */,
       8,    0,  156,    2, 0x0a /* Public */,
       9,    0,  157,    2, 0x0a /* Public */,
      10,    0,  158,    2, 0x0a /* Public */,
      11,    0,  159,    2, 0x0a /* Public */,
      12,    1,  160,    2, 0x0a /* Public */,
      14,    0,  163,    2, 0x0a /* Public */,
      15,    1,  164,    2, 0x0a /* Public */,
      17,    2,  167,    2, 0x0a /* Public */,
      21,    1,  172,    2, 0x0a /* Public */,
      23,    1,  175,    2, 0x0a /* Public */,
      24,    1,  178,    2, 0x0a /* Public */,
      25,    1,  181,    2, 0x0a /* Public */,
      26,    1,  184,    2, 0x0a /* Public */,
      27,    1,  187,    2, 0x0a /* Public */,
      28,    3,  190,    2, 0x0a /* Public */,
      31,    3,  197,    2, 0x0a /* Public */,
      34,    1,  204,    2, 0x0a /* Public */,
      36,    1,  207,    2, 0x0a /* Public */,
      38,    1,  210,    2, 0x0a /* Public */,
      39,    1,  213,    2, 0x0a /* Public */,
      40,    1,  216,    2, 0x0a /* Public */,
      42,    1,  219,    2, 0x0a /* Public */,
      44,    1,  222,    2, 0x0a /* Public */,
      46,    1,  225,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 19,    4,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   22,   29,   30,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   22,   32,   33,
    QMetaType::Void, 0x80000000 | 35,    4,
    QMetaType::Void, 0x80000000 | 37,    4,
    QMetaType::Void, 0x80000000 | 35,    4,
    QMetaType::Void, 0x80000000 | 37,    4,
    QMetaType::Void, 0x80000000 | 41,    4,
    QMetaType::Void, 0x80000000 | 43,    4,
    QMetaType::Void, 0x80000000 | 45,    4,
    QMetaType::Void, 0x80000000 | 47,    4,

       0        // eod
};

void SipMsgSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SipMsgSender *_t = static_cast<SipMsgSender *>(_o);
        switch (_id) {
        case 0: _t->SendTerminalLoginInfoSlot((*reinterpret_cast< const TerminalLoginParam(*)>(_a[1]))); break;
        case 1: _t->SendTerminalInfoForSAVASlot((*reinterpret_cast< const TerminalSAVAParam(*)>(_a[1]))); break;
        case 2: _t->SendRegisterHeartBeatToSipServerSlot(); break;
        case 3: _t->SendOnlineHeartBeatToKeepLoginSlot(); break;
        case 4: _t->SendOnlineTestSlot(); break;
        case 5: _t->SendTerminalLogoutInfoSlot(); break;
        case 6: _t->SendGetTerminalConfigSlot(); break;
        case 7: _t->SendReportTerminalConfigSlot((*reinterpret_cast< const ConfigDict(*)>(_a[1]))); break;
        case 8: _t->RequestConferenceListSlot(); break;
        case 9: _t->RequestFloorControlInfoSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->SendConferenceJoinInfoSlot((*reinterpret_cast< const ConferenceJoinParam(*)>(_a[1])),(*reinterpret_cast< const SDPInfo(*)>(_a[2]))); break;
        case 11: _t->SendOnlineHeartBeatToKeepInConferenceSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->SendConferenceLeaveInfoSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->RequestTerminalListSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->QueryMainSpeakerSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->QueryChairmanSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->QueryShareScreenSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->SendHandUpMsgSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 18: _t->SendAllowSpeakMsgSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 19: _t->RequestStartMediaSlot((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 20: _t->ReplyStartMediaRequestSlot((*reinterpret_cast< const MediaReplyParam(*)>(_a[1]))); break;
        case 21: _t->RequestStopMediaSlot((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 22: _t->ReplyStopMediaRequestSlot((*reinterpret_cast< const MediaReplyParam(*)>(_a[1]))); break;
        case 23: _t->SendScreenShareControlSlot((*reinterpret_cast< const ScreenControlParam(*)>(_a[1]))); break;
        case 24: _t->SendMediaControlInfoSlot((*reinterpret_cast< const MediaControlParam(*)>(_a[1]))); break;
        case 25: _t->SendRecordControlSlot((*reinterpret_cast< const RecordControlParam(*)>(_a[1]))); break;
        case 26: _t->SendRecordControlAckSlot((*reinterpret_cast< const RecordControlAckParam(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SipMsgSender::staticMetaObject = {
    { &IMsgSender::staticMetaObject, qt_meta_stringdata_SipMsgSender.data,
      qt_meta_data_SipMsgSender,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SipMsgSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SipMsgSender::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SipMsgSender.stringdata))
        return static_cast<void*>(const_cast< SipMsgSender*>(this));
    return IMsgSender::qt_metacast(_clname);
}

int SipMsgSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IMsgSender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
