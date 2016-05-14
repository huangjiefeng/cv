/****************************************************************************
** Meta object code from reading C++ file 'trayicon.h'
**
** Created: Mon Apr 20 22:00:04 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../libs/trayicon/trayicon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trayicon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrayIcon[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       2,   50, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x05,
      32,    9,    9,    9, 0x05,
      54,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   63,    9,    9, 0x0a,
      89,   85,    9,    9, 0x0a,
     109,    9,    9,    9, 0x0a,
     116,    9,    9,    9, 0x0a,
     123,    9,    9,    9, 0x0a,

 // properties: name, type, flags
     146,  138, 0x0a095103,
      63,  154, 0x41095103,

       0        // eod
};

static const char qt_meta_stringdata_TrayIcon[] = {
    "TrayIcon\0\0,\0clicked(QPoint,int)\0"
    "doubleClicked(QPoint)\0closed()\0icon\0"
    "setIcon(QPixmap)\0tip\0setToolTip(QString)\0"
    "show()\0hide()\0newTrayOwner()\0QString\0"
    "toolTip\0QPixmap\0"
};

const QMetaObject TrayIcon::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrayIcon,
      qt_meta_data_TrayIcon, 0 }
};

const QMetaObject *TrayIcon::metaObject() const
{
    return &staticMetaObject;
}

void *TrayIcon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrayIcon))
        return static_cast<void*>(const_cast< TrayIcon*>(this));
    return QObject::qt_metacast(_clname);
}

int TrayIcon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: doubleClicked((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: closed(); break;
        case 3: setIcon((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 4: setToolTip((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: show(); break;
        case 6: hide(); break;
        case 7: newTrayOwner(); break;
        }
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = toolTip(); break;
        case 1: *reinterpret_cast< QPixmap*>(_v) = icon(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setToolTip(*reinterpret_cast< QString*>(_v)); break;
        case 1: setIcon(*reinterpret_cast< QPixmap*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TrayIcon::clicked(const QPoint & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrayIcon::doubleClicked(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TrayIcon::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
