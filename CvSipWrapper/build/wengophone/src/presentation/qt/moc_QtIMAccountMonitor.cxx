/****************************************************************************
** Meta object code from reading C++ file 'QtIMAccountMonitor.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtIMAccountMonitor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtIMAccountMonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtIMAccountMonitor[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      32,   20,   19,   19, 0x05,
      56,   20,   19,   19, 0x05,
      82,   20,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtIMAccountMonitor[] = {
    "QtIMAccountMonitor\0\0imAccountId\0"
    "imAccountAdded(QString)\0"
    "imAccountRemoved(QString)\0"
    "imAccountUpdated(QString)\0"
};

const QMetaObject QtIMAccountMonitor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtIMAccountMonitor,
      qt_meta_data_QtIMAccountMonitor, 0 }
};

const QMetaObject *QtIMAccountMonitor::metaObject() const
{
    return &staticMetaObject;
}

void *QtIMAccountMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtIMAccountMonitor))
        return static_cast<void*>(const_cast< QtIMAccountMonitor*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtIMAccountMonitor*>(this));
    return QObject::qt_metacast(_clname);
}

int QtIMAccountMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: imAccountAdded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: imAccountRemoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: imAccountUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtIMAccountMonitor::imAccountAdded(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtIMAccountMonitor::imAccountRemoved(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtIMAccountMonitor::imAccountUpdated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
