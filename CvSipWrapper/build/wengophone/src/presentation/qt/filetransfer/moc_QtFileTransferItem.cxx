/****************************************************************************
** Meta object code from reading C++ file 'QtFileTransferItem.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/filetransfer/QtFileTransferItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtFileTransferItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtFileTransferItem[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x05,
      61,   52,   19,   19, 0x05,
      92,   86,   19,   19, 0x05,
     114,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,   52,   19,   19, 0x08,
     147,   20,   19,   19, 0x08,
     165,   86,   19,   19, 0x08,
     182,   19,   19,   19, 0x09,
     191,   19,   19,   19, 0x09,
     198,   19,   19,   19, 0x09,
     206,   19,   19,   19, 0x09,
     215,   19,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QtFileTransferItem[] = {
    "QtFileTransferItem\0\0state\0"
    "stateChangeEvent(QString)\0progress\0"
    "progressChangeEvent(int)\0event\0"
    "updateStateEvent(int)\0removeClicked()\0"
    "setProgress(int)\0setState(QString)\0"
    "updateState(int)\0remove()\0open()\0"
    "pause()\0resume()\0stop()\0"
};

const QMetaObject QtFileTransferItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtFileTransferItem,
      qt_meta_data_QtFileTransferItem, 0 }
};

const QMetaObject *QtFileTransferItem::metaObject() const
{
    return &staticMetaObject;
}

void *QtFileTransferItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtFileTransferItem))
        return static_cast<void*>(const_cast< QtFileTransferItem*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtFileTransferItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtFileTransferItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChangeEvent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: progressChangeEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: updateStateEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: removeClicked(); break;
        case 4: setProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setState((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: updateState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: remove(); break;
        case 8: open(); break;
        case 9: pause(); break;
        case 10: resume(); break;
        case 11: stop(); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QtFileTransferItem::stateChangeEvent(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtFileTransferItem::progressChangeEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtFileTransferItem::updateStateEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtFileTransferItem::removeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
