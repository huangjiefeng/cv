/****************************************************************************
** Meta object code from reading C++ file 'QtEmoticonButton.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../wengophone/src/presentation/qt/chat/emoticons/QtEmoticonButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtEmoticonButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtEmoticonButton[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtEmoticonButton[] = {
    "QtEmoticonButton\0\0emoticon\0"
    "buttonClicked(QtEmoticon)\0buttonClickedSlot()\0"
};

const QMetaObject QtEmoticonButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_QtEmoticonButton,
      qt_meta_data_QtEmoticonButton, 0 }
};

const QMetaObject *QtEmoticonButton::metaObject() const
{
    return &staticMetaObject;
}

void *QtEmoticonButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtEmoticonButton))
        return static_cast<void*>(const_cast< QtEmoticonButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int QtEmoticonButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buttonClicked((*reinterpret_cast< QtEmoticon(*)>(_a[1]))); break;
        case 1: buttonClickedSlot(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QtEmoticonButton::buttonClicked(QtEmoticon _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
