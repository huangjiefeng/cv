/****************************************************************************
** Meta object code from reading C++ file 'QtStatusBar.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/statusbar/QtStatusBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtStatusBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtStatusBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      40,   12,   12,   12, 0x05,
      66,   12,   12,   12, 0x05,
      87,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     109,   12,   12,   12, 0x08,
     130,   12,   12,   12, 0x08,
     148,   12,   12,   12, 0x08,
     170,   12,   12,   12, 0x08,
     191,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtStatusBar[] = {
    "QtStatusBar\0\0audioOutputConfigChanged()\0"
    "audioInputConfigChanged()\0"
    "teleInputVolume(int)\0teleOutputVolume(int)\0"
    "userProfileDeleted()\0languageChanged()\0"
    "updateAudioOutputUi()\0updateAudioInputUi()\0"
    "setVolume(int)\0"
};

const QMetaObject QtStatusBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtStatusBar,
      qt_meta_data_QtStatusBar, 0 }
};

const QMetaObject *QtStatusBar::metaObject() const
{
    return &staticMetaObject;
}

void *QtStatusBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtStatusBar))
        return static_cast<void*>(const_cast< QtStatusBar*>(this));
    if (!strcmp(_clname, "Trackable"))
        return static_cast< Trackable*>(const_cast< QtStatusBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtStatusBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: audioOutputConfigChanged(); break;
        case 1: audioInputConfigChanged(); break;
        case 4: userProfileDeleted(); break;
        case 5: languageChanged(); break;
        case 6: updateAudioOutputUi(); break;
        case 7: updateAudioInputUi(); break;
        case 8: setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QtStatusBar::audioOutputConfigChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtStatusBar::audioInputConfigChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

QT_END_MOC_NAMESPACE
