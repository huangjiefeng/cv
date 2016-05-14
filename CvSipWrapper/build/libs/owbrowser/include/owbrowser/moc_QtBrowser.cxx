/****************************************************************************
** Meta object code from reading C++ file 'QtBrowser.h'
**
** Created: Mon Apr 20 21:59:55 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/owbrowser/include/owbrowser/QtBrowser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtBrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtBrowser[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x08,
      81,   76,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtBrowser[] = {
    "QtBrowser\0\0,,,,,\0"
    "beforeNavigate(QString,int,QString,QVariant,QString,bool&)\0"
    "link\0beforeNavigate(QUrl)\0"
};

const QMetaObject QtBrowser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtBrowser,
      qt_meta_data_QtBrowser, 0 }
};

const QMetaObject *QtBrowser::metaObject() const
{
    return &staticMetaObject;
}

void *QtBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtBrowser))
        return static_cast<void*>(const_cast< QtBrowser*>(this));
    if (!strcmp(_clname, "OWBrowser"))
        return static_cast< OWBrowser*>(const_cast< QtBrowser*>(this));
    return QObject::qt_metacast(_clname);
}

int QtBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: beforeNavigate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 1: beforeNavigate((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
