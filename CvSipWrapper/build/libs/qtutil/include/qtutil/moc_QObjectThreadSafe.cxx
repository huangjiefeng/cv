/****************************************************************************
** Meta object code from reading C++ file 'QObjectThreadSafe.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/QObjectThreadSafe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QObjectThreadSafe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QObjectThreadSafe[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      30,   24,   19,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QObjectThreadSafe[] = {
    "QObjectThreadSafe\0\0bool\0event\0"
    "event(QEvent*)\0"
};

const QMetaObject QObjectThreadSafe::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QObjectThreadSafe,
      qt_meta_data_QObjectThreadSafe, 0 }
};

const QMetaObject *QObjectThreadSafe::metaObject() const
{
    return &staticMetaObject;
}

void *QObjectThreadSafe::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QObjectThreadSafe))
        return static_cast<void*>(const_cast< QObjectThreadSafe*>(this));
    return QObject::qt_metacast(_clname);
}

int QObjectThreadSafe::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = event((*reinterpret_cast< QEvent*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
