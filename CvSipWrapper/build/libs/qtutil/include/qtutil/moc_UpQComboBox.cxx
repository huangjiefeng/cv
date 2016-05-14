/****************************************************************************
** Meta object code from reading C++ file 'UpQComboBox.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/UpQComboBox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UpQComboBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpQComboBox[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      30,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_UpQComboBox[] = {
    "UpQComboBox\0\0popupDisplayed()\0"
    "popupHidden()\0"
};

const QMetaObject UpQComboBox::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_UpQComboBox,
      qt_meta_data_UpQComboBox, 0 }
};

const QMetaObject *UpQComboBox::metaObject() const
{
    return &staticMetaObject;
}

void *UpQComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpQComboBox))
        return static_cast<void*>(const_cast< UpQComboBox*>(this));
    return QComboBox::qt_metacast(_clname);
}

int UpQComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: popupDisplayed(); break;
        case 1: popupHidden(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void UpQComboBox::popupDisplayed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void UpQComboBox::popupHidden()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
