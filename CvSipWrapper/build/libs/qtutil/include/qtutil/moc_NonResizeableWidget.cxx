/****************************************************************************
** Meta object code from reading C++ file 'NonResizeableWidget.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/NonResizeableWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NonResizeableWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NonResizeableWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NonResizeableWidget[] = {
    "NonResizeableWidget\0\0resizeEvent()\0"
};

const QMetaObject NonResizeableWidget::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NonResizeableWidget,
      qt_meta_data_NonResizeableWidget, 0 }
};

const QMetaObject *NonResizeableWidget::metaObject() const
{
    return &staticMetaObject;
}

void *NonResizeableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NonResizeableWidget))
        return static_cast<void*>(const_cast< NonResizeableWidget*>(this));
    return QObject::qt_metacast(_clname);
}

int NonResizeableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resizeEvent(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
