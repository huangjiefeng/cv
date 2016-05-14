/****************************************************************************
** Meta object code from reading C++ file 'QtIMProfileWidget.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profilebar/QtIMProfileWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtIMProfileWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtIMProfileWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x0a,
      57,   19,   18,   18, 0x0a,
      86,   19,   18,   18, 0x0a,
     115,   18,   18,   18, 0x0a,
     123,   18,   18,   18, 0x08,
     145,   18,   18,   18, 0x08,
     164,   18,   18,   18, 0x08,
     189,  182,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtIMProfileWidget[] = {
    "QtIMProfileWidget\0\0imAccountId\0"
    "addAccountButton(QString)\0"
    "removeAccountButton(QString)\0"
    "updateAccountButton(QString)\0reset()\0"
    "changeAvatarClicked()\0aliasTextChanged()\0"
    "languageChanged()\0action\0"
    "addIMAccount(QAction*)\0"
};

const QMetaObject QtIMProfileWidget::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtIMProfileWidget,
      qt_meta_data_QtIMProfileWidget, 0 }
};

const QMetaObject *QtIMProfileWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtIMProfileWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtIMProfileWidget))
        return static_cast<void*>(const_cast< QtIMProfileWidget*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtIMProfileWidget*>(this));
    return QObject::qt_metacast(_clname);
}

int QtIMProfileWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addAccountButton((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: removeAccountButton((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: updateAccountButton((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: reset(); break;
        case 4: changeAvatarClicked(); break;
        case 5: aliasTextChanged(); break;
        case 6: languageChanged(); break;
        case 7: addIMAccount((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
