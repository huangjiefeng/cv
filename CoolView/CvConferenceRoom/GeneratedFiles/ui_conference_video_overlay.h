/********************************************************************************
** Form generated from reading UI file 'conference_video_overlay.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFERENCE_VIDEO_OVERLAY_H
#define UI_CONFERENCE_VIDEO_OVERLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConferenceVideoOverlay
{
public:
    QVBoxLayout *mainLayout;
    QLabel *qosLabel;
    QHBoxLayout *middleLayout;
    QVBoxLayout *placeHolderLayout;
    QVBoxLayout *middleLayout_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *centerLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *videoStatusLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *indicatorLayout;
    QFrame *stopSendAudioFrame;
    QFrame *stopRecvAudioFrame;
    QSpacerItem *verticalSpacer_2;
    QLabel *userNameLabel;

    void setupUi(QWidget *ConferenceVideoOverlay)
    {
        if (ConferenceVideoOverlay->objectName().isEmpty())
            ConferenceVideoOverlay->setObjectName(QStringLiteral("ConferenceVideoOverlay"));
        ConferenceVideoOverlay->resize(400, 300);
        mainLayout = new QVBoxLayout(ConferenceVideoOverlay);
        mainLayout->setSpacing(6);
        mainLayout->setContentsMargins(11, 11, 11, 11);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        qosLabel = new QLabel(ConferenceVideoOverlay);
        qosLabel->setObjectName(QStringLiteral("qosLabel"));

        mainLayout->addWidget(qosLabel);

        middleLayout = new QHBoxLayout();
        middleLayout->setSpacing(6);
        middleLayout->setObjectName(QStringLiteral("middleLayout"));
        placeHolderLayout = new QVBoxLayout();
        placeHolderLayout->setSpacing(6);
        placeHolderLayout->setObjectName(QStringLiteral("placeHolderLayout"));
        placeHolderLayout->setContentsMargins(55, -1, 0, -1);

        middleLayout->addLayout(placeHolderLayout);

        middleLayout_2 = new QVBoxLayout();
        middleLayout_2->setSpacing(6);
        middleLayout_2->setObjectName(QStringLiteral("middleLayout_2"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        middleLayout_2->addItem(verticalSpacer_3);

        centerLayout = new QHBoxLayout();
        centerLayout->setSpacing(6);
        centerLayout->setObjectName(QStringLiteral("centerLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        centerLayout->addItem(horizontalSpacer_2);

        videoStatusLabel = new QLabel(ConferenceVideoOverlay);
        videoStatusLabel->setObjectName(QStringLiteral("videoStatusLabel"));
        videoStatusLabel->setAlignment(Qt::AlignCenter);
        videoStatusLabel->setMargin(10);

        centerLayout->addWidget(videoStatusLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        centerLayout->addItem(horizontalSpacer);


        middleLayout_2->addLayout(centerLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        middleLayout_2->addItem(verticalSpacer);


        middleLayout->addLayout(middleLayout_2);

        indicatorLayout = new QVBoxLayout();
        indicatorLayout->setSpacing(6);
        indicatorLayout->setObjectName(QStringLiteral("indicatorLayout"));
        indicatorLayout->setContentsMargins(10, 6, 10, 9);
        stopSendAudioFrame = new QFrame(ConferenceVideoOverlay);
        stopSendAudioFrame->setObjectName(QStringLiteral("stopSendAudioFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stopSendAudioFrame->sizePolicy().hasHeightForWidth());
        stopSendAudioFrame->setSizePolicy(sizePolicy);
        stopSendAudioFrame->setMinimumSize(QSize(35, 35));
        stopSendAudioFrame->setMaximumSize(QSize(100, 100));
        stopSendAudioFrame->setFrameShape(QFrame::StyledPanel);
        stopSendAudioFrame->setFrameShadow(QFrame::Raised);

        indicatorLayout->addWidget(stopSendAudioFrame);

        stopRecvAudioFrame = new QFrame(ConferenceVideoOverlay);
        stopRecvAudioFrame->setObjectName(QStringLiteral("stopRecvAudioFrame"));
        sizePolicy.setHeightForWidth(stopRecvAudioFrame->sizePolicy().hasHeightForWidth());
        stopRecvAudioFrame->setSizePolicy(sizePolicy);
        stopRecvAudioFrame->setMinimumSize(QSize(35, 35));
        stopRecvAudioFrame->setMaximumSize(QSize(100, 100));
        stopRecvAudioFrame->setFrameShape(QFrame::StyledPanel);
        stopRecvAudioFrame->setFrameShadow(QFrame::Raised);

        indicatorLayout->addWidget(stopRecvAudioFrame);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        indicatorLayout->addItem(verticalSpacer_2);


        middleLayout->addLayout(indicatorLayout);


        mainLayout->addLayout(middleLayout);

        userNameLabel = new QLabel(ConferenceVideoOverlay);
        userNameLabel->setObjectName(QStringLiteral("userNameLabel"));
        userNameLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(userNameLabel);


        retranslateUi(ConferenceVideoOverlay);

        QMetaObject::connectSlotsByName(ConferenceVideoOverlay);
    } // setupUi

    void retranslateUi(QWidget *ConferenceVideoOverlay)
    {
        ConferenceVideoOverlay->setWindowTitle(QApplication::translate("ConferenceVideoOverlay", "ConferenceVideoOverlay", 0));
        qosLabel->setText(QApplication::translate("ConferenceVideoOverlay", "QoS", 0));
        videoStatusLabel->setText(QApplication::translate("ConferenceVideoOverlay", "\346\227\240\350\247\206\351\242\221\344\277\241\345\217\267", 0));
        userNameLabel->setText(QApplication::translate("ConferenceVideoOverlay", "UserName", 0));
    } // retranslateUi

};

namespace Ui {
    class ConferenceVideoOverlay: public Ui_ConferenceVideoOverlay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFERENCE_VIDEO_OVERLAY_H
