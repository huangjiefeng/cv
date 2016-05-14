/****************************************************************************
** Meta object code from reading C++ file 'CvScreenMediaAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/channel/type/screenMedia/service/CvScreenMediaAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvScreenMediaAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvScreenMediaAdaptor_t {
    QByteArrayData data[18];
    char stringdata[1332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvScreenMediaAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvScreenMediaAdaptor_t qt_meta_stringdata_CvScreenMediaAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CvScreenMediaAdaptor"
QT_MOC_LITERAL(1, 21, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 37, 45), // "com.dcampus.coolview.channel...."
QT_MOC_LITERAL(3, 83, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 103, 1102), // "  <interface name=\"com.dcamp..."
QT_MOC_LITERAL(5, 1121, 19), // "RunningStateChanged"
QT_MOC_LITERAL(6, 1141, 0), // ""
QT_MOC_LITERAL(7, 1142, 8), // "media_id"
QT_MOC_LITERAL(8, 1151, 13), // "current_state"
QT_MOC_LITERAL(9, 1165, 7), // "Destroy"
QT_MOC_LITERAL(10, 1173, 12), // "GetMediaInfo"
QT_MOC_LITERAL(11, 1186, 9), // "info_type"
QT_MOC_LITERAL(12, 1196, 5), // "Pause"
QT_MOC_LITERAL(13, 1202, 3), // "Run"
QT_MOC_LITERAL(14, 1206, 12), // "SetMediaInfo"
QT_MOC_LITERAL(15, 1219, 9), // "action_id"
QT_MOC_LITERAL(16, 1229, 12), // "input_garray"
QT_MOC_LITERAL(17, 1242, 4) // "Stop"

    },
    "CvScreenMediaAdaptor\0D-Bus Interface\0"
    "com.dcampus.coolview.channel.type.screenMedia\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.dcampus.coolview.channel.type.screenMedia\">\n"
    "    <method name=\"SetMediaInfo\">\n      <annotation value=\"__client"
    "_request_cb\" name=\"org.freedesktop.DBus.GLib.CSymbol\"/>\n      <arg"
    " direction=\"in\" type=\"i\" name=\"action_id\"/>\n      <arg directio"
    "n=\"in\" type=\"ay\" name=\"input_garray\"/>\n      <!--set media para"
    "ms-->\n    </method>\n    <method name=\"GetMediaInfo\">\n      <!--ge"
    "t media info-->\n      <arg direction=\"in\" type=\"i\" name=\"info_ty"
    "pe\"/>\n      <arg direction=\"out\" type=\"ay\" name=\"output_garray\""
    "/>\n    </method>\n    <method name=\"Run\">\n      <!--run proc : thi"
    "s will cause screen captrue-->\n    </method>\n    <method name=\"Paus"
    "e\">\n      <!--pause sending image file-->\n    </method>\n    <metho"
    "d name=\"Stop\">\n      <!--stop proc-->\n    </method>\n    <method n"
    "ame=\"Destroy\">\n      <!--destroy proc-->\n    </method>\n    <signa"
    "l name=\"RunningStateChanged\">\n      <arg direction=\"out\" type=\"s"
    "\" name=\"media_id\"/>\n      <arg direction=\"out\" type=\"i\" name=\""
    "current_state\"/>\n    </signal>\n  </interface>\n\0"
    "RunningStateChanged\0\0media_id\0"
    "current_state\0Destroy\0GetMediaInfo\0"
    "info_type\0Pause\0Run\0SetMediaInfo\0"
    "action_id\0input_garray\0Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvScreenMediaAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       7,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags
       5,    2,   53,    6, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   58,    6, 0x0a /* Public */,
      10,    1,   59,    6, 0x0a /* Public */,
      12,    0,   62,    6, 0x0a /* Public */,
      13,    0,   63,    6, 0x0a /* Public */,
      14,    2,   64,    6, 0x0a /* Public */,
      17,    0,   69,    6, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::QByteArray, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,   15,   16,
    QMetaType::Void,

       0        // eod
};

void CvScreenMediaAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvScreenMediaAdaptor *_t = static_cast<CvScreenMediaAdaptor *>(_o);
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
            typedef void (CvScreenMediaAdaptor::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvScreenMediaAdaptor::RunningStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvScreenMediaAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_CvScreenMediaAdaptor.data,
      qt_meta_data_CvScreenMediaAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvScreenMediaAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvScreenMediaAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvScreenMediaAdaptor.stringdata))
        return static_cast<void*>(const_cast< CvScreenMediaAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CvScreenMediaAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void CvScreenMediaAdaptor::RunningStateChanged(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
