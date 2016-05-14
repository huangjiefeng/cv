/****************************************************************************
** Meta object code from reading C++ file 'QtLanguagesSettings.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/config/QtLanguagesSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtLanguagesSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtLanguagesSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtLanguagesSettings[] = {
    "QtLanguagesSettings\0\0link\0"
    "launchpadLinkClicked(QString)\0"
};

const QMetaObject QtLanguagesSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtLanguagesSettings,
      qt_meta_data_QtLanguagesSettings, 0 }
};

const QMetaObject *QtLanguagesSettings::metaObject() const
{
    return &staticMetaObject;
}

void *QtLanguagesSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtLanguagesSettings))
        return static_cast<void*>(const_cast< QtLanguagesSettings*>(this));
    if (!strcmp(_clname, "QtISettings"))
        return static_cast< QtISettings*>(const_cast< QtLanguagesSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtLanguagesSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: launchpadLinkClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
