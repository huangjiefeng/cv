/****************************************************************************
** Meta object code from reading C++ file 'speaker_volume_control_button.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../speaker_volume_control_button.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'speaker_volume_control_button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SpeakerVolumeControlButton_t {
    QByteArrayData data[10];
    char stringdata[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpeakerVolumeControlButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpeakerVolumeControlButton_t qt_meta_stringdata_SpeakerVolumeControlButton = {
    {
QT_MOC_LITERAL(0, 0, 26), // "SpeakerVolumeControlButton"
QT_MOC_LITERAL(1, 27, 29), // "RequestControlVoiceMuteSignal"
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 15), // "AudioDeviceType"
QT_MOC_LITERAL(4, 74, 4), // "type"
QT_MOC_LITERAL(5, 79, 7), // "is_mute"
QT_MOC_LITERAL(6, 87, 26), // "DisplaySpeakerConfigSignal"
QT_MOC_LITERAL(7, 114, 26), // "HandleMuteChangeNotifySlot"
QT_MOC_LITERAL(8, 141, 23), // "HandleButtonClickedSlot"
QT_MOC_LITERAL(9, 165, 7) // "checked"

    },
    "SpeakerVolumeControlButton\0"
    "RequestControlVoiceMuteSignal\0\0"
    "AudioDeviceType\0type\0is_mute\0"
    "DisplaySpeakerConfigSignal\0"
    "HandleMuteChangeNotifySlot\0"
    "HandleButtonClickedSlot\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpeakerVolumeControlButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   40,    2, 0x0a /* Public */,
       8,    1,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void SpeakerVolumeControlButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SpeakerVolumeControlButton *_t = static_cast<SpeakerVolumeControlButton *>(_o);
        switch (_id) {
        case 0: _t->RequestControlVoiceMuteSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->DisplaySpeakerConfigSignal(); break;
        case 2: _t->HandleMuteChangeNotifySlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->HandleButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SpeakerVolumeControlButton::*_t)(AudioDeviceType , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpeakerVolumeControlButton::RequestControlVoiceMuteSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SpeakerVolumeControlButton::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpeakerVolumeControlButton::DisplaySpeakerConfigSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject SpeakerVolumeControlButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_SpeakerVolumeControlButton.data,
      qt_meta_data_SpeakerVolumeControlButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SpeakerVolumeControlButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpeakerVolumeControlButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SpeakerVolumeControlButton.stringdata))
        return static_cast<void*>(const_cast< SpeakerVolumeControlButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int SpeakerVolumeControlButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SpeakerVolumeControlButton::RequestControlVoiceMuteSignal(AudioDeviceType _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SpeakerVolumeControlButton::DisplaySpeakerConfigSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
