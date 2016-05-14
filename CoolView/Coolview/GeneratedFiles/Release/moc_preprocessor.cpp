/****************************************************************************
** Meta object code from reading C++ file 'preprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../preprocessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'preprocessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Preprocessor_t {
    QByteArrayData data[8];
    char stringdata[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Preprocessor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Preprocessor_t qt_meta_stringdata_Preprocessor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Preprocessor"
QT_MOC_LITERAL(1, 13, 13), // "StartedSignal"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "FinishedSignal"
QT_MOC_LITERAL(4, 43, 13), // "ShowMsgSignal"
QT_MOC_LITERAL(5, 57, 3), // "msg"
QT_MOC_LITERAL(6, 61, 10), // "Preprocess"
QT_MOC_LITERAL(7, 72, 6) // "Launch"

    },
    "Preprocessor\0StartedSignal\0\0FinishedSignal\0"
    "ShowMsgSignal\0msg\0Preprocess\0Launch"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Preprocessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    1,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   44,    2, 0x09 /* Protected */,
       7,    0,   45,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Preprocessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Preprocessor *_t = static_cast<Preprocessor *>(_o);
        switch (_id) {
        case 0: _t->StartedSignal(); break;
        case 1: _t->FinishedSignal(); break;
        case 2: _t->ShowMsgSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->Preprocess(); break;
        case 4: _t->Launch(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Preprocessor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Preprocessor::StartedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (Preprocessor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Preprocessor::FinishedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (Preprocessor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Preprocessor::ShowMsgSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Preprocessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Preprocessor.data,
      qt_meta_data_Preprocessor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Preprocessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Preprocessor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Preprocessor.stringdata))
        return static_cast<void*>(const_cast< Preprocessor*>(this));
    return QObject::qt_metacast(_clname);
}

int Preprocessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Preprocessor::StartedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Preprocessor::FinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Preprocessor::ShowMsgSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
