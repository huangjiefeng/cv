/****************************************************************************
** Meta object code from reading C++ file 'devicelist_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../devicelist_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicelist_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DeviceListWidget_t {
    QByteArrayData data[11];
    char stringdata[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceListWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceListWidget_t qt_meta_stringdata_DeviceListWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DeviceListWidget"
QT_MOC_LITERAL(1, 17, 20), // "HandleTabChangedSlot"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 5), // "index"
QT_MOC_LITERAL(4, 45, 10), // "ReadConfig"
QT_MOC_LITERAL(5, 56, 31), // "HandleActivateCameraSettingSlot"
QT_MOC_LITERAL(6, 88, 30), // "HandleActivateDeviceConfigSlot"
QT_MOC_LITERAL(7, 119, 18), // "TerminalConfigType"
QT_MOC_LITERAL(8, 138, 4), // "type"
QT_MOC_LITERAL(9, 143, 30), // "HandlePrimaryScreenChangedSlot"
QT_MOC_LITERAL(10, 174, 14) // "primary_screen"

    },
    "DeviceListWidget\0HandleTabChangedSlot\0"
    "\0index\0ReadConfig\0HandleActivateCameraSettingSlot\0"
    "HandleActivateDeviceConfigSlot\0"
    "TerminalConfigType\0type\0"
    "HandlePrimaryScreenChangedSlot\0"
    "primary_screen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    0,   42,    2, 0x0a /* Public */,
       5,    1,   43,    2, 0x0a /* Public */,
       6,    1,   46,    2, 0x0a /* Public */,
       9,    1,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void DeviceListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceListWidget *_t = static_cast<DeviceListWidget *>(_o);
        switch (_id) {
        case 0: _t->HandleTabChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ReadConfig(); break;
        case 2: _t->HandleActivateCameraSettingSlot((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->HandleActivateDeviceConfigSlot((*reinterpret_cast< TerminalConfigType(*)>(_a[1]))); break;
        case 4: _t->HandlePrimaryScreenChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DeviceListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DeviceListWidget.data,
      qt_meta_data_DeviceListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DeviceListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceListWidget.stringdata))
        return static_cast<void*>(const_cast< DeviceListWidget*>(this));
    if (!strcmp(_clname, "Ui::DeviceSettingForm"))
        return static_cast< Ui::DeviceSettingForm*>(const_cast< DeviceListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DeviceListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
