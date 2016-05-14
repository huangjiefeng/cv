/****************************************************************************
** Meta object code from reading C++ file 'QtPhoneLine.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/phoneline/QtPhoneLine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtPhoneLine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtPhoneLine[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      37,   33,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtPhoneLine[] = {
    "QtPhoneLine\0\0callButtonClicked()\0url\0"
    "openURLRequest(QString)\0"
};

const QMetaObject QtPhoneLine::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtPhoneLine,
      qt_meta_data_QtPhoneLine, 0 }
};

const QMetaObject *QtPhoneLine::metaObject() const
{
    return &staticMetaObject;
}

void *QtPhoneLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtPhoneLine))
        return static_cast<void*>(const_cast< QtPhoneLine*>(this));
    if (!strcmp(_clname, "PPhoneLine"))
        return static_cast< PPhoneLine*>(const_cast< QtPhoneLine*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtPhoneLine*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtPhoneLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: callButtonClicked(); break;
        case 1: openURLRequest((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
