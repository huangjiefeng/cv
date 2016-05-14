/****************************************************************************
** Meta object code from reading C++ file 'QtSoftUpdate.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../wengophone/src/presentation/qt/webservices/softupdate/QtSoftUpdate.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtSoftUpdate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSoftUpdate[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      36,   30,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtSoftUpdate[] = {
    "QtSoftUpdate\0\0abortDownload()\0error\0"
    "updateProcessError(QProcess::ProcessError)\0"
};

const QMetaObject QtSoftUpdate::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtSoftUpdate,
      qt_meta_data_QtSoftUpdate, 0 }
};

const QMetaObject *QtSoftUpdate::metaObject() const
{
    return &staticMetaObject;
}

void *QtSoftUpdate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSoftUpdate))
        return static_cast<void*>(const_cast< QtSoftUpdate*>(this));
    if (!strcmp(_clname, "PSoftUpdate"))
        return static_cast< PSoftUpdate*>(const_cast< QtSoftUpdate*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtSoftUpdate*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtSoftUpdate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: abortDownload(); break;
        case 1: updateProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
