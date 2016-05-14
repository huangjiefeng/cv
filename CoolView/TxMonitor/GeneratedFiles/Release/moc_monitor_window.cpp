/****************************************************************************
** Meta object code from reading C++ file 'monitor_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MonitorWindow_t {
    QByteArrayData data[13];
    char stringdata[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorWindow_t qt_meta_stringdata_MonitorWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MonitorWindow"
QT_MOC_LITERAL(1, 14, 28), // "HandleWeblibErrorMessageSlot"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 3), // "msg"
QT_MOC_LITERAL(4, 48, 31), // "HandleNetworkScheduleNotifySlot"
QT_MOC_LITERAL(5, 80, 11), // "NetworkInfo"
QT_MOC_LITERAL(6, 92, 4), // "info"
QT_MOC_LITERAL(7, 97, 18), // "weblib_speed_limit"
QT_MOC_LITERAL(8, 116, 35), // "HandleFreeDiskSpaceChangeNoti..."
QT_MOC_LITERAL(9, 152, 5), // "space"
QT_MOC_LITERAL(10, 158, 22), // "HandleViewSelectedSlot"
QT_MOC_LITERAL(11, 181, 3), // "vid"
QT_MOC_LITERAL(12, 185, 4) // "file"

    },
    "MonitorWindow\0HandleWeblibErrorMessageSlot\0"
    "\0msg\0HandleNetworkScheduleNotifySlot\0"
    "NetworkInfo\0info\0weblib_speed_limit\0"
    "HandleFreeDiskSpaceChangeNotifySlot\0"
    "space\0HandleViewSelectedSlot\0vid\0file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    2,   37,    2, 0x0a /* Public */,
       8,    1,   42,    2, 0x0a /* Public */,
      10,    2,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5, QMetaType::ULongLong,    6,    7,
    QMetaType::Void, QMetaType::ULongLong,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   11,   12,

       0        // eod
};

void MonitorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MonitorWindow *_t = static_cast<MonitorWindow *>(_o);
        switch (_id) {
        case 0: _t->HandleWeblibErrorMessageSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->HandleNetworkScheduleNotifySlot((*reinterpret_cast< const NetworkInfo(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 2: _t->HandleFreeDiskSpaceChangeNotifySlot((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _t->HandleViewSelectedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject MonitorWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MonitorWindow.data,
      qt_meta_data_MonitorWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MonitorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorWindow.stringdata))
        return static_cast<void*>(const_cast< MonitorWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MonitorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
