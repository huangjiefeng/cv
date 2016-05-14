/****************************************************************************
** Meta object code from reading C++ file 'ILogin.h'
**
** Created: Thu Jul 29 12:39:43 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/login/ILogin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ILogin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ILogin[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      26,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ILogin[] = {
    "ILogin\0\0languageChanged()\0goBack()\0"
};

const QMetaObject ILogin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ILogin,
      qt_meta_data_ILogin, 0 }
};

const QMetaObject *ILogin::metaObject() const
{
    return &staticMetaObject;
}

void *ILogin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ILogin))
        return static_cast<void*>(const_cast< ILogin*>(this));
    return QWidget::qt_metacast(_clname);
}

int ILogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: languageChanged(); break;
        case 1: goBack(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
