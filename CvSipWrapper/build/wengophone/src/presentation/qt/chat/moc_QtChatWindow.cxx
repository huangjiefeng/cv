/****************************************************************************
** Meta object code from reading C++ file 'QtChatWindow.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,
      82,   59,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     179,  173,   13,   13, 0x0a,
     201,   14,   13,   13, 0x0a,
     237,   13,   13,   13, 0x0a,
     244,   59,   13,   13, 0x0a,
     339,   13,   13,   13, 0x0a,
     362,   13,   13,   13, 0x0a,
     390,   13,   13,   13, 0x0a,
     419,   13,   13,   13, 0x0a,
     442,   13,   13,   13, 0x0a,
     469,   13,   13,   13, 0x0a,
     493,   13,   13,   13, 0x0a,
     510,   13,   13,   13, 0x0a,
     535,  525,   13,   13, 0x0a,
     562,   13,   13,   13, 0x0a,
     589,   13,   13,   13, 0x08,
     595,   13,   13,   13, 0x08,
     602,   13,   13,   13, 0x08,
     610,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtChatWindow[] = {
    "QtChatWindow\0\0sender\0"
    "messageReceivedSignal(IMChatSession*)\0"
    "sender,imContact,state\0"
    "typingStateChangedSignal(const IMChatSession*,const IMContact*,const I"
    "MChat::TypingState*)\0"
    "index\0activeTabChanged(int)\0"
    "messageReceivedSlot(IMChatSession*)\0"
    "show()\0"
    "typingStateChangedThreadSafe(const IMChatSession*,const IMContact*,con"
    "st IMChat::TypingState*)\0"
    "callActiveTabContact()\0"
    "sendSmsToActiveTabContact()\0"
    "sendFileToActiveTabContact()\0"
    "createChatConference()\0"
    "showActiveTabContactInfo()\0"
    "blockActiveTabContact()\0closeActiveTab()\0"
    "closeAllTabs()\0contactId\0"
    "statusChangedSlot(QString)\0"
    "saveActiveTabChatHistory()\0cut()\0"
    "copy()\0paste()\0updateClipboardActions()\0"
};

const QMetaObject QtChatWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtChatWindow,
      qt_meta_data_QtChatWindow, 0 }
};

const QMetaObject *QtChatWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatWindow))
        return static_cast<void*>(const_cast< QtChatWindow*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtChatWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtChatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: messageReceivedSignal((*reinterpret_cast< IMChatSession*(*)>(_a[1]))); break;
        case 1: typingStateChangedSignal((*reinterpret_cast< const IMChatSession*(*)>(_a[1])),(*reinterpret_cast< const IMContact*(*)>(_a[2])),(*reinterpret_cast< const IMChat::TypingState*(*)>(_a[3]))); break;
        case 2: activeTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: messageReceivedSlot((*reinterpret_cast< IMChatSession*(*)>(_a[1]))); break;
        case 4: show(); break;
        case 5: typingStateChangedThreadSafe((*reinterpret_cast< const IMChatSession*(*)>(_a[1])),(*reinterpret_cast< const IMContact*(*)>(_a[2])),(*reinterpret_cast< const IMChat::TypingState*(*)>(_a[3]))); break;
        case 6: callActiveTabContact(); break;
        case 7: sendSmsToActiveTabContact(); break;
        case 8: sendFileToActiveTabContact(); break;
        case 9: createChatConference(); break;
        case 10: showActiveTabContactInfo(); break;
        case 11: blockActiveTabContact(); break;
        case 12: closeActiveTab(); break;
        case 13: closeAllTabs(); break;
        case 14: statusChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: saveActiveTabChatHistory(); break;
        case 16: cut(); break;
        case 17: copy(); break;
        case 18: paste(); break;
        case 19: updateClipboardActions(); break;
        }
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void QtChatWindow::messageReceivedSignal(IMChatSession * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtChatWindow::typingStateChangedSignal(const IMChatSession * _t1, const IMContact * _t2, const IMChat::TypingState * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
