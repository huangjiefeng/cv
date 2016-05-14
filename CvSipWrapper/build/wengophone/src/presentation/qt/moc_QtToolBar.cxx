/****************************************************************************
** Meta object code from reading C++ file 'QtToolBar.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtToolBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtToolBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   10,   10,   10, 0x0a,
      57,   10,   10,   10, 0x0a,
      73,   10,   10,   10, 0x0a,
      86,   10,   10,   10, 0x0a,
      99,   10,   10,   10, 0x0a,
     116,   10,   10,   10, 0x0a,
     128,   10,   10,   10, 0x0a,
     138,   10,   10,   10, 0x0a,
     153,   10,   10,   10, 0x0a,
     173,   10,   10,   10, 0x0a,
     197,   10,   10,   10, 0x0a,
     223,   10,   10,   10, 0x0a,
     246,   10,   10,   10, 0x0a,
     274,   10,   10,   10, 0x0a,
     302,   10,   10,   10, 0x0a,
     328,   10,   10,   10, 0x0a,
     356,   10,   10,   10, 0x0a,
     383,   10,   10,   10, 0x0a,
     401,   10,   10,   10, 0x0a,
     419,   10,   10,   10, 0x0a,
     440,   10,   10,   10, 0x0a,
     449,   10,   10,   10, 0x0a,
     462,   10,   10,   10, 0x0a,
     479,   10,   10,   10, 0x0a,
     492,   10,   10,   10, 0x0a,
     509,   10,   10,   10, 0x0a,
     534,   10,   10,   10, 0x0a,
     548,   11,   10,   10, 0x0a,
     575,   10,   10,   10, 0x0a,
     602,   10,   10,   10, 0x0a,
     618,   10,   10,   10, 0x0a,
     638,   10,   10,   10, 0x0a,
     656,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtToolBar[] = {
    "QtToolBar\0\0key\0configChanged(QString)\0"
    "showWengoAccount()\0editMyProfile()\0"
    "addContact()\0showConfig()\0showWengoForum()\0"
    "showAbout()\0sendSms()\0showWengoFAQ()\0"
    "showWengoServices()\0showHideContactGroups()\0"
    "showHideOfflineContacts()\0"
    "createConferenceCall()\0"
    "clearHistoryOutgoingCalls()\0"
    "clearHistoryIncomingCalls()\0"
    "clearHistoryMissedCalls()\0"
    "clearHistoryRejectedCalls()\0"
    "clearHistoryChatSessions()\0clearHistorySms()\0"
    "clearHistoryAll()\0searchWengoContact()\0"
    "logOff()\0acceptCall()\0holdResumeCall()\0"
    "hangUpCall()\0showChatWindow()\0"
    "showFileTransferWindow()\0toggleVideo()\0"
    "slotConfigChanged(QString)\0"
    "userProfileIsInitialized()\0enableMenuBar()\0"
    "updateMenuActions()\0tryingToConnect()\0"
    "userProfileDeleted()\0"
};

const QMetaObject QtToolBar::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtToolBar,
      qt_meta_data_QtToolBar, 0 }
};

const QMetaObject *QtToolBar::metaObject() const
{
    return &staticMetaObject;
}

void *QtToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtToolBar))
        return static_cast<void*>(const_cast< QtToolBar*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtToolBar*>(this));
    return QObject::qt_metacast(_clname);
}

int QtToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: configChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: showWengoAccount(); break;
        case 2: editMyProfile(); break;
        case 3: addContact(); break;
        case 4: showConfig(); break;
        case 5: showWengoForum(); break;
        case 6: showAbout(); break;
        case 7: sendSms(); break;
        case 8: showWengoFAQ(); break;
        case 9: showWengoServices(); break;
        case 10: showHideContactGroups(); break;
        case 11: showHideOfflineContacts(); break;
        case 12: createConferenceCall(); break;
        case 13: clearHistoryOutgoingCalls(); break;
        case 14: clearHistoryIncomingCalls(); break;
        case 15: clearHistoryMissedCalls(); break;
        case 16: clearHistoryRejectedCalls(); break;
        case 17: clearHistoryChatSessions(); break;
        case 18: clearHistorySms(); break;
        case 19: clearHistoryAll(); break;
        case 20: searchWengoContact(); break;
        case 21: logOff(); break;
        case 22: acceptCall(); break;
        case 23: holdResumeCall(); break;
        case 24: hangUpCall(); break;
        case 25: showChatWindow(); break;
        case 26: showFileTransferWindow(); break;
        case 27: toggleVideo(); break;
        case 28: slotConfigChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: userProfileIsInitialized(); break;
        case 30: enableMenuBar(); break;
        case 31: updateMenuActions(); break;
        case 32: tryingToConnect(); break;
        case 33: userProfileDeleted(); break;
        }
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void QtToolBar::configChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
