/****************************************************************************
** Meta object code from reading C++ file 'terminal_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../terminal_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminal_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TerminalWidget_t {
    QByteArrayData data[16];
    char stringdata[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TerminalWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TerminalWidget_t qt_meta_stringdata_TerminalWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TerminalWidget"
QT_MOC_LITERAL(1, 15, 17), // "PermitSpeakSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 3), // "uri"
QT_MOC_LITERAL(4, 38, 5), // "allow"
QT_MOC_LITERAL(5, 44, 25), // "ControlReceiveAudioSignal"
QT_MOC_LITERAL(6, 70, 7), // "receive"
QT_MOC_LITERAL(7, 78, 22), // "StopReceiveVideoSignal"
QT_MOC_LITERAL(8, 101, 4), // "vuri"
QT_MOC_LITERAL(9, 106, 28), // "HandleReceiveVideoNotifySlot"
QT_MOC_LITERAL(10, 135, 25), // "HandleStopVideoNotifySlot"
QT_MOC_LITERAL(11, 161, 15), // "PermitSpeakSlot"
QT_MOC_LITERAL(12, 177, 24), // "ControlAudioReceivedSlot"
QT_MOC_LITERAL(13, 202, 23), // "StopReceiveAllVideoSlot"
QT_MOC_LITERAL(14, 226, 24), // "HideVirtualTerminalsSlot"
QT_MOC_LITERAL(15, 251, 27) // "TerminalOnlineAnimationSlot"

    },
    "TerminalWidget\0PermitSpeakSignal\0\0uri\0"
    "allow\0ControlReceiveAudioSignal\0receive\0"
    "StopReceiveVideoSignal\0vuri\0"
    "HandleReceiveVideoNotifySlot\0"
    "HandleStopVideoNotifySlot\0PermitSpeakSlot\0"
    "ControlAudioReceivedSlot\0"
    "StopReceiveAllVideoSlot\0"
    "HideVirtualTerminalsSlot\0"
    "TerminalOnlineAnimationSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerminalWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    2,   69,    2, 0x06 /* Public */,
       7,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   77,    2, 0x08 /* Private */,
      10,    1,   80,    2, 0x08 /* Private */,
      11,    0,   83,    2, 0x08 /* Private */,
      12,    0,   84,    2, 0x08 /* Private */,
      13,    0,   85,    2, 0x08 /* Private */,
      14,    0,   86,    2, 0x08 /* Private */,
      15,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    3,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TerminalWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerminalWidget *_t = static_cast<TerminalWidget *>(_o);
        switch (_id) {
        case 0: _t->PermitSpeakSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->ControlReceiveAudioSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->StopReceiveVideoSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->HandleReceiveVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->HandleStopVideoNotifySlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->PermitSpeakSlot(); break;
        case 6: _t->ControlAudioReceivedSlot(); break;
        case 7: _t->StopReceiveAllVideoSlot(); break;
        case 8: _t->HideVirtualTerminalsSlot(); break;
        case 9: _t->TerminalOnlineAnimationSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TerminalWidget::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalWidget::PermitSpeakSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (TerminalWidget::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalWidget::ControlReceiveAudioSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (TerminalWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalWidget::StopReceiveVideoSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject TerminalWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TerminalWidget.data,
      qt_meta_data_TerminalWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TerminalWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerminalWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalWidget.stringdata))
        return static_cast<void*>(const_cast< TerminalWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int TerminalWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void TerminalWidget::PermitSpeakSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TerminalWidget::ControlReceiveAudioSignal(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TerminalWidget::StopReceiveVideoSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
