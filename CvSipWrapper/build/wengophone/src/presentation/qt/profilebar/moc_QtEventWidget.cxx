/****************************************************************************
** Meta object code from reading C++ file 'QtEventWidget.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profilebar/QtEventWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtEventWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtEventWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      34,   14,   14,   14, 0x08,
      54,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtEventWidget[] = {
    "QtEventWidget\0\0voiceMailClicked()\0"
    "missedCallClicked()\0languageChanged()\0"
};

const QMetaObject QtEventWidget::staticMetaObject = {
    { &QObjectThreadSafe::staticMetaObject, qt_meta_stringdata_QtEventWidget,
      qt_meta_data_QtEventWidget, 0 }
};

const QMetaObject *QtEventWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtEventWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtEventWidget))
        return static_cast<void*>(const_cast< QtEventWidget*>(this));
    return QObjectThreadSafe::qt_metacast(_clname);
}

int QtEventWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObjectThreadSafe::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: voiceMailClicked(); break;
        case 1: missedCallClicked(); break;
        case 2: languageChanged(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
