/****************************************************************************
** Meta object code from reading C++ file 'ImageSelector.h'
**
** Created: Mon Apr 20 22:16:00 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../libs/qtutil/include/qtutil/ImageSelector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImageSelector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageSelector[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      26,   15,   14,   14, 0x0a,
      49,   14,   14,   14, 0x08,
      56,   14,   14,   14, 0x08,
      66,   14,   14,   14, 0x08,
      91,   83,   14,   14, 0x08,
     135,   83,   14,   14, 0x08,
     182,  176,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageSelector[] = {
    "ImageSelector\0\0currentDir\0"
    "setCurrentDir(QString)\0goUp()\0refresh()\0"
    "updateOkButton()\0current\0"
    "slotThumbnailListViewActivated(QModelIndex)\0"
    "slotStartDirListViewChanged(QModelIndex)\0"
    "index\0slotDirComboBoxActivated(int)\0"
};

const QMetaObject ImageSelector::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ImageSelector,
      qt_meta_data_ImageSelector, 0 }
};

const QMetaObject *ImageSelector::metaObject() const
{
    return &staticMetaObject;
}

void *ImageSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageSelector))
        return static_cast<void*>(const_cast< ImageSelector*>(this));
    return QDialog::qt_metacast(_clname);
}

int ImageSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setCurrentDir((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: goUp(); break;
        case 2: refresh(); break;
        case 3: updateOkButton(); break;
        case 4: slotThumbnailListViewActivated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: slotStartDirListViewChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: slotDirComboBoxActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
