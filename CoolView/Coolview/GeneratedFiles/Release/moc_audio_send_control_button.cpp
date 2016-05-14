/****************************************************************************
** Meta object code from reading C++ file 'audio_send_control_button.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../audio_send_control_button.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audio_send_control_button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioSendControlButton_t {
    QByteArrayData data[8];
    char stringdata[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioSendControlButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioSendControlButton_t qt_meta_stringdata_AudioSendControlButton = {
    {
QT_MOC_LITERAL(0, 0, 22), // "AudioSendControlButton"
QT_MOC_LITERAL(1, 23, 29), // "RequestControlAudioSendSignal"
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 4), // "send"
QT_MOC_LITERAL(4, 59, 22), // "DisplayMicConfigSignal"
QT_MOC_LITERAL(5, 82, 32), // "HandleControlAudioSendNotifySlot"
QT_MOC_LITERAL(6, 115, 23), // "HandleButtonClickedSlot"
QT_MOC_LITERAL(7, 139, 7) // "checked"

    },
    "AudioSendControlButton\0"
    "RequestControlAudioSendSignal\0\0send\0"
    "DisplayMicConfigSignal\0"
    "HandleControlAudioSendNotifySlot\0"
    "HandleButtonClickedSlot\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioSendControlButton[] = {

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
       1,    1,   34,    2, 0x06 /* Public */,
       4,    0,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   38,    2, 0x0a /* Public */,
       6,    1,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    7,

       0        // eod
};

void AudioSendControlButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioSendControlButton *_t = static_cast<AudioSendControlButton *>(_o);
        switch (_id) {
        case 0: _t->RequestControlAudioSendSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->DisplayMicConfigSignal(); break;
        case 2: _t->HandleControlAudioSendNotifySlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->HandleButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioSendControlButton::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioSendControlButton::RequestControlAudioSendSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (AudioSendControlButton::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioSendControlButton::DisplayMicConfigSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject AudioSendControlButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_AudioSendControlButton.data,
      qt_meta_data_AudioSendControlButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AudioSendControlButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioSendControlButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AudioSendControlButton.stringdata))
        return static_cast<void*>(const_cast< AudioSendControlButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int AudioSendControlButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AudioSendControlButton::RequestControlAudioSendSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AudioSendControlButton::DisplayMicConfigSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
