/****************************************************************************
** Meta object code from reading C++ file 'QtPhoneCall.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/phonecall/QtPhoneCall.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtPhoneCall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtPhoneCall[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   12,   12,   12, 0x0a,
      57,   12,   12,   12, 0x0a,
      70,   12,   12,   12, 0x0a,
      83,   12,   12,   12, 0x0a,
     106,   12,   12,   12, 0x08,
     127,   12,   12,   12, 0x08,
     147,   12,   12,   12, 0x08,
     160,   12,   12,   12, 0x08,
     180,   12,   12,   12, 0x08,
     200,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtPhoneCall[] = {
    "QtPhoneCall\0\0startedTalking(QtPhoneCall*)\0"
    "holdOrResume()\0acceptCall()\0rejectCall()\0"
    "toggleFlipVideoImage()\0updateCallDuration()\0"
    "updateStatusLabel()\0addContact()\0"
    "toggleSmileys(bool)\0toggleDialpad(bool)\0"
    "createConf()\0"
};

const QMetaObject QtPhoneCall::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtPhoneCall,
      qt_meta_data_QtPhoneCall, 0 }
};

const QMetaObject *QtPhoneCall::metaObject() const
{
    return &staticMetaObject;
}

void *QtPhoneCall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtPhoneCall))
        return static_cast<void*>(const_cast< QtPhoneCall*>(this));
    if (!strcmp(_clname, "PPhoneCall"))
        return static_cast< PPhoneCall*>(const_cast< QtPhoneCall*>(this));
    return QObject::qt_metacast(_clname);
}

int QtPhoneCall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startedTalking((*reinterpret_cast< QtPhoneCall*(*)>(_a[1]))); break;
        case 1: holdOrResume(); break;
        case 2: acceptCall(); break;
        case 3: rejectCall(); break;
        case 4: toggleFlipVideoImage(); break;
        case 5: updateCallDuration(); break;
        case 6: updateStatusLabel(); break;
        case 7: addContact(); break;
        case 8: toggleSmileys((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: toggleDialpad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: createConf(); break;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QtPhoneCall::startedTalking(QtPhoneCall * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
