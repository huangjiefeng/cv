/****************************************************************************
** Meta object code from reading C++ file 'EventFilter.h'
**
** Created: Mon Apr 20 22:16:02 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/EventFilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EventFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventFilter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_EventFilter[] = {
    "EventFilter\0\0event\0activate(QEvent*)\0"
};

const QMetaObject EventFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EventFilter,
      qt_meta_data_EventFilter, 0 }
};

const QMetaObject *EventFilter::metaObject() const
{
    return &staticMetaObject;
}

void *EventFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventFilter))
        return static_cast<void*>(const_cast< EventFilter*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< EventFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int EventFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activate((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void EventFilter::activate(QEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
