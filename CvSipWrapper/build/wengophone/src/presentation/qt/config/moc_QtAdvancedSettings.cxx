/****************************************************************************
** Meta object code from reading C++ file 'QtAdvancedSettings.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/config/QtAdvancedSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAdvancedSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtAdvancedSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtAdvancedSettings[] = {
    "QtAdvancedSettings\0\0showAdvancedConfigWindow()\0"
};

const QMetaObject QtAdvancedSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtAdvancedSettings,
      qt_meta_data_QtAdvancedSettings, 0 }
};

const QMetaObject *QtAdvancedSettings::metaObject() const
{
    return &staticMetaObject;
}

void *QtAdvancedSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtAdvancedSettings))
        return static_cast<void*>(const_cast< QtAdvancedSettings*>(this));
    if (!strcmp(_clname, "QtISettings"))
        return static_cast< QtISettings*>(const_cast< QtAdvancedSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtAdvancedSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showAdvancedConfigWindow(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
