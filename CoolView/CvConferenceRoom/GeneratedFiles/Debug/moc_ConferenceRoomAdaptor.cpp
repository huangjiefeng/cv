/****************************************************************************
** Meta object code from reading C++ file 'ConferenceRoomAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/conferenceRoom/service/ConferenceRoomAdaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConferenceRoomAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConferenceRoomAdaptor_t {
    QByteArrayData data[26];
    char stringdata[2484];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceRoomAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceRoomAdaptor_t qt_meta_stringdata_ConferenceRoomAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ConferenceRoomAdaptor"
QT_MOC_LITERAL(1, 22, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 38, 30), // "com.coolview.ui.conferenceroom"
QT_MOC_LITERAL(3, 69, 19), // "D-Bus Introspection"
QT_MOC_LITERAL(4, 89, 2141), // "  <interface name=\"com.coolv..."
QT_MOC_LITERAL(5, 2053, 14), // "AddMediaWindow"
QT_MOC_LITERAL(6, 2068, 0), // ""
QT_MOC_LITERAL(7, 2069, 4), // "send"
QT_MOC_LITERAL(8, 2074, 12), // "input_garray"
QT_MOC_LITERAL(9, 2087, 11), // "screenIndex"
QT_MOC_LITERAL(10, 2099, 4), // "seet"
QT_MOC_LITERAL(11, 2104, 12), // "ChangeLayout"
QT_MOC_LITERAL(12, 2117, 12), // "displayModel"
QT_MOC_LITERAL(13, 2130, 16), // "CloseMediaWindow"
QT_MOC_LITERAL(14, 2147, 7), // "user_id"
QT_MOC_LITERAL(15, 2155, 11), // "CloseWindow"
QT_MOC_LITERAL(16, 2167, 14), // "ExitConference"
QT_MOC_LITERAL(17, 2182, 17), // "ModifyMediaWindow"
QT_MOC_LITERAL(18, 2200, 10), // "actionIdex"
QT_MOC_LITERAL(19, 2211, 9), // "arguments"
QT_MOC_LITERAL(20, 2221, 19), // "OpenScreenShareFile"
QT_MOC_LITERAL(21, 2241, 8), // "filePath"
QT_MOC_LITERAL(22, 2250, 24), // "RecoveryRecvMediaProcess"
QT_MOC_LITERAL(23, 2275, 11), // "ShowRtcpMsg"
QT_MOC_LITERAL(24, 2287, 4), // "show"
QT_MOC_LITERAL(25, 2292, 13) // "UpdateRtcpMsg"

    },
    "ConferenceRoomAdaptor\0D-Bus Interface\0"
    "com.coolview.ui.conferenceroom\0"
    "D-Bus Introspection\0"
    "  <interface name=\"com.coolview.ui.conferenceroom\">\n    <method nam"
    "e=\"AddMediaWindow\">\n      <annotation value=\"__client_request_cb\""
    " name=\"org.freedesktop.DBus.GLib.CSymbol\"/>\n      <arg direction=\""
    "in\" type=\"b\" name=\"send\"/>  	     //send or receive&#xd;\n       "
    "<arg direction=\"in\" type=\"ay\" name=\"input_garray\"/>  //MediaInfo"
    "&#xd;\n	   <arg direction=\"in\" type=\"i\" name=\"screenIndex\"/>  //"
    "Screen Index&#xd;\n       <arg direction=\"in\" type=\"i\" name=\"seet"
    "\"/> 	     //video frame position&#xd;\n    </method>\n    <method nam"
    "e=\"CloseMediaWindow\">\n      <arg direction=\"in\" type=\"s\" name=\""
    "user_id\"/>  //user id&#xd;\n    </method>\n    <method name=\"ChangeL"
    "ayout\">\n      <arg direction=\"in\" type=\"i\" name=\"screenIndex\"/"
    ">	//Screen Index&#xd;\n       <arg direction=\"in\" type=\"i\" name=\""
    "displayModel\"/>  //display model for video windows&#xd;\n    </method"
    ">\n    <method name=\"ShowRtcpMsg\">\n      <arg direction=\"in\" type"
    "=\"i\" name=\"screenIndex\"/>\n      <arg direction=\"in\" type=\"b\" "
    "name=\"show\"/>\n    </method>\n    <method name=\"UpdateRtcpMsg\">\n "
    "     <annotation value=\"__client_request_cb\" name=\"org.freedesktop."
    "DBus.GLib.CSymbol\"/>\n      <arg direction=\"in\" type=\"ay\" name=\""
    "input_garray\"/>  //RtcpInfo&#xd;\n    </method>\n    <method name=\"M"
    "odifyMediaWindow\">\n      <arg direction=\"in\" type=\"s\" name=\"use"
    "r_id\"/>\n      <arg direction=\"in\" type=\"i\" name=\"actionIdex\"/>"
    "\n      <arg direction=\"in\" type=\"ay\" name=\"arguments\"/>\n    </"
    "method>\n    <method name=\"OpenScreenShareFile\">\n      <arg directi"
    "on=\"in\" type=\"s\" name=\"filePath\"/>\n      <arg direction=\"in\" "
    "type=\"i\" name=\"screenIndex\"/>\n    </method>\n    <method name=\"C"
    "loseWindow\"/>\n    <method name=\"ExitConference\"/>\n    <method nam"
    "e=\"RecoveryRecvMediaProcess\">\n      <arg direction=\"in\" type=\"ay"
    "\" name=\"input_garray\"/>  //MediaInfo&#xd;\n	   <arg direction=\"in\""
    " type=\"i\" name=\"screenIndex\"/>  //Screen Index&#xd;\n       <arg d"
    "irection=\"in\" type=\"i\" name=\"seet\"/> 	     //video frame positio"
    "n&#xd;\n    </method>\n  </interface>\n\0"
    "AddMediaWindow\0\0send\0input_garray\0"
    "screenIndex\0seet\0ChangeLayout\0"
    "displayModel\0CloseMediaWindow\0user_id\0"
    "CloseWindow\0ExitConference\0ModifyMediaWindow\0"
    "actionIdex\0arguments\0OpenScreenShareFile\0"
    "filePath\0RecoveryRecvMediaProcess\0"
    "ShowRtcpMsg\0show\0UpdateRtcpMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceRoomAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
      10,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // slots: name, argc, parameters, tag, flags
       5,    4,   68,    6, 0x0a /* Public */,
      11,    2,   77,    6, 0x0a /* Public */,
      13,    1,   82,    6, 0x0a /* Public */,
      15,    0,   85,    6, 0x0a /* Public */,
      16,    0,   86,    6, 0x0a /* Public */,
      17,    3,   87,    6, 0x0a /* Public */,
      20,    2,   94,    6, 0x0a /* Public */,
      22,    3,   99,    6, 0x0a /* Public */,
      23,    2,  106,    6, 0x0a /* Public */,
      25,    1,  111,    6, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   14,   18,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   21,    9,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    9,   24,
    QMetaType::Void, QMetaType::QByteArray,    8,

       0        // eod
};

void ConferenceRoomAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConferenceRoomAdaptor *_t = static_cast<ConferenceRoomAdaptor *>(_o);
        switch (_id) {
        case 0: _t->AddMediaWindow((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->ChangeLayout((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->CloseMediaWindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->CloseWindow(); break;
        case 4: _t->ExitConference(); break;
        case 5: _t->ModifyMediaWindow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 6: _t->OpenScreenShareFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->RecoveryRecvMediaProcess((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->ShowRtcpMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->UpdateRtcpMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ConferenceRoomAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_ConferenceRoomAdaptor.data,
      qt_meta_data_ConferenceRoomAdaptor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConferenceRoomAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceRoomAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceRoomAdaptor.stringdata))
        return static_cast<void*>(const_cast< ConferenceRoomAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int ConferenceRoomAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
