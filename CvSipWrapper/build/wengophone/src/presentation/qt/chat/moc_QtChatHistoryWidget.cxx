/****************************************************************************
** Meta object code from reading C++ file 'QtChatHistoryWidget.h'
**
** Created: Thu Jul 29 12:39:42 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../wengophone/src/presentation/qt/chat/QtChatHistoryWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtChatHistoryWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtChatHistoryWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x08,
      43,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtChatHistoryWidget[] = {
    "QtChatHistoryWidget\0\0link\0urlClicked(QUrl)\0"
    "scrollToBottom()\0"
};

const QMetaObject QtChatHistoryWidget::staticMetaObject = {
    { &QTextBrowser::staticMetaObject, qt_meta_stringdata_QtChatHistoryWidget,
      qt_meta_data_QtChatHistoryWidget, 0 }
};

const QMetaObject *QtChatHistoryWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QtChatHistoryWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtChatHistoryWidget))
        return static_cast<void*>(const_cast< QtChatHistoryWidget*>(this));
    return QTextBrowser::qt_metacast(_clname);
}

int QtChatHistoryWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextBrowser::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: urlClicked((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: scrollToBottom(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
