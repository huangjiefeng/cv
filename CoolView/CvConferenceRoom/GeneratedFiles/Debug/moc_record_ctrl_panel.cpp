/****************************************************************************
** Meta object code from reading C++ file 'record_ctrl_panel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../record_ctrl_panel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'record_ctrl_panel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RecordCtrlPanel_t {
    QByteArrayData data[7];
    char stringdata[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecordCtrlPanel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecordCtrlPanel_t qt_meta_stringdata_RecordCtrlPanel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RecordCtrlPanel"
QT_MOC_LITERAL(1, 16, 17), // "StartRecordSignal"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 16), // "StopRecordSignal"
QT_MOC_LITERAL(4, 52, 22), // "ShowRecInfoPanelSignal"
QT_MOC_LITERAL(5, 75, 23), // "HandleRecBtnClickedSlot"
QT_MOC_LITERAL(6, 99, 31) // "HandleRecInfoEditBtnClickedSlot"

    },
    "RecordCtrlPanel\0StartRecordSignal\0\0"
    "StopRecordSignal\0ShowRecInfoPanelSignal\0"
    "HandleRecBtnClickedSlot\0"
    "HandleRecInfoEditBtnClickedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecordCtrlPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    1,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   44,    2, 0x0a /* Public */,
       6,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RecordCtrlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecordCtrlPanel *_t = static_cast<RecordCtrlPanel *>(_o);
        switch (_id) {
        case 0: _t->StartRecordSignal(); break;
        case 1: _t->StopRecordSignal(); break;
        case 2: _t->ShowRecInfoPanelSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->HandleRecBtnClickedSlot(); break;
        case 4: _t->HandleRecInfoEditBtnClickedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RecordCtrlPanel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RecordCtrlPanel::StartRecordSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (RecordCtrlPanel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RecordCtrlPanel::StopRecordSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (RecordCtrlPanel::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RecordCtrlPanel::ShowRecInfoPanelSignal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject RecordCtrlPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RecordCtrlPanel.data,
      qt_meta_data_RecordCtrlPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RecordCtrlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecordCtrlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RecordCtrlPanel.stringdata))
        return static_cast<void*>(const_cast< RecordCtrlPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int RecordCtrlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RecordCtrlPanel::StartRecordSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void RecordCtrlPanel::StopRecordSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RecordCtrlPanel::ShowRecInfoPanelSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
