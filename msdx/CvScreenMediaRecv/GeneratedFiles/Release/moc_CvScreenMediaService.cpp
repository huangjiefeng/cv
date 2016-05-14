/****************************************************************************
** Meta object code from reading C++ file 'CvScreenMediaService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CvScreenMediaService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvScreenMediaService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvScreenMediaService_t {
    QByteArrayData data[14];
    char stringdata[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvScreenMediaService_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvScreenMediaService_t qt_meta_stringdata_CvScreenMediaService = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CvScreenMediaService"
QT_MOC_LITERAL(1, 21, 19), // "RunningStateChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 8), // "media_id"
QT_MOC_LITERAL(4, 51, 13), // "current_state"
QT_MOC_LITERAL(5, 65, 7), // "Destroy"
QT_MOC_LITERAL(6, 73, 12), // "GetMediaInfo"
QT_MOC_LITERAL(7, 86, 9), // "info_type"
QT_MOC_LITERAL(8, 96, 5), // "Pause"
QT_MOC_LITERAL(9, 102, 3), // "Run"
QT_MOC_LITERAL(10, 106, 12), // "SetMediaInfo"
QT_MOC_LITERAL(11, 119, 9), // "action_id"
QT_MOC_LITERAL(12, 129, 12), // "input_garray"
QT_MOC_LITERAL(13, 142, 4) // "Stop"

    },
    "CvScreenMediaService\0RunningStateChanged\0"
    "\0media_id\0current_state\0Destroy\0"
    "GetMediaInfo\0info_type\0Pause\0Run\0"
    "SetMediaInfo\0action_id\0input_garray\0"
    "Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvScreenMediaService[] = {

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
       6,    1,   55,    2, 0x0a /* Public */,
       8,    0,   58,    2, 0x0a /* Public */,
       9,    0,   59,    2, 0x0a /* Public */,
      10,    2,   60,    2, 0x0a /* Public */,
      13,    0,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::QByteArray, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   11,   12,
    QMetaType::Void,

       0        // eod
};

void CvScreenMediaService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvScreenMediaService *_t = static_cast<CvScreenMediaService *>(_o);
        switch (_id) {
        case 0: _t->RunningStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->Destroy(); break;
        case 2: { QByteArray _r = _t->GetMediaInfo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 3: _t->Pause(); break;
        case 4: _t->Run(); break;
        case 5: _t->SetMediaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 6: _t->Stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvScreenMediaService::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvScreenMediaService::RunningStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvScreenMediaService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CvScreenMediaService.data,
      qt_meta_data_CvScreenMediaService,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvScreenMediaService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvScreenMediaService::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvScreenMediaService.stringdata))
        return static_cast<void*>(const_cast< CvScreenMediaService*>(this));
    return QObject::qt_metacast(_clname);
}

int CvScreenMediaService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CvScreenMediaService::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
