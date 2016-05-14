/****************************************************************************
** Meta object code from reading C++ file 'QtVideoQt.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/phonecall/QtVideoQt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtVideoQt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtVideoQt[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   10,   10,   10, 0x08,
      61,   10,   10,   10, 0x08,
      74,   10,   10,   10, 0x08,
      88,   10,   10,   10, 0x08,
     114,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtVideoQt[] = {
    "QtVideoQt\0\0toggleFlipVideoImageSignal()\0"
    "toggleAttachDetach()\0paintEvent()\0"
    "resizeEvent()\0fullScreenButtonClicked()\0"
    "flipWebcamButtonClicked()\0"
};

const QMetaObject QtVideoQt::staticMetaObject = {
    { &QtVideo::staticMetaObject, qt_meta_stringdata_QtVideoQt,
      qt_meta_data_QtVideoQt, 0 }
};

const QMetaObject *QtVideoQt::metaObject() const
{
    return &staticMetaObject;
}

void *QtVideoQt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtVideoQt))
        return static_cast<void*>(const_cast< QtVideoQt*>(this));
    return QtVideo::qt_metacast(_clname);
}

int QtVideoQt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtVideo::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: toggleFlipVideoImageSignal(); break;
        case 1: toggleAttachDetach(); break;
        case 2: paintEvent(); break;
        case 3: resizeEvent(); break;
        case 4: fullScreenButtonClicked(); break;
        case 5: flipWebcamButtonClicked(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QtVideoQt::toggleFlipVideoImageSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
