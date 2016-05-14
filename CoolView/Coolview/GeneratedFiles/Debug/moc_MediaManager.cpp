/****************************************************************************
** Meta object code from reading C++ file 'MediaManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../util/MediaManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MediaManager_t {
    QByteArrayData data[12];
    char stringdata[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaManager_t qt_meta_stringdata_MediaManager = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MediaManager"
QT_MOC_LITERAL(1, 13, 27), // "NotifySendMediaStateChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 8), // "media_id"
QT_MOC_LITERAL(4, 51, 4), // "type"
QT_MOC_LITERAL(5, 56, 27), // "NotifyRecvMediaStateChanged"
QT_MOC_LITERAL(6, 84, 7), // "user_id"
QT_MOC_LITERAL(7, 92, 22), // "rtcpReportTimerTimeout"
QT_MOC_LITERAL(8, 115, 31), // "HandleNofityChannelStateChanged"
QT_MOC_LITERAL(9, 147, 10), // "channel_id"
QT_MOC_LITERAL(10, 158, 12), // "channel_type"
QT_MOC_LITERAL(11, 171, 13) // "channel_state"

    },
    "MediaManager\0NotifySendMediaStateChanged\0"
    "\0media_id\0type\0NotifyRecvMediaStateChanged\0"
    "user_id\0rtcpReportTimerTimeout\0"
    "HandleNofityChannelStateChanged\0"
    "channel_id\0channel_type\0channel_state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaManager[] = {

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
       5,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   44,    2, 0x08 /* Private */,
       8,    4,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    9,    6,   10,   11,

       0        // eod
};

void MediaManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MediaManager *_t = static_cast<MediaManager *>(_o);
        switch (_id) {
        case 0: _t->NotifySendMediaStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->NotifyRecvMediaStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->rtcpReportTimerTimeout(); break;
        case 3: _t->HandleNofityChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MediaManager::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaManager::NotifySendMediaStateChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MediaManager::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaManager::NotifyRecvMediaStateChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MediaManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MediaManager.data,
      qt_meta_data_MediaManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MediaManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MediaManager.stringdata))
        return static_cast<void*>(const_cast< MediaManager*>(this));
    return QObject::qt_metacast(_clname);
}

int MediaManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void MediaManager::NotifySendMediaStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MediaManager::NotifyRecvMediaStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
