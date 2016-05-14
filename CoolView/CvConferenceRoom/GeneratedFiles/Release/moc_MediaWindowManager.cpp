/****************************************************************************
** Meta object code from reading C++ file 'MediaWindowManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MediaWindowManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaWindowManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MediaWindowManager_t {
    QByteArrayData data[73];
    char stringdata[1032];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaWindowManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaWindowManager_t qt_meta_stringdata_MediaWindowManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MediaWindowManager"
QT_MOC_LITERAL(1, 19, 14), // "AddMediaWindow"
QT_MOC_LITERAL(2, 34, 11), // "VideoFrame*"
QT_MOC_LITERAL(3, 46, 0), // ""
QT_MOC_LITERAL(4, 47, 6), // "userID"
QT_MOC_LITERAL(5, 54, 11), // "displayName"
QT_MOC_LITERAL(6, 66, 11), // "screenIndex"
QT_MOC_LITERAL(7, 78, 4), // "seet"
QT_MOC_LITERAL(8, 83, 10), // "videoWidth"
QT_MOC_LITERAL(9, 94, 11), // "videoHeight"
QT_MOC_LITERAL(10, 106, 18), // "AddScreenMediaRecv"
QT_MOC_LITERAL(11, 125, 2), // "ip"
QT_MOC_LITERAL(12, 128, 4), // "port"
QT_MOC_LITERAL(13, 133, 29), // "AddMainSpeakerScreenMediaRecv"
QT_MOC_LITERAL(14, 163, 21), // "RemoveScreenMediaRecv"
QT_MOC_LITERAL(15, 185, 16), // "CloseMediaWindow"
QT_MOC_LITERAL(16, 202, 16), // "ClearMediaWindow"
QT_MOC_LITERAL(17, 219, 14), // "ShowVideoFrame"
QT_MOC_LITERAL(18, 234, 5), // "frame"
QT_MOC_LITERAL(19, 240, 14), // "HideVideoFrame"
QT_MOC_LITERAL(20, 255, 16), // "ChangeMediaState"
QT_MOC_LITERAL(21, 272, 17), // "const VideoFrame*"
QT_MOC_LITERAL(22, 290, 9), // "mediaType"
QT_MOC_LITERAL(23, 300, 12), // "UiMediaState"
QT_MOC_LITERAL(24, 313, 10), // "mediaState"
QT_MOC_LITERAL(25, 324, 11), // "isAudioSend"
QT_MOC_LITERAL(26, 336, 19), // "ChangeWindowPostion"
QT_MOC_LITERAL(27, 356, 14), // "newScreenIndex"
QT_MOC_LITERAL(28, 371, 7), // "newSeet"
QT_MOC_LITERAL(29, 379, 15), // "ShowRtcpMessage"
QT_MOC_LITERAL(30, 395, 4), // "show"
QT_MOC_LITERAL(31, 400, 17), // "UpdateRtcpMessage"
QT_MOC_LITERAL(32, 418, 11), // "RtpStatItem"
QT_MOC_LITERAL(33, 430, 9), // "stateItem"
QT_MOC_LITERAL(34, 440, 19), // "UpdateRecordMessage"
QT_MOC_LITERAL(35, 460, 11), // "RecStatItem"
QT_MOC_LITERAL(36, 472, 12), // "ChangeLayout"
QT_MOC_LITERAL(37, 485, 12), // "displayModel"
QT_MOC_LITERAL(38, 498, 18), // "ControlScreenShare"
QT_MOC_LITERAL(39, 517, 7), // "control"
QT_MOC_LITERAL(40, 525, 19), // "OpenScreenShareFile"
QT_MOC_LITERAL(41, 545, 8), // "filePath"
QT_MOC_LITERAL(42, 554, 21), // "RemoveSendShareScreen"
QT_MOC_LITERAL(43, 576, 15), // "FindMediaWindow"
QT_MOC_LITERAL(44, 592, 18), // "ScreenCountChanged"
QT_MOC_LITERAL(45, 611, 8), // "newCount"
QT_MOC_LITERAL(46, 620, 26), // "MemberLocationUpdateNotify"
QT_MOC_LITERAL(47, 647, 15), // "controllerIndex"
QT_MOC_LITERAL(48, 663, 28), // "ScreenShareStateUpdateNotify"
QT_MOC_LITERAL(49, 692, 23), // "MemberLocationAddNotify"
QT_MOC_LITERAL(50, 716, 10), // "memberName"
QT_MOC_LITERAL(51, 727, 6), // "screen"
QT_MOC_LITERAL(52, 734, 11), // "audioEnable"
QT_MOC_LITERAL(53, 746, 11), // "videoEnable"
QT_MOC_LITERAL(54, 758, 26), // "MemberLocationRemoveNotify"
QT_MOC_LITERAL(55, 785, 26), // "MemberLocationChangeNotify"
QT_MOC_LITERAL(56, 812, 19), // "LayoutChangedNofity"
QT_MOC_LITERAL(57, 832, 23), // "MediaStateChangedNotify"
QT_MOC_LITERAL(58, 856, 13), // "SetScreenPort"
QT_MOC_LITERAL(59, 870, 10), // "screenPort"
QT_MOC_LITERAL(60, 881, 13), // "GetScreenPort"
QT_MOC_LITERAL(61, 895, 11), // "SetScreenIP"
QT_MOC_LITERAL(62, 907, 8), // "screenIP"
QT_MOC_LITERAL(63, 916, 11), // "GetScreenIP"
QT_MOC_LITERAL(64, 928, 17), // "KinectGestureSlot"
QT_MOC_LITERAL(65, 946, 2), // "to"
QT_MOC_LITERAL(66, 949, 4), // "type"
QT_MOC_LITERAL(67, 954, 21), // "ControlVideoRecording"
QT_MOC_LITERAL(68, 976, 6), // "userId"
QT_MOC_LITERAL(69, 983, 5), // "param"
QT_MOC_LITERAL(70, 989, 17), // "KinectMessageSlot"
QT_MOC_LITERAL(71, 1007, 7), // "message"
QT_MOC_LITERAL(72, 1015, 16) // "updateScreenSlot"

    },
    "MediaWindowManager\0AddMediaWindow\0"
    "VideoFrame*\0\0userID\0displayName\0"
    "screenIndex\0seet\0videoWidth\0videoHeight\0"
    "AddScreenMediaRecv\0ip\0port\0"
    "AddMainSpeakerScreenMediaRecv\0"
    "RemoveScreenMediaRecv\0CloseMediaWindow\0"
    "ClearMediaWindow\0ShowVideoFrame\0frame\0"
    "HideVideoFrame\0ChangeMediaState\0"
    "const VideoFrame*\0mediaType\0UiMediaState\0"
    "mediaState\0isAudioSend\0ChangeWindowPostion\0"
    "newScreenIndex\0newSeet\0ShowRtcpMessage\0"
    "show\0UpdateRtcpMessage\0RtpStatItem\0"
    "stateItem\0UpdateRecordMessage\0RecStatItem\0"
    "ChangeLayout\0displayModel\0ControlScreenShare\0"
    "control\0OpenScreenShareFile\0filePath\0"
    "RemoveSendShareScreen\0FindMediaWindow\0"
    "ScreenCountChanged\0newCount\0"
    "MemberLocationUpdateNotify\0controllerIndex\0"
    "ScreenShareStateUpdateNotify\0"
    "MemberLocationAddNotify\0memberName\0"
    "screen\0audioEnable\0videoEnable\0"
    "MemberLocationRemoveNotify\0"
    "MemberLocationChangeNotify\0"
    "LayoutChangedNofity\0MediaStateChangedNotify\0"
    "SetScreenPort\0screenPort\0GetScreenPort\0"
    "SetScreenIP\0screenIP\0GetScreenIP\0"
    "KinectGestureSlot\0to\0type\0"
    "ControlVideoRecording\0userId\0param\0"
    "KinectMessageSlot\0message\0updateScreenSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaWindowManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    6,  214,    3, 0x0a /* Public */,
      10,    4,  227,    3, 0x0a /* Public */,
      10,    3,  236,    3, 0x2a /* Public | MethodCloned */,
      13,    3,  243,    3, 0x0a /* Public */,
      14,    2,  250,    3, 0x0a /* Public */,
      14,    1,  255,    3, 0x2a /* Public | MethodCloned */,
      15,    1,  258,    3, 0x0a /* Public */,
      16,    0,  261,    3, 0x0a /* Public */,
      17,    1,  262,    3, 0x0a /* Public */,
      19,    1,  265,    3, 0x0a /* Public */,
      20,    4,  268,    3, 0x0a /* Public */,
      26,    3,  277,    3, 0x0a /* Public */,
      29,    2,  284,    3, 0x0a /* Public */,
      31,    1,  289,    3, 0x0a /* Public */,
      34,    1,  292,    3, 0x0a /* Public */,
      36,    2,  295,    3, 0x0a /* Public */,
      38,    1,  300,    3, 0x0a /* Public */,
      40,    2,  303,    3, 0x0a /* Public */,
      42,    0,  308,    3, 0x0a /* Public */,
      43,    1,  309,    3, 0x0a /* Public */,
      44,    1,  312,    3, 0x0a /* Public */,
      46,    1,  315,    3, 0x0a /* Public */,
      46,    0,  318,    3, 0x2a /* Public | MethodCloned */,
      48,    1,  319,    3, 0x0a /* Public */,
      48,    0,  322,    3, 0x2a /* Public | MethodCloned */,
      49,    5,  323,    3, 0x0a /* Public */,
      54,    3,  334,    3, 0x0a /* Public */,
      55,    3,  341,    3, 0x0a /* Public */,
      56,    1,  348,    3, 0x0a /* Public */,
      56,    0,  351,    3, 0x2a /* Public | MethodCloned */,
      57,    4,  352,    3, 0x0a /* Public */,
      57,    3,  361,    3, 0x2a /* Public | MethodCloned */,
      58,    1,  368,    3, 0x0a /* Public */,
      60,    0,  371,    3, 0x0a /* Public */,
      61,    1,  372,    3, 0x0a /* Public */,
      63,    0,  375,    3, 0x0a /* Public */,
      64,    2,  376,    3, 0x0a /* Public */,
      67,    3,  381,    3, 0x0a /* Public */,
      70,    2,  388,    3, 0x08 /* Private */,
      72,    0,  393,    3, 0x08 /* Private */,

 // slots: parameters
    0x80000000 | 2, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    4,    5,    6,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,    4,   11,   12,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    4,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    4,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,    6,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 2,   18,
    QMetaType::Void, 0x80000000 | 2,   18,
    QMetaType::Void, 0x80000000 | 21, QMetaType::QString, 0x80000000 | 23, QMetaType::Bool,   18,   22,   24,   25,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    4,   27,   28,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    6,   30,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void, 0x80000000 | 35,   33,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,   37,
    QMetaType::Void, QMetaType::Bool,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   41,    6,
    QMetaType::Void,
    0x80000000 | 2, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,   45,
    QMetaType::Void, QMetaType::Int,   47,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   47,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,   50,   51,    7,   52,   53,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   50,   51,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   50,   27,   28,
    QMetaType::Void, QMetaType::Int,   47,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,   50,   22,   24,   47,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   50,   22,   24,
    QMetaType::Void, QMetaType::Int,   59,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QString,   62,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   65,   66,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   68,   66,   69,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   66,   71,
    QMetaType::Void,

       0        // eod
};

void MediaWindowManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MediaWindowManager *_t = static_cast<MediaWindowManager *>(_o);
        switch (_id) {
        case 0: { VideoFrame* _r = _t->AddMediaWindow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5])),(*reinterpret_cast< const int(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< VideoFrame**>(_a[0]) = _r; }  break;
        case 1: _t->AddScreenMediaRecv((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 2: _t->AddScreenMediaRecv((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 3: _t->AddMainSpeakerScreenMediaRecv((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->RemoveScreenMediaRecv((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 5: _t->RemoveScreenMediaRecv((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->CloseMediaWindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->ClearMediaWindow(); break;
        case 8: _t->ShowVideoFrame((*reinterpret_cast< VideoFrame*(*)>(_a[1]))); break;
        case 9: _t->HideVideoFrame((*reinterpret_cast< VideoFrame*(*)>(_a[1]))); break;
        case 10: _t->ChangeMediaState((*reinterpret_cast< const VideoFrame*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const UiMediaState(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 11: _t->ChangeWindowPostion((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 12: _t->ShowRtcpMessage((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 13: _t->UpdateRtcpMessage((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 14: _t->UpdateRecordMessage((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 15: _t->ChangeLayout((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->ControlScreenShare((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 17: _t->OpenScreenShareFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->RemoveSendShareScreen(); break;
        case 19: { VideoFrame* _r = _t->FindMediaWindow((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< VideoFrame**>(_a[0]) = _r; }  break;
        case 20: _t->ScreenCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->MemberLocationUpdateNotify((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 22: _t->MemberLocationUpdateNotify(); break;
        case 23: _t->ScreenShareStateUpdateNotify((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 24: _t->ScreenShareStateUpdateNotify(); break;
        case 25: _t->MemberLocationAddNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const bool(*)>(_a[4])),(*reinterpret_cast< const bool(*)>(_a[5]))); break;
        case 26: _t->MemberLocationRemoveNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 27: _t->MemberLocationChangeNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 28: _t->LayoutChangedNofity((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 29: _t->LayoutChangedNofity(); break;
        case 30: _t->MediaStateChangedNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 31: _t->MediaStateChangedNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 32: _t->SetScreenPort((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 33: { int _r = _t->GetScreenPort();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 34: _t->SetScreenIP((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 35: { QString _r = _t->GetScreenIP();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 36: _t->KinectGestureSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 37: _t->ControlVideoRecording((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 38: _t->KinectMessageSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 39: _t->updateScreenSlot(); break;
        default: ;
        }
    }
}

const QMetaObject MediaWindowManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MediaWindowManager.data,
      qt_meta_data_MediaWindowManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MediaWindowManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaWindowManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MediaWindowManager.stringdata))
        return static_cast<void*>(const_cast< MediaWindowManager*>(this));
    return QObject::qt_metacast(_clname);
}

int MediaWindowManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
