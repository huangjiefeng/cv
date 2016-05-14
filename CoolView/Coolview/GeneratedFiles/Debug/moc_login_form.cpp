/****************************************************************************
** Meta object code from reading C++ file 'login_form.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../login_form.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login_form.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LoginForm_t {
    QByteArrayData data[22];
    char stringdata[439];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoginForm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoginForm_t qt_meta_stringdata_LoginForm = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LoginForm"
QT_MOC_LITERAL(1, 10, 18), // "LoginRequestSignal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 20), // "ShutDownSystemSignal"
QT_MOC_LITERAL(4, 51, 19), // "RestartSystemSignal"
QT_MOC_LITERAL(5, 71, 15), // "QuitSytemSignal"
QT_MOC_LITERAL(6, 87, 26), // "HandleLoginErrorNotifySlot"
QT_MOC_LITERAL(7, 114, 30), // "ISessionController::LoginError"
QT_MOC_LITERAL(8, 145, 5), // "error"
QT_MOC_LITERAL(9, 151, 35), // "HandleSessionStateChangedNoti..."
QT_MOC_LITERAL(10, 187, 32), // "ISessionController::SessionState"
QT_MOC_LITERAL(11, 220, 5), // "state"
QT_MOC_LITERAL(12, 226, 14), // "TryToLoginSlot"
QT_MOC_LITERAL(13, 241, 26), // "ShowOrHideExtraSettingSlot"
QT_MOC_LITERAL(14, 268, 20), // "AutoLoginCheckedSlot"
QT_MOC_LITERAL(15, 289, 22), // "ShowIpConfigDialogSlot"
QT_MOC_LITERAL(16, 312, 27), // "ShowDeviceSettingWidgetSlot"
QT_MOC_LITERAL(17, 340, 14), // "LoadIPListSlot"
QT_MOC_LITERAL(18, 355, 14), // "LoadConfigSlot"
QT_MOC_LITERAL(19, 370, 14), // "SaveConfigSlot"
QT_MOC_LITERAL(20, 385, 16), // "UpdateScreenSlot"
QT_MOC_LITERAL(21, 402, 36) // "HandlePrimaryScreenChangedNot..."

    },
    "LoginForm\0LoginRequestSignal\0\0"
    "ShutDownSystemSignal\0RestartSystemSignal\0"
    "QuitSytemSignal\0HandleLoginErrorNotifySlot\0"
    "ISessionController::LoginError\0error\0"
    "HandleSessionStateChangedNotifySlot\0"
    "ISessionController::SessionState\0state\0"
    "TryToLoginSlot\0ShowOrHideExtraSettingSlot\0"
    "AutoLoginCheckedSlot\0ShowIpConfigDialogSlot\0"
    "ShowDeviceSettingWidgetSlot\0LoadIPListSlot\0"
    "LoadConfigSlot\0SaveConfigSlot\0"
    "UpdateScreenSlot\0HandlePrimaryScreenChangedNotifySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoginForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,
       4,    0,   96,    2, 0x06 /* Public */,
       5,    0,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   98,    2, 0x08 /* Private */,
       9,    1,  101,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    1,  106,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,
      18,    0,  112,    2, 0x08 /* Private */,
      19,    0,  113,    2, 0x08 /* Private */,
      20,    0,  114,    2, 0x08 /* Private */,
      21,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoginForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoginForm *_t = static_cast<LoginForm *>(_o);
        switch (_id) {
        case 0: _t->LoginRequestSignal(); break;
        case 1: _t->ShutDownSystemSignal(); break;
        case 2: _t->RestartSystemSignal(); break;
        case 3: _t->QuitSytemSignal(); break;
        case 4: _t->HandleLoginErrorNotifySlot((*reinterpret_cast< ISessionController::LoginError(*)>(_a[1]))); break;
        case 5: _t->HandleSessionStateChangedNotifySlot((*reinterpret_cast< ISessionController::SessionState(*)>(_a[1]))); break;
        case 6: _t->TryToLoginSlot(); break;
        case 7: _t->ShowOrHideExtraSettingSlot(); break;
        case 8: _t->AutoLoginCheckedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->ShowIpConfigDialogSlot(); break;
        case 10: _t->ShowDeviceSettingWidgetSlot(); break;
        case 11: _t->LoadIPListSlot(); break;
        case 12: _t->LoadConfigSlot(); break;
        case 13: _t->SaveConfigSlot(); break;
        case 14: _t->UpdateScreenSlot(); break;
        case 15: _t->HandlePrimaryScreenChangedNotifySlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LoginForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginForm::LoginRequestSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (LoginForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginForm::ShutDownSystemSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (LoginForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginForm::RestartSystemSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (LoginForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginForm::QuitSytemSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject LoginForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LoginForm.data,
      qt_meta_data_LoginForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LoginForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LoginForm.stringdata))
        return static_cast<void*>(const_cast< LoginForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int LoginForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void LoginForm::LoginRequestSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void LoginForm::ShutDownSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void LoginForm::RestartSystemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void LoginForm::QuitSytemSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
