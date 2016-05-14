/****************************************************************************
** Meta object code from reading C++ file 'QtLoginDialog.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/login/QtLoginDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtLoginDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtLoginDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      42,   31,   14,   14, 0x0a,
      72,   31,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtLoginDialog[] = {
    "QtLoginDialog\0\0loginRejected()\0"
    "sipAccount\0setInvalidAccount(SipAccount)\0"
    "setValidAccount(SipAccount)\0"
};

const QMetaObject QtLoginDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtLoginDialog,
      qt_meta_data_QtLoginDialog, 0 }
};

const QMetaObject *QtLoginDialog::metaObject() const
{
    return &staticMetaObject;
}

void *QtLoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtLoginDialog))
        return static_cast<void*>(const_cast< QtLoginDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QtLoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loginRejected(); break;
        case 1: setInvalidAccount((*reinterpret_cast< const SipAccount(*)>(_a[1]))); break;
        case 2: setValidAccount((*reinterpret_cast< SipAccount(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
