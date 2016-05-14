/****************************************************************************
** Meta object code from reading C++ file 'ChannelDispatcherAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/channelDispatcher/service/ChannelDispatcherAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChannelDispatcherAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChannelDispatcherAdaptor_t {
    QByteArrayData data[18];
    char stringdata[1735];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChannelDispatcherAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChannelDispatcherAdaptor_t qt_meta_stringdata_ChannelDispatcherAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ChannelDispatcherAdaptor"
QT_MOC_LITERAL(1, 25, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 41, 38), // "com.dcampus.coolview.channelD..."
QT_MOC_LITERAL(3, 80, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 100, 1465), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 1420, 25), // "NofityChannelStateChanged"
QT_MOC_LITERAL(6, 1446, 0), // ""
QT_MOC_LITERAL(7, 1447, 10), // "channel_id"
QT_MOC_LITERAL(8, 1458, 7), // "user_id"
QT_MOC_LITERAL(9, 1466, 12), // "channel_type"
QT_MOC_LITERAL(10, 1479, 13), // "channel_state"
QT_MOC_LITERAL(11, 1493, 19), // "ChannelStateChanged"
QT_MOC_LITERAL(12, 1513, 13), // "CreateChannel"
QT_MOC_LITERAL(13, 1527, 12), // "input_garray"
QT_MOC_LITERAL(14, 1540, 7), // "Destroy"
QT_MOC_LITERAL(15, 1548, 13), // "ModifyChannel"
QT_MOC_LITERAL(16, 1562, 11), // "modify_type"
QT_MOC_LITERAL(17, 1574, 14) // "ReleaseChannel"

    },
    "ChannelDispatcherAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.channelDispatcher\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.channelDispatcher\">\n    <me"
    "thod name=\"CreateChannel\">\n      <arg direction=\"in\" type=\"s\" n"
    "ame=\"channel_id\"/>\n      <arg direction=\"in\" type=\"s\" name=\"ch"
    "annel_type\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"input_g"
    "array\"/>\n    </method>\n    <method name=\"ReleaseChannel\">\n      "
    "<arg direction=\"in\" type=\"s\" name=\"channel_id\"/>\n      <arg dir"
    "ection=\"in\" type=\"s\" name=\"channel_type\"/>\n    </method>\n    <"
    "method name=\"ModifyChannel\">\n      <arg direction=\"in\" type=\"s\""
    " name=\"channel_id\"/>\n      <arg direction=\"in\" type=\"s\" name=\""
    "channel_type\"/>\n      <arg direction=\"in\" type=\"i\" name=\"modify"
    "_type\"/>\n      <arg direction=\"in\" type=\"ay\" name=\"input_garray"
    "\"/>\n    </method>\n    <method name=\"Destroy\"/>\n    <method name="
    "\"ChannelStateChanged\">\n      <arg direction=\"in\" type=\"s\" name="
    "\"channel_id\"/>\n      <arg direction=\"in\" type=\"s\" name=\"user_i"
    "d\"/>\n      <arg direction=\"in\" type=\"s\" name=\"channel_type\"/>\n"
    "      <arg direction=\"in\" type=\"i\" name=\"channel_state\"/>\n    <"
    "/method>\n    <signal name=\"NofityChannelStateChanged\">\n      <arg "
    "direction=\"out\" type=\"s\" name=\"channel_id\"/>\n      <arg directi"
    "on=\"out\" type=\"s\" name=\"user_id\"/>\n      <arg direction=\"out\""
    " type=\"s\" name=\"channel_type\"/>\n      <arg direction=\"out\" type"
    "=\"i\" name=\"channel_state\"/>\n    </signal>\n  </interface>\n\0"
    "NofityChannelStateChanged\0\0channel_id\0"
    "user_id\0channel_type\0channel_state\0"
    "ChannelStateChanged\0CreateChannel\0"
    "input_garray\0Destroy\0ModifyChannel\0"
    "modify_type\0ReleaseChannel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChannelDispatcherAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       6,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    4,   48,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    4,   57,    6, 0x0a /* Public */,
      12,    3,   66,    6, 0x0a /* Public */,
      14,    0,   73,    6, 0x0a /* Public */,
      15,    4,   74,    6, 0x0a /* Public */,
      17,    2,   83,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    7,    8,    9,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,    7,    9,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,    7,    9,   16,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    9,

       0        // eod
};

void ChannelDispatcherAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChannelDispatcherAdaptor *_t = static_cast<ChannelDispatcherAdaptor *>(_o);
        switch (_id) {
        case 0: _t->NofityChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->ChannelStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->CreateChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 3: _t->Destroy(); break;
        case 4: _t->ModifyChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 5: _t->ReleaseChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ChannelDispatcherAdaptor::*_t)(const QString & , const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChannelDispatcherAdaptor::NofityChannelStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ChannelDispatcherAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_ChannelDispatcherAdaptor.data,
      qt_meta_data_ChannelDispatcherAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ChannelDispatcherAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChannelDispatcherAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ChannelDispatcherAdaptor.stringdata))
        return static_cast<void*>(const_cast< ChannelDispatcherAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int ChannelDispatcherAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ChannelDispatcherAdaptor::NofityChannelStateChanged(const QString & _t1, const QString & _t2, const QString & _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
