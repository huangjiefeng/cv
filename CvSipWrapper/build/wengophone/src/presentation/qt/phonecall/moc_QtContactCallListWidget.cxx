/****************************************************************************
** Meta object code from reading C++ file 'QtContactCallListWidget.h'
**
** Created: Thu Jul 29 12:39:41 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/phonecall/QtContactCallListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtContactCallListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtContactCallListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   24,   24,   24, 0x0a,
      59,   49,   24,   24, 0x0a,
     104,   92,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtContactCallListWidget[] = {
    "QtContactCallListWidget\0\0aboutToClose()\0"
    "hangup()\0phoneCall\0slotStartedTalking(QtPhoneCall*)\0"
    "qtPhoneCall\0slotQtPhoneCallDestroyed(QObject*)\0"
};

const QMetaObject QtContactCallListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtContactCallListWidget,
      qt_meta_data_QtContactCallListWidget, 0 }
};

const QMetaObject *QtContactCallListWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtContactCallListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtContactCallListWidget))
        return static_cast<void*>(const_cast< QtContactCallListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtContactCallListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: aboutToClose(); break;
        case 1: hangup(); break;
        case 2: slotStartedTalking((*reinterpret_cast< QtPhoneCall*(*)>(_a[1]))); break;
        case 3: slotQtPhoneCallDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void QtContactCallListWidget::aboutToClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
