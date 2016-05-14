/****************************************************************************
** Meta object code from reading C++ file 'QtIMAccountPresenceMenuManager.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profilebar/QtIMAccountPresenceMenuManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtIMAccountPresenceMenuManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtIMAccountPresenceMenuManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      32,   31,   31,   31, 0x08,
      62,   31,   31,   31, 0x08,
      75,   31,   31,   31, 0x08,
      91,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtIMAccountPresenceMenuManager[] = {
    "QtIMAccountPresenceMenuManager\0\0"
    "slotPresenceActionTriggered()\0"
    "disconnect()\0deleteAccount()\0"
    "updateActions()\0"
};

const QMetaObject QtIMAccountPresenceMenuManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtIMAccountPresenceMenuManager,
      qt_meta_data_QtIMAccountPresenceMenuManager, 0 }
};

const QMetaObject *QtIMAccountPresenceMenuManager::metaObject() const
{
    return &staticMetaObject;
}

void *QtIMAccountPresenceMenuManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtIMAccountPresenceMenuManager))
        return static_cast<void*>(const_cast< QtIMAccountPresenceMenuManager*>(this));
    return QObject::qt_metacast(_clname);
}

int QtIMAccountPresenceMenuManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotPresenceActionTriggered(); break;
        case 1: disconnect(); break;
        case 2: deleteAccount(); break;
        case 3: updateActions(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
