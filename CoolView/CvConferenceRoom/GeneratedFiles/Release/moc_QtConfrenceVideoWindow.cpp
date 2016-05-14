/****************************************************************************
** Meta object code from reading C++ file 'QtConfrenceVideoWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtConfrenceVideoWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtConfrenceVideoWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtConfrenceVideoWindow_t {
    QByteArrayData data[10];
    char stringdata[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtConfrenceVideoWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtConfrenceVideoWindow_t qt_meta_stringdata_QtConfrenceVideoWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "QtConfrenceVideoWindow"
QT_MOC_LITERAL(1, 23, 15), // "showRtcpMessage"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 15), // "hideRtcpMessage"
QT_MOC_LITERAL(4, 56, 14), // "getDisplayName"
QT_MOC_LITERAL(5, 71, 14), // "setDisplayName"
QT_MOC_LITERAL(6, 86, 11), // "displayName"
QT_MOC_LITERAL(7, 98, 11), // "startRecord"
QT_MOC_LITERAL(8, 110, 10), // "stopRecord"
QT_MOC_LITERAL(9, 121, 23) // "ShowRecordAdvancedPanel"

    },
    "QtConfrenceVideoWindow\0showRtcpMessage\0"
    "\0hideRtcpMessage\0getDisplayName\0"
    "setDisplayName\0displayName\0startRecord\0"
    "stopRecord\0ShowRecordAdvancedPanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtConfrenceVideoWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    1,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void QtConfrenceVideoWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtConfrenceVideoWindow *_t = static_cast<QtConfrenceVideoWindow *>(_o);
        switch (_id) {
        case 0: _t->showRtcpMessage(); break;
        case 1: _t->hideRtcpMessage(); break;
        case 2: { QString _r = _t->getDisplayName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: _t->setDisplayName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->startRecord(); break;
        case 5: _t->stopRecord(); break;
        case 6: _t->ShowRecordAdvancedPanel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QtConfrenceVideoWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtConfrenceVideoWindow.data,
      qt_meta_data_QtConfrenceVideoWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtConfrenceVideoWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtConfrenceVideoWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtConfrenceVideoWindow.stringdata))
        return static_cast<void*>(const_cast< QtConfrenceVideoWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtConfrenceVideoWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
