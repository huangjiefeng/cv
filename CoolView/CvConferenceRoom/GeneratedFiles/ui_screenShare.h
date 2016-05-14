/********************************************************************************
** Form generated from reading UI file 'screenShare.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENSHARE_H
#define UI_SCREENSHARE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenShare
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QToolButton *shareButton;

    void setupUi(QWidget *ScreenShare)
    {
        if (ScreenShare->objectName().isEmpty())
            ScreenShare->setObjectName(QStringLiteral("ScreenShare"));
        ScreenShare->resize(854, 565);
        vboxLayout = new QVBoxLayout(ScreenShare);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        frame = new QFrame(ScreenShare);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(176, 144));
        frame->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        scrollArea = new QScrollArea(frame);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 846, 535));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 2);

        shareButton = new QToolButton(frame);
        shareButton->setObjectName(QStringLiteral("shareButton"));

        gridLayout->addWidget(shareButton, 0, 0, 1, 1);


        vboxLayout->addWidget(frame);


        retranslateUi(ScreenShare);

        QMetaObject::connectSlotsByName(ScreenShare);
    } // setupUi

    void retranslateUi(QWidget *ScreenShare)
    {
        ScreenShare->setWindowTitle(QString());
        shareButton->setText(QApplication::translate("ScreenShare", "\345\205\261\344\272\253\345\261\217\345\271\225", 0));
    } // retranslateUi

};

namespace Ui {
    class ScreenShare: public Ui_ScreenShare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENSHARE_H
