/****************************************************************************
** Meta object code from reading C++ file 'CvPerfMonitorIf.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/dbus/performance/monitor/client/CvPerfMonitorIf.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvPerfMonitorIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvPerfMonitorIf_t {
    QByteArrayData data[17];
    char stringdata[263];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvPerfMonitorIf_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvPerfMonitorIf_t qt_meta_stringdata_CvPerfMonitorIf = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CvPerfMonitorIf"
QT_MOC_LITERAL(1, 16, 32), // "cvMonitorCofMessageReceivedEvent"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 14), // "MonitorCofInfo"
QT_MOC_LITERAL(4, 65, 15), // "CreateTestMedia"
QT_MOC_LITERAL(5, 81, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(6, 101, 8), // "media_id"
QT_MOC_LITERAL(7, 110, 12), // "input_garray"
QT_MOC_LITERAL(8, 123, 9), // "HelloInfo"
QT_MOC_LITERAL(9, 133, 9), // "processID"
QT_MOC_LITERAL(10, 143, 12), // "processImage"
QT_MOC_LITERAL(11, 156, 20), // "RegisterMediaProcess"
QT_MOC_LITERAL(12, 177, 11), // "processInfo"
QT_MOC_LITERAL(13, 189, 15), // "RegisterProcess"
QT_MOC_LITERAL(14, 205, 16), // "ReleaseTestMedia"
QT_MOC_LITERAL(15, 222, 22), // "UnregisterMediaProcess"
QT_MOC_LITERAL(16, 245, 17) // "UnregisterProcess"

    },
    "CvPerfMonitorIf\0cvMonitorCofMessageReceivedEvent\0"
    "\0MonitorCofInfo\0CreateTestMedia\0"
    "QDBusPendingReply<>\0media_id\0input_garray\0"
    "HelloInfo\0processID\0processImage\0"
    "RegisterMediaProcess\0processInfo\0"
    "RegisterProcess\0ReleaseTestMedia\0"
    "UnregisterMediaProcess\0UnregisterProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvPerfMonitorIf[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   57,    2, 0x0a /* Public */,
       8,    2,   62,    2, 0x0a /* Public */,
      11,    1,   67,    2, 0x0a /* Public */,
      13,    1,   70,    2, 0x0a /* Public */,
      14,    1,   73,    2, 0x0a /* Public */,
      15,    1,   76,    2, 0x0a /* Public */,
      16,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    0x80000000 | 5, QMetaType::QString, QMetaType::QByteArray,    6,    7,
    0x80000000 | 5, QMetaType::Int, QMetaType::QByteArray,    9,   10,
    0x80000000 | 5, QMetaType::QByteArray,   12,
    0x80000000 | 5, QMetaType::QByteArray,   12,
    0x80000000 | 5, QMetaType::QString,    6,
    0x80000000 | 5, QMetaType::Int,    9,
    0x80000000 | 5, QMetaType::Int,    9,

       0        // eod
};

void CvPerfMonitorIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvPerfMonitorIf *_t = static_cast<CvPerfMonitorIf *>(_o);
        switch (_id) {
        case 0: _t->cvMonitorCofMessageReceivedEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: { QDBusPendingReply<> _r = _t->CreateTestMedia((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<> _r = _t->HelloInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->RegisterMediaProcess((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<> _r = _t->RegisterProcess((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->ReleaseTestMedia((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->UnregisterMediaProcess((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->UnregisterProcess((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvPerfMonitorIf::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvPerfMonitorIf::cvMonitorCofMessageReceivedEvent)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvPerfMonitorIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_CvPerfMonitorIf.data,
      qt_meta_data_CvPerfMonitorIf,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvPerfMonitorIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvPerfMonitorIf::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvPerfMonitorIf.stringdata))
        return static_cast<void*>(const_cast< CvPerfMonitorIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int CvPerfMonitorIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void CvPerfMonitorIf::cvMonitorCofMessageReceivedEvent(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
