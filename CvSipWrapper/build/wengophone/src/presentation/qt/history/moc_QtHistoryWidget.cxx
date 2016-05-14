/****************************************************************************
** Meta object code from reading C++ file 'QtHistoryWidget.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/history/QtHistoryWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtHistoryWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtHistoryWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      20,   17,   16,   16, 0x05,
      45,   17,   16,   16, 0x05,
      70,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   16,   16,   16, 0x0a,
     104,   16,   16,   16, 0x08,
     140,   16,   16,   16, 0x08,
     171,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtHistoryWidget[] = {
    "QtHistoryWidget\0\0id\0replayItemRequested(int)\0"
    "removeItemRequested(int)\0missedCallsSeen()\0"
    "retranslateUi()\0updateCallBarFromIndex(QModelIndex)\0"
    "showPopupMenuForCurrentIndex()\0"
    "emitReplayItemRequested(QModelIndex)\0"
};

const QMetaObject QtHistoryWidget::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtHistoryWidget,
      qt_meta_data_QtHistoryWidget, 0 }
};

const QMetaObject *QtHistoryWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtHistoryWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtHistoryWidget))
        return static_cast<void*>(const_cast< QtHistoryWidget*>(this));
    return QObject::qt_metacast(_clname);
}

int QtHistoryWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: replayItemRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: removeItemRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: missedCallsSeen(); break;
        case 3: retranslateUi(); break;
        case 4: updateCallBarFromIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: showPopupMenuForCurrentIndex(); break;
        case 6: emitReplayItemRequested((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QtHistoryWidget::replayItemRequested(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtHistoryWidget::removeItemRequested(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtHistoryWidget::missedCallsSeen()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
