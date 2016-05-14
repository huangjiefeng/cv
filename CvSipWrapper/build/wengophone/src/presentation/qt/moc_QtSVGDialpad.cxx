/****************************************************************************
** Meta object code from reading C++ file 'QtSVGDialpad.h'
**
** Created: Thu Jul 29 12:39:38 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtSVGDialpad.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtSVGDialpad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSVGDialpad[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      32,   13,   13,   13, 0x05,
      49,   13,   13,   13, 0x05,
      66,   13,   13,   13, 0x05,
      85,   13,   13,   13, 0x05,
     103,   13,   13,   13, 0x05,
     121,   13,   13,   13, 0x05,
     138,   13,   13,   13, 0x05,
     157,   13,   13,   13, 0x05,
     176,   13,   13,   13, 0x05,
     194,   13,   13,   13, 0x05,
     212,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtSVGDialpad[] = {
    "QtSVGDialpad\0\0keyZeroSelected()\0"
    "keyOneSelected()\0keyTwoSelected()\0"
    "keyThreeSelected()\0keyFourSelected()\0"
    "keyFiveSelected()\0keySixSelected()\0"
    "keySevenSelected()\0keyEightSelected()\0"
    "keyNineSelected()\0keyStarSelected()\0"
    "keyPoundSelected()\0"
};

const QMetaObject QtSVGDialpad::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QtSVGDialpad,
      qt_meta_data_QtSVGDialpad, 0 }
};

const QMetaObject *QtSVGDialpad::metaObject() const
{
    return &staticMetaObject;
}

void *QtSVGDialpad::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSVGDialpad))
        return static_cast<void*>(const_cast< QtSVGDialpad*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtSVGDialpad*>(this));
    return QFrame::qt_metacast(_clname);
}

int QtSVGDialpad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: keyZeroSelected(); break;
        case 1: keyOneSelected(); break;
        case 2: keyTwoSelected(); break;
        case 3: keyThreeSelected(); break;
        case 4: keyFourSelected(); break;
        case 5: keyFiveSelected(); break;
        case 6: keySixSelected(); break;
        case 7: keySevenSelected(); break;
        case 8: keyEightSelected(); break;
        case 9: keyNineSelected(); break;
        case 10: keyStarSelected(); break;
        case 11: keyPoundSelected(); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QtSVGDialpad::keyZeroSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtSVGDialpad::keyOneSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QtSVGDialpad::keyTwoSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QtSVGDialpad::keyThreeSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QtSVGDialpad::keyFourSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void QtSVGDialpad::keyFiveSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void QtSVGDialpad::keySixSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void QtSVGDialpad::keySevenSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void QtSVGDialpad::keyEightSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void QtSVGDialpad::keyNineSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void QtSVGDialpad::keyStarSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void QtSVGDialpad::keyPoundSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}
QT_END_MOC_NAMESPACE
