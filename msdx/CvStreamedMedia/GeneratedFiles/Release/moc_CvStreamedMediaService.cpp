/****************************************************************************
** Meta object code from reading C++ file 'CvStreamedMediaService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CvStreamedMediaService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvStreamedMediaService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvStreamedMedia_t {
    QByteArrayData data[18];
    char stringdata[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvStreamedMedia_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvStreamedMedia_t qt_meta_stringdata_CvStreamedMedia = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CvStreamedMedia"
QT_MOC_LITERAL(1, 16, 9), // "QosNofity"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "output_garray"
QT_MOC_LITERAL(4, 41, 12), // "notify_index"
QT_MOC_LITERAL(5, 54, 19), // "RunningStateChanged"
QT_MOC_LITERAL(6, 74, 8), // "media_id"
QT_MOC_LITERAL(7, 83, 13), // "current_state"
QT_MOC_LITERAL(8, 97, 7), // "Destroy"
QT_MOC_LITERAL(9, 105, 12), // "GetMediaInfo"
QT_MOC_LITERAL(10, 118, 9), // "info_type"
QT_MOC_LITERAL(11, 128, 5), // "Pause"
QT_MOC_LITERAL(12, 134, 3), // "Run"
QT_MOC_LITERAL(13, 138, 12), // "SetMediaInfo"
QT_MOC_LITERAL(14, 151, 9), // "action_id"
QT_MOC_LITERAL(15, 161, 12), // "input_garray"
QT_MOC_LITERAL(16, 174, 4), // "Stop"
QT_MOC_LITERAL(17, 179, 38) // "HandleVideoWindowCheckTimerTi..."

    },
    "CvStreamedMedia\0QosNofity\0\0output_garray\0"
    "notify_index\0RunningStateChanged\0"
    "media_id\0current_state\0Destroy\0"
    "GetMediaInfo\0info_type\0Pause\0Run\0"
    "SetMediaInfo\0action_id\0input_garray\0"
    "Stop\0HandleVideoWindowCheckTimerTimeoutSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvStreamedMedia[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       5,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   69,    2, 0x0a /* Public */,
       9,    1,   70,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x0a /* Public */,
      12,    0,   74,    2, 0x0a /* Public */,
      13,    2,   75,    2, 0x0a /* Public */,
      16,    0,   80,    2, 0x0a /* Public */,
      17,    0,   81,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::QByteArray, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   14,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CvStreamedMedia::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvStreamedMedia *_t = static_cast<CvStreamedMedia *>(_o);
        switch (_id) {
        case 0: _t->QosNofity((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->RunningStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->Destroy(); break;
        case 3: { QByteArray _r = _t->GetMediaInfo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 4: _t->Pause(); break;
        case 5: _t->Run(); break;
        case 6: _t->SetMediaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 7: _t->Stop(); break;
        case 8: _t->HandleVideoWindowCheckTimerTimeoutSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvStreamedMedia::*_t)(const QByteArray & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMedia::QosNofity)) {
                *result = 0;
            }
        }
        {
            typedef void (CvStreamedMedia::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMedia::RunningStateChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CvStreamedMedia::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CvStreamedMedia.data,
      qt_meta_data_CvStreamedMedia,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvStreamedMedia::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvStreamedMedia::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvStreamedMedia.stringdata))
        return static_cast<void*>(const_cast< CvStreamedMedia*>(this));
    return QObject::qt_metacast(_clname);
}

int CvStreamedMedia::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CvStreamedMedia::QosNofity(const QByteArray & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CvStreamedMedia::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
