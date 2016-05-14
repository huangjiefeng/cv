/****************************************************************************
** Meta object code from reading C++ file 'QtThumbnailManager.h'
**
** Created: Mon Apr 20 22:16:00 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../libs/qtutil/src/QtThumbnailManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtThumbnailManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtThumbnailManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtThumbnailManager[] = {
    "QtThumbnailManager\0\0fileInfo\0"
    "thumbnailUpdated(QFileInfo)\0"
};

const QMetaObject QtThumbnailManager::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QtThumbnailManager,
      qt_meta_data_QtThumbnailManager, 0 }
};

const QMetaObject *QtThumbnailManager::metaObject() const
{
    return &staticMetaObject;
}

void *QtThumbnailManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtThumbnailManager))
        return static_cast<void*>(const_cast< QtThumbnailManager*>(this));
    return QThread::qt_metacast(_clname);
}

int QtThumbnailManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: thumbnailUpdated((*reinterpret_cast< const QFileInfo(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QtThumbnailManager::thumbnailUpdated(const QFileInfo & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
