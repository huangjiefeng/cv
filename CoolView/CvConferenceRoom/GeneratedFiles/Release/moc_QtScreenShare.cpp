/****************************************************************************
** Meta object code from reading C++ file 'QtScreenShare.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtScreenShare.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtScreenShare.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtScreenShare_t {
    QByteArrayData data[16];
    char stringdata[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtScreenShare_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtScreenShare_t qt_meta_stringdata_QtScreenShare = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QtScreenShare"
QT_MOC_LITERAL(1, 14, 24), // "cvHostUnShareScreenSinal"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 8), // "openFile"
QT_MOC_LITERAL(4, 49, 9), // "closeFile"
QT_MOC_LITERAL(5, 59, 18), // "sendSpecificScreen"
QT_MOC_LITERAL(6, 78, 4), // "left"
QT_MOC_LITERAL(7, 83, 3), // "top"
QT_MOC_LITERAL(8, 87, 5), // "right"
QT_MOC_LITERAL(9, 93, 6), // "bottom"
QT_MOC_LITERAL(10, 100, 10), // "recvScreen"
QT_MOC_LITERAL(11, 111, 6), // "enable"
QT_MOC_LITERAL(12, 118, 23), // "cvHostUnShareScreenSlot"
QT_MOC_LITERAL(13, 142, 7), // "release"
QT_MOC_LITERAL(14, 150, 20), // "pptProcessFinishSlot"
QT_MOC_LITERAL(15, 171, 20) // "QProcess::ExitStatus"

    },
    "QtScreenShare\0cvHostUnShareScreenSinal\0"
    "\0openFile\0closeFile\0sendSpecificScreen\0"
    "left\0top\0right\0bottom\0recvScreen\0"
    "enable\0cvHostUnShareScreenSlot\0release\0"
    "pptProcessFinishSlot\0QProcess::ExitStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtScreenShare[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    4,   77,    2, 0x0a /* Public */,
       5,    3,   86,    2, 0x2a /* Public | MethodCloned */,
       5,    2,   93,    2, 0x2a /* Public | MethodCloned */,
       5,    1,   98,    2, 0x2a /* Public | MethodCloned */,
       5,    0,  101,    2, 0x2a /* Public | MethodCloned */,
      10,    1,  102,    2, 0x0a /* Public */,
      12,    0,  105,    2, 0x0a /* Public */,
      13,    0,  106,    2, 0x0a /* Public */,
      14,    2,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,    2,    2,

       0        // eod
};

void QtScreenShare::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtScreenShare *_t = static_cast<QtScreenShare *>(_o);
        switch (_id) {
        case 0: _t->cvHostUnShareScreenSinal(); break;
        case 1: _t->openFile(); break;
        case 2: _t->closeFile(); break;
        case 3: _t->sendSpecificScreen((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 4: _t->sendSpecificScreen((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 5: _t->sendSpecificScreen((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 6: _t->sendSpecificScreen((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 7: _t->sendSpecificScreen(); break;
        case 8: _t->recvScreen((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 9: _t->cvHostUnShareScreenSlot(); break;
        case 10: _t->release(); break;
        case 11: _t->pptProcessFinishSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QtScreenShare::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QtScreenShare::cvHostUnShareScreenSinal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QtScreenShare::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtScreenShare.data,
      qt_meta_data_QtScreenShare,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtScreenShare::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtScreenShare::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtScreenShare.stringdata))
        return static_cast<void*>(const_cast< QtScreenShare*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtScreenShare::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QtScreenShare::cvHostUnShareScreenSinal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
