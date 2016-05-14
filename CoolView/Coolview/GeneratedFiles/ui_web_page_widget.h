/********************************************************************************
** Form generated from reading UI file 'web_page_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEB_PAGE_WIDGET_H
#define UI_WEB_PAGE_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WebPageWidget
{
public:

    void setupUi(QWidget *WebPageWidget)
    {
        if (WebPageWidget->objectName().isEmpty())
            WebPageWidget->setObjectName(QStringLiteral("WebPageWidget"));
        WebPageWidget->resize(713, 487);

        retranslateUi(WebPageWidget);

        QMetaObject::connectSlotsByName(WebPageWidget);
    } // setupUi

    void retranslateUi(QWidget *WebPageWidget)
    {
        WebPageWidget->setWindowTitle(QApplication::translate("WebPageWidget", "\344\274\232\350\256\256\347\256\241\347\220\206", 0));
    } // retranslateUi

};

namespace Ui {
    class WebPageWidget: public Ui_WebPageWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEB_PAGE_WIDGET_H
