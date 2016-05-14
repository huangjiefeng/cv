/****************************************************************************
** Meta object code from reading C++ file 'QtIMContactManagerInterface.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/imcontact/QtIMContactManagerInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtIMContactManagerInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtIMContactManagerInterface[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtIMContactManagerInterface[] = {
    "QtIMContactManagerInterface\0\0"
    "contentHasChanged()\0"
};

const QMetaObject QtIMContactManagerInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtIMContactManagerInterface,
      qt_meta_data_QtIMContactManagerInterface, 0 }
};

const QMetaObject *QtIMContactManagerInterface::metaObject() const
{
    return &staticMetaObject;
}

void *QtIMContactManagerInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtIMContactManagerInterface))
        return static_cast<void*>(const_cast< QtIMContactManagerInterface*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtIMContactManagerInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int QtIMContactManagerInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: contentHasChanged(); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QtIMContactManagerInterface::contentHasChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
