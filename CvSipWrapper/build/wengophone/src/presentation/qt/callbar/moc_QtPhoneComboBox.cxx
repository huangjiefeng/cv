/****************************************************************************
** Meta object code from reading C++ file 'QtPhoneComboBox.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/callbar/QtPhoneComboBox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtPhoneComboBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtPhoneComboBox[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x0a,
      43,   16,   16,   16, 0x0a,
      61,   16,   16,   16, 0x08,
      81,   16,   16,   16, 0x08,
     102,   16,   16,   16, 0x08,
     120,   17,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtPhoneComboBox[] = {
    "QtPhoneComboBox\0\0text\0setEditText(QString)\0"
    "languageChanged()\0comboBoxActivated()\0"
    "popupDisplayedSlot()\0popupHiddenSlot()\0"
    "itemhighlighted(QString)\0"
};

const QMetaObject QtPhoneComboBox::staticMetaObject = {
    { &UpQComboBox::staticMetaObject, qt_meta_stringdata_QtPhoneComboBox,
      qt_meta_data_QtPhoneComboBox, 0 }
};

const QMetaObject *QtPhoneComboBox::metaObject() const
{
    return &staticMetaObject;
}

void *QtPhoneComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtPhoneComboBox))
        return static_cast<void*>(const_cast< QtPhoneComboBox*>(this));
    return UpQComboBox::qt_metacast(_clname);
}

int QtPhoneComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = UpQComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setEditText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: languageChanged(); break;
        case 2: comboBoxActivated(); break;
        case 3: popupDisplayedSlot(); break;
        case 4: popupHiddenSlot(); break;
        case 5: itemhighlighted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 6;
    }
    return _id;
}
static const uint qt_meta_data_QtPhoneComboBoxLineEdit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      33,   25,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtPhoneComboBoxLineEdit[] = {
    "QtPhoneComboBoxLineEdit\0\0newText\0"
    "textEditedSlot(QString)\0"
};

const QMetaObject QtPhoneComboBoxLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_QtPhoneComboBoxLineEdit,
      qt_meta_data_QtPhoneComboBoxLineEdit, 0 }
};

const QMetaObject *QtPhoneComboBoxLineEdit::metaObject() const
{
    return &staticMetaObject;
}

void *QtPhoneComboBoxLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtPhoneComboBoxLineEdit))
        return static_cast<void*>(const_cast< QtPhoneComboBoxLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int QtPhoneComboBoxLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: textEditedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
