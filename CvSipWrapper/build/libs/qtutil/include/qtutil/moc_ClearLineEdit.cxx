/****************************************************************************
** Meta object code from reading C++ file 'ClearLineEdit.h'
**
** Created: Mon Apr 20 22:16:00 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/ClearLineEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClearLineEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClearLineEdit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ClearLineEdit[] = {
    "ClearLineEdit\0\0updateRightButton()\0"
};

const QMetaObject ClearLineEdit::staticMetaObject = {
    { &IconLineEdit::staticMetaObject, qt_meta_stringdata_ClearLineEdit,
      qt_meta_data_ClearLineEdit, 0 }
};

const QMetaObject *ClearLineEdit::metaObject() const
{
    return &staticMetaObject;
}

void *ClearLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClearLineEdit))
        return static_cast<void*>(const_cast< ClearLineEdit*>(this));
    return IconLineEdit::qt_metacast(_clname);
}

int ClearLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IconLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateRightButton(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
