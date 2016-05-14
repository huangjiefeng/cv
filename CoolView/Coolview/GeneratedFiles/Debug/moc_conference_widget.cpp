/****************************************************************************
** Meta object code from reading C++ file 'conference_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../conference_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conference_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConferenceWidget_t {
    QByteArrayData data[24];
    char stringdata[536];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConferenceWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConferenceWidget_t qt_meta_stringdata_ConferenceWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ConferenceWidget"
QT_MOC_LITERAL(1, 17, 20), // "JoinConferenceSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 14), // "conference_uri"
QT_MOC_LITERAL(4, 54, 21), // "LeaveConferenceSignal"
QT_MOC_LITERAL(5, 76, 27), // "RequestConferenceListSignal"
QT_MOC_LITERAL(6, 104, 20), // "SetWindowTitleSignal"
QT_MOC_LITERAL(7, 125, 5), // "title"
QT_MOC_LITERAL(8, 131, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(9, 167, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(10, 200, 5), // "state"
QT_MOC_LITERAL(11, 206, 38), // "HandleConferenceListReceivedN..."
QT_MOC_LITERAL(12, 245, 19), // "ConstConferenceList"
QT_MOC_LITERAL(13, 265, 11), // "conferences"
QT_MOC_LITERAL(14, 277, 38), // "HandleConferenceStateChangedN..."
QT_MOC_LITERAL(15, 316, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(16, 355, 35), // "HandleJoinConferenceErrorNoti..."
QT_MOC_LITERAL(17, 391, 42), // "IConferenceController::JoinCo..."
QT_MOC_LITERAL(18, 434, 5), // "error"
QT_MOC_LITERAL(19, 440, 23), // "TryToJoinConferenceSlot"
QT_MOC_LITERAL(20, 464, 21), // "ShowConferenceTipSlot"
QT_MOC_LITERAL(21, 486, 5), // "index"
QT_MOC_LITERAL(22, 492, 18), // "ShowHelpWidgetSlot"
QT_MOC_LITERAL(23, 511, 24) // "UpdateConferenceListSlot"

    },
    "ConferenceWidget\0JoinConferenceSignal\0"
    "\0conference_uri\0LeaveConferenceSignal\0"
    "RequestConferenceListSignal\0"
    "SetWindowTitleSignal\0title\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "HandleConferenceListReceivedNotifySlot\0"
    "ConstConferenceList\0conferences\0"
    "HandleConferenceStateChangedNotifySlot\0"
    "IConferenceController::ConferenceState\0"
    "HandleJoinConferenceErrorNotifySlot\0"
    "IConferenceController::JoinConferenceError\0"
    "error\0TryToJoinConferenceSlot\0"
    "ShowConferenceTipSlot\0index\0"
    "ShowHelpWidgetSlot\0UpdateConferenceListSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConferenceWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    0,   77,    2, 0x06 /* Public */,
       5,    0,   78,    2, 0x06 /* Public */,
       6,    1,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   82,    2, 0x0a /* Public */,
      11,    1,   85,    2, 0x0a /* Public */,
      14,    2,   88,    2, 0x0a /* Public */,
      16,    2,   93,    2, 0x0a /* Public */,
      19,    0,   98,    2, 0x08 /* Private */,
      20,    1,   99,    2, 0x08 /* Private */,
      22,    0,  102,    2, 0x08 /* Private */,
      23,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 15,    3,   10,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 17,    3,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConferenceWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConferenceWidget *_t = static_cast<ConferenceWidget *>(_o);
        switch (_id) {
        case 0: _t->JoinConferenceSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->LeaveConferenceSignal(); break;
        case 2: _t->RequestConferenceListSignal(); break;
        case 3: _t->SetWindowTitleSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 5: _t->HandleConferenceListReceivedNotifySlot((*reinterpret_cast< const ConstConferenceList(*)>(_a[1]))); break;
        case 6: _t->HandleConferenceStateChangedNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 7: _t->HandleJoinConferenceErrorNotifySlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::JoinConferenceError(*)>(_a[2]))); break;
        case 8: _t->TryToJoinConferenceSlot(); break;
        case 9: _t->ShowConferenceTipSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->ShowHelpWidgetSlot(); break;
        case 11: _t->UpdateConferenceListSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConferenceWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceWidget::JoinConferenceSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ConferenceWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceWidget::LeaveConferenceSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (ConferenceWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceWidget::RequestConferenceListSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (ConferenceWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConferenceWidget::SetWindowTitleSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ConferenceWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConferenceWidget.data,
      qt_meta_data_ConferenceWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConferenceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConferenceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConferenceWidget.stringdata))
        return static_cast<void*>(const_cast< ConferenceWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConferenceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ConferenceWidget::JoinConferenceSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConferenceWidget::LeaveConferenceSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ConferenceWidget::RequestConferenceListSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ConferenceWidget::SetWindowTitleSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
