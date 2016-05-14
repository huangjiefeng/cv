/****************************************************************************
** Meta object code from reading C++ file 'ptz_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ptz_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ptz_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PTZController_t {
    QByteArrayData data[23];
    char stringdata[332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PTZController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PTZController_t qt_meta_stringdata_PTZController = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PTZController"
QT_MOC_LITERAL(1, 14, 21), // "SerialPortErrorSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(4, 66, 5), // "error"
QT_MOC_LITERAL(5, 72, 28), // "SerialPortSettingErrorSignal"
QT_MOC_LITERAL(6, 101, 20), // "OpenComSuccessSignal"
QT_MOC_LITERAL(7, 122, 11), // "OpenComSlot"
QT_MOC_LITERAL(8, 134, 12), // "CloseComSlot"
QT_MOC_LITERAL(9, 147, 20), // "SetCameraAddressSlot"
QT_MOC_LITERAL(10, 168, 4), // "addr"
QT_MOC_LITERAL(11, 173, 18), // "SetPtzProtocolSlot"
QT_MOC_LITERAL(12, 192, 13), // "protocol_type"
QT_MOC_LITERAL(13, 206, 12), // "TurnLeftSlot"
QT_MOC_LITERAL(14, 219, 13), // "TurnRightSlot"
QT_MOC_LITERAL(15, 233, 10), // "TurnUpSlot"
QT_MOC_LITERAL(16, 244, 12), // "TurnDownSlot"
QT_MOC_LITERAL(17, 257, 12), // "FocusFarSlot"
QT_MOC_LITERAL(18, 270, 13), // "FocusNearSlot"
QT_MOC_LITERAL(19, 284, 10), // "ZoomInSlot"
QT_MOC_LITERAL(20, 295, 11), // "ZoomOutSlot"
QT_MOC_LITERAL(21, 307, 8), // "StopSlot"
QT_MOC_LITERAL(22, 316, 15) // "CallPreset1Slot"

    },
    "PTZController\0SerialPortErrorSignal\0"
    "\0QSerialPort::SerialPortError\0error\0"
    "SerialPortSettingErrorSignal\0"
    "OpenComSuccessSignal\0OpenComSlot\0"
    "CloseComSlot\0SetCameraAddressSlot\0"
    "addr\0SetPtzProtocolSlot\0protocol_type\0"
    "TurnLeftSlot\0TurnRightSlot\0TurnUpSlot\0"
    "TurnDownSlot\0FocusFarSlot\0FocusNearSlot\0"
    "ZoomInSlot\0ZoomOutSlot\0StopSlot\0"
    "CallPreset1Slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PTZController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    0,  103,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  104,    2, 0x0a /* Public */,
       8,    0,  105,    2, 0x0a /* Public */,
       9,    1,  106,    2, 0x0a /* Public */,
      11,    1,  109,    2, 0x0a /* Public */,
      13,    0,  112,    2, 0x0a /* Public */,
      14,    0,  113,    2, 0x0a /* Public */,
      15,    0,  114,    2, 0x0a /* Public */,
      16,    0,  115,    2, 0x0a /* Public */,
      17,    0,  116,    2, 0x0a /* Public */,
      18,    0,  117,    2, 0x0a /* Public */,
      19,    0,  118,    2, 0x0a /* Public */,
      20,    0,  119,    2, 0x0a /* Public */,
      21,    0,  120,    2, 0x0a /* Public */,
      22,    0,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UChar,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PTZController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PTZController *_t = static_cast<PTZController *>(_o);
        switch (_id) {
        case 0: _t->SerialPortErrorSignal((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 1: _t->SerialPortSettingErrorSignal(); break;
        case 2: _t->OpenComSuccessSignal(); break;
        case 3: _t->OpenComSlot(); break;
        case 4: _t->CloseComSlot(); break;
        case 5: _t->SetCameraAddressSlot((*reinterpret_cast< unsigned char(*)>(_a[1]))); break;
        case 6: _t->SetPtzProtocolSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->TurnLeftSlot(); break;
        case 8: _t->TurnRightSlot(); break;
        case 9: _t->TurnUpSlot(); break;
        case 10: _t->TurnDownSlot(); break;
        case 11: _t->FocusFarSlot(); break;
        case 12: _t->FocusNearSlot(); break;
        case 13: _t->ZoomInSlot(); break;
        case 14: _t->ZoomOutSlot(); break;
        case 15: _t->StopSlot(); break;
        case 16: _t->CallPreset1Slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PTZController::*_t)(QSerialPort::SerialPortError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PTZController::SerialPortErrorSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (PTZController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PTZController::SerialPortSettingErrorSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (PTZController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PTZController::OpenComSuccessSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject PTZController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PTZController.data,
      qt_meta_data_PTZController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PTZController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PTZController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PTZController.stringdata))
        return static_cast<void*>(const_cast< PTZController*>(this));
    return QObject::qt_metacast(_clname);
}

int PTZController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void PTZController::SerialPortErrorSignal(QSerialPort::SerialPortError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PTZController::SerialPortSettingErrorSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void PTZController::OpenComSuccessSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
