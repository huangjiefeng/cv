/****************************************************************************
** Meta object code from reading C++ file 'upload_status_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../upload_status_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'upload_status_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UploadStatusWidget_t {
    QByteArrayData data[9];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UploadStatusWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UploadStatusWidget_t qt_meta_stringdata_UploadStatusWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "UploadStatusWidget"
QT_MOC_LITERAL(1, 19, 26), // "HandleShowErrorMessageSlot"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 4), // "info"
QT_MOC_LITERAL(4, 52, 26), // "HandleItemStateChangedSlot"
QT_MOC_LITERAL(5, 79, 11), // "ItemOperate"
QT_MOC_LITERAL(6, 91, 4), // "oper"
QT_MOC_LITERAL(7, 96, 10), // "UploadItem"
QT_MOC_LITERAL(8, 107, 4) // "item"

    },
    "UploadStatusWidget\0HandleShowErrorMessageSlot\0"
    "\0info\0HandleItemStateChangedSlot\0"
    "ItemOperate\0oper\0UploadItem\0item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UploadStatusWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x09 /* Protected */,
       4,    2,   27,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,

       0        // eod
};

void UploadStatusWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UploadStatusWidget *_t = static_cast<UploadStatusWidget *>(_o);
        switch (_id) {
        case 0: _t->HandleShowErrorMessageSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->HandleItemStateChangedSlot((*reinterpret_cast< ItemOperate(*)>(_a[1])),(*reinterpret_cast< const UploadItem(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject UploadStatusWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UploadStatusWidget.data,
      qt_meta_data_UploadStatusWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UploadStatusWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UploadStatusWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UploadStatusWidget.stringdata))
        return static_cast<void*>(const_cast< UploadStatusWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UploadStatusWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
