/****************************************************************************
** Meta object code from reading C++ file 'CvStreamedMediaIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/dbus/channel/type/streamedMedia/client/CvStreamedMediaIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvStreamedMediaIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvStreamedMediaIf_t {
    QByteArrayData data[19];
    char stringdata[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvStreamedMediaIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvStreamedMediaIf_t qt_meta_stringdata_CvStreamedMediaIf = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CvStreamedMediaIf"
QT_MOC_LITERAL(1, 18, 9), // "QosNofity"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "output_garray"
QT_MOC_LITERAL(4, 43, 12), // "notify_index"
QT_MOC_LITERAL(5, 56, 19), // "RunningStateChanged"
QT_MOC_LITERAL(6, 76, 8), // "media_id"
QT_MOC_LITERAL(7, 85, 13), // "current_state"
QT_MOC_LITERAL(8, 99, 7), // "Destroy"
QT_MOC_LITERAL(9, 107, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(10, 127, 12), // "GetMediaInfo"
QT_MOC_LITERAL(11, 140, 29), // "QDBusPendingReply<QByteArray>"
QT_MOC_LITERAL(12, 170, 9), // "info_type"
QT_MOC_LITERAL(13, 180, 5), // "Pause"
QT_MOC_LITERAL(14, 186, 3), // "Run"
QT_MOC_LITERAL(15, 190, 12), // "SetMediaInfo"
QT_MOC_LITERAL(16, 203, 9), // "action_id"
QT_MOC_LITERAL(17, 213, 12), // "input_garray"
QT_MOC_LITERAL(18, 226, 4) // "Stop"

    },
    "CvStreamedMediaIf\0QosNofity\0\0output_garray\0"
    "notify_index\0RunningStateChanged\0"
    "media_id\0current_state\0Destroy\0"
    "QDBusPendingReply<>\0GetMediaInfo\0"
    "QDBusPendingReply<QByteArray>\0info_type\0"
    "Pause\0Run\0SetMediaInfo\0action_id\0"
    "input_garray\0Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvStreamedMediaIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   64,    2, 0x0a /* Public */,
      10,    1,   65,    2, 0x0a /* Public */,
      13,    0,   68,    2, 0x0a /* Public */,
      14,    0,   69,    2, 0x0a /* Public */,
      15,    2,   70,    2, 0x0a /* Public */,
      18,    0,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,

 // slots: parameters
    0x80000000 | 9,
    0x80000000 | 11, QMetaType::Int,   12,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9, QMetaType::Int, QMetaType::QByteArray,   16,   17,
    0x80000000 | 9,

       0        // eod
};

void CvStreamedMediaIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvStreamedMediaIf *_t = static_cast<CvStreamedMediaIf *>(_o);
        switch (_id) {
        case 0: _t->QosNofity((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->RunningStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: { QDBusPendingReply<> _r = _t->Destroy();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<QByteArray> _r = _t->GetMediaInfo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QByteArray>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->Pause();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->Run();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->SetMediaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->Stop();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvStreamedMediaIf::*_t)(const QByteArray & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMediaIf::QosNofity)) {
                *result = 0;
            }
        }
        {
            typedef void (CvStreamedMediaIf::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMediaIf::RunningStateChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CvStreamedMediaIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvStreamedMediaIf.data,
      qt_meta_data_CvStreamedMediaIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvStreamedMediaIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvStreamedMediaIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvStreamedMediaIf.stringdata))
        return static_cast<void*>(const_cast< CvStreamedMediaIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvStreamedMediaIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CvStreamedMediaIf::QosNofity(const QByteArray & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CvStreamedMediaIf::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
