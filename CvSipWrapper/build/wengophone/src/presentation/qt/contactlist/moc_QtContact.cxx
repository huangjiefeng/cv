/****************************************************************************
** Meta object code from reading C++ file 'QtContact.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/contactlist/QtContact.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtContact.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtContact[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      25,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtContact[] = {
    "QtContact\0\0qtContact,prt\0"
    "clicked(QtContact*,int)\0contactUpdated()\0"
};

const QMetaObject QtContact::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtContact,
      qt_meta_data_QtContact, 0 }
};

const QMetaObject *QtContact::metaObject() const
{
    return &staticMetaObject;
}

void *QtContact::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtContact))
        return static_cast<void*>(const_cast< QtContact*>(this));
    return QObject::qt_metacast(_clname);
}

int QtContact::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< QtContact*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: contactUpdated(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QtContact::clicked(QtContact * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
