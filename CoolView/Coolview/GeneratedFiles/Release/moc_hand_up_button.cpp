/****************************************************************************
** Meta object code from reading C++ file 'hand_up_button.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../hand_up_button.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hand_up_button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HandUpButton_t {
    QByteArrayData data[6];
    char stringdata[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HandUpButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HandUpButton_t qt_meta_stringdata_HandUpButton = {
    {
QT_MOC_LITERAL(0, 0, 12), // "HandUpButton"
QT_MOC_LITERAL(1, 13, 12), // "HandUpSignal"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "handup"
QT_MOC_LITERAL(4, 34, 10), // "HandUpSlot"
QT_MOC_LITERAL(5, 45, 22) // "HandleHandUpNotifySlot"

    },
    "HandUpButton\0HandUpSignal\0\0handup\0"
    "HandUpSlot\0HandleHandUpNotifySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HandUpButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x08 /* Private */,
       5,    1,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void HandUpButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HandUpButton *_t = static_cast<HandUpButton *>(_o);
        switch (_id) {
        case 0: _t->HandUpSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->HandUpSlot(); break;
        case 2: _t->HandleHandUpNotifySlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HandUpButton::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HandUpButton::HandUpSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject HandUpButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_HandUpButton.data,
      qt_meta_data_HandUpButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HandUpButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HandUpButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HandUpButton.stringdata))
        return static_cast<void*>(const_cast< HandUpButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int HandUpButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void HandUpButton::HandUpSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
