/****************************************************************************
** Meta object code from reading C++ file 'CvStreamedMediaAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/channel/type/streamedMedia/service/CvStreamedMediaAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvStreamedMediaAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvStreamedMediaAdaptor_t {
    QByteArrayData data[21];
    char stringdata[1510];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvStreamedMediaAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvStreamedMediaAdaptor_t qt_meta_stringdata_CvStreamedMediaAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 22), // "CvStreamedMediaAdaptor"
QT_MOC_LITERAL(1, 23, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 39, 47), // "com.dcampus.coolview.channel...."
QT_MOC_LITERAL(3, 87, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 107, 1239), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 1244, 9), // "QosNofity"
QT_MOC_LITERAL(6, 1254, 0), // ""
QT_MOC_LITERAL(7, 1255, 13), // "output_garray"
QT_MOC_LITERAL(8, 1269, 12), // "notify_index"
QT_MOC_LITERAL(9, 1282, 19), // "RunningStateChanged"
QT_MOC_LITERAL(10, 1302, 8), // "media_id"
QT_MOC_LITERAL(11, 1311, 13), // "current_state"
QT_MOC_LITERAL(12, 1325, 7), // "Destroy"
QT_MOC_LITERAL(13, 1333, 12), // "GetMediaInfo"
QT_MOC_LITERAL(14, 1346, 9), // "info_type"
QT_MOC_LITERAL(15, 1356, 5), // "Pause"
QT_MOC_LITERAL(16, 1362, 3), // "Run"
QT_MOC_LITERAL(17, 1366, 12), // "SetMediaInfo"
QT_MOC_LITERAL(18, 1379, 9), // "action_id"
QT_MOC_LITERAL(19, 1389, 12), // "input_garray"
QT_MOC_LITERAL(20, 1402, 4) // "Stop"

    },
    "CvStreamedMediaAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.channel.type.streamedMedia\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.channel.type.streamedMedia\">"
    "\n    <method name=\"SetMediaInfo\">\n      <annotation value=\"__clie"
    "nt_request_cb\" name=\"org.freedesktop.DBus.GLib.CSymbol\"/>\n      <a"
    "rg direction=\"in\" type=\"i\" name=\"action_id\"/>\n      <arg direct"
    "ion=\"in\" type=\"ay\" name=\"input_garray\"/>\n      <!--media params"
    "-->\n    </method>\n    <method name=\"GetMediaInfo\">\n      <!--get "
    "media info -->\n      <arg direction=\"in\" type=\"i\" name=\"info_typ"
    "e\"/>\n      <arg direction=\"out\" type=\"ay\" name=\"output_garray\""
    "/>\n    </method>\n    <method name=\"Run\">\n      <!--run media-->\n"
    "    </method>\n    <method name=\"Pause\">\n      <!--pause media-->\n"
    "    </method>\n    <method name=\"Stop\">\n      <!--stop media -->\n "
    "   </method>\n    <method name=\"Destroy\">\n      <!--destroy media--"
    ">\n    </method>\n    <signal name=\"QosNofity\">\n      <arg directio"
    "n=\"out\" type=\"ay\" name=\"output_garray\"/>\n      <arg direction=\""
    "out\" type=\"i\" name=\"notify_index\"/>\n    </signal>\n    <signal n"
    "ame=\"RunningStateChanged\">\n      <arg direction=\"out\" type=\"s\" "
    "name=\"media_id\"/>\n      <arg direction=\"out\" type=\"i\" name=\"cu"
    "rrent_state\"/>\n    </signal>\n  </interface>\n\0"
    "QosNofity\0\0output_garray\0notify_index\0"
    "RunningStateChanged\0media_id\0current_state\0"
    "Destroy\0GetMediaInfo\0info_type\0Pause\0"
    "Run\0SetMediaInfo\0action_id\0input_garray\0"
    "Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvStreamedMediaAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       8,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    2,   58,    6, 0x06 /* Public */,
       9,    2,   63,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   68,    6, 0x0a /* Public */,
      13,    1,   69,    6, 0x0a /* Public */,
      15,    0,   72,    6, 0x0a /* Public */,
      16,    0,   73,    6, 0x0a /* Public */,
      17,    2,   74,    6, 0x0a /* Public */,
      20,    0,   79,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::QByteArray, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   18,   19,
    QMetaType::Void,

       0        // eod
};

void CvStreamedMediaAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvStreamedMediaAdaptor *_t = static_cast<CvStreamedMediaAdaptor *>(_o);
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
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvStreamedMediaAdaptor::*_t)(const QByteArray & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMediaAdaptor::QosNofity)) {
                *result = 0;
            }
        }
        {
            typedef void (CvStreamedMediaAdaptor::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvStreamedMediaAdaptor::RunningStateChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CvStreamedMediaAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_CvStreamedMediaAdaptor.data,
      qt_meta_data_CvStreamedMediaAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvStreamedMediaAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvStreamedMediaAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvStreamedMediaAdaptor.stringdata))
        return static_cast<void*>(const_cast< CvStreamedMediaAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CvStreamedMediaAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CvStreamedMediaAdaptor::QosNofity(const QByteArray & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CvStreamedMediaAdaptor::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
