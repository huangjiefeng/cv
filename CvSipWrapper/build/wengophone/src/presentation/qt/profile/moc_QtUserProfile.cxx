/****************************************************************************
** Meta object code from reading C++ file 'QtUserProfile.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profile/QtUserProfile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtUserProfile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtUserProfile[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   15,   14,   14, 0x05,
      82,   15,   14,   14, 0x05,
     171,  146,   14,   14, 0x05,
     282,  246,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     364,   15,   14,   14, 0x08,
     415,   15,   14,   14, 0x08,
     477,  146,   14,   14, 0x08,
     550,  246,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtUserProfile[] = {
    "QtUserProfile\0\0sender,iState\0"
    "loginStateChangedEventHandlerSignal(SipAccount*,int)\0"
    "networkDiscoveryStateChangedEventHandlerSignal(SipAccount*,int)\0"
    "sender,imContact,message\0"
    "authorizationRequestEventHandlerSignal(PresenceHandler*,IMContact,QStr"
    "ing)\0"
    "sender,imaccountId,sid,from,evtType\0"
    "incomingSubscribeEventHandlerSignal(PresenceHandler*,QString,int,QStri"
    "ng,QString)\0"
    "loginStateChangedEventHandlerSlot(SipAccount*,int)\0"
    "networkDiscoveryStateChangedEventHandlerSlot(SipAccount*,int)\0"
    "authorizationRequestEventHandlerSlot(PresenceHandler*,IMContact,QStrin"
    "g)\0"
    "incomingSubscribeEventHandlerSlot(PresenceHandler*,QString,int,QString"
    ",QString)\0"
};

const QMetaObject QtUserProfile::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtUserProfile,
      qt_meta_data_QtUserProfile, 0 }
};

const QMetaObject *QtUserProfile::metaObject() const
{
    return &staticMetaObject;
}

void *QtUserProfile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtUserProfile))
        return static_cast<void*>(const_cast< QtUserProfile*>(this));
    if (!strcmp(_clname, "PUserProfile"))
        return static_cast< PUserProfile*>(const_cast< QtUserProfile*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtUserProfile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loginStateChangedEventHandlerSignal((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: networkDiscoveryStateChangedEventHandlerSignal((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: authorizationRequestEventHandlerSignal((*reinterpret_cast< PresenceHandler*(*)>(_a[1])),(*reinterpret_cast< IMContact(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: incomingSubscribeEventHandlerSignal((*reinterpret_cast< PresenceHandler*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 4: loginStateChangedEventHandlerSlot((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: networkDiscoveryStateChangedEventHandlerSlot((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: authorizationRequestEventHandlerSlot((*reinterpret_cast< PresenceHandler*(*)>(_a[1])),(*reinterpret_cast< IMContact(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: incomingSubscribeEventHandlerSlot((*reinterpret_cast< PresenceHandler*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QtUserProfile::loginStateChangedEventHandlerSignal(SipAccount * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtUserProfile::networkDiscoveryStateChangedEventHandlerSignal(SipAccount * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtUserProfile::authorizationRequestEventHandlerSignal(PresenceHandler * _t1, IMContact _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtUserProfile::incomingSubscribeEventHandlerSignal(PresenceHandler * _t1, const QString & _t2, int _t3, const QString & _t4, const QString & _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
