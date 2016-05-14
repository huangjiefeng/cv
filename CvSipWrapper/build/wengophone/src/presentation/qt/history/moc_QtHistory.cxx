/****************************************************************************
** Meta object code from reading C++ file 'QtHistory.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/history/QtHistory.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtHistory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtHistory[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   11,   10,   10, 0x08,
      30,   11,   10,   10, 0x08,
      46,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtHistory[] = {
    "QtHistory\0\0id\0replayItem(int)\0"
    "removeItem(int)\0resetUnseenMissedCalls()\0"
};

const QMetaObject QtHistory::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_QtHistory,
      qt_meta_data_QtHistory, 0 }
};

const QMetaObject *QtHistory::metaObject() const
{
    return &staticMetaObject;
}

void *QtHistory::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtHistory))
        return static_cast<void*>(const_cast< QtHistory*>(this));
    if (!strcmp(_clname, "PHistory"))
        return static_cast< PHistory*>(const_cast< QtHistory*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int QtHistory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: replayItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: removeItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: resetUnseenMissedCalls(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
