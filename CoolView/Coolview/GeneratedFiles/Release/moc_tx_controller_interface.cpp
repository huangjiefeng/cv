/****************************************************************************
** Meta object code from reading C++ file 'tx_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tx_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tx_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ITXController_t {
    QByteArrayData data[33];
    char stringdata[685];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ITXController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ITXController_t qt_meta_stringdata_ITXController = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ITXController"
QT_MOC_LITERAL(1, 14, 30), // "StartReceiveVideoRequestSignal"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 4), // "vuri"
QT_MOC_LITERAL(4, 51, 29), // "StopReceiveVideoRequestSignal"
QT_MOC_LITERAL(5, 81, 27), // "NotifyRecordRequesterSignal"
QT_MOC_LITERAL(6, 109, 13), // "requester_uri"
QT_MOC_LITERAL(7, 123, 27), // "NotifyRecStatReceivedSignal"
QT_MOC_LITERAL(8, 151, 11), // "RecStatItem"
QT_MOC_LITERAL(9, 163, 9), // "rec_state"
QT_MOC_LITERAL(10, 173, 33), // "SendRecordControlRequestAckSi..."
QT_MOC_LITERAL(11, 207, 21), // "RecordControlAckParam"
QT_MOC_LITERAL(12, 229, 5), // "param"
QT_MOC_LITERAL(13, 235, 30), // "NotifyAllRecordJobFinishSignal"
QT_MOC_LITERAL(14, 266, 29), // "HandleReceiveUDPRecReportSlot"
QT_MOC_LITERAL(15, 296, 7), // "recstat"
QT_MOC_LITERAL(16, 304, 29), // "HandleReceiveUDPQoSReportSlot"
QT_MOC_LITERAL(17, 334, 11), // "RtpStatItem"
QT_MOC_LITERAL(18, 346, 8), // "rtp_stat"
QT_MOC_LITERAL(19, 355, 31), // "HandleTerminalStatusChangedSlot"
QT_MOC_LITERAL(20, 387, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(21, 408, 8), // "terminal"
QT_MOC_LITERAL(22, 417, 14), // "conference_uri"
QT_MOC_LITERAL(23, 432, 25), // "HandleTerminalReloginSlot"
QT_MOC_LITERAL(24, 458, 29), // "HandleSegmentMediaRequestSlot"
QT_MOC_LITERAL(25, 488, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(26, 524, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(27, 557, 5), // "state"
QT_MOC_LITERAL(28, 563, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(29, 596, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(30, 635, 27), // "HandleRecvRecordControlSlot"
QT_MOC_LITERAL(31, 663, 16), // "RecordControlMsg"
QT_MOC_LITERAL(32, 680, 4) // "info"

    },
    "ITXController\0StartReceiveVideoRequestSignal\0"
    "\0vuri\0StopReceiveVideoRequestSignal\0"
    "NotifyRecordRequesterSignal\0requester_uri\0"
    "NotifyRecStatReceivedSignal\0RecStatItem\0"
    "rec_state\0SendRecordControlRequestAckSignal\0"
    "RecordControlAckParam\0param\0"
    "NotifyAllRecordJobFinishSignal\0"
    "HandleReceiveUDPRecReportSlot\0recstat\0"
    "HandleReceiveUDPQoSReportSlot\0RtpStatItem\0"
    "rtp_stat\0HandleTerminalStatusChangedSlot\0"
    "ConstTerminalPointer\0terminal\0"
    "conference_uri\0HandleTerminalReloginSlot\0"
    "HandleSegmentMediaRequestSlot\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "HandleConferenceStateChangedSlot\0"
    "IConferenceController::ConferenceState\0"
    "HandleRecvRecordControlSlot\0"
    "RecordControlMsg\0info"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ITXController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       5,    2,   90,    2, 0x06 /* Public */,
       7,    1,   95,    2, 0x06 /* Public */,
      10,    1,   98,    2, 0x06 /* Public */,
      13,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,  102,    2, 0x0a /* Public */,
      16,    1,  105,    2, 0x0a /* Public */,
      19,    2,  108,    2, 0x0a /* Public */,
      23,    2,  113,    2, 0x0a /* Public */,
      24,    1,  118,    2, 0x0a /* Public */,
      25,    1,  121,    2, 0x0a /* Public */,
      28,    2,  124,    2, 0x0a /* Public */,
      30,    1,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20, QMetaType::QString,   21,   22,
    QMetaType::Void, 0x80000000 | 20, QMetaType::QString,   21,   22,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 29,   22,   27,
    QMetaType::Void, 0x80000000 | 31,   32,

       0        // eod
};

void ITXController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ITXController *_t = static_cast<ITXController *>(_o);
        switch (_id) {
        case 0: _t->StartReceiveVideoRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->StopReceiveVideoRequestSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->NotifyRecordRequesterSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->NotifyRecStatReceivedSignal((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 4: _t->SendRecordControlRequestAckSignal((*reinterpret_cast< const RecordControlAckParam(*)>(_a[1]))); break;
        case 5: _t->NotifyAllRecordJobFinishSignal(); break;
        case 6: _t->HandleReceiveUDPRecReportSlot((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 7: _t->HandleReceiveUDPQoSReportSlot((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 8: _t->HandleTerminalStatusChangedSlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->HandleTerminalReloginSlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->HandleSegmentMediaRequestSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 12: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 13: _t->HandleRecvRecordControlSlot((*reinterpret_cast< const RecordControlMsg(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ITXController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::StartReceiveVideoRequestSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ITXController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::StopReceiveVideoRequestSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ITXController::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::NotifyRecordRequesterSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (ITXController::*_t)(const RecStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::NotifyRecStatReceivedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (ITXController::*_t)(const RecordControlAckParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::SendRecordControlRequestAckSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (ITXController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXController::NotifyAllRecordJobFinishSignal)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject ITXController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ITXController.data,
      qt_meta_data_ITXController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ITXController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ITXController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ITXController.stringdata))
        return static_cast<void*>(const_cast< ITXController*>(this));
    return QObject::qt_metacast(_clname);
}

int ITXController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ITXController::StartReceiveVideoRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ITXController::StopReceiveVideoRequestSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ITXController::NotifyRecordRequesterSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ITXController::NotifyRecStatReceivedSignal(const RecStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ITXController::SendRecordControlRequestAckSignal(const RecordControlAckParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ITXController::NotifyAllRecordJobFinishSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
