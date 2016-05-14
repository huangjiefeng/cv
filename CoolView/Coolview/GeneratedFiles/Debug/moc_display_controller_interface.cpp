/****************************************************************************
** Meta object code from reading C++ file 'display_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../display_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'display_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IDisplayController_t {
    QByteArrayData data[53];
    char stringdata[1127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IDisplayController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IDisplayController_t qt_meta_stringdata_IDisplayController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "IDisplayController"
QT_MOC_LITERAL(1, 19, 24), // "NotifyReceiveVideoSignal"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "vuri"
QT_MOC_LITERAL(4, 50, 12), // "screen_index"
QT_MOC_LITERAL(5, 63, 10), // "seat_index"
QT_MOC_LITERAL(6, 74, 15), // "use_small_video"
QT_MOC_LITERAL(7, 90, 21), // "NotifyStopVideoSignal"
QT_MOC_LITERAL(8, 112, 24), // "NotifyReceiveAudioSignal"
QT_MOC_LITERAL(9, 137, 3), // "uri"
QT_MOC_LITERAL(10, 141, 6), // "enable"
QT_MOC_LITERAL(11, 148, 27), // "NotifyReceiveDocumentSignal"
QT_MOC_LITERAL(12, 176, 24), // "NotifyStopDocumentSignal"
QT_MOC_LITERAL(13, 201, 27), // "NotifyShowRTCPMessageSignal"
QT_MOC_LITERAL(14, 229, 4), // "show"
QT_MOC_LITERAL(15, 234, 26), // "NotifySendLocalAudioSignal"
QT_MOC_LITERAL(16, 261, 26), // "NotifySendLocalVideoSignal"
QT_MOC_LITERAL(17, 288, 13), // "virtual_index"
QT_MOC_LITERAL(18, 302, 23), // "NotifyVoiceChangeSignal"
QT_MOC_LITERAL(19, 326, 15), // "AudioDeviceType"
QT_MOC_LITERAL(20, 342, 4), // "type"
QT_MOC_LITERAL(21, 347, 6), // "volume"
QT_MOC_LITERAL(22, 354, 22), // "NotifyMuteChangeSignal"
QT_MOC_LITERAL(23, 377, 7), // "is_mute"
QT_MOC_LITERAL(24, 385, 25), // "NotifyTerminalSpeakSignal"
QT_MOC_LITERAL(25, 411, 5), // "allow"
QT_MOC_LITERAL(26, 417, 31), // "NotifyScreenLayoutChangedSignal"
QT_MOC_LITERAL(27, 449, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(28, 482, 6), // "layout"
QT_MOC_LITERAL(29, 489, 21), // "SetPanelVisibleSignal"
QT_MOC_LITERAL(30, 511, 7), // "visible"
QT_MOC_LITERAL(31, 519, 28), // "CheckPanelVisibleStateSignal"
QT_MOC_LITERAL(32, 548, 20), // "telecontroller_index"
QT_MOC_LITERAL(33, 569, 29), // "HandleReceiveVideoRequestSlot"
QT_MOC_LITERAL(34, 599, 39), // "HandleRequestStreamMediaAutoR..."
QT_MOC_LITERAL(35, 639, 10), // "remote_uri"
QT_MOC_LITERAL(36, 650, 26), // "HandleStopVideoRequestSlot"
QT_MOC_LITERAL(37, 677, 29), // "HandleReceiveAudioRequestSlot"
QT_MOC_LITERAL(38, 707, 32), // "HandleReceiveDocumentRequestSlot"
QT_MOC_LITERAL(39, 740, 29), // "HandleStopDocumentRequestSlot"
QT_MOC_LITERAL(40, 770, 29), // "HandleOpenDocumentRequestSlot"
QT_MOC_LITERAL(41, 800, 8), // "filename"
QT_MOC_LITERAL(42, 809, 20), // "HandlePPTControlSlot"
QT_MOC_LITERAL(43, 830, 32), // "HandleShowRTCPMessageRequestSlot"
QT_MOC_LITERAL(44, 863, 31), // "HandleSendLocalAudioRequestSlot"
QT_MOC_LITERAL(45, 895, 31), // "HandleSendLocalVideoRequestSlot"
QT_MOC_LITERAL(46, 927, 28), // "HandleVoiceChangeRequestSlot"
QT_MOC_LITERAL(47, 956, 26), // "HandleVoiceMuteRequestSlot"
QT_MOC_LITERAL(48, 983, 6), // "isMute"
QT_MOC_LITERAL(49, 990, 28), // "HandlePermitSpeakRequestSlot"
QT_MOC_LITERAL(50, 1019, 29), // "HandleScreenLayoutRequestSlot"
QT_MOC_LITERAL(51, 1049, 40), // "HandlePanelVisibleStateChange..."
QT_MOC_LITERAL(52, 1090, 36) // "HandleReceiveAllLocalVideoNot..."

    },
    "IDisplayController\0NotifyReceiveVideoSignal\0"
    "\0vuri\0screen_index\0seat_index\0"
    "use_small_video\0NotifyStopVideoSignal\0"
    "NotifyReceiveAudioSignal\0uri\0enable\0"
    "NotifyReceiveDocumentSignal\0"
    "NotifyStopDocumentSignal\0"
    "NotifyShowRTCPMessageSignal\0show\0"
    "NotifySendLocalAudioSignal\0"
    "NotifySendLocalVideoSignal\0virtual_index\0"
    "NotifyVoiceChangeSignal\0AudioDeviceType\0"
    "type\0volume\0NotifyMuteChangeSignal\0"
    "is_mute\0NotifyTerminalSpeakSignal\0"
    "allow\0NotifyScreenLayoutChangedSignal\0"
    "IDisplayController::ScreenLayout\0"
    "layout\0SetPanelVisibleSignal\0visible\0"
    "CheckPanelVisibleStateSignal\0"
    "telecontroller_index\0HandleReceiveVideoRequestSlot\0"
    "HandleRequestStreamMediaAutoReceiveSlot\0"
    "remote_uri\0HandleStopVideoRequestSlot\0"
    "HandleReceiveAudioRequestSlot\0"
    "HandleReceiveDocumentRequestSlot\0"
    "HandleStopDocumentRequestSlot\0"
    "HandleOpenDocumentRequestSlot\0filename\0"
    "HandlePPTControlSlot\0"
    "HandleShowRTCPMessageRequestSlot\0"
    "HandleSendLocalAudioRequestSlot\0"
    "HandleSendLocalVideoRequestSlot\0"
    "HandleVoiceChangeRequestSlot\0"
    "HandleVoiceMuteRequestSlot\0isMute\0"
    "HandlePermitSpeakRequestSlot\0"
    "HandleScreenLayoutRequestSlot\0"
    "HandlePanelVisibleStateChangedNotifySlot\0"
    "HandleReceiveAllLocalVideoNotifySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IDisplayController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  169,    2, 0x06 /* Public */,
       7,    3,  178,    2, 0x06 /* Public */,
       8,    2,  185,    2, 0x06 /* Public */,
      11,    2,  190,    2, 0x06 /* Public */,
      12,    2,  195,    2, 0x06 /* Public */,
      13,    2,  200,    2, 0x06 /* Public */,
      15,    1,  205,    2, 0x06 /* Public */,
      16,    2,  208,    2, 0x06 /* Public */,
      18,    2,  213,    2, 0x06 /* Public */,
      22,    2,  218,    2, 0x06 /* Public */,
      24,    2,  223,    2, 0x06 /* Public */,
      26,    2,  228,    2, 0x06 /* Public */,
      29,    1,  233,    2, 0x06 /* Public */,
      31,    1,  236,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      33,    4,  239,    2, 0x0a /* Public */,
      34,    1,  248,    2, 0x0a /* Public */,
      36,    1,  251,    2, 0x0a /* Public */,
      37,    2,  254,    2, 0x0a /* Public */,
      38,    2,  259,    2, 0x0a /* Public */,
      39,    1,  264,    2, 0x0a /* Public */,
      40,    2,  267,    2, 0x0a /* Public */,
      42,    1,  272,    2, 0x0a /* Public */,
      43,    2,  275,    2, 0x0a /* Public */,
      44,    1,  280,    2, 0x0a /* Public */,
      45,    2,  283,    2, 0x0a /* Public */,
      46,    2,  288,    2, 0x0a /* Public */,
      47,    2,  293,    2, 0x0a /* Public */,
      49,    2,  298,    2, 0x0a /* Public */,
      50,    2,  303,    2, 0x0a /* Public */,
      51,    2,  308,    2, 0x0a /* Public */,
      52,    0,  313,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   14,    4,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   17,   10,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Int,   20,   21,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Bool,   20,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   25,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 27,    4,   28,
    QMetaType::Void, QMetaType::Bool,   30,
    QMetaType::Void, QMetaType::Int,   32,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   41,    4,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   14,    4,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   17,   10,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Int,   20,   21,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Bool,   20,   48,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   25,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 27,    4,   28,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   32,   30,
    QMetaType::Void,

       0        // eod
};

void IDisplayController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IDisplayController *_t = static_cast<IDisplayController *>(_o);
        switch (_id) {
        case 0: _t->NotifyReceiveVideoSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->NotifyStopVideoSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->NotifyReceiveAudioSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->NotifyReceiveDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->NotifyStopDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->NotifyShowRTCPMessageSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->NotifySendLocalAudioSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->NotifySendLocalVideoSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->NotifyVoiceChangeSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->NotifyMuteChangeSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->NotifyTerminalSpeakSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 11: _t->NotifyScreenLayoutChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 12: _t->SetPanelVisibleSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->CheckPanelVisibleStateSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->HandleReceiveVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 15: _t->HandleRequestStreamMediaAutoReceiveSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->HandleStopVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->HandleReceiveAudioRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 18: _t->HandleReceiveDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: _t->HandleStopDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->HandleOpenDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->HandlePPTControlSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->HandleShowRTCPMessageRequestSlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: _t->HandleSendLocalAudioRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->HandleSendLocalVideoRequestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 25: _t->HandleVoiceChangeRequestSlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 26: _t->HandleVoiceMuteRequestSlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 27: _t->HandlePermitSpeakRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 28: _t->HandleScreenLayoutRequestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 29: _t->HandlePanelVisibleStateChangedNotifySlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 30: _t->HandleReceiveAllLocalVideoNotifySlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IDisplayController::*_t)(const QString & , int , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyReceiveVideoSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (IDisplayController::*_t)(const QString & , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyStopVideoSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (IDisplayController::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyReceiveAudioSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (IDisplayController::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyReceiveDocumentSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (IDisplayController::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyStopDocumentSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (IDisplayController::*_t)(bool , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyShowRTCPMessageSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (IDisplayController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifySendLocalAudioSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (IDisplayController::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifySendLocalVideoSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (IDisplayController::*_t)(AudioDeviceType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyVoiceChangeSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (IDisplayController::*_t)(AudioDeviceType , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyMuteChangeSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (IDisplayController::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyTerminalSpeakSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (IDisplayController::*_t)(int , IDisplayController::ScreenLayout );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::NotifyScreenLayoutChangedSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (IDisplayController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::SetPanelVisibleSignal)) {
                *result = 12;
            }
        }
        {
            typedef void (IDisplayController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IDisplayController::CheckPanelVisibleStateSignal)) {
                *result = 13;
            }
        }
    }
}

const QMetaObject IDisplayController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IDisplayController.data,
      qt_meta_data_IDisplayController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IDisplayController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IDisplayController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IDisplayController.stringdata))
        return static_cast<void*>(const_cast< IDisplayController*>(this));
    return QObject::qt_metacast(_clname);
}

int IDisplayController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void IDisplayController::NotifyReceiveVideoSignal(const QString & _t1, int _t2, int _t3, bool _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IDisplayController::NotifyStopVideoSignal(const QString & _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IDisplayController::NotifyReceiveAudioSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IDisplayController::NotifyReceiveDocumentSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void IDisplayController::NotifyStopDocumentSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void IDisplayController::NotifyShowRTCPMessageSignal(bool _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void IDisplayController::NotifySendLocalAudioSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void IDisplayController::NotifySendLocalVideoSignal(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void IDisplayController::NotifyVoiceChangeSignal(AudioDeviceType _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void IDisplayController::NotifyMuteChangeSignal(AudioDeviceType _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void IDisplayController::NotifyTerminalSpeakSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void IDisplayController::NotifyScreenLayoutChangedSignal(int _t1, IDisplayController::ScreenLayout _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void IDisplayController::SetPanelVisibleSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void IDisplayController::CheckPanelVisibleStateSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_END_MOC_NAMESPACE
