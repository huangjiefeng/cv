/****************************************************************************
** Meta object code from reading C++ file 'ChannelDispatcherIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/dbus/channelDispatcher/client/ChannelDispatcherIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChannelDispatcherIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChannelDispatcherIf_t {
    QByteArrayData data[15];
    char stringdata[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChannelDispatcherIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChannelDispatcherIf_t qt_meta_stringdata_ChannelDispatcherIf = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ChannelDispatcherIf"
QT_MOC_LITERAL(1, 20, 25), // "NofityChannelStateChanged"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 10), // "channel_id"
QT_MOC_LITERAL(4, 58, 7), // "user_id"
QT_MOC_LITERAL(5, 66, 12), // "channel_type"
QT_MOC_LITERAL(6, 79, 13), // "channel_state"
QT_MOC_LITERAL(7, 93, 19), // "ChannelStateChanged"
QT_MOC_LITERAL(8, 113, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(9, 133, 13), // "CreateChannel"
QT_MOC_LITERAL(10, 147, 12), // "input_garray"
QT_MOC_LITERAL(11, 160, 7), // "Destroy"
QT_MOC_LITERAL(12, 168, 13), // "ModifyChannel"
QT_MOC_LITERAL(13, 182, 11), // "modify_type"
QT_MOC_LITERAL(14, 194, 14) // "ReleaseChannel"

    },
    "ChannelDispatcherIf\0NofityChannelStateChanged\0"
    "\0channel_id\0user_id\0channel_type\0"
    "channel_state\0ChannelStateChanged\0"
    "QDBusPendingReply<>\0CreateChannel\0"
    "input_garray\0Destroy\0ModifyChannel\0"
    "modify_type\0ReleaseChannel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChannelDispatcherIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    4,   53,    2, 0x0a /* Public */,
       9,    3,   62,    2, 0x0a /* Public */,
      11,    0,   69,    2, 0x0a /* Public */,
      12,    4,   70,    2, 0x0a /* Public */,
      14,    2,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,

 // slots: parameters
    0x80000000 | 8, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,
    0x80000000 | 8, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,    3,    5,   10,
    0x80000000 | 8,
    0x80000000 | 8, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,    3,    5,   13,   10,
    0x80000000 | 8, QMetaType::QString, QMetaType::QString,    3,    5,

       0        // eod
};

void ChannelDispatcherIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChannelDispatcherIf *_t = static_cast<ChannelDispatcherIf *>(_o);
        switch (_id) {
        case 0: _t->NofityChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: { QDBusPendingReply<> _r = _t->ChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<> _r = _t->CreateChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->Destroy();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->ModifyChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->ReleaseChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ChannelDispatcherIf::*_t)(const QString & , const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChannelDispatcherIf::NofityChannelStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ChannelDispatcherIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_ChannelDispatcherIf.data,
      qt_meta_data_ChannelDispatcherIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ChannelDispatcherIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChannelDispatcherIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ChannelDispatcherIf.stringdata))
        return static_cast<void*>(const_cast< ChannelDispatcherIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int ChannelDispatcherIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ChannelDispatcherIf::NofityChannelStateChanged(const QString & _t1, const QString & _t2, const QString & _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
