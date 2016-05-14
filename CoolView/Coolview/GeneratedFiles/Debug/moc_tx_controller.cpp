/****************************************************************************
** Meta object code from reading C++ file 'tx_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tx_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tx_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TXController_t {
    QByteArrayData data[33];
    char stringdata[625];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TXController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TXController_t qt_meta_stringdata_TXController = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TXController"
QT_MOC_LITERAL(1, 13, 29), // "HandleReceiveUDPRecReportSlot"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 11), // "RecStatItem"
QT_MOC_LITERAL(4, 56, 8), // "rec_stat"
QT_MOC_LITERAL(5, 65, 29), // "HandleReceiveUDPQoSReportSlot"
QT_MOC_LITERAL(6, 95, 11), // "RtpStatItem"
QT_MOC_LITERAL(7, 107, 8), // "rtp_stat"
QT_MOC_LITERAL(8, 116, 31), // "HandleTerminalStatusChangedSlot"
QT_MOC_LITERAL(9, 148, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(10, 169, 8), // "terminal"
QT_MOC_LITERAL(11, 178, 14), // "conference_uri"
QT_MOC_LITERAL(12, 193, 25), // "HandleTerminalReloginSlot"
QT_MOC_LITERAL(13, 219, 29), // "HandleSegmentMediaRequestSlot"
QT_MOC_LITERAL(14, 249, 11), // "uri_or_vuri"
QT_MOC_LITERAL(15, 261, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(16, 297, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(17, 330, 5), // "state"
QT_MOC_LITERAL(18, 336, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(19, 369, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(20, 408, 27), // "HandleRecvRecordControlSlot"
QT_MOC_LITERAL(21, 436, 16), // "RecordControlMsg"
QT_MOC_LITERAL(22, 453, 4), // "info"
QT_MOC_LITERAL(23, 458, 13), // "ControlRecord"
QT_MOC_LITERAL(24, 472, 2), // "op"
QT_MOC_LITERAL(25, 475, 6), // "params"
QT_MOC_LITERAL(26, 482, 15), // "GetRecordStatus"
QT_MOC_LITERAL(27, 498, 5), // "focus"
QT_MOC_LITERAL(28, 504, 3), // "uri"
QT_MOC_LITERAL(29, 508, 35), // "HandleStreamMediaReceiveStart..."
QT_MOC_LITERAL(30, 544, 11), // "remote_vuri"
QT_MOC_LITERAL(31, 556, 35), // "HandleStreamMediaReceiveStopp..."
QT_MOC_LITERAL(32, 592, 32) // "HandleAutoRecordTimerTimeoutSlot"

    },
    "TXController\0HandleReceiveUDPRecReportSlot\0"
    "\0RecStatItem\0rec_stat\0"
    "HandleReceiveUDPQoSReportSlot\0RtpStatItem\0"
    "rtp_stat\0HandleTerminalStatusChangedSlot\0"
    "ConstTerminalPointer\0terminal\0"
    "conference_uri\0HandleTerminalReloginSlot\0"
    "HandleSegmentMediaRequestSlot\0uri_or_vuri\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "HandleConferenceStateChangedSlot\0"
    "IConferenceController::ConferenceState\0"
    "HandleRecvRecordControlSlot\0"
    "RecordControlMsg\0info\0ControlRecord\0"
    "op\0params\0GetRecordStatus\0focus\0uri\0"
    "HandleStreamMediaReceiveStartedSlot\0"
    "remote_vuri\0HandleStreamMediaReceiveStoppedSlot\0"
    "HandleAutoRecordTimerTimeoutSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TXController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       5,    1,   82,    2, 0x0a /* Public */,
       8,    2,   85,    2, 0x0a /* Public */,
      12,    2,   90,    2, 0x0a /* Public */,
      13,    1,   95,    2, 0x0a /* Public */,
      15,    1,   98,    2, 0x0a /* Public */,
      18,    2,  101,    2, 0x0a /* Public */,
      20,    1,  106,    2, 0x0a /* Public */,
      23,    2,  109,    2, 0x0a /* Public */,
      26,    2,  114,    2, 0x0a /* Public */,
      29,    1,  119,    2, 0x09 /* Protected */,
      31,    1,  122,    2, 0x09 /* Protected */,
      32,    0,  125,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9, QMetaType::QString,   10,   11,
    QMetaType::Void, 0x80000000 | 9, QMetaType::QString,   10,   11,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 19,   11,   17,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,   24,   25,
    QMetaType::QByteArray, QMetaType::QString, QMetaType::QString,   27,   28,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void,

       0        // eod
};

void TXController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TXController *_t = static_cast<TXController *>(_o);
        switch (_id) {
        case 0: _t->HandleReceiveUDPRecReportSlot((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 1: _t->HandleReceiveUDPQoSReportSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 2: _t->HandleTerminalStatusChangedSlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->HandleTerminalReloginSlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->HandleSegmentMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 6: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 7: _t->HandleRecvRecordControlSlot((*reinterpret_cast< const RecordControlMsg(*)>(_a[1]))); break;
        case 8: { int _r = _t->ControlRecord((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { QByteArray _r = _t->GetRecordStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 10: _t->HandleStreamMediaReceiveStartedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->HandleStreamMediaReceiveStoppedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->HandleAutoRecordTimerTimeoutSlot(); break;
        default: ;
        }
    }
}

const QMetaObject TXController::staticMetaObject = {
    { &ITXController::staticMetaObject, qt_meta_stringdata_TXController.data,
      qt_meta_data_TXController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TXController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TXController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TXController.stringdata))
        return static_cast<void*>(const_cast< TXController*>(this));
    return ITXController::qt_metacast(_clname);
}

int TXController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITXController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
