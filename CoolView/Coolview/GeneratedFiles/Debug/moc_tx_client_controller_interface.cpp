/****************************************************************************
** Meta object code from reading C++ file 'tx_client_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tx_client_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tx_client_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ITXClientController_t {
    QByteArrayData data[22];
    char stringdata[442];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ITXClientController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ITXClientController_t qt_meta_stringdata_ITXClientController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ITXClientController"
QT_MOC_LITERAL(1, 20, 30), // "SendRecordControlRequestSignal"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 18), // "RecordControlParam"
QT_MOC_LITERAL(4, 71, 5), // "param"
QT_MOC_LITERAL(5, 77, 25), // "ReceiveUDPRecReportSignal"
QT_MOC_LITERAL(6, 103, 11), // "RecStatItem"
QT_MOC_LITERAL(7, 115, 4), // "item"
QT_MOC_LITERAL(8, 120, 37), // "HandleSendMediaStateChangedNo..."
QT_MOC_LITERAL(9, 158, 8), // "media_id"
QT_MOC_LITERAL(10, 167, 4), // "type"
QT_MOC_LITERAL(11, 172, 40), // "HandleSegmentLocalRecordVideo..."
QT_MOC_LITERAL(12, 213, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(13, 246, 14), // "conference_uri"
QT_MOC_LITERAL(14, 261, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(15, 300, 5), // "state"
QT_MOC_LITERAL(16, 306, 31), // "HandleOneVideoRecordRequestSlot"
QT_MOC_LITERAL(17, 338, 7), // "user_id"
QT_MOC_LITERAL(18, 346, 33), // "HandleWholeVideoRecordRequest..."
QT_MOC_LITERAL(19, 380, 36), // "HandleRecvRecordControlAckNot..."
QT_MOC_LITERAL(20, 417, 19), // "RecordControlAckMsg"
QT_MOC_LITERAL(21, 437, 4) // "info"

    },
    "ITXClientController\0SendRecordControlRequestSignal\0"
    "\0RecordControlParam\0param\0"
    "ReceiveUDPRecReportSignal\0RecStatItem\0"
    "item\0HandleSendMediaStateChangedNotifySlot\0"
    "media_id\0type\0HandleSegmentLocalRecordVideoRequestSlot\0"
    "HandleConferenceStateChangedSlot\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleOneVideoRecordRequestSlot\0"
    "user_id\0HandleWholeVideoRecordRequestSlot\0"
    "HandleRecvRecordControlAckNotifySlot\0"
    "RecordControlAckMsg\0info"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ITXClientController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   60,    2, 0x0a /* Public */,
      11,    0,   65,    2, 0x0a /* Public */,
      12,    2,   66,    2, 0x0a /* Public */,
      16,    2,   71,    2, 0x0a /* Public */,
      18,    3,   76,    2, 0x0a /* Public */,
      19,    1,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 14,   13,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   17,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QByteArray,   17,   10,    4,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void ITXClientController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ITXClientController *_t = static_cast<ITXClientController *>(_o);
        switch (_id) {
        case 0: _t->SendRecordControlRequestSignal((*reinterpret_cast< const RecordControlParam(*)>(_a[1]))); break;
        case 1: _t->ReceiveUDPRecReportSignal((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 2: _t->HandleSendMediaStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->HandleSegmentLocalRecordVideoRequestSlot(); break;
        case 4: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 5: _t->HandleOneVideoRecordRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->HandleWholeVideoRecordRequestSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 7: _t->HandleRecvRecordControlAckNotifySlot((*reinterpret_cast< const RecordControlAckMsg(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ITXClientController::*_t)(const RecordControlParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXClientController::SendRecordControlRequestSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ITXClientController::*_t)(const RecStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ITXClientController::ReceiveUDPRecReportSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ITXClientController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ITXClientController.data,
      qt_meta_data_ITXClientController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ITXClientController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ITXClientController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ITXClientController.stringdata))
        return static_cast<void*>(const_cast< ITXClientController*>(this));
    return QObject::qt_metacast(_clname);
}

int ITXClientController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ITXClientController::SendRecordControlRequestSignal(const RecordControlParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ITXClientController::ReceiveUDPRecReportSignal(const RecStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
