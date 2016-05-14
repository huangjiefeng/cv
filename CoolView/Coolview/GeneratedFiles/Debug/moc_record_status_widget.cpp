/****************************************************************************
** Meta object code from reading C++ file 'record_status_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../record_status_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'record_status_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RecordStatusWidget_t {
    QByteArrayData data[23];
    char stringdata[503];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecordStatusWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecordStatusWidget_t qt_meta_stringdata_RecordStatusWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RecordStatusWidget"
QT_MOC_LITERAL(1, 19, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(2, 58, 0), // ""
QT_MOC_LITERAL(3, 59, 14), // "conference_uri"
QT_MOC_LITERAL(4, 74, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(5, 113, 5), // "state"
QT_MOC_LITERAL(6, 119, 32), // "HandleTerminalReceivedNotifySlot"
QT_MOC_LITERAL(7, 152, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(8, 173, 8), // "terminal"
QT_MOC_LITERAL(9, 182, 30), // "HandleTerminalUpdateNotifySlot"
QT_MOC_LITERAL(10, 213, 31), // "HandleNotifyRTPStatReceivedSlot"
QT_MOC_LITERAL(11, 245, 11), // "RtpStatItem"
QT_MOC_LITERAL(12, 257, 7), // "rtpstat"
QT_MOC_LITERAL(13, 265, 31), // "HandleNotifyRecStatReceivedSlot"
QT_MOC_LITERAL(14, 297, 11), // "RecStatItem"
QT_MOC_LITERAL(15, 309, 7), // "recstat"
QT_MOC_LITERAL(16, 317, 41), // "HandleNotifyStreamMediaReceiv..."
QT_MOC_LITERAL(17, 359, 4), // "vuri"
QT_MOC_LITERAL(18, 364, 41), // "HandleNotifyStreamMediaReceiv..."
QT_MOC_LITERAL(19, 406, 39), // "HandleNotifyStreamMediaReceiv..."
QT_MOC_LITERAL(20, 446, 10), // "permission"
QT_MOC_LITERAL(21, 457, 31), // "HandleNotifyRecordRequesterSlot"
QT_MOC_LITERAL(22, 489, 13) // "requester_uri"

    },
    "RecordStatusWidget\0"
    "HandleConferenceStateChangedNotifySlot\0"
    "\0conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleTerminalReceivedNotifySlot\0"
    "ConstTerminalPointer\0terminal\0"
    "HandleTerminalUpdateNotifySlot\0"
    "HandleNotifyRTPStatReceivedSlot\0"
    "RtpStatItem\0rtpstat\0HandleNotifyRecStatReceivedSlot\0"
    "RecStatItem\0recstat\0"
    "HandleNotifyStreamMediaReceiveStartedSlot\0"
    "vuri\0HandleNotifyStreamMediaReceiveStoppedSlot\0"
    "HandleNotifyStreamMediaReceiveReplySlot\0"
    "permission\0HandleNotifyRecordRequesterSlot\0"
    "requester_uri"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecordStatusWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x08 /* Private */,
       6,    2,   64,    2, 0x08 /* Private */,
       9,    2,   69,    2, 0x08 /* Private */,
      10,    1,   74,    2, 0x08 /* Private */,
      13,    1,   77,    2, 0x08 /* Private */,
      16,    1,   80,    2, 0x08 /* Private */,
      18,    1,   83,    2, 0x08 /* Private */,
      19,    2,   86,    2, 0x08 /* Private */,
      21,    2,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QString,    8,    3,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QString,    8,    3,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   17,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   17,   22,

       0        // eod
};

void RecordStatusWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecordStatusWidget *_t = static_cast<RecordStatusWidget *>(_o);
        switch (_id) {
        case 0: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 1: _t->HandleTerminalReceivedNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->HandleTerminalUpdateNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->HandleNotifyRTPStatReceivedSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 4: _t->HandleNotifyRecStatReceivedSlot((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 5: _t->HandleNotifyStreamMediaReceiveStartedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->HandleNotifyStreamMediaReceiveStoppedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->HandleNotifyStreamMediaReceiveReplySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->HandleNotifyRecordRequesterSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject RecordStatusWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RecordStatusWidget.data,
      qt_meta_data_RecordStatusWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RecordStatusWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecordStatusWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RecordStatusWidget.stringdata))
        return static_cast<void*>(const_cast< RecordStatusWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int RecordStatusWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
