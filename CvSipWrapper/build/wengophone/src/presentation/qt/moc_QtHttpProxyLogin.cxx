/****************************************************************************
** Meta object code from reading C++ file 'QtHttpProxyLogin.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtHttpProxyLogin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtHttpProxyLogin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtHttpProxyLogin[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      22,   17,   18,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtHttpProxyLogin[] = {
    "QtHttpProxyLogin\0\0int\0show()\0"
};

const QMetaObject QtHttpProxyLogin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtHttpProxyLogin,
      qt_meta_data_QtHttpProxyLogin, 0 }
};

const QMetaObject *QtHttpProxyLogin::metaObject() const
{
    return &staticMetaObject;
}

void *QtHttpProxyLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtHttpProxyLogin))
        return static_cast<void*>(const_cast< QtHttpProxyLogin*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtHttpProxyLogin*>(this));
    return QObject::qt_metacast(_clname);
}

int QtHttpProxyLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = show();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
