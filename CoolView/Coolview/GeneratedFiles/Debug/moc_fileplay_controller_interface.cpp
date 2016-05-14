/****************************************************************************
** Meta object code from reading C++ file 'fileplay_controller_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fileplay_controller_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileplay_controller_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IFilePlayController_t {
    QByteArrayData data[22];
    char stringdata[391];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IFilePlayController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IFilePlayController_t qt_meta_stringdata_IFilePlayController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "IFilePlayController"
QT_MOC_LITERAL(1, 20, 24), // "NotifyPlayProgressSignal"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 3), // "pos"
QT_MOC_LITERAL(4, 50, 8), // "duration"
QT_MOC_LITERAL(5, 59, 21), // "NotifyFileErrorSignal"
QT_MOC_LITERAL(6, 81, 3), // "msg"
QT_MOC_LITERAL(7, 85, 35), // "NotifyVirtualRecvTerminalList..."
QT_MOC_LITERAL(8, 121, 12), // "TerminalList"
QT_MOC_LITERAL(9, 134, 21), // "HandleAddPlayListSlot"
QT_MOC_LITERAL(10, 156, 24), // "HandleClearPlayListsSlot"
QT_MOC_LITERAL(11, 181, 14), // "HandlePlaySlot"
QT_MOC_LITERAL(12, 196, 15), // "HandlePauseSlot"
QT_MOC_LITERAL(13, 212, 14), // "HandleStopSlot"
QT_MOC_LITERAL(14, 227, 14), // "HandleSeekSlot"
QT_MOC_LITERAL(15, 242, 3), // "sec"
QT_MOC_LITERAL(16, 246, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(17, 279, 14), // "conference_uri"
QT_MOC_LITERAL(18, 294, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(19, 333, 5), // "state"
QT_MOC_LITERAL(20, 339, 34), // "HandleReceiveUDPFilePlayRepor..."
QT_MOC_LITERAL(21, 374, 16) // "FilePlayStatItem"

    },
    "IFilePlayController\0NotifyPlayProgressSignal\0"
    "\0pos\0duration\0NotifyFileErrorSignal\0"
    "msg\0NotifyVirtualRecvTerminalListSignal\0"
    "TerminalList\0HandleAddPlayListSlot\0"
    "HandleClearPlayListsSlot\0HandlePlaySlot\0"
    "HandlePauseSlot\0HandleStopSlot\0"
    "HandleSeekSlot\0sec\0HandleConferenceStateChangedSlot\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleReceiveUDPFilePlayReportSlot\0"
    "FilePlayStatItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IFilePlayController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       5,    1,   74,    2, 0x06 /* Public */,
       7,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   80,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      13,    0,   86,    2, 0x0a /* Public */,
      14,    1,   87,    2, 0x0a /* Public */,
      16,    2,   90,    2, 0x0a /* Public */,
      20,    1,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::ULong, QMetaType::ULong,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 8,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULong,   15,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 18,   17,   19,
    QMetaType::Void, 0x80000000 | 21,    2,

       0        // eod
};

void IFilePlayController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IFilePlayController *_t = static_cast<IFilePlayController *>(_o);
        switch (_id) {
        case 0: _t->NotifyPlayProgressSignal((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2]))); break;
        case 1: _t->NotifyFileErrorSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->NotifyVirtualRecvTerminalListSignal((*reinterpret_cast< const TerminalList(*)>(_a[1]))); break;
        case 3: _t->HandleAddPlayListSlot((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->HandleClearPlayListsSlot(); break;
        case 5: _t->HandlePlaySlot(); break;
        case 6: _t->HandlePauseSlot(); break;
        case 7: _t->HandleStopSlot(); break;
        case 8: _t->HandleSeekSlot((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 9: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 10: _t->HandleReceiveUDPFilePlayReportSlot((*reinterpret_cast< const FilePlayStatItem(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IFilePlayController::*_t)(unsigned long , unsigned long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IFilePlayController::NotifyPlayProgressSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (IFilePlayController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IFilePlayController::NotifyFileErrorSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (IFilePlayController::*_t)(const TerminalList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IFilePlayController::NotifyVirtualRecvTerminalListSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject IFilePlayController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IFilePlayController.data,
      qt_meta_data_IFilePlayController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IFilePlayController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IFilePlayController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IFilePlayController.stringdata))
        return static_cast<void*>(const_cast< IFilePlayController*>(this));
    return QObject::qt_metacast(_clname);
}

int IFilePlayController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void IFilePlayController::NotifyPlayProgressSignal(unsigned long _t1, unsigned long _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IFilePlayController::NotifyFileErrorSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IFilePlayController::NotifyVirtualRecvTerminalListSignal(const TerminalList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
