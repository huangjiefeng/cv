/****************************************************************************
** Meta object code from reading C++ file 'CvPerfMonitorService.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CvPerfMonitorService.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CvPerfMonitorService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CvPerfMonitorService_t {
    QByteArrayData data[18];
    char stringdata[302];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CvPerfMonitorService_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CvPerfMonitorService_t qt_meta_stringdata_CvPerfMonitorService = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CvPerfMonitorService"
QT_MOC_LITERAL(1, 21, 32), // "cvMonitorCofMessageReceivedEvent"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 14), // "MonitorCofInfo"
QT_MOC_LITERAL(4, 70, 15), // "CreateTestMedia"
QT_MOC_LITERAL(5, 86, 8), // "media_id"
QT_MOC_LITERAL(6, 95, 12), // "input_garray"
QT_MOC_LITERAL(7, 108, 9), // "HelloInfo"
QT_MOC_LITERAL(8, 118, 9), // "processID"
QT_MOC_LITERAL(9, 128, 12), // "processImage"
QT_MOC_LITERAL(10, 141, 15), // "RegisterProcess"
QT_MOC_LITERAL(11, 157, 11), // "processInfo"
QT_MOC_LITERAL(12, 169, 17), // "UnregisterProcess"
QT_MOC_LITERAL(13, 187, 16), // "ReleaseTestMedia"
QT_MOC_LITERAL(14, 204, 20), // "RegisterMediaProcess"
QT_MOC_LITERAL(15, 225, 22), // "UnregisterMediaProcess"
QT_MOC_LITERAL(16, 248, 25), // "UpgradeMonitorCofInfoSlot"
QT_MOC_LITERAL(17, 274, 27) // "UpgradeMonitorAirServerSlot"

    },
    "CvPerfMonitorService\0"
    "cvMonitorCofMessageReceivedEvent\0\0"
    "MonitorCofInfo\0CreateTestMedia\0media_id\0"
    "input_garray\0HelloInfo\0processID\0"
    "processImage\0RegisterProcess\0processInfo\0"
    "UnregisterProcess\0ReleaseTestMedia\0"
    "RegisterMediaProcess\0UnregisterMediaProcess\0"
    "UpgradeMonitorCofInfoSlot\0"
    "UpgradeMonitorAirServerSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CvPerfMonitorService[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   67,    2, 0x0a /* Public */,
       7,    2,   72,    2, 0x0a /* Public */,
      10,    1,   77,    2, 0x0a /* Public */,
      12,    1,   80,    2, 0x0a /* Public */,
      13,    1,   83,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x0a /* Public */,
      15,    1,   89,    2, 0x0a /* Public */,
      16,    0,   92,    2, 0x0a /* Public */,
      17,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    8,    9,
    QMetaType::Void, QMetaType::QByteArray,   11,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,   11,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CvPerfMonitorService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CvPerfMonitorService *_t = static_cast<CvPerfMonitorService *>(_o);
        switch (_id) {
        case 0: _t->cvMonitorCofMessageReceivedEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->CreateTestMedia((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->HelloInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->RegisterProcess((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->UnregisterProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ReleaseTestMedia((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->RegisterMediaProcess((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->UnregisterMediaProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->UpgradeMonitorCofInfoSlot(); break;
        case 9: _t->UpgradeMonitorAirServerSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CvPerfMonitorService::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CvPerfMonitorService::cvMonitorCofMessageReceivedEvent)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CvPerfMonitorService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CvPerfMonitorService.data,
      qt_meta_data_CvPerfMonitorService,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CvPerfMonitorService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CvPerfMonitorService::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CvPerfMonitorService.stringdata))
        return static_cast<void*>(const_cast< CvPerfMonitorService*>(this));
    return QObject::qt_metacast(_clname);
}

int CvPerfMonitorService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CvPerfMonitorService::cvMonitorCofMessageReceivedEvent(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
