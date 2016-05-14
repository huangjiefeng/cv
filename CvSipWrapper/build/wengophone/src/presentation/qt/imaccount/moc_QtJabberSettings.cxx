/****************************************************************************
** Meta object code from reading C++ file 'QtJabberSettings.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/imaccount/QtJabberSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtJabberSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtJabberSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QtJabberSettings[] = {
    "QtJabberSettings\0"
};

const QMetaObject QtJabberSettings::staticMetaObject = {
    { &QtIMAccountPlugin::staticMetaObject, qt_meta_stringdata_QtJabberSettings,
      qt_meta_data_QtJabberSettings, 0 }
};

const QMetaObject *QtJabberSettings::metaObject() const
{
    return &staticMetaObject;
}

void *QtJabberSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtJabberSettings))
        return static_cast<void*>(const_cast< QtJabberSettings*>(this));
    return QtIMAccountPlugin::qt_metacast(_clname);
}

int QtJabberSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtIMAccountPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
