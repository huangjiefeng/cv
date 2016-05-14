/****************************************************************************
** Meta object code from reading C++ file 'Uploader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/util/Uploader/Uploader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Uploader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CUploader_t {
    QByteArrayData data[13];
    char stringdata[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUploader_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUploader_t qt_meta_stringdata_CUploader = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CUploader"
QT_MOC_LITERAL(1, 10, 18), // "UploadStateChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 22), // "CUploader::UploadState"
QT_MOC_LITERAL(4, 53, 5), // "state"
QT_MOC_LITERAL(5, 59, 3), // "msg"
QT_MOC_LITERAL(6, 63, 16), // "OnUploadFinished"
QT_MOC_LITERAL(7, 80, 13), // "OnUpLoadError"
QT_MOC_LITERAL(8, 94, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(9, 122, 9), // "errorCode"
QT_MOC_LITERAL(10, 132, 16), // "OnUploadProgress"
QT_MOC_LITERAL(11, 149, 9), // "bytesSent"
QT_MOC_LITERAL(12, 159, 10) // "bytesTotal"

    },
    "CUploader\0UploadStateChanged\0\0"
    "CUploader::UploadState\0state\0msg\0"
    "OnUploadFinished\0OnUpLoadError\0"
    "QNetworkReply::NetworkError\0errorCode\0"
    "OnUploadProgress\0bytesSent\0bytesTotal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUploader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   39,    2, 0x08 /* Private */,
       7,    1,   40,    2, 0x08 /* Private */,
      10,    2,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   11,   12,

       0        // eod
};

void CUploader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUploader *_t = static_cast<CUploader *>(_o);
        switch (_id) {
        case 0: _t->UploadStateChanged((*reinterpret_cast< CUploader::UploadState(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->OnUploadFinished(); break;
        case 2: _t->OnUpLoadError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: _t->OnUploadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CUploader::*_t)(CUploader::UploadState , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUploader::UploadStateChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CUploader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CUploader.data,
      qt_meta_data_CUploader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CUploader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUploader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CUploader.stringdata))
        return static_cast<void*>(const_cast< CUploader*>(this));
    return QObject::qt_metacast(_clname);
}

int CUploader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CUploader::UploadStateChanged(CUploader::UploadState _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
