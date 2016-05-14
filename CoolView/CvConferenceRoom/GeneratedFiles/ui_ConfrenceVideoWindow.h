/********************************************************************************
** Form generated from reading UI file 'ConfrenceVideoWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFRENCEVIDEOWINDOW_H
#define UI_CONFRENCEVIDEOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfrenceVideoWindow
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *topWidget;
    QHBoxLayout *topLayout;
    QFrame *videoFrame;

    void setupUi(QWidget *ConfrenceVideoWindow)
    {
        if (ConfrenceVideoWindow->objectName().isEmpty())
            ConfrenceVideoWindow->setObjectName(QStringLiteral("ConfrenceVideoWindow"));
        ConfrenceVideoWindow->resize(176, 144);
        verticalLayout = new QVBoxLayout(ConfrenceVideoWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        topWidget = new QWidget(ConfrenceVideoWindow);
        topWidget->setObjectName(QStringLiteral("topWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(topWidget->sizePolicy().hasHeightForWidth());
        topWidget->setSizePolicy(sizePolicy);
        topWidget->setMinimumSize(QSize(0, 16));
        topWidget->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 240);border-width:0px;"));
        topLayout = new QHBoxLayout(topWidget);
        topLayout->setObjectName(QStringLiteral("topLayout"));
        topLayout->setContentsMargins(6, 0, 0, 0);

        verticalLayout->addWidget(topWidget);

        videoFrame = new QFrame(ConfrenceVideoWindow);
        videoFrame->setObjectName(QStringLiteral("videoFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(videoFrame->sizePolicy().hasHeightForWidth());
        videoFrame->setSizePolicy(sizePolicy1);
        videoFrame->setMinimumSize(QSize(0, 0));
        videoFrame->setMaximumSize(QSize(16777215, 16777215));
        videoFrame->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        videoFrame->setFrameShape(QFrame::StyledPanel);
        videoFrame->setFrameShadow(QFrame::Raised);
        videoFrame->setLineWidth(1);

        verticalLayout->addWidget(videoFrame);


        retranslateUi(ConfrenceVideoWindow);

        QMetaObject::connectSlotsByName(ConfrenceVideoWindow);
    } // setupUi

    void retranslateUi(QWidget *ConfrenceVideoWindow)
    {
        ConfrenceVideoWindow->setWindowTitle(QApplication::translate("ConfrenceVideoWindow", "@product@ - Video", 0));
    } // retranslateUi

};

namespace Ui {
    class ConfrenceVideoWindow: public Ui_ConfrenceVideoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFRENCEVIDEOWINDOW_H
