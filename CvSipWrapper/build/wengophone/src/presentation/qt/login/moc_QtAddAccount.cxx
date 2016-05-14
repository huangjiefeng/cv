/****************************************************************************
** Meta object code from reading C++ file 'QtAddAccount.h'
**
** Created: Thu Jul 29 12:39:43 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/login/QtAddAccount.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAddAccount.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtAddAccount[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x08,
      48,   13,   13,   13, 0x08,
      68,   13,   13,   13, 0x08,
      82,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtAddAccount[] = {
    "QtAddAccount\0\0url\0createAccountClicked(QString)\0"
    "helpButtonClicked()\0nextClicked()\0"
    "languageChanged()\0"
};

const QMetaObject QtAddAccount::staticMetaObject = {
    { &ILogin::staticMetaObject, qt_meta_stringdata_QtAddAccount,
      qt_meta_data_QtAddAccount, 0 }
};

const QMetaObject *QtAddAccount::metaObject() const
{
    return &staticMetaObject;
}

void *QtAddAccount::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtAddAccount))
        return static_cast<void*>(const_cast< QtAddAccount*>(this));
    return ILogin::qt_metacast(_clname);
}

int QtAddAccount::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ILogin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createAccountClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: helpButtonClicked(); break;
        case 2: nextClicked(); break;
        case 3: languageChanged(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
