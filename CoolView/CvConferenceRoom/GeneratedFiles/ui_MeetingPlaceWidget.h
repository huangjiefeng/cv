/********************************************************************************
** Form generated from reading UI file 'MeetingPlaceWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEETINGPLACEWIDGET_H
#define UI_MEETINGPLACEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeetingPlaceWidget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *MeetingPlaceWidget)
    {
        if (MeetingPlaceWidget->objectName().isEmpty())
            MeetingPlaceWidget->setObjectName(QStringLiteral("MeetingPlaceWidget"));
        MeetingPlaceWidget->resize(723, 500);
        gridLayout_2 = new QGridLayout(MeetingPlaceWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea = new QScrollArea(MeetingPlaceWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);


        retranslateUi(MeetingPlaceWidget);

        QMetaObject::connectSlotsByName(MeetingPlaceWidget);
    } // setupUi

    void retranslateUi(QWidget *MeetingPlaceWidget)
    {
        MeetingPlaceWidget->setWindowTitle(QApplication::translate("MeetingPlaceWidget", "@product@ - Video", 0));
#ifndef QT_NO_TOOLTIP
        MeetingPlaceWidget->setToolTip(QString());
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MeetingPlaceWidget: public Ui_MeetingPlaceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEETINGPLACEWIDGET_H
