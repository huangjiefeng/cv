/****************************************************************************
** Meta object code from reading C++ file 'conference_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../conference_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conference_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IConferenceController_t {
    QByteArrayData data[72];
    char stringdata[1668];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IConferenceController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IConferenceController_t qt_meta_stringdata_IConferenceController = {
    {
QT_MOC_LITERAL(0, 0, 21), // "IConferenceController"
QT_MOC_LITERAL(1, 22, 34), // "NotifyConferenceStateChangedS..."
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 14), // "conference_uri"
QT_MOC_LITERAL(4, 73, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(5, 112, 5), // "state"
QT_MOC_LITERAL(6, 118, 31), // "NotifyJoinConferenceErrorSignal"
QT_MOC_LITERAL(7, 150, 42), // "IConferenceController::JoinCo..."
QT_MOC_LITERAL(8, 193, 5), // "error"
QT_MOC_LITERAL(9, 199, 34), // "NotifyConferenceListReceivedS..."
QT_MOC_LITERAL(10, 234, 19), // "ConstConferenceList"
QT_MOC_LITERAL(11, 254, 15), // "conference_list"
QT_MOC_LITERAL(12, 270, 28), // "NotifyTerminalReceivedSignal"
QT_MOC_LITERAL(13, 299, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(14, 320, 8), // "terminal"
QT_MOC_LITERAL(15, 329, 27), // "NotifyTerminalRemovedSignal"
QT_MOC_LITERAL(16, 357, 12), // "terminal_uri"
QT_MOC_LITERAL(17, 370, 26), // "NotifyTerminalUpdateSignal"
QT_MOC_LITERAL(18, 397, 27), // "NotifyTerminalReloginSignal"
QT_MOC_LITERAL(19, 425, 32), // "NotifyTerminalCountChangedSignal"
QT_MOC_LITERAL(20, 458, 5), // "count"
QT_MOC_LITERAL(21, 464, 37), // "NotifyStreamMediaReceiveStart..."
QT_MOC_LITERAL(22, 502, 11), // "remote_vuri"
QT_MOC_LITERAL(23, 514, 35), // "NotifyStreamMediaReceiveReply..."
QT_MOC_LITERAL(24, 550, 10), // "permission"
QT_MOC_LITERAL(25, 561, 37), // "NotifyStreamMediaReceiveStopp..."
QT_MOC_LITERAL(26, 599, 33), // "NotifyEraseAudioDisplayDictSi..."
QT_MOC_LITERAL(27, 633, 32), // "NotifyScreenReceiveStartedSignal"
QT_MOC_LITERAL(28, 666, 10), // "remote_uri"
QT_MOC_LITERAL(29, 677, 12), // "screen_index"
QT_MOC_LITERAL(30, 690, 32), // "NotifyScreenReceiveStoppedSignal"
QT_MOC_LITERAL(31, 723, 24), // "NotifyTerminalSpeakSinal"
QT_MOC_LITERAL(32, 748, 5), // "allow"
QT_MOC_LITERAL(33, 754, 27), // "NotifyRTPStatReceivedSignal"
QT_MOC_LITERAL(34, 782, 11), // "RtpStatItem"
QT_MOC_LITERAL(35, 794, 9), // "rtp_state"
QT_MOC_LITERAL(36, 804, 26), // "NotifyTerminalHandUpSignal"
QT_MOC_LITERAL(37, 831, 6), // "handup"
QT_MOC_LITERAL(38, 838, 35), // "RequestStreamMediaAutoReceive..."
QT_MOC_LITERAL(39, 874, 31), // "HandleJoinConferenceRequestSlot"
QT_MOC_LITERAL(40, 906, 3), // "uri"
QT_MOC_LITERAL(41, 910, 32), // "HandleLeaveConferenceRequestSlot"
QT_MOC_LITERAL(42, 943, 31), // "HandleConferenceListRequestSlot"
QT_MOC_LITERAL(43, 975, 29), // "HandleTerminalListRequestSlot"
QT_MOC_LITERAL(44, 1005, 23), // "HandleHandUpRequestSlot"
QT_MOC_LITERAL(45, 1029, 7), // "hand_up"
QT_MOC_LITERAL(46, 1037, 29), // "HandleReceiveUDPQoSReportSlot"
QT_MOC_LITERAL(47, 1067, 8), // "rtp_stat"
QT_MOC_LITERAL(48, 1076, 35), // "HandleAddRecvStreamMediaReque..."
QT_MOC_LITERAL(49, 1112, 4), // "seat"
QT_MOC_LITERAL(50, 1117, 15), // "use_small_video"
QT_MOC_LITERAL(51, 1133, 12), // "enable_audio"
QT_MOC_LITERAL(52, 1146, 32), // "HandleStartMediaReplyTimeoutSlot"
QT_MOC_LITERAL(53, 1179, 38), // "HandleRemoveRecvStreamMediaRe..."
QT_MOC_LITERAL(54, 1218, 36), // "HanldePermitTerminalSpeakRequ..."
QT_MOC_LITERAL(55, 1255, 32), // "HandleChangeVideoSeatRequestSlot"
QT_MOC_LITERAL(56, 1288, 38), // "HandleCreateSendStreamMediaRe..."
QT_MOC_LITERAL(57, 1327, 33), // "HandleControlSendMediaRequest..."
QT_MOC_LITERAL(58, 1361, 16), // "MediaControlType"
QT_MOC_LITERAL(59, 1378, 4), // "type"
QT_MOC_LITERAL(60, 1383, 6), // "enable"
QT_MOC_LITERAL(61, 1390, 13), // "virtual_index"
QT_MOC_LITERAL(62, 1404, 38), // "HandleCreateSendScreenMediaRe..."
QT_MOC_LITERAL(63, 1443, 3), // "wnd"
QT_MOC_LITERAL(64, 1447, 38), // "HandleCreateRecvScreenMediaRe..."
QT_MOC_LITERAL(65, 1486, 38), // "HandleRemoveRecvScreenMediaRe..."
QT_MOC_LITERAL(66, 1525, 35), // "HandleScreenShareControlReque..."
QT_MOC_LITERAL(67, 1561, 37), // "HandleRecoveryMediaProcessReq..."
QT_MOC_LITERAL(68, 1599, 15), // "MediaStreamType"
QT_MOC_LITERAL(69, 1615, 7), // "is_send"
QT_MOC_LITERAL(70, 1623, 14), // "remove_user_id"
QT_MOC_LITERAL(71, 1638, 29) // "HandleAllMediaProcessExitSlot"

    },
    "IConferenceController\0"
    "NotifyConferenceStateChangedSignal\0\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0NotifyJoinConferenceErrorSignal\0"
    "IConferenceController::JoinConferenceError\0"
    "error\0NotifyConferenceListReceivedSignal\0"
    "ConstConferenceList\0conference_list\0"
    "NotifyTerminalReceivedSignal\0"
    "ConstTerminalPointer\0terminal\0"
    "NotifyTerminalRemovedSignal\0terminal_uri\0"
    "NotifyTerminalUpdateSignal\0"
    "NotifyTerminalReloginSignal\0"
    "NotifyTerminalCountChangedSignal\0count\0"
    "NotifyStreamMediaReceiveStartedSignal\0"
    "remote_vuri\0NotifyStreamMediaReceiveReplySignal\0"
    "permission\0NotifyStreamMediaReceiveStoppedSignal\0"
    "NotifyEraseAudioDisplayDictSignal\0"
    "NotifyScreenReceiveStartedSignal\0"
    "remote_uri\0screen_index\0"
    "NotifyScreenReceiveStoppedSignal\0"
    "NotifyTerminalSpeakSinal\0allow\0"
    "NotifyRTPStatReceivedSignal\0RtpStatItem\0"
    "rtp_state\0NotifyTerminalHandUpSignal\0"
    "handup\0RequestStreamMediaAutoReceiveSignal\0"
    "HandleJoinConferenceRequestSlot\0uri\0"
    "HandleLeaveConferenceRequestSlot\0"
    "HandleConferenceListRequestSlot\0"
    "HandleTerminalListRequestSlot\0"
    "HandleHandUpRequestSlot\0hand_up\0"
    "HandleReceiveUDPQoSReportSlot\0rtp_stat\0"
    "HandleAddRecvStreamMediaRequestSlot\0"
    "seat\0use_small_video\0enable_audio\0"
    "HandleStartMediaReplyTimeoutSlot\0"
    "HandleRemoveRecvStreamMediaRequestSlot\0"
    "HanldePermitTerminalSpeakRequestSlot\0"
    "HandleChangeVideoSeatRequestSlot\0"
    "HandleCreateSendStreamMediaRequestSlot\0"
    "HandleControlSendMediaRequestSlot\0"
    "MediaControlType\0type\0enable\0virtual_index\0"
    "HandleCreateSendScreenMediaRequestSlot\0"
    "wnd\0HandleCreateRecvScreenMediaRequestSlot\0"
    "HandleRemoveRecvScreenMediaRequestSlot\0"
    "HandleScreenShareControlRequestSlot\0"
    "HandleRecoveryMediaProcessRequestSlot\0"
    "MediaStreamType\0is_send\0remove_user_id\0"
    "HandleAllMediaProcessExitSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IConferenceController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  204,    2, 0x06 /* Public */,
       6,    2,  209,    2, 0x06 /* Public */,
       9,    1,  214,    2, 0x06 /* Public */,
      12,    2,  217,    2, 0x06 /* Public */,
      15,    2,  222,    2, 0x06 /* Public */,
      17,    2,  227,    2, 0x06 /* Public */,
      18,    2,  232,    2, 0x06 /* Public */,
      19,    2,  237,    2, 0x06 /* Public */,
      21,    1,  242,    2, 0x06 /* Public */,
      23,    2,  245,    2, 0x06 /* Public */,
      25,    1,  250,    2, 0x06 /* Public */,
      26,    0,  253,    2, 0x06 /* Public */,
      27,    2,  254,    2, 0x06 /* Public */,
      30,    2,  259,    2, 0x06 /* Public */,
      31,    2,  264,    2, 0x06 /* Public */,
      33,    1,  269,    2, 0x06 /* Public */,
      36,    1,  272,    2, 0x06 /* Public */,
      38,    1,  275,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      39,    1,  278,    2, 0x0a /* Public */,
      41,    0,  281,    2, 0x0a /* Public */,
      42,    0,  282,    2, 0x0a /* Public */,
      43,    0,  283,    2, 0x0a /* Public */,
      44,    1,  284,    2, 0x0a /* Public */,
      46,    1,  287,    2, 0x0a /* Public */,
      48,    5,  290,    2, 0x0a /* Public */,
      52,    1,  301,    2, 0x0a /* Public */,
      53,    1,  304,    2, 0x0a /* Public */,
      54,    2,  307,    2, 0x0a /* Public */,
      55,    3,  312,    2, 0x0a /* Public */,
      56,    0,  319,    2, 0x0a /* Public */,
      57,    3,  320,    2, 0x0a /* Public */,
      57,    2,  327,    2, 0x2a /* Public | MethodCloned */,
      62,    1,  332,    2, 0x0a /* Public */,
      64,    2,  335,    2, 0x0a /* Public */,
      65,    2,  340,    2, 0x0a /* Public */,
      66,    1,  345,    2, 0x0a /* Public */,
      67,    3,  348,    2, 0x0a /* Public */,
      71,    0,  355,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    3,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QString,   14,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,    3,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QString,   14,    3,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QString,   14,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   20,    3,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   22,   24,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   28,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   28,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   28,   32,
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void, QMetaType::Bool,   37,
    QMetaType::Void, QMetaType::QString,   22,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   40,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   45,
    QMetaType::Void, 0x80000000 | 34,   47,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,   22,   29,   49,   50,   51,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   40,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   22,   29,   49,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 58, QMetaType::Bool, QMetaType::Int,   59,   60,   61,
    QMetaType::Void, 0x80000000 | 58, QMetaType::Bool,   59,   60,
    QMetaType::Void, QMetaType::QRect,   63,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   28,   29,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   28,   29,
    QMetaType::Void, QMetaType::Bool,   60,
    QMetaType::Void, 0x80000000 | 68, QMetaType::Bool, QMetaType::QString,   59,   69,   70,
    QMetaType::Void,

       0        // eod
};

void IConferenceController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IConferenceController *_t = static_cast<IConferenceController *>(_o);
        switch (_id) {
        case 0: _t->NotifyConferenceStateChangedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 1: _t->NotifyJoinConferenceErrorSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::JoinConferenceError(*)>(_a[2]))); break;
        case 2: _t->NotifyConferenceListReceivedSignal((*reinterpret_cast< const ConstConferenceList(*)>(_a[1]))); break;
        case 3: _t->NotifyTerminalReceivedSignal((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->NotifyTerminalRemovedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->NotifyTerminalUpdateSignal((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->NotifyTerminalReloginSignal((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->NotifyTerminalCountChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->NotifyStreamMediaReceiveStartedSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->NotifyStreamMediaReceiveReplySignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->NotifyStreamMediaReceiveStoppedSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->NotifyEraseAudioDisplayDictSignal(); break;
        case 12: _t->NotifyScreenReceiveStartedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->NotifyScreenReceiveStoppedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->NotifyTerminalSpeakSinal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: _t->NotifyRTPStatReceivedSignal((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 16: _t->NotifyTerminalHandUpSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->RequestStreamMediaAutoReceiveSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->HandleJoinConferenceRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->HandleLeaveConferenceRequestSlot(); break;
        case 20: _t->HandleConferenceListRequestSlot(); break;
        case 21: _t->HandleTerminalListRequestSlot(); break;
        case 22: _t->HandleHandUpRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->HandleReceiveUDPQoSReportSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 24: _t->HandleAddRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 25: _t->HandleStartMediaReplyTimeoutSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->HandleRemoveRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->HanldePermitTerminalSpeakRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 28: _t->HandleChangeVideoSeatRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 29: _t->HandleCreateSendStreamMediaRequestSlot(); break;
        case 30: _t->HandleControlSendMediaRequestSlot((*reinterpret_cast< MediaControlType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 31: _t->HandleControlSendMediaRequestSlot((*reinterpret_cast< MediaControlType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 32: _t->HandleCreateSendScreenMediaRequestSlot((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 33: _t->HandleCreateRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 34: _t->HandleRemoveRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 35: _t->HandleScreenShareControlRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->HandleRecoveryMediaProcessRequestSlot((*reinterpret_cast< MediaStreamType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 37: _t->HandleAllMediaProcessExitSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IConferenceController::*_t)(const QString & , IConferenceController::ConferenceState );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyConferenceStateChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , IConferenceController::JoinConferenceError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyJoinConferenceErrorSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const ConstConferenceList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyConferenceListReceivedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (IConferenceController::*_t)(ConstTerminalPointer , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalReceivedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalRemovedSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (IConferenceController::*_t)(ConstTerminalPointer , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalUpdateSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (IConferenceController::*_t)(ConstTerminalPointer , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalReloginSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (IConferenceController::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalCountChangedSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyStreamMediaReceiveStartedSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyStreamMediaReceiveReplySignal)) {
                *result = 9;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyStreamMediaReceiveStoppedSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (IConferenceController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyEraseAudioDisplayDictSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyScreenReceiveStartedSignal)) {
                *result = 12;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyScreenReceiveStoppedSignal)) {
                *result = 13;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalSpeakSinal)) {
                *result = 14;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const RtpStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyRTPStatReceivedSignal)) {
                *result = 15;
            }
        }
        {
            typedef void (IConferenceController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::NotifyTerminalHandUpSignal)) {
                *result = 16;
            }
        }
        {
            typedef void (IConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IConferenceController::RequestStreamMediaAutoReceiveSignal)) {
                *result = 17;
            }
        }
    }
}

const QMetaObject IConferenceController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IConferenceController.data,
      qt_meta_data_IConferenceController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IConferenceController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IConferenceController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IConferenceController.stringdata))
        return static_cast<void*>(const_cast< IConferenceController*>(this));
    return QObject::qt_metacast(_clname);
}

int IConferenceController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void IConferenceController::NotifyConferenceStateChangedSignal(const QString & _t1, IConferenceController::ConferenceState _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IConferenceController::NotifyJoinConferenceErrorSignal(const QString & _t1, IConferenceController::JoinConferenceError _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IConferenceController::NotifyConferenceListReceivedSignal(const ConstConferenceList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IConferenceController::NotifyTerminalReceivedSignal(ConstTerminalPointer _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void IConferenceController::NotifyTerminalRemovedSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void IConferenceController::NotifyTerminalUpdateSignal(ConstTerminalPointer _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void IConferenceController::NotifyTerminalReloginSignal(ConstTerminalPointer _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void IConferenceController::NotifyTerminalCountChangedSignal(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void IConferenceController::NotifyStreamMediaReceiveStartedSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void IConferenceController::NotifyStreamMediaReceiveReplySignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void IConferenceController::NotifyStreamMediaReceiveStoppedSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void IConferenceController::NotifyEraseAudioDisplayDictSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}

// SIGNAL 12
void IConferenceController::NotifyScreenReceiveStartedSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void IConferenceController::NotifyScreenReceiveStoppedSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void IConferenceController::NotifyTerminalSpeakSinal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void IConferenceController::NotifyRTPStatReceivedSignal(const RtpStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void IConferenceController::NotifyTerminalHandUpSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void IConferenceController::RequestStreamMediaAutoReceiveSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
QT_END_MOC_NAMESPACE
