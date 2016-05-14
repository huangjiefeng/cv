/****************************************************************************
** Meta object code from reading C++ file 'CvCoreAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/core/service/CvCoreAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvCoreAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvCoreAdaptor_t {
    QByteArrayData data[9];
    char stringdata[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvCoreAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvCoreAdaptor_t qt_meta_stringdata_CvCoreAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CvCoreAdaptor"
QT_MOC_LITERAL(1, 14, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 30, 25), // "com.dcampus.coolview.core"
QT_MOC_LITERAL(3, 56, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 76, 246), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 301, 11), // "TeleCommand"
QT_MOC_LITERAL(6, 313, 0), // ""
QT_MOC_LITERAL(7, 314, 13), // "command_index"
QT_MOC_LITERAL(8, 328, 12) // "intput_array"

    },
    "CvCoreAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.core\0D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.core\">\n    <method name=\"T"
    "eleCommand\">\n      <arg direction=\"in\" type=\"i\" name=\"command_i"
    "ndex\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"intput_array\""
    "/>\n    </method>\n  </interface>\n\0"
    "TeleCommand\0\0command_index\0intput_array"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvCoreAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       1,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // slots: name, argc, parameters, tag, flags
       5,    2,   23,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    7,    8,

       0        // eod
};

void CvCoreAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvCoreAdaptor *_t = static_cast<CvCoreAdaptor *>(_o);
        switch (_id) {
        case 0: _t->TeleCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CvCoreAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_CvCoreAdaptor.data,
      qt_meta_data_CvCoreAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvCoreAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvCoreAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvCoreAdaptor.stringdata))
        return static_cast<void*>(const_cast< CvCoreAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CvCoreAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
