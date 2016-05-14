/****************************************************************************
** Meta object code from reading C++ file 'sip_msg_receiver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sip_msg_receiver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sip_msg_receiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SipMsgReceiver_t {
    QByteArrayData data[5];
    char stringdata[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SipMsgReceiver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SipMsgReceiver_t qt_meta_stringdata_SipMsgReceiver = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SipMsgReceiver"
QT_MOC_LITERAL(1, 15, 31), // "HandleMessageReceivedNotifySlot"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 3), // "msg"
QT_MOC_LITERAL(4, 52, 4) // "from"

    },
    "SipMsgReceiver\0HandleMessageReceivedNotifySlot\0"
    "\0msg\0from"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SipMsgReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

       0        // eod
};

void SipMsgReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SipMsgReceiver *_t = static_cast<SipMsgReceiver *>(_o);
        switch (_id) {
        case 0: _t->HandleMessageReceivedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject SipMsgReceiver::staticMetaObject = {
    { &IMsgReceiver::staticMetaObject, qt_meta_stringdata_SipMsgReceiver.data,
      qt_meta_data_SipMsgReceiver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SipMsgReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SipMsgReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SipMsgReceiver.stringdata))
        return static_cast<void*>(const_cast< SipMsgReceiver*>(this));
    return IMsgReceiver::qt_metacast(_clname);
}

int SipMsgReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IMsgReceiver::qt_metacall(_c, _id, _a);
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
