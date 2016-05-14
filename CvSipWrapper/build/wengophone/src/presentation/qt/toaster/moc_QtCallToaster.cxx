/****************************************************************************
** Meta object code from reading C++ file 'QtCallToaster.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/toaster/QtCallToaster.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtCallToaster.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCallToaster[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      37,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   14,   14,   14, 0x0a,
      67,   14,   14,   14, 0x08,
      86,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtCallToaster[] = {
    "QtCallToaster\0\0hangUpButtonClicked()\0"
    "pickUpButtonClicked()\0close()\0"
    "hangUpButtonSlot()\0pickUpButtonSlot()\0"
};

const QMetaObject QtCallToaster::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtCallToaster,
      qt_meta_data_QtCallToaster, 0 }
};

const QMetaObject *QtCallToaster::metaObject() const
{
    return &staticMetaObject;
}

void *QtCallToaster::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCallToaster))
        return static_cast<void*>(const_cast< QtCallToaster*>(this));
    if (!strcmp(_clname, "IQtToaster"))
        return static_cast< IQtToaster*>(const_cast< QtCallToaster*>(this));
    return QObject::qt_metacast(_clname);
}

int QtCallToaster::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: hangUpButtonClicked(); break;
        case 1: pickUpButtonClicked(); break;
        case 2: close(); break;
        case 3: hangUpButtonSlot(); break;
        case 4: pickUpButtonSlot(); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QtCallToaster::hangUpButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtCallToaster::pickUpButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
