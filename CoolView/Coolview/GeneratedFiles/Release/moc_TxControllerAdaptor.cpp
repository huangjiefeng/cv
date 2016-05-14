/****************************************************************************
** Meta object code from reading C++ file 'TxControllerAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/txController/service/TxControllerAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TxControllerAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TxControllerAdaptor_t {
    QByteArrayData data[12];
    char stringdata[664];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TxControllerAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TxControllerAdaptor_t qt_meta_stringdata_TxControllerAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 19), // "TxControllerAdaptor"
QT_MOC_LITERAL(1, 20, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 36, 33), // "com.dcampus.coolview.txContro..."
QT_MOC_LITERAL(3, 70, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 90, 523), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 560, 13), // "ControlRecord"
QT_MOC_LITERAL(6, 574, 0), // ""
QT_MOC_LITERAL(7, 575, 2), // "op"
QT_MOC_LITERAL(8, 578, 5), // "param"
QT_MOC_LITERAL(9, 584, 15), // "GetRecordStatus"
QT_MOC_LITERAL(10, 600, 5), // "focus"
QT_MOC_LITERAL(11, 606, 3) // "uri"

    },
    "TxControllerAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.txController\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.txController\">\n    <method "
    "name=\"ControlRecord\">\n      <arg direction=\"in\" type=\"i\" name=\""
    "op\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"param\"/>\n    "
    "  <arg direction=\"out\" type=\"i\" name=\"result\"/>\n    </method>\n"
    "    <method name=\"GetRecordStatus\">\n      <arg direction=\"in\" typ"
    "e=\"s\" name=\"focus\"/>\n      <arg direction=\"in\" type=\"s\" name="
    "\"uri\"/>\n      <arg direction=\"out\" type=\"ay\" name=\"status\"/>\n"
    "    </method>\n  </interface>\n\0"
    "ControlRecord\0\0op\0param\0GetRecordStatus\0"
    "focus\0uri"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TxControllerAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       2,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // slots: name, argc, parameters, tag, flags
       5,    2,   28,    6, 0x0a /* Public */,
       9,    2,   33,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    7,    8,
    QMetaType::QByteArray, QMetaType::QString, QMetaType::QString,   10,   11,

       0        // eod
};

void TxControllerAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TxControllerAdaptor *_t = static_cast<TxControllerAdaptor *>(_o);
        switch (_id) {
        case 0: { int _r = _t->ControlRecord((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { QByteArray _r = _t->GetRecordStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject TxControllerAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TxControllerAdaptor.data,
      qt_meta_data_TxControllerAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TxControllerAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TxControllerAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TxControllerAdaptor.stringdata))
        return static_cast<void*>(const_cast< TxControllerAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TxControllerAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
