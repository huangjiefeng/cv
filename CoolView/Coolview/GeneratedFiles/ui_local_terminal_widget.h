/********************************************************************************
** Form generated from reading UI file 'local_terminal_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCAL_TERMINAL_WIDGET_H
#define UI_LOCAL_TERMINAL_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LocalTerminalWidget
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *localScrollArea;
    QWidget *scrollAreaWidgetContents2;

    void setupUi(QWidget *LocalTerminalWidget)
    {
        if (LocalTerminalWidget->objectName().isEmpty())
            LocalTerminalWidget->setObjectName(QStringLiteral("LocalTerminalWidget"));
        LocalTerminalWidget->resize(813, 100);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LocalTerminalWidget->sizePolicy().hasHeightForWidth());
        LocalTerminalWidget->setSizePolicy(sizePolicy);
        LocalTerminalWidget->setMaximumSize(QSize(16777215, 100));
        LocalTerminalWidget->setAcceptDrops(true);
        verticalLayout = new QVBoxLayout(LocalTerminalWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        localScrollArea = new QScrollArea(LocalTerminalWidget);
        localScrollArea->setObjectName(QStringLiteral("localScrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(localScrollArea->sizePolicy().hasHeightForWidth());
        localScrollArea->setSizePolicy(sizePolicy1);
        localScrollArea->setMinimumSize(QSize(0, 100));
        localScrollArea->setAcceptDrops(true);
        localScrollArea->setFrameShape(QFrame::NoFrame);
        localScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents2 = new QWidget();
        scrollAreaWidgetContents2->setObjectName(QStringLiteral("scrollAreaWidgetContents2"));
        scrollAreaWidgetContents2->setGeometry(QRect(0, 0, 813, 100));
        localScrollArea->setWidget(scrollAreaWidgetContents2);

        verticalLayout->addWidget(localScrollArea);


        retranslateUi(LocalTerminalWidget);

        QMetaObject::connectSlotsByName(LocalTerminalWidget);
    } // setupUi

    void retranslateUi(QWidget *LocalTerminalWidget)
    {
        LocalTerminalWidget->setWindowTitle(QApplication::translate("LocalTerminalWidget", "LocalTerminalWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class LocalTerminalWidget: public Ui_LocalTerminalWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCAL_TERMINAL_WIDGET_H
