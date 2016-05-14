/****************************************************************************
** Meta object code from reading C++ file 'QtNotificationSettings.h'
**
** Created: Thu Jul 29 12:39:39 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/config/QtNotificationSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtNotificationSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtNotificationSettingsItem[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,
      43,   27,   27,   27, 0x08,
      55,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtNotificationSettingsItem[] = {
    "QtNotificationSettingsItem\0\0browseSounds()\0"
    "playSound()\0updatePlayButton()\0"
};

const QMetaObject QtNotificationSettingsItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtNotificationSettingsItem,
      qt_meta_data_QtNotificationSettingsItem, 0 }
};

const QMetaObject *QtNotificationSettingsItem::metaObject() const
{
    return &staticMetaObject;
}

void *QtNotificationSettingsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtNotificationSettingsItem))
        return static_cast<void*>(const_cast< QtNotificationSettingsItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtNotificationSettingsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: browseSounds(); break;
        case 1: playSound(); break;
        case 2: updatePlayButton(); break;
        }
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_QtNotificationSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QtNotificationSettings[] = {
    "QtNotificationSettings\0"
};

const QMetaObject QtNotificationSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtNotificationSettings,
      qt_meta_data_QtNotificationSettings, 0 }
};

const QMetaObject *QtNotificationSettings::metaObject() const
{
    return &staticMetaObject;
}

void *QtNotificationSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtNotificationSettings))
        return static_cast<void*>(const_cast< QtNotificationSettings*>(this));
    if (!strcmp(_clname, "QtISettings"))
        return static_cast< QtISettings*>(const_cast< QtNotificationSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtNotificationSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
