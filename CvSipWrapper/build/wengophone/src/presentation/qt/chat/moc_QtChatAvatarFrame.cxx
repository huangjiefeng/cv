/****************************************************************************
** Meta object code from reading C++ file 'QtChatAvatarFrame.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatAvatarFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatAvatarFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatAvatarFrame[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QtChatAvatarFrame[] = {
    "QtChatAvatarFrame\0"
};

const QMetaObject QtChatAvatarFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QtChatAvatarFrame,
      qt_meta_data_QtChatAvatarFrame, 0 }
};

const QMetaObject *QtChatAvatarFrame::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatAvatarFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatAvatarFrame))
        return static_cast<void*>(const_cast< QtChatAvatarFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int QtChatAvatarFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
