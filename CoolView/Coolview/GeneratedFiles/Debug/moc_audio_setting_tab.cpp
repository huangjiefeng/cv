/****************************************************************************
** Meta object code from reading C++ file 'audio_setting_tab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../audio_setting_tab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audio_setting_tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioSettingTab_t {
    QByteArrayData data[17];
    char stringdata[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioSettingTab_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioSettingTab_t qt_meta_stringdata_AudioSettingTab = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AudioSettingTab"
QT_MOC_LITERAL(1, 16, 25), // "RequestChangeVolumeSignal"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 15), // "AudioDeviceType"
QT_MOC_LITERAL(4, 59, 4), // "type"
QT_MOC_LITERAL(5, 64, 6), // "volume"
QT_MOC_LITERAL(6, 71, 10), // "ReadConfig"
QT_MOC_LITERAL(7, 82, 10), // "SaveConfig"
QT_MOC_LITERAL(8, 93, 19), // "outputVolumnChanged"
QT_MOC_LITERAL(9, 113, 5), // "index"
QT_MOC_LITERAL(10, 119, 18), // "inputVolumnChanged"
QT_MOC_LITERAL(11, 138, 21), // "audioCodecChangedSlot"
QT_MOC_LITERAL(12, 160, 5), // "codec"
QT_MOC_LITERAL(13, 166, 14), // "aecChangedSlot"
QT_MOC_LITERAL(14, 181, 29), // "HandleOutputVolumeRequestSlot"
QT_MOC_LITERAL(15, 211, 28), // "HandleInputVolumeRequestSlot"
QT_MOC_LITERAL(16, 240, 29) // "HandleVolumeChangedNotifySlot"

    },
    "AudioSettingTab\0RequestChangeVolumeSignal\0"
    "\0AudioDeviceType\0type\0volume\0ReadConfig\0"
    "SaveConfig\0outputVolumnChanged\0index\0"
    "inputVolumnChanged\0audioCodecChangedSlot\0"
    "codec\0aecChangedSlot\0HandleOutputVolumeRequestSlot\0"
    "HandleInputVolumeRequestSlot\0"
    "HandleVolumeChangedNotifySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioSettingTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   69,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    1,   71,    2, 0x08 /* Private */,
      10,    1,   74,    2, 0x08 /* Private */,
      11,    1,   77,    2, 0x08 /* Private */,
      13,    1,   80,    2, 0x08 /* Private */,
      14,    1,   83,    2, 0x08 /* Private */,
      15,    1,   86,    2, 0x08 /* Private */,
      16,    2,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

       0        // eod
};

void AudioSettingTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioSettingTab *_t = static_cast<AudioSettingTab *>(_o);
        switch (_id) {
        case 0: _t->RequestChangeVolumeSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->ReadConfig(); break;
        case 2: _t->SaveConfig(); break;
        case 3: _t->outputVolumnChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->inputVolumnChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->audioCodecChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->aecChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->HandleOutputVolumeRequestSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->HandleInputVolumeRequestSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->HandleVolumeChangedNotifySlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioSettingTab::*_t)(AudioDeviceType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioSettingTab::RequestChangeVolumeSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject AudioSettingTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AudioSettingTab.data,
      qt_meta_data_AudioSettingTab,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AudioSettingTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioSettingTab::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AudioSettingTab.stringdata))
        return static_cast<void*>(const_cast< AudioSettingTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int AudioSettingTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AudioSettingTab::RequestChangeVolumeSignal(AudioDeviceType _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
