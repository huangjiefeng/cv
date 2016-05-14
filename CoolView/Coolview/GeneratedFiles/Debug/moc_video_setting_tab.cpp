/****************************************************************************
** Meta object code from reading C++ file 'video_setting_tab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../video_setting_tab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'video_setting_tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VideoSettingTab_t {
    QByteArrayData data[15];
    char stringdata[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoSettingTab_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoSettingTab_t qt_meta_stringdata_VideoSettingTab = {
    {
QT_MOC_LITERAL(0, 0, 15), // "VideoSettingTab"
QT_MOC_LITERAL(1, 16, 10), // "ReadConfig"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "SaveConfig"
QT_MOC_LITERAL(4, 39, 17), // "ChangeCameraIndex"
QT_MOC_LITERAL(5, 57, 5), // "index"
QT_MOC_LITERAL(6, 63, 22), // "cameraIndexChangedSlot"
QT_MOC_LITERAL(7, 86, 8), // "indexStr"
QT_MOC_LITERAL(8, 95, 17), // "webcamChangedSlot"
QT_MOC_LITERAL(9, 113, 10), // "webcamName"
QT_MOC_LITERAL(10, 124, 19), // "crossbarChangedSlot"
QT_MOC_LITERAL(11, 144, 8), // "crossbar"
QT_MOC_LITERAL(12, 153, 30), // "ckbRecvAutoResync_StateChanged"
QT_MOC_LITERAL(13, 184, 5), // "state"
QT_MOC_LITERAL(14, 190, 32) // "ckbAdaptiveCodeRate_StateChanged"

    },
    "VideoSettingTab\0ReadConfig\0\0SaveConfig\0"
    "ChangeCameraIndex\0index\0cameraIndexChangedSlot\0"
    "indexStr\0webcamChangedSlot\0webcamName\0"
    "crossbarChangedSlot\0crossbar\0"
    "ckbRecvAutoResync_StateChanged\0state\0"
    "ckbAdaptiveCodeRate_StateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoSettingTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    1,   56,    2, 0x0a /* Public */,
       6,    1,   59,    2, 0x08 /* Private */,
       8,    1,   62,    2, 0x08 /* Private */,
      10,    1,   65,    2, 0x08 /* Private */,
      12,    1,   68,    2, 0x08 /* Private */,
      14,    1,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void VideoSettingTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoSettingTab *_t = static_cast<VideoSettingTab *>(_o);
        switch (_id) {
        case 0: _t->ReadConfig(); break;
        case 1: _t->SaveConfig(); break;
        case 2: _t->ChangeCameraIndex((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->cameraIndexChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->webcamChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->crossbarChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->ckbRecvAutoResync_StateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ckbAdaptiveCodeRate_StateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VideoSettingTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VideoSettingTab.data,
      qt_meta_data_VideoSettingTab,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VideoSettingTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoSettingTab::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VideoSettingTab.stringdata))
        return static_cast<void*>(const_cast< VideoSettingTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int VideoSettingTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
