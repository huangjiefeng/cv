/****************************************************************************
** Meta object code from reading C++ file 'virtual_terminal_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../virtual_terminal_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'virtual_terminal_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VirtualTerminalWidget_t {
    QByteArrayData data[7];
    char stringdata[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualTerminalWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualTerminalWidget_t qt_meta_stringdata_VirtualTerminalWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "VirtualTerminalWidget"
QT_MOC_LITERAL(1, 22, 23), // "StopReceivedVideoSignal"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 4), // "vuri"
QT_MOC_LITERAL(4, 52, 20), // "CutFileRequestSignal"
QT_MOC_LITERAL(5, 73, 21), // "StopReceivedVideoSlot"
QT_MOC_LITERAL(6, 95, 11) // "CutFileSlot"

    },
    "VirtualTerminalWidget\0StopReceivedVideoSignal\0"
    "\0vuri\0CutFileRequestSignal\0"
    "StopReceivedVideoSlot\0CutFileSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualTerminalWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   40,    2, 0x08 /* Private */,
       6,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VirtualTerminalWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VirtualTerminalWidget *_t = static_cast<VirtualTerminalWidget *>(_o);
        switch (_id) {
        case 0: _t->StopReceivedVideoSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->CutFileRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->StopReceivedVideoSlot(); break;
        case 3: _t->CutFileSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VirtualTerminalWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VirtualTerminalWidget::StopReceivedVideoSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (VirtualTerminalWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VirtualTerminalWidget::CutFileRequestSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject VirtualTerminalWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VirtualTerminalWidget.data,
      qt_meta_data_VirtualTerminalWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VirtualTerminalWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualTerminalWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualTerminalWidget.stringdata))
        return static_cast<void*>(const_cast< VirtualTerminalWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int VirtualTerminalWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void VirtualTerminalWidget::StopReceivedVideoSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VirtualTerminalWidget::CutFileRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
