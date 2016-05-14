/****************************************************************************
** Meta object code from reading C++ file 'video_send_control_button.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../video_send_control_button.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'video_send_control_button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VideoSendControlButton_t {
    QByteArrayData data[9];
    char stringdata[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoSendControlButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoSendControlButton_t qt_meta_stringdata_VideoSendControlButton = {
    {
QT_MOC_LITERAL(0, 0, 22), // "VideoSendControlButton"
QT_MOC_LITERAL(1, 23, 29), // "RequestControlVideoSendSignal"
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 13), // "virtual_index"
QT_MOC_LITERAL(4, 68, 4), // "send"
QT_MOC_LITERAL(5, 73, 18), // "ConfigCameraSignal"
QT_MOC_LITERAL(6, 92, 28), // "HandleVideoButtonClickedSlot"
QT_MOC_LITERAL(7, 121, 7), // "checked"
QT_MOC_LITERAL(8, 129, 32) // "HandleControlVideoSendNotifySlot"

    },
    "VideoSendControlButton\0"
    "RequestControlVideoSendSignal\0\0"
    "virtual_index\0send\0ConfigCameraSignal\0"
    "HandleVideoButtonClickedSlot\0checked\0"
    "HandleControlVideoSendNotifySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoSendControlButton[] = {

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
       5,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   42,    2, 0x08 /* Private */,
       8,    2,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,

       0        // eod
};

void VideoSendControlButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoSendControlButton *_t = static_cast<VideoSendControlButton *>(_o);
        switch (_id) {
        case 0: _t->RequestControlVideoSendSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->ConfigCameraSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->HandleVideoButtonClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->HandleControlVideoSendNotifySlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VideoSendControlButton::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoSendControlButton::RequestControlVideoSendSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (VideoSendControlButton::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoSendControlButton::ConfigCameraSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject VideoSendControlButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_VideoSendControlButton.data,
      qt_meta_data_VideoSendControlButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VideoSendControlButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoSendControlButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VideoSendControlButton.stringdata))
        return static_cast<void*>(const_cast< VideoSendControlButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int VideoSendControlButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void VideoSendControlButton::RequestControlVideoSendSignal(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoSendControlButton::ConfigCameraSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
