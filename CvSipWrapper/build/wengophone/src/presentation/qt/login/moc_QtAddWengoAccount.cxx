/****************************************************************************
** Meta object code from reading C++ file 'QtAddWengoAccount.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/login/QtAddWengoAccount.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAddWengoAccount.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtAddWengoAccount[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      48,   18,   18,   18, 0x08,
      68,   18,   18,   18, 0x08,
      98,   18,   18,   18, 0x08,
     113,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtAddWengoAccount[] = {
    "QtAddWengoAccount\0\0createAccountButtonClicked()\0"
    "helpButtonClicked()\0forgotPasswordButtonClicked()\0"
    "loginClicked()\0languageChanged()\0"
};

const QMetaObject QtAddWengoAccount::staticMetaObject = {
    { &ILogin::staticMetaObject, qt_meta_stringdata_QtAddWengoAccount,
      qt_meta_data_QtAddWengoAccount, 0 }
};

const QMetaObject *QtAddWengoAccount::metaObject() const
{
    return &staticMetaObject;
}

void *QtAddWengoAccount::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtAddWengoAccount))
        return static_cast<void*>(const_cast< QtAddWengoAccount*>(this));
    return ILogin::qt_metacast(_clname);
}

int QtAddWengoAccount::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ILogin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createAccountButtonClicked(); break;
        case 1: helpButtonClicked(); break;
        case 2: forgotPasswordButtonClicked(); break;
        case 3: loginClicked(); break;
        case 4: languageChanged(); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
