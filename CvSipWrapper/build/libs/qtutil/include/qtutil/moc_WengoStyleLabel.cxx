/****************************************************************************
** Meta object code from reading C++ file 'WengoStyleLabel.h'
**
** Created: Mon Apr 20 22:16:01 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/WengoStyleLabel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WengoStyleLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WengoStyleLabel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   27,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WengoStyleLabel[] = {
    "WengoStyleLabel\0\0clicked()\0text\0"
    "setText(QString)\0"
};

const QMetaObject WengoStyleLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_WengoStyleLabel,
      qt_meta_data_WengoStyleLabel, 0 }
};

const QMetaObject *WengoStyleLabel::metaObject() const
{
    return &staticMetaObject;
}

void *WengoStyleLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WengoStyleLabel))
        return static_cast<void*>(const_cast< WengoStyleLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int WengoStyleLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void WengoStyleLabel::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
