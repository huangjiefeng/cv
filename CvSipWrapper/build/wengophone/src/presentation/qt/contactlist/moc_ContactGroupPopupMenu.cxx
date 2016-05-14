/****************************************************************************
** Meta object code from reading C++ file 'ContactGroupPopupMenu.h'
**
** Created: Thu Jul 29 12:39:43 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/contactlist/ContactGroupPopupMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContactGroupPopupMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ContactGroupPopupMenu[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      31,   23,   22,   22, 0x0a,
      49,   22,   22,   22, 0x08,
      70,   22,   22,   22, 0x08,
      91,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ContactGroupPopupMenu[] = {
    "ContactGroupPopupMenu\0\0groupId\0"
    "showMenu(QString)\0removeContactGroup()\0"
    "renameContactGroup()\0sendSms()\0"
};

const QMetaObject ContactGroupPopupMenu::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_ContactGroupPopupMenu,
      qt_meta_data_ContactGroupPopupMenu, 0 }
};

const QMetaObject *ContactGroupPopupMenu::metaObject() const
{
    return &staticMetaObject;
}

void *ContactGroupPopupMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContactGroupPopupMenu))
        return static_cast<void*>(const_cast< ContactGroupPopupMenu*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< ContactGroupPopupMenu*>(this));
    return QMenu::qt_metacast(_clname);
}

int ContactGroupPopupMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showMenu((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: removeContactGroup(); break;
        case 2: renameContactGroup(); break;
        case 3: sendSms(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
