/****************************************************************************
** Meta object code from reading C++ file 'QtAdvancedIMContactManager.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/imcontact/QtAdvancedIMContactManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAdvancedIMContactManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtAdvancedIMContactManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,
      43,   27,   27,   27, 0x08,
      78,   61,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtAdvancedIMContactManager[] = {
    "QtAdvancedIMContactManager\0\0addIMContact()\0"
    "deleteIMContact()\0current,previous\0"
    "currentItemChangedSlot(QTreeWidgetItem*,QTreeWidgetItem*)\0"
};

const QMetaObject QtAdvancedIMContactManager::staticMetaObject = {
    { &QtIMContactManagerInterface::staticMetaObject, qt_meta_stringdata_QtAdvancedIMContactManager,
      qt_meta_data_QtAdvancedIMContactManager, 0 }
};

const QMetaObject *QtAdvancedIMContactManager::metaObject() const
{
    return &staticMetaObject;
}

void *QtAdvancedIMContactManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtAdvancedIMContactManager))
        return static_cast<void*>(const_cast< QtAdvancedIMContactManager*>(this));
    return QtIMContactManagerInterface::qt_metacast(_clname);
}

int QtAdvancedIMContactManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtIMContactManagerInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addIMContact(); break;
        case 1: deleteIMContact(); break;
        case 2: currentItemChangedSlot((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
