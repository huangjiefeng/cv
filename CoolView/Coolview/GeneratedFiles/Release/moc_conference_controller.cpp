/****************************************************************************
** Meta object code from reading C++ file 'conference_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../conference_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conference_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConferenceController_t {
    QByteArrayData data[129];
    char stringdata[3019];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceController_t qt_meta_stringdata_ConferenceController = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ConferenceController"
QT_MOC_LITERAL(1, 21, 34), // "SendConferenceListRequestMsgS..."
QT_MOC_LITERAL(2, 56, 0), // ""
QT_MOC_LITERAL(3, 57, 34), // "SendJoinConferenceRequestMsgS..."
QT_MOC_LITERAL(4, 92, 19), // "ConferenceJoinParam"
QT_MOC_LITERAL(5, 112, 16), // "conference_param"
QT_MOC_LITERAL(6, 129, 7), // "SDPInfo"
QT_MOC_LITERAL(7, 137, 8), // "sdp_info"
QT_MOC_LITERAL(8, 146, 26), // "SendServerOnlineTestSignal"
QT_MOC_LITERAL(9, 173, 40), // "SendHeartBeatToKeepInConferen..."
QT_MOC_LITERAL(10, 214, 14), // "conference_uri"
QT_MOC_LITERAL(11, 229, 35), // "SendLeaveConferenceRequestMsg..."
QT_MOC_LITERAL(12, 265, 32), // "SendTerminalListRequestMsgSignal"
QT_MOC_LITERAL(13, 298, 32), // "SendMainSpeakerInfoRequestSignal"
QT_MOC_LITERAL(14, 331, 29), // "SendChairmanInfoRequestSignal"
QT_MOC_LITERAL(15, 361, 33), // "SendSharedScreenInfoRequestSi..."
QT_MOC_LITERAL(16, 395, 19), // "SendHandUpMsgSignal"
QT_MOC_LITERAL(17, 415, 9), // "local_uri"
QT_MOC_LITERAL(18, 425, 6), // "handup"
QT_MOC_LITERAL(19, 432, 23), // "SendAllowSpeakMsgSignal"
QT_MOC_LITERAL(20, 456, 12), // "terminal_uri"
QT_MOC_LITERAL(21, 469, 5), // "allow"
QT_MOC_LITERAL(22, 475, 30), // "SendRequestStartMediaMsgSignal"
QT_MOC_LITERAL(23, 506, 15), // "MediaRelayParam"
QT_MOC_LITERAL(24, 522, 5), // "param"
QT_MOC_LITERAL(25, 528, 28), // "SendReplyStartMediaMsgSignal"
QT_MOC_LITERAL(26, 557, 15), // "MediaReplyParam"
QT_MOC_LITERAL(27, 573, 29), // "SendRequestStopMediaMsgSignal"
QT_MOC_LITERAL(28, 603, 27), // "SendReplyStopMediaMsgSignal"
QT_MOC_LITERAL(29, 631, 26), // "SendScreenControlMsgSignal"
QT_MOC_LITERAL(30, 658, 18), // "ScreenControlParam"
QT_MOC_LITERAL(31, 677, 25), // "SendMediaControlMsgSignal"
QT_MOC_LITERAL(32, 703, 17), // "MediaControlParam"
QT_MOC_LITERAL(33, 721, 34), // "SendTerminalListRequestByCIDS..."
QT_MOC_LITERAL(34, 756, 3), // "cid"
QT_MOC_LITERAL(35, 760, 27), // "SendFloorControlInfoRequest"
QT_MOC_LITERAL(36, 788, 30), // "CreateFloorControlClientSignal"
QT_MOC_LITERAL(37, 819, 8), // "uint16_t"
QT_MOC_LITERAL(38, 828, 6), // "userid"
QT_MOC_LITERAL(39, 835, 23), // "FloorControlInfoPointer"
QT_MOC_LITERAL(40, 859, 4), // "info"
QT_MOC_LITERAL(41, 864, 23), // "ApplyFloorRequestSignal"
QT_MOC_LITERAL(42, 888, 24), // "AcceptFloorRequestSignal"
QT_MOC_LITERAL(43, 913, 7), // "user_id"
QT_MOC_LITERAL(44, 921, 25), // "ReleaseFloorRequestSignal"
QT_MOC_LITERAL(45, 947, 31), // "DestoryFloorControlClientSignal"
QT_MOC_LITERAL(46, 979, 31), // "HandleConferenceListRequestSlot"
QT_MOC_LITERAL(47, 1011, 31), // "HandleJoinConferenceRequestSlot"
QT_MOC_LITERAL(48, 1043, 3), // "uri"
QT_MOC_LITERAL(49, 1047, 32), // "HandleLeaveConferenceRequestSlot"
QT_MOC_LITERAL(50, 1080, 29), // "HandleTerminalListRequestSlot"
QT_MOC_LITERAL(51, 1110, 23), // "HandleHandUpRequestSlot"
QT_MOC_LITERAL(52, 1134, 29), // "HandleReceiveUDPQoSReportSlot"
QT_MOC_LITERAL(53, 1164, 11), // "RtpStatItem"
QT_MOC_LITERAL(54, 1176, 8), // "rtp_stat"
QT_MOC_LITERAL(55, 1185, 35), // "HandleAddRecvStreamMediaReque..."
QT_MOC_LITERAL(56, 1221, 11), // "remote_vuri"
QT_MOC_LITERAL(57, 1233, 12), // "screen_index"
QT_MOC_LITERAL(58, 1246, 4), // "seat"
QT_MOC_LITERAL(59, 1251, 15), // "use_small_video"
QT_MOC_LITERAL(60, 1267, 12), // "enable_audio"
QT_MOC_LITERAL(61, 1280, 32), // "HandleStartMediaReplyTimeoutSlot"
QT_MOC_LITERAL(62, 1313, 38), // "HandleRemoveRecvStreamMediaRe..."
QT_MOC_LITERAL(63, 1352, 32), // "HandleChangeVideoSeatRequestSlot"
QT_MOC_LITERAL(64, 1385, 36), // "HanldePermitTerminalSpeakRequ..."
QT_MOC_LITERAL(65, 1422, 38), // "HandleCreateSendStreamMediaRe..."
QT_MOC_LITERAL(66, 1461, 33), // "HandleControlSendMediaRequest..."
QT_MOC_LITERAL(67, 1495, 16), // "MediaControlType"
QT_MOC_LITERAL(68, 1512, 4), // "type"
QT_MOC_LITERAL(69, 1517, 6), // "enable"
QT_MOC_LITERAL(70, 1524, 13), // "virtual_index"
QT_MOC_LITERAL(71, 1538, 38), // "HandleCreateSendScreenMediaRe..."
QT_MOC_LITERAL(72, 1577, 3), // "wnd"
QT_MOC_LITERAL(73, 1581, 38), // "HandleCreateRecvScreenMediaRe..."
QT_MOC_LITERAL(74, 1620, 10), // "remote_uri"
QT_MOC_LITERAL(75, 1631, 38), // "HandleRemoveRecvScreenMediaRe..."
QT_MOC_LITERAL(76, 1670, 35), // "HandleScreenShareControlReque..."
QT_MOC_LITERAL(77, 1706, 37), // "HandleRecoveryMediaProcessReq..."
QT_MOC_LITERAL(78, 1744, 15), // "MediaStreamType"
QT_MOC_LITERAL(79, 1760, 7), // "is_send"
QT_MOC_LITERAL(80, 1768, 14), // "remove_user_id"
QT_MOC_LITERAL(81, 1783, 29), // "HandleAllMediaProcessExitSlot"
QT_MOC_LITERAL(82, 1813, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(83, 1849, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(84, 1882, 5), // "state"
QT_MOC_LITERAL(85, 1888, 29), // "HandleOnlineMessageNotifySlot"
QT_MOC_LITERAL(86, 1918, 4), // "from"
QT_MOC_LITERAL(87, 1923, 34), // "HandleQoSLoginPermissionNotif..."
QT_MOC_LITERAL(88, 1958, 19), // "LoginPermissionInfo"
QT_MOC_LITERAL(89, 1978, 33), // "HandleQoSLoginRejectionNotify..."
QT_MOC_LITERAL(90, 2012, 18), // "LoginRejectionInfo"
QT_MOC_LITERAL(91, 2031, 29), // "HandleQoSServerInfoNotifySlot"
QT_MOC_LITERAL(92, 2061, 13), // "QoSServerInfo"
QT_MOC_LITERAL(93, 2075, 38), // "HandleConferenceListReceivedN..."
QT_MOC_LITERAL(94, 2114, 14), // "ConferenceList"
QT_MOC_LITERAL(95, 2129, 11), // "conferences"
QT_MOC_LITERAL(96, 2141, 36), // "HandleTerminalListReceivedNot..."
QT_MOC_LITERAL(97, 2178, 12), // "TerminalList"
QT_MOC_LITERAL(98, 2191, 9), // "terminals"
QT_MOC_LITERAL(99, 2201, 29), // "HandleTerminalLoginNotifySlot"
QT_MOC_LITERAL(100, 2231, 30), // "HandleTerminalLogoutNotifySlot"
QT_MOC_LITERAL(101, 2262, 30), // "HandleTerminalHandUpNotifySlot"
QT_MOC_LITERAL(102, 2293, 32), // "HandleTerminalHandDownNotifySlot"
QT_MOC_LITERAL(103, 2326, 35), // "HandleTerminalPermitSpeakNoti..."
QT_MOC_LITERAL(104, 2362, 35), // "HandleTerminalForbidSpeakNoti..."
QT_MOC_LITERAL(105, 2398, 31), // "HandleSpeakerTerminalNotifySlot"
QT_MOC_LITERAL(106, 2430, 32), // "HandleChairmanTerminalNotifySlot"
QT_MOC_LITERAL(107, 2463, 30), // "HandleChairmanUpdateNotifySlot"
QT_MOC_LITERAL(108, 2494, 35), // "HandleSharedScreenControlNoti..."
QT_MOC_LITERAL(109, 2530, 9), // "sender_ip"
QT_MOC_LITERAL(110, 2540, 36), // "HandleSharedScreenTerminalNot..."
QT_MOC_LITERAL(111, 2577, 31), // "HandleStartMediaRelayNotifySlot"
QT_MOC_LITERAL(112, 2609, 13), // "MediaRelayMsg"
QT_MOC_LITERAL(113, 2623, 31), // "HandleStartMediaReplyNotifySlot"
QT_MOC_LITERAL(114, 2655, 13), // "MediaReplyMsg"
QT_MOC_LITERAL(115, 2669, 30), // "HandleStopMediaRelayNotifySlot"
QT_MOC_LITERAL(116, 2700, 30), // "HandleStopMediaReplyNotifySlot"
QT_MOC_LITERAL(117, 2731, 33), // "HandleRemoteMediaStatusNotify..."
QT_MOC_LITERAL(118, 2765, 14), // "MediaStatusMsg"
QT_MOC_LITERAL(119, 2780, 28), // "HandleMediaControlNotifySlot"
QT_MOC_LITERAL(120, 2809, 15), // "MediaControlMsg"
QT_MOC_LITERAL(121, 2825, 32), // "HandleFloorControlInfoNotifySlot"
QT_MOC_LITERAL(122, 2858, 23), // "SendOnlineHeartBeatSlot"
QT_MOC_LITERAL(123, 2882, 24), // "SendServerOnlineTestSlot"
QT_MOC_LITERAL(124, 2907, 26), // "ServerHeartBeatTimeoutSlot"
QT_MOC_LITERAL(125, 2934, 21), // "CheckTerminalListSlot"
QT_MOC_LITERAL(126, 2956, 34), // "HandleFloorRequestStatusNotif..."
QT_MOC_LITERAL(127, 2991, 20), // "bfcpc::RequestStatus"
QT_MOC_LITERAL(128, 3012, 6) // "status"

    },
    "ConferenceController\0"
    "SendConferenceListRequestMsgSignal\0\0"
    "SendJoinConferenceRequestMsgSignal\0"
    "ConferenceJoinParam\0conference_param\0"
    "SDPInfo\0sdp_info\0SendServerOnlineTestSignal\0"
    "SendHeartBeatToKeepInConferenceMsgSignal\0"
    "conference_uri\0SendLeaveConferenceRequestMsgSignal\0"
    "SendTerminalListRequestMsgSignal\0"
    "SendMainSpeakerInfoRequestSignal\0"
    "SendChairmanInfoRequestSignal\0"
    "SendSharedScreenInfoRequestSignal\0"
    "SendHandUpMsgSignal\0local_uri\0handup\0"
    "SendAllowSpeakMsgSignal\0terminal_uri\0"
    "allow\0SendRequestStartMediaMsgSignal\0"
    "MediaRelayParam\0param\0"
    "SendReplyStartMediaMsgSignal\0"
    "MediaReplyParam\0SendRequestStopMediaMsgSignal\0"
    "SendReplyStopMediaMsgSignal\0"
    "SendScreenControlMsgSignal\0"
    "ScreenControlParam\0SendMediaControlMsgSignal\0"
    "MediaControlParam\0SendTerminalListRequestByCIDSignal\0"
    "cid\0SendFloorControlInfoRequest\0"
    "CreateFloorControlClientSignal\0uint16_t\0"
    "userid\0FloorControlInfoPointer\0info\0"
    "ApplyFloorRequestSignal\0"
    "AcceptFloorRequestSignal\0user_id\0"
    "ReleaseFloorRequestSignal\0"
    "DestoryFloorControlClientSignal\0"
    "HandleConferenceListRequestSlot\0"
    "HandleJoinConferenceRequestSlot\0uri\0"
    "HandleLeaveConferenceRequestSlot\0"
    "HandleTerminalListRequestSlot\0"
    "HandleHandUpRequestSlot\0"
    "HandleReceiveUDPQoSReportSlot\0RtpStatItem\0"
    "rtp_stat\0HandleAddRecvStreamMediaRequestSlot\0"
    "remote_vuri\0screen_index\0seat\0"
    "use_small_video\0enable_audio\0"
    "HandleStartMediaReplyTimeoutSlot\0"
    "HandleRemoveRecvStreamMediaRequestSlot\0"
    "HandleChangeVideoSeatRequestSlot\0"
    "HanldePermitTerminalSpeakRequestSlot\0"
    "HandleCreateSendStreamMediaRequestSlot\0"
    "HandleControlSendMediaRequestSlot\0"
    "MediaControlType\0type\0enable\0virtual_index\0"
    "HandleCreateSendScreenMediaRequestSlot\0"
    "wnd\0HandleCreateRecvScreenMediaRequestSlot\0"
    "remote_uri\0HandleRemoveRecvScreenMediaRequestSlot\0"
    "HandleScreenShareControlRequestSlot\0"
    "HandleRecoveryMediaProcessRequestSlot\0"
    "MediaStreamType\0is_send\0remove_user_id\0"
    "HandleAllMediaProcessExitSlot\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "HandleOnlineMessageNotifySlot\0from\0"
    "HandleQoSLoginPermissionNotifySlot\0"
    "LoginPermissionInfo\0"
    "HandleQoSLoginRejectionNotifySlot\0"
    "LoginRejectionInfo\0HandleQoSServerInfoNotifySlot\0"
    "QoSServerInfo\0HandleConferenceListReceivedNotifySlot\0"
    "ConferenceList\0conferences\0"
    "HandleTerminalListReceivedNotifySlot\0"
    "TerminalList\0terminals\0"
    "HandleTerminalLoginNotifySlot\0"
    "HandleTerminalLogoutNotifySlot\0"
    "HandleTerminalHandUpNotifySlot\0"
    "HandleTerminalHandDownNotifySlot\0"
    "HandleTerminalPermitSpeakNotifySlot\0"
    "HandleTerminalForbidSpeakNotifySlot\0"
    "HandleSpeakerTerminalNotifySlot\0"
    "HandleChairmanTerminalNotifySlot\0"
    "HandleChairmanUpdateNotifySlot\0"
    "HandleSharedScreenControlNotifySlot\0"
    "sender_ip\0HandleSharedScreenTerminalNotifySlot\0"
    "HandleStartMediaRelayNotifySlot\0"
    "MediaRelayMsg\0HandleStartMediaReplyNotifySlot\0"
    "MediaReplyMsg\0HandleStopMediaRelayNotifySlot\0"
    "HandleStopMediaReplyNotifySlot\0"
    "HandleRemoteMediaStatusNotifySlot\0"
    "MediaStatusMsg\0HandleMediaControlNotifySlot\0"
    "MediaControlMsg\0HandleFloorControlInfoNotifySlot\0"
    "SendOnlineHeartBeatSlot\0"
    "SendServerOnlineTestSlot\0"
    "ServerHeartBeatTimeoutSlot\0"
    "CheckTerminalListSlot\0"
    "HandleFloorRequestStatusNotifySlot\0"
    "bfcpc::RequestStatus\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      74,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  384,    2, 0x06 /* Public */,
       3,    2,  385,    2, 0x06 /* Public */,
       8,    0,  390,    2, 0x06 /* Public */,
       9,    1,  391,    2, 0x06 /* Public */,
      11,    1,  394,    2, 0x06 /* Public */,
      12,    1,  397,    2, 0x06 /* Public */,
      13,    1,  400,    2, 0x06 /* Public */,
      14,    1,  403,    2, 0x06 /* Public */,
      15,    1,  406,    2, 0x06 /* Public */,
      16,    3,  409,    2, 0x06 /* Public */,
      19,    3,  416,    2, 0x06 /* Public */,
      22,    1,  423,    2, 0x06 /* Public */,
      25,    1,  426,    2, 0x06 /* Public */,
      27,    1,  429,    2, 0x06 /* Public */,
      28,    1,  432,    2, 0x06 /* Public */,
      29,    1,  435,    2, 0x06 /* Public */,
      31,    1,  438,    2, 0x06 /* Public */,
      33,    1,  441,    2, 0x06 /* Public */,
      35,    1,  444,    2, 0x06 /* Public */,
      36,    2,  447,    2, 0x06 /* Public */,
      41,    0,  452,    2, 0x06 /* Public */,
      42,    1,  453,    2, 0x06 /* Public */,
      44,    1,  456,    2, 0x06 /* Public */,
      45,    0,  459,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      46,    0,  460,    2, 0x0a /* Public */,
      47,    1,  461,    2, 0x0a /* Public */,
      49,    0,  464,    2, 0x0a /* Public */,
      50,    0,  465,    2, 0x0a /* Public */,
      51,    1,  466,    2, 0x0a /* Public */,
      52,    1,  469,    2, 0x0a /* Public */,
      55,    5,  472,    2, 0x0a /* Public */,
      61,    1,  483,    2, 0x0a /* Public */,
      62,    1,  486,    2, 0x0a /* Public */,
      63,    3,  489,    2, 0x0a /* Public */,
      64,    2,  496,    2, 0x0a /* Public */,
      65,    0,  501,    2, 0x0a /* Public */,
      66,    3,  502,    2, 0x0a /* Public */,
      66,    2,  509,    2, 0x2a /* Public | MethodCloned */,
      71,    1,  514,    2, 0x0a /* Public */,
      73,    2,  517,    2, 0x0a /* Public */,
      75,    2,  522,    2, 0x0a /* Public */,
      76,    1,  527,    2, 0x0a /* Public */,
      77,    3,  530,    2, 0x0a /* Public */,
      81,    0,  537,    2, 0x0a /* Public */,
      82,    1,  538,    2, 0x08 /* Private */,
      85,    2,  541,    2, 0x08 /* Private */,
      87,    1,  546,    2, 0x08 /* Private */,
      89,    1,  549,    2, 0x08 /* Private */,
      91,    1,  552,    2, 0x08 /* Private */,
      93,    1,  555,    2, 0x08 /* Private */,
      96,    1,  558,    2, 0x08 /* Private */,
      99,    1,  561,    2, 0x08 /* Private */,
     100,    1,  564,    2, 0x08 /* Private */,
     101,    1,  567,    2, 0x08 /* Private */,
     102,    1,  570,    2, 0x08 /* Private */,
     103,    1,  573,    2, 0x08 /* Private */,
     104,    1,  576,    2, 0x08 /* Private */,
     105,    1,  579,    2, 0x08 /* Private */,
     106,    1,  582,    2, 0x08 /* Private */,
     107,    1,  585,    2, 0x08 /* Private */,
     108,    2,  588,    2, 0x08 /* Private */,
     110,    1,  593,    2, 0x08 /* Private */,
     111,    1,  596,    2, 0x08 /* Private */,
     113,    1,  599,    2, 0x08 /* Private */,
     115,    1,  602,    2, 0x08 /* Private */,
     116,    1,  605,    2, 0x08 /* Private */,
     117,    1,  608,    2, 0x08 /* Private */,
     119,    1,  611,    2, 0x08 /* Private */,
     121,    1,  614,    2, 0x08 /* Private */,
     122,    0,  617,    2, 0x08 /* Private */,
     123,    0,  618,    2, 0x08 /* Private */,
     124,    0,  619,    2, 0x08 /* Private */,
     125,    0,  620,    2, 0x08 /* Private */,
     126,    2,  621,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   17,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   10,   20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, 0x80000000 | 26,   24,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, 0x80000000 | 26,   24,
    QMetaType::Void, 0x80000000 | 30,   24,
    QMetaType::Void, 0x80000000 | 32,   24,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, 0x80000000 | 37, 0x80000000 | 39,   38,   40,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 37,   43,
    QMetaType::Void, 0x80000000 | 37,   43,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   48,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, 0x80000000 | 53,   54,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,   56,   57,   58,   59,   60,
    QMetaType::Void, QMetaType::QString,   56,
    QMetaType::Void, QMetaType::QString,   56,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   56,   57,   58,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   48,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 67, QMetaType::Bool, QMetaType::Int,   68,   69,   70,
    QMetaType::Void, 0x80000000 | 67, QMetaType::Bool,   68,   69,
    QMetaType::Void, QMetaType::QRect,   72,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   74,   57,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   74,   57,
    QMetaType::Void, QMetaType::Bool,   69,
    QMetaType::Void, 0x80000000 | 78, QMetaType::Bool, QMetaType::QString,   68,   79,   80,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 83,   84,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   68,   86,
    QMetaType::Void, 0x80000000 | 88,   40,
    QMetaType::Void, 0x80000000 | 90,   40,
    QMetaType::Void, 0x80000000 | 92,   40,
    QMetaType::Void, 0x80000000 | 94,   95,
    QMetaType::Void, 0x80000000 | 97,   98,
    QMetaType::Void, 0x80000000 | 97,   98,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,  109,   69,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, 0x80000000 | 112,   40,
    QMetaType::Void, 0x80000000 | 114,   40,
    QMetaType::Void, 0x80000000 | 112,   40,
    QMetaType::Void, 0x80000000 | 114,   40,
    QMetaType::Void, 0x80000000 | 118,   40,
    QMetaType::Void, 0x80000000 | 120,   40,
    QMetaType::Void, 0x80000000 | 39,   40,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 37, 0x80000000 | 127,   43,  128,

       0        // eod
};

void ConferenceController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConferenceController *_t = static_cast<ConferenceController *>(_o);
        switch (_id) {
        case 0: _t->SendConferenceListRequestMsgSignal(); break;
        case 1: _t->SendJoinConferenceRequestMsgSignal((*reinterpret_cast< const ConferenceJoinParam(*)>(_a[1])),(*reinterpret_cast< const SDPInfo(*)>(_a[2]))); break;
        case 2: _t->SendServerOnlineTestSignal(); break;
        case 3: _t->SendHeartBeatToKeepInConferenceMsgSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->SendLeaveConferenceRequestMsgSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->SendTerminalListRequestMsgSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->SendMainSpeakerInfoRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->SendChairmanInfoRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->SendSharedScreenInfoRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->SendHandUpMsgSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->SendAllowSpeakMsgSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 11: _t->SendRequestStartMediaMsgSignal((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 12: _t->SendReplyStartMediaMsgSignal((*reinterpret_cast< const MediaReplyParam(*)>(_a[1]))); break;
        case 13: _t->SendRequestStopMediaMsgSignal((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        case 14: _t->SendReplyStopMediaMsgSignal((*reinterpret_cast< const MediaReplyParam(*)>(_a[1]))); break;
        case 15: _t->SendScreenControlMsgSignal((*reinterpret_cast< const ScreenControlParam(*)>(_a[1]))); break;
        case 16: _t->SendMediaControlMsgSignal((*reinterpret_cast< const MediaControlParam(*)>(_a[1]))); break;
        case 17: _t->SendTerminalListRequestByCIDSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->SendFloorControlInfoRequest((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->CreateFloorControlClientSignal((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< const FloorControlInfoPointer(*)>(_a[2]))); break;
        case 20: _t->ApplyFloorRequestSignal(); break;
        case 21: _t->AcceptFloorRequestSignal((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 22: _t->ReleaseFloorRequestSignal((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 23: _t->DestoryFloorControlClientSignal(); break;
        case 24: _t->HandleConferenceListRequestSlot(); break;
        case 25: _t->HandleJoinConferenceRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->HandleLeaveConferenceRequestSlot(); break;
        case 27: _t->HandleTerminalListRequestSlot(); break;
        case 28: _t->HandleHandUpRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->HandleReceiveUDPQoSReportSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 30: _t->HandleAddRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 31: _t->HandleStartMediaReplyTimeoutSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->HandleRemoveRecvStreamMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->HandleChangeVideoSeatRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 34: _t->HanldePermitTerminalSpeakRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 35: _t->HandleCreateSendStreamMediaRequestSlot(); break;
        case 36: _t->HandleControlSendMediaRequestSlot((*reinterpret_cast< MediaControlType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 37: _t->HandleControlSendMediaRequestSlot((*reinterpret_cast< MediaControlType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 38: _t->HandleCreateSendScreenMediaRequestSlot((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 39: _t->HandleCreateRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 40: _t->HandleRemoveRecvScreenMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 41: _t->HandleScreenShareControlRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 42: _t->HandleRecoveryMediaProcessRequestSlot((*reinterpret_cast< MediaStreamType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 43: _t->HandleAllMediaProcessExitSlot(); break;
        case 44: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 45: _t->HandleOnlineMessageNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 46: _t->HandleQoSLoginPermissionNotifySlot((*reinterpret_cast< const LoginPermissionInfo(*)>(_a[1]))); break;
        case 47: _t->HandleQoSLoginRejectionNotifySlot((*reinterpret_cast< const LoginRejectionInfo(*)>(_a[1]))); break;
        case 48: _t->HandleQoSServerInfoNotifySlot((*reinterpret_cast< const QoSServerInfo(*)>(_a[1]))); break;
        case 49: _t->HandleConferenceListReceivedNotifySlot((*reinterpret_cast< const ConferenceList(*)>(_a[1]))); break;
        case 50: _t->HandleTerminalListReceivedNotifySlot((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 51: _t->HandleTerminalLoginNotifySlot((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 52: _t->HandleTerminalLogoutNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 53: _t->HandleTerminalHandUpNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 54: _t->HandleTerminalHandDownNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 55: _t->HandleTerminalPermitSpeakNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 56: _t->HandleTerminalForbidSpeakNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 57: _t->HandleSpeakerTerminalNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 58: _t->HandleChairmanTerminalNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 59: _t->HandleChairmanUpdateNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 60: _t->HandleSharedScreenControlNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 61: _t->HandleSharedScreenTerminalNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 62: _t->HandleStartMediaRelayNotifySlot((*reinterpret_cast< const MediaRelayMsg(*)>(_a[1]))); break;
        case 63: _t->HandleStartMediaReplyNotifySlot((*reinterpret_cast< const MediaReplyMsg(*)>(_a[1]))); break;
        case 64: _t->HandleStopMediaRelayNotifySlot((*reinterpret_cast< const MediaRelayMsg(*)>(_a[1]))); break;
        case 65: _t->HandleStopMediaReplyNotifySlot((*reinterpret_cast< const MediaReplyMsg(*)>(_a[1]))); break;
        case 66: _t->HandleRemoteMediaStatusNotifySlot((*reinterpret_cast< const MediaStatusMsg(*)>(_a[1]))); break;
        case 67: _t->HandleMediaControlNotifySlot((*reinterpret_cast< const MediaControlMsg(*)>(_a[1]))); break;
        case 68: _t->HandleFloorControlInfoNotifySlot((*reinterpret_cast< const FloorControlInfoPointer(*)>(_a[1]))); break;
        case 69: _t->SendOnlineHeartBeatSlot(); break;
        case 70: _t->SendServerOnlineTestSlot(); break;
        case 71: _t->ServerHeartBeatTimeoutSlot(); break;
        case 72: _t->CheckTerminalListSlot(); break;
        case 73: _t->HandleFloorRequestStatusNotifySlot((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< bfcpc::RequestStatus(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConferenceController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendConferenceListRequestMsgSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const ConferenceJoinParam & , const SDPInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendJoinConferenceRequestMsgSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ConferenceController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendServerOnlineTestSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendHeartBeatToKeepInConferenceMsgSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendLeaveConferenceRequestMsgSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendTerminalListRequestMsgSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendMainSpeakerInfoRequestSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendChairmanInfoRequestSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendSharedScreenInfoRequestSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & , const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendHandUpMsgSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & , const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendAllowSpeakMsgSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const MediaRelayParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendRequestStartMediaMsgSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const MediaReplyParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendReplyStartMediaMsgSignal)) {
                *result = 12;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const MediaRelayParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendRequestStopMediaMsgSignal)) {
                *result = 13;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const MediaReplyParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendReplyStopMediaMsgSignal)) {
                *result = 14;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const ScreenControlParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendScreenControlMsgSignal)) {
                *result = 15;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const MediaControlParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendMediaControlMsgSignal)) {
                *result = 16;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendTerminalListRequestByCIDSignal)) {
                *result = 17;
            }
        }
        {
            typedef void (ConferenceController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::SendFloorControlInfoRequest)) {
                *result = 18;
            }
        }
        {
            typedef void (ConferenceController::*_t)(uint16_t , const FloorControlInfoPointer & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::CreateFloorControlClientSignal)) {
                *result = 19;
            }
        }
        {
            typedef void (ConferenceController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::ApplyFloorRequestSignal)) {
                *result = 20;
            }
        }
        {
            typedef void (ConferenceController::*_t)(uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::AcceptFloorRequestSignal)) {
                *result = 21;
            }
        }
        {
            typedef void (ConferenceController::*_t)(uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::ReleaseFloorRequestSignal)) {
                *result = 22;
            }
        }
        {
            typedef void (ConferenceController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceController::DestoryFloorControlClientSignal)) {
                *result = 23;
            }
        }
    }
}

const QMetaObject ConferenceController::staticMetaObject = {
    { &IConferenceController::staticMetaObject, qt_meta_stringdata_ConferenceController.data,
      qt_meta_data_ConferenceController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConferenceController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceController.stringdata))
        return static_cast<void*>(const_cast< ConferenceController*>(this));
    return IConferenceController::qt_metacast(_clname);
}

int ConferenceController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IConferenceController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 74)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 74;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 74)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 74;
    }
    return _id;
}

// SIGNAL 0
void ConferenceController::SendConferenceListRequestMsgSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ConferenceController::SendJoinConferenceRequestMsgSignal(const ConferenceJoinParam & _t1, const SDPInfo & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConferenceController::SendServerOnlineTestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ConferenceController::SendHeartBeatToKeepInConferenceMsgSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ConferenceController::SendLeaveConferenceRequestMsgSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ConferenceController::SendTerminalListRequestMsgSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ConferenceController::SendMainSpeakerInfoRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ConferenceController::SendChairmanInfoRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ConferenceController::SendSharedScreenInfoRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ConferenceController::SendHandUpMsgSignal(const QString & _t1, const QString & _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ConferenceController::SendAllowSpeakMsgSignal(const QString & _t1, const QString & _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ConferenceController::SendRequestStartMediaMsgSignal(const MediaRelayParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ConferenceController::SendReplyStartMediaMsgSignal(const MediaReplyParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ConferenceController::SendRequestStopMediaMsgSignal(const MediaRelayParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void ConferenceController::SendReplyStopMediaMsgSignal(const MediaReplyParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void ConferenceController::SendScreenControlMsgSignal(const ScreenControlParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void ConferenceController::SendMediaControlMsgSignal(const MediaControlParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void ConferenceController::SendTerminalListRequestByCIDSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void ConferenceController::SendFloorControlInfoRequest(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void ConferenceController::CreateFloorControlClientSignal(uint16_t _t1, const FloorControlInfoPointer & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void ConferenceController::ApplyFloorRequestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 20, Q_NULLPTR);
}

// SIGNAL 21
void ConferenceController::AcceptFloorRequestSignal(uint16_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void ConferenceController::ReleaseFloorRequestSignal(uint16_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void ConferenceController::DestoryFloorControlClientSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 23, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
