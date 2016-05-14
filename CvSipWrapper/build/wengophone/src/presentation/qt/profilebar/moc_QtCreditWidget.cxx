/****************************************************************************
** Meta object code from reading C++ file 'QtCreditWidget.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profilebar/QtCreditWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtCreditWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCreditWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      36,   15,   15,   15, 0x08,
      61,   15,   15,   15, 0x08,
      80,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtCreditWidget[] = {
    "QtCreditWidget\0\0buyCreditsClicked()\0"
    "callForwardModeClicked()\0voiceMailClicked()\0"
    "languageChanged()\0"
};

const QMetaObject QtCreditWidget::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtCreditWidget,
      qt_meta_data_QtCreditWidget, 0 }
};

const QMetaObject *QtCreditWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtCreditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCreditWidget))
        return static_cast<void*>(const_cast< QtCreditWidget*>(this));
    return QObject::qt_metacast(_clname);
}

int QtCreditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buyCreditsClicked(); break;
        case 1: callForwardModeClicked(); break;
        case 2: voiceMailClicked(); break;
        case 3: languageChanged(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
