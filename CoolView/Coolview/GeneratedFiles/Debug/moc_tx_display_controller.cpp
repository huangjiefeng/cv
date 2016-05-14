/****************************************************************************
** Meta object code from reading C++ file 'tx_display_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tx_display_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tx_display_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TXDisplayController_t {
    QByteArrayData data[17];
    char stringdata[306];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TXDisplayController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TXDisplayController_t qt_meta_stringdata_TXDisplayController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "TXDisplayController"
QT_MOC_LITERAL(1, 20, 29), // "HandleReceiveVideoRequestSlot"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 4), // "vuri"
QT_MOC_LITERAL(4, 56, 12), // "screen_index"
QT_MOC_LITERAL(5, 69, 10), // "seat_index"
QT_MOC_LITERAL(6, 80, 15), // "use_small_video"
QT_MOC_LITERAL(7, 96, 26), // "HandleStopVideoRequestSlot"
QT_MOC_LITERAL(8, 123, 29), // "HandleReceiveAudioRequestSlot"
QT_MOC_LITERAL(9, 153, 3), // "uri"
QT_MOC_LITERAL(10, 157, 6), // "enable"
QT_MOC_LITERAL(11, 164, 32), // "HandleReceiveDocumentRequestSlot"
QT_MOC_LITERAL(12, 197, 29), // "HandleOpenDocumentRequestSlot"
QT_MOC_LITERAL(13, 227, 8), // "filename"
QT_MOC_LITERAL(14, 236, 29), // "HandleScreenLayoutRequestSlot"
QT_MOC_LITERAL(15, 266, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(16, 299, 6) // "layout"

    },
    "TXDisplayController\0HandleReceiveVideoRequestSlot\0"
    "\0vuri\0screen_index\0seat_index\0"
    "use_small_video\0HandleStopVideoRequestSlot\0"
    "HandleReceiveAudioRequestSlot\0uri\0"
    "enable\0HandleReceiveDocumentRequestSlot\0"
    "HandleOpenDocumentRequestSlot\0filename\0"
    "HandleScreenLayoutRequestSlot\0"
    "IDisplayController::ScreenLayout\0"
    "layout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TXDisplayController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   44,    2, 0x0a /* Public */,
       7,    1,   53,    2, 0x0a /* Public */,
       8,    2,   56,    2, 0x0a /* Public */,
      11,    2,   61,    2, 0x0a /* Public */,
      12,    2,   66,    2, 0x0a /* Public */,
      14,    2,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   13,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,    4,   16,

       0        // eod
};

void TXDisplayController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TXDisplayController *_t = static_cast<TXDisplayController *>(_o);
        switch (_id) {
        case 0: _t->HandleReceiveVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->HandleStopVideoRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->HandleReceiveAudioRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->HandleReceiveDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->HandleOpenDocumentRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->HandleScreenLayoutRequestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject TXDisplayController::staticMetaObject = {
    { &DisplayController::staticMetaObject, qt_meta_stringdata_TXDisplayController.data,
      qt_meta_data_TXDisplayController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TXDisplayController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TXDisplayController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TXDisplayController.stringdata))
        return static_cast<void*>(const_cast< TXDisplayController*>(this));
    return DisplayController::qt_metacast(_clname);
}

int TXDisplayController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DisplayController::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
