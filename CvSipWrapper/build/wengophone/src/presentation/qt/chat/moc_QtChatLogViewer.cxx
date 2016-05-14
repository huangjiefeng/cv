/****************************************************************************
** Meta object code from reading C++ file 'QtChatLogViewer.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatLogViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatLogViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatLogViewer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x0a,
      60,   16,   16,   16, 0x0a,
      74,   16,   16,   16, 0x0a,
      93,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     136,   16,   16,   16, 0x0a,
     154,   16,   16,   16, 0x0a,
     168,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtChatLogViewer[] = {
    "QtChatLogViewer\0\0contactId\0"
    "contactChangedEventSlot(QString)\0"
    "callContact()\0sendSmsToContact()\0"
    "sendFileToContact()\0createChatConference()\0"
    "showContactInfo()\0restartChat()\0"
    "saveChatHistory()\0"
};

const QMetaObject QtChatLogViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtChatLogViewer,
      qt_meta_data_QtChatLogViewer, 0 }
};

const QMetaObject *QtChatLogViewer::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatLogViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatLogViewer))
        return static_cast<void*>(const_cast< QtChatLogViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtChatLogViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: contactChangedEventSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: callContact(); break;
        case 2: sendSmsToContact(); break;
        case 3: sendFileToContact(); break;
        case 4: createChatConference(); break;
        case 5: showContactInfo(); break;
        case 6: restartChat(); break;
        case 7: saveChatHistory(); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
