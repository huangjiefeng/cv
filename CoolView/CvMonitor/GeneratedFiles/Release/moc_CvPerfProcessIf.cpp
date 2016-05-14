/****************************************************************************
** Meta object code from reading C++ file 'CvPerfProcessIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/performance/process/client/CvPerfProcessIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvPerfProcessIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvPerfProcessIf_t {
    QByteArrayData data[6];
    char stringdata[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvPerfProcessIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvPerfProcessIf_t qt_meta_stringdata_CvPerfProcessIf = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CvPerfProcessIf"
QT_MOC_LITERAL(1, 16, 11), // "ExitProcess"
QT_MOC_LITERAL(2, 28, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(3, 48, 0), // ""
QT_MOC_LITERAL(4, 49, 7), // "Recover"
QT_MOC_LITERAL(5, 57, 12) // "processImage"

    },
    "CvPerfProcessIf\0ExitProcess\0"
    "QDBusPendingReply<>\0\0Recover\0processImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvPerfProcessIf[] = {

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
       1,    0,   24,    3, 0x0a /* Public */,
       4,    1,   25,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    0x80000000 | 2, QMetaType::QByteArray,    5,

       0        // eod
};

void CvPerfProcessIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvPerfProcessIf *_t = static_cast<CvPerfProcessIf *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<> _r = _t->ExitProcess();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 1: { QDBusPendingReply<> _r = _t->Recover((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CvPerfProcessIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvPerfProcessIf.data,
      qt_meta_data_CvPerfProcessIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvPerfProcessIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvPerfProcessIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvPerfProcessIf.stringdata))
        return static_cast<void*>(const_cast< CvPerfProcessIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvPerfProcessIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
