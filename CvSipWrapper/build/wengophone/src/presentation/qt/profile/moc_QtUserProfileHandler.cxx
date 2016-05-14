/****************************************************************************
** Meta object code from reading C++ file 'QtUserProfileHandler.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profile/QtUserProfileHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtUserProfileHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtUserProfileHandler[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      66,   21,   21,   21, 0x05,
     112,   21,   21,   21, 0x05,
     167,  155,   21,   21, 0x05,
     270,  258,   21,   21, 0x05,
     322,  258,   21,   21, 0x05,
     382,  371,   21,   21, 0x05,
     429,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     471,   21,   21,   21, 0x0a,
     502,  489,   21,   21, 0x0a,
     548,   21,   21,   21, 0x08,
     590,   21,   21,   21, 0x08,
     634,   21,   21,   21, 0x08,
     675,  155,   21,   21, 0x08,
     764,  258,   21,   21, 0x08,
     814,  258,   21,   21, 0x08,
     861,  371,   21,   21, 0x08,
     906,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtUserProfileHandler[] = {
    "QtUserProfileHandler\0\0"
    "noCurrentUserProfileSetEventHandlerSignal()\0"
    "currentUserProfileWillDieEventHandlerSignal()\0"
    "userProfileInitializedEventHandlerSignal()\0"
    "sipAccount,\0"
    "sipAccountConnectionFailedEventHandlerSignal(SipAccount*,EnumSipLoginS"
    "tate::SipLoginState)\0"
    "profileName\0"
    "profileLoadedFromBackupsEventHandlerSignal(QString)\0"
    "profileCannotBeLoadedEventHandlerSignal(QString)\0"
    "sipAccount\0sipNetworkErrorEventHandlerSignal(SipAccount*)\0"
    "setCurrentUserProfileEventHandlerSignal()\0"
    "showLoginWindow()\0wengoAccount\0"
    "showLoginWindowWithWengoAccount(WengoAccount)\0"
    "noCurrentUserProfileSetEventHandlerSlot()\0"
    "currentUserProfileWillDieEventHandlerSlot()\0"
    "userProfileInitializedEventHandlerSlot()\0"
    "sipAccountConnectionFailedEventHandlerSlot(SipAccount*,EnumSipLoginSta"
    "te::SipLoginState)\0"
    "profileLoadedFromBackupsEventHandlerSlot(QString)\0"
    "profileCannotBeLoadedEventHandlerSlot(QString)\0"
    "sipNetworkErrorEventHandlerSlot(SipAccount*)\0"
    "setCurrentUserProfileEventHandlerSlot()\0"
};

const QMetaObject QtUserProfileHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtUserProfileHandler,
      qt_meta_data_QtUserProfileHandler, 0 }
};

const QMetaObject *QtUserProfileHandler::metaObject() const
{
    return &staticMetaObject;
}

void *QtUserProfileHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtUserProfileHandler))
        return static_cast<void*>(const_cast< QtUserProfileHandler*>(this));
    if (!strcmp(_clname, "PUserProfileHandler"))
        return static_cast< PUserProfileHandler*>(const_cast< QtUserProfileHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int QtUserProfileHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: noCurrentUserProfileSetEventHandlerSignal(); break;
        case 1: currentUserProfileWillDieEventHandlerSignal(); break;
        case 2: userProfileInitializedEventHandlerSignal(); break;
        case 3: sipAccountConnectionFailedEventHandlerSignal((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< EnumSipLoginState::SipLoginState(*)>(_a[2]))); break;
        case 4: profileLoadedFromBackupsEventHandlerSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: profileCannotBeLoadedEventHandlerSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: sipNetworkErrorEventHandlerSignal((*reinterpret_cast< SipAccount*(*)>(_a[1]))); break;
        case 7: setCurrentUserProfileEventHandlerSignal(); break;
        case 8: showLoginWindow(); break;
        case 9: showLoginWindowWithWengoAccount((*reinterpret_cast< const WengoAccount(*)>(_a[1]))); break;
        case 10: noCurrentUserProfileSetEventHandlerSlot(); break;
        case 11: currentUserProfileWillDieEventHandlerSlot(); break;
        case 12: userProfileInitializedEventHandlerSlot(); break;
        case 13: sipAccountConnectionFailedEventHandlerSlot((*reinterpret_cast< SipAccount*(*)>(_a[1])),(*reinterpret_cast< EnumSipLoginState::SipLoginState(*)>(_a[2]))); break;
        case 14: profileLoadedFromBackupsEventHandlerSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: profileCannotBeLoadedEventHandlerSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: sipNetworkErrorEventHandlerSlot((*reinterpret_cast< SipAccount*(*)>(_a[1]))); break;
        case 17: setCurrentUserProfileEventHandlerSlot(); break;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void QtUserProfileHandler::noCurrentUserProfileSetEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtUserProfileHandler::currentUserProfileWillDieEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QtUserProfileHandler::userProfileInitializedEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QtUserProfileHandler::sipAccountConnectionFailedEventHandlerSignal(SipAccount * _t1, EnumSipLoginState::SipLoginState _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtUserProfileHandler::profileLoadedFromBackupsEventHandlerSignal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtUserProfileHandler::profileCannotBeLoadedEventHandlerSignal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtUserProfileHandler::sipNetworkErrorEventHandlerSignal(SipAccount * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QtUserProfileHandler::setCurrentUserProfileEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
