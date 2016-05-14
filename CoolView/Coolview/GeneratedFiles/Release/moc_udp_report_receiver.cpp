/****************************************************************************
** Meta object code from reading C++ file 'udp_report_receiver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../udp_report_receiver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udp_report_receiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UDPReportReceiver_t {
    QByteArrayData data[11];
    char stringdata[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UDPReportReceiver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UDPReportReceiver_t qt_meta_stringdata_UDPReportReceiver = {
    {
QT_MOC_LITERAL(0, 0, 17), // "UDPReportReceiver"
QT_MOC_LITERAL(1, 18, 25), // "ReceiveUDPQoSReportSignal"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 11), // "RtpStatItem"
QT_MOC_LITERAL(4, 57, 4), // "item"
QT_MOC_LITERAL(5, 62, 25), // "ReceiveUDPRecReportSignal"
QT_MOC_LITERAL(6, 88, 11), // "RecStatItem"
QT_MOC_LITERAL(7, 100, 30), // "ReceiveUDPFilePlayReportSignal"
QT_MOC_LITERAL(8, 131, 16), // "FilePlayStatItem"
QT_MOC_LITERAL(9, 148, 9), // "StartSlot"
QT_MOC_LITERAL(10, 158, 8) // "StopSlot"

    },
    "UDPReportReceiver\0ReceiveUDPQoSReportSignal\0"
    "\0RtpStatItem\0item\0ReceiveUDPRecReportSignal\0"
    "RecStatItem\0ReceiveUDPFilePlayReportSignal\0"
    "FilePlayStatItem\0StartSlot\0StopSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UDPReportReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       7,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   48,    2, 0x0a /* Public */,
      10,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UDPReportReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UDPReportReceiver *_t = static_cast<UDPReportReceiver *>(_o);
        switch (_id) {
        case 0: _t->ReceiveUDPQoSReportSignal((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 1: _t->ReceiveUDPRecReportSignal((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 2: _t->ReceiveUDPFilePlayReportSignal((*reinterpret_cast< const FilePlayStatItem(*)>(_a[1]))); break;
        case 3: _t->StartSlot(); break;
        case 4: _t->StopSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UDPReportReceiver::*_t)(const RtpStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDPReportReceiver::ReceiveUDPQoSReportSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (UDPReportReceiver::*_t)(const RecStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDPReportReceiver::ReceiveUDPRecReportSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (UDPReportReceiver::*_t)(const FilePlayStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDPReportReceiver::ReceiveUDPFilePlayReportSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject UDPReportReceiver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UDPReportReceiver.data,
      qt_meta_data_UDPReportReceiver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UDPReportReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UDPReportReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UDPReportReceiver.stringdata))
        return static_cast<void*>(const_cast< UDPReportReceiver*>(this));
    return QObject::qt_metacast(_clname);
}

int UDPReportReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void UDPReportReceiver::ReceiveUDPQoSReportSignal(const RtpStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UDPReportReceiver::ReceiveUDPRecReportSignal(const RecStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UDPReportReceiver::ReceiveUDPFilePlayReportSignal(const FilePlayStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
