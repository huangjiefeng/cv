/********************************************************************************
** Form generated from reading UI file 'seat_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEAT_WIDGET_H
#define UI_SEAT_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeatWidget
{
public:
    QAction *highQualityVideoAction;
    QAction *lowQualityVideoAction;
    QAction *stopReceiveAction;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QWidget *headWidget;
    QGridLayout *gridLayout;
    QLabel *headLabel;
    QFrame *vCheckFrame;
    QLabel *nameLabel;
    QFrame *aCheckFrame;
    QLabel *documentLabel;

    void setupUi(QWidget *SeatWidget)
    {
        if (SeatWidget->objectName().isEmpty())
            SeatWidget->setObjectName(QStringLiteral("SeatWidget"));
        SeatWidget->resize(240, 152);
        SeatWidget->setAcceptDrops(true);
        highQualityVideoAction = new QAction(SeatWidget);
        highQualityVideoAction->setObjectName(QStringLiteral("highQualityVideoAction"));
        highQualityVideoAction->setCheckable(true);
        lowQualityVideoAction = new QAction(SeatWidget);
        lowQualityVideoAction->setObjectName(QStringLiteral("lowQualityVideoAction"));
        lowQualityVideoAction->setCheckable(true);
        stopReceiveAction = new QAction(SeatWidget);
        stopReceiveAction->setObjectName(QStringLiteral("stopReceiveAction"));
        horizontalLayout = new QHBoxLayout(SeatWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(SeatWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setAcceptDrops(true);
        frame->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        headWidget = new QWidget(frame);
        headWidget->setObjectName(QStringLiteral("headWidget"));
        headWidget->setGeometry(QRect(70, 20, 61, 61));
        headWidget->setMaximumSize(QSize(16777215, 16777215));
        headWidget->setAcceptDrops(true);
        gridLayout = new QGridLayout(headWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        headLabel = new QLabel(headWidget);
        headLabel->setObjectName(QStringLiteral("headLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(headLabel->sizePolicy().hasHeightForWidth());
        headLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(headLabel, 0, 0, 1, 1);

        vCheckFrame = new QFrame(frame);
        vCheckFrame->setObjectName(QStringLiteral("vCheckFrame"));
        vCheckFrame->setGeometry(QRect(150, 20, 20, 20));
        vCheckFrame->setMinimumSize(QSize(0, 0));
        vCheckFrame->setMaximumSize(QSize(16777215, 16777215));
        vCheckFrame->setStyleSheet(QStringLiteral("image: url(:/image/stopvideo.png);"));
        nameLabel = new QLabel(frame);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));
        nameLabel->setGeometry(QRect(60, 96, 40, 40));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(6);
        font.setBold(true);
        font.setWeight(75);
        nameLabel->setFont(font);
        nameLabel->setWordWrap(true);
        aCheckFrame = new QFrame(frame);
        aCheckFrame->setObjectName(QStringLiteral("aCheckFrame"));
        aCheckFrame->setGeometry(QRect(180, 20, 20, 20));
        aCheckFrame->setMinimumSize(QSize(0, 0));
        aCheckFrame->setMaximumSize(QSize(16777215, 16777215));
        aCheckFrame->setStyleSheet(QStringLiteral("image: url(:/image/stopaudio.png);"));
        documentLabel = new QLabel(frame);
        documentLabel->setObjectName(QStringLiteral("documentLabel"));
        documentLabel->setGeometry(QRect(10, 10, 62, 17));

        horizontalLayout->addWidget(frame);


        retranslateUi(SeatWidget);

        QMetaObject::connectSlotsByName(SeatWidget);
    } // setupUi

    void retranslateUi(QWidget *SeatWidget)
    {
        SeatWidget->setWindowTitle(QApplication::translate("SeatWidget", "GridInScreenWidget", 0));
        highQualityVideoAction->setText(QApplication::translate("SeatWidget", "\351\253\230\346\270\205", 0));
#ifndef QT_NO_TOOLTIP
        highQualityVideoAction->setToolTip(QApplication::translate("SeatWidget", "\346\216\245\346\224\266\351\253\230\346\270\205\350\247\206\351\242\221", 0));
#endif // QT_NO_TOOLTIP
        lowQualityVideoAction->setText(QApplication::translate("SeatWidget", "\344\275\216\346\270\205", 0));
#ifndef QT_NO_TOOLTIP
        lowQualityVideoAction->setToolTip(QApplication::translate("SeatWidget", "\346\216\245\346\224\266\344\275\216\346\270\205\350\247\206\351\242\221", 0));
#endif // QT_NO_TOOLTIP
        stopReceiveAction->setText(QApplication::translate("SeatWidget", "\345\201\234\346\255\242\346\216\245\346\224\266", 0));
#ifndef QT_NO_TOOLTIP
        stopReceiveAction->setToolTip(QApplication::translate("SeatWidget", "\345\201\234\346\255\242\346\216\245\346\224\266", 0));
#endif // QT_NO_TOOLTIP
        headLabel->setText(QString());
        nameLabel->setText(QApplication::translate("SeatWidget", "TextLabel", 0));
        documentLabel->setText(QApplication::translate("SeatWidget", "\345\261\217\345\271\225\345\205\261\344\272\253", 0));
    } // retranslateUi

};

namespace Ui {
    class SeatWidget: public Ui_SeatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEAT_WIDGET_H
