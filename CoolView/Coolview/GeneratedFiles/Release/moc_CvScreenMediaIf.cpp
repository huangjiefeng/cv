/****************************************************************************
** Meta object code from reading C++ file 'CvScreenMediaIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/channel/type/screenMedia/client/CvScreenMediaIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvScreenMediaIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvScreenMediaIf_t {
    QByteArrayData data[16];
    char stringdata[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvScreenMediaIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvScreenMediaIf_t qt_meta_stringdata_CvScreenMediaIf = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CvScreenMediaIf"
QT_MOC_LITERAL(1, 16, 19), // "RunningStateChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 8), // "media_id"
QT_MOC_LITERAL(4, 46, 13), // "current_state"
QT_MOC_LITERAL(5, 60, 7), // "Destroy"
QT_MOC_LITERAL(6, 68, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(7, 88, 12), // "GetMediaInfo"
QT_MOC_LITERAL(8, 101, 29), // "QDBusPendingReply<QByteArray>"
QT_MOC_LITERAL(9, 131, 9), // "info_type"
QT_MOC_LITERAL(10, 141, 5), // "Pause"
QT_MOC_LITERAL(11, 147, 3), // "Run"
QT_MOC_LITERAL(12, 151, 12), // "SetMediaInfo"
QT_MOC_LITERAL(13, 164, 9), // "action_id"
QT_MOC_LITERAL(14, 174, 12), // "input_garray"
QT_MOC_LITERAL(15, 187, 4) // "Stop"

    },
    "CvScreenMediaIf\0RunningStateChanged\0"
    "\0media_id\0current_state\0Destroy\0"
    "QDBusPendingReply<>\0GetMediaInfo\0"
    "QDBusPendingReply<QByteArray>\0info_type\0"
    "Pause\0Run\0SetMediaInfo\0action_id\0"
    "input_garray\0Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvScreenMediaIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   54,    2, 0x0a /* Public */,
       7,    1,   55,    2, 0x0a /* Public */,
      10,    0,   58,    2, 0x0a /* Public */,
      11,    0,   59,    2, 0x0a /* Public */,
      12,    2,   60,    2, 0x0a /* Public */,
      15,    0,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,

 // slots: parameters
    0x80000000 | 6,
    0x80000000 | 8, QMetaType::Int,    9,
    0x80000000 | 6,
    0x80000000 | 6,
    0x80000000 | 6, QMetaType::Int, QMetaType::QByteArray,   13,   14,
    0x80000000 | 6,

       0        // eod
};

void CvScreenMediaIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvScreenMediaIf *_t = static_cast<CvScreenMediaIf *>(_o);
        switch (_id) {
        case 0: _t->RunningStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: { QDBusPendingReply<> _r = _t->Destroy();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<QByteArray> _r = _t->GetMediaInfo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QByteArray>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->Pause();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->Run();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->SetMediaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->Stop();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvScreenMediaIf::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvScreenMediaIf::RunningStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvScreenMediaIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvScreenMediaIf.data,
      qt_meta_data_CvScreenMediaIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvScreenMediaIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvScreenMediaIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvScreenMediaIf.stringdata))
        return static_cast<void*>(const_cast< CvScreenMediaIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvScreenMediaIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CvScreenMediaIf::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
