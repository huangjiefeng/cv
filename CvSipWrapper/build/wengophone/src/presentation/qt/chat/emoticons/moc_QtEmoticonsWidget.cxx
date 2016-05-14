/****************************************************************************
** Meta object code from reading C++ file 'QtEmoticonsWidget.h'
**
** Created: Thu Jul 29 12:39:40 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../wengophone/src/presentation/qt/chat/emoticons/QtEmoticonsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtEmoticonsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EmoticonsWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      26,   17,   16,   16, 0x05,
      54,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   16,   16,   16, 0x0a,
      77,   17,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EmoticonsWidget[] = {
    "EmoticonsWidget\0\0emoticon\0"
    "emoticonClicked(QtEmoticon)\0closed()\0"
    "changeState()\0buttonClicked(QtEmoticon)\0"
};

const QMetaObject EmoticonsWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_EmoticonsWidget,
      qt_meta_data_EmoticonsWidget, 0 }
};

const QMetaObject *EmoticonsWidget::metaObject() const
{
    return &staticMetaObject;
}

void *EmoticonsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EmoticonsWidget))
        return static_cast<void*>(const_cast< EmoticonsWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int EmoticonsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emoticonClicked((*reinterpret_cast< const QtEmoticon(*)>(_a[1]))); break;
        case 1: closed(); break;
        case 2: changeState(); break;
        case 3: buttonClicked((*reinterpret_cast< const QtEmoticon(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EmoticonsWidget::emoticonClicked(const QtEmoticon & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EmoticonsWidget::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
