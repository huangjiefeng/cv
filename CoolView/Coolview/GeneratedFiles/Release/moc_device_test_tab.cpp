/****************************************************************************
** Meta object code from reading C++ file 'device_test_tab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../device_test_tab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'device_test_tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DeviceTestTab_t {
    QByteArrayData data[23];
    char stringdata[392];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceTestTab_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceTestTab_t qt_meta_stringdata_DeviceTestTab = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DeviceTestTab"
QT_MOC_LITERAL(1, 14, 16), // "serverTestSignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 24), // "preViewWebcamChangedSlot"
QT_MOC_LITERAL(4, 57, 10), // "webcamName"
QT_MOC_LITERAL(5, 68, 26), // "preViewCrossbarChangedSlot"
QT_MOC_LITERAL(6, 95, 8), // "crossbar"
QT_MOC_LITERAL(7, 104, 16), // "videoPreViewSlot"
QT_MOC_LITERAL(8, 121, 19), // "startAudioInputTest"
QT_MOC_LITERAL(9, 141, 20), // "startAudioOutputTest"
QT_MOC_LITERAL(10, 162, 18), // "stopAudioInputTest"
QT_MOC_LITERAL(11, 181, 19), // "stopAudioOutputTest"
QT_MOC_LITERAL(12, 201, 19), // "serverTestStartSlot"
QT_MOC_LITERAL(13, 221, 24), // "recvServerTestResultSlot"
QT_MOC_LITERAL(14, 246, 9), // "isTimeOut"
QT_MOC_LITERAL(15, 256, 8), // "interval"
QT_MOC_LITERAL(16, 265, 28), // "compressANDUploadLogFileSlot"
QT_MOC_LITERAL(17, 294, 24), // "OnUploadStateChangedSlot"
QT_MOC_LITERAL(18, 319, 22), // "CUploader::UploadState"
QT_MOC_LITERAL(19, 342, 5), // "state"
QT_MOC_LITERAL(20, 348, 3), // "msg"
QT_MOC_LITERAL(21, 352, 21), // "OnZipStateChangedSlot"
QT_MOC_LITERAL(22, 374, 17) // "CZipDir::ZipState"

    },
    "DeviceTestTab\0serverTestSignal\0\0"
    "preViewWebcamChangedSlot\0webcamName\0"
    "preViewCrossbarChangedSlot\0crossbar\0"
    "videoPreViewSlot\0startAudioInputTest\0"
    "startAudioOutputTest\0stopAudioInputTest\0"
    "stopAudioOutputTest\0serverTestStartSlot\0"
    "recvServerTestResultSlot\0isTimeOut\0"
    "interval\0compressANDUploadLogFileSlot\0"
    "OnUploadStateChangedSlot\0"
    "CUploader::UploadState\0state\0msg\0"
    "OnZipStateChangedSlot\0CZipDir::ZipState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceTestTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   80,    2, 0x08 /* Private */,
       5,    1,   83,    2, 0x08 /* Private */,
       7,    0,   86,    2, 0x08 /* Private */,
       8,    0,   87,    2, 0x08 /* Private */,
       9,    0,   88,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x08 /* Private */,
      13,    2,   92,    2, 0x08 /* Private */,
      16,    0,   97,    2, 0x08 /* Private */,
      17,    2,   98,    2, 0x08 /* Private */,
      21,    2,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   14,   15,
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 18, QMetaType::QString,   19,   20,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString,   19,   20,

       0        // eod
};

void DeviceTestTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceTestTab *_t = static_cast<DeviceTestTab *>(_o);
        switch (_id) {
        case 0: _t->serverTestSignal(); break;
        case 1: _t->preViewWebcamChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->preViewCrossbarChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->videoPreViewSlot(); break;
        case 4: _t->startAudioInputTest(); break;
        case 5: _t->startAudioOutputTest(); break;
        case 6: _t->stopAudioInputTest(); break;
        case 7: _t->stopAudioOutputTest(); break;
        case 8: _t->serverTestStartSlot(); break;
        case 9: _t->recvServerTestResultSlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: { bool _r = _t->compressANDUploadLogFileSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->OnUploadStateChangedSlot((*reinterpret_cast< CUploader::UploadState(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 12: _t->OnZipStateChangedSlot((*reinterpret_cast< CZipDir::ZipState(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DeviceTestTab::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DeviceTestTab::serverTestSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DeviceTestTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DeviceTestTab.data,
      qt_meta_data_DeviceTestTab,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DeviceTestTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceTestTab::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceTestTab.stringdata))
        return static_cast<void*>(const_cast< DeviceTestTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int DeviceTestTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void DeviceTestTab::serverTestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
