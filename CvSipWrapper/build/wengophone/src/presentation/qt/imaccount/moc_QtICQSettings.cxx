/****************************************************************************
** Meta object code from reading C++ file 'QtICQSettings.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/imaccount/QtICQSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtICQSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtICQSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      45,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtICQSettings[] = {
    "QtICQSettings\0\0forgotPasswordButtonClicked()\0"
    "createAccountButtonClicked()\0"
};

const QMetaObject QtICQSettings::staticMetaObject = {
    { &QtIMAccountPlugin::staticMetaObject, qt_meta_stringdata_QtICQSettings,
      qt_meta_data_QtICQSettings, 0 }
};

const QMetaObject *QtICQSettings::metaObject() const
{
    return &staticMetaObject;
}

void *QtICQSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtICQSettings))
        return static_cast<void*>(const_cast< QtICQSettings*>(this));
    return QtIMAccountPlugin::qt_metacast(_clname);
}

int QtICQSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtIMAccountPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: forgotPasswordButtonClicked(); break;
        case 1: createAccountButtonClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE