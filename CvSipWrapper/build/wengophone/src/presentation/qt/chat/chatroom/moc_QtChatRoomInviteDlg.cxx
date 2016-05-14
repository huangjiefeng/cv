/****************************************************************************
** Meta object code from reading C++ file 'QtChatRoomInviteDlg.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../wengophone/src/presentation/qt/chat/chatroom/QtChatRoomInviteDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatRoomInviteDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatRoomInviteDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x09,
      39,   20,   20,   20, 0x09,
      57,   20,   20,   20, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QtChatRoomInviteDlg[] = {
    "QtChatRoomInviteDlg\0\0startConference()\0"
    "addToConference()\0removeFromConference()\0"
};

const QMetaObject QtChatRoomInviteDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtChatRoomInviteDlg,
      qt_meta_data_QtChatRoomInviteDlg, 0 }
};

const QMetaObject *QtChatRoomInviteDlg::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatRoomInviteDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatRoomInviteDlg))
        return static_cast<void*>(const_cast< QtChatRoomInviteDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QtChatRoomInviteDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startConference(); break;
        case 1: addToConference(); break;
        case 2: removeFromConference(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
