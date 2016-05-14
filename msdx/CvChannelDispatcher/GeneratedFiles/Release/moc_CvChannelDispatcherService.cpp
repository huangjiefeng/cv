/****************************************************************************
** Meta object code from reading C++ file 'CvChannelDispatcherService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CvChannelDispatcherService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvChannelDispatcherService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvChannelDispatcherService_t {
    QByteArrayData data[17];
    char stringdata[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvChannelDispatcherService_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvChannelDispatcherService_t qt_meta_stringdata_CvChannelDispatcherService = {
    {
QT_MOC_LITERAL(0, 0, 26), // "CvChannelDispatcherService"
QT_MOC_LITERAL(1, 27, 25), // "NofityChannelStateChanged"
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 10), // "channel_id"
QT_MOC_LITERAL(4, 65, 7), // "user_id"
QT_MOC_LITERAL(5, 73, 12), // "channel_type"
QT_MOC_LITERAL(6, 86, 13), // "channel_state"
QT_MOC_LITERAL(7, 100, 19), // "ChannelStateChanged"
QT_MOC_LITERAL(8, 120, 7), // "mediaID"
QT_MOC_LITERAL(9, 128, 9), // "mediaType"
QT_MOC_LITERAL(10, 138, 5), // "state"
QT_MOC_LITERAL(11, 144, 13), // "CreateChannel"
QT_MOC_LITERAL(12, 158, 5), // "param"
QT_MOC_LITERAL(13, 164, 7), // "Destroy"
QT_MOC_LITERAL(14, 172, 13), // "ModifyChannel"
QT_MOC_LITERAL(15, 186, 4), // "type"
QT_MOC_LITERAL(16, 191, 14) // "ReleaseChannel"

    },
    "CvChannelDispatcherService\0"
    "NofityChannelStateChanged\0\0channel_id\0"
    "user_id\0channel_type\0channel_state\0"
    "ChannelStateChanged\0mediaID\0mediaType\0"
    "state\0CreateChannel\0param\0Destroy\0"
    "ModifyChannel\0type\0ReleaseChannel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvChannelDispatcherService[] = {

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
      11,    3,   62,    2, 0x0a /* Public */,
      13,    0,   69,    2, 0x0a /* Public */,
      14,    4,   70,    2, 0x0a /* Public */,
      16,    2,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    8,    4,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,    8,    9,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,    8,    9,   15,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,

       0        // eod
};

void CvChannelDispatcherService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvChannelDispatcherService *_t = static_cast<CvChannelDispatcherService *>(_o);
        switch (_id) {
        case 0: _t->NofityChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->ChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->CreateChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 3: _t->Destroy(); break;
        case 4: _t->ModifyChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 5: _t->ReleaseChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvChannelDispatcherService::*_t)(const QString & , const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvChannelDispatcherService::NofityChannelStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvChannelDispatcherService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CvChannelDispatcherService.data,
      qt_meta_data_CvChannelDispatcherService,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvChannelDispatcherService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvChannelDispatcherService::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvChannelDispatcherService.stringdata))
        return static_cast<void*>(const_cast< CvChannelDispatcherService*>(this));
    return QObject::qt_metacast(_clname);
}

int CvChannelDispatcherService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CvChannelDispatcherService::NofityChannelStateChanged(const QString & _t1, const QString & _t2, const QString & _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
