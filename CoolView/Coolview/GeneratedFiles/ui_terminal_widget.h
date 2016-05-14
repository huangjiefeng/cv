/********************************************************************************
** Form generated from reading UI file 'terminal_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERMINAL_WIDGET_H
#define UI_TERMINAL_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerminalWidget
{
public:
    QAction *stopReceiveAllVideoAction;
    QAction *permitSpeakAction;
    QAction *receiveAudioAction;
    QGridLayout *gridLayout_3;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QFrame *headFrame;
    QGridLayout *gridLayout;
    QPushButton *speakMarkButton;
    QPushButton *audioMarkButton;
    QPushButton *handUpMarkButton;
    QPushButton *videoMarkButton;
    QHBoxLayout *horizontalLayout_15;
    QLabel *memberNameLabel;
    QFrame *frame_2;
    QGridLayout *virtualGridLayout;

    void setupUi(QWidget *TerminalWidget)
    {
        if (TerminalWidget->objectName().isEmpty())
            TerminalWidget->setObjectName(QStringLiteral("TerminalWidget"));
        TerminalWidget->resize(160, 108);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TerminalWidget->sizePolicy().hasHeightForWidth());
        TerminalWidget->setSizePolicy(sizePolicy);
        TerminalWidget->setMinimumSize(QSize(160, 0));
        TerminalWidget->setMaximumSize(QSize(160, 16777215));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(0, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        TerminalWidget->setPalette(palette);
        stopReceiveAllVideoAction = new QAction(TerminalWidget);
        stopReceiveAllVideoAction->setObjectName(QStringLiteral("stopReceiveAllVideoAction"));
        permitSpeakAction = new QAction(TerminalWidget);
        permitSpeakAction->setObjectName(QStringLiteral("permitSpeakAction"));
        permitSpeakAction->setCheckable(true);
        receiveAudioAction = new QAction(TerminalWidget);
        receiveAudioAction->setObjectName(QStringLiteral("receiveAudioAction"));
        receiveAudioAction->setCheckable(true);
        gridLayout_3 = new QGridLayout(TerminalWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(TerminalWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(160, 51));
        frame->setMaximumSize(QSize(160, 51));
        frame->setStyleSheet(QLatin1String("QFrame {\n"
"  border-width:2px;\n"
"  border-color:darkGray;\n"
"  border-style:solid;\n"
"  border-radius: 5px\n"
"}"));
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(1, 1, 1, 1);
        headFrame = new QFrame(frame);
        headFrame->setObjectName(QStringLiteral("headFrame"));
        headFrame->setMinimumSize(QSize(32, 32));
        headFrame->setMaximumSize(QSize(32, 32));
        headFrame->setStyleSheet(QStringLiteral(""));
        headFrame->setFrameShape(QFrame::StyledPanel);
        headFrame->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(headFrame, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        speakMarkButton = new QPushButton(frame);
        speakMarkButton->setObjectName(QStringLiteral("speakMarkButton"));
        speakMarkButton->setMinimumSize(QSize(12, 12));
        speakMarkButton->setMaximumSize(QSize(12, 12));
        speakMarkButton->setStyleSheet(QStringLiteral("border-color:rbg(0,0,0,0);"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/speak.png"), QSize(), QIcon::Normal, QIcon::Off);
        speakMarkButton->setIcon(icon);
        speakMarkButton->setIconSize(QSize(12, 12));
        speakMarkButton->setFlat(true);

        gridLayout->addWidget(speakMarkButton, 0, 0, 1, 1);

        audioMarkButton = new QPushButton(frame);
        audioMarkButton->setObjectName(QStringLiteral("audioMarkButton"));
        audioMarkButton->setMinimumSize(QSize(16, 16));
        audioMarkButton->setMaximumSize(QSize(16, 16));
        audioMarkButton->setStyleSheet(QStringLiteral("border-color:rbg(0,0,0,0);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/mic.png"), QSize(), QIcon::Normal, QIcon::Off);
        audioMarkButton->setIcon(icon1);
        audioMarkButton->setIconSize(QSize(16, 16));
        audioMarkButton->setFlat(true);

        gridLayout->addWidget(audioMarkButton, 0, 1, 1, 1);

        handUpMarkButton = new QPushButton(frame);
        handUpMarkButton->setObjectName(QStringLiteral("handUpMarkButton"));
        handUpMarkButton->setMinimumSize(QSize(16, 16));
        handUpMarkButton->setMaximumSize(QSize(16, 16));
        handUpMarkButton->setStyleSheet(QStringLiteral("border-color:rbg(0,0,0,0);"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/hire-me.png"), QSize(), QIcon::Normal, QIcon::Off);
        handUpMarkButton->setIcon(icon2);
        handUpMarkButton->setIconSize(QSize(16, 16));
        handUpMarkButton->setFlat(true);

        gridLayout->addWidget(handUpMarkButton, 1, 0, 1, 1);

        videoMarkButton = new QPushButton(frame);
        videoMarkButton->setObjectName(QStringLiteral("videoMarkButton"));
        videoMarkButton->setMinimumSize(QSize(16, 16));
        videoMarkButton->setMaximumSize(QSize(16, 16));
        videoMarkButton->setStyleSheet(QStringLiteral("border-color:rbg(0,0,0,0);"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/camera-web.png"), QSize(), QIcon::Normal, QIcon::Off);
        videoMarkButton->setIcon(icon3);
        videoMarkButton->setIconSize(QSize(16, 16));
        videoMarkButton->setFlat(true);

        gridLayout->addWidget(videoMarkButton, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(0);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        memberNameLabel = new QLabel(frame);
        memberNameLabel->setObjectName(QStringLiteral("memberNameLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(memberNameLabel->sizePolicy().hasHeightForWidth());
        memberNameLabel->setSizePolicy(sizePolicy1);
        memberNameLabel->setMinimumSize(QSize(80, 20));
        memberNameLabel->setMaximumSize(QSize(80, 40));
        QFont font;
        font.setKerning(true);
        memberNameLabel->setFont(font);
        memberNameLabel->setStyleSheet(QStringLiteral("border-color:rbg(0,0,0,0);"));
        memberNameLabel->setAlignment(Qt::AlignCenter);
        memberNameLabel->setWordWrap(true);

        horizontalLayout_15->addWidget(memberNameLabel);


        gridLayout_2->addLayout(horizontalLayout_15, 0, 2, 1, 1);


        gridLayout_3->addWidget(frame, 0, 0, 1, 1);

        frame_2 = new QFrame(TerminalWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setMinimumSize(QSize(160, 51));
        frame_2->setMaximumSize(QSize(160, 51));
        frame_2->setStyleSheet(QLatin1String("QFrame {\n"
"border-width:2px;\n"
"border-color:darkGray;\n"
"border-style:solid;\n"
"border-radius: 5px\n"
"}"));
        virtualGridLayout = new QGridLayout(frame_2);
        virtualGridLayout->setSpacing(0);
        virtualGridLayout->setContentsMargins(11, 11, 11, 11);
        virtualGridLayout->setObjectName(QStringLiteral("virtualGridLayout"));
        virtualGridLayout->setContentsMargins(3, 0, 3, 0);

        gridLayout_3->addWidget(frame_2, 1, 0, 1, 1);


        retranslateUi(TerminalWidget);

        QMetaObject::connectSlotsByName(TerminalWidget);
    } // setupUi

    void retranslateUi(QWidget *TerminalWidget)
    {
        TerminalWidget->setWindowTitle(QApplication::translate("TerminalWidget", "TerminalWidget", 0));
        stopReceiveAllVideoAction->setText(QApplication::translate("TerminalWidget", "\345\201\234\346\255\242\346\216\245\346\224\266\346\211\200\346\234\211\350\247\206\351\242\221", 0));
#ifndef QT_NO_TOOLTIP
        stopReceiveAllVideoAction->setToolTip(QApplication::translate("TerminalWidget", "\345\201\234\346\255\242\346\216\245\346\224\266\346\211\200\346\234\211\350\247\206\351\242\221", 0));
#endif // QT_NO_TOOLTIP
        permitSpeakAction->setText(QApplication::translate("TerminalWidget", "\345\205\201\350\256\270\345\217\221\350\250\200", 0));
#ifndef QT_NO_TOOLTIP
        permitSpeakAction->setToolTip(QApplication::translate("TerminalWidget", "\345\205\201\350\256\270/\347\246\201\346\255\242\345\217\221\350\250\200", 0));
#endif // QT_NO_TOOLTIP
        receiveAudioAction->setText(QApplication::translate("TerminalWidget", "\346\216\245\346\224\266\351\237\263\351\242\221", 0));
#ifndef QT_NO_TOOLTIP
        receiveAudioAction->setToolTip(QApplication::translate("TerminalWidget", "\346\216\245\346\224\266/\345\201\234\346\255\242\346\216\245\346\224\266\351\237\263\351\242\221", 0));
#endif // QT_NO_TOOLTIP
        speakMarkButton->setText(QString());
        audioMarkButton->setText(QString());
        handUpMarkButton->setText(QString());
        videoMarkButton->setText(QString());
        memberNameLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TerminalWidget: public Ui_TerminalWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERMINAL_WIDGET_H
