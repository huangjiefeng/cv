/****************************************************************************
** Meta object code from reading C++ file 'display_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../display_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'display_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DisplayController_t {
    QByteArrayData data[52];
    char stringdata[1121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DisplayController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DisplayController_t qt_meta_stringdata_DisplayController = {
    {
QT_MOC_LITERAL(0, 0, 17), // "DisplayController"
QT_MOC_LITERAL(1, 18, 29), // "HandleReceiveVideoRequestSlot"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 4), // "vuri"
QT_MOC_LITERAL(4, 54, 12), // "screen_index"
QT_MOC_LITERAL(5, 67, 10), // "seat_index"
QT_MOC_LITERAL(6, 78, 15), // "use_small_video"
QT_MOC_LITERAL(7, 94, 39), // "HandleRequestStreamMediaAutoR..."
QT_MOC_LITERAL(8, 134, 10), // "remote_uri"
QT_MOC_LITERAL(9, 145, 26), // "HandleStopVideoRequestSlot"
QT_MOC_LITERAL(10, 172, 29), // "HandleReceiveAudioRequestSlot"
QT_MOC_LITERAL(11, 202, 3), // "uri"
QT_MOC_LITERAL(12, 206, 6), // "enable"
QT_MOC_LITERAL(13, 213, 32), // "HandleReceiveDocumentRequestSlot"
QT_MOC_LITERAL(14, 246, 29), // "HandleStopDocumentRequestSlot"
QT_MOC_LITERAL(15, 276, 29), // "HandleOpenDocumentRequestSlot"
QT_MOC_LITERAL(16, 306, 8), // "filename"
QT_MOC_LITERAL(17, 315, 20), // "HandlePPTControlSlot"
QT_MOC_LITERAL(18, 336, 4), // "type"
QT_MOC_LITERAL(19, 341, 32), // "HandleShowRTCPMessageRequestSlot"
QT_MOC_LITERAL(20, 374, 4), // "show"
QT_MOC_LITERAL(21, 379, 31), // "HandleSendLocalAudioRequestSlot"
QT_MOC_LITERAL(22, 411, 31), // "HandleSendLocalVideoRequestSlot"
QT_MOC_LITERAL(23, 443, 13), // "virtual_index"
QT_MOC_LITERAL(24, 457, 28), // "HandleVoiceChangeRequestSlot"
QT_MOC_LITERAL(25, 486, 15), // "AudioDeviceType"
QT_MOC_LITERAL(26, 502, 6), // "volume"
QT_MOC_LITERAL(27, 509, 26), // "HandleVoiceMuteRequestSlot"
QT_MOC_LITERAL(28, 536, 6), // "isMute"
QT_MOC_LITERAL(29, 543, 28), // "HandlePermitSpeakRequestSlot"
QT_MOC_LITERAL(30, 572, 5), // "allow"
QT_MOC_LITERAL(31, 578, 29), // "HandleScreenLayoutRequestSlot"
QT_MOC_LITERAL(32, 608, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(33, 641, 6), // "layout"
QT_MOC_LITERAL(34, 648, 40), // "HandlePanelVisibleStateChange..."
QT_MOC_LITERAL(35, 689, 20), // "telecontroller_index"
QT_MOC_LITERAL(36, 710, 7), // "visible"
QT_MOC_LITERAL(37, 718, 36), // "HandleReceiveAllLocalVideoNot..."
QT_MOC_LITERAL(38, 755, 37), // "HandleClearAudioDisplayDictNo..."
QT_MOC_LITERAL(39, 793, 30), // "HandleTerminalUpdateNotifySlot"
QT_MOC_LITERAL(40, 824, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(41, 845, 8), // "terminal"
QT_MOC_LITERAL(42, 854, 14), // "conference_uri"
QT_MOC_LITERAL(43, 869, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(44, 908, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(45, 947, 5), // "state"
QT_MOC_LITERAL(46, 953, 36), // "HandleScreenReceiveStartedNot..."
QT_MOC_LITERAL(47, 990, 36), // "HandleScreenReceiveStoppedNot..."
QT_MOC_LITERAL(48, 1027, 30), // "HandleCheckReceiveVideoAckSlot"
QT_MOC_LITERAL(49, 1058, 39), // "HandleStreamMediaReceiveReply..."
QT_MOC_LITERAL(50, 1098, 11), // "remote_vuri"
QT_MOC_LITERAL(51, 1110, 10) // "permission"

    },
    "DisplayController\0HandleReceiveVideoRequestSlot\0"
    "\0vuri\0screen_index\0seat_index\0"
    "use_small_video\0HandleRequestStreamMediaAutoReceiveSlot\0"
    "remote_uri\0HandleStopVideoRequestSlot\0"
    "HandleReceiveAudioRequestSlot\0uri\0"
    "enable\0HandleReceiveDocumentRequestSlot\0"
    "HandleStopDocumentRequestSlot\0"
    "HandleOpenDocumentRequestSlot\0filename\0"
    "HandlePPTControlSlot\0type\0"
    "HandleShowRTCPMessageRequestSlot\0show\0"
    "HandleSendLocalAudioRequestSlot\0"
    "HandleSendLocalVideoRequestSlot\0"
    "virtual_index\0HandleVoiceChangeRequestSlot\0"
    "AudioDeviceType\0volume\0"
    "HandleVoiceMuteRequestSlot\0isMute\0"
    "HandlePermitSpeakRequestSlot\0allow\0"
    "HandleScreenLayoutRequestSlot\0"
    "IDisplayController::ScreenLayout\0"
    "layout\0HandlePanelVisibleStateChangedNotifySlot\0"
    "telecontroller_index\0visible\0"
    "HandleReceiveAllLocalVideoNotifySlot\0"
    "HandleClearAudioDisplayDictNotifySlot\0"
    "HandleTerminalUpdateNotifySlot\0"
    "ConstTerminalPointer\0terminal\0"
    "conference_uri\0HandleConferenceStateChangedNotifySlot\0"
    "IConferenceController::ConferenceState\0"
    "state\0HandleScreenReceiveStartedNotifySlot\0"
    "HandleScreenReceiveStoppedNotifySlot\0"
    "HandleCheckReceiveVideoAckSlot\0"
    "HandleStreamMediaReceiveReplyNotifySlot\0"
    "remote_vuri\0permission"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DisplayController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,  134,    2, 0x0a /* Public */,
       7,    1,  143,    2, 0x0a /* Public */,
       9,    1,  146,    2, 0x0a /* Public */,
      10,    2,  149,    2, 0x0a /* Public */,
      13,    2,  154,    2, 0x0a /* Public */,
      14,    1,  159,    2, 0x0a /* Public */,
      15,    2,  162,    2, 0x0a /* Public */,
      17,    1,  167,    2, 0x0a /* Public */,
      19,    2,  170,    2, 0x0a /* Public */,
      21,    1,  175,    2, 0x0a /* Public */,
      22,    2,  178,    2, 0x0a /* Public */,
      24,    2,  183,    2, 0x0a /* Public */,
      27,    2,  188,    2, 0x0a /* Public */,
      29,    2,  193,    2, 0x0a /* Public */,
      31,    2,  198,    2, 0x0a /* Public */,
      34,    2,  203,    2, 0x0a /* Public */,
      37,    0,  208,    2, 0x0a /* Public */,
      38,    0,  209,    2, 0x0a /* Public */,
      39,    2,  210,    2, 0x09 /* Protected */,
      43,    2,  215,    2, 0x09 /* Protected */,
      46,    2,  220,    2, 0x09 /* Protected */,
      47,    2,  225,    2, 0x09 /* Protected */,
      48,    0,  230,    2, 0x09 /* Protected */,
      49,    2,  231,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   11,    4,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   16,    4,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   20,    4,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   23,   12,
    QMetaType::Void, 0x80000000 | 25, QMetaType::Int,   18,   26,
    QMetaType::Void, 0x80000000 | 25, QMetaType::Bool,   18,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   11,   30,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 32,    4,   33,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   35,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 40, QMetaType::QString,   41,   42,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 44,   42,   45,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   50,   51,

       0        // eod
};

void DisplayController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DisplayController *_t = static_cast<DisplayController *>(_o);
        switch (_id) {
        case 0: _t->HandleReceiveVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->HandleRequestStreamMediaAutoReceiveSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->HandleStopVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->HandleReceiveAudioRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->HandleReceiveDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->HandleStopDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->HandleOpenDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->HandlePPTControlSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->HandleShowRTCPMessageRequestSlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->HandleSendLocalAudioRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->HandleSendLocalVideoRequestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 11: _t->HandleVoiceChangeRequestSlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->HandleVoiceMuteRequestSlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 13: _t->HandlePermitSpeakRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 14: _t->HandleScreenLayoutRequestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 15: _t->HandlePanelVisibleStateChangedNotifySlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: _t->HandleReceiveAllLocalVideoNotifySlot(); break;
        case 17: _t->HandleClearAudioDisplayDictNotifySlot(); break;
        case 18: _t->HandleTerminalUpdateNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 20: _t->HandleScreenReceiveStartedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->HandleScreenReceiveStoppedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->HandleCheckReceiveVideoAckSlot(); break;
        case 23: _t->HandleStreamMediaReceiveReplyNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject DisplayController::staticMetaObject = {
    { &IDisplayController::staticMetaObject, qt_meta_stringdata_DisplayController.data,
      qt_meta_data_DisplayController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DisplayController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DisplayController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayController.stringdata))
        return static_cast<void*>(const_cast< DisplayController*>(this));
    return IDisplayController::qt_metacast(_clname);
}

int DisplayController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IDisplayController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
