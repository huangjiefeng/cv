/****************************************************************************
** Meta object code from reading C++ file 'QtWengoConfigDialog.h'
**
** Created: Thu Jul 29 12:39:37 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/config/QtWengoConfigDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtWengoConfigDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtWengoConfigDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      28,   20,   20,   20, 0x0a,
      46,   20,   20,   20, 0x0a,
      65,   20,   20,   20, 0x0a,
      84,   20,   20,   20, 0x0a,
     102,   20,   20,   20, 0x0a,
     118,   20,   20,   20, 0x0a,
     134,   20,   20,   20, 0x0a,
     158,   20,   20,   20, 0x0a,
     180,   20,   20,   20, 0x0a,
     199,   20,   20,   20, 0x0a,
     219,   20,   20,   20, 0x0a,
     242,  238,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtWengoConfigDialog[] = {
    "QtWengoConfigDialog\0\0save()\0"
    "showGeneralPage()\0showLanguagePage()\0"
    "showAccountsPage()\0showPrivacyPage()\0"
    "showAudioPage()\0showVideoPage()\0"
    "showNotificationsPage()\0showCallForwardPage()\0"
    "showAdvancedPage()\0showVoicemailPage()\0"
    "showSecurityPage()\0row\0showSettingPage(int)\0"
};

const QMetaObject QtWengoConfigDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtWengoConfigDialog,
      qt_meta_data_QtWengoConfigDialog, 0 }
};

const QMetaObject *QtWengoConfigDialog::metaObject() const
{
    return &staticMetaObject;
}

void *QtWengoConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtWengoConfigDialog))
        return static_cast<void*>(const_cast< QtWengoConfigDialog*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtWengoConfigDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QtWengoConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: save(); break;
        case 1: showGeneralPage(); break;
        case 2: showLanguagePage(); break;
        case 3: showAccountsPage(); break;
        case 4: showPrivacyPage(); break;
        case 5: showAudioPage(); break;
        case 6: showVideoPage(); break;
        case 7: showNotificationsPage(); break;
        case 8: showCallForwardPage(); break;
        case 9: showAdvancedPage(); break;
        case 10: showVoicemailPage(); break;
        case 11: showSecurityPage(); break;
        case 12: showSettingPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
