/****************************************************************************
** Meta object code from reading C++ file 'CvCoreService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/core/service/CvCoreService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvCoreService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CoreService2_t {
    QByteArrayData data[60];
    char stringdata[952];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CoreService2_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CoreService2_t qt_meta_stringdata_CoreService2 = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CoreService2"
QT_MOC_LITERAL(1, 13, 14), // "ShutDownSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "RestartSignal"
QT_MOC_LITERAL(4, 43, 13), // "SendQoSSignal"
QT_MOC_LITERAL(5, 57, 11), // "RtpStatItem"
QT_MOC_LITERAL(6, 69, 4), // "item"
QT_MOC_LITERAL(7, 74, 20), // "JoinConferenceSignal"
QT_MOC_LITERAL(8, 95, 14), // "conference_uri"
QT_MOC_LITERAL(9, 110, 21), // "LeaveConferenceSignal"
QT_MOC_LITERAL(10, 132, 27), // "CreateSendScreenMediaSignal"
QT_MOC_LITERAL(11, 160, 3), // "wnd"
QT_MOC_LITERAL(12, 164, 27), // "ControlDocumentSharedSignal"
QT_MOC_LITERAL(13, 192, 6), // "enable"
QT_MOC_LITERAL(14, 199, 23), // "SegmentLocalVideoSignal"
QT_MOC_LITERAL(15, 223, 25), // "RecoverMediaProcessSignal"
QT_MOC_LITERAL(16, 249, 15), // "MediaStreamType"
QT_MOC_LITERAL(17, 265, 10), // "media_type"
QT_MOC_LITERAL(18, 276, 7), // "is_send"
QT_MOC_LITERAL(19, 284, 14), // "remote_user_id"
QT_MOC_LITERAL(20, 299, 18), // "ReceiveVideoSignal"
QT_MOC_LITERAL(21, 318, 4), // "vuri"
QT_MOC_LITERAL(22, 323, 12), // "screen_index"
QT_MOC_LITERAL(23, 336, 10), // "seat_index"
QT_MOC_LITERAL(24, 347, 15), // "use_small_video"
QT_MOC_LITERAL(25, 363, 15), // "StopVideoSignal"
QT_MOC_LITERAL(26, 379, 25), // "ControlAudioReceiveSignal"
QT_MOC_LITERAL(27, 405, 3), // "uri"
QT_MOC_LITERAL(28, 409, 7), // "receive"
QT_MOC_LITERAL(29, 417, 23), // "ChangeVoiceVolumeSignal"
QT_MOC_LITERAL(30, 441, 15), // "AudioDeviceType"
QT_MOC_LITERAL(31, 457, 4), // "type"
QT_MOC_LITERAL(32, 462, 6), // "volume"
QT_MOC_LITERAL(33, 469, 27), // "ControlSendLocalAudioSignal"
QT_MOC_LITERAL(34, 497, 27), // "ControlSendLocalVideoSignal"
QT_MOC_LITERAL(35, 525, 13), // "virtual_index"
QT_MOC_LITERAL(36, 539, 18), // "OpenDocumentSignal"
QT_MOC_LITERAL(37, 558, 8), // "filename"
QT_MOC_LITERAL(38, 567, 21), // "ReceiveDocumentSignal"
QT_MOC_LITERAL(39, 589, 18), // "StopDocumentSignal"
QT_MOC_LITERAL(40, 608, 16), // "ControlPPTSignal"
QT_MOC_LITERAL(41, 625, 23), // "ChangeSceenLayoutSignal"
QT_MOC_LITERAL(42, 649, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(43, 682, 6), // "layout"
QT_MOC_LITERAL(44, 689, 15), // "ShowPanelSignal"
QT_MOC_LITERAL(45, 705, 7), // "visible"
QT_MOC_LITERAL(46, 713, 26), // "GetPanelVisibleStateSignal"
QT_MOC_LITERAL(47, 740, 17), // "telecontroller_id"
QT_MOC_LITERAL(48, 758, 22), // "StartUDPReceiverSignal"
QT_MOC_LITERAL(49, 781, 21), // "StopUDPReceiverSignal"
QT_MOC_LITERAL(50, 803, 22), // "NetworkBandwidthSignal"
QT_MOC_LITERAL(51, 826, 12), // "in_bandwidth"
QT_MOC_LITERAL(52, 839, 13), // "out_bandwidth"
QT_MOC_LITERAL(53, 853, 24), // "ControlVideoRecordSignal"
QT_MOC_LITERAL(54, 878, 7), // "user_id"
QT_MOC_LITERAL(55, 886, 5), // "param"
QT_MOC_LITERAL(56, 892, 11), // "TeleCommand"
QT_MOC_LITERAL(57, 904, 13), // "command_index"
QT_MOC_LITERAL(58, 918, 12), // "intput_array"
QT_MOC_LITERAL(59, 931, 20) // "MoveToMainThreadSlot"

    },
    "CoreService2\0ShutDownSignal\0\0RestartSignal\0"
    "SendQoSSignal\0RtpStatItem\0item\0"
    "JoinConferenceSignal\0conference_uri\0"
    "LeaveConferenceSignal\0CreateSendScreenMediaSignal\0"
    "wnd\0ControlDocumentSharedSignal\0enable\0"
    "SegmentLocalVideoSignal\0"
    "RecoverMediaProcessSignal\0MediaStreamType\0"
    "media_type\0is_send\0remote_user_id\0"
    "ReceiveVideoSignal\0vuri\0screen_index\0"
    "seat_index\0use_small_video\0StopVideoSignal\0"
    "ControlAudioReceiveSignal\0uri\0receive\0"
    "ChangeVoiceVolumeSignal\0AudioDeviceType\0"
    "type\0volume\0ControlSendLocalAudioSignal\0"
    "ControlSendLocalVideoSignal\0virtual_index\0"
    "OpenDocumentSignal\0filename\0"
    "ReceiveDocumentSignal\0StopDocumentSignal\0"
    "ControlPPTSignal\0ChangeSceenLayoutSignal\0"
    "IDisplayController::ScreenLayout\0"
    "layout\0ShowPanelSignal\0visible\0"
    "GetPanelVisibleStateSignal\0telecontroller_id\0"
    "StartUDPReceiverSignal\0StopUDPReceiverSignal\0"
    "NetworkBandwidthSignal\0in_bandwidth\0"
    "out_bandwidth\0ControlVideoRecordSignal\0"
    "user_id\0param\0TeleCommand\0command_index\0"
    "intput_array\0MoveToMainThreadSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CoreService2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      26,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  154,    2, 0x06 /* Public */,
       3,    0,  155,    2, 0x06 /* Public */,
       4,    1,  156,    2, 0x06 /* Public */,
       7,    1,  159,    2, 0x06 /* Public */,
       9,    0,  162,    2, 0x06 /* Public */,
      10,    1,  163,    2, 0x06 /* Public */,
      12,    1,  166,    2, 0x06 /* Public */,
      14,    0,  169,    2, 0x06 /* Public */,
      15,    3,  170,    2, 0x06 /* Public */,
      20,    4,  177,    2, 0x06 /* Public */,
      25,    1,  186,    2, 0x06 /* Public */,
      26,    2,  189,    2, 0x06 /* Public */,
      29,    2,  194,    2, 0x06 /* Public */,
      33,    1,  199,    2, 0x06 /* Public */,
      34,    2,  202,    2, 0x06 /* Public */,
      36,    2,  207,    2, 0x06 /* Public */,
      38,    2,  212,    2, 0x06 /* Public */,
      39,    1,  217,    2, 0x06 /* Public */,
      40,    1,  220,    2, 0x06 /* Public */,
      41,    2,  223,    2, 0x06 /* Public */,
      44,    1,  228,    2, 0x06 /* Public */,
      46,    1,  231,    2, 0x06 /* Public */,
      48,    0,  234,    2, 0x06 /* Public */,
      49,    0,  235,    2, 0x06 /* Public */,
      50,    2,  236,    2, 0x06 /* Public */,
      53,    3,  241,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      56,    2,  248,    2, 0x0a /* Public */,
      59,    0,  253,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16, QMetaType::Bool, QMetaType::QString,   17,   18,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,   21,   22,   23,   24,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   27,   28,
    QMetaType::Void, 0x80000000 | 30, QMetaType::Int,   31,   32,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   35,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   37,   22,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   27,   22,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 42,   22,   43,
    QMetaType::Void, QMetaType::Bool,   45,
    QMetaType::Void, QMetaType::Int,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   51,   52,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   54,   31,   55,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   57,   58,
    QMetaType::Void,

       0        // eod
};

void CoreService2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CoreService2 *_t = static_cast<CoreService2 *>(_o);
        switch (_id) {
        case 0: _t->ShutDownSignal(); break;
        case 1: _t->RestartSignal(); break;
        case 2: _t->SendQoSSignal((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 3: _t->JoinConferenceSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->LeaveConferenceSignal(); break;
        case 5: _t->CreateSendScreenMediaSignal((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 6: _t->ControlDocumentSharedSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->SegmentLocalVideoSignal(); break;
        case 8: _t->RecoverMediaProcessSignal((*reinterpret_cast< MediaStreamType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: _t->ReceiveVideoSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 10: _t->StopVideoSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->ControlAudioReceiveSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->ChangeVoiceVolumeSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->ControlSendLocalAudioSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->ControlSendLocalVideoSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: _t->OpenDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->ReceiveDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->StopDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->ControlPPTSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->ChangeSceenLayoutSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 20: _t->ShowPanelSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->GetPanelVisibleStateSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->StartUDPReceiverSignal(); break;
        case 23: _t->StopUDPReceiverSignal(); break;
        case 24: _t->NetworkBandwidthSignal((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 25: _t->ControlVideoRecordSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 26: _t->TeleCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 27: _t->MoveToMainThreadSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ShutDownSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::RestartSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (CoreService2::*_t)(const RtpStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::SendQoSSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::JoinConferenceSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::LeaveConferenceSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QRect & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::CreateSendScreenMediaSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (CoreService2::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlDocumentSharedSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::SegmentLocalVideoSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (CoreService2::*_t)(MediaStreamType , bool , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::RecoverMediaProcessSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & , int , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ReceiveVideoSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::StopVideoSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlAudioReceiveSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (CoreService2::*_t)(AudioDeviceType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ChangeVoiceVolumeSignal)) {
                *result = 12;
            }
        }
        {
            typedef void (CoreService2::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlSendLocalAudioSignal)) {
                *result = 13;
            }
        }
        {
            typedef void (CoreService2::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlSendLocalVideoSignal)) {
                *result = 14;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::OpenDocumentSignal)) {
                *result = 15;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ReceiveDocumentSignal)) {
                *result = 16;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::StopDocumentSignal)) {
                *result = 17;
            }
        }
        {
            typedef void (CoreService2::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlPPTSignal)) {
                *result = 18;
            }
        }
        {
            typedef void (CoreService2::*_t)(int , IDisplayController::ScreenLayout );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ChangeSceenLayoutSignal)) {
                *result = 19;
            }
        }
        {
            typedef void (CoreService2::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ShowPanelSignal)) {
                *result = 20;
            }
        }
        {
            typedef void (CoreService2::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::GetPanelVisibleStateSignal)) {
                *result = 21;
            }
        }
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::StartUDPReceiverSignal)) {
                *result = 22;
            }
        }
        {
            typedef void (CoreService2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::StopUDPReceiverSignal)) {
                *result = 23;
            }
        }
        {
            typedef void (CoreService2::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::NetworkBandwidthSignal)) {
                *result = 24;
            }
        }
        {
            typedef void (CoreService2::*_t)(const QString & , int , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoreService2::ControlVideoRecordSignal)) {
                *result = 25;
            }
        }
    }
}

const QMetaObject CoreService2::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CoreService2.data,
      qt_meta_data_CoreService2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CoreService2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoreService2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CoreService2.stringdata))
        return static_cast<void*>(const_cast< CoreService2*>(this));
    return QObject::qt_metacast(_clname);
}

int CoreService2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void CoreService2::ShutDownSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CoreService2::RestartSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CoreService2::SendQoSSignal(const RtpStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CoreService2::JoinConferenceSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CoreService2::LeaveConferenceSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void CoreService2::CreateSendScreenMediaSignal(const QRect & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CoreService2::ControlDocumentSharedSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CoreService2::SegmentLocalVideoSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void CoreService2::RecoverMediaProcessSignal(MediaStreamType _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CoreService2::ReceiveVideoSignal(const QString & _t1, int _t2, int _t3, bool _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CoreService2::StopVideoSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CoreService2::ControlAudioReceiveSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CoreService2::ChangeVoiceVolumeSignal(AudioDeviceType _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CoreService2::ControlSendLocalAudioSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CoreService2::ControlSendLocalVideoSignal(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CoreService2::OpenDocumentSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CoreService2::ReceiveDocumentSignal(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CoreService2::StopDocumentSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CoreService2::ControlPPTSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CoreService2::ChangeSceenLayoutSignal(int _t1, IDisplayController::ScreenLayout _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CoreService2::ShowPanelSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CoreService2::GetPanelVisibleStateSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CoreService2::StartUDPReceiverSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 22, Q_NULLPTR);
}

// SIGNAL 23
void CoreService2::StopUDPReceiverSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 23, Q_NULLPTR);
}

// SIGNAL 24
void CoreService2::NetworkBandwidthSignal(double _t1, double _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CoreService2::ControlVideoRecordSignal(const QString & _t1, int _t2, QByteArray _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}
QT_END_MOC_NAMESPACE
