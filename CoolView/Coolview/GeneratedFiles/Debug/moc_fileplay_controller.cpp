/****************************************************************************
** Meta object code from reading C++ file 'fileplay_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fileplay_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileplay_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FilePlayController_t {
    QByteArrayData data[17];
    char stringdata[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FilePlayController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FilePlayController_t qt_meta_stringdata_FilePlayController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "FilePlayController"
QT_MOC_LITERAL(1, 19, 21), // "HandleAddPlayListSlot"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 8), // "playlist"
QT_MOC_LITERAL(4, 51, 24), // "HandleClearPlayListsSlot"
QT_MOC_LITERAL(5, 76, 14), // "HandlePlaySlot"
QT_MOC_LITERAL(6, 91, 15), // "HandlePauseSlot"
QT_MOC_LITERAL(7, 107, 14), // "HandleStopSlot"
QT_MOC_LITERAL(8, 122, 14), // "HandleSeekSlot"
QT_MOC_LITERAL(9, 137, 3), // "sec"
QT_MOC_LITERAL(10, 141, 32), // "HandleConferenceStateChangedSlot"
QT_MOC_LITERAL(11, 174, 14), // "conference_uri"
QT_MOC_LITERAL(12, 189, 38), // "IConferenceController::Confer..."
QT_MOC_LITERAL(13, 228, 5), // "state"
QT_MOC_LITERAL(14, 234, 34), // "HandleReceiveUDPFilePlayRepor..."
QT_MOC_LITERAL(15, 269, 16), // "FilePlayStatItem"
QT_MOC_LITERAL(16, 286, 4) // "item"

    },
    "FilePlayController\0HandleAddPlayListSlot\0"
    "\0playlist\0HandleClearPlayListsSlot\0"
    "HandlePlaySlot\0HandlePauseSlot\0"
    "HandleStopSlot\0HandleSeekSlot\0sec\0"
    "HandleConferenceStateChangedSlot\0"
    "conference_uri\0IConferenceController::ConferenceState\0"
    "state\0HandleReceiveUDPFilePlayReportSlot\0"
    "FilePlayStatItem\0item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilePlayController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    0,   57,    2, 0x0a /* Public */,
       5,    0,   58,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    1,   61,    2, 0x0a /* Public */,
      10,    2,   64,    2, 0x0a /* Public */,
      14,    1,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULong,    9,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12,   11,   13,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void FilePlayController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FilePlayController *_t = static_cast<FilePlayController *>(_o);
        switch (_id) {
        case 0: _t->HandleAddPlayListSlot((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->HandleClearPlayListsSlot(); break;
        case 2: _t->HandlePlaySlot(); break;
        case 3: _t->HandlePauseSlot(); break;
        case 4: _t->HandleStopSlot(); break;
        case 5: _t->HandleSeekSlot((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 6: _t->HandleConferenceStateChangedSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IConferenceController::ConferenceState(*)>(_a[2]))); break;
        case 7: _t->HandleReceiveUDPFilePlayReportSlot((*reinterpret_cast< const FilePlayStatItem(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FilePlayController::staticMetaObject = {
    { &IFilePlayController::staticMetaObject, qt_meta_stringdata_FilePlayController.data,
      qt_meta_data_FilePlayController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FilePlayController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FilePlayController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FilePlayController.stringdata))
        return static_cast<void*>(const_cast< FilePlayController*>(this));
    return IFilePlayController::qt_metacast(_clname);
}

int FilePlayController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IFilePlayController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
