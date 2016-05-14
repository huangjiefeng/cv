/****************************************************************************
** Meta object code from reading C++ file 'QtDialpad.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../wengophone/src/presentation/qt/QtDialpad.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtDialpad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtDialpad[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      30,   10,   10,   10, 0x08,
      49,   10,   10,   10, 0x08,
      70,   10,   10,   10, 0x08,
      90,   10,   10,   10, 0x08,
     110,   10,   10,   10, 0x08,
     129,   10,   10,   10, 0x08,
     150,   10,   10,   10, 0x08,
     171,   10,   10,   10, 0x08,
     191,   10,   10,   10, 0x08,
     211,   10,   10,   10, 0x08,
     231,   10,   10,   10, 0x08,
     265,  252,   10,   10, 0x08,
     290,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtDialpad[] = {
    "QtDialpad\0\0oneButtonClicked()\0"
    "twoButtonClicked()\0threeButtonClicked()\0"
    "fourButtonClicked()\0fiveButtonClicked()\0"
    "sixButtonClicked()\0sevenButtonClicked()\0"
    "eightButtonClicked()\0nineButtonClicked()\0"
    "zeroButtonClicked()\0starButtonClicked()\0"
    "poundButtonClicked()\0newThemeName\0"
    "setCurrentTheme(QString)\0refreshComboBox()\0"
};

const QMetaObject QtDialpad::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtDialpad,
      qt_meta_data_QtDialpad, 0 }
};

const QMetaObject *QtDialpad::metaObject() const
{
    return &staticMetaObject;
}

void *QtDialpad::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtDialpad))
        return static_cast<void*>(const_cast< QtDialpad*>(this));
    if (!strcmp(_clname, "NonCopyable"))
        return static_cast< NonCopyable*>(const_cast< QtDialpad*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtDialpad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: oneButtonClicked(); break;
        case 1: twoButtonClicked(); break;
        case 2: threeButtonClicked(); break;
        case 3: fourButtonClicked(); break;
        case 4: fiveButtonClicked(); break;
        case 5: sixButtonClicked(); break;
        case 6: sevenButtonClicked(); break;
        case 7: eightButtonClicked(); break;
        case 8: nineButtonClicked(); break;
        case 9: zeroButtonClicked(); break;
        case 10: starButtonClicked(); break;
        case 11: poundButtonClicked(); break;
        case 12: setCurrentTheme((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: refreshComboBox(); break;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
