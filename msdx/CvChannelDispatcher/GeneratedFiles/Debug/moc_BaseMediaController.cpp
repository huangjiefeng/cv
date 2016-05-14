/****************************************************************************
** Meta object code from reading C++ file 'BaseMediaController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BaseMediaController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseMediaController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseMediaController_t {
    QByteArrayData data[16];
    char stringdata[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseMediaController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseMediaController_t qt_meta_stringdata_BaseMediaController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "BaseMediaController"
QT_MOC_LITERAL(1, 20, 12), // "CreateSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "param"
QT_MOC_LITERAL(4, 40, 12), // "ModifySignal"
QT_MOC_LITERAL(5, 53, 4), // "type"
QT_MOC_LITERAL(6, 58, 13), // "ReleaseSignal"
QT_MOC_LITERAL(7, 72, 18), // "StateChangedSignal"
QT_MOC_LITERAL(8, 91, 5), // "state"
QT_MOC_LITERAL(9, 97, 13), // "DestroySignal"
QT_MOC_LITERAL(10, 111, 12), // "HandleCreate"
QT_MOC_LITERAL(11, 124, 12), // "HandleModify"
QT_MOC_LITERAL(12, 137, 13), // "HandleRelease"
QT_MOC_LITERAL(13, 151, 13), // "HandleDestroy"
QT_MOC_LITERAL(14, 165, 18), // "HandleStateChanged"
QT_MOC_LITERAL(15, 184, 20) // "HandleCheckerTimeout"

    },
    "BaseMediaController\0CreateSignal\0\0"
    "param\0ModifySignal\0type\0ReleaseSignal\0"
    "StateChangedSignal\0state\0DestroySignal\0"
    "HandleCreate\0HandleModify\0HandleRelease\0"
    "HandleDestroy\0HandleStateChanged\0"
    "HandleCheckerTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseMediaController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    2,   72,    2, 0x06 /* Public */,
       6,    0,   77,    2, 0x06 /* Public */,
       7,    1,   78,    2, 0x06 /* Public */,
       9,    0,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   82,    2, 0x09 /* Protected */,
      11,    2,   85,    2, 0x09 /* Protected */,
      12,    0,   90,    2, 0x09 /* Protected */,
      13,    0,   91,    2, 0x09 /* Protected */,
      14,    1,   92,    2, 0x09 /* Protected */,
      15,    0,   95,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    5,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    5,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

       0        // eod
};

void BaseMediaController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseMediaController *_t = static_cast<BaseMediaController *>(_o);
        switch (_id) {
        case 0: _t->CreateSignal((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->ModifySignal((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->ReleaseSignal(); break;
        case 3: _t->StateChangedSignal((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->DestroySignal(); break;
        case 5: _t->HandleCreate((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->HandleModify((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 7: _t->HandleRelease(); break;
        case 8: _t->HandleDestroy(); break;
        case 9: _t->HandleStateChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 10: _t->HandleCheckerTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseMediaController::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseMediaController::CreateSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (BaseMediaController::*_t)(const int , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseMediaController::ModifySignal)) {
                *result = 1;
            }
        }
        {
            typedef void (BaseMediaController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseMediaController::ReleaseSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (BaseMediaController::*_t)(const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseMediaController::StateChangedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (BaseMediaController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseMediaController::DestroySignal)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject BaseMediaController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseMediaController.data,
      qt_meta_data_BaseMediaController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseMediaController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseMediaController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseMediaController.stringdata))
        return static_cast<void*>(const_cast< BaseMediaController*>(this));
    if (!strcmp(_clname, "IMediaControl"))
        return static_cast< IMediaControl*>(const_cast< BaseMediaController*>(this));
    if (!strcmp(_clname, "ISpecMediaControl"))
        return static_cast< ISpecMediaControl*>(const_cast< BaseMediaController*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseMediaController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void BaseMediaController::CreateSignal(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BaseMediaController::ModifySignal(const int _t1, const QByteArray & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BaseMediaController::ReleaseSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void BaseMediaController::StateChangedSignal(const int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BaseMediaController::DestroySignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
