/****************************************************************************
** Meta object code from reading C++ file 'terminal_list_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../terminal_list_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminal_list_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TerminalListWidget_t {
    QByteArrayData data[25];
    char stringdata[521];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TerminalListWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TerminalListWidget_t qt_meta_stringdata_TerminalListWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TerminalListWidget"
QT_MOC_LITERAL(1, 19, 15), // "StopVideoSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "vuri"
QT_MOC_LITERAL(4, 41, 18), // "StopDocumentSignal"
QT_MOC_LITERAL(5, 60, 3), // "uri"
QT_MOC_LITERAL(6, 64, 21), // "LeaveConferenceSignal"
QT_MOC_LITERAL(7, 86, 22), // "SetConferenceTitleSlot"
QT_MOC_LITERAL(8, 109, 5), // "title"
QT_MOC_LITERAL(9, 115, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(10, 154, 14), // "conference_uri"
QT_MOC_LITERAL(11, 169, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(12, 208, 5), // "state"
QT_MOC_LITERAL(13, 214, 32), // "HandleTerminalReceivedNotifySlot"
QT_MOC_LITERAL(14, 247, 20), // "ConstTerminalPointer"
QT_MOC_LITERAL(15, 268, 8), // "terminal"
QT_MOC_LITERAL(16, 277, 36), // "HandleTerminalCountChangedNot..."
QT_MOC_LITERAL(17, 314, 20), // "total_terminal_count"
QT_MOC_LITERAL(18, 335, 30), // "HandleTerminalUpdateNotifySlot"
QT_MOC_LITERAL(19, 366, 28), // "HandleReceiveAudioNotifySlot"
QT_MOC_LITERAL(20, 395, 7), // "receive"
QT_MOC_LITERAL(21, 403, 26), // "HandleShowAllTerminalsSlot"
QT_MOC_LITERAL(22, 430, 29), // "HandleShowOnlineTerminalsSlot"
QT_MOC_LITERAL(23, 460, 30), // "HandleShowOfflineTerminalsSlot"
QT_MOC_LITERAL(24, 491, 29) // "ConfirmAndLeaveConferenceSlot"

    },
    "TerminalListWidget\0StopVideoSignal\0\0"
    "vuri\0StopDocumentSignal\0uri\0"
    "LeaveConferenceSignal\0SetConferenceTitleSlot\0"
    "title\0HandleConferenceStateChangedNotifySlot\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleTerminalReceivedNotifySlot\0"
    "ConstTerminalPointer\0terminal\0"
    "HandleTerminalCountChangedNotifySlot\0"
    "total_terminal_count\0"
    "HandleTerminalUpdateNotifySlot\0"
    "HandleReceiveAudioNotifySlot\0receive\0"
    "HandleShowAllTerminalsSlot\0"
    "HandleShowOnlineTerminalsSlot\0"
    "HandleShowOfflineTerminalsSlot\0"
    "ConfirmAndLeaveConferenceSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerminalListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   86,    2, 0x0a /* Public */,
       9,    2,   89,    2, 0x08 /* Private */,
      13,    2,   94,    2, 0x08 /* Private */,
      16,    2,   99,    2, 0x08 /* Private */,
      18,    2,  104,    2, 0x08 /* Private */,
      19,    2,  109,    2, 0x08 /* Private */,
      21,    0,  114,    2, 0x08 /* Private */,
      22,    0,  115,    2, 0x08 /* Private */,
      23,    0,  116,    2, 0x08 /* Private */,
      24,    0,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 11,   10,   12,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QString,   15,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   17,   10,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QString,   15,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    5,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TerminalListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerminalListWidget *_t = static_cast<TerminalListWidget *>(_o);
        switch (_id) {
        case 0: _t->StopVideoSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->StopDocumentSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->LeaveConferenceSignal(); break;
        case 3: _t->SetConferenceTitleSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 5: _t->HandleTerminalReceivedNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->HandleTerminalCountChangedNotifySlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->HandleTerminalUpdateNotifySlot((*reinterpret_cast< ConstTerminalPointer(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->HandleReceiveAudioNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->HandleShowAllTerminalsSlot(); break;
        case 10: _t->HandleShowOnlineTerminalsSlot(); break;
        case 11: _t->HandleShowOfflineTerminalsSlot(); break;
        case 12: _t->ConfirmAndLeaveConferenceSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TerminalListWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalListWidget::StopVideoSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (TerminalListWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalListWidget::StopDocumentSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (TerminalListWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerminalListWidget::LeaveConferenceSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject TerminalListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TerminalListWidget.data,
      qt_meta_data_TerminalListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TerminalListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerminalListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalListWidget.stringdata))
        return static_cast<void*>(const_cast< TerminalListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int TerminalListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void TerminalListWidget::StopVideoSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TerminalListWidget::StopDocumentSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TerminalListWidget::LeaveConferenceSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
