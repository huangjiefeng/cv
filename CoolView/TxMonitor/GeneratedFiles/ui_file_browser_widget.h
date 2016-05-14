/********************************************************************************
** Form generated from reading UI file 'file_browser_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_BROWSER_WIDGET_H
#define UI_FILE_BROWSER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileBrowserWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *fileView;

    void setupUi(QWidget *FileBrowserWidget)
    {
        if (FileBrowserWidget->objectName().isEmpty())
            FileBrowserWidget->setObjectName(QStringLiteral("FileBrowserWidget"));
        FileBrowserWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(FileBrowserWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        fileView = new QTreeView(FileBrowserWidget);
        fileView->setObjectName(QStringLiteral("fileView"));

        verticalLayout->addWidget(fileView);


        retranslateUi(FileBrowserWidget);

        QMetaObject::connectSlotsByName(FileBrowserWidget);
    } // setupUi

    void retranslateUi(QWidget *FileBrowserWidget)
    {
        FileBrowserWidget->setWindowTitle(QApplication::translate("FileBrowserWidget", "FileBrowserWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class FileBrowserWidget: public Ui_FileBrowserWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_BROWSER_WIDGET_H
