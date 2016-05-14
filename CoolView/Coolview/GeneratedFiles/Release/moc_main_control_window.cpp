/****************************************************************************
** Meta object code from reading C++ file 'main_control_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../main_control_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_control_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainControlWindow_t {
    QByteArrayData data[39];
    char stringdata[822];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainControlWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainControlWindow_t qt_meta_stringdata_MainControlWindow = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MainControlWindow"
QT_MOC_LITERAL(1, 18, 19), // "RestartSystemSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 20), // "ShutDownSystemSignal"
QT_MOC_LITERAL(4, 60, 16), // "QuitSystemSignal"
QT_MOC_LITERAL(5, 77, 12), // "LogoutSignal"
QT_MOC_LITERAL(6, 90, 31), // "NotifyVisibleStateChangedSignal"
QT_MOC_LITERAL(7, 122, 17), // "telecontroller_id"
QT_MOC_LITERAL(8, 140, 7), // "visible"
QT_MOC_LITERAL(9, 148, 18), // "SetWindowTitleSlot"
QT_MOC_LITERAL(10, 167, 5), // "title"
QT_MOC_LITERAL(11, 173, 31), // "HandleActivateCameraSettingSlot"
QT_MOC_LITERAL(12, 205, 5), // "index"
QT_MOC_LITERAL(13, 211, 32), // "HandleActivateTerminalConfigSlot"
QT_MOC_LITERAL(14, 244, 18), // "TerminalConfigType"
QT_MOC_LITERAL(15, 263, 4), // "type"
QT_MOC_LITERAL(16, 268, 20), // "ConfirmAndLogoutSlot"
QT_MOC_LITERAL(17, 289, 24), // "ConfirmAndQuitSystemSlot"
QT_MOC_LITERAL(18, 314, 22), // "ConfirmAndShutdownSlot"
QT_MOC_LITERAL(19, 337, 27), // "ConfirmAndRestartSystemSlot"
QT_MOC_LITERAL(20, 365, 18), // "ShowDeviceListSlot"
QT_MOC_LITERAL(21, 384, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(22, 420, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(23, 453, 5), // "state"
QT_MOC_LITERAL(24, 459, 24), // "HandleRecvSaviNotifySlot"
QT_MOC_LITERAL(25, 484, 3), // "res"
QT_MOC_LITERAL(26, 488, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(27, 527, 3), // "uri"
QT_MOC_LITERAL(28, 531, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(29, 570, 27), // "HandleSetVisibleRequestSlot"
QT_MOC_LITERAL(30, 598, 29), // "HandleVisibleStateRequestSlot"
QT_MOC_LITERAL(31, 628, 26), // "HandleQuitSystemNotifySlot"
QT_MOC_LITERAL(32, 655, 35), // "HandleScreenLayoutChangedNoti..."
QT_MOC_LITERAL(33, 691, 12), // "screen_index"
QT_MOC_LITERAL(34, 704, 32), // "IDisplayController::ScreenLayout"
QT_MOC_LITERAL(35, 737, 6), // "layout"
QT_MOC_LITERAL(36, 744, 25), // "HandleNewConfigNotifySlot"
QT_MOC_LITERAL(37, 770, 36), // "HandlePrimaryScreenChangedNot..."
QT_MOC_LITERAL(38, 807, 14) // "primary_screen"

    },
    "MainControlWindow\0RestartSystemSignal\0"
    "\0ShutDownSystemSignal\0QuitSystemSignal\0"
    "LogoutSignal\0NotifyVisibleStateChangedSignal\0"
    "telecontroller_id\0visible\0SetWindowTitleSlot\0"
    "title\0HandleActivateCameraSettingSlot\0"
    "index\0HandleActivateTerminalConfigSlot\0"
    "TerminalConfigType\0type\0ConfirmAndLogoutSlot\0"
    "ConfirmAndQuitSystemSlot\0"
    "ConfirmAndShutdownSlot\0"
    "ConfirmAndRestartSystemSlot\0"
    "ShowDeviceListSlot\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "HandleRecvSaviNotifySlot\0res\0"
    "HandleConferenceStateChangedNotifySlot\0"
    "uri\0IConferenceController::ConferenceState\0"
    "HandleSetVisibleRequestSlot\0"
    "HandleVisibleStateRequestSlot\0"
    "HandleQuitSystemNotifySlot\0"
    "HandleScreenLayoutChangedNotifySlot\0"
    "screen_index\0IDisplayController::ScreenLayout\0"
    "layout\0HandleNewConfigNotifySlot\0"
    "HandlePrimaryScreenChangedNotifySlot\0"
    "primary_screen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainControlWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,
       3,    0,  125,    2, 0x06 /* Public */,
       4,    0,  126,    2, 0x06 /* Public */,
       5,    0,  127,    2, 0x06 /* Public */,
       6,    2,  128,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  133,    2, 0x0a /* Public */,
      11,    1,  136,    2, 0x0a /* Public */,
      13,    1,  139,    2, 0x0a /* Public */,
      16,    0,  142,    2, 0x0a /* Public */,
      17,    0,  143,    2, 0x0a /* Public */,
      18,    0,  144,    2, 0x0a /* Public */,
      19,    0,  145,    2, 0x0a /* Public */,
      20,    0,  146,    2, 0x08 /* Private */,
      21,    1,  147,    2, 0x08 /* Private */,
      24,    1,  150,    2, 0x08 /* Private */,
      26,    2,  153,    2, 0x08 /* Private */,
      29,    1,  158,    2, 0x08 /* Private */,
      30,    1,  161,    2, 0x08 /* Private */,
      31,    0,  164,    2, 0x08 /* Private */,
      32,    2,  165,    2, 0x08 /* Private */,
      36,    0,  170,    2, 0x08 /* Private */,
      37,    1,  171,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::Bool,   25,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 28,   27,   23,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 34,   33,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   38,

       0        // eod
};

void MainControlWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainControlWindow *_t = static_cast<MainControlWindow *>(_o);
        switch (_id) {
        case 0: _t->RestartSystemSignal(); break;
        case 1: _t->ShutDownSystemSignal(); break;
        case 2: _t->QuitSystemSignal(); break;
        case 3: _t->LogoutSignal(); break;
        case 4: _t->NotifyVisibleStateChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->SetWindowTitleSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->HandleActivateCameraSettingSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->HandleActivateTerminalConfigSlot((*reinterpret_cast< TerminalConfigType(*)>(_a[1]))); break;
        case 8: _t->ConfirmAndLogoutSlot(); break;
        case 9: _t->ConfirmAndQuitSystemSlot(); break;
        case 10: _t->ConfirmAndShutdownSlot(); break;
        case 11: _t->ConfirmAndRestartSystemSlot(); break;
        case 12: _t->ShowDeviceListSlot(); break;
        case 13: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 14: _t->HandleRecvSaviNotifySlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 16: _t->HandleSetVisibleRequestSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->HandleVisibleStateRequestSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->HandleQuitSystemNotifySlot(); break;
        case 19: _t->HandleScreenLayoutChangedNotifySlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< IDisplayController::ScreenLayout(*)>(_a[2]))); break;
        case 20: _t->HandleNewConfigNotifySlot(); break;
        case 21: _t->HandlePrimaryScreenChangedNotifySlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainControlWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainControlWindow::RestartSystemSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (MainControlWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainControlWindow::ShutDownSystemSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (MainControlWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainControlWindow::QuitSystemSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (MainControlWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainControlWindow::LogoutSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (MainControlWindow::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainControlWindow::NotifyVisibleStateChangedSignal)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject MainControlWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainControlWindow.data,
      qt_meta_data_MainControlWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainControlWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainControlWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainControlWindow.stringdata))
        return static_cast<void*>(const_cast< MainControlWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainControlWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void MainControlWindow::RestartSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MainControlWindow::ShutDownSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MainControlWindow::QuitSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MainControlWindow::LogoutSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void MainControlWindow::NotifyVisibleStateChangedSignal(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
