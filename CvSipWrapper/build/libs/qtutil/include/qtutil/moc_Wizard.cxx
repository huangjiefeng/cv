/****************************************************************************
** Meta object code from reading C++ file 'Wizard.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/Wizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Wizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Wizard[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      19,    7,    7,    7, 0x08,
      39,    7,    7,    7, 0x08,
      59,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Wizard[] = {
    "Wizard\0\0finished()\0backButtonClicked()\0"
    "nextButtonClicked()\0finishedButtonClicked()\0"
};

const QMetaObject Wizard::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Wizard,
      qt_meta_data_Wizard, 0 }
};

const QMetaObject *Wizard::metaObject() const
{
    return &staticMetaObject;
}

void *Wizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Wizard))
        return static_cast<void*>(const_cast< Wizard*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< Wizard*>(this));
    return QObject::qt_metacast(_clname);
}

int Wizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        case 1: backButtonClicked(); break;
        case 2: nextButtonClicked(); break;
        case 3: finishedButtonClicked(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Wizard::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
