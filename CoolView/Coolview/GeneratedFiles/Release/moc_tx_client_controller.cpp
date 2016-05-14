/****************************************************************************
** Meta object code from reading C++ file 'tx_client_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tx_client_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tx_client_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TXClientController_t {
    QByteArrayData data[18];
    char stringdata[388];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TXClientController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TXClientController_t qt_meta_stringdata_TXClientController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TXClientController"
QT_MOC_LITERAL(1, 19, 37), // "HandleSendMediaStateChangedNo..."
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 8), // "media_id"
QT_MOC_LITERAL(4, 67, 4), // "type"
QT_MOC_LITERAL(5, 72, 40), // "HandleSegmentLocalRecordVideo..."
QT_MOC_LITERAL(6, 113, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(7, 146, 14), // "conference_uri"
QT_MOC_LITERAL(8, 161, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(9, 200, 5), // "state"
QT_MOC_LITERAL(10, 206, 31), // "HandleOneVideoRecordRequestSlot"
QT_MOC_LITERAL(11, 238, 7), // "user_id"
QT_MOC_LITERAL(12, 246, 33), // "HandleWholeVideoRecordRequest..."
QT_MOC_LITERAL(13, 280, 5), // "param"
QT_MOC_LITERAL(14, 286, 36), // "HandleRecvRecordControlAckNot..."
QT_MOC_LITERAL(15, 323, 19), // "RecordControlAckMsg"
QT_MOC_LITERAL(16, 343, 4), // "info"
QT_MOC_LITERAL(17, 348, 39) // "HandleGetTxRecordStatusTimerT..."

    },
    "TXClientController\0"
    "HandleSendMediaStateChangedNotifySlot\0"
    "\0media_id\0type\0HandleSegmentLocalRecordVideoRequestSlot\0"
    "HandleConferenceStateChangedSlot\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleOneVideoRecordRequestSlot\0"
    "user_id\0HandleWholeVideoRecordRequestSlot\0"
    "param\0HandleRecvRecordControlAckNotifySlot\0"
    "RecordControlAckMsg\0info\0"
    "HandleGetTxRecordStatusTimerTimeoutSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TXClientController[] = {

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
       1,    2,   49,    2, 0x0a /* Public */,
       5,    0,   54,    2, 0x0a /* Public */,
       6,    2,   55,    2, 0x0a /* Public */,
      10,    2,   60,    2, 0x0a /* Public */,
      12,    3,   65,    2, 0x0a /* Public */,
      14,    1,   72,    2, 0x0a /* Public */,
      17,    0,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   11,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   11,    4,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,

       0        // eod
};

void TXClientController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TXClientController *_t = static_cast<TXClientController *>(_o);
        switch (_id) {
        case 0: _t->HandleSendMediaStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->HandleSegmentLocalRecordVideoRequestSlot(); break;
        case 2: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 3: _t->HandleOneVideoRecordRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->HandleWholeVideoRecordRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 5: _t->HandleRecvRecordControlAckNotifySlot((*reinterpret_cast< const RecordControlAckMsg(*)>(_a[1]))); break;
        case 6: _t->HandleGetTxRecordStatusTimerTimeoutSlot(); break;
        default: ;
        }
    }
}

const QMetaObject TXClientController::staticMetaObject = {
    { &ITXClientController::staticMetaObject, qt_meta_stringdata_TXClientController.data,
      qt_meta_data_TXClientController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TXClientController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TXClientController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TXClientController.stringdata))
        return static_cast<void*>(const_cast< TXClientController*>(this));
    return ITXClientController::qt_metacast(_clname);
}

int TXClientController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITXClientController::qt_metacall(_c, _id, _a);
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
