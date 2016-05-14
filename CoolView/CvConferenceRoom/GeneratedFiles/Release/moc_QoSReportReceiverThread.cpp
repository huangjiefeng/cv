/****************************************************************************
** Meta object code from reading C++ file 'QoSReportReceiverThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QoSReportReceiverThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QoSReportReceiverThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QoSReportReceiverThread_t {
    QByteArrayData data[9];
    char stringdata[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QoSReportReceiverThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QoSReportReceiverThread_t qt_meta_stringdata_QoSReportReceiverThread = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QoSReportReceiverThread"
QT_MOC_LITERAL(1, 24, 22), // "SendUDPQoSReportSignal"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 11), // "RtpStatItem"
QT_MOC_LITERAL(4, 60, 4), // "item"
QT_MOC_LITERAL(5, 65, 22), // "SendUDPRecReportSignal"
QT_MOC_LITERAL(6, 88, 11), // "RecStatItem"
QT_MOC_LITERAL(7, 100, 11), // "StartSignal"
QT_MOC_LITERAL(8, 112, 3) // "run"

    },
    "QoSReportReceiverThread\0SendUDPQoSReportSignal\0"
    "\0RtpStatItem\0item\0SendUDPRecReportSignal\0"
    "RecStatItem\0StartSignal\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QoSReportReceiverThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       7,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   41,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QoSReportReceiverThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QoSReportReceiverThread *_t = static_cast<QoSReportReceiverThread *>(_o);
        switch (_id) {
        case 0: _t->SendUDPQoSReportSignal((*reinterpret_cast< const RtpStatItem(*)>(_a[1]))); break;
        case 1: _t->SendUDPRecReportSignal((*reinterpret_cast< const RecStatItem(*)>(_a[1]))); break;
        case 2: _t->StartSignal(); break;
        case 3: _t->run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QoSReportReceiverThread::*_t)(const RtpStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QoSReportReceiverThread::SendUDPQoSReportSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (QoSReportReceiverThread::*_t)(const RecStatItem & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QoSReportReceiverThread::SendUDPRecReportSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (QoSReportReceiverThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QoSReportReceiverThread::StartSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject QoSReportReceiverThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QoSReportReceiverThread.data,
      qt_meta_data_QoSReportReceiverThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QoSReportReceiverThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QoSReportReceiverThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QoSReportReceiverThread.stringdata))
        return static_cast<void*>(const_cast< QoSReportReceiverThread*>(this));
    return QObject::qt_metacast(_clname);
}

int QoSReportReceiverThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void QoSReportReceiverThread::SendUDPQoSReportSignal(const RtpStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QoSReportReceiverThread::SendUDPRecReportSignal(const RecStatItem & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QoSReportReceiverThread::StartSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
