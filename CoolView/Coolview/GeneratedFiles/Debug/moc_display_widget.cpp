/****************************************************************************
** Meta object code from reading C++ file 'display_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../display_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'display_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DisplayWidget_t {
    QByteArrayData data[20];
    char stringdata[364];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DisplayWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DisplayWidget_t qt_meta_stringdata_DisplayWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DisplayWidget"
QT_MOC_LITERAL(1, 14, 28), // "HandleReceiveVideoNotifySlot"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 4), // "vuri"
QT_MOC_LITERAL(4, 49, 12), // "screen_index"
QT_MOC_LITERAL(5, 62, 10), // "seat_index"
QT_MOC_LITERAL(6, 73, 15), // "use_small_video"
QT_MOC_LITERAL(7, 89, 25), // "HandleStopVideoNotifySlot"
QT_MOC_LITERAL(8, 115, 31), // "HandleReceiveDocumentNotifySlot"
QT_MOC_LITERAL(9, 147, 3), // "uri"
QT_MOC_LITERAL(10, 151, 28), // "HandleStopDocumentNotifySlot"
QT_MOC_LITERAL(11, 180, 31), // "HandleShowRTCPMessageNotifySlot"
QT_MOC_LITERAL(12, 212, 4), // "show"
QT_MOC_LITERAL(13, 217, 29), // "HandleScreenLayoutChangedSlot"
QT_MOC_LITERAL(14, 247, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(15, 280, 6), // "layout"
QT_MOC_LITERAL(16, 287, 31), // "HandleTerminalReceiveNotifySlot"
QT_MOC_LITERAL(17, 319, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(18, 340, 8), // "terminal"
QT_MOC_LITERAL(19, 349, 14) // "conference_uri"

    },
    "DisplayWidget\0HandleReceiveVideoNotifySlot\0"
    "\0vuri\0screen_index\0seat_index\0"
    "use_small_video\0HandleStopVideoNotifySlot\0"
    "HandleReceiveDocumentNotifySlot\0uri\0"
    "HandleStopDocumentNotifySlot\0"
    "HandleShowRTCPMessageNotifySlot\0show\0"
    "HandleScreenLayoutChangedSlot\0"
    "IDisplayController::ScreenLayout\0"
    "layout\0HandleTerminalReceiveNotifySlot\0"
    "ConstTerminalPointer\0terminal\0"
    "conference_uri"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DisplayWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   49,    2, 0x0a /* Public */,
       7,    3,   58,    2, 0x0a /* Public */,
       8,    2,   65,    2, 0x0a /* Public */,
      10,    2,   70,    2, 0x0a /* Public */,
      11,    2,   75,    2, 0x0a /* Public */,
      13,    2,   80,    2, 0x0a /* Public */,
      16,    2,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,    4,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   12,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 14,    4,   15,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QString,   18,   19,

       0        // eod
};

void DisplayWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DisplayWidget *_t = static_cast<DisplayWidget *>(_o);
        switch (_id) {
        case 0: _t->HandleReceiveVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->HandleStopVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->HandleReceiveDocumentNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->HandleStopDocumentNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->HandleShowRTCPMessageNotifySlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->HandleScreenLayoutChangedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 6: _t->HandleTerminalReceiveNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject DisplayWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DisplayWidget.data,
      qt_meta_data_DisplayWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DisplayWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DisplayWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayWidget.stringdata))
        return static_cast<void*>(const_cast< DisplayWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DisplayWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
