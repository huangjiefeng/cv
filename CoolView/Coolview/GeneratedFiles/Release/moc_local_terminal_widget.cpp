/****************************************************************************
** Meta object code from reading C++ file 'local_terminal_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../local_terminal_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'local_terminal_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LocalTerminalWidget_t {
    QByteArrayData data[28];
    char stringdata[533];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocalTerminalWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocalTerminalWidget_t qt_meta_stringdata_LocalTerminalWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "LocalTerminalWidget"
QT_MOC_LITERAL(1, 20, 15), // "StopVideoSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 4), // "vuri"
QT_MOC_LITERAL(4, 42, 18), // "StopDocumentSignal"
QT_MOC_LITERAL(5, 61, 3), // "uri"
QT_MOC_LITERAL(6, 65, 18), // "ConfigCameraSignal"
QT_MOC_LITERAL(7, 84, 5), // "index"
QT_MOC_LITERAL(8, 90, 27), // "DisplayTerminalConfigSignal"
QT_MOC_LITERAL(9, 118, 18), // "TerminalConfigType"
QT_MOC_LITERAL(10, 137, 4), // "type"
QT_MOC_LITERAL(11, 142, 19), // "RequestLogoutSignal"
QT_MOC_LITERAL(12, 162, 21), // "RequestShutdownSignal"
QT_MOC_LITERAL(13, 184, 20), // "RequestRestartSignal"
QT_MOC_LITERAL(14, 205, 23), // "RequestQuitSystemSignal"
QT_MOC_LITERAL(15, 229, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(16, 268, 14), // "confernece_uri"
QT_MOC_LITERAL(17, 283, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(18, 322, 5), // "state"
QT_MOC_LITERAL(19, 328, 26), // "HandleStartVideoNotifySlot"
QT_MOC_LITERAL(20, 355, 25), // "HandleStopVideoNotifySlot"
QT_MOC_LITERAL(21, 381, 23), // "DisplayDeviceConfigSlot"
QT_MOC_LITERAL(22, 405, 20), // "DisplayMicConfigSlot"
QT_MOC_LITERAL(23, 426, 23), // "DisplaySpeakerConfiSlot"
QT_MOC_LITERAL(24, 450, 27), // "DisplayDeviceTestConfigSlot"
QT_MOC_LITERAL(25, 478, 24), // "DisplayMonitorConfigSlot"
QT_MOC_LITERAL(26, 503, 21), // "ChangeSettingModeSlot"
QT_MOC_LITERAL(27, 525, 7) // "setting"

    },
    "LocalTerminalWidget\0StopVideoSignal\0"
    "\0vuri\0StopDocumentSignal\0uri\0"
    "ConfigCameraSignal\0index\0"
    "DisplayTerminalConfigSignal\0"
    "TerminalConfigType\0type\0RequestLogoutSignal\0"
    "RequestShutdownSignal\0RequestRestartSignal\0"
    "RequestQuitSystemSignal\0"
    "HandleConferenceStateChangedNotifySlot\0"
    "confernece_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleStartVideoNotifySlot\0"
    "HandleStopVideoNotifySlot\0"
    "DisplayDeviceConfigSlot\0DisplayMicConfigSlot\0"
    "DisplaySpeakerConfiSlot\0"
    "DisplayDeviceTestConfigSlot\0"
    "DisplayMonitorConfigSlot\0ChangeSettingModeSlot\0"
    "setting"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocalTerminalWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       4,    1,  102,    2, 0x06 /* Public */,
       6,    1,  105,    2, 0x06 /* Public */,
       8,    1,  108,    2, 0x06 /* Public */,
      11,    0,  111,    2, 0x06 /* Public */,
      12,    0,  112,    2, 0x06 /* Public */,
      13,    0,  113,    2, 0x06 /* Public */,
      14,    0,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    2,  115,    2, 0x08 /* Private */,
      19,    1,  120,    2, 0x08 /* Private */,
      20,    1,  123,    2, 0x08 /* Private */,
      21,    0,  126,    2, 0x08 /* Private */,
      22,    0,  127,    2, 0x08 /* Private */,
      23,    0,  128,    2, 0x08 /* Private */,
      24,    0,  129,    2, 0x08 /* Private */,
      25,    0,  130,    2, 0x08 /* Private */,
      26,    1,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 17,   16,   18,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   27,

       0        // eod
};

void LocalTerminalWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LocalTerminalWidget *_t = static_cast<LocalTerminalWidget *>(_o);
        switch (_id) {
        case 0: _t->StopVideoSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->StopDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->ConfigCameraSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->DisplayTerminalConfigSignal((*reinterpret_cast< TerminalConfigType(*)>(_a[1]))); break;
        case 4: _t->RequestLogoutSignal(); break;
        case 5: _t->RequestShutdownSignal(); break;
        case 6: _t->RequestRestartSignal(); break;
        case 7: _t->RequestQuitSystemSignal(); break;
        case 8: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 9: _t->HandleStartVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->HandleStopVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->DisplayDeviceConfigSlot(); break;
        case 12: _t->DisplayMicConfigSlot(); break;
        case 13: _t->DisplaySpeakerConfiSlot(); break;
        case 14: _t->DisplayDeviceTestConfigSlot(); break;
        case 15: _t->DisplayMonitorConfigSlot(); break;
        case 16: _t->ChangeSettingModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LocalTerminalWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::StopVideoSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::StopDocumentSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::ConfigCameraSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)(TerminalConfigType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::DisplayTerminalConfigSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::RequestLogoutSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::RequestShutdownSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::RequestRestartSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (LocalTerminalWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalTerminalWidget::RequestQuitSystemSignal)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject LocalTerminalWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LocalTerminalWidget.data,
      qt_meta_data_LocalTerminalWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LocalTerminalWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalTerminalWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LocalTerminalWidget.stringdata))
        return static_cast<void*>(const_cast< LocalTerminalWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int LocalTerminalWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void LocalTerminalWidget::StopVideoSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LocalTerminalWidget::StopDocumentSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LocalTerminalWidget::ConfigCameraSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LocalTerminalWidget::DisplayTerminalConfigSignal(TerminalConfigType _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LocalTerminalWidget::RequestLogoutSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void LocalTerminalWidget::RequestShutdownSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void LocalTerminalWidget::RequestRestartSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void LocalTerminalWidget::RequestQuitSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
