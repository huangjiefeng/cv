/****************************************************************************
** Meta object code from reading C++ file 'accountproxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/im/jabber/client/accountproxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'accountproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvMsgAccountProxy_t {
    QByteArrayData data[60];
    char stringdata[685];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvMsgAccountProxy_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvMsgAccountProxy_t qt_meta_stringdata_CvMsgAccountProxy = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CvMsgAccountProxy"
QT_MOC_LITERAL(1, 18, 12), // "accountError"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "errCode"
QT_MOC_LITERAL(4, 40, 7), // "details"
QT_MOC_LITERAL(5, 48, 22), // "accountPresenceChanged"
QT_MOC_LITERAL(6, 71, 3), // "jid"
QT_MOC_LITERAL(7, 75, 8), // "presence"
QT_MOC_LITERAL(8, 84, 19), // "accountStateChanged"
QT_MOC_LITERAL(9, 104, 5), // "state"
QT_MOC_LITERAL(10, 110, 15), // "connectionClose"
QT_MOC_LITERAL(11, 126, 14), // "groupChatError"
QT_MOC_LITERAL(12, 141, 1), // "j"
QT_MOC_LITERAL(13, 143, 4), // "code"
QT_MOC_LITERAL(14, 148, 6), // "detail"
QT_MOC_LITERAL(15, 155, 16), // "groupChatInvited"
QT_MOC_LITERAL(16, 172, 2), // "gc"
QT_MOC_LITERAL(17, 175, 4), // "from"
QT_MOC_LITERAL(18, 180, 6), // "reason"
QT_MOC_LITERAL(19, 187, 15), // "groupChatJoined"
QT_MOC_LITERAL(20, 203, 13), // "groupChatLeft"
QT_MOC_LITERAL(21, 217, 17), // "groupChatPresence"
QT_MOC_LITERAL(22, 235, 6), // "status"
QT_MOC_LITERAL(23, 242, 12), // "loginSuccess"
QT_MOC_LITERAL(24, 255, 15), // "messageReceived"
QT_MOC_LITERAL(25, 271, 2), // "to"
QT_MOC_LITERAL(26, 274, 7), // "subject"
QT_MOC_LITERAL(27, 282, 4), // "body"
QT_MOC_LITERAL(28, 287, 4), // "time"
QT_MOC_LITERAL(29, 292, 15), // "presenceChanged"
QT_MOC_LITERAL(30, 308, 20), // "subscriptionAccepted"
QT_MOC_LITERAL(31, 329, 18), // "subscriptionDenied"
QT_MOC_LITERAL(32, 348, 20), // "subscriptionReceived"
QT_MOC_LITERAL(33, 369, 4), // "nick"
QT_MOC_LITERAL(34, 374, 18), // "acceptSubscription"
QT_MOC_LITERAL(35, 393, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(36, 413, 19), // "groupChatChangeNick"
QT_MOC_LITERAL(37, 433, 4), // "host"
QT_MOC_LITERAL(38, 438, 4), // "room"
QT_MOC_LITERAL(39, 443, 15), // "groupChatInvite"
QT_MOC_LITERAL(40, 459, 3), // "msg"
QT_MOC_LITERAL(41, 463, 13), // "groupChatJoin"
QT_MOC_LITERAL(42, 477, 23), // "QDBusPendingReply<bool>"
QT_MOC_LITERAL(43, 501, 8), // "password"
QT_MOC_LITERAL(44, 510, 8), // "maxchars"
QT_MOC_LITERAL(45, 519, 10), // "maxstanzas"
QT_MOC_LITERAL(46, 530, 7), // "seconds"
QT_MOC_LITERAL(47, 538, 6), // "Status"
QT_MOC_LITERAL(48, 545, 14), // "groupChatLeave"
QT_MOC_LITERAL(49, 560, 18), // "groupChatSetStatus"
QT_MOC_LITERAL(50, 579, 11), // "initAccount"
QT_MOC_LITERAL(51, 591, 22), // "QDBusPendingReply<int>"
QT_MOC_LITERAL(52, 614, 1), // "p"
QT_MOC_LITERAL(53, 616, 9), // "overwrite"
QT_MOC_LITERAL(54, 626, 5), // "login"
QT_MOC_LITERAL(55, 632, 6), // "logout"
QT_MOC_LITERAL(56, 639, 11), // "sendMessage"
QT_MOC_LITERAL(57, 651, 11), // "setPresence"
QT_MOC_LITERAL(58, 663, 9), // "subscribe"
QT_MOC_LITERAL(59, 673, 11) // "unsubscribe"

    },
    "CvMsgAccountProxy\0accountError\0\0errCode\0"
    "details\0accountPresenceChanged\0jid\0"
    "presence\0accountStateChanged\0state\0"
    "connectionClose\0groupChatError\0j\0code\0"
    "detail\0groupChatInvited\0gc\0from\0reason\0"
    "groupChatJoined\0groupChatLeft\0"
    "groupChatPresence\0status\0loginSuccess\0"
    "messageReceived\0to\0subject\0body\0time\0"
    "presenceChanged\0subscriptionAccepted\0"
    "subscriptionDenied\0subscriptionReceived\0"
    "nick\0acceptSubscription\0QDBusPendingReply<>\0"
    "groupChatChangeNick\0host\0room\0"
    "groupChatInvite\0msg\0groupChatJoin\0"
    "QDBusPendingReply<bool>\0password\0"
    "maxchars\0maxstanzas\0seconds\0Status\0"
    "groupChatLeave\0groupChatSetStatus\0"
    "initAccount\0QDBusPendingReply<int>\0p\0"
    "overwrite\0login\0logout\0sendMessage\0"
    "setPresence\0subscribe\0unsubscribe"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvMsgAccountProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  189,    2, 0x06 /* Public */,
       5,    2,  194,    2, 0x06 /* Public */,
       8,    2,  199,    2, 0x06 /* Public */,
      10,    0,  204,    2, 0x06 /* Public */,
      11,    3,  205,    2, 0x06 /* Public */,
      15,    3,  212,    2, 0x06 /* Public */,
      19,    1,  219,    2, 0x06 /* Public */,
      20,    1,  222,    2, 0x06 /* Public */,
      21,    2,  225,    2, 0x06 /* Public */,
      23,    1,  230,    2, 0x06 /* Public */,
      24,    5,  233,    2, 0x06 /* Public */,
      29,    1,  244,    2, 0x06 /* Public */,
      30,    1,  247,    2, 0x06 /* Public */,
      31,    1,  250,    2, 0x06 /* Public */,
      32,    2,  253,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      34,    1,  258,    2, 0x0a /* Public */,
      36,    4,  261,    2, 0x0a /* Public */,
      39,    3,  270,    2, 0x0a /* Public */,
      41,    3,  277,    2, 0x0a /* Public */,
      41,    4,  284,    2, 0x0a /* Public */,
      41,    5,  293,    2, 0x0a /* Public */,
      41,    6,  304,    2, 0x0a /* Public */,
      41,    7,  317,    2, 0x0a /* Public */,
      41,    8,  332,    2, 0x0a /* Public */,
      48,    2,  349,    2, 0x0a /* Public */,
      49,    3,  354,    2, 0x0a /* Public */,
      50,    2,  361,    2, 0x0a /* Public */,
      50,    3,  366,    2, 0x0a /* Public */,
      54,    1,  373,    2, 0x0a /* Public */,
      55,    0,  376,    2, 0x0a /* Public */,
      56,    3,  377,    2, 0x0a /* Public */,
      57,    1,  384,    2, 0x0a /* Public */,
      58,    1,  387,    2, 0x0a /* Public */,
      58,    2,  390,    2, 0x0a /* Public */,
      59,    1,  395,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   12,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   16,   17,   18,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   12,   22,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   25,   26,   27,   28,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   12,   33,

 // slots: parameters
    0x80000000 | 35, QMetaType::QString,    6,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   37,   38,   33,   22,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString, QMetaType::QString,   38,    6,   40,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString,   37,   38,   33,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   37,   38,   33,   43,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   37,   38,   33,   43,   44,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   37,   38,   33,   43,   44,   45,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   37,   38,   33,   43,   44,   45,   46,
    0x80000000 | 42, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   37,   38,   33,   43,   44,   45,   46,   47,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString,   37,   38,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString, QMetaType::Int,   37,   38,   22,
    0x80000000 | 51, QMetaType::QString, QMetaType::QString,   12,   52,
    0x80000000 | 51, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   12,   52,   53,
    0x80000000 | 51, QMetaType::Int,   22,
    0x80000000 | 35,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString, QMetaType::QString,   25,   26,   27,
    0x80000000 | 51, QMetaType::Int,   22,
    0x80000000 | 35, QMetaType::QString,    6,
    0x80000000 | 35, QMetaType::QString, QMetaType::QString,    6,   33,
    0x80000000 | 35, QMetaType::QString,    6,

       0        // eod
};

void CvMsgAccountProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvMsgAccountProxy *_t = static_cast<CvMsgAccountProxy *>(_o);
        switch (_id) {
        case 0: _t->accountError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->accountPresenceChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->accountStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->connectionClose(); break;
        case 4: _t->groupChatError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: _t->groupChatInvited((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: _t->groupChatJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->groupChatLeft((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->groupChatPresence((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->loginSuccess((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 11: _t->presenceChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->subscriptionAccepted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->subscriptionDenied((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->subscriptionReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 15: { QDBusPendingReply<> _r = _t->acceptSubscription((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 16: { QDBusPendingReply<> _r = _t->groupChatChangeNick((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 17: { QDBusPendingReply<> _r = _t->groupChatInvite((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 18: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 19: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 20: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 21: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 22: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 23: { QDBusPendingReply<bool> _r = _t->groupChatJoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 24: { QDBusPendingReply<> _r = _t->groupChatLeave((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 25: { QDBusPendingReply<> _r = _t->groupChatSetStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 26: { QDBusPendingReply<int> _r = _t->initAccount((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<int>*>(_a[0]) = _r; }  break;
        case 27: { QDBusPendingReply<int> _r = _t->initAccount((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<int>*>(_a[0]) = _r; }  break;
        case 28: { QDBusPendingReply<int> _r = _t->login((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<int>*>(_a[0]) = _r; }  break;
        case 29: { QDBusPendingReply<> _r = _t->logout();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 30: { QDBusPendingReply<> _r = _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 31: { QDBusPendingReply<int> _r = _t->setPresence((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<int>*>(_a[0]) = _r; }  break;
        case 32: { QDBusPendingReply<> _r = _t->subscribe((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 33: { QDBusPendingReply<> _r = _t->subscribe((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 34: { QDBusPendingReply<> _r = _t->unsubscribe((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvMsgAccountProxy::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::accountError)) {
                *result = 0;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::accountPresenceChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::accountStateChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::connectionClose)) {
                *result = 3;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::groupChatError)) {
                *result = 4;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::groupChatInvited)) {
                *result = 5;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::groupChatJoined)) {
                *result = 6;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::groupChatLeft)) {
                *result = 7;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::groupChatPresence)) {
                *result = 8;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::loginSuccess)) {
                *result = 9;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , const QString & , const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::messageReceived)) {
                *result = 10;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::presenceChanged)) {
                *result = 11;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::subscriptionAccepted)) {
                *result = 12;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::subscriptionDenied)) {
                *result = 13;
            }
        }
        {
            typedef void (CvMsgAccountProxy::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvMsgAccountProxy::subscriptionReceived)) {
                *result = 14;
            }
        }
    }
}

const QMetaObject CvMsgAccountProxy::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvMsgAccountProxy.data,
      qt_meta_data_CvMsgAccountProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvMsgAccountProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvMsgAccountProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvMsgAccountProxy.stringdata))
        return static_cast<void*>(const_cast< CvMsgAccountProxy*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvMsgAccountProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void CvMsgAccountProxy::accountError(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CvMsgAccountProxy::accountPresenceChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CvMsgAccountProxy::accountStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CvMsgAccountProxy::connectionClose()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void CvMsgAccountProxy::groupChatError(const QString & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CvMsgAccountProxy::groupChatInvited(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CvMsgAccountProxy::groupChatJoined(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CvMsgAccountProxy::groupChatLeft(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CvMsgAccountProxy::groupChatPresence(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CvMsgAccountProxy::loginSuccess(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CvMsgAccountProxy::messageReceived(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CvMsgAccountProxy::presenceChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CvMsgAccountProxy::subscriptionAccepted(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CvMsgAccountProxy::subscriptionDenied(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CvMsgAccountProxy::subscriptionReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_END_MOC_NAMESPACE
