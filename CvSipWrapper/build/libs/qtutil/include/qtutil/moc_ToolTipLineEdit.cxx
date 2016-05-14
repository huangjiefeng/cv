/****************************************************************************
** Meta object code from reading C++ file 'ToolTipLineEdit.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/ToolTipLineEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolTipLineEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToolTipLineEdit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x0a,
      39,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ToolTipLineEdit[] = {
    "ToolTipLineEdit\0\0text\0setText(QString)\0"
    "languageChanged()\0"
};

const QMetaObject ToolTipLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_ToolTipLineEdit,
      qt_meta_data_ToolTipLineEdit, 0 }
};

const QMetaObject *ToolTipLineEdit::metaObject() const
{
    return &staticMetaObject;
}

void *ToolTipLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolTipLineEdit))
        return static_cast<void*>(const_cast< ToolTipLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int ToolTipLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: languageChanged(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
