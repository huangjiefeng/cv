/****************************************************************************
** Meta object code from reading C++ file 'screen_helper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/util/desktop/screen_helper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_helper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScreenHelper_t {
    QByteArrayData data[6];
    char stringdata[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScreenHelper_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScreenHelper_t qt_meta_stringdata_ScreenHelper = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ScreenHelper"
QT_MOC_LITERAL(1, 13, 18), // "ScreenCountChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 12), // "screen_count"
QT_MOC_LITERAL(4, 46, 20), // "PrimaryScreenChanged"
QT_MOC_LITERAL(5, 67, 13) // "primary_index"

    },
    "ScreenHelper\0ScreenCountChanged\0\0"
    "screen_count\0PrimaryScreenChanged\0"
    "primary_index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScreenHelper[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void ScreenHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScreenHelper *_t = static_cast<ScreenHelper *>(_o);
        switch (_id) {
        case 0: _t->ScreenCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->PrimaryScreenChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ScreenHelper::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScreenHelper::ScreenCountChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ScreenHelper::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScreenHelper::PrimaryScreenChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ScreenHelper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ScreenHelper.data,
      qt_meta_data_ScreenHelper,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ScreenHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ScreenHelper.stringdata))
        return static_cast<void*>(const_cast< ScreenHelper*>(this));
    return QObject::qt_metacast(_clname);
}

int ScreenHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ScreenHelper::ScreenCountChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ScreenHelper::PrimaryScreenChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
