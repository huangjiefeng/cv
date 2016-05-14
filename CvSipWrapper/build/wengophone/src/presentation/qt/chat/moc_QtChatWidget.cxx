/****************************************************************************
** Meta object code from reading C++ file 'QtChatWidget.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      26,   14,   13,   13, 0x05,
      71,   61,   13,   13, 0x05,
     106,   61,   13,   13, 0x05,
     143,   13,   13,   13, 0x05,
     160,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     195,   13,   13,   13, 0x0a,
     217,   13,   13,   13, 0x0a,
     230,   13,   13,   13, 0x0a,
     256,  247,   13,   13, 0x0a,
     285,   13,   13,   13, 0x0a,
     304,   61,   13,   13, 0x0a,
     337,   61,   13,   13, 0x0a,
     380,  372,   13,   13, 0x0a,
     406,  397,   13,   13, 0x0a,
     433,   13,   13,   13, 0x08,
     460,   13,   13,   13, 0x08,
     478,   13,   13,   13, 0x08,
     496,   13,   13,   13, 0x08,
     516,   13,   13,   13, 0x08,
     549,  539,   13,   13, 0x08,
     577,   13,   13,   13, 0x08,
     591,   13,   13,   13, 0x08,
     611,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtChatWidget[] = {
    "QtChatWidget\0\0session,msg\0"
    "newMessage(IMChatSession*,QString)\0"
    "imContact\0contactAddedEventSignal(IMContact)\0"
    "contactRemovedEventSignal(IMContact)\0"
    "ctrlTabPressed()\0profileChangedEventHandlerSignal()\0"
    "chatEditTextChanged()\0changeFont()\0"
    "chooseEmoticon()\0emoticon\0"
    "emoticonSelected(QtEmoticon)\0"
    "showInviteDialog()\0contactAddedEventSlot(IMContact)\0"
    "contactRemovedEventSlot(IMContact)\0"
    "visible\0setVisible(bool)\0filename\0"
    "sendFileToSession(QString)\0"
    "avatarFrameButtonClicked()\0changeFontColor()\0"
    "boldClickedSlot()\0italicClickedSlot()\0"
    "underlineClickedSlot()\0contactId\0"
    "contactChangedSlot(QString)\0sendMessage()\0"
    "stoppedTypingSlot()\0updateUserAvatar()\0"
};

const QMetaObject QtChatWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtChatWidget,
      qt_meta_data_QtChatWidget, 0 }
};

const QMetaObject *QtChatWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatWidget))
        return static_cast<void*>(const_cast< QtChatWidget*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtChatWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtChatWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newMessage((*reinterpret_cast< IMChatSession*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: contactAddedEventSignal((*reinterpret_cast< const IMContact(*)>(_a[1]))); break;
        case 2: contactRemovedEventSignal((*reinterpret_cast< const IMContact(*)>(_a[1]))); break;
        case 3: ctrlTabPressed(); break;
        case 4: profileChangedEventHandlerSignal(); break;
        case 5: chatEditTextChanged(); break;
        case 6: changeFont(); break;
        case 7: chooseEmoticon(); break;
        case 8: emoticonSelected((*reinterpret_cast< QtEmoticon(*)>(_a[1]))); break;
        case 9: showInviteDialog(); break;
        case 10: contactAddedEventSlot((*reinterpret_cast< const IMContact(*)>(_a[1]))); break;
        case 11: contactRemovedEventSlot((*reinterpret_cast< const IMContact(*)>(_a[1]))); break;
        case 12: setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: sendFileToSession((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: avatarFrameButtonClicked(); break;
        case 15: changeFontColor(); break;
        case 16: boldClickedSlot(); break;
        case 17: italicClickedSlot(); break;
        case 18: underlineClickedSlot(); break;
        case 19: contactChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: sendMessage(); break;
        case 21: stoppedTypingSlot(); break;
        case 22: updateUserAvatar(); break;
        }
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void QtChatWidget::newMessage(IMChatSession * _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtChatWidget::contactAddedEventSignal(const IMContact & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtChatWidget::contactRemovedEventSignal(const IMContact & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtChatWidget::ctrlTabPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QtChatWidget::profileChangedEventHandlerSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
