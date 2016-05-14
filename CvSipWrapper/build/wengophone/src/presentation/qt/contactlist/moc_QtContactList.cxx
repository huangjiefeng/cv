/****************************************************************************
** Meta object code from reading C++ file 'QtContactList.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/contactlist/QtContactList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtContactList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtContactList[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      25,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      70,   60,   14,   14, 0x08,
     123,  101,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtContactList[] = {
    "QtContactList\0\0contactId\0"
    "contactChangedEventSignal(QString)\0"
    "groupName\0groupRightClickedSlot(QString)\0"
    "dstContact,srcContact\0"
    "mergeContactsSlot(QString,QString)\0"
};

const QMetaObject QtContactList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtContactList,
      qt_meta_data_QtContactList, 0 }
};

const QMetaObject *QtContactList::metaObject() const
{
    return &staticMetaObject;
}

void *QtContactList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtContactList))
        return static_cast<void*>(const_cast< QtContactList*>(this));
    if (!strcmp(_clname, "PContactList"))
        return static_cast< PContactList*>(const_cast< QtContactList*>(this));
    return QObject::qt_metacast(_clname);
}

int QtContactList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: contactChangedEventSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: groupRightClickedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: mergeContactsSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtContactList::contactChangedEventSignal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
