/****************************************************************************
** Meta object code from reading C++ file 'QtProfileBar.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profilebar/QtProfileBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtProfileBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtProfileBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,
      52,   46,   13,   13, 0x05,
      85,   78,   13,   13, 0x05,
     125,  120,   13,   13, 0x05,
     157,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     181,   13,   13,   13, 0x0a,
     197,   13,   13,   13, 0x0a,
     215,   13,   13,   13, 0x0a,
     231,   14,   13,   13, 0x08,
     260,   46,   13,   13, 0x08,
     290,   78,   13,   13, 0x08,
     329,  120,   13,   13, 0x08,
     365,   13,   13,   13, 0x08,
     393,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtProfileBar[] = {
    "QtProfileBar\0\0wengos\0wsInfoWengosEvent(float)\0"
    "count\0wsInfoVoiceMailEvent(int)\0number\0"
    "wsInfoLandlineNumberEvent(QString)\0"
    "mode\0wsCallForwardInfoEvent(QString)\0"
    "phoneLineCreatedEvent()\0statusClicked()\0"
    "nicknameClicked()\0creditClicked()\0"
    "wsInfoWengosEventSlot(float)\0"
    "wsInfoVoiceMailEventSlot(int)\0"
    "wsInfoLandlineNumberEventSlot(QString)\0"
    "wsCallForwardInfoEventSlot(QString)\0"
    "phoneLineCreatedEventSlot()\0"
    "updateStatusLabel()\0"
};

const QMetaObject QtProfileBar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QtProfileBar,
      qt_meta_data_QtProfileBar, 0 }
};

const QMetaObject *QtProfileBar::metaObject() const
{
    return &staticMetaObject;
}

void *QtProfileBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtProfileBar))
        return static_cast<void*>(const_cast< QtProfileBar*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtProfileBar*>(this));
    return QFrame::qt_metacast(_clname);
}

int QtProfileBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: wsInfoWengosEvent((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: wsInfoVoiceMailEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: wsInfoLandlineNumberEvent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: wsCallForwardInfoEvent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: phoneLineCreatedEvent(); break;
        case 5: statusClicked(); break;
        case 6: nicknameClicked(); break;
        case 7: creditClicked(); break;
        case 8: wsInfoWengosEventSlot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: wsInfoVoiceMailEventSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: wsInfoLandlineNumberEventSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: wsCallForwardInfoEventSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: phoneLineCreatedEventSlot(); break;
        case 13: updateStatusLabel(); break;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void QtProfileBar::wsInfoWengosEvent(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtProfileBar::wsInfoVoiceMailEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtProfileBar::wsInfoLandlineNumberEvent(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtProfileBar::wsCallForwardInfoEvent(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtProfileBar::phoneLineCreatedEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
