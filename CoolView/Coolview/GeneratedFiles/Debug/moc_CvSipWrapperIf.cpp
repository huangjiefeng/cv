/****************************************************************************
** Meta object code from reading C++ file 'CvSipWrapperIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/sipwrapper/client/CvSipWrapperIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvSipWrapperIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvSipWrapperIf_t {
    QByteArrayData data[73];
    char stringdata[830];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvSipWrapperIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvSipWrapperIf_t qt_meta_stringdata_CvSipWrapperIf = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CvSipWrapperIf"
QT_MOC_LITERAL(1, 15, 22), // "cvMessageReceivedEvent"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "message"
QT_MOC_LITERAL(4, 47, 4), // "from"
QT_MOC_LITERAL(5, 52, 23), // "incomingSubscribeSignal"
QT_MOC_LITERAL(6, 76, 3), // "sid"
QT_MOC_LITERAL(7, 80, 7), // "evtType"
QT_MOC_LITERAL(8, 88, 27), // "phoneCallStateChangedSignal"
QT_MOC_LITERAL(9, 116, 6), // "callId"
QT_MOC_LITERAL(10, 123, 5), // "state"
QT_MOC_LITERAL(11, 129, 27), // "phoneLineStateChangedSignal"
QT_MOC_LITERAL(12, 157, 6), // "lineId"
QT_MOC_LITERAL(13, 164, 10), // "acceptCall"
QT_MOC_LITERAL(14, 175, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(15, 195, 8), // "username"
QT_MOC_LITERAL(16, 204, 9), // "memberURI"
QT_MOC_LITERAL(17, 214, 11), // "enableVideo"
QT_MOC_LITERAL(18, 226, 10), // "sdpMessage"
QT_MOC_LITERAL(19, 237, 18), // "acceptSubscription"
QT_MOC_LITERAL(20, 256, 14), // "addVirtualLine"
QT_MOC_LITERAL(21, 271, 11), // "displayName"
QT_MOC_LITERAL(22, 283, 8), // "identity"
QT_MOC_LITERAL(23, 292, 8), // "password"
QT_MOC_LITERAL(24, 301, 5), // "realm"
QT_MOC_LITERAL(25, 307, 11), // "proxyServer"
QT_MOC_LITERAL(26, 319, 14), // "registerServer"
QT_MOC_LITERAL(27, 334, 14), // "address_family"
QT_MOC_LITERAL(28, 349, 9), // "closeCall"
QT_MOC_LITERAL(29, 359, 14), // "exitConference"
QT_MOC_LITERAL(30, 374, 7), // "confURI"
QT_MOC_LITERAL(31, 382, 8), // "holdCall"
QT_MOC_LITERAL(32, 391, 4), // "init"
QT_MOC_LITERAL(33, 396, 11), // "ipv6_enable"
QT_MOC_LITERAL(34, 408, 13), // "isInitialized"
QT_MOC_LITERAL(35, 422, 23), // "QDBusPendingReply<bool>"
QT_MOC_LITERAL(36, 446, 8), // "makeCall"
QT_MOC_LITERAL(37, 455, 12), // "makeInfoCall"
QT_MOC_LITERAL(38, 468, 6), // "sipURI"
QT_MOC_LITERAL(39, 475, 9), // "szContent"
QT_MOC_LITERAL(40, 485, 18), // "makeJoinConfInvite"
QT_MOC_LITERAL(41, 504, 8), // "focusURI"
QT_MOC_LITERAL(42, 513, 19), // "registerVirtualLine"
QT_MOC_LITERAL(43, 533, 7), // "content"
QT_MOC_LITERAL(44, 541, 10), // "rejectCall"
QT_MOC_LITERAL(45, 552, 18), // "rejectSubscription"
QT_MOC_LITERAL(46, 571, 17), // "removeVirtualLine"
QT_MOC_LITERAL(47, 589, 5), // "force"
QT_MOC_LITERAL(48, 595, 10), // "resumeCall"
QT_MOC_LITERAL(49, 606, 11), // "sendQosPara"
QT_MOC_LITERAL(50, 618, 13), // "currentUserID"
QT_MOC_LITERAL(51, 632, 10), // "sendSipMsg"
QT_MOC_LITERAL(52, 643, 9), // "remoteURI"
QT_MOC_LITERAL(53, 653, 10), // "msgContent"
QT_MOC_LITERAL(54, 664, 10), // "setNatType"
QT_MOC_LITERAL(55, 675, 7), // "natType"
QT_MOC_LITERAL(56, 683, 8), // "setProxy"
QT_MOC_LITERAL(57, 692, 7), // "address"
QT_MOC_LITERAL(58, 700, 4), // "port"
QT_MOC_LITERAL(59, 705, 5), // "login"
QT_MOC_LITERAL(60, 711, 6), // "setSIP"
QT_MOC_LITERAL(61, 718, 6), // "server"
QT_MOC_LITERAL(62, 725, 10), // "serverPort"
QT_MOC_LITERAL(63, 736, 9), // "localPort"
QT_MOC_LITERAL(64, 746, 13), // "setSipOptions"
QT_MOC_LITERAL(65, 760, 7), // "optname"
QT_MOC_LITERAL(66, 768, 6), // "optval"
QT_MOC_LITERAL(67, 775, 9), // "setTunnel"
QT_MOC_LITERAL(68, 785, 3), // "ssl"
QT_MOC_LITERAL(69, 789, 9), // "setUaName"
QT_MOC_LITERAL(70, 799, 4), // "name"
QT_MOC_LITERAL(71, 804, 15), // "startConference"
QT_MOC_LITERAL(72, 820, 9) // "terminate"

    },
    "CvSipWrapperIf\0cvMessageReceivedEvent\0"
    "\0message\0from\0incomingSubscribeSignal\0"
    "sid\0evtType\0phoneCallStateChangedSignal\0"
    "callId\0state\0phoneLineStateChangedSignal\0"
    "lineId\0acceptCall\0QDBusPendingReply<>\0"
    "username\0memberURI\0enableVideo\0"
    "sdpMessage\0acceptSubscription\0"
    "addVirtualLine\0displayName\0identity\0"
    "password\0realm\0proxyServer\0registerServer\0"
    "address_family\0closeCall\0exitConference\0"
    "confURI\0holdCall\0init\0ipv6_enable\0"
    "isInitialized\0QDBusPendingReply<bool>\0"
    "makeCall\0makeInfoCall\0sipURI\0szContent\0"
    "makeJoinConfInvite\0focusURI\0"
    "registerVirtualLine\0content\0rejectCall\0"
    "rejectSubscription\0removeVirtualLine\0"
    "force\0resumeCall\0sendQosPara\0currentUserID\0"
    "sendSipMsg\0remoteURI\0msgContent\0"
    "setNatType\0natType\0setProxy\0address\0"
    "port\0login\0setSIP\0server\0serverPort\0"
    "localPort\0setSipOptions\0optname\0optval\0"
    "setTunnel\0ssl\0setUaName\0name\0"
    "startConference\0terminate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvSipWrapperIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  164,    2, 0x06 /* Public */,
       5,    3,  169,    2, 0x06 /* Public */,
       8,    3,  176,    2, 0x06 /* Public */,
      11,    2,  183,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    4,  188,    2, 0x0a /* Public */,
      19,    1,  197,    2, 0x0a /* Public */,
      20,    8,  200,    2, 0x0a /* Public */,
      28,    2,  217,    2, 0x0a /* Public */,
      29,    1,  222,    2, 0x0a /* Public */,
      31,    2,  225,    2, 0x0a /* Public */,
      32,    1,  230,    2, 0x0a /* Public */,
      34,    0,  233,    2, 0x0a /* Public */,
      36,    3,  234,    2, 0x0a /* Public */,
      37,    2,  241,    2, 0x0a /* Public */,
      40,    4,  246,    2, 0x0a /* Public */,
      42,    2,  255,    2, 0x0a /* Public */,
      44,    2,  260,    2, 0x0a /* Public */,
      45,    1,  265,    2, 0x0a /* Public */,
      46,    2,  268,    2, 0x0a /* Public */,
      48,    2,  273,    2, 0x0a /* Public */,
      49,    3,  278,    2, 0x0a /* Public */,
      51,    3,  285,    2, 0x0a /* Public */,
      54,    1,  292,    2, 0x0a /* Public */,
      56,    4,  295,    2, 0x0a /* Public */,
      60,    3,  304,    2, 0x0a /* Public */,
      64,    2,  311,    2, 0x0a /* Public */,
      67,    3,  316,    2, 0x0a /* Public */,
      69,    1,  323,    2, 0x0a /* Public */,
      71,    3,  326,    2, 0x0a /* Public */,
      72,    0,  333,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    6,    4,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    9,   10,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   12,   10,

 // slots: parameters
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::QByteArray,   15,   16,   17,   18,
    0x80000000 | 14, QMetaType::Int,    6,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   21,   15,   22,   23,   24,   25,   26,   27,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   15,   16,
    0x80000000 | 14, QMetaType::QString,   30,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   15,   16,
    0x80000000 | 14, QMetaType::Int,   33,
    0x80000000 | 35,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,   15,   16,   18,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   38,   39,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,   15,   41,   39,   18,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   15,   43,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   15,   16,
    0x80000000 | 14, QMetaType::Int,    6,
    0x80000000 | 14, QMetaType::QString, QMetaType::Bool,   15,   47,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   15,   16,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QString,   50,   41,   39,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QString,   50,   52,   53,
    0x80000000 | 14, QMetaType::Int,   55,
    0x80000000 | 14, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::QString,   57,   58,   59,   23,
    0x80000000 | 14, QMetaType::QString, QMetaType::Int, QMetaType::Int,   61,   62,   63,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString,   65,   66,
    0x80000000 | 14, QMetaType::QString, QMetaType::Int, QMetaType::Bool,   57,   58,   68,
    0x80000000 | 14, QMetaType::QString,   70,
    0x80000000 | 14, QMetaType::QString, QMetaType::QString, QMetaType::QString,   15,   30,   39,
    0x80000000 | 14,

       0        // eod
};

void CvSipWrapperIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvSipWrapperIf *_t = static_cast<CvSipWrapperIf *>(_o);
        switch (_id) {
        case 0: _t->cvMessageReceivedEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->incomingSubscribeSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: _t->phoneCallStateChangedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->phoneLineStateChangedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: { QDBusPendingReply<> _r = _t->acceptCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->acceptSubscription((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->addVirtualLine((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->closeCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->exitConference((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->holdCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 10: { QDBusPendingReply<> _r = _t->init((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 11: { QDBusPendingReply<bool> _r = _t->isInitialized();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 12: { QDBusPendingReply<> _r = _t->makeCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 13: { QDBusPendingReply<> _r = _t->makeInfoCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 14: { QDBusPendingReply<> _r = _t->makeJoinConfInvite((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 15: { QDBusPendingReply<> _r = _t->registerVirtualLine((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 16: { QDBusPendingReply<> _r = _t->rejectCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 17: { QDBusPendingReply<> _r = _t->rejectSubscription((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 18: { QDBusPendingReply<> _r = _t->removeVirtualLine((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 19: { QDBusPendingReply<> _r = _t->resumeCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 20: { QDBusPendingReply<> _r = _t->sendQosPara((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 21: { QDBusPendingReply<> _r = _t->sendSipMsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 22: { QDBusPendingReply<> _r = _t->setNatType((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 23: { QDBusPendingReply<> _r = _t->setProxy((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 24: { QDBusPendingReply<> _r = _t->setSIP((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 25: { QDBusPendingReply<> _r = _t->setSipOptions((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 26: { QDBusPendingReply<> _r = _t->setTunnel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 27: { QDBusPendingReply<> _r = _t->setUaName((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 28: { QDBusPendingReply<> _r = _t->startConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 29: { QDBusPendingReply<> _r = _t->terminate();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvSipWrapperIf::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvSipWrapperIf::cvMessageReceivedEvent)) {
                *result = 0;
            }
        }
        {
            typedef void (CvSipWrapperIf::*_t)(int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvSipWrapperIf::incomingSubscribeSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (CvSipWrapperIf::*_t)(const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvSipWrapperIf::phoneCallStateChangedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (CvSipWrapperIf::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvSipWrapperIf::phoneLineStateChangedSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject CvSipWrapperIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvSipWrapperIf.data,
      qt_meta_data_CvSipWrapperIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvSipWrapperIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvSipWrapperIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvSipWrapperIf.stringdata))
        return static_cast<void*>(const_cast< CvSipWrapperIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvSipWrapperIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void CvSipWrapperIf::cvMessageReceivedEvent(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CvSipWrapperIf::incomingSubscribeSignal(int _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CvSipWrapperIf::phoneCallStateChangedSignal(const QString & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CvSipWrapperIf::phoneLineStateChangedSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
