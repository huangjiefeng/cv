/****************************************************************************
** Meta object code from reading C++ file 'msg_receiver_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../msg_receiver_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msg_receiver_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IMsgReceiver_t {
    QByteArrayData data[61];
    char stringdata[1273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IMsgReceiver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IMsgReceiver_t qt_meta_stringdata_IMsgReceiver = {
    {
QT_MOC_LITERAL(0, 0, 12), // "IMsgReceiver"
QT_MOC_LITERAL(1, 13, 34), // "NotifySipAccountStateChangedS..."
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 4), // "name"
QT_MOC_LITERAL(4, 54, 5), // "state"
QT_MOC_LITERAL(5, 60, 26), // "NotifyRecvSAVAResultSignal"
QT_MOC_LITERAL(6, 87, 5), // "valid"
QT_MOC_LITERAL(7, 93, 30), // "NotifyRecvTerminalConfigSignal"
QT_MOC_LITERAL(8, 124, 10), // "ConfigDict"
QT_MOC_LITERAL(9, 135, 6), // "config"
QT_MOC_LITERAL(10, 142, 30), // "NotifyRecvConferenceListSignal"
QT_MOC_LITERAL(11, 173, 14), // "ConferenceList"
QT_MOC_LITERAL(12, 188, 11), // "conferences"
QT_MOC_LITERAL(13, 200, 32), // "NotifyRecvFloorControlInfoSignal"
QT_MOC_LITERAL(14, 233, 23), // "FloorControlInfoPointer"
QT_MOC_LITERAL(15, 257, 18), // "floor_control_info"
QT_MOC_LITERAL(16, 276, 28), // "NotifyRecvTerminalListSignal"
QT_MOC_LITERAL(17, 305, 12), // "TerminalList"
QT_MOC_LITERAL(18, 318, 9), // "terminals"
QT_MOC_LITERAL(19, 328, 29), // "NotifyRecvTerminalLoginSignal"
QT_MOC_LITERAL(20, 358, 30), // "NotifyRecvTerminalLogoutSignal"
QT_MOC_LITERAL(21, 389, 3), // "uri"
QT_MOC_LITERAL(22, 393, 30), // "NotifyRecvTerminalHandUpSignal"
QT_MOC_LITERAL(23, 424, 32), // "NotifyRecvTerminalHandDownSignal"
QT_MOC_LITERAL(24, 457, 31), // "NotifyRecvStartMediaRelaySignal"
QT_MOC_LITERAL(25, 489, 13), // "MediaRelayMsg"
QT_MOC_LITERAL(26, 503, 4), // "info"
QT_MOC_LITERAL(27, 508, 31), // "NotifyRecvStartMediaReplySignal"
QT_MOC_LITERAL(28, 540, 13), // "MediaReplyMsg"
QT_MOC_LITERAL(29, 554, 30), // "NotifyRecvStopMediaRelaySignal"
QT_MOC_LITERAL(30, 585, 30), // "NotifyRecvStopMediaReplySignal"
QT_MOC_LITERAL(31, 616, 33), // "NotifyRecvRemoteMediaStatusSi..."
QT_MOC_LITERAL(32, 650, 14), // "MediaStatusMsg"
QT_MOC_LITERAL(33, 665, 32), // "NotifyRecvMediaControlInfoSignal"
QT_MOC_LITERAL(34, 698, 15), // "MediaControlMsg"
QT_MOC_LITERAL(35, 714, 34), // "NotifyRecvQoSLoginPermissionS..."
QT_MOC_LITERAL(36, 749, 19), // "LoginPermissionInfo"
QT_MOC_LITERAL(37, 769, 33), // "NotifyRecvQoSLoginRejectionSi..."
QT_MOC_LITERAL(38, 803, 18), // "LoginRejectionInfo"
QT_MOC_LITERAL(39, 822, 29), // "NotifyRecvQoSServerInfoSignal"
QT_MOC_LITERAL(40, 852, 13), // "QoSServerInfo"
QT_MOC_LITERAL(41, 866, 27), // "NotifyRecvPermitSpeakSignal"
QT_MOC_LITERAL(42, 894, 27), // "NotifyRecvForbidSpeakSignal"
QT_MOC_LITERAL(43, 922, 31), // "NotifyRecvSpeakerTerminalSignal"
QT_MOC_LITERAL(44, 954, 30), // "NotifyRecvChairmanUpdateSignal"
QT_MOC_LITERAL(45, 985, 24), // "NotifyRecvChairmanSignal"
QT_MOC_LITERAL(46, 1010, 36), // "NotifyRecvSharedScreenTermina..."
QT_MOC_LITERAL(47, 1047, 35), // "NotifyRecvSharedScreenControl..."
QT_MOC_LITERAL(48, 1083, 2), // "ip"
QT_MOC_LITERAL(49, 1086, 6), // "enable"
QT_MOC_LITERAL(50, 1093, 29), // "NotifyRecvOnlineMessageSignal"
QT_MOC_LITERAL(51, 1123, 4), // "type"
QT_MOC_LITERAL(52, 1128, 4), // "from"
QT_MOC_LITERAL(53, 1133, 23), // "NotifyTextMessageSignal"
QT_MOC_LITERAL(54, 1157, 4), // "text"
QT_MOC_LITERAL(55, 1162, 6), // "sender"
QT_MOC_LITERAL(56, 1169, 29), // "NotifyRecvRecordControlSignal"
QT_MOC_LITERAL(57, 1199, 16), // "RecordControlMsg"
QT_MOC_LITERAL(58, 1216, 3), // "msg"
QT_MOC_LITERAL(59, 1220, 32), // "NotifyRecvRecordControlAckSignal"
QT_MOC_LITERAL(60, 1253, 19) // "RecordControlAckMsg"

    },
    "IMsgReceiver\0NotifySipAccountStateChangedSignal\0"
    "\0name\0state\0NotifyRecvSAVAResultSignal\0"
    "valid\0NotifyRecvTerminalConfigSignal\0"
    "ConfigDict\0config\0NotifyRecvConferenceListSignal\0"
    "ConferenceList\0conferences\0"
    "NotifyRecvFloorControlInfoSignal\0"
    "FloorControlInfoPointer\0floor_control_info\0"
    "NotifyRecvTerminalListSignal\0TerminalList\0"
    "terminals\0NotifyRecvTerminalLoginSignal\0"
    "NotifyRecvTerminalLogoutSignal\0uri\0"
    "NotifyRecvTerminalHandUpSignal\0"
    "NotifyRecvTerminalHandDownSignal\0"
    "NotifyRecvStartMediaRelaySignal\0"
    "MediaRelayMsg\0info\0NotifyRecvStartMediaReplySignal\0"
    "MediaReplyMsg\0NotifyRecvStopMediaRelaySignal\0"
    "NotifyRecvStopMediaReplySignal\0"
    "NotifyRecvRemoteMediaStatusSignal\0"
    "MediaStatusMsg\0NotifyRecvMediaControlInfoSignal\0"
    "MediaControlMsg\0NotifyRecvQoSLoginPermissionSignal\0"
    "LoginPermissionInfo\0"
    "NotifyRecvQoSLoginRejectionSignal\0"
    "LoginRejectionInfo\0NotifyRecvQoSServerInfoSignal\0"
    "QoSServerInfo\0NotifyRecvPermitSpeakSignal\0"
    "NotifyRecvForbidSpeakSignal\0"
    "NotifyRecvSpeakerTerminalSignal\0"
    "NotifyRecvChairmanUpdateSignal\0"
    "NotifyRecvChairmanSignal\0"
    "NotifyRecvSharedScreenTerminalSignal\0"
    "NotifyRecvSharedScreenControlSignal\0"
    "ip\0enable\0NotifyRecvOnlineMessageSignal\0"
    "type\0from\0NotifyTextMessageSignal\0"
    "text\0sender\0NotifyRecvRecordControlSignal\0"
    "RecordControlMsg\0msg\0"
    "NotifyRecvRecordControlAckSignal\0"
    "RecordControlAckMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IMsgReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      30,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  164,    2, 0x06 /* Public */,
       5,    1,  169,    2, 0x06 /* Public */,
       7,    1,  172,    2, 0x06 /* Public */,
      10,    1,  175,    2, 0x06 /* Public */,
      13,    1,  178,    2, 0x06 /* Public */,
      16,    1,  181,    2, 0x06 /* Public */,
      19,    1,  184,    2, 0x06 /* Public */,
      20,    1,  187,    2, 0x06 /* Public */,
      22,    1,  190,    2, 0x06 /* Public */,
      23,    1,  193,    2, 0x06 /* Public */,
      24,    1,  196,    2, 0x06 /* Public */,
      27,    1,  199,    2, 0x06 /* Public */,
      29,    1,  202,    2, 0x06 /* Public */,
      30,    1,  205,    2, 0x06 /* Public */,
      31,    1,  208,    2, 0x06 /* Public */,
      33,    1,  211,    2, 0x06 /* Public */,
      35,    1,  214,    2, 0x06 /* Public */,
      37,    1,  217,    2, 0x06 /* Public */,
      39,    1,  220,    2, 0x06 /* Public */,
      41,    1,  223,    2, 0x06 /* Public */,
      42,    1,  226,    2, 0x06 /* Public */,
      43,    1,  229,    2, 0x06 /* Public */,
      44,    1,  232,    2, 0x06 /* Public */,
      45,    1,  235,    2, 0x06 /* Public */,
      46,    1,  238,    2, 0x06 /* Public */,
      47,    2,  241,    2, 0x06 /* Public */,
      50,    2,  246,    2, 0x06 /* Public */,
      53,    2,  251,    2, 0x06 /* Public */,
      56,    2,  256,    2, 0x06 /* Public */,
      59,    2,  261,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 28,   26,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 28,   26,
    QMetaType::Void, 0x80000000 | 32,   26,
    QMetaType::Void, 0x80000000 | 34,   26,
    QMetaType::Void, 0x80000000 | 36,   26,
    QMetaType::Void, 0x80000000 | 38,   26,
    QMetaType::Void, 0x80000000 | 40,   26,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   48,   49,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   51,   52,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   54,   55,
    QMetaType::Void, 0x80000000 | 57, QMetaType::QString,   58,   52,
    QMetaType::Void, 0x80000000 | 60, QMetaType::QString,   58,   52,

       0        // eod
};

void IMsgReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IMsgReceiver *_t = static_cast<IMsgReceiver *>(_o);
        switch (_id) {
        case 0: _t->NotifySipAccountStateChangedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->NotifyRecvSAVAResultSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->NotifyRecvTerminalConfigSignal((*reinterpret_cast< const ConfigDict(*)>(_a[1]))); break;
        case 3: _t->NotifyRecvConferenceListSignal((*reinterpret_cast< const ConferenceList(*)>(_a[1]))); break;
        case 4: _t->NotifyRecvFloorControlInfoSignal((*reinterpret_cast< const FloorControlInfoPointer(*)>(_a[1]))); break;
        case 5: _t->NotifyRecvTerminalListSignal((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 6: _t->NotifyRecvTerminalLoginSignal((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 7: _t->NotifyRecvTerminalLogoutSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->NotifyRecvTerminalHandUpSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->NotifyRecvTerminalHandDownSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->NotifyRecvStartMediaRelaySignal((*reinterpret_cast< const MediaRelayMsg(*)>(_a[1]))); break;
        case 11: _t->NotifyRecvStartMediaReplySignal((*reinterpret_cast< const MediaReplyMsg(*)>(_a[1]))); break;
        case 12: _t->NotifyRecvStopMediaRelaySignal((*reinterpret_cast< const MediaRelayMsg(*)>(_a[1]))); break;
        case 13: _t->NotifyRecvStopMediaReplySignal((*reinterpret_cast< const MediaReplyMsg(*)>(_a[1]))); break;
        case 14: _t->NotifyRecvRemoteMediaStatusSignal((*reinterpret_cast< const MediaStatusMsg(*)>(_a[1]))); break;
        case 15: _t->NotifyRecvMediaControlInfoSignal((*reinterpret_cast< const MediaControlMsg(*)>(_a[1]))); break;
        case 16: _t->NotifyRecvQoSLoginPermissionSignal((*reinterpret_cast< const LoginPermissionInfo(*)>(_a[1]))); break;
        case 17: _t->NotifyRecvQoSLoginRejectionSignal((*reinterpret_cast< const LoginRejectionInfo(*)>(_a[1]))); break;
        case 18: _t->NotifyRecvQoSServerInfoSignal((*reinterpret_cast< const QoSServerInfo(*)>(_a[1]))); break;
        case 19: _t->NotifyRecvPermitSpeakSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->NotifyRecvForbidSpeakSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->NotifyRecvSpeakerTerminalSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->NotifyRecvChairmanUpdateSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->NotifyRecvChairmanSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->NotifyRecvSharedScreenTerminalSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->NotifyRecvSharedScreenControlSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 26: _t->NotifyRecvOnlineMessageSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 27: _t->NotifyTextMessageSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 28: _t->NotifyRecvRecordControlSignal((*reinterpret_cast< const RecordControlMsg(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 29: _t->NotifyRecvRecordControlAckSignal((*reinterpret_cast< const RecordControlAckMsg(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IMsgReceiver::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifySipAccountStateChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvSAVAResultSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const ConfigDict & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalConfigSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const ConferenceList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvConferenceListSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const FloorControlInfoPointer & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvFloorControlInfoSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const TerminalList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalListSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const TerminalList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalLoginSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalLogoutSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalHandUpSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvTerminalHandDownSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaRelayMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvStartMediaRelaySignal)) {
                *result = 10;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaReplyMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvStartMediaReplySignal)) {
                *result = 11;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaRelayMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvStopMediaRelaySignal)) {
                *result = 12;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaReplyMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvStopMediaReplySignal)) {
                *result = 13;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaStatusMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvRemoteMediaStatusSignal)) {
                *result = 14;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const MediaControlMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvMediaControlInfoSignal)) {
                *result = 15;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const LoginPermissionInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvQoSLoginPermissionSignal)) {
                *result = 16;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const LoginRejectionInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvQoSLoginRejectionSignal)) {
                *result = 17;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QoSServerInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvQoSServerInfoSignal)) {
                *result = 18;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvPermitSpeakSignal)) {
                *result = 19;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvForbidSpeakSignal)) {
                *result = 20;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvSpeakerTerminalSignal)) {
                *result = 21;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvChairmanUpdateSignal)) {
                *result = 22;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvChairmanSignal)) {
                *result = 23;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvSharedScreenTerminalSignal)) {
                *result = 24;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvSharedScreenControlSignal)) {
                *result = 25;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvOnlineMessageSignal)) {
                *result = 26;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyTextMessageSignal)) {
                *result = 27;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const RecordControlMsg & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvRecordControlSignal)) {
                *result = 28;
            }
        }
        {
            typedef void (IMsgReceiver::*_t)(const RecordControlAckMsg & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IMsgReceiver::NotifyRecvRecordControlAckSignal)) {
                *result = 29;
            }
        }
    }
}

const QMetaObject IMsgReceiver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IMsgReceiver.data,
      qt_meta_data_IMsgReceiver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IMsgReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IMsgReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IMsgReceiver.stringdata))
        return static_cast<void*>(const_cast< IMsgReceiver*>(this));
    return QObject::qt_metacast(_clname);
}

int IMsgReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void IMsgReceiver::NotifySipAccountStateChangedSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IMsgReceiver::NotifyRecvSAVAResultSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IMsgReceiver::NotifyRecvTerminalConfigSignal(const ConfigDict & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IMsgReceiver::NotifyRecvConferenceListSignal(const ConferenceList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void IMsgReceiver::NotifyRecvFloorControlInfoSignal(const FloorControlInfoPointer & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void IMsgReceiver::NotifyRecvTerminalListSignal(const TerminalList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void IMsgReceiver::NotifyRecvTerminalLoginSignal(const TerminalList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void IMsgReceiver::NotifyRecvTerminalLogoutSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void IMsgReceiver::NotifyRecvTerminalHandUpSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void IMsgReceiver::NotifyRecvTerminalHandDownSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void IMsgReceiver::NotifyRecvStartMediaRelaySignal(const MediaRelayMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void IMsgReceiver::NotifyRecvStartMediaReplySignal(const MediaReplyMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void IMsgReceiver::NotifyRecvStopMediaRelaySignal(const MediaRelayMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void IMsgReceiver::NotifyRecvStopMediaReplySignal(const MediaReplyMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void IMsgReceiver::NotifyRecvRemoteMediaStatusSignal(const MediaStatusMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void IMsgReceiver::NotifyRecvMediaControlInfoSignal(const MediaControlMsg & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void IMsgReceiver::NotifyRecvQoSLoginPermissionSignal(const LoginPermissionInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void IMsgReceiver::NotifyRecvQoSLoginRejectionSignal(const LoginRejectionInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void IMsgReceiver::NotifyRecvQoSServerInfoSignal(const QoSServerInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void IMsgReceiver::NotifyRecvPermitSpeakSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void IMsgReceiver::NotifyRecvForbidSpeakSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void IMsgReceiver::NotifyRecvSpeakerTerminalSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void IMsgReceiver::NotifyRecvChairmanUpdateSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void IMsgReceiver::NotifyRecvChairmanSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void IMsgReceiver::NotifyRecvSharedScreenTerminalSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void IMsgReceiver::NotifyRecvSharedScreenControlSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void IMsgReceiver::NotifyRecvOnlineMessageSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void IMsgReceiver::NotifyTextMessageSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void IMsgReceiver::NotifyRecvRecordControlSignal(const RecordControlMsg & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void IMsgReceiver::NotifyRecvRecordControlAckSignal(const RecordControlAckMsg & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}
QT_END_MOC_NAMESPACE
