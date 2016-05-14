/****************************************************************************
** Meta object code from reading C++ file 'QtWengoStyleBar.h'
**
** Created: Thu Jul 29 12:39:37 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtWengoStyleBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtWengoStyleBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtWengoStyleBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      41,   16,   16,   16, 0x05,
      60,   16,   16,   16, 0x05,
      84,   16,   16,   16, 0x05,
     103,   16,   16,   16, 0x05,
     124,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtWengoStyleBar[] = {
    "QtWengoStyleBar\0\0emoticonsLabelClicked()\0"
    "fontLabelClicked()\0fontColorLabelClicked()\0"
    "boldLabelClicked()\0italicLabelClicked()\0"
    "underlineLabelClicked()\0"
};

const QMetaObject QtWengoStyleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtWengoStyleBar,
      qt_meta_data_QtWengoStyleBar, 0 }
};

const QMetaObject *QtWengoStyleBar::metaObject() const
{
    return &staticMetaObject;
}

void *QtWengoStyleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtWengoStyleBar))
        return static_cast<void*>(const_cast< QtWengoStyleBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtWengoStyleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emoticonsLabelClicked(); break;
        case 1: fontLabelClicked(); break;
        case 2: fontColorLabelClicked(); break;
        case 3: boldLabelClicked(); break;
        case 4: italicLabelClicked(); break;
        case 5: underlineLabelClicked(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QtWengoStyleBar::emoticonsLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QtWengoStyleBar::fontLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QtWengoStyleBar::fontColorLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QtWengoStyleBar::boldLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QtWengoStyleBar::italicLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void QtWengoStyleBar::underlineLabelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
