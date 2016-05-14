/****************************************************************************
** Meta object code from reading C++ file 'QtFileTransferDownloadItem.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/filetransfer/QtFileTransferDownloadItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtFileTransferDownloadItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtFileTransferDownloadItem[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,
      36,   27,   27,   27, 0x08,
      45,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtFileTransferDownloadItem[] = {
    "QtFileTransferDownloadItem\0\0pause()\0"
    "resume()\0stop()\0"
};

const QMetaObject QtFileTransferDownloadItem::staticMetaObject = {
    { &QtFileTransferItem::staticMetaObject, qt_meta_stringdata_QtFileTransferDownloadItem,
      qt_meta_data_QtFileTransferDownloadItem, 0 }
};

const QMetaObject *QtFileTransferDownloadItem::metaObject() const
{
    return &staticMetaObject;
}

void *QtFileTransferDownloadItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtFileTransferDownloadItem))
        return static_cast<void*>(const_cast< QtFileTransferDownloadItem*>(this));
    return QtFileTransferItem::qt_metacast(_clname);
}

int QtFileTransferDownloadItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtFileTransferItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pause(); break;
        case 1: resume(); break;
        case 2: stop(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
