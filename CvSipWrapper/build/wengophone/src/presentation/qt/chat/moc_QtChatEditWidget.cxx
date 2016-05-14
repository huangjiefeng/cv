/****************************************************************************
** Meta object code from reading C++ file 'QtChatEditWidget.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatEditWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatEditWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatEditWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QtChatEditWidget[] = {
    "QtChatEditWidget\0\0filename\0"
    "fileDragged(QString)\0"
};

const QMetaObject QtChatEditWidget::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_QtChatEditWidget,
      qt_meta_data_QtChatEditWidget, 0 }
};

const QMetaObject *QtChatEditWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatEditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatEditWidget))
        return static_cast<void*>(const_cast< QtChatEditWidget*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int QtChatEditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fileDragged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QtChatEditWidget::fileDragged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
