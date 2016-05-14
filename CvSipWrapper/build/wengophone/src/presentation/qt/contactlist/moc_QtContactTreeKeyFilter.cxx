/****************************************************************************
** Meta object code from reading C++ file 'QtContactTreeKeyFilter.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/contactlist/QtContactTreeKeyFilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtContactTreeKeyFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtContactTreeKeyFilter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   24,   23,   23, 0x05,
      56,   24,   23,   23, 0x05,
      84,   24,   23,   23, 0x05,
     115,   24,   23,   23, 0x05,
     142,   24,   23,   23, 0x05,
     169,   24,   23,   23, 0x05,
     196,   24,   23,   23, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtContactTreeKeyFilter[] = {
    "QtContactTreeKeyFilter\0\0item\0"
    "openItem(QTreeWidgetItem*)\0"
    "closeItem(QTreeWidgetItem*)\0"
    "enterPressed(QTreeWidgetItem*)\0"
    "cPressed(QTreeWidgetItem*)\0"
    "wPressed(QTreeWidgetItem*)\0"
    "sPressed(QTreeWidgetItem*)\0"
    "deleteItem(QTreeWidgetItem*)\0"
};

const QMetaObject QtContactTreeKeyFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtContactTreeKeyFilter,
      qt_meta_data_QtContactTreeKeyFilter, 0 }
};

const QMetaObject *QtContactTreeKeyFilter::metaObject() const
{
    return &staticMetaObject;
}

void *QtContactTreeKeyFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtContactTreeKeyFilter))
        return static_cast<void*>(const_cast< QtContactTreeKeyFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int QtContactTreeKeyFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: closeItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 2: enterPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: cPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: wPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 5: sPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: deleteItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QtContactTreeKeyFilter::openItem(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtContactTreeKeyFilter::closeItem(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtContactTreeKeyFilter::enterPressed(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtContactTreeKeyFilter::cPressed(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtContactTreeKeyFilter::wPressed(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtContactTreeKeyFilter::sPressed(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtContactTreeKeyFilter::deleteItem(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
