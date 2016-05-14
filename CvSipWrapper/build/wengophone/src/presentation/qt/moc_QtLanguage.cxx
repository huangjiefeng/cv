/****************************************************************************
** Meta object code from reading C++ file 'QtLanguage.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtLanguage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtLanguage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtLanguage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QtLanguage[] = {
    "QtLanguage\0"
};

const QMetaObject QtLanguage::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtLanguage,
      qt_meta_data_QtLanguage, 0 }
};

const QMetaObject *QtLanguage::metaObject() const
{
    return &staticMetaObject;
}

void *QtLanguage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtLanguage))
        return static_cast<void*>(const_cast< QtLanguage*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtLanguage*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtLanguage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
