/****************************************************************************
** Meta object code from reading C++ file 'ConferenceManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../util/ConferenceManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConferenceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConferenceManager_t {
    QByteArrayData data[119];
    char stringdata[2332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceManager_t qt_meta_stringdata_ConferenceManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ConferenceManager"
QT_MOC_LITERAL(1, 18, 24), // "cvConfListReceivedSignal"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 16), // "ConferenceVector"
QT_MOC_LITERAL(4, 61, 10), // "confVector"
QT_MOC_LITERAL(5, 72, 26), // "cvMemberListReceivedSignal"
QT_MOC_LITERAL(6, 99, 12), // "MemberVector"
QT_MOC_LITERAL(7, 112, 10), // "memberList"
QT_MOC_LITERAL(8, 123, 11), // "std::string"
QT_MOC_LITERAL(9, 135, 9), // "confTitle"
QT_MOC_LITERAL(10, 145, 28), // "cvTerminalListReceivedSignal"
QT_MOC_LITERAL(11, 174, 14), // "TerminalVector"
QT_MOC_LITERAL(12, 189, 12), // "terminalList"
QT_MOC_LITERAL(13, 202, 7), // "confURI"
QT_MOC_LITERAL(14, 210, 20), // "cvMemberHandUpSignal"
QT_MOC_LITERAL(15, 231, 9), // "memberURI"
QT_MOC_LITERAL(16, 241, 22), // "cvMemberHandDownSignal"
QT_MOC_LITERAL(17, 264, 27), // "cvTermianlCountChangeNotify"
QT_MOC_LITERAL(18, 292, 16), // "TotalTermianlNum"
QT_MOC_LITERAL(19, 309, 29), // "cvJoinConferenceSuccessSignal"
QT_MOC_LITERAL(20, 339, 19), // "cvMemberLoginSignal"
QT_MOC_LITERAL(21, 359, 10), // "MemberInfo"
QT_MOC_LITERAL(22, 370, 6), // "member"
QT_MOC_LITERAL(23, 377, 21), // "cvTerminalLoginSignal"
QT_MOC_LITERAL(24, 399, 21), // "cvJoinRejectionSignal"
QT_MOC_LITERAL(25, 421, 6), // "reason"
QT_MOC_LITERAL(26, 428, 22), // "cvTerminalLogoutSignal"
QT_MOC_LITERAL(27, 451, 3), // "sip"
QT_MOC_LITERAL(28, 455, 21), // "cvEndConferenceSignal"
QT_MOC_LITERAL(29, 477, 23), // "cvConfrenceInviteSignal"
QT_MOC_LITERAL(30, 501, 15), // "const ConfInfo*"
QT_MOC_LITERAL(31, 517, 10), // "conference"
QT_MOC_LITERAL(32, 528, 4), // "from"
QT_MOC_LITERAL(33, 533, 25), // "cvPasswordIncorrectSignal"
QT_MOC_LITERAL(34, 559, 14), // "cvKickedSignal"
QT_MOC_LITERAL(35, 574, 23), // "cvTextMsgReceivedSignal"
QT_MOC_LITERAL(36, 598, 7), // "message"
QT_MOC_LITERAL(37, 606, 29), // "cvWhiteBoardMsgReceivedSignal"
QT_MOC_LITERAL(38, 636, 19), // "cvPermitSpeakSignal"
QT_MOC_LITERAL(39, 656, 25), // "cvHostUnShareScreenSignal"
QT_MOC_LITERAL(40, 682, 25), // "cvRTSendMediaStateChanged"
QT_MOC_LITERAL(41, 708, 4), // "type"
QT_MOC_LITERAL(42, 713, 6), // "enable"
QT_MOC_LITERAL(43, 720, 33), // "cvRemoteTerminalAskForMediaSi..."
QT_MOC_LITERAL(44, 754, 11), // "terminalURI"
QT_MOC_LITERAL(45, 766, 12), // "terminalName"
QT_MOC_LITERAL(46, 779, 14), // "RecvSaviSignal"
QT_MOC_LITERAL(47, 794, 3), // "res"
QT_MOC_LITERAL(48, 798, 38), // "cvSetMemberWindowToFirstPosit..."
QT_MOC_LITERAL(49, 837, 27), // "cvUpgradeTerminalInfoSignal"
QT_MOC_LITERAL(50, 865, 12), // "TerminalInfo"
QT_MOC_LITERAL(51, 878, 8), // "terminal"
QT_MOC_LITERAL(52, 887, 24), // "cvModifyStopMyAudioState"
QT_MOC_LITERAL(53, 912, 24), // "cvServerTestResultSignal"
QT_MOC_LITERAL(54, 937, 9), // "isTimeOut"
QT_MOC_LITERAL(55, 947, 8), // "interval"
QT_MOC_LITERAL(56, 956, 20), // "receiveRtpStatSignal"
QT_MOC_LITERAL(57, 977, 11), // "RtpStatItem"
QT_MOC_LITERAL(58, 989, 7), // "rtpstat"
QT_MOC_LITERAL(59, 997, 20), // "receiveRecStatSignal"
QT_MOC_LITERAL(60, 1018, 11), // "RecStatItem"
QT_MOC_LITERAL(61, 1030, 7), // "recstat"
QT_MOC_LITERAL(62, 1038, 31), // "streamMediaReceiveStartedSignal"
QT_MOC_LITERAL(63, 1070, 3), // "uri"
QT_MOC_LITERAL(64, 1074, 31), // "streamMediaReceiveStoppedSignal"
QT_MOC_LITERAL(65, 1106, 19), // "StartServerTsetSlot"
QT_MOC_LITERAL(66, 1126, 17), // "SegmentLocalMedia"
QT_MOC_LITERAL(67, 1144, 21), // "cvMessageReceivedSlot"
QT_MOC_LITERAL(68, 1166, 30), // "recvTerminalConfModeMsgHandler"
QT_MOC_LITERAL(69, 1197, 12), // "CVMsgParser&"
QT_MOC_LITERAL(70, 1210, 15), // "cvMessageParser"
QT_MOC_LITERAL(71, 1226, 7), // "fromUri"
QT_MOC_LITERAL(72, 1234, 22), // "recvConfListMsgHandler"
QT_MOC_LITERAL(73, 1257, 32), // "recvLoginResultCommandMsgHandler"
QT_MOC_LITERAL(74, 1290, 31), // "recvMemberListCommandMsgHandler"
QT_MOC_LITERAL(75, 1322, 25), // "recvLoginNotifyMsgHandler"
QT_MOC_LITERAL(76, 1348, 26), // "recvLogoutNotifyMsgHandler"
QT_MOC_LITERAL(77, 1375, 20), // "recvInviteMsgHandler"
QT_MOC_LITERAL(78, 1396, 29), // "recvApplyConfResultMsgHandler"
QT_MOC_LITERAL(79, 1426, 31), // "recvPasswordIncorrectMsgHandler"
QT_MOC_LITERAL(80, 1458, 18), // "recvTextMsgHandler"
QT_MOC_LITERAL(81, 1477, 20), // "recvKickedMsgHandler"
QT_MOC_LITERAL(82, 1498, 24), // "recvWhiteBoardMsgHandler"
QT_MOC_LITERAL(83, 1523, 20), // "recvHandUpMsgHandler"
QT_MOC_LITERAL(84, 1544, 22), // "recvHandDownMsgHandler"
QT_MOC_LITERAL(85, 1567, 28), // "recvScreenReceiverMsgHandler"
QT_MOC_LITERAL(86, 1596, 27), // "recvUnShareScreenMsgHandler"
QT_MOC_LITERAL(87, 1624, 35), // "recvScreenCancel2ReceiverMsgH..."
QT_MOC_LITERAL(88, 1660, 17), // "recvQoSMsgHandler"
QT_MOC_LITERAL(89, 1678, 33), // "recvTerminalListCommandMsgHan..."
QT_MOC_LITERAL(90, 1712, 20), // "RecvTerminalListSlot"
QT_MOC_LITERAL(91, 1733, 19), // "recvStartMediaReply"
QT_MOC_LITERAL(92, 1753, 25), // "recvStartSendMediaCommand"
QT_MOC_LITERAL(93, 1779, 18), // "recvStopMediaReply"
QT_MOC_LITERAL(94, 1798, 24), // "recvStopSendMediaCommand"
QT_MOC_LITERAL(95, 1823, 27), // "recvSendMediaControlCommand"
QT_MOC_LITERAL(96, 1851, 16), // "queryMainSpeaker"
QT_MOC_LITERAL(97, 1868, 21), // "queryChairmanTermianl"
QT_MOC_LITERAL(98, 1890, 31), // "queryCurrentScreenShareTermianl"
QT_MOC_LITERAL(99, 1922, 19), // "recvPermitSpeakInfo"
QT_MOC_LITERAL(100, 1942, 19), // "recvForbidSpeakInfo"
QT_MOC_LITERAL(101, 1962, 16), // "forbidSpeakByURI"
QT_MOC_LITERAL(102, 1979, 3), // "Uri"
QT_MOC_LITERAL(103, 1983, 12), // "recvSaviInfo"
QT_MOC_LITERAL(104, 1996, 27), // "recvChairmanTerminalHandler"
QT_MOC_LITERAL(105, 2024, 20), // "recvMediaControlInfo"
QT_MOC_LITERAL(106, 2045, 27), // "recvMainSpeakerTermianlInfo"
QT_MOC_LITERAL(107, 2073, 24), // "recvChairmanTermianlInfo"
QT_MOC_LITERAL(108, 2098, 34), // "recvCurrentScreenShareTermina..."
QT_MOC_LITERAL(109, 2133, 26), // "recvShareScreenControlInfo"
QT_MOC_LITERAL(110, 2160, 26), // "recvMainSpeakerShareScreen"
QT_MOC_LITERAL(111, 2187, 8), // "QString&"
QT_MOC_LITERAL(112, 2196, 21), // "recvOnlineMessageInfo"
QT_MOC_LITERAL(113, 2218, 17), // "_SipAccountLogout"
QT_MOC_LITERAL(114, 2236, 6), // "sipURI"
QT_MOC_LITERAL(115, 2243, 14), // "onlineInfoSlot"
QT_MOC_LITERAL(116, 2258, 18), // "TerminalLogOutSlot"
QT_MOC_LITERAL(117, 2277, 20), // "severTestTimeOutSlot"
QT_MOC_LITERAL(118, 2298, 33) // "serverHeartbeatMessageTimeOut..."

    },
    "ConferenceManager\0cvConfListReceivedSignal\0"
    "\0ConferenceVector\0confVector\0"
    "cvMemberListReceivedSignal\0MemberVector\0"
    "memberList\0std::string\0confTitle\0"
    "cvTerminalListReceivedSignal\0"
    "TerminalVector\0terminalList\0confURI\0"
    "cvMemberHandUpSignal\0memberURI\0"
    "cvMemberHandDownSignal\0"
    "cvTermianlCountChangeNotify\0"
    "TotalTermianlNum\0cvJoinConferenceSuccessSignal\0"
    "cvMemberLoginSignal\0MemberInfo\0member\0"
    "cvTerminalLoginSignal\0cvJoinRejectionSignal\0"
    "reason\0cvTerminalLogoutSignal\0sip\0"
    "cvEndConferenceSignal\0cvConfrenceInviteSignal\0"
    "const ConfInfo*\0conference\0from\0"
    "cvPasswordIncorrectSignal\0cvKickedSignal\0"
    "cvTextMsgReceivedSignal\0message\0"
    "cvWhiteBoardMsgReceivedSignal\0"
    "cvPermitSpeakSignal\0cvHostUnShareScreenSignal\0"
    "cvRTSendMediaStateChanged\0type\0enable\0"
    "cvRemoteTerminalAskForMediaSignal\0"
    "terminalURI\0terminalName\0RecvSaviSignal\0"
    "res\0cvSetMemberWindowToFirstPositionSignal\0"
    "cvUpgradeTerminalInfoSignal\0TerminalInfo\0"
    "terminal\0cvModifyStopMyAudioState\0"
    "cvServerTestResultSignal\0isTimeOut\0"
    "interval\0receiveRtpStatSignal\0RtpStatItem\0"
    "rtpstat\0receiveRecStatSignal\0RecStatItem\0"
    "recstat\0streamMediaReceiveStartedSignal\0"
    "uri\0streamMediaReceiveStoppedSignal\0"
    "StartServerTsetSlot\0SegmentLocalMedia\0"
    "cvMessageReceivedSlot\0"
    "recvTerminalConfModeMsgHandler\0"
    "CVMsgParser&\0cvMessageParser\0fromUri\0"
    "recvConfListMsgHandler\0"
    "recvLoginResultCommandMsgHandler\0"
    "recvMemberListCommandMsgHandler\0"
    "recvLoginNotifyMsgHandler\0"
    "recvLogoutNotifyMsgHandler\0"
    "recvInviteMsgHandler\0recvApplyConfResultMsgHandler\0"
    "recvPasswordIncorrectMsgHandler\0"
    "recvTextMsgHandler\0recvKickedMsgHandler\0"
    "recvWhiteBoardMsgHandler\0recvHandUpMsgHandler\0"
    "recvHandDownMsgHandler\0"
    "recvScreenReceiverMsgHandler\0"
    "recvUnShareScreenMsgHandler\0"
    "recvScreenCancel2ReceiverMsgHandler\0"
    "recvQoSMsgHandler\0recvTerminalListCommandMsgHandler\0"
    "RecvTerminalListSlot\0recvStartMediaReply\0"
    "recvStartSendMediaCommand\0recvStopMediaReply\0"
    "recvStopSendMediaCommand\0"
    "recvSendMediaControlCommand\0"
    "queryMainSpeaker\0queryChairmanTermianl\0"
    "queryCurrentScreenShareTermianl\0"
    "recvPermitSpeakInfo\0recvForbidSpeakInfo\0"
    "forbidSpeakByURI\0Uri\0recvSaviInfo\0"
    "recvChairmanTerminalHandler\0"
    "recvMediaControlInfo\0recvMainSpeakerTermianlInfo\0"
    "recvChairmanTermianlInfo\0"
    "recvCurrentScreenShareTerminalInfo\0"
    "recvShareScreenControlInfo\0"
    "recvMainSpeakerShareScreen\0QString&\0"
    "recvOnlineMessageInfo\0_SipAccountLogout\0"
    "sipURI\0onlineInfoSlot\0TerminalLogOutSlot\0"
    "severTestTimeOutSlot\0"
    "serverHeartbeatMessageTimeOutSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      78,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      30,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  404,    2, 0x06 /* Public */,
       5,    2,  407,    2, 0x06 /* Public */,
      10,    2,  412,    2, 0x06 /* Public */,
      14,    1,  417,    2, 0x06 /* Public */,
      16,    1,  420,    2, 0x06 /* Public */,
      17,    1,  423,    2, 0x06 /* Public */,
      19,    1,  426,    2, 0x06 /* Public */,
      20,    1,  429,    2, 0x06 /* Public */,
      23,    1,  432,    2, 0x06 /* Public */,
      24,    2,  435,    2, 0x06 /* Public */,
      26,    1,  440,    2, 0x06 /* Public */,
      28,    0,  443,    2, 0x06 /* Public */,
      29,    2,  444,    2, 0x06 /* Public */,
      33,    0,  449,    2, 0x06 /* Public */,
      34,    0,  450,    2, 0x06 /* Public */,
      35,    2,  451,    2, 0x06 /* Public */,
      37,    2,  456,    2, 0x06 /* Public */,
      38,    0,  461,    2, 0x06 /* Public */,
      39,    0,  462,    2, 0x06 /* Public */,
      40,    3,  463,    2, 0x06 /* Public */,
      43,    2,  470,    2, 0x06 /* Public */,
      46,    1,  475,    2, 0x06 /* Public */,
      48,    1,  478,    2, 0x06 /* Public */,
      49,    1,  481,    2, 0x06 /* Public */,
      52,    1,  484,    2, 0x06 /* Public */,
      53,    2,  487,    2, 0x06 /* Public */,
      56,    1,  492,    2, 0x06 /* Public */,
      59,    1,  495,    2, 0x06 /* Public */,
      62,    1,  498,    2, 0x06 /* Public */,
      64,    1,  501,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      65,    0,  504,    2, 0x0a /* Public */,
      66,    0,  505,    2, 0x0a /* Public */,
      67,    2,  506,    2, 0x0a /* Public */,
      68,    2,  511,    2, 0x0a /* Public */,
      72,    2,  516,    2, 0x0a /* Public */,
      73,    2,  521,    2, 0x0a /* Public */,
      74,    2,  526,    2, 0x0a /* Public */,
      75,    2,  531,    2, 0x0a /* Public */,
      76,    2,  536,    2, 0x0a /* Public */,
      77,    2,  541,    2, 0x0a /* Public */,
      78,    2,  546,    2, 0x0a /* Public */,
      79,    2,  551,    2, 0x0a /* Public */,
      80,    2,  556,    2, 0x0a /* Public */,
      81,    2,  561,    2, 0x0a /* Public */,
      82,    2,  566,    2, 0x0a /* Public */,
      83,    2,  571,    2, 0x0a /* Public */,
      84,    2,  576,    2, 0x0a /* Public */,
      85,    2,  581,    2, 0x0a /* Public */,
      86,    2,  586,    2, 0x0a /* Public */,
      87,    2,  591,    2, 0x0a /* Public */,
      88,    2,  596,    2, 0x0a /* Public */,
      89,    2,  601,    2, 0x0a /* Public */,
      90,    1,  606,    2, 0x0a /* Public */,
      91,    2,  609,    2, 0x0a /* Public */,
      92,    2,  614,    2, 0x0a /* Public */,
      93,    2,  619,    2, 0x0a /* Public */,
      94,    2,  624,    2, 0x0a /* Public */,
      95,    2,  629,    2, 0x0a /* Public */,
      96,    0,  634,    2, 0x0a /* Public */,
      97,    0,  635,    2, 0x0a /* Public */,
      98,    0,  636,    2, 0x0a /* Public */,
      99,    2,  637,    2, 0x0a /* Public */,
     100,    2,  642,    2, 0x0a /* Public */,
     101,    1,  647,    2, 0x0a /* Public */,
     103,    2,  650,    2, 0x0a /* Public */,
     104,    2,  655,    2, 0x0a /* Public */,
     105,    2,  660,    2, 0x0a /* Public */,
     106,    2,  665,    2, 0x0a /* Public */,
     107,    2,  670,    2, 0x0a /* Public */,
     108,    2,  675,    2, 0x0a /* Public */,
     109,    2,  680,    2, 0x0a /* Public */,
     110,    1,  685,    2, 0x0a /* Public */,
     112,    2,  688,    2, 0x0a /* Public */,
     113,    1,  693,    2, 0x0a /* Public */,
     115,    0,  696,    2, 0x0a /* Public */,
     116,    0,  697,    2, 0x0a /* Public */,
     117,    0,  698,    2, 0x0a /* Public */,
     118,    0,  699,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 8,   12,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, 0x80000000 | 8,   13,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 11,   22,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8,   13,   25,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30, 0x80000000 | 8,   31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8,   36,   32,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8,   36,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8, QMetaType::Bool,   15,   41,   42,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8,   44,   45,
    QMetaType::Void, QMetaType::Bool,   47,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 50,   51,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   54,   55,
    QMetaType::Void, 0x80000000 | 57,   58,
    QMetaType::Void, 0x80000000 | 60,   61,
    QMetaType::Void, QMetaType::QString,   63,
    QMetaType::Void, QMetaType::QString,   63,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   36,   32,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 8,  102,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 111,  102,
    QMetaType::Void, 0x80000000 | 69, 0x80000000 | 8,   70,   71,
    QMetaType::Void, 0x80000000 | 8,  114,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConferenceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConferenceManager *_t = static_cast<ConferenceManager *>(_o);
        switch (_id) {
        case 0: _t->cvConfListReceivedSignal((*reinterpret_cast< const ConferenceVector(*)>(_a[1]))); break;
        case 1: _t->cvMemberListReceivedSignal((*reinterpret_cast< const MemberVector(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 2: _t->cvTerminalListReceivedSignal((*reinterpret_cast< const TerminalVector(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 3: _t->cvMemberHandUpSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->cvMemberHandDownSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->cvTermianlCountChangeNotify((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->cvJoinConferenceSuccessSignal((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 7: _t->cvMemberLoginSignal((*reinterpret_cast< const MemberInfo(*)>(_a[1]))); break;
        case 8: _t->cvTerminalLoginSignal((*reinterpret_cast< const TerminalVector(*)>(_a[1]))); break;
        case 9: _t->cvJoinRejectionSignal((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 10: _t->cvTerminalLogoutSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->cvEndConferenceSignal(); break;
        case 12: _t->cvConfrenceInviteSignal((*reinterpret_cast< const ConfInfo*(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 13: _t->cvPasswordIncorrectSignal(); break;
        case 14: _t->cvKickedSignal(); break;
        case 15: _t->cvTextMsgReceivedSignal((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 16: _t->cvWhiteBoardMsgReceivedSignal((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 17: _t->cvPermitSpeakSignal(); break;
        case 18: _t->cvHostUnShareScreenSignal(); break;
        case 19: _t->cvRTSendMediaStateChanged((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 20: _t->cvRemoteTerminalAskForMediaSignal((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 21: _t->RecvSaviSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->cvSetMemberWindowToFirstPositionSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->cvUpgradeTerminalInfoSignal((*reinterpret_cast< const TerminalInfo(*)>(_a[1]))); break;
        case 24: _t->cvModifyStopMyAudioState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->cvServerTestResultSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 26: _t->receiveRtpStatSignal((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 27: _t->receiveRecStatSignal((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 28: _t->streamMediaReceiveStartedSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->streamMediaReceiveStoppedSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->StartServerTsetSlot(); break;
        case 31: _t->SegmentLocalMedia(); break;
        case 32: _t->cvMessageReceivedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 33: _t->recvTerminalConfModeMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 34: _t->recvConfListMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 35: _t->recvLoginResultCommandMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 36: _t->recvMemberListCommandMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 37: _t->recvLoginNotifyMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 38: _t->recvLogoutNotifyMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 39: _t->recvInviteMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 40: _t->recvApplyConfResultMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 41: _t->recvPasswordIncorrectMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 42: _t->recvTextMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 43: _t->recvKickedMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 44: _t->recvWhiteBoardMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 45: _t->recvHandUpMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 46: _t->recvHandDownMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 47: _t->recvScreenReceiverMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 48: _t->recvUnShareScreenMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 49: _t->recvScreenCancel2ReceiverMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 50: _t->recvQoSMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 51: _t->recvTerminalListCommandMsgHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 52: _t->RecvTerminalListSlot((*reinterpret_cast< TerminalVector(*)>(_a[1]))); break;
        case 53: _t->recvStartMediaReply((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 54: _t->recvStartSendMediaCommand((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 55: _t->recvStopMediaReply((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 56: _t->recvStopSendMediaCommand((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 57: _t->recvSendMediaControlCommand((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 58: _t->queryMainSpeaker(); break;
        case 59: _t->queryChairmanTermianl(); break;
        case 60: _t->queryCurrentScreenShareTermianl(); break;
        case 61: _t->recvPermitSpeakInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 62: _t->recvForbidSpeakInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 63: _t->forbidSpeakByURI((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 64: _t->recvSaviInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 65: _t->recvChairmanTerminalHandler((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 66: _t->recvMediaControlInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 67: _t->recvMainSpeakerTermianlInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 68: _t->recvChairmanTermianlInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 69: _t->recvCurrentScreenShareTerminalInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 70: _t->recvShareScreenControlInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 71: _t->recvMainSpeakerShareScreen((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 72: _t->recvOnlineMessageInfo((*reinterpret_cast< CVMsgParser(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 73: _t->_SipAccountLogout((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 74: _t->onlineInfoSlot(); break;
        case 75: _t->TerminalLogOutSlot(); break;
        case 76: _t->severTestTimeOutSlot(); break;
        case 77: _t->serverHeartbeatMessageTimeOutSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConferenceManager::*_t)(const ConferenceVector & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvConfListReceivedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const MemberVector & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvMemberListReceivedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const TerminalVector & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvTerminalListReceivedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvMemberHandUpSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvMemberHandDownSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvTermianlCountChangeNotify)) {
                *result = 5;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvJoinConferenceSuccessSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const MemberInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvMemberLoginSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const TerminalVector & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvTerminalLoginSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvJoinRejectionSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvTerminalLogoutSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (ConferenceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvEndConferenceSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const ConfInfo * , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvConfrenceInviteSignal)) {
                *result = 12;
            }
        }
        {
            typedef void (ConferenceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvPasswordIncorrectSignal)) {
                *result = 13;
            }
        }
        {
            typedef void (ConferenceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvKickedSignal)) {
                *result = 14;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvTextMsgReceivedSignal)) {
                *result = 15;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvWhiteBoardMsgReceivedSignal)) {
                *result = 16;
            }
        }
        {
            typedef void (ConferenceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvPermitSpeakSignal)) {
                *result = 17;
            }
        }
        {
            typedef void (ConferenceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvHostUnShareScreenSignal)) {
                *result = 18;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & , const std::string & , const bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvRTSendMediaStateChanged)) {
                *result = 19;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const std::string & , const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvRemoteTerminalAskForMediaSignal)) {
                *result = 20;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::RecvSaviSignal)) {
                *result = 21;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvSetMemberWindowToFirstPositionSignal)) {
                *result = 22;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const TerminalInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvUpgradeTerminalInfoSignal)) {
                *result = 23;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvModifyStopMyAudioState)) {
                *result = 24;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(bool , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::cvServerTestResultSignal)) {
                *result = 25;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const RtpStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::receiveRtpStatSignal)) {
                *result = 26;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(const RecStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::receiveRecStatSignal)) {
                *result = 27;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::streamMediaReceiveStartedSignal)) {
                *result = 28;
            }
        }
        {
            typedef void (ConferenceManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceManager::streamMediaReceiveStoppedSignal)) {
                *result = 29;
            }
        }
    }
}

const QMetaObject ConferenceManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConferenceManager.data,
      qt_meta_data_ConferenceManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConferenceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceManager.stringdata))
        return static_cast<void*>(const_cast< ConferenceManager*>(this));
    return QObject::qt_metacast(_clname);
}

int ConferenceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 78)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 78;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 78)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 78;
    }
    return _id;
}

// SIGNAL 0
void ConferenceManager::cvConfListReceivedSignal(const ConferenceVector & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConferenceManager::cvMemberListReceivedSignal(const MemberVector & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConferenceManager::cvTerminalListReceivedSignal(const TerminalVector & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConferenceManager::cvMemberHandUpSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ConferenceManager::cvMemberHandDownSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ConferenceManager::cvTermianlCountChangeNotify(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ConferenceManager::cvJoinConferenceSuccessSignal(const std::string & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ConferenceManager::cvMemberLoginSignal(const MemberInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ConferenceManager::cvTerminalLoginSignal(const TerminalVector & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ConferenceManager::cvJoinRejectionSignal(const std::string & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ConferenceManager::cvTerminalLogoutSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ConferenceManager::cvEndConferenceSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}

// SIGNAL 12
void ConferenceManager::cvConfrenceInviteSignal(const ConfInfo * _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ConferenceManager::cvPasswordIncorrectSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 13, Q_NULLPTR);
}

// SIGNAL 14
void ConferenceManager::cvKickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 14, Q_NULLPTR);
}

// SIGNAL 15
void ConferenceManager::cvTextMsgReceivedSignal(const std::string & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void ConferenceManager::cvWhiteBoardMsgReceivedSignal(const std::string & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void ConferenceManager::cvPermitSpeakSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 17, Q_NULLPTR);
}

// SIGNAL 18
void ConferenceManager::cvHostUnShareScreenSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 18, Q_NULLPTR);
}

// SIGNAL 19
void ConferenceManager::cvRTSendMediaStateChanged(const std::string & _t1, const std::string & _t2, const bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void ConferenceManager::cvRemoteTerminalAskForMediaSignal(const std::string & _t1, const std::string & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void ConferenceManager::RecvSaviSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void ConferenceManager::cvSetMemberWindowToFirstPositionSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void ConferenceManager::cvUpgradeTerminalInfoSignal(const TerminalInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void ConferenceManager::cvModifyStopMyAudioState(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void ConferenceManager::cvServerTestResultSignal(bool _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void ConferenceManager::receiveRtpStatSignal(const RtpStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void ConferenceManager::receiveRecStatSignal(const RecStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void ConferenceManager::streamMediaReceiveStartedSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void ConferenceManager::streamMediaReceiveStoppedSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}
QT_END_MOC_NAMESPACE
