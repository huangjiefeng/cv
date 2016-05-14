/****************************************************************************
** Meta object code from reading C++ file 'local_speaker_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../local_speaker_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'local_speaker_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LocalSpeakerWidget_t {
    QByteArrayData data[10];
    char stringdata[164];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocalSpeakerWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocalSpeakerWidget_t qt_meta_stringdata_LocalSpeakerWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "LocalSpeakerWidget"
QT_MOC_LITERAL(1, 19, 23), // "ChangeVoiceVolumeSignal"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 15), // "AudioDeviceType"
QT_MOC_LITERAL(4, 60, 4), // "type"
QT_MOC_LITERAL(5, 65, 6), // "volume"
QT_MOC_LITERAL(6, 72, 28), // "HandleVoiceChangedNotifySlot"
QT_MOC_LITERAL(7, 101, 29), // "HandleVoiceChangedRequestSlot"
QT_MOC_LITERAL(8, 131, 27), // "ShowVolumeSettingWidgetSlot"
QT_MOC_LITERAL(9, 159, 4) // "show"

    },
    "LocalSpeakerWidget\0ChangeVoiceVolumeSignal\0"
    "\0AudioDeviceType\0type\0volume\0"
    "HandleVoiceChangedNotifySlot\0"
    "HandleVoiceChangedRequestSlot\0"
    "ShowVolumeSettingWidgetSlot\0show"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocalSpeakerWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   39,    2, 0x08 /* Private */,
       7,    1,   44,    2, 0x08 /* Private */,
       8,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void LocalSpeakerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LocalSpeakerWidget *_t = static_cast<LocalSpeakerWidget *>(_o);
        switch (_id) {
        case 0: _t->ChangeVoiceVolumeSignal((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->HandleVoiceChangedNotifySlot((*reinterpret_cast< AudioDeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->HandleVoiceChangedRequestSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ShowVolumeSettingWidgetSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LocalSpeakerWidget::*_t)(AudioDeviceType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalSpeakerWidget::ChangeVoiceVolumeSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject LocalSpeakerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LocalSpeakerWidget.data,
      qt_meta_data_LocalSpeakerWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LocalSpeakerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalSpeakerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LocalSpeakerWidget.stringdata))
        return static_cast<void*>(const_cast< LocalSpeakerWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int LocalSpeakerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void LocalSpeakerWidget::ChangeVoiceVolumeSignal(AudioDeviceType _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
