/****************************************************************************
** Meta object code from reading C++ file 'QtChatToaster.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/toaster/QtChatToaster.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatToaster.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatToaster[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   14,   14,   14, 0x0a,
      43,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtChatToaster[] = {
    "QtChatToaster\0\0chatButtonClicked()\0"
    "close()\0chatButtonSlot()\0"
};

const QMetaObject QtChatToaster::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtChatToaster,
      qt_meta_data_QtChatToaster, 0 }
};

const QMetaObject *QtChatToaster::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatToaster::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatToaster))
        return static_cast<void*>(const_cast< QtChatToaster*>(this));
    if (!strcmp(_clname, "IQtToaster"))
        return static_cast< IQtToaster*>(const_cast< QtChatToaster*>(this));
    return QObject::qt_metacast(_clname);
}

int QtChatToaster::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: chatButtonClicked(); break;
        case 1: close(); break;
        case 2: chatButtonSlot(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtChatToaster::chatButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
