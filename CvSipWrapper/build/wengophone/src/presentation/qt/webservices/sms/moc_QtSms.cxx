/****************************************************************************
** Meta object code from reading C++ file 'QtSms.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../wengophone/src/presentation/qt/webservices/sms/QtSms.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtSms.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSms[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      23,    6,    6,    6, 0x08,
      43,    6,    6,    6, 0x08,
      80,   73,    6,    6, 0x08,
     116,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtSms[] = {
    "QtSms\0\0updateCounter()\0sendButtonClicked()\0"
    "addContactToolButtonClicked()\0action\0"
    "updatePhoneNumberLineEdit(QAction*)\0"
    "CloseEventFilterSlot()\0"
};

const QMetaObject QtSms::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtSms,
      qt_meta_data_QtSms, 0 }
};

const QMetaObject *QtSms::metaObject() const
{
    return &staticMetaObject;
}

void *QtSms::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSms))
        return static_cast<void*>(const_cast< QtSms*>(this));
    if (!strcmp(_clname, "PSms"))
        return static_cast< PSms*>(const_cast< QtSms*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtSms*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtSms::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateCounter(); break;
        case 1: sendButtonClicked(); break;
        case 2: addContactToolButtonClicked(); break;
        case 3: updatePhoneNumberLineEdit((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 4: CloseEventFilterSlot(); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
