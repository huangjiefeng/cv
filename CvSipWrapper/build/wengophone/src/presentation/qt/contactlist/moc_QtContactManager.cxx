/****************************************************************************
** Meta object code from reading C++ file 'QtContactManager.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/contactlist/QtContactManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtContactManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtContactManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      28,   18,   17,   17, 0x05,
      68,   55,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     121,  114,   17,   17, 0x0a,
     153,   17,   17,   17, 0x0a,
     182,  172,   17,   17, 0x0a,
     208,  203,   17,   17, 0x08,
     240,   17,   17,   17, 0x08,
     256,   17,   17,   17, 0x08,
     270,   17,   17,   17, 0x08,
     303,  291,   17,   17, 0x08,
     337,  203,   17,   17, 0x08,
     371,   17,   17,   17, 0x08,
     390,   17,   17,   17, 0x08,
     419,  407,   17,   17, 0x08,
     438,   17,   17,   17, 0x28,
     453,   17,   17,   17, 0x08,
     465,   17,   17,   17, 0x08,
     476,   17,   17,   17, 0x08,
     492,   17,   17,   17, 0x08,
     510,   17,   17,   17, 0x08,
     521,   17,   17,   17, 0x08,
     538,   17,   17,   17, 0x08,
     554,   17,   17,   17, 0x08,
     568,   17,   17,   17, 0x08,
     595,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtContactManager[] = {
    "QtContactManager\0\0groupName\0"
    "groupRightClicked(QString)\0phone,target\0"
    "inviteToConferenceClicked(QString,PhoneCall*)\0"
    "button\0setMouseButton(Qt::MouseButton)\0"
    "userStateChanged()\0contactId\0"
    "editContact(QString)\0item\0"
    "defaultAction(QTreeWidgetItem*)\0"
    "deleteContact()\0editContact()\0"
    "inviteToConference()\0item,column\0"
    "itemClicked(QTreeWidgetItem*,int)\0"
    "openContactInfo(QTreeWidgetItem*)\0"
    "closeContactInfo()\0redrawContacts()\0"
    "bypassTimer\0sortContacts(bool)\0"
    "sortContacts()\0startChat()\0sendFile()\0"
    "startHomeCall()\0startMobileCall()\0"
    "startSMS()\0startWengoCall()\0startWorkCall()\0"
    "goToWebsite()\0treeViewSelectionChanged()\0"
    "languageChanged()\0"
};

const QMetaObject QtContactManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtContactManager,
      qt_meta_data_QtContactManager, 0 }
};

const QMetaObject *QtContactManager::metaObject() const
{
    return &staticMetaObject;
}

void *QtContactManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtContactManager))
        return static_cast<void*>(const_cast< QtContactManager*>(this));
    return QObject::qt_metacast(_clname);
}

int QtContactManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: groupRightClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: inviteToConferenceClicked((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< PhoneCall*(*)>(_a[2]))); break;
        case 2: setMouseButton((*reinterpret_cast< Qt::MouseButton(*)>(_a[1]))); break;
        case 3: userStateChanged(); break;
        case 4: editContact((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: defaultAction((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: deleteContact(); break;
        case 7: editContact(); break;
        case 8: inviteToConference(); break;
        case 9: itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: openContactInfo((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 11: closeContactInfo(); break;
        case 12: redrawContacts(); break;
        case 13: sortContacts((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: sortContacts(); break;
        case 15: startChat(); break;
        case 16: sendFile(); break;
        case 17: startHomeCall(); break;
        case 18: startMobileCall(); break;
        case 19: startSMS(); break;
        case 20: startWengoCall(); break;
        case 21: startWorkCall(); break;
        case 22: goToWebsite(); break;
        case 23: treeViewSelectionChanged(); break;
        case 24: languageChanged(); break;
        }
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void QtContactManager::groupRightClicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtContactManager::inviteToConferenceClicked(QString _t1, PhoneCall * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
