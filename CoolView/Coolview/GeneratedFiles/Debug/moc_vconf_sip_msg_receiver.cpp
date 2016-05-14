/****************************************************************************
** Meta object code from reading C++ file 'vconf_sip_msg_receiver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vconf_sip_msg_receiver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vconf_sip_msg_receiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VConfSipMsgReceiver_t {
    QByteArrayData data[9];
    char stringdata[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VConfSipMsgReceiver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VConfSipMsgReceiver_t qt_meta_stringdata_VConfSipMsgReceiver = {
    {
QT_MOC_LITERAL(0, 0, 19), // "VConfSipMsgReceiver"
QT_MOC_LITERAL(1, 20, 29), // "HandleVRecvConferenceListSlot"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 33), // "HandleVRecvQoSLoginPermission..."
QT_MOC_LITERAL(4, 85, 27), // "HandleVRecvTerminalListSlot"
QT_MOC_LITERAL(5, 113, 12), // "TerminalList"
QT_MOC_LITERAL(6, 126, 30), // "HandleVRecvStartMediaReplySlot"
QT_MOC_LITERAL(7, 157, 15), // "MediaRelayParam"
QT_MOC_LITERAL(8, 173, 5) // "param"

    },
    "VConfSipMsgReceiver\0HandleVRecvConferenceListSlot\0"
    "\0HandleVRecvQoSLoginPermissionSlot\0"
    "HandleVRecvTerminalListSlot\0TerminalList\0"
    "HandleVRecvStartMediaReplySlot\0"
    "MediaRelayParam\0param"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VConfSipMsgReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    1,   36,    2, 0x0a /* Public */,
       6,    1,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void VConfSipMsgReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VConfSipMsgReceiver *_t = static_cast<VConfSipMsgReceiver *>(_o);
        switch (_id) {
        case 0: _t->HandleVRecvConferenceListSlot(); break;
        case 1: _t->HandleVRecvQoSLoginPermissionSlot(); break;
        case 2: _t->HandleVRecvTerminalListSlot((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 3: _t->HandleVRecvStartMediaReplySlot((*reinterpret_cast< const MediaRelayParam(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VConfSipMsgReceiver::staticMetaObject = {
    { &SipMsgReceiver::staticMetaObject, qt_meta_stringdata_VConfSipMsgReceiver.data,
      qt_meta_data_VConfSipMsgReceiver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VConfSipMsgReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VConfSipMsgReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VConfSipMsgReceiver.stringdata))
        return static_cast<void*>(const_cast< VConfSipMsgReceiver*>(this));
    return SipMsgReceiver::qt_metacast(_clname);
}

int VConfSipMsgReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SipMsgReceiver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
