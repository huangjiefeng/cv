/****************************************************************************
** Meta object code from reading C++ file 'monitor_tab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monitor_tab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MonitorTab_t {
    QByteArrayData data[12];
    char stringdata[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorTab_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorTab_t qt_meta_stringdata_MonitorTab = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MonitorTab"
QT_MOC_LITERAL(1, 11, 25), // "RequestHardwareInfoSignal"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 24), // "StartRequestHardwareInfo"
QT_MOC_LITERAL(4, 63, 23), // "StopRequestHarewareInfo"
QT_MOC_LITERAL(5, 87, 21), // "saveMonitorConfigSlot"
QT_MOC_LITERAL(6, 109, 28), // "HandleHardwareInfoNotifySlot"
QT_MOC_LITERAL(7, 138, 12), // "HardwareInfo"
QT_MOC_LITERAL(8, 151, 4), // "info"
QT_MOC_LITERAL(9, 156, 27), // "HandleNetworkInfoNotifySlot"
QT_MOC_LITERAL(10, 184, 12), // "in_bandwidth"
QT_MOC_LITERAL(11, 197, 13) // "out_bandwidth"

    },
    "MonitorTab\0RequestHardwareInfoSignal\0"
    "\0StartRequestHardwareInfo\0"
    "StopRequestHarewareInfo\0saveMonitorConfigSlot\0"
    "HandleHardwareInfoNotifySlot\0HardwareInfo\0"
    "info\0HandleNetworkInfoNotifySlot\0"
    "in_bandwidth\0out_bandwidth"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorTab[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       9,    2,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   10,   11,

       0        // eod
};

void MonitorTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MonitorTab *_t = static_cast<MonitorTab *>(_o);
        switch (_id) {
        case 0: _t->RequestHardwareInfoSignal(); break;
        case 1: _t->StartRequestHardwareInfo(); break;
        case 2: _t->StopRequestHarewareInfo(); break;
        case 3: _t->saveMonitorConfigSlot(); break;
        case 4: _t->HandleHardwareInfoNotifySlot((*reinterpret_cast< const HardwareInfo(*)>(_a[1]))); break;
        case 5: _t->HandleNetworkInfoNotifySlot((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MonitorTab::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MonitorTab::RequestHardwareInfoSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MonitorTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MonitorTab.data,
      qt_meta_data_MonitorTab,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MonitorTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorTab::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorTab.stringdata))
        return static_cast<void*>(const_cast< MonitorTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int MonitorTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MonitorTab::RequestHardwareInfoSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
