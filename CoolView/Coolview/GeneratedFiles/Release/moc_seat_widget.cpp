/****************************************************************************
** Meta object code from reading C++ file 'seat_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../seat_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'seat_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SeatWidget_t {
    QByteArrayData data[10];
    char stringdata[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SeatWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SeatWidget_t qt_meta_stringdata_SeatWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SeatWidget"
QT_MOC_LITERAL(1, 11, 18), // "ReceiveVideoSignal"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "vuri"
QT_MOC_LITERAL(4, 36, 10), // "seat_index"
QT_MOC_LITERAL(5, 47, 15), // "use_small_video"
QT_MOC_LITERAL(6, 63, 15), // "StopVideoSignal"
QT_MOC_LITERAL(7, 79, 21), // "ReceiveDocumentSignal"
QT_MOC_LITERAL(8, 101, 3), // "uri"
QT_MOC_LITERAL(9, 105, 18) // "StopDocumentSignal"

    },
    "SeatWidget\0ReceiveVideoSignal\0\0vuri\0"
    "seat_index\0use_small_video\0StopVideoSignal\0"
    "ReceiveDocumentSignal\0uri\0StopDocumentSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SeatWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       6,    2,   41,    2, 0x06 /* Public */,
       7,    2,   46,    2, 0x06 /* Public */,
       9,    2,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    4,

       0        // eod
};

void SeatWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SeatWidget *_t = static_cast<SeatWidget *>(_o);
        switch (_id) {
        case 0: _t->ReceiveVideoSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->StopVideoSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->ReceiveDocumentSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->StopDocumentSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SeatWidget::*_t)(QString , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeatWidget::ReceiveVideoSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SeatWidget::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeatWidget::StopVideoSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (SeatWidget::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeatWidget::ReceiveDocumentSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (SeatWidget::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SeatWidget::StopDocumentSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject SeatWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SeatWidget.data,
      qt_meta_data_SeatWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SeatWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeatWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SeatWidget.stringdata))
        return static_cast<void*>(const_cast< SeatWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SeatWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SeatWidget::ReceiveVideoSignal(QString _t1, int _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SeatWidget::StopVideoSignal(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SeatWidget::ReceiveDocumentSignal(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SeatWidget::StopDocumentSignal(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
