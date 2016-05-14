/****************************************************************************
** Meta object code from reading C++ file 'QtCallBar.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/callbar/QtCallBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtCallBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCallBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      31,   10,   10,   10, 0x05,
      53,   10,   10,   10, 0x05,
      87,   82,   10,   10, 0x05,
     125,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     148,   10,   10,   10, 0x08,
     172,   10,   10,   10, 0x08,
     198,   10,   10,   10, 0x08,
     231,   82,   10,   10, 0x08,
     273,   10,   10,   10, 0x08,
     300,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtCallBar[] = {
    "QtCallBar\0\0callButtonClicked()\0"
    "hangUpButtonClicked()\0"
    "phoneComboBoxReturnPressed()\0text\0"
    "phoneComboBoxEditTextChanged(QString)\0"
    "phoneComboBoxClicked()\0callButtonClickedSlot()\0"
    "hangUpButtonClickedSlot()\0"
    "phoneComboBoxReturnPressedSlot()\0"
    "phoneComboBoxEditTextChangedSlot(QString)\0"
    "phoneComboBoxClickedSlot()\0languageChanged()\0"
};

const QMetaObject QtCallBar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QtCallBar,
      qt_meta_data_QtCallBar, 0 }
};

const QMetaObject *QtCallBar::metaObject() const
{
    return &staticMetaObject;
}

void *QtCallBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCallBar))
        return static_cast<void*>(const_cast< QtCallBar*>(this));
    return QFrame::qt_metacast(_clname);
}

int QtCallBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: callButtonClicked(); break;
        case 1: hangUpButtonClicked(); break;
        case 2: phoneComboBoxReturnPressed(); break;
        case 3: phoneComboBoxEditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: phoneComboBoxClicked(); break;
        case 5: callButtonClickedSlot(); break;
        case 6: hangUpButtonClickedSlot(); break;
        case 7: phoneComboBoxReturnPressedSlot(); break;
        case 8: phoneComboBoxEditTextChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: phoneComboBoxClickedSlot(); break;
        case 10: languageChanged(); break;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QtCallBar::callButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtCallBar::hangUpButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QtCallBar::phoneComboBoxReturnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QtCallBar::phoneComboBoxEditTextChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtCallBar::phoneComboBoxClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
