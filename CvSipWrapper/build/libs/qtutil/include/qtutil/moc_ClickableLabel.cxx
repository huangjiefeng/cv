/****************************************************************************
** Meta object code from reading C++ file 'ClickableLabel.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/ClickableLabel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClickableLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClickableLabel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      26,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ClickableLabel[] = {
    "ClickableLabel\0\0clicked()\0rightClicked()\0"
};

const QMetaObject ClickableLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_ClickableLabel,
      qt_meta_data_ClickableLabel, 0 }
};

const QMetaObject *ClickableLabel::metaObject() const
{
    return &staticMetaObject;
}

void *ClickableLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClickableLabel))
        return static_cast<void*>(const_cast< ClickableLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int ClickableLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: rightClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ClickableLabel::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ClickableLabel::rightClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
