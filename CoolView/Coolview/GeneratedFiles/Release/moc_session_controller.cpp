/****************************************************************************
** Meta object code from reading C++ file 'session_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../session_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'session_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SessionController_t {
    QByteArrayData data[22];
    char stringdata[489];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SessionController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SessionController_t qt_meta_stringdata_SessionController = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SessionController"
QT_MOC_LITERAL(1, 18, 26), // "SendTerminalLoginMsgSignal"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 18), // "TerminalLoginParam"
QT_MOC_LITERAL(4, 65, 4), // "info"
QT_MOC_LITERAL(5, 70, 25), // "SendTerminalSAVAMsgSignal"
QT_MOC_LITERAL(6, 96, 17), // "TerminalSAVAParam"
QT_MOC_LITERAL(7, 114, 21), // "SendRegisterMsgSignal"
QT_MOC_LITERAL(8, 136, 19), // "SendOnlineMsgSignal"
QT_MOC_LITERAL(9, 156, 27), // "SendTerminalLogoutMsgSignal"
QT_MOC_LITERAL(10, 184, 28), // "LeaveConferenceRequestSignal"
QT_MOC_LITERAL(11, 213, 22), // "HandleLoginRequestSlot"
QT_MOC_LITERAL(12, 236, 23), // "HandleLogoutRequestSlot"
QT_MOC_LITERAL(13, 260, 32), // "HandleSipAccountStateChangedSlot"
QT_MOC_LITERAL(14, 293, 8), // "username"
QT_MOC_LITERAL(15, 302, 5), // "state"
QT_MOC_LITERAL(16, 308, 29), // "HandleCheckNetworkRequestSlot"
QT_MOC_LITERAL(17, 338, 29), // "HandleSendRegisterRequestSlot"
QT_MOC_LITERAL(18, 368, 27), // "HandleSendOnlineRequestSlot"
QT_MOC_LITERAL(19, 396, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(20, 435, 14), // "conference_uri"
QT_MOC_LITERAL(21, 450, 38) // "IConferenceController::Confer..."

    },
    "SessionController\0SendTerminalLoginMsgSignal\0"
    "\0TerminalLoginParam\0info\0"
    "SendTerminalSAVAMsgSignal\0TerminalSAVAParam\0"
    "SendRegisterMsgSignal\0SendOnlineMsgSignal\0"
    "SendTerminalLogoutMsgSignal\0"
    "LeaveConferenceRequestSignal\0"
    "HandleLoginRequestSlot\0HandleLogoutRequestSlot\0"
    "HandleSipAccountStateChangedSlot\0"
    "username\0state\0HandleCheckNetworkRequestSlot\0"
    "HandleSendRegisterRequestSlot\0"
    "HandleSendOnlineRequestSlot\0"
    "HandleConferenceStateChangedNotifySlot\0"
    "conference_uri\0IConferenceController::ConferenceState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SessionController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       5,    1,   82,    2, 0x06 /* Public */,
       7,    0,   85,    2, 0x06 /* Public */,
       8,    0,   86,    2, 0x06 /* Public */,
       9,    0,   87,    2, 0x06 /* Public */,
      10,    0,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   89,    2, 0x0a /* Public */,
      12,    0,   90,    2, 0x0a /* Public */,
      13,    2,   91,    2, 0x08 /* Private */,
      16,    0,   96,    2, 0x08 /* Private */,
      17,    0,   97,    2, 0x08 /* Private */,
      18,    0,   98,    2, 0x08 /* Private */,
      19,    2,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 21,   20,   15,

       0        // eod
};

void SessionController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SessionController *_t = static_cast<SessionController *>(_o);
        switch (_id) {
        case 0: _t->SendTerminalLoginMsgSignal((*reinterpret_cast< const TerminalLoginParam(*)>(_a[1]))); break;
        case 1: _t->SendTerminalSAVAMsgSignal((*reinterpret_cast< const TerminalSAVAParam(*)>(_a[1]))); break;
        case 2: _t->SendRegisterMsgSignal(); break;
        case 3: _t->SendOnlineMsgSignal(); break;
        case 4: _t->SendTerminalLogoutMsgSignal(); break;
        case 5: _t->LeaveConferenceRequestSignal(); break;
        case 6: _t->HandleLoginRequestSlot(); break;
        case 7: _t->HandleLogoutRequestSlot(); break;
        case 8: _t->HandleSipAccountStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->HandleCheckNetworkRequestSlot(); break;
        case 10: _t->HandleSendRegisterRequestSlot(); break;
        case 11: _t->HandleSendOnlineRequestSlot(); break;
        case 12: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SessionController::*_t)(const TerminalLoginParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::SendTerminalLoginMsgSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SessionController::*_t)(const TerminalSAVAParam & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::SendTerminalSAVAMsgSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (SessionController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::SendRegisterMsgSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (SessionController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::SendOnlineMsgSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (SessionController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::SendTerminalLogoutMsgSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (SessionController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionController::LeaveConferenceRequestSignal)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject SessionController::staticMetaObject = {
    { &ISessionController::staticMetaObject, qt_meta_stringdata_SessionController.data,
      qt_meta_data_SessionController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SessionController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SessionController.stringdata))
        return static_cast<void*>(const_cast< SessionController*>(this));
    return ISessionController::qt_metacast(_clname);
}

int SessionController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ISessionController::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SessionController::SendTerminalLoginMsgSignal(const TerminalLoginParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SessionController::SendTerminalSAVAMsgSignal(const TerminalSAVAParam & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SessionController::SendRegisterMsgSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void SessionController::SendOnlineMsgSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void SessionController::SendTerminalLogoutMsgSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void SessionController::LeaveConferenceRequestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
