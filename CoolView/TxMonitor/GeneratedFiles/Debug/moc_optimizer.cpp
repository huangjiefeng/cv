/****************************************************************************
** Meta object code from reading C++ file 'optimizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../optimizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'optimizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Optimizer_t {
    QByteArrayData data[6];
    char stringdata[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Optimizer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Optimizer_t qt_meta_stringdata_Optimizer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Optimizer"
QT_MOC_LITERAL(1, 10, 14), // "OptCmdRunError"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(4, 49, 5), // "error"
QT_MOC_LITERAL(5, 55, 15) // "OptCmdStdOutput"

    },
    "Optimizer\0OptCmdRunError\0\0"
    "QProcess::ProcessError\0error\0"
    "OptCmdStdOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Optimizer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x09 /* Protected */,
       5,    0,   27,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void Optimizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Optimizer *_t = static_cast<Optimizer *>(_o);
        switch (_id) {
        case 0: _t->OptCmdRunError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 1: _t->OptCmdStdOutput(); break;
        default: ;
        }
    }
}

const QMetaObject Optimizer::staticMetaObject = {
    { &BaseLoopJob::staticMetaObject, qt_meta_stringdata_Optimizer.data,
      qt_meta_data_Optimizer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Optimizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Optimizer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Optimizer.stringdata))
        return static_cast<void*>(const_cast< Optimizer*>(this));
    return BaseLoopJob::qt_metacast(_clname);
}

int Optimizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseLoopJob::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
