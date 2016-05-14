/****************************************************************************
** Meta object code from reading C++ file 'QtFileTransfer.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/filetransfer/QtFileTransfer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtFileTransfer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtFileTransfer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      28,   16,   15,   15, 0x05,
      96,   15,   15,   15, 0x05,
     138,  128,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     182,   16,   15,   15, 0x08,
     248,   15,   15,   15, 0x08,
     278,  128,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtFileTransfer[] = {
    "QtFileTransfer\0\0fileSession\0"
    "newReceiveFileSessionCreatedEventHandlerSignal(ReceiveFileSession*)\0"
    "needUpgradeEventHandlerSignal()\0"
    "contactID\0peerNeedsUpgradeEventHandlerSignal(QString)\0"
    "newReceiveFileSessionCreatedEventHandlerSlot(ReceiveFileSession*)\0"
    "needUpgradeEventHandlerSlot()\0"
    "peerNeedsUpgradeEventHandlerSlot(QString)\0"
};

const QMetaObject QtFileTransfer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtFileTransfer,
      qt_meta_data_QtFileTransfer, 0 }
};

const QMetaObject *QtFileTransfer::metaObject() const
{
    return &staticMetaObject;
}

void *QtFileTransfer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtFileTransfer))
        return static_cast<void*>(const_cast< QtFileTransfer*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtFileTransfer*>(this));
    return QObject::qt_metacast(_clname);
}

int QtFileTransfer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newReceiveFileSessionCreatedEventHandlerSignal((*reinterpret_cast< ReceiveFileSession*(*)>(_a[1]))); break;
        case 1: needUpgradeEventHandlerSignal(); break;
        case 2: peerNeedsUpgradeEventHandlerSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: newReceiveFileSessionCreatedEventHandlerSlot((*reinterpret_cast< ReceiveFileSession*(*)>(_a[1]))); break;
        case 4: needUpgradeEventHandlerSlot(); break;
        case 5: peerNeedsUpgradeEventHandlerSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QtFileTransfer::newReceiveFileSessionCreatedEventHandlerSignal(ReceiveFileSession * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtFileTransfer::needUpgradeEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QtFileTransfer::peerNeedsUpgradeEventHandlerSignal(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
