/****************************************************************************
** Meta object code from reading C++ file 'QtProfileDetails.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/profile/QtProfileDetails.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtProfileDetails.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtProfileDetails[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      44,   17,   17,   17, 0x08,
      68,   17,   17,   17, 0x08,
      82,   17,   17,   17, 0x08,
     100,   17,   17,   17, 0x08,
     122,   17,   17,   17, 0x08,
     146,   17,   17,   17, 0x08,
     169,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtProfileDetails[] = {
    "QtProfileDetails\0\0changeUserProfileAvatar()\0"
    "updateSaveButtonState()\0saveContact()\0"
    "saveUserProfile()\0cancelButtonClicked()\0"
    "advancedButtonClicked()\0websiteButtonClicked()\0"
    "emailButtonClicked()\0"
};

const QMetaObject QtProfileDetails::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtProfileDetails,
      qt_meta_data_QtProfileDetails, 0 }
};

const QMetaObject *QtProfileDetails::metaObject() const
{
    return &staticMetaObject;
}

void *QtProfileDetails::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtProfileDetails))
        return static_cast<void*>(const_cast< QtProfileDetails*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtProfileDetails*>(this));
    return QObject::qt_metacast(_clname);
}

int QtProfileDetails::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeUserProfileAvatar(); break;
        case 1: updateSaveButtonState(); break;
        case 2: saveContact(); break;
        case 3: saveUserProfile(); break;
        case 4: cancelButtonClicked(); break;
        case 5: advancedButtonClicked(); break;
        case 6: websiteButtonClicked(); break;
        case 7: emailButtonClicked(); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
