/********************************************************************************
** Form generated from reading UI file 'display_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_WIDGET_H
#define UI_DISPLAY_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayWidget
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *screenScrollArea;
    QWidget *scrollAreaWidgetContents1;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *DisplayWidget)
    {
        if (DisplayWidget->objectName().isEmpty())
            DisplayWidget->setObjectName(QStringLiteral("DisplayWidget"));
        DisplayWidget->resize(917, 220);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DisplayWidget->sizePolicy().hasHeightForWidth());
        DisplayWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(DisplayWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        screenScrollArea = new QScrollArea(DisplayWidget);
        screenScrollArea->setObjectName(QStringLiteral("screenScrollArea"));
        screenScrollArea->setMinimumSize(QSize(0, 0));
        screenScrollArea->setAcceptDrops(false);
        screenScrollArea->setFrameShape(QFrame::NoFrame);
        screenScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents1 = new QWidget();
        scrollAreaWidgetContents1->setObjectName(QStringLiteral("scrollAreaWidgetContents1"));
        scrollAreaWidgetContents1->setGeometry(QRect(0, 0, 917, 220));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        screenScrollArea->setWidget(scrollAreaWidgetContents1);

        verticalLayout->addWidget(screenScrollArea);


        retranslateUi(DisplayWidget);

        QMetaObject::connectSlotsByName(DisplayWidget);
    } // setupUi

    void retranslateUi(QWidget *DisplayWidget)
    {
        DisplayWidget->setWindowTitle(QApplication::translate("DisplayWidget", "DisplayWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class DisplayWidget: public Ui_DisplayWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_WIDGET_H
