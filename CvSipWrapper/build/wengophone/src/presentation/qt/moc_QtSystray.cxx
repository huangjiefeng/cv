/****************************************************************************
** Meta object code from reading C++ file 'QtSystray.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtSystray.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtSystray.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSystray[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      31,   10,   10,   10, 0x0a,
      45,   10,   10,   10, 0x0a,
      52,   10,   10,   10, 0x08,
      78,   71,   10,   10, 0x08,
      97,   71,   10,   10, 0x08,
     117,   71,   10,   10, 0x08,
     135,   71,   10,   10, 0x08,
     154,   10,   10,   10, 0x08,
     175,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtSystray[] = {
    "QtSystray\0\0updateSystrayIcon()\0"
    "setTrayMenu()\0hide()\0toggleMainWindow()\0"
    "action\0makeCall(QAction*)\0startChat(QAction*)\0"
    "sendSms(QAction*)\0sendFile(QAction*)\0"
    "userProfileDeleted()\0languageChanged()\0"
};

const QMetaObject QtSystray::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtSystray,
      qt_meta_data_QtSystray, 0 }
};

const QMetaObject *QtSystray::metaObject() const
{
    return &staticMetaObject;
}

void *QtSystray::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSystray))
        return static_cast<void*>(const_cast< QtSystray*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtSystray*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtSystray::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateSystrayIcon(); break;
        case 1: setTrayMenu(); break;
        case 2: hide(); break;
        case 3: toggleMainWindow(); break;
        case 4: makeCall((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 5: startChat((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 6: sendSms((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 7: sendFile((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: userProfileDeleted(); break;
        case 9: languageChanged(); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
